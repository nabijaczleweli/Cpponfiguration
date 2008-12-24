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


using namespace std;
using namespace cpponfig;


TEST_CASE("property - operator") {
	SECTION("=", "[property] [operator]") {
		SECTION("comment is copied") {
			property empty("");
			property full("", "KCmitUvyF");

			empty = full;
			REQUIRE(empty.comment == full.comment);
		}

		SECTION("Value is copied") {
			property empty("");
			property full("vuIcexpuqFIgKKxO");

			empty = full;
			REQUIRE(empty.textual() == full.textual());
		}

		SECTION("comment is moved") {
			const auto cmt = "xEGJleLdPOU";

			property empty("");
			property full("", cmt);

			empty = move(full);
			REQUIRE(empty.comment == cmt);
		}

		SECTION("Value is moved") {
			const auto txt = "CaxGKujmbUkDzJcRIsV";

			property empty("");
			property full(txt);

			empty = move(full);
			REQUIRE(empty.textual() == txt);
		}
	}

	SECTION("==/!=", "[property] [operator]") {
		SECTION("comment is considered for inequality") {
			property first("", "MqghaDwBgzMOooIcMusLdu");
			property second("", "KCmitUvyFdqFBicYtmn");

			REQUIRE(first != second);
			REQUIRE_FALSE(first == second);
		}

		SECTION("Value is considered for inequality") {
			property first("xFwstFIXtrgouVZshrWJqVZOFGBggBjKUOlY");
			property second("LlWiwybCRjlhgaDDfjkIyrwtxvsJVN");

			REQUIRE(first.textual() != second.textual());
			REQUIRE_FALSE(first.textual() == second.textual());
		}

		SECTION("comment is considered for equality") {
			const auto cmt = "istJNTeeHDvqKymlPowpHxFuzS";

			property first("", cmt);
			property second("", cmt);

			REQUIRE(first == second);
			REQUIRE_FALSE(first != second);
		}

		SECTION("Value is considered for equality") {
			const auto txt = "EmQviyCvxrnCmpXTfrYsUXSeldKClFgkUXzkHXZ";

			property first(txt);
			property second(txt);

			REQUIRE(first == second);
			REQUIRE_FALSE(first != second);
		}
	}
}
