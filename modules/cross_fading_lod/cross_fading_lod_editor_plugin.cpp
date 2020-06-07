#include <core/os/input.h>
#include <scene/3d/camera.h>
#include <scene/scene_string_names.h>
#include <scene/gui/color_rect.h>

#include "cross_fading_lod_editor_plugin.h"
#include "editor/editor_scale.h"

inline Ref<Texture> get_icon(String name) {
	return EditorNode::get_singleton()->get_gui_base()->get_icon(name, "EditorIcons");
}

CrossFadingLodEditorPlugin::CrossFadingLodEditorPlugin(EditorNode *p_editor) {
	_editor = p_editor;
	_mouse_pressed = false;
}

CrossFadingLodEditorPlugin::~CrossFadingLodEditorPlugin() {
}

bool CrossFadingLodEditorPlugin::forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event) {

	if (cross_fading_load == nullptr)
		return false;

	cross_fading_load->m_manual_viewer_pos = p_camera->get_global_transform().origin;

	bool captured_event = false;

	return captured_event;
}

void CrossFadingLodEditorPlugin::edit(Object *p_object) {

	//printf("Edit %i\n", p_object);
	CrossFadingLoad *node = p_object ? Object::cast_to<CrossFadingLoad>(p_object) : nullptr;

	if (cross_fading_load) {
		cross_fading_load->disconnect(SceneStringNames::get_singleton()->tree_exited, this, "cross_fading_load_exited_scene");
	}

	cross_fading_load = node;

	if (cross_fading_load) {
		cross_fading_load->connect(SceneStringNames::get_singleton()->tree_exited, this, "cross_fading_load_exited_scene");
	}
}

void CrossFadingLodEditorPlugin::cross_fading_load_exited_scene() {
	//print_line("HeightMap exited scene");
	edit(NULL);
}

bool CrossFadingLodEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("HeightMap");
}

void CrossFadingLodEditorPlugin::make_visible(bool p_visible) {
	//_toolbar->set_visible(p_visible);
	m_toolbar.set_visible(p_visible);
}

static Point2i get_size_from_raw_length(int len) {
	size_t side_len = static_cast<int>(Math::round(Math::sqrt(static_cast<float>(len/2))));
	Point2i size(side_len, side_len);
	return size;
}

void CrossFadingLodEditorPlugin::_bind_methods() {

	ClassDB::bind_method(D_METHOD("cross_fading_load_exited_scene"), &CrossFadingLodEditorPlugin::cross_fading_load_exited_scene);
}
