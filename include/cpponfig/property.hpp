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
#ifndef PROPERTY_HPP
#define PROPERTY_HPP


#include <limits>
#include <memory>
#include <string>
#include <vector>


namespace cpponfiguration {
	class property {
		friend struct std::hash<property>;

	public:
		using boolean_type       = bool;
		using signed_type        = long long int;
		using unsigned_type      = unsigned long long int;
		using floating_type      = long double;
		using signed_list_type   = std::vector<signed_type>;
		using unsigned_list_type = std::vector<unsigned_type>;
		using floating_list_type = std::vector<floating_type>;
		using boolean_list_type  = std::vector<bool>;

	private:
		std::string raw_value;
		std::unique_ptr<boolean_type> boolean_value;
		std::unique_ptr<signed_type> signed_value;
		std::unique_ptr<unsigned_type> unsigned_value;
		std::unique_ptr<floating_type> floating_value;
		std::unique_ptr<boolean_list_type> boolean_list_value;
		std::unique_ptr<signed_list_type> signed_list_value;
		std::unique_ptr<unsigned_list_type> unsigned_list_value;
		std::unique_ptr<floating_list_type> floating_list_value;


		void compute_boolean();
		void compute_integer();
		void compute_floating();
		void compute_list();

		void clear_except(const void * except);
		template <class T>
		inline void clear_except(const std::unique_ptr<T> & except) {
			clear_except(except ? std::addressof(except) : nullptr);
		}

		size_t hash_code() const;

	public:
		static unsigned int floating_precision;


		std::string comment;


		bool & boolean();
		signed_type & integer();
		unsigned_type & unsigned_integer();
		floating_type & floating();
		inline const std::string & textual() const {
			return raw_value;
		}
		inline std::string & textual() {
			return raw_value;
		}
		boolean_list_type & boolean_list();
		signed_list_type & integer_list();
		unsigned_list_type & unsigned_integer_list();
		floating_list_type & floating_list();


		void update_from_boolean();
		void update_from_integer();
		void update_from_unsigned_integer();
		void update_from_floating();
		void update_from_textual();
		void update_from_boolean_list();
		void update_from_integer_list();
		void update_from_unsigned_integer_list();
		void update_from_floating_list();


		property & operator=(const property & other);
		property & operator=(property && other) = default;

		void swap(property & other) noexcept;

		property(const std::string & val, const std::string & cmt = "");
		property(const property & other);
		property(property && other);
	};
}

namespace cpponfig = cpponfiguration;


bool operator==(const cpponfig::property & lhs, const cpponfig::property & rhs);
bool operator!=(const cpponfig::property & lhs, const cpponfig::property & rhs);


namespace std {
	void swap(cpponfiguration::property & lhs, cpponfiguration::property & rhs) noexcept;

	template <>
	struct hash<cpponfiguration::property> {
		inline size_t operator()(const cpponfiguration::property & tohash) const {
			return tohash.hash_code();
		}
	};
}


#endif  // PROPERTY_HPP
