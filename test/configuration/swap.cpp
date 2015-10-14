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
#include <fstream>
#include <sstream>
#include <cstdlib>


using namespace std;
using namespace bandit;
using namespace cpponfig;


go_bandit([] {
	describe("configuration", [&] {
		describe("swap", [&] {
			it("swaps comments", [&] {
				configuration first, second;
				first.sof_comments  = {"first", "one"};
				second.sof_comments = {"second", "two"};

				swap(first, second);

				AssertThat(first.sof_comments, Is().EqualToContainer(vector<string>{"second", "two"}));
				AssertThat(second.sof_comments, Is().EqualToContainer(vector<string>{"first", "one"}));
			});

			it("swaps elements", [&] {
				configuration first, second;
				first.get("first", "one");
				first.get("a:first", "a:one");
				second.get("second", "two");
				second.get("b:second", "b:two");

				swap(first, second);

				AssertThat(first.get("second").textual(), Is().EqualTo("two"));
				AssertThat(first.get("b:second").textual(), Is().EqualTo("b:two"));
				AssertThat(second.get("first").textual(), Is().EqualTo("one"));
				AssertThat(second.get("a:first").textual(), Is().EqualTo("a:one"));
			});
		});
	});
});
