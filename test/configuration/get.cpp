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
#include "bandit/bandit.h"


using namespace std;
using namespace bandit;
using namespace cpponfig;


go_bandit([] {
	describe("configuration", [&] {
		describe("get", [&] {
			property testprop("WsAd", "DaSw");

			it("carries over simple defaults", [&] {
				configuration c;
				AssertThat(c.get("a").textual(), Is().EqualTo(""));
				AssertThat(c.get("b", "").textual(), Is().EqualTo(""));
				AssertThat(c.get("c", "AsDf").textual(), Is().EqualTo("AsDf"));
			});

			it("doesn't override with simple defaults provided", [&] {
				configuration c;
				c.get("a", "AsDf");
				AssertThat(c.get("a", "FdSa").textual(), Is().EqualTo("AsDf"));
			});

			it("carries over explicit defaults", [&] {
				configuration c;
				AssertThat(c.get("a", testprop), Is().EqualTo(testprop));
			});

			it("doesn't override with explicit defaults provided", [&] {
				configuration c;
				c.get("a", testprop);
				AssertThat(c.get("a", property("ASDFASDF", "DSAASD")), Is().EqualTo(testprop));
			});
		});
	});
});
