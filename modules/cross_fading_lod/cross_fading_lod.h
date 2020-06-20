#pragma once

#include <scene/3d/spatial.h>

#include <scene/3d/mesh_instance.h>

#include "direct_multimesh_instance.h"

#include "entt/src/entt/entity/registry.hpp"

static constexpr float step_fading = 1.f / 16;

template <uint8_t IncrLod, uint8_t DecrLod>
struct FadeComponent {

	float m_incr_fade = 0.f;
	float m_decr_fade = 1.f;

	static constexpr uint8_t m_incr_lod = IncrLod;
	static constexpr uint8_t m_decr_lod = DecrLod;
};

template <uint8_t Index>
struct Lod {
	static constexpr auto value = Index;
};

class CrossFadingLod : public Spatial {
	GDCLASS(CrossFadingLod, Spatial)
public:
	CrossFadingLod();

	virtual ~CrossFadingLod() {
	}

	void _notification(int p_what);
	void _process();

	void update_fade();

	struct DistanceLod {
		std::array<uint16_t, 5> m_value;
	};

	void set_lod_by_distance(const Vector3 &viewer_pos);
	void set_mesh_by_lod();

	void replace_nodes();

	entt::registry m_registry;

	template <uint8_t Index>
	struct LevelLodGroup {

		LevelLodGroup(entt::registry &reg) :
				m_value(reg.group<Lod<Index>>(entt::get<Transform>)) {}

		entt::basic_group<entt::entity, entt::exclude_t<>, entt::get_t<Transform>, Lod<Index>>
				m_value;
	};

	std::tuple<LevelLodGroup<0>, LevelLodGroup<1>, LevelLodGroup<2>, LevelLodGroup<3>, LevelLodGroup<4>> groups{
		m_registry, m_registry, m_registry, m_registry, m_registry
	};

	std::array<DirectMultiMeshInstance, 5> m_mesh_cache;

	static void _bind_methods();

	void set_custom_material(Ref<ShaderMaterial> p_material);
	inline Ref<ShaderMaterial> get_custom_material() const { return m_custom_material; }

	Ref<ShaderMaterial> m_custom_material;

	Vector3 m_manual_viewer_pos;
};
