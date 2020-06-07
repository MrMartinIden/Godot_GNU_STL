#pragma once

#include "editor/editor_plugin.h"

#include "editor/editor_node.h"

#include "editor/editor_resource_preview.h"

#include "cross_fading_load.h"

class CrossFadingLodEditorPlugin : public EditorPlugin {
	GDCLASS(CrossFadingLodEditorPlugin, EditorPlugin)
public:
	CrossFadingLodEditorPlugin(EditorNode *p_editor);
	~CrossFadingLodEditorPlugin();

	virtual bool forward_spatial_gui_input(Camera *p_camera, const Ref<InputEvent> &p_event);
	virtual String get_name() const { return "CrossFadingLod"; }
	bool has_main_screen() const { return false; }
	virtual void edit(Object *p_object);
	virtual bool handles(Object *p_object) const;
	virtual void make_visible(bool p_visible);

protected:
	static void _bind_methods();

private:
	void cross_fading_load_exited_scene();

private:
	EditorNode *_editor;
	CrossFadingLoad *cross_fading_load;
	HBoxContainer *_toolbar;
    HBoxContainer m_toolbar;

    FileDialog m_import_dialog;
	String _import_file_path;
	ConfirmationDialog *_import_confirmation_dialog;
	AcceptDialog *_accept_dialog;

	bool _mouse_pressed;
};
