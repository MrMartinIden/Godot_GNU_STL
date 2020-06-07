#include "cross_fading_load.h"

#include <scene/3d/camera.h>
#include <scene/main/viewport.h>

CrossFadingLoad::CrossFadingLoad()
{

}

void CrossFadingLoad::_notification(int p_what) {
	switch (p_what) {

		case NOTIFICATION_ENTER_TREE:
			set_process(true);
			break;

		case NOTIFICATION_ENTER_WORLD:
			break;

		case NOTIFICATION_EXIT_WORLD:
			break;

		case NOTIFICATION_TRANSFORM_CHANGED:
			break;

		case NOTIFICATION_VISIBILITY_CHANGED:
			break;

		case NOTIFICATION_PROCESS:
			_process();
			break;
	}
}

void CrossFadingLoad::_process() {
	// Get viewer pos
	Vector3 viewer_pos = m_manual_viewer_pos;
	Viewport *viewport = get_viewport();
	if (viewport != nullptr) {
		Camera *camera = viewport->get_camera();
		if (camera != nullptr) {
			viewer_pos = camera->get_global_transform().origin;
		}
	}
}
