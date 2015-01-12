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


#pragma once
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP


#include "property.hpp"
#include <iosfwd>
#include <string>
#include <unordered_map>


class configuration {
	protected:
		std::unordered_map<std::string, property> properties;
		std::string * filename = nullptr;

		configuration(std::string * name);

		void load_properties(std::istream & from);
		void save_properties(std::ostream & to);

	public:
		static char comment_character;
		static char assignment_character;

		configuration();
		configuration(const std::string & name);

		~configuration();

		bool load();
		bool load(const std::string & name);
		bool load(std::istream & stream);

		bool save();
		bool save(const std::string & name);
		bool save(std::ostream & stream);

		property & get(const std::string & key, const std::string & default_value);
		inline property & get(const std::string & key) { return get(key, ""); }

		bool contains(const std::string & key);
};


#endif  // CONFIGURATION_HPP
