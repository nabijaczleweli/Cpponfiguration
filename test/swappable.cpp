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


#include "swappable.hpp"
#include "catch.hpp"
#include <random>


using namespace std;
using namespace cpponfig;


struct testswap : public swappable<testswap> {
	size_t val;

	constexpr testswap(size_t v) : val(v) {}

private:
		inline virtual void swap(testswap & other) {
			std::swap(val, other.val);
		}
};


TEST_CASE("swappable") {
	mt19937 rng;
	uniform_int_distribution<size_t> dist(numeric_limits<size_t>::min());

	for(int i = 0; i < 10; ++i) {
		auto totest1 = dist(rng);
		auto totest2 = dist(rng);

		testswap test1 = totest1;
		testswap test2 = totest2;

		swap(test1, test2);
		REQUIRE(test1.val == totest2);
		REQUIRE(test2.val == totest1);
	}
}
