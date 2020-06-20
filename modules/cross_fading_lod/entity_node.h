#pragma once

#include <scene/3d/spatial.h>

class EntityNode : public Spatial {
	GDCLASS(EntityNode, Spatial)
public:
	EntityNode();
	virtual ~EntityNode() {}

	static void _bind_methods();

	void set_meshes(const Array &p_polygons);

	Array get_meshes() {
		Array ret;
		for (auto &&mesh : m_meshes) {
			ret.push_back(mesh);
		}

		return ret;
	}

	void set_distance_lod(const Array &p_distance_lod);

	Array get_distance_lod() {
		Array ret;
		for (auto &&distance : m_distance_lod) {
			ret.push_back(distance);
		}

		return ret;
	}

	static constexpr uint8_t max_lod = 5;

	std::array<Ref<ArrayMesh>, max_lod> m_meshes;

	std::array<uint16_t, max_lod> m_distance_lod{ 10, 20, 30, 40, 50 };
};
