#pragma once

#include <scene/3d/multimesh_instance.h>

struct DirectMultiMeshInstance
{
    DirectMultiMeshInstance() :
          m_multimesh_instance(VS::get_singleton()->instance_create()),
          m_multimesh(VS::get_singleton()->multimesh_create())
    {
        VS::get_singleton()->instance_set_base(m_multimesh_instance, m_multimesh);

    }

	void set_instance_count(uint32_t p_count);
	inline uint32_t get_instance_count() const {
		return m_instance_count;
	}

	void set_scenario(Ref<World> world);

	void set_instance_transform(uint32_t p_instance, const Transform &p_transform);

	void set_mesh(Ref<Mesh> mesh);

	void set_material(Ref<Material> material);

	void set_lod_and_fade(uint32_t p_instance, const uint8_t &lod, const float &p_fade);

	void set_instance_custom_data(uint32_t p_instance, const Color &p_custom_data);

	RID m_multimesh_instance;
	RID m_multimesh;
	Ref<Mesh> m_mesh;
	uint32_t m_instance_count;
};
