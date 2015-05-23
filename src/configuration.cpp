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


#include "configuration.hpp"
#include "util/strings.hpp"
#include "util/salt.hpp"
#include <istream>
#include <fstream>
#include <cstring>


using namespace std;
using namespace std::experimental;
using namespace cpponfig;


typedef configuration::datetime_mode datetime_mode;


char configuration::comment_character = '#';
char configuration::assignment_character = '=';
datetime_mode configuration::datetime_footer_type = datetime_mode::none;


configuration::configuration() {}
configuration::configuration(const string & name) : filename(name) {}
configuration::configuration(const configuration & other) : properties(other.properties), filename(other.filename),
                                                            sof_comments(other.sof_comments) {}
configuration::configuration(configuration && other) : properties(move(other.properties)), filename(move(other.filename)),
                                                       sof_comments(move(other.sof_comments)) {}

configuration::~configuration() {}

void configuration::swap(configuration & other) {
	properties.swap(other.properties);
	filename.swap(other.filename);
	sof_comments.swap(other.sof_comments);
}

// All hex numbers here are primes
size_t configuration::hash_code() const {
	#define COLHASH(col, hash, prime) if(col.empty()) \
	                             result ^= prime; \
	                           else \
	                             for(const auto & elem : col) \
	                                result ^= hash(elem);

	static const salt slt;
	static const hash<pair<string, property>> kv_hash;
	static const hash<string> string_hash;

	size_t result = 0x26FE1F8D;

	result ^= (filename ? string_hash(*filename) : 0x12C0852B);
	COLHASH(properties, kv_hash, 0x16447FAB)
	COLHASH(sof_comments, string_hash, 0x39531FBF)

	return result ^ slt;

	#undef COLHASH
}

configuration & configuration::operator=(const configuration & other) {
	configuration temp(other);
	swap(temp);
	return *this;
}

configuration & configuration::operator+=(const configuration & other) {
	properties.insert(other.properties.begin(), other.properties.end());
	sof_comments.insert(sof_comments.end(), other.sof_comments.begin(), other.sof_comments.end());

	return *this;
}

// Better version? This is (I think) from O(n) to O(n^2), where n: `other.properties.size()`.
configuration & configuration::operator-=(const configuration & other) {
	for(const auto & kv : other.properties)
		properties.erase(kv.first);

	for(const auto & cmt : other.sof_comments)
		sof_comments.remove(cmt);

	return *this;
}

void configuration::load_properties(istream & from) {
	static const auto readfromline = [&](string & line) {
		size_t equals_idx = 0;

		if(line.empty() || line[0] == comment_character || (equals_idx = line.find_first_of(assignment_character)) == string::npos)
			return;

		ltrim(line);
		if(line.empty() || line[0] == comment_character)
			return;
		equals_idx = line.find_first_of(assignment_character);

		const size_t comment_idx = line.find_first_of(comment_character);
		string comment;
		if(comment_idx != string::npos) {
			comment = line.substr(line.find_first_of(comment_character) + 1);
			line = line.substr(0, line.find_first_of(comment_character));
			rtrim(line);
			if(line.empty() || comment_idx < equals_idx)
				return;
		}

		properties.emplace(trim(move(line.substr(0, equals_idx))), property(trim(move(line.substr(equals_idx + 1))), trim(comment)));
	};

	for(string line; getline(from, line);) {
		if(line.empty())
			break;

		ltrim(line);
		if(line[0] != comment_character) {
			readfromline(line);
			break;
		}

		sof_comments.emplace_back(trim(move(string(line.c_str() + 1))));
	}

	for(string line; getline(from, line);)
		readfromline(line);
}

void configuration::save_properties(ostream & to) const {
	for(const auto & cmt : sof_comments)
		to << comment_character << ' ' << cmt << '\n';
	to << (sof_comments.empty() ? "" : "\n\n");
	for(const auto & pr : properties)
		to << pr.first << assignment_character << pr.second.textual() <<
	                                            (pr.second.comment.empty() ? "" : string(" ") + comment_character + " " + pr.second.comment) << "\n\n";

	if(datetime_footer_type != datetime_mode::none) {
		const bool isgmt = datetime_footer_type == datetime_mode::gmt;
		const time_t * tme = new time_t(time(nullptr));
		char * buf = new char[20];

		to << '\n' << comment_character << "  ";
		if(strftime(buf, 20, "%d.%m.%Y %H:%M:%S", isgmt ? gmtime(tme) : localtime(tme)))
			to << buf << (isgmt ? " GMT" : "");
		else
			to << "<<DATE ERROR>>";
		to << "\n\n";

		delete tme; tme = nullptr;
		delete[] buf; buf = nullptr;
	}
}

bool configuration::load() {
	if(filename && !filename->empty()) {
		ifstream file(*filename);
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
	return save(filename.value_or(""));
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
	return get(key, property(trim(move(string(default_value)))));  // Construct string because `*trim()`s are mutating
}

property & configuration::get(const string & key, const property & default_value) {
	auto itr = properties.find(key);
	if(itr == properties.end())
		itr = properties.emplace(trim(move(string(key))), default_value).first;  // Construct string because `*trim()`s are mutating
	return itr->second;
}

void configuration::remove(const string & key) {
	properties.erase(key);
}

bool configuration::contains(const string & key) const {
	return properties.find(key) != properties.end();
}

void configuration::rename(const string & name) {
	filename = name;
}

bool configuration::empty() {
	return properties.empty() && sof_comments.empty();
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
