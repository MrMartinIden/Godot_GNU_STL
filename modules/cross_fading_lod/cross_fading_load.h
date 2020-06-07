#pragma once

#include <scene/3d/spatial.h>

class CrossFadingLoad : public Spatial {
	GDCLASS(CrossFadingLoad, Spatial)
public:
	CrossFadingLoad();

	virtual ~CrossFadingLoad() {}

	void _notification(int p_what);
	void _process();

	Vector3 m_manual_viewer_pos;
};
