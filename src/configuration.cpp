// The MIT License (MIT)

// Copyright (c) 2015 nabijaczlweli

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
#include <iostream>


using namespace std;


char configuration::comment_character = '#';
char configuration::assignment_character = '=';
bool configuration::force_create_files = true;


configuration::configuration() : configuration(nullptr) {}
configuration::configuration(string * name) : filename(name) {}
configuration::configuration(const string & name) : configuration(new string(name)) {}
configuration::configuration(const configuration & other) : properties(other.properties), filename(other.filename ? new string(*other.filename) : nullptr) {}
configuration::configuration(configuration && other) : properties(other.properties), filename(other.filename) {
	other.filename = nullptr;
}

configuration::~configuration() {
	if(filename) {
		delete filename;
		filename = nullptr;
	}
}

void configuration::swap(configuration & other) {
	#define SWAP(a)	{const auto temp((a)); (a) = (other.a); (other.a) = temp;}

	properties.swap(other.properties);
	SWAP(filename)

	#undef SWAP
}

// All hex numbers here are primes
size_t configuration::hash_code() const {
	static salt slt;
	static hash<pair<string, property>> kv_hash;
	static hash<string> string_hash;

	size_t result = 0x26FE1F8D;

	result ^= (filename ? string_hash(*filename) : 0x12C0852B);
	for(const auto & kv : properties)
		result ^= kv_hash(kv);

	return result ^ slt;
}

configuration & configuration::operator=(const configuration & other) {
	configuration temp(other);
	swap(temp);
	return *this;
}

configuration configuration::operator+(const configuration & other) {
	configuration temp(*this);
	temp += other;
	return temp;
}

configuration & configuration::operator+=(const configuration & other) {
	properties.insert(other.properties.begin(), other.properties.end());
	return *this;
}

configuration configuration::operator-(const configuration & other) {
	configuration temp(*this);
	temp -= other;
	return temp;
}

// Better version? This is (I think) from O(n) to O(n^2), where n: `other.properties.size()`.
configuration & configuration::operator-=(const configuration & other) {
	for(const auto & kv : other.properties)
		properties.erase(kv.first);
	return *this;
}

void configuration::load_properties(istream & from) {
	for(string line; getline(from, line);) {
		size_t equals_idx = 0;

		if(line.empty() || line[0] == comment_character || (equals_idx = line.find_first_of(assignment_character)) == string::npos)
			continue;

		ltrim(line);
		if(line.empty() || line[0] == comment_character)
			continue;
		equals_idx = line.find_first_of(assignment_character);

		const size_t comment_idx = line.find_first_of(comment_character);
		string comment;
		if(comment_idx != string::npos) {
			comment = line.substr(line.find_first_of(comment_character) + 1);
			line = line.substr(0, line.find_first_of(comment_character));
			rtrim(line);
			if(line.empty() || comment_idx < equals_idx)
				continue;
		}

		properties.emplace(trim(move(line.substr(0, equals_idx))), property(trim(move(line.substr(equals_idx + 1))), trim(comment)));
	}
}

void configuration::save_properties(ostream & to) const {
	for(const auto & pr : properties)
		to << pr.first << '=' << pr.second.textual() << (pr.second.comment.empty() ? "" : string(" ") + comment_character + " " + pr.second.comment) << "\n\n";
}

bool configuration::save(const string * name) const {
	if(name) {
		ofstream file(*name);
		if(file.is_open()) {
			save_properties(file);
			return true;
		}
	}
	return false;
}

bool configuration::load() {
	if(filename) {
		if(force_create_files)
			ofstream(*filename).rdstate();  // Constructor creates file, `.rdstate()` to force compiler into not seeing this as a variable declaration
		ifstream file(*filename);
		if(file.is_open()) {
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
	return save(&name);
}

bool configuration::save(ostream & stream) const {
	if(stream) {
		save_properties(stream);
		return true;
	}
	return false;
}

property & configuration::get(const string & key, const string & default_value) {
	auto itr = properties.find(key);
	if(itr == properties.end())
		itr = properties.emplace(trim(move(string(key))), property(trim(move(string(default_value))))).first;  // Construct strings because `*trim()`s are mutating
	return itr->second;
}

bool configuration::contains(const string & key) const {
	return properties.find(key) != properties.end();
}

void configuration::rename(const string & name) {
	if(filename)
		*filename = name;
	else
		filename = new string(name);
}
