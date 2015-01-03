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
#include <cstdlib>
#include <sstream>

using namespace std;

typedef property::signed_type signed_type;
typedef property::unsigned_type unsigned_type;
typedef property::floating_type floating_type;


void property::compute_integer() {
	if(!int_signed_value || !int_unsigned_value) {
		re_create(int_signed_value);
		re_create(int_unsigned_value);
		*int_signed_value = strtoull(raw_value.c_str(), nullptr, 0);
		*int_unsigned_value = strtoll(raw_value.c_str(), nullptr, 0);
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
		bool temp0, temp1;
		stringstream(raw_value) >> temp0;
		stringstream(raw_value) >> boolalpha >> temp1;
		*boolean_value = temp0 || temp1;
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
