#include "cross_fading_lod.h"

#include <scene/3d/camera.h>
#include <scene/main/scene_tree.h>
#include <scene/main/viewport.h>

#include <cassert>
#include <utility>

#include "entity_node.h"

template <std::size_t... Is>
constexpr auto indexSequenceReverse(std::index_sequence<Is...> const &)
		-> decltype(std::index_sequence<sizeof...(Is) - 1U - Is...>{});

template <std::size_t N>
using makeIndexSequenceReverse = decltype(indexSequenceReverse(std::make_index_sequence<N>{}));

namespace detail {
template <class F, size_t... I>
constexpr void for_constexpr_impl(F &&func, std::index_sequence<I...>) {
	(func(std::integral_constant<std::size_t, I>{}), ...);
}
} // namespace detail

enum class Order : uint8_t {
	AscendingOrder,
	DescendingOrder,
};

template <size_t C, Order order = Order::AscendingOrder, class F>
constexpr void for_constexpr(F &&func) {

	if constexpr (order == Order::AscendingOrder) {
		detail::for_constexpr_impl(std::forward<F>(func), std::make_index_sequence<C>{});
	} else {
		detail::for_constexpr_impl(std::forward<F>(func), makeIndexSequenceReverse<C>{});
	}
}

CrossFadingLod::CrossFadingLod() {
}

void CrossFadingLod::_notification(int p_what) {
	switch (p_what) {

		case NOTIFICATION_ENTER_TREE:
			set_process(true);
			break;

		case NOTIFICATION_ENTER_WORLD:
			break;

		case NOTIFICATION_EXIT_WORLD:
			break;

		case NOTIFICATION_TRANSFORM_CHANGED:
			break;

		case NOTIFICATION_VISIBILITY_CHANGED:
			break;

		case NOTIFICATION_PROCESS:
			_process();
			break;
	}
}

static constexpr std::array<std::pair<uint8_t, uint8_t>, 8> combination_fade_components{
	std::pair{ 0, 1 },
	std::pair{ 1, 2 },
	std::pair{ 2, 3 },
	std::pair{ 3, 4 },
	std::pair{ 4, 3 },
	std::pair{ 3, 2 },
	std::pair{ 2, 1 },
	std::pair{ 1, 0 }
};

template <uint8_t Start = 0, uint8_t End = 5>
void static_iterate(const real_t &distance, const std::array<uint16_t, 5> &arr, entt::registry &registry, const entt::entity &entity) {
	if constexpr (Start < End) {

		const auto &size = arr[Start];

		if (distance < size) {

			constexpr auto len = combination_fade_components.size();

			for_constexpr<len>([&](auto index) {
				constexpr auto pair = combination_fade_components[index];

				if constexpr (pair.first == Start) {
					if (registry.has<Lod<pair.second>>(entity)) {
						using FadeComp = FadeComponent<pair.first, pair.second>;
						if (!registry.has<FadeComp>(entity)) {
							registry.emplace<FadeComp>(entity);
							registry.emplace_or_replace<Lod<pair.first>>(entity);
						}
					} else if (!registry.has<Lod<pair.first>>(entity)) {
						registry.emplace<Lod<pair.first>>(entity);
					}
				}
			});

			return;
		}

		static_iterate<Start + 1, End>(distance, arr, registry, entity);
	}
}

void CrossFadingLod::replace_nodes() {
	if (!is_inside_tree()) {
		return;
	}

	{
		m_material = Ref<ShaderMaterial>(memnew(ShaderMaterial));
		String path{ "res://shaders/terr.shader" };

		Ref<Shader> shader = ResourceLoader::load(path, "");
		if (shader.is_null()) {
			assert(0);
		}
		m_material->set_shader(shader);
	}

	m_registry.prepare<Transform>();
	m_registry.prepare<DistanceLod>();

	SceneTree *tree = get_tree();
	if (tree == nullptr) {
		return;
	}

	Node *current_scene = tree->get_edited_scene_root();
	if (current_scene == nullptr) {
		return;
	}

	if (current_scene->get_child_count() > 0) {

		for (int i = 0; i < current_scene->get_child_count(); ++i) {
			EntityNode *node = Object::cast_to<EntityNode>(current_scene->get_child(i));
			if (node != nullptr && !node->m_meshes.empty()) {

				const auto entity = m_registry.create();
				Transform transform = node->get_global_transform();
				m_registry.emplace<Transform>(entity, transform);

				m_registry.emplace<DistanceLod>(entity, node->m_distance_lod);

				for (size_t mesh_idx = 0; mesh_idx < node->m_meshes.size(); ++mesh_idx) {
					Ref<ArrayMesh> &mesh = node->m_meshes[mesh_idx];

					DirectMultiMeshInstance &chunk = m_mesh_cache[mesh_idx];
					chunk.set_mesh(mesh);
					chunk.set_scenario(get_world());
				}

				//current_scene->remove_child(node);
			}
		}
	}

	print_line(String{ "Created entity = " } + String::num_uint64(m_registry.size<Transform>()));
}

void CrossFadingLod::set_lod_by_distance(const Vector3 &viewer_pos) {

	auto view = m_registry.view<Transform, DistanceLod>();

	view.each([&](const auto entity, const Transform &transform, struct DistanceLod &distance_lod) {
		//Vector2 camera{ viewer_pos.x, viewer_pos.z };
		//Vector2 pos{ transform.origin.x, transform.origin.z };
		const real_t distance = viewer_pos.distance_to(transform.origin);

		print_line(String{ "distance = " } + String::num(distance));

		static_iterate(distance, distance_lod.m_value, m_registry, entity);
	});
}

void CrossFadingLod::update_fade() {

	constexpr auto len = combination_fade_components.size();

	for_constexpr<len>([&](auto index) {
		constexpr auto pair = combination_fade_components[index];

		auto view_fading = m_registry.view<FadeComponent<pair.first, pair.second>>();

		for (decltype(view_fading.size()) i{ 0 }; i < view_fading.size(); ++i) {
			const auto entity = view_fading[i];

			auto &fade = view_fading.get(entity);

			if (fade.m_incr_fade != 1.f) {
				fade.m_incr_fade += step_fading;
				fade.m_decr_fade -= step_fading;

				print_line(String{ "Fade of incr_lod = " } + String::num_real(fade.m_incr_fade));
			} else {
				m_registry.remove_if_exists<Lod<pair.second>, FadeComponent<pair.first, pair.second>>(entity);
			}
		}
	});
}

void CrossFadingLod::set_mesh_by_lod() {

	for_constexpr<5>([&](auto index) {
		DirectMultiMeshInstance &multimesh_instance = m_mesh_cache[index];

		auto &group = std::get<LevelLodGroup<index>>(groups).m_value;
		auto instance_count = group.size();
		multimesh_instance.set_instance_count(instance_count);

		for (decltype(instance_count) i{ 0 }; i < instance_count; ++i) {
			const auto entity = group[i];

			auto &transform = group.template get<Transform>(entity);
			multimesh_instance.set_instance_transform(i, transform);

			constexpr auto len = combination_fade_components.size();
			for_constexpr<len>([&](auto idx) {
				constexpr auto pair = combination_fade_components[idx];

				if constexpr (pair.first == index || pair.second == index) {
					using FadeComp = FadeComponent<pair.first, pair.second>;
					if (m_registry.has<FadeComp>(entity)) {
						auto &fade = m_registry.get<FadeComp>(entity);

						if constexpr (pair.first == index) {
							multimesh_instance.set_lod_and_fade(i, index, fade.m_incr_fade);
						} else {
							multimesh_instance.set_lod_and_fade(i, index, fade.m_decr_fade);
						}
					}
				}
			});
		}
	});
}

void CrossFadingLod::_process() {
	// Get viewer pos
	Vector3 viewer_pos = m_manual_viewer_pos;
	Viewport *viewport = get_viewport();
	if (viewport != nullptr) {
		Camera *camera = viewport->get_camera();
		if (camera != nullptr) {
			viewer_pos = camera->get_global_transform().origin;
		}
	}

	update_fade();

	set_lod_by_distance(viewer_pos);

	set_mesh_by_lod();
}
