#include "direct_multimesh_instance.h"

#include <cassert>

void DirectMultiMeshInstance::set_mesh(Ref<Mesh> mesh) {
	if (!m_multimesh.is_valid()) {
		assert(0);
	}

	VS::get_singleton()->multimesh_set_mesh(m_multimesh, mesh.is_valid() ? mesh->get_rid() : RID());
	m_mesh = mesh;
}

void DirectMultiMeshInstance::set_instance_count(uint32_t p_count) {

    if(!m_multimesh.is_valid())
    {
        assert(0);
    }

    if(m_instance_count == p_count)
    {
        return;
    }

    VisualServer::get_singleton()->multimesh_allocate(m_multimesh,
                                                      p_count,
                                                      VS::MultimeshTransformFormat::MULTIMESH_TRANSFORM_3D,
                                                      VS::MultimeshColorFormat::MULTIMESH_COLOR_FLOAT,
                                                      VS::MultimeshCustomDataFormat::MULTIMESH_CUSTOM_DATA_FLOAT);
    m_instance_count = p_count;
}

void DirectMultiMeshInstance::set_scenario(Ref<World> world) {
	if (!m_multimesh_instance.is_valid()) {
		assert(0);
	}

	VS::get_singleton()->instance_set_scenario(m_multimesh_instance, world->get_scenario());
}

void DirectMultiMeshInstance::set_instance_transform(uint32_t p_instance, const Transform &p_transform) {

    if(!m_multimesh.is_valid())
    {
        assert(0);
    }

    VS::get_singleton()->multimesh_instance_set_transform(m_multimesh, p_instance, p_transform);
}

void DirectMultiMeshInstance::set_material(Ref<Material> material) {
    if(!m_multimesh_instance.is_valid())
    {
        assert(0);
    }
    VS::get_singleton()->instance_geometry_set_material_override(m_multimesh_instance, material.is_valid() ? material->get_rid() : RID());
}

void DirectMultiMeshInstance::set_lod_and_fade(uint32_t p_instance, const uint8_t &lod, const float &p_fade) {
	Color color{ 2.f / (1 << lod), p_fade + 1.f, 0 };

	set_instance_custom_data(p_instance, color);
}

void DirectMultiMeshInstance::set_instance_custom_data(uint32_t p_instance, const Color &p_custom_data) {
    if(!m_multimesh.is_valid())
    {
      assert(0);
    }

    VisualServer::get_singleton()->multimesh_instance_set_custom_data(m_multimesh, p_instance, p_custom_data);
}
