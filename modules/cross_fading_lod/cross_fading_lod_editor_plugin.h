#pragma once

#include "editor/editor_plugin.h"

#include "editor/editor_node.h"

#include "editor/editor_resource_preview.h"

#include "cross_fading_lod.h"

class CrossFadingLodEditorPlugin : public EditorPlugin {
	GDCLASS(CrossFadingLodEditorPlugin, EditorPlugin)
public:
	CrossFadingLodEditorPlugin(EditorNode *p_editor);
	~CrossFadingLodEditorPlugin();

	virtual bool forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event);
	virtual String get_name() const { return "CrossFadingLoad"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

	void replace_nodes();

protected:
	static void _bind_methods();

private:
	void cross_fading_lod_exited_scene();

private:
	EditorNode *_editor;
	CrossFadingLod *cross_fading_lod;
	HBoxContainer *_toolbar;
};
