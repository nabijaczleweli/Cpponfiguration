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


TEST_CASE("configuration_category - operator") {
	SECTION("=", "[configuration_category] [operator]") {
		SECTION("comment is copied") {
			configuration_category empty, full;
			full.comment = "KCmitUvyF";

			empty = full;
			REQUIRE(empty.comment == full.comment);
		}

		SECTION("Properties are copied") {
			configuration_category full, empty;
			full.get("asdf", {"fdsa", "KtqbeSZYFgNZr"});

			empty = full;
			CHECK_FALSE(empty.empty());
			REQUIRE(empty.get("asdf", empty_prop).textual() == "fdsa");
		}

		SECTION("Comments are moved") {
			const string cmt = "KXGToLlJknS";

			configuration_category empty, full;
			full.comment = cmt;

			empty = move(full);
			REQUIRE(empty.comment == cmt);
		}

		SECTION("Properties are moved") {
			configuration_category full, empty;
			full.get("asdf", {"fdsa", "HxyIKVtBX"});

			empty = move(full);
			CHECK(full.empty());
			CHECK_FALSE(empty.empty());
			REQUIRE(empty.get("asdf", empty_prop).textual() == "fdsa");
		}
	}

	SECTION("+=", "[configuration_category] [operator]") {
		SECTION("Properties are appended") {
			configuration_category first, second;
			first.get("first", {"one", "xbePDebxxp"});
			second.get("second", {"two", "mLiVwOLDxdTcJGpOrY"});

			first += second;
			REQUIRE(first.get("first", empty_prop).textual() == "one");
			REQUIRE(first.get("second", empty_prop).textual() == "two");
		}
	}

	SECTION("-=", "[configuration_category] [operator]") {
		SECTION("Properties are removed") {
			configuration_category first, second;
			first.get("first", {"one", "xHvSwglsdYh"});
			second.get("first", {"two", "NGVjAUy"});

			first -= second;
			REQUIRE(first.get("first", empty_prop) == empty_prop);
		}
	}

	SECTION("+", "[configuration_category] [operator]") {
		SECTION("Properties are appended") {
			configuration_category first, second;
			first.get("first", {"one", "uastebHibuaYQJujBq"});
			second.get("second", {"two", "kakFkLvEAcQ"});

			auto added = first + second;
			REQUIRE(added.get("first", empty_prop).textual() == "one");
			REQUIRE(added.get("second", empty_prop).textual() == "two");
		}
	}

	SECTION("-", "[configuration_category] [operator]") {
		SECTION("Properties are appended") {
			configuration_category first, second;
			first.get("first", {"one", "zypU"});
			second.get("first", {"two", "rJcGRYWcSiEjObo"});

			auto substracted = first - second;
			REQUIRE(substracted.get("first", empty_prop) == empty_prop);
		}
	}
}
