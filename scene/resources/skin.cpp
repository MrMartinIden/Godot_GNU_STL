/*************************************************************************/
/*  skin.cpp                                                             */
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

#include "skin.h"

// warning_update : p_size can be only positiv
void Skin::set_bind_count(int p_size) {
	ERR_FAIL_COND(p_size < 0);

	binds.resize(p_size);

	emit_changed();
}

void Skin::add_bind(int p_bone, const Transform &p_pose) {
	unsigned index = binds.size();

	set_bind_count(index + 1);

	set_bind_bone(index, p_bone);

	set_bind_pose(index, p_pose);
}

void Skin::set_bind_bone(unsigned p_index, int p_bone) {
	ERR_FAIL_INDEX(p_index, binds.size());

	binds[p_index].bone = p_bone;

	emit_changed();
}

void Skin::set_bind_pose(unsigned p_index, const Transform &p_pose) {
	ERR_FAIL_INDEX(p_index, binds.size());

	binds[p_index].pose = p_pose;

	emit_changed();
}

void Skin::clear_binds() {
	binds.clear();

	emit_changed();
}

bool Skin::_set(const StringName &p_name, const Variant &p_value) {
	String name = p_name;
	if (name == "bind_count") {
		set_bind_count(p_value);
		return true;
	} else if (name.begins_with("bind/")) {
		int index = name.get_slicec('/', 1).to_int();
		String what = name.get_slicec('/', 2);
		if (what == "bone") {
			set_bind_bone(index, p_value);
			return true;
		} else if (what == "pose") {
			set_bind_pose(index, p_value);
			return true;
		}
	}
	return false;
}

bool Skin::_get(const StringName &p_name, Variant &r_ret) const {

	String name = p_name;
	if (name == "bind_count") {
		r_ret = get_bind_count();
		return true;
	} else if (name.begins_with("bind/")) {
		int index = name.get_slicec('/', 1).to_int();
		String what = name.get_slicec('/', 2);
		if (what == "bone") {
			r_ret = get_bind_bone(index);
			return true;
		} else if (what == "pose") {
			r_ret = get_bind_pose(index);
			return true;
		}
	}
	return false;
}
void Skin::_get_property_list(List<PropertyInfo> *p_list) const {
	p_list->push_back(PropertyInfo(Variant::INT, "bind_count", PROPERTY_HINT_RANGE, "0,16384,1,or_greater"));
	for (int i = 0; i < get_bind_count(); i++) {
		p_list->push_back(PropertyInfo(Variant::INT, "bind/" + itos(i) + "/bone", PROPERTY_HINT_RANGE, "0,16384,1,or_greater"));
		p_list->push_back(PropertyInfo(Variant::TRANSFORM, "bind/" + itos(i) + "/pose"));
	}
}

void Skin::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_bind_count", "bind_count"), &Skin::set_bind_count);
	ClassDB::bind_method(D_METHOD("get_bind_count"), &Skin::get_bind_count);

	ClassDB::bind_method(D_METHOD("add_bind", "bone", "pose"), &Skin::add_bind);

	ClassDB::bind_method(D_METHOD("set_bind_pose", "bind_index", "pose"), &Skin::set_bind_pose);
	ClassDB::bind_method(D_METHOD("get_bind_pose", "bind_index"), &Skin::get_bind_pose);

	ClassDB::bind_method(D_METHOD("set_bind_bone", "bind_index", "bone"), &Skin::set_bind_bone);
	ClassDB::bind_method(D_METHOD("get_bind_bone", "bind_index"), &Skin::get_bind_bone);

	ClassDB::bind_method(D_METHOD("clear_binds"), &Skin::clear_binds);
}

Skin::Skin() {
}
