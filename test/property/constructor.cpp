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


TEST_CASE("property - constructor") {
	SECTION("comment is initialized", "[property] [constructor]") {
		const auto cmt = "VvSbMOEYBzjqwPymNZ";

		property c("", cmt);

		REQUIRE(c.comment == cmt);
	}

	SECTION("comment is empty by default", "[property] [constructor]") {
		property c("");

		CHECK(c.comment.empty());
	}

	SECTION("textual() is initialized", "[property] [constructor]") {
		const auto txt = "FdvrfpuiUJYIEpqYXaxLwcCWAUGTsQaKji";

		property c(txt);

		REQUIRE(c.textual() == txt);
	}

	SECTION("textual() is copied", "[property] [constructor]") {
		property full("DLWrQUnreELUDXQTlakEqx");
		property empty(full);

		REQUIRE(full.textual() == empty.textual());
	}

	SECTION("comment is copied", "[property] [constructor]") {
		property full("", "cdpPEBCgaiXpnYShJRd");
		property empty(full);

		REQUIRE(empty.comment == full.comment);
	}

	SECTION("textual() is moved", "[property] [constructor]") {
		const auto txt = "DLWrQUnreELUDXQTlakEqx";

		property full(txt);
		property empty(move(full));

		CHECK(full.textual().empty());
		REQUIRE(empty.textual() == txt);
	}

	SECTION("comment is moved", "[property] [constructor]") {
		const auto cmt = "VvSbMOEYBzjqwPymNZ";

		property full("", cmt);
		property empty(move(full));

		CHECK(full.comment.empty());
		REQUIRE(empty.comment == cmt);
	}
}
