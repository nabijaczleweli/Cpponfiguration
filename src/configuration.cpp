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


#include "../include/cpponfig/configuration.hpp"
#include "../include/cpponfig/parsing_error.hpp"
#include "../include/cpponfig/util/strings.hpp"
#include "../include/cpponfig/util/regex.hpp"
#include "../include/cpponfig/util/salt.hpp"
#include "config.h"
#include <iomanip>
#include <istream>
#include <fstream>
#include <cstring>
#include <regex>


using namespace std;
using namespace cpponfig;
using namespace cpponfig::util;


using datetime_mode = configuration::datetime_mode;


bool configuration::save_on_destruction           = true;
char configuration::comment_character             = '#';
char configuration::assignment_character          = '=';
char configuration::category_character            = ':';
char configuration::category_start_character      = '{';
char configuration::category_end_character        = '}';
datetime_mode configuration::datetime_footer_type = datetime_mode::none;


std::pair<std::string, std::string> configuration::property_path(const std::string & name) {
	const auto idx = name.find(category_character);
	if(idx == string::npos)
		return {"", trim(name)};
	else
		return {trim(name.substr(0, idx)), trim(name.substr(idx + 1))};
}

static void actually_put_time(ostream & to, datetime_mode mode, char comment) {
	if(mode != datetime_mode::none) {
		const bool isgmt = mode == datetime_mode::gmt;
		const time_t tme = time(nullptr);
		to << '\n' << comment << "  ";
#ifdef HAVE_STD_PUT_TIME_CHAR_
		to << put_time(isgmt ? gmtime(&tme) : localtime(&tme), "%d.%m.%Y %H:%M:%S");
#else
		char buf[20];
		to << (strftime(buf, 20, "%d.%m.%Y %H:%M:%S", isgmt ? gmtime(&tme) : localtime(&tme)) ? buf : "<<DATE ERROR>>");
#endif
		if(isgmt)
			to << " GMT";
		to << "\n\n";
	}
}


configuration::configuration(const string & name) : filename(name) {}

configuration::~configuration() {
	if(save_on_destruction)
		save();
}

void configuration::swap(configuration & other) {
	using std::swap;
	swap(categories, other.categories);
	swap(filename, other.filename);
	swap(sof_comments, other.sof_comments);
}

// All hex numbers here are primes
size_t configuration::hash_code() const {
#define COLHASH(col, hash, prime) \
	if(col.empty())                 \
		result ^= prime;              \
	else                            \
		for(const auto & elem : col)  \
			result ^= hash(elem);

	static const salt slt{};
	static const hash<pair<string, configuration_category>> kv_hash{};
	static const hash<string> string_hash{};

	size_t result = 0x26FE1F8D;

	result ^= (filename.empty() ? 0x12C0852B : string_hash(filename));
	COLHASH(categories, kv_hash, 0x16447FAB)
	COLHASH(sof_comments, string_hash, 0x39531FBF)

	return result ^ slt;

#undef COLHASH
}

configuration & configuration::operator+=(const configuration & other) {
	for(const auto & kv : other.categories) {
		const auto itr = categories.find(kv.first);
		if(itr == categories.end())
			categories.emplace(kv.first, kv.second);
		else
			itr->second += kv.second;
	}
	sof_comments.insert(sof_comments.end(), other.sof_comments.begin(), other.sof_comments.end());

	return *this;
}

configuration & configuration::operator-=(const configuration & other) {
	for(const auto & kv : other.categories)
		categories.erase(kv.first);

	for(const auto & cmt : other.sof_comments)
		sof_comments.erase(find(sof_comments.begin(), sof_comments.end(), cmt));

	return *this;
}

void configuration::load_properties(istream & from) {
	static const auto readfromline = [&](string & line, size_t lineno) {
		//
		static const auto is_comment       = CPPONFIG_CHARCACHED_REGEX("(?:[[:space:]]*(?:"s + chr + "[[:space:]]*(.*))?)?");
		static const auto is_tagless_start = CPPONFIG_TWOCHARCACHED_REGEX("[[:space:]]*\\"s + chr2 + "[[:space:]]*(?:\\" + chr1 + "[[:space:]]*(.*))?");
		static const auto is_tagged_start =
		    CPPONFIG_TWOCHARCACHED_REGEX("[[:space:]]*([^[:space:]]+)[[:space:]]*\\"s + chr2 + "[[:space:]]*(?:\\" + chr1 + "[[:space:]]*(.*))?");

		smatch match;
		if(regex_match(line, match, is_comment(comment_character)))
			return match.str(1);
		else if(regex_match(line, match, is_tagless_start(comment_character, category_start_character)))
			categories.emplace("", configuration_category(match.str(1))).first->second.load(from);
		else if(regex_match(line, match, is_tagged_start(comment_character, category_start_character)))
			categories.emplace(match.str(1), configuration_category(match.str(2))).first->second.load(from);
		else
			throw parsing_error("Line " + to_string(lineno) + " (\"" + line + "\") is not a comment nor a category start");
		return ""s;
	};

	size_t lineno = 1;
	for(string line; getline(from, line); ++lineno) {
		const auto & comment = readfromline(line, lineno);
		if(!comment.empty())
			sof_comments.emplace_back(comment);
		else
			break;
	}

	for(string line; getline(from, line); ++lineno)
		readfromline(line, lineno);
}

void configuration::save_properties(ostream & to) const {
	for(const auto & cmt : sof_comments)
		to << comment_character << ' ' << cmt << '\n';
	to << (sof_comments.empty() ? "" : "\n\n");
	for(const auto & pr : categories)
		pr.second.save(to, pr.first);

	actually_put_time(to, datetime_footer_type, comment_character);
}

bool configuration::load() {
	if(!filename.empty()) {
		ifstream file(filename);
		if(file && file.is_open()) {
			load_properties(file);
			return true;
		}
	}
	return false;
}

bool configuration::load(const string & name) {
	rename(name);
	return load();
}

bool configuration::load(istream & stream) {
	if(stream) {
		load_properties(stream);
		return true;
	}
	return false;
}

bool configuration::save() const {
	return save(filename);
}

bool configuration::save(const string & name) const {
	if(!name.empty()) {
		ofstream file(name);
		if(file && file.is_open()) {
			save_properties(file);
			return true;
		}
	}
	return false;
}

bool configuration::save(ostream & stream) const {
	if(stream) {
		save_properties(stream);
		return true;
	}
	return false;
}

property & configuration::get(const string & key, const string & default_value) {
	return get(key, property(trim(string(default_value))));
}

property & configuration::get(const string & key, const property & default_value) {
	const auto path = property_path(key);
	auto itr = categories.find(path.first);
	if(itr == categories.end()) {
		const auto kv = make_pair(path.second, default_value);
		itr           = categories.emplace(path.first, configuration_category(addressof(kv), addressof(kv) + 1)).first;
	}
	return itr->second.get(path.second, default_value);
}

void configuration::remove(const string & key) {
	const auto path = property_path(key);
	const auto itr = categories.find(path.first);
	if(itr != categories.end())
		itr->second.remove(path.second);
}

bool configuration::contains(const string & key) const {
	const auto path = property_path(key);
	const auto itr = categories.find(path.first);
	return itr != categories.end() && itr->second.contains(path.second);
}

void configuration::rename(const string & name) {
	filename = name;
}

bool configuration::empty() const {
	return sof_comments.empty() && all_of(categories.begin(), categories.end(), [](const auto & pr) { return pr.second.empty(); });
}


configuration operator+(const configuration & lhs, const configuration & rhs) {
	configuration temp(lhs);
	temp += rhs;
	return temp;
}

configuration operator-(const configuration & lhs, const configuration & rhs) {
	configuration temp(lhs);
	temp -= rhs;
	return temp;
}
