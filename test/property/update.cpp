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
#include <random>


#define FUNDAMENTAL_TEST(func, value)                          \
	do {                                                         \
		property p("");                                            \
		p.func() = value;                                          \
		p.update_from_##func();                                    \
		INFO(p.textual())                                          \
		REQUIRE(property(p.textual()).func() == Approx(p.func())); \
	} while(false)

#define FUNDAMENTAL_MINMAX_TESTS(func, type)                              \
	do {                                                                    \
		FUNDAMENTAL_TEST(func, numeric_limits<property::type##_type>::min()); \
		FUNDAMENTAL_TEST(func, property::type##_type{0});                     \
		FUNDAMENTAL_TEST(func, numeric_limits<property::type##_type>::max()); \
	} while(false)

#define FUNDAMENTAL_TESTER(func, type)                                                                  \
	SECTION(#func) {                                                                                      \
		uniform_int_distribution<property::type##_type> dist(numeric_limits<property::type##_type>::min()); \
                                                                                                        \
		FUNDAMENTAL_MINMAX_TESTS(func, type);                                                               \
		for(auto i = 0u; i < 10; ++i) {                                                                     \
			const auto totest = dist(rng);                                                                    \
			FUNDAMENTAL_TEST(func, totest);                                                                   \
		}                                                                                                   \
	}

#define LIST_TEST(func, value)                                       \
	do {                                                               \
		property p("");                                                  \
		p.func##_list() = value;                                         \
		p.update_from_##func##_list();                                   \
		REQUIRE(property(p.textual()).func##_list() == p.func##_list()); \
	} while(false)

#define FUNDAMENTAL_SIZE_TESTS(func, type)                  \
	do {                                                      \
		LIST_TEST(func, property::type##_list_type{});          \
		LIST_TEST(func, property::type##_list_type(10000, {})); \
	} while(false)

#define LIST_TESTER_DIST(func, type, dist)                                 \
	SECTION(#func "_list") {                                                 \
		FUNDAMENTAL_SIZE_TESTS(func, type);                                    \
                                                                           \
		for(auto i = 0u; i < 10; ++i) {                                        \
			property::type##_list_type totest(1000, {});                         \
			generate(totest.begin(), totest.end(), [&]() { return dist(rng); }); \
			LIST_TEST(func, totest);                                             \
		}                                                                      \
	}

#define LIST_TESTER(func, type)                                                                         \
	{                                                                                            \
		uniform_int_distribution<property::type##_type> dist(numeric_limits<property::type##_type>::min()); \
		LIST_TESTER_DIST(func, type, dist)                                                                  \
	}


using namespace std;
using namespace cpponfig;


TEST_CASE("property - update") {
	mt19937 rng;

	SECTION("fundamental", "[property] [update]") {
		SECTION("boolean") {
			FUNDAMENTAL_TEST(boolean, true);
			FUNDAMENTAL_TEST(boolean, false);
		}

		FUNDAMENTAL_TESTER(integer, signed)
		FUNDAMENTAL_TESTER(unsigned_integer, unsigned)
		SECTION("floating") {
			for(property::floating_type i = -1; i <= 1; i += .01)
				FUNDAMENTAL_TEST(floating, i);
		}
	}

	SECTION("list", "[property] [update]") {
		{
			bernoulli_distribution dist;
			LIST_TESTER_DIST(boolean, boolean, dist)
		}
		LIST_TESTER(integer, signed)
		LIST_TESTER(unsigned_integer, unsigned);
		{
			auto dist = [dst = uniform_int_distribution<unsigned long int>(numeric_limits<unsigned long int>::min())](auto & randgen) mutable {
				return .01l * dst(randgen);
			};
			LIST_TESTER_DIST(floating, floating, dist)
		}
	}
}
