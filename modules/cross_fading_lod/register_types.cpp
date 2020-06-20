#include "register_types.h"

#include "cross_fading_lod_editor_plugin.h"
#include "entity_node.h"

void register_cross_fading_lod_types() {
#ifndef _3D_DISABLED
	ClassDB::register_class<CrossFadingLod>();
	ClassDB::register_class<EntityNode>();

#ifdef TOOLS_ENABLED
	EditorPlugins::add_by_type<CrossFadingLodEditorPlugin>();
#endif
#endif
}

void unregister_cross_fading_lod_types() {
}
