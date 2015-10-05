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


#include "../include/cpponfig/configuration_category.hpp"
#include "../include/cpponfig/configuration.hpp"
#include "../include/cpponfig/util/strings.hpp"
#include "../include/cpponfig/util/salt.hpp"
#include <istream>
#include <fstream>
#include <cstring>


using namespace std;
using namespace std::experimental;
using namespace cpponfig;
using namespace cpponfig::util;


configuration_category::configuration_category() {}
configuration_category::configuration_category(const configuration_category & other) : properties(other.properties) {}
configuration_category::configuration_category(configuration_category && other) : properties(move(other.properties)) {}

configuration_category::~configuration_category() {}

void configuration_category::swap(configuration_category & other) {
	properties.swap(other.properties);
}

// All hex numbers here are primes
size_t configuration_category::hash_code() const {
#define COLHASH(col, hash, prime) \
	if(col.empty())                 \
		result ^= prime;              \
	else                            \
		for(const auto & elem : col)  \
			result ^= hash(elem);

	static const salt slt{};
	static const hash<pair<string, property>> kv_hash{};

	size_t result = 0x13EBB53B;

	COLHASH(properties, kv_hash, 0x0BD4BCC1)

	return result ^ slt;

#undef COLHASH
}

configuration_category & configuration_category::operator=(const configuration_category & other) {
	configuration_category temp(other);
	swap(temp);
	return *this;
}

configuration_category & configuration_category::operator+=(const configuration_category & other) {
	properties.insert(other.properties.begin(), other.properties.end());

	return *this;
}

// Better version? This is (I think) from O(n) to O(n^2), where n: `other.properties.size()`.
configuration_category & configuration_category::operator-=(const configuration_category & other) {
	for(const auto & kv : other.properties)
		properties.erase(kv.first);

	return *this;
}

void configuration_category::load(istream & from) {
	for(string line; getline(from, line);) {
		size_t equals_idx = 0;

		if(line.empty() || line[0] == configuration::comment_character || (equals_idx = line.find_first_of(configuration::assignment_character)) == string::npos)
			return;
		if(line[0] == configuration::category_end_character)
			break;

		ltrim(line);
		if(line.empty() || line[0] == configuration::comment_character)
			return;
		if(line[0] == configuration::category_end_character)
			break;
		equals_idx = line.find_first_of(configuration::assignment_character);

		const size_t comment_idx = line.find_first_of(configuration::comment_character);
		string comment;
		if(comment_idx != string::npos) {
			comment = line.substr(line.find_first_of(configuration::comment_character) + 1);
			line = line.substr(0, line.find_first_of(configuration::comment_character));
			rtrim(line);
			if(line.empty() || comment_idx < equals_idx)
				return;
		}

		properties.emplace(trim(move(line.substr(0, equals_idx))), property(trim(move(line.substr(equals_idx + 1))), trim(comment)));
	}
}

void configuration_category::save(ostream & to) const {
	for(const auto & pr : properties)
		to << '\t' << pr.first << configuration::assignment_character << pr.second.textual()
		   << (pr.second.comment.empty() ? "" : " "s + configuration::comment_character + " " + pr.second.comment) << '\n';
}

property & configuration_category::get(const string & key, const property & default_value) {
	auto itr = properties.find(key);
	if(itr == properties.end())
		itr = properties.emplace(trim(move(string(key))), default_value).first;  // Construct string because `*trim()`s are mutating
	return itr->second;
}

void configuration_category::remove(const string & key) {
	properties.erase(key);
}

bool configuration_category::contains(const string & key) const {
	return properties.find(key) != properties.end();
}

bool configuration_category::empty() const {
	return properties.empty();
}


configuration_category operator+(const configuration_category & lhs, const configuration_category & rhs) {
	configuration_category temp(lhs);
	temp += rhs;
	return temp;
}

configuration_category operator-(const configuration_category & lhs, const configuration_category & rhs) {
	configuration_category temp(lhs);
	temp -= rhs;
	return temp;
}
