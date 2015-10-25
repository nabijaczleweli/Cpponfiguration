// The MIT License (MIT)

// Copyright (c) 2015 nabijaczleweli

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.


#include "property.hpp"
#include "catch.hpp"


#define FUNDAMENTAL_TEST(func, value) REQUIRE(property(to_string(value)).func() == Approx(value));

#define FUNDAMENTAL_MINMAX_TESTS(func, type)                              \
	do {                                                                    \
		FUNDAMENTAL_TEST(func, numeric_limits<property::type##_type>::min()); \
		FUNDAMENTAL_TEST(func, property::type##_type{0});                     \
		FUNDAMENTAL_TEST(func, numeric_limits<property::type##_type>::max()); \
	} while(false)

#define FUNDAMENTAL_TESTER_DIST_LOOP(func, type, dist) \
	for(auto i = 0u; i < 10; ++i) {                      \
		const auto totest = dist(rng);                     \
		FUNDAMENTAL_TEST(func, totest);                    \
	}

#define FUNDAMENTAL_TESTER_DIST(func, type, dist)  \
	SECTION(#func) {                                 \
		FUNDAMENTAL_MINMAX_TESTS(func, type);          \
		FUNDAMENTAL_TESTER_DIST_LOOP(func, type, dist) \
	}

#define FUNDAMENTAL_TESTER(func, type)                                                                  \
	{                                                                                                     \
		uniform_int_distribution<property::type##_type> dist(numeric_limits<property::type##_type>::min()); \
		FUNDAMENTAL_TESTER_DIST(func, type, dist)                                                           \
	}


using namespace std;
using namespace cpponfig;


TEST_CASE("property - getter") {
	mt19937 rng;

	SECTION("fundamental", "[property] [getter]") {
		SECTION("boolean") {
			for(auto tru : {"true", "1"})
				REQUIRE(property(tru).boolean() == true);
			for(auto fls : {"false", "0"})
				REQUIRE(property(fls).boolean() == false);
		}

		FUNDAMENTAL_TESTER(integer, signed)
		FUNDAMENTAL_TESTER(unsigned_integer, unsigned);
		{
			auto dist = [dst = uniform_int_distribution<unsigned long int>(numeric_limits<unsigned long int>::min())](auto & randgen) mutable {
				return .01l * dst(randgen);
			};
			FUNDAMENTAL_TEST(floating, numeric_limits<property::floating_type>::min());
			FUNDAMENTAL_TEST(floating, 0.);
			FUNDAMENTAL_TESTER_DIST_LOOP(floating, floating, dist)
		}
	}
}
