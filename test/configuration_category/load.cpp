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


#include "configuration_category.hpp"
#include "catch.hpp"
#include <fstream>


using namespace std;
using namespace cpponfig;


static const property empty_prop("");


TEST_CASE("configuration_category - load") {
	SECTION("Reads from empty istream", "[configuration_category] [load]") {
		istringstream ss;

		configuration_category c;
		c.load(ss);

		CHECK(c.empty());
	}

	SECTION("Reads from nonempty istream", "[configuration_category] [load]") {
		istringstream ss(" \tasdf=fdsa # assa\n"
		                 "}");

		configuration_category c;
		c.load(ss);

		REQUIRE(c.get("asdf", empty_prop) == property("fdsa", "assa"));
	}

	SECTION("Doesn't read from failed istream", "[configuration_category] [load]") {
		istringstream ss;
		ss.setstate(ios::failbit);

		configuration_category c;
		c.load(ss);

		CHECK(c.empty());
	}
}
