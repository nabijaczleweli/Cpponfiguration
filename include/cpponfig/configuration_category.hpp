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


#pragma once
#ifndef CONFIGURATION_CATEGORY_HPP
#define CONFIGURATION_CATEGORY_HPP


#include "property.hpp"
#include <iosfwd>
#include <string>
#include <map>


namespace cpponfiguration {
	class configuration_category {
		friend struct std::hash<configuration_category>;

	protected:
		std::map<std::string, property> properties;


		size_t hash_code() const;

	public:
		std::string comment;


		configuration_category() = default;
		template <class Iterator>
		configuration_category(Iterator begin, Iterator end)
		      : properties(begin, end) {}
		template <class Iterator>
		configuration_category(const std::string & cmt, Iterator begin, Iterator end)
		      : properties(begin, end), comment(cmt) {}
		configuration_category(const std::string & comment);
		configuration_category(const configuration_category & other) = default;
		configuration_category(configuration_category && other) = default;

		~configuration_category() = default;

		void swap(configuration_category & other) noexcept;

		configuration_category & operator=(const configuration_category & other) = default;
		configuration_category & operator=(configuration_category && other) = default;
		configuration_category & operator+=(const configuration_category & other);
		configuration_category & operator-=(const configuration_category & other);

		void load(std::istream & stream);
		void save(std::ostream & stream, const std::string & name) const;

		property & get(const std::string & key, const property & default_value);
		void remove(const std::string & key);
		bool contains(const std::string & key) const;

		bool empty() const;
	};
}

namespace cpponfig = cpponfiguration;


cpponfig::configuration_category operator+(const cpponfig::configuration_category & lhs, const cpponfig::configuration_category & rhs);
cpponfig::configuration_category operator-(const cpponfig::configuration_category & lhs, const cpponfig::configuration_category & rhs);


namespace std {
	void swap(cpponfiguration::configuration_category & lhs, cpponfiguration::configuration_category & rhs) noexcept;

	template <>
	struct hash<cpponfiguration::configuration_category> {
		inline size_t operator()(const cpponfiguration::configuration_category & tohash) const {
			return tohash.hash_code();
		}
	};
}


#endif  // CONFIGURATION_CATEGORY_HPP
