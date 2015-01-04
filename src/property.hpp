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


class property {
	public:
		typedef long long int signed_type;
		typedef unsigned long long int unsigned_type;
		typedef long double floating_type;

	private:
		std::string raw_value;
		//  Pointers: computed yet?
		bool * boolean_value = nullptr;
		signed_type * int_signed_value = nullptr;
		unsigned_type * int_unsigned_value = nullptr;
		floating_type * floating_value = nullptr;


		template<class T>
		inline void re_create(T *& ptr) const {
			if(ptr)
				delete ptr;
			ptr = new T;
		}

		void compute_integer();

		void compute_floating();

		void compute_logical();

	public:
		const signed_type & integer();

		const unsigned_type & unsigned_integer();

		const floating_type & floating();

		const bool & boolean();

		inline const std::string & textual() const {
			return raw_value;
		}


		property(const std::string & val);

		~property();
};


#endif // PROPERTY_HPP
