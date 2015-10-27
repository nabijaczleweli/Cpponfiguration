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


#include "util/salt.hpp"
#include "catch.hpp"


using namespace std;
using namespace cpponfig;


TEST_CASE("salt") {
	SECTION("randomize_salt") {
		REQUIRE(salt{} != salt{});

		const auto randomize_salt = salt::randomize_salt;
		salt::randomize_salt      = false;

		REQUIRE(salt{} == salt{});

		salt::randomize_salt = randomize_salt;

		REQUIRE(salt{} != salt{});
	}

	for(int i = 0; i < 10; ++i)
		REQUIRE(salt{} != salt{});

	vector<salt> salts(1000000);
	REQUIRE(accumulate(salts.begin(), salts.end(), size_t(0)) == Approx(numeric_limits<size_t>::max() / 2).epsilon(numeric_limits<size_t>::max() / salts.size()));
}
