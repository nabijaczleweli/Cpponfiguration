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


TEST_CASE("property - swap") {
	SECTION("textual() is swapped", "[property] [swap]") {
		const auto first_txt = "KHQuvrWXJHUuonvIrEFVaCPEI";
		const auto second_txt = "slEedElkHCCUQjvJrKSJM";

		property first(first_txt);
		property second(second_txt);

		REQUIRE_NOTHROW(swap(first, second));

		REQUIRE(first.textual() == second_txt);
		REQUIRE(second.textual() == first_txt);
	}

	SECTION("comment is swapped", "[property] [swap]") {
		const auto first_cmt = "vxmEiZTHDYDalccawXLwzrprhaxw";
		const auto second_cmt = "WTevmstsxQAhFyPYLpgRdeIbFjZiic";

		property first("", first_cmt);
		property second("", second_cmt);

		REQUIRE_NOTHROW(swap(first, second));

		REQUIRE(first.comment == second_cmt);
		REQUIRE(second.comment == first_cmt);
	}
}
