#include "entity_node.h"

EntityNode::EntityNode()
{

}

void EntityNode::set_meshes(const Array &p_meshes) {

	if (p_meshes.size() > m_meshes.size()) {
		return;
	}

	for (int i = 0; i < p_meshes.size(); ++i) {
		m_meshes[i] = p_meshes[i];
	}
}

void EntityNode::set_distance_lod(const Array &p_distance_lod) {

	if (p_distance_lod.size() > m_distance_lod.size()) {
		return;
	}

	for (int i = 0; i < p_distance_lod.size(); ++i) {
		m_distance_lod[i] = p_distance_lod[i];
	}
}

void EntityNode::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_meshes", "p_meshes"), &EntityNode::set_meshes);
	ClassDB::bind_method(D_METHOD("get_meshes"), &EntityNode::get_meshes);

	ClassDB::bind_method(D_METHOD("set_distance_lod", "p_distance_lod"), &EntityNode::set_distance_lod);
	ClassDB::bind_method(D_METHOD("get_distance_lod"), &EntityNode::get_distance_lod);

	ClassDB::add_property(get_class_static(),
			PropertyInfo(Variant::ARRAY, "meshes"),
			_scs_create("set_meshes"),
			_scs_create("get_meshes"));

	ClassDB::add_property(get_class_static(),
			PropertyInfo(Variant::ARRAY, "distance_lod"),
			_scs_create("set_distance_lod"),
			_scs_create("get_distance_lod"));
}
