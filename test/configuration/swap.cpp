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


#include "configuration.hpp"
#include "catch.hpp"


using namespace std;
using namespace cpponfig;


TEST_CASE("configuration - swap") {
	SECTION("Properties are swapped", "[configuration] [swap]") {
		configuration first, second;
		first.get("first", "one");
		first.get("a:first", "a:one");
		second.get("second", "two");
		second.get("b:second", "b:two");

		REQUIRE_NOTHROW(swap(first, second));

		REQUIRE(first.get("second").textual() == "two");
		REQUIRE(first.get("b:second").textual() == "b:two");
		REQUIRE(second.get("first").textual() == "one");
		REQUIRE(second.get("a:first").textual() == "a:one");
	}

	SECTION("SOF comments are swapped", "[configuration] [swap]") {
		configuration first, second;
		first.sof_comments  = {"first", "one"};
		second.sof_comments = {"second", "two"};

		REQUIRE_NOTHROW(swap(first, second));

		REQUIRE(first.sof_comments == (vector<string>{"second", "two"}));
		REQUIRE(second.sof_comments == (vector<string>{"first", "one"}));
	}
}
