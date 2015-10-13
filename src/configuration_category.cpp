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
#include "../include/cpponfig/parsing_error.hpp"
#include "../include/cpponfig/util/strings.hpp"
#include "../include/cpponfig/util/regex.hpp"
#include "../include/cpponfig/util/salt.hpp"
#include <istream>
#include <fstream>
#include <cstring>
#include <regex>


using namespace std;
using namespace cpponfig;
using namespace cpponfig::util;


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

configuration_category & configuration_category::operator+=(const configuration_category & other) {
	properties.insert(other.properties.begin(), other.properties.end());

	return *this;
}

configuration_category & configuration_category::operator-=(const configuration_category & other) {
	for(const auto & kv : other.properties)
		properties.erase(kv.first);

	return *this;
}

configuration_category::configuration_category(const string & cmt) : comment(cmt) {}

void configuration_category::load(istream & from) {
	static const auto is_comment    = CPPONFIG_CHARCACHED_REGEX("[[:space:]]*\\"s + chr + ".*");
	static const auto is_assignment = CPPONFIG_TWOCHARCACHED_REGEX("[[:space:]]*([^[:space:]]+)[[:space:]]*\\"s + chr1 +
	                                                               "[[:space:]]*([^[:space:]]+)[[:space:]]*(?:\\" + chr2 + "[[:space:]]*(.*))?");
	static const auto is_end_of_category = CPPONFIG_TWOCHARCACHED_REGEX("[[:space:]]*\\"s + chr1 + "[[:space:]]*(?:\\" + chr2 + ".*)?");

	smatch match;
	size_t lineno = 1;
	for(string line; getline(from, line); ++lineno) {
		++lineno;
		if(line.empty() || regex_match(line, is_comment(configuration::comment_character)))
			continue;

		if(!regex_match(line, match, is_assignment(configuration::assignment_character, configuration::comment_character))) {
			if(regex_match(line, is_end_of_category(configuration::category_end_character, configuration::comment_character)))
				return;
			else
				throw parsing_error("Line " + to_string(lineno) + " (\"" + line + "\") is not a comment nor an assignment");
		}

		properties.emplace(match.str(1), property(match.str(2), match.str(3)));
	}
}

void configuration_category::save(ostream & to, const string & name) const {
	to << name << (name.empty() ? "" : " ") << configuration::category_start_character;
	if(!comment.empty())
		to << ' ' << configuration::comment_character << ' ' << comment;
	to << '\n';
	for(const auto & pr : properties)
		to << '\t' << pr.first << configuration::assignment_character << pr.second.textual()
		   << (pr.second.comment.empty() ? "" : " "s + configuration::comment_character + " " + pr.second.comment) << '\n';
	to << configuration::category_end_character << "\n\n";
}

property & configuration_category::get(const string & key, const property & default_value) {
	auto itr = properties.find(key);
	if(itr == properties.end())
		itr = properties.emplace(trim(string(key)), default_value).first;
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
