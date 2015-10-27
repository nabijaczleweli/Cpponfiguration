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


#include "hashable.hpp"
#include "catch.hpp"
#include <random>


using namespace std;
using namespace cpponfig;


struct testhash : public hashable<testhash> {
	size_t val;

	constexpr testhash(size_t v) : val(v) {}

private:
	inline virtual size_t hash_code() const {
		return val;
	}
};


TEST_CASE("hashable") {
	mt19937 rng;
	uniform_int_distribution<size_t> dist(numeric_limits<size_t>::min());

	const hash<testhash> hasher{};

	for(auto i : {numeric_limits<size_t>::min(), numeric_limits<size_t>::max()})
		REQUIRE(hasher(testhash{i}) == i);
	for(int i = 0; i < 10; ++i) {
		const auto totest = dist(rng);
		REQUIRE(hasher(testhash{totest}) == totest);
	}
}
