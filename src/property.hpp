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


#include <string>
#include <list>


class property {
	public:
		typedef long long int signed_type;
		typedef unsigned long long int unsigned_type;
		typedef long double floating_type;
		typedef std::list<signed_type> signed_list_type;
		typedef std::list<unsigned_type> unsigned_list_type;
		typedef std::list<floating_type> floating_list_type;
		typedef std::list<bool> boolean_list_type;

	private:
		std::string raw_value;
		//  Pointers: computed yet?
		bool * boolean_value = nullptr;
		signed_type * int_signed_value = nullptr;
		unsigned_type * int_unsigned_value = nullptr;
		floating_type * floating_value = nullptr;
		signed_list_type * signed_list_value = nullptr;
		unsigned_list_type * unsigned_list_value = nullptr;
		floating_list_type * floating_list_value = nullptr;
		boolean_list_type * boolean_list_value = nullptr;


		void compute_integer();

		void compute_floating();

		void compute_logical();

		void compute_list();

	public:
		const signed_type & integer();

		const unsigned_type & unsigned_integer();

		const floating_type & floating();

		const bool & boolean();

		inline const std::string & textual() const {
			return raw_value;
		}

		const signed_list_type & integer_list();

		const unsigned_list_type & unsigned_integer_list();

		const floating_list_type & floating_list();

		const boolean_list_type & boolean_list();


		property(const std::string & val);

		~property();
};


#endif // PROPERTY_HPP
