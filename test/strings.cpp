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


#include "util/strings.hpp"
#include "catch.hpp"

#define TESTSTR "A S\tD\t F"


using namespace std;
using namespace cpponfig::util;


TEST_CASE("strings") {
	SECTION("&") {
		SECTION("ltrim") {
			string s = " \t\t " TESTSTR " \t ";
			REQUIRE(ltrim(s) == TESTSTR " \t ");
			REQUIRE(s == TESTSTR " \t ");
		}

		SECTION("rtrim") {
			string s = " \t " TESTSTR "\t\t   \t \t ";
			REQUIRE(rtrim(s) == " \t " TESTSTR);
			REQUIRE(s == " \t " TESTSTR);
		}

		SECTION("trim") {
			string s = " \t\t\t    \t " TESTSTR "\t\t   \t \t ";
			REQUIRE(trim(s) == TESTSTR);
			REQUIRE(s == TESTSTR);
		}
	}

	SECTION("&&") {
		SECTION("ltrim") {
			REQUIRE(ltrim(" \t\t " TESTSTR " \t ") == TESTSTR " \t ");
		}

		SECTION("rtrim") {
			REQUIRE(rtrim(" \t " TESTSTR "\t\t   \t \t ") == " \t " TESTSTR);
		}

		SECTION("trim") {
			REQUIRE(trim(" \t\t\t    \t " TESTSTR "\t\t   \t \t ") == TESTSTR);
		}
	}

	SECTION("const&") {
		SECTION("ltrim") {
			const string s = " \t\t " TESTSTR " \t ";
			REQUIRE(ltrim(s) == TESTSTR " \t ");
		}

		SECTION("rtrim") {
			const string s = " \t " TESTSTR "\t\t   \t \t ";
			REQUIRE(rtrim(s) == " \t " TESTSTR);
		}

		SECTION("trim") {
			const string s = " \t\t\t    \t " TESTSTR "\t\t   \t \t ";
			REQUIRE(trim(s) == TESTSTR);
		}
	}
}
