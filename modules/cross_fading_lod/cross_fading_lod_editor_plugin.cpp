#include <core/os/input.h>
#include <scene/3d/camera.h>
#include <scene/scene_string_names.h>
#include <scene/gui/color_rect.h>

#include "cross_fading_lod_editor_plugin.h"
#include "editor/editor_scale.h"

CrossFadingLodEditorPlugin::CrossFadingLodEditorPlugin(EditorNode *p_editor) {
	_editor = p_editor;
	_mouse_pressed = false;

	_toolbar = memnew(HBoxContainer);
	add_control_to_container(CONTAINER_SPATIAL_EDITOR_MENU, _toolbar);
	_toolbar->hide();

	{
		Button *button = memnew(Button);
		button->set_text(TTR("Replace nodes"));
		button->connect("pressed", this, "replace_nodes");
		_toolbar->add_child(button);
	}
}

CrossFadingLodEditorPlugin::~CrossFadingLodEditorPlugin() {
}

void CrossFadingLodEditorPlugin::replace_nodes() {
	cross_fading_lod->replace_nodes();
}

bool CrossFadingLodEditorPlugin::forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event) {

	if (cross_fading_lod == nullptr)
		return false;

	cross_fading_lod->m_manual_viewer_pos = p_camera->get_global_transform().origin;

	bool captured_event = false;

	return captured_event;
}

void CrossFadingLodEditorPlugin::edit(Object *p_object) {

	//printf("Edit %i\n", p_object);
	CrossFadingLod *node = p_object ? Object::cast_to<CrossFadingLod>(p_object) : nullptr;

	if (cross_fading_lod) {
		cross_fading_lod->disconnect(SceneStringNames::get_singleton()->tree_exited, this, "cross_fading_lod_exited_scene");
	}

	cross_fading_lod = node;

	if (cross_fading_lod) {
		cross_fading_lod->connect(SceneStringNames::get_singleton()->tree_exited, this, "cross_fading_lod_exited_scene");
	}
}

void CrossFadingLodEditorPlugin::cross_fading_lod_exited_scene() {
	//print_line("HeightMap exited scene");
	edit(NULL);
}

bool CrossFadingLodEditorPlugin::handles(Object *p_object) const {
	return p_object->is_class("CrossFadingLod");
}

void CrossFadingLodEditorPlugin::make_visible(bool p_visible) {
	_toolbar->set_visible(p_visible);
}

void CrossFadingLodEditorPlugin::_bind_methods() {

	ClassDB::bind_method(D_METHOD("cross_fading_lod_exited_scene"), &CrossFadingLodEditorPlugin::cross_fading_lod_exited_scene);
	ClassDB::bind_method(D_METHOD("replace_nodes"), &CrossFadingLodEditorPlugin::replace_nodes);
}
