/*************************************************************************/
/*  skin.h                                                               */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2019 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2019 Godot Engine contributors (cf. AUTHORS.md)    */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#ifndef SKIN_H
#define SKIN_H

#include <vector>

#include "core/resource.h"

class Skin : public Resource {
	GDCLASS(Skin, Resource)

	struct Bind {
		int bone;
		Transform pose;
	};

	std::vector<Bind> binds;

protected:
	bool _set(const StringName &p_name, const Variant &p_value);
	bool _get(const StringName &p_name, Variant &r_ret) const;
	void _get_property_list(List<PropertyInfo> *p_list) const;

	static void _bind_methods();

public:
	void set_bind_count(int p_size);
	inline int get_bind_count() const { return binds.size(); }

	void add_bind(int p_bone, const Transform &p_pose);

	void set_bind_bone(unsigned p_index, int p_bone);
	void set_bind_pose(unsigned p_index, const Transform &p_pose);

	inline int get_bind_bone(unsigned p_index) const {
#ifdef DEBUG_ENABLED
		ERR_FAIL_INDEX_V(p_index, binds.size(), -1);
#endif
		return binds[p_index].bone;
	}

	inline Transform get_bind_pose(unsigned p_index) const {
#ifdef DEBUG_ENABLED
		ERR_FAIL_INDEX_V(p_index, binds.size(), Transform());
#endif
		return binds[p_index].pose;
	}

	void clear_binds();

	Skin();
};

#endif // SKIN_H
