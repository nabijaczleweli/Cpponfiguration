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
#include "swappable.hpp"
#include "hashable.hpp"
#include <iosfwd>
#include <string>
#include <map>


namespace cpponfiguration {
	class configuration_category : swappable<configuration_category>, public hashable<configuration_category> {
	protected:
		std::map<std::string, property> properties;


		virtual size_t hash_code() const override;

	public:
		configuration_category();
		template <class Iterator>
		configuration_category(const Iterator & begin, const Iterator & end) {
			properties.insert(begin, end);
		}
		configuration_category(const configuration_category & other);
		configuration_category(configuration_category && other);

		~configuration_category();

		virtual void swap(configuration_category & other) override;

		configuration_category & operator=(const configuration_category & other);
		configuration_category & operator+=(const configuration_category & other);
		configuration_category & operator-=(const configuration_category & other);

		void load(std::istream & stream);
		void save(std::ostream & stream) const;

		property & get(const std::string & key, const property & default_value);
		void remove(const std::string & key);
		bool contains(const std::string & key) const;

		bool empty() const;
	};
}

namespace cpponfig = cpponfiguration;


cpponfig::configuration_category operator+(const cpponfig::configuration_category & lhs, const cpponfig::configuration_category & rhs);
cpponfig::configuration_category operator-(const cpponfig::configuration_category & lhs, const cpponfig::configuration_category & rhs);


#endif  // CONFIGURATION_CATEGORY_HPP
