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
#include "util/salt.hpp"
#include <cstdlib>
#include <sstream>


using namespace std;
using namespace cpponfig;


typedef property::signed_type signed_type;
typedef property::unsigned_type unsigned_type;
typedef property::floating_type floating_type;
typedef property::boolean_list_type boolean_list_type;
typedef property::signed_list_type signed_list_type;
typedef property::unsigned_list_type unsigned_list_type;
typedef property::floating_list_type floating_list_type;


unsigned int property::floating_precision = std::numeric_limits<floating_type>::digits10 + 1;


template<class T>
void re_create(T *& ptr) {
	delete ptr;
	ptr = new T;
}

template<class T, class... A>
void re_create(T *& ptr, A&&... args) {
	delete ptr;
	ptr = new T(forward<A>(args)...);
}

ios_base & empty_modifier(ios_base & base) {
	return base;
}

ios_base & precision_modifier(ios_base & base) {
	base.precision(property::floating_precision);
	return base;
}

template<class T>
void generic_list_update(const T & from, string & out, ios_base &(*mod)(ios_base &) = empty_modifier) {
	stringstream strm;
	strm << mod;
	size_t left = from.size();
	for(const auto & i : from)
		strm << i << (--left ? "," : "");
	out = '[' + strm.str() + ']';
}


template<class T>
void generic_single_update(const T & from, string & out, ios_base &(*mod)(ios_base &) = empty_modifier) {
	out = dynamic_cast<ostringstream &>(ostringstream() << mod << from).str();
}

bool string_to_boolean(const string & str) {
	bool temp0, temp1;
	stringstream(str) >> temp0;
	stringstream(str) >> boolalpha >> temp1;
	return temp0 || temp1;
}


void property::compute_logical() {
	if(!boolean_value)
		re_create(boolean_value, string_to_boolean(raw_value));
}

void property::compute_integer() {
	if(!int_signed_value || !int_unsigned_value) {
		re_create(int_signed_value, strtoll(raw_value.c_str(), nullptr, 0));
		re_create(int_unsigned_value, strtoull(raw_value.c_str(), nullptr, 0));
	}
}

void property::compute_floating() {
	if(!floating_value)
		re_create(floating_value, strtold(raw_value.c_str(), nullptr));
}

void property::compute_list() {
	if(!raw_value.empty() && raw_value[0] == '[' && raw_value.find(']') != string::npos && raw_value.find(',') != string::npos &&
	   (!boolean_list_value || !signed_list_value || !unsigned_list_value || !floating_list_value)) {
		re_create(boolean_list_value);
		re_create(signed_list_value);
		re_create(unsigned_list_value);
		re_create(floating_list_value);

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
			boolean_list_value->emplace_front(string_to_boolean(element));
			signed_list_value->emplace_front(strtoll(element.c_str(), nullptr, 0));
			unsigned_list_value->emplace_front(strtoull(element.c_str(), nullptr, 0));
			floating_list_value->emplace_front(strtold(element.c_str(), nullptr));
		}
	}

	if(!boolean_list_value || !signed_list_value || !unsigned_list_value || !floating_list_value) {
		re_create(boolean_list_value);
		re_create(signed_list_value);
		re_create(unsigned_list_value);
		re_create(floating_list_value);
	}
}

void property::clear_except(const void * except) {
	#define DEL(which) if((which) && (which) != except) { \
	                     delete (which); \
	                     (which) = nullptr; \
	                   }

	DEL(boolean_value)
	DEL(int_signed_value)
	DEL(int_unsigned_value)
	DEL(floating_value)
	DEL(boolean_list_value)
	DEL(signed_list_value)
	DEL(unsigned_list_value)
	DEL(floating_list_value)

	#undef DEL
}

bool & property::boolean() {
	compute_logical();
	return *boolean_value;
}

signed_type & property::integer() {
	compute_integer();
	return *int_signed_value;
}

unsigned_type & property::unsigned_integer() {
	compute_integer();
	return *int_unsigned_value;
}

floating_type & property::floating() {
	compute_floating();
	return *floating_value;
}

boolean_list_type & property::boolean_list() {
	compute_list();
	return *boolean_list_value;
}

signed_list_type & property::integer_list() {
	compute_list();
	return *signed_list_value;
}

unsigned_list_type & property::unsigned_integer_list() {
	compute_list();
	return *unsigned_list_value;
}

floating_list_type & property::floating_list() {
	compute_list();
	return *floating_list_value;
}

void property::update_from_boolean() {
	generic_single_update(boolean(), raw_value, boolalpha);
	clear_except(boolean_value);
}

void property::update_from_integer() {
	generic_single_update(integer(), raw_value);
	clear_except(int_signed_value);
}

void property::update_from_unsigned_integer() {
	generic_single_update(unsigned_integer(), raw_value);
	clear_except(int_unsigned_value);
}

void property::update_from_floating() {
	generic_single_update(floating(), raw_value, precision_modifier);
	clear_except(floating_value);
}

void property::update_from_textual() {
	clear();
}

void property::update_from_boolean_list() {
	generic_list_update(boolean_list(), raw_value, boolalpha);
	clear_except(boolean_list_value);
}

void property::update_from_integer_list() {
	generic_list_update(integer_list(), raw_value);
	clear_except(signed_list_value);
}

void property::update_from_unsigned_integer_list() {
	generic_list_update(unsigned_integer_list(), raw_value);
	clear_except(unsigned_list_value);
}

void property::update_from_floating_list() {
	generic_list_update(floating_list(), raw_value, precision_modifier);
	clear_except(floating_list_value);
}

void property::clear() {
	clear_except(nullptr);
}

property & property::operator=(const property & other) {
	property temp(other);
	swap(temp);
	return *this;
}

void property::swap(property & other) {
	#define SWAP(a)	{const auto temp((a)); (a) = (other.a); (other.a) = temp;}

	raw_value.swap(other.raw_value);
	SWAP(boolean_value)
	SWAP(int_signed_value)
	SWAP(int_unsigned_value)
	SWAP(floating_value)
	SWAP(boolean_list_value)
	SWAP(signed_list_value)
	SWAP(unsigned_list_value)
	SWAP(floating_list_value)
	comment.swap(other.comment);

	#undef SWAP
}

// All hex numbers here are primes
size_t property::hash_code() const {
	static const salt slt;
	static const hash<string> string_hash;

	return 0x3A8F05C5 ^ slt ^ string_hash(raw_value) ^ (!comment.empty() ? 0x1AFF2BAD : string_hash(comment));
}

property::property(const string & val, const string & cmt) : raw_value(val), comment(cmt) {}

property::property(const property & other) : property(other.raw_value, other.comment) {
	#define RECR(which) if(other.which) \
	                      re_create((which), *(other.which));

	RECR(boolean_value)
	RECR(int_signed_value)
	RECR(int_unsigned_value)
	RECR(floating_value)
	RECR(boolean_list_value)
	RECR(signed_list_value)
	RECR(unsigned_list_value)
	RECR(floating_list_value)

	#undef RECR
}

#define CTR(var) var(move(other.var))
property::property(property && other) : CTR(raw_value), CTR(boolean_value), CTR(int_signed_value), CTR(int_unsigned_value), CTR(floating_value),
                                        CTR(boolean_list_value), CTR(signed_list_value), CTR(unsigned_list_value), CTR(floating_list_value), CTR(comment) {
	other.boolean_value = nullptr;
	other.int_signed_value = nullptr;
	other.int_unsigned_value = nullptr;
	other.floating_value = nullptr;
	other.boolean_list_value = nullptr;
	other.signed_list_value = nullptr;
	other.unsigned_list_value = nullptr;
	other.floating_list_value = nullptr;
}
#undef CTR

property::~property() {
	clear();
}
