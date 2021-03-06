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
#include "configuration_category.hpp"
#include "util/salt.hpp"
#include <vector>
#include <iosfwd>
#include <string>
#include <map>


namespace cpponfiguration {
	class configuration {
		friend struct std::hash<configuration>;

	protected:
		std::map<std::string, configuration_category> categories;
		std::string filename;


		void load_properties(std::istream & from);
		void save_properties(std::ostream & to) const;

		size_t hash_code() const;

	public:
		enum class datetime_mode : unsigned char { none, gmt, local };
		using dt_m = datetime_mode;


		static bool load_on_construction;
		static bool save_on_destruction;
		static char comment_character;
		static char assignment_character;
		static char category_character;
		static char category_start_character;
		static char category_end_character;
		static dt_m datetime_footer_type;


		static std::pair<std::string, std::string> property_path(const std::string & name);


		std::vector<std::string> sof_comments;


		configuration() = default;
		explicit configuration(const std::string & name);
		configuration(const configuration & other) = default;
		configuration(configuration && other) = default;

		~configuration();

		void swap(configuration & other) noexcept;

		configuration & operator=(const configuration & other) = default;
		configuration & operator=(configuration && other) = default;
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

		std::string & comment(const std::string & cat);

		void rename(const std::string & name);
		bool empty() const;
	};
}

namespace cpponfig = cpponfiguration;


cpponfig::configuration operator+(const cpponfig::configuration & lhs, const cpponfig::configuration & rhs);
cpponfig::configuration operator-(const cpponfig::configuration & lhs, const cpponfig::configuration & rhs);


namespace std {
	void swap(cpponfiguration::configuration & lhs, cpponfiguration::configuration & rhs) noexcept;

	// All hex numbers here are primes
	template <class T0, class T1>
	struct hash<pair<T0, T1>> {
		size_t operator()(const pair<T0, T1> & pr) const {
			static const cpponfig::salt slt{};
			static const hash<T0> T0_hash{};
			static const hash<T1> T1_hash{};

			return 0x2E48EDC9 ^ slt ^ T0_hash(pr.first) ^ T1_hash(pr.second);
		}
	};

	template <>
	struct hash<cpponfiguration::configuration> {
		inline size_t operator()(const cpponfiguration::configuration & tohash) const {
			return tohash.hash_code();
		}
	};
}


#endif  // CONFIGURATION_HPP
