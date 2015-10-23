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


using namespace std;
using namespace cpponfig;


static const property empty_prop("");


TEST_CASE("configuration_category - swap") {
	SECTION("Properties are swapped", "[configuration_category] [swap]") {
		configuration_category first, second;
		first.get("first", {"one"});
		second.get("second", {"two"});

		REQUIRE_NOTHROW(swap(first, second));

		REQUIRE(first.get("second", empty_prop).textual() == "two");
		REQUIRE(second.get("first", empty_prop).textual() == "one");
	}

	SECTION("Comment is swapped", "[configuration_category] [swap]") {
		configuration_category first, second;
		first.comment  = "first";
		second.comment = "second";

		REQUIRE_NOTHROW(swap(first, second));

		REQUIRE(first.comment == "second");
		REQUIRE(second.comment == "first");
	}
}
