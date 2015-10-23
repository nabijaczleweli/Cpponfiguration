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


#include "../include/cpponfig/property.hpp"
#include "../include/cpponfig/util/strings.hpp"
#include "../include/cpponfig/util/salt.hpp"
#include <iterator>
#include <cstdlib>
#include <sstream>


using namespace std;
using namespace cpponfig;
using namespace cpponfig::util;


using boolean_type       = property::boolean_type;
using signed_type        = property::signed_type;
using unsigned_type      = property::unsigned_type;
using floating_type      = property::floating_type;
using boolean_list_type  = property::boolean_list_type;
using signed_list_type   = property::signed_list_type;
using unsigned_list_type = property::unsigned_list_type;
using floating_list_type = property::floating_list_type;


unsigned int property::floating_precision = numeric_limits<floating_type>::digits10 + 1;


static ios_base & empty_modifier(ios_base & base) {
	return base;
}

static ios_base & precision_modifier(ios_base & base) {
	base.precision(property::floating_precision);
	return base;
}

template <class T>
static void generic_list_update(const T & from, string & out, ios_base & (*mod)(ios_base &) = empty_modifier) {
	stringstream strm;
	strm << mod << '[';
	copy(from.begin(), from.end(), ostream_iterator<typename T::value_type>(strm, ","));
	string temp = strm.str();
	temp[temp.size() - 1] = ']';
	out = temp;
}


template <class T>
static void generic_single_update(const T & from, string & out, ios_base & (*mod)(ios_base &) = empty_modifier) {
	out = dynamic_cast<const ostringstream &>(ostringstream() << mod << from).str();
}

static bool string_to_boolean(const string & str) {
	bool temp0, temp1;
	stringstream(str) >> temp0;
	stringstream(str) >> boolalpha >> temp1;
	return temp0 || temp1;
}


void property::compute_boolean() {
	if(!boolean_value)
		boolean_value = make_unique<boolean_type>(string_to_boolean(raw_value));
}

void property::compute_integer() {
	if(!signed_value || !unsigned_value) {
		signed_value   = make_unique<signed_type>(strtoll(raw_value.c_str(), nullptr, 0));
		unsigned_value = make_unique<unsigned_type>(strtoull(raw_value.c_str(), nullptr, 0));
	}
}

void property::compute_floating() {
	if(!floating_value)
		floating_value = make_unique<floating_type>(strtold(raw_value.c_str(), nullptr));
}

void property::compute_list() {
	if((!boolean_list_value || !signed_list_value || !unsigned_list_value || !floating_list_value) && !raw_value.empty() && raw_value[0] == '[' &&
	   raw_value.find(']') != string::npos && raw_value.find(',') != string::npos) {
		boolean_list_value  = make_unique<boolean_list_type>();
		signed_list_value   = make_unique<signed_list_type>();
		unsigned_list_value = make_unique<unsigned_list_type>();
		floating_list_value = make_unique<floating_list_type>();

		list<string> elements;
		size_t comma_idx;
		string temp = raw_value.substr(1, raw_value.find_first_of(']') - 1);
		trim(temp);
		temp.push_back(',');

		while((comma_idx = temp.find_first_of(',')) != string::npos) {
			elements.emplace_front(trim(temp.substr(0, comma_idx)));
			temp = temp.substr(temp.find_first_of(',') + 1);
			ltrim(temp);
		}

		for(const auto & element : elements) {
			boolean_list_value->emplace_front(string_to_boolean(element));
			signed_list_value->emplace_front(strtoll(element.c_str(), nullptr, 0));
			unsigned_list_value->emplace_front(strtoull(element.c_str(), nullptr, 0));
			floating_list_value->emplace_front(strtold(element.c_str(), nullptr));
		}
	}

	if(!boolean_list_value || !signed_list_value || !unsigned_list_value || !floating_list_value) {
		boolean_list_value  = make_unique<boolean_list_type>();
		signed_list_value   = make_unique<signed_list_type>();
		unsigned_list_value = make_unique<unsigned_list_type>();
		floating_list_value = make_unique<floating_list_type>();
	}
}

void property::clear_except(const void * except) {
#define DEL(which)                          \
	if((which) && addressof(which) != except) \
		(which).reset();

	DEL(boolean_value)
	DEL(signed_value)
	DEL(unsigned_value)
	DEL(floating_value)
	DEL(boolean_list_value)
	DEL(signed_list_value)
	DEL(unsigned_list_value)
	DEL(floating_list_value)

#undef DEL
}


#define PROPERTY(type, name, computer) \
	type##_type & property::name() {     \
		compute_##computer();              \
		return *type##_value;              \
	}

PROPERTY(boolean, boolean, boolean)
PROPERTY(signed, integer, integer)
PROPERTY(unsigned, unsigned_integer, integer)
PROPERTY(floating, floating, floating)
PROPERTY(boolean_list, boolean_list, list)
PROPERTY(signed_list, integer_list, list)
PROPERTY(unsigned_list, unsigned_integer_list, list)
PROPERTY(floating_list, floating_list, list)

#undef PROPERTY
#define UPDATE(type, name, updatetype, format)                \
	void property::update_from_##name() {                       \
		generic_##updatetype##_update(name(), raw_value, format); \
		clear_except(type##_value);                               \
	}

UPDATE(boolean, boolean, single, boolalpha)
UPDATE(signed, integer, single, empty_modifier)
UPDATE(unsigned, unsigned_integer, single, empty_modifier)
UPDATE(floating, floating, single, precision_modifier)
UPDATE(boolean_list, boolean_list, list, boolalpha)
UPDATE(signed_list, integer_list, list, empty_modifier)
UPDATE(unsigned_list, unsigned_integer_list, list, empty_modifier)
UPDATE(floating_list, floating_list, list, precision_modifier)

void property::update_from_textual() {
	clear();
}

#undef UPDATE

void property::clear() {
	clear_except(nullptr);
}

void property::swap(property & other) {
	using std::swap;
	swap(raw_value, other.raw_value);
	swap(boolean_value, other.boolean_value);
	swap(signed_value, other.signed_value);
	swap(unsigned_value, other.unsigned_value);
	swap(floating_value, other.floating_value);
	swap(boolean_list_value, other.boolean_list_value);
	swap(signed_list_value, other.signed_list_value);
	swap(unsigned_list_value, other.unsigned_list_value);
	swap(floating_list_value, other.floating_list_value);
	swap(comment, other.comment);
}

// All hex numbers here are primes
size_t property::hash_code() const {
	static const salt slt{};
	static const hash<string> string_hash{};

	return 0x3A8F05C5 ^ slt ^ string_hash(raw_value) ^ (!comment.empty() ? 0x1AFF2BAD : string_hash(comment));
}

property::property(const string & val, const string & cmt) : raw_value(val), comment(cmt) {}

#define CTR(var) var##_value(other.var##_value ? new var##_type(*other.var##_value) : nullptr)
property::property(const property & other)
      : raw_value(other.raw_value), CTR(boolean), CTR(signed), CTR(unsigned), CTR(floating), CTR(boolean_list), CTR(signed_list), CTR(unsigned_list),
        CTR(floating_list), comment(other.comment) {}
#undef CTR

#define CTR(var) var(move(other.var))
property::property(property && other)
      : CTR(raw_value), CTR(boolean_value), CTR(signed_value), CTR(unsigned_value), CTR(floating_value), CTR(boolean_list_value), CTR(signed_list_value),
        CTR(unsigned_list_value), CTR(floating_list_value), CTR(comment) {}
#undef CTR


bool operator==(const property & lhs, const property & rhs) {
	return lhs.textual() == rhs.textual() && lhs.comment == rhs.comment;
}
