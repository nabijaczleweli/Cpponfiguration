// The MIT License (MIT)

// Copyright (c) 2015 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "property.hpp"
#include "util/strings.hpp"
#include <cstdlib>
#include <sstream>

using namespace std;

typedef property::signed_type signed_type;
typedef property::unsigned_type unsigned_type;
typedef property::floating_type floating_type;
typedef property::signed_list_type signed_list_type;
typedef property::unsigned_list_type unsigned_list_type;
typedef property::floating_list_type floating_list_type;
typedef property::boolean_list_type boolean_list_type;

template<class T>
inline void re_create(T *& ptr) {
	if(ptr)
		delete ptr;
	ptr = new T;
}

bool string_to_boolean(const string & str) {
	bool temp0, temp1;
	stringstream(str) >> temp0;
	stringstream(str) >> boolalpha >> temp1;
	return temp0 || temp1;
}


void property::compute_integer() {
	if(!int_signed_value || !int_unsigned_value) {
		re_create(int_signed_value);
		re_create(int_unsigned_value);
		*int_signed_value = strtoll(raw_value.c_str(), nullptr, 0);
		*int_unsigned_value = strtoull(raw_value.c_str(), nullptr, 0);
	}
}

void property::compute_floating() {
	if(!floating_value) {
		re_create(floating_value);
		*floating_value = strtold(raw_value.c_str(), nullptr);
	}
}

void property::compute_logical() {
	if(!boolean_value) {
		re_create(boolean_value);
		*boolean_value = string_to_boolean(raw_value);
	}
}

void property::compute_list() {
	if(!raw_value.empty() && raw_value[0] == '[' && raw_value.find(']') != string::npos && raw_value.find(',') != string::npos &&
	   (!signed_list_value || !unsigned_list_value || !floating_list_value || !boolean_list_value)) {
		re_create(signed_list_value);
		re_create(unsigned_list_value);
		re_create(floating_list_value);
		re_create(boolean_list_value);

		list<string> elements;
		size_t comma_idx;
		string temp = raw_value.substr(1, raw_value.find_first_of(']') - 1);
		trim(temp);
		temp.push_back(',');

		while((comma_idx = temp.find_first_of(',')) != string::npos) {
			elements.emplace_front(trim(move(temp.substr(0, comma_idx))));
			temp = temp.substr(temp.find_first_of(',') + 1);
			ltrim(temp);
		}

		for(const auto & element : elements) {
			signed_list_value->emplace_front(strtoll(element.c_str(), nullptr, 0));
			unsigned_list_value->emplace_front(strtoull(element.c_str(), nullptr, 0));
			floating_list_value->emplace_front(strtold(element.c_str(), nullptr));
			boolean_list_value->emplace_front(string_to_boolean(element));
		}
	}
}

const signed_type & property::integer() {
	compute_integer();
	return *int_signed_value;
}

const unsigned_type & property::unsigned_integer() {
	compute_integer();
	return *int_unsigned_value;
}

const floating_type & property::floating() {
	compute_floating();
	return *floating_value;
}

const bool & property::boolean() {
	compute_logical();
	return *boolean_value;
}

const signed_list_type & property::integer_list() {
	compute_list();
	return *signed_list_value;
}

const unsigned_list_type & property::unsigned_integer_list() {
	compute_list();
	return *unsigned_list_value;
}

const floating_list_type & property::floating_list() {
	compute_list();
	return *floating_list_value;
}

const boolean_list_type & property::boolean_list() {
	compute_list();
	return *boolean_list_value;
}

property::property(const string & val) : raw_value(val) {}

property::~property() {
	if(boolean_value) {
		delete boolean_value;
		boolean_value = nullptr;
	}
	if(int_signed_value) {
		delete int_signed_value;
		int_signed_value = nullptr;
	}
	if(int_unsigned_value) {
		delete int_unsigned_value;
		int_unsigned_value = nullptr;
	}
	if(floating_value) {
		delete floating_value;
		floating_value = nullptr;
	}
}
