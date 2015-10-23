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


TEST_CASE("configuration - operator") {
	SECTION("=", "[configuration] [operator]") {
		SECTION("Comments are copied") {
			configuration empty, full;
			full.sof_comments = {"asdf", "fdsa"};

			empty = full;
			CHECK_FALSE(empty.empty());
			REQUIRE(empty.sof_comments == full.sof_comments);
		}

		SECTION("Properties are copied") {
			configuration full, empty;
			full.get("asdf", "fdsa");
			full.get("a:asdf", "a:fdsa");

			empty = full;
			CHECK_FALSE(empty.empty());
			REQUIRE(empty.get("asdf").textual() == "fdsa");
			REQUIRE(empty.get("a:asdf").textual() == "a:fdsa");
		}

		SECTION("Comments are moved") {
			const vector<string> cmts({"asdf", "fdsa"});

			configuration empty, full;
			full.sof_comments = cmts;

			empty = move(full);
			CHECK(full.empty());
			CHECK_FALSE(empty.empty());
			REQUIRE(empty.sof_comments == cmts);
		}

		SECTION("Properties are moved") {
			configuration full, empty;
			full.get("asdf", "fdsa");
			full.get("a:asdf", "a:fdsa");

			empty = move(full);
			CHECK(full.empty());
			CHECK_FALSE(empty.empty());
			REQUIRE(empty.get("asdf").textual() == "fdsa");
			REQUIRE(empty.get("a:asdf").textual() == "a:fdsa");
		}
	}

	SECTION("+=", "[configuration] [operator]") {
		SECTION("Comments are appended") {
			configuration first, second;
			first.sof_comments  = {"first", "one"};
			second.sof_comments = {"second", "two"};

			first += second;
			REQUIRE(first.sof_comments == (vector<string>{"first", "one", "second", "two"}));
		}

		SECTION("Properties are appended") {
			configuration first, second;
			first.get("first", "one");
			first.get("a:first", "a:one");
			second.get("second", "two");
			second.get("b:second", "b:two");

			first += second;
			REQUIRE(first.get("first").textual() == "one");
			REQUIRE(first.get("a:first").textual() == "a:one");
			REQUIRE(first.get("second").textual() == "two");
			REQUIRE(first.get("b:second").textual() == "b:two");
		}
	}

	SECTION("-=", "[configuration] [operator]") {
		SECTION("Comments are removed") {
			configuration first, second;
			first.sof_comments  = {"first", "one"};
			second.sof_comments = {"first", "two"};

			first -= second;
			REQUIRE(first.sof_comments == (vector<string>{"one"}));
		}

		SECTION("Properties are removed") {
			configuration first, second;
			first.get("first", "one");
			first.get("a:first", "a:one");
			second.get("first", "two");
			second.get("a:first", "a:two");

			first -= second;
			REQUIRE(first.get("first").textual() == "");
			REQUIRE(first.get("a:first").textual() == "");
		}
	}

	SECTION("+", "[configuration] [operator]") {
		SECTION("Comments are appended") {
			configuration first, second;
			first.sof_comments  = {"first", "one"};
			second.sof_comments = {"second", "two"};

			auto added = first + second;
			REQUIRE(added.sof_comments == (vector<string>{"first", "one", "second", "two"}));
		}

		SECTION("Properties are appended") {
			configuration first, second;
			first.get("first", "one");
			first.get("a:first", "a:one");
			second.get("second", "two");
			second.get("b:second", "b:two");

			auto added = first + second;
			REQUIRE(added.get("first").textual() == "one");
			REQUIRE(added.get("a:first").textual() == "a:one");
			REQUIRE(added.get("second").textual() == "two");
			REQUIRE(added.get("b:second").textual() == "b:two");
		}
	}

	SECTION("-", "[configuration] [operator]") {
		SECTION("Comments are appended") {
			configuration first, second;
			first.sof_comments  = {"first", "one"};
			second.sof_comments = {"first", "two"};

			auto substracted = first - second;
			REQUIRE(substracted.sof_comments == (vector<string>{"one"}));
		}

		SECTION("Properties are appended") {
			configuration first, second;
			first.get("first", "one");
			first.get("a:first", "a:one");
			second.get("first", "two");
			second.get("a:first", "a:two");

			auto substracted = first - second;
			REQUIRE(substracted.get("first").textual() == "");
			REQUIRE(substracted.get("a:first").textual() == "");
		}
	}
}
