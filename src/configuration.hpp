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
#ifndef CONFIGURATION_HPP
#define CONFIGURATION_HPP


#include "property.hpp"
#include "swappable.hpp"
#include "hashable.hpp"
#include "util/salt.hpp"
#include <iosfwd>
#include <string>
#include <unordered_map>
#include <list>


namespace cpponfiguration {
	class configuration : swappable<configuration>, public hashable<configuration> {
		protected:
			std::unordered_map<std::string, property> properties;
			std::list<std::string> sof_comments;
			std::string * filename = nullptr;

			configuration(std::string * name);

			void load_properties(std::istream & from);
			void save_properties(std::ostream & to) const;
			bool save(const std::string * name) const;

			virtual size_t hash_code() const override;

		public:
			enum class datetime_mode : unsigned char {none, gmt, local};
			typedef datetime_mode dt_m;


			static char comment_character;
			static char assignment_character;
			static bool force_create_files;
			static dt_m add_datetime_to_footer;


			configuration();
			configuration(const std::string & name);
			configuration(const configuration & other);
			configuration(configuration && other);

			~configuration();

			virtual void swap(configuration & other) override;

			configuration & operator=(const configuration & other);
			configuration & operator+=(const configuration & other);
			configuration & operator-=(const configuration & other);

			bool load();
			bool load(const std::string & name);
			bool load(std::istream & stream);

			bool save() const;
			bool save(const std::string & name) const;
			bool save(std::ostream & stream) const;

			property & get(const std::string & key, const std::string & default_value = "");
			property & get(const std::string & key, const property & default_value);
			void remove(const std::string & key);
			bool contains(const std::string & key) const;

			void rename(const std::string & name);
			bool empty();
	};
}

namespace cpponfig = cpponfiguration;


cpponfig::configuration operator+(const cpponfig::configuration & lhs, const cpponfig::configuration & rhs);
cpponfig::configuration operator-(const cpponfig::configuration & lhs, const cpponfig::configuration & rhs);


namespace std {
	// All hex numbers here are primes
	template<class T0, class T1>
	struct hash<pair<T0, T1>> {
		size_t operator()(const pair<T0, T1> & pr) const {
			static const cpponfig::salt slt;
			static const hash<T0> T0_hash;
			static const hash<T1> T1_hash;

			return 0x2E48EDC9 ^ slt ^ T0_hash(pr.first) ^ T1_hash(pr.second);
		}
	};
}


#endif  // CONFIGURATION_HPP
