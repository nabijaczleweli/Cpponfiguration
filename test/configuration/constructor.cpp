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


TEST_CASE("configuration - constructor") {
	SECTION("Empty is default state", "[configuration] [constructor]") {
		configuration c;
		CHECK(c.empty());
		CHECK(c.sof_comments.empty());
	}

	SECTION("Empty after load from nonexistant file", "[configuration] [constructor]") {
		const auto save_on_destruction     = configuration::save_on_destruction;
		configuration::save_on_destruction = false;

		{
			configuration c("arEAODBbJZ.EufHuyVyrwbTRNMmEm");
			CHECK(c.empty());
			CHECK(c.sof_comments.empty());
		}

		configuration::save_on_destruction = save_on_destruction;
	}

	SECTION("Properties are copied", "[configuration] [constructor]") {
		configuration full;
		full.get("asdf", "fdsa");

		configuration empty(full);
		CHECK_FALSE(empty.empty());
		REQUIRE(empty.get("asdf").textual() == "fdsa");
	}

	SECTION("SOF comments are copied", "[configuration] [constructor]") {
		configuration full;
		full.sof_comments = {"asdf", "fdsa"};

		configuration empty(full);
		CHECK_FALSE(empty.empty());
		REQUIRE(empty.sof_comments == (vector<string>{"asdf", "fdsa"}));
	}

	SECTION("Properties are moved", "[configuration] [constructor]") {
		configuration full;
		full.get("asdf", "fdsa");

		configuration empty(move(full));
		CHECK(full.empty());
		CHECK_FALSE(empty.empty());
		REQUIRE(empty.get("asdf").textual() == "fdsa");
	}

	SECTION("SOF comments are moved", "[configuration] [constructor]") {
		configuration full;
		full.sof_comments = {"asdf", "fdsa"};

		configuration empty(move(full));
		CHECK(full.empty());
		CHECK_FALSE(empty.empty());
		REQUIRE(empty.sof_comments == (vector<string>{"asdf", "fdsa"}));
	}
}
