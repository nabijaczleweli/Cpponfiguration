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
#include <istream>
#include <fstream>
#include <iostream>


using namespace std;


char configuration::comment_character = '#';
char configuration::assignment_character = '=';


configuration::configuration() : configuration(nullptr) {}
configuration::configuration(string * name) : filename(name) {}
configuration::configuration(const string & name) : configuration(new string(name)) {}

configuration::~configuration() {
	if(filename) {
		delete filename;
		filename = nullptr;
	}
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

void configuration::save_properties(std::ostream & to) {
	for(const auto & pr : properties)
		to << pr.first << '=' << pr.second.textual() << (pr.second.comment.empty() ? "" : string(" ") + comment_character + " " + pr.second.comment) << "\n\n";
}

bool configuration::load() {
	if(filename) {
		ifstream file(*filename);
		if(file.is_open()) {
			load_properties(file);
			return true;
		}
	}
	return false;
}

bool configuration::load(const string & name) {
	if(filename)
		delete filename;
	filename = new string(name);
	return load();
}

bool configuration::load(istream & stream) {
	if(stream) {
		load_properties(stream);
		return true;
	}
	return false;
}

bool configuration::save() {
	if(filename) {
		ofstream file(*filename);
		if(file.is_open()) {
			save_properties(file);
			return true;
		}
	}
	return false;
}

bool configuration::save(const string & name) {
	if(filename)
		delete filename;
	filename = new string(name);
	return save();
}

bool configuration::save(ostream & stream) {
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

bool configuration::contains(const std::string & key) {
	return properties.find(key) != properties.end();
}
