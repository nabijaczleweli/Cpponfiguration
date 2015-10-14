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
		describe("operator", [&] {
			describe("=", [&] {
				it("copies comments", [&] {
					configuration empty, full;
					full.sof_comments = {"asdf", "fdsa"};

					empty = full;
					AssertThat(empty.empty(), Is().EqualTo(false));
					AssertThat(empty.sof_comments, Is().EqualToContainer(vector<string>{"asdf", "fdsa"}));
				});

				it("copies properties", [&] {
					configuration full, empty;
					full.get("asdf", "fdsa");
					full.get("a:asdf", "a:fdsa");

					empty = full;
					AssertThat(empty.empty(), Is().EqualTo(false));
					AssertThat(empty.get("asdf").textual(), Is().EqualTo("fdsa"));
					AssertThat(empty.get("a:asdf").textual(), Is().EqualTo("a:fdsa"));
				});

				it("moves comments and empties", [&] {
					configuration empty, full;
					full.sof_comments = {"asdf", "fdsa"};

					empty = move(full);
					AssertThat(full.empty(), Is().EqualTo(true));
					AssertThat(empty.empty(), Is().EqualTo(false));
					AssertThat(empty.sof_comments, Is().EqualToContainer(vector<string>{"asdf", "fdsa"}));
				});

				it("moves properties and empties", [&] {
					configuration full, empty;
					full.get("asdf", "fdsa");
					full.get("a:asdf", "a:fdsa");

					empty = move(full);
					AssertThat(full.empty(), Is().EqualTo(true));
					AssertThat(empty.empty(), Is().EqualTo(false));
					AssertThat(empty.get("asdf").textual(), Is().EqualTo("fdsa"));
					AssertThat(empty.get("a:asdf").textual(), Is().EqualTo("a:fdsa"));
				});
			});

			describe("+=", [&] {
				it("appends comments", [&] {
					configuration first, second;
					first.sof_comments  = {"first", "one"};
					second.sof_comments = {"second", "two"};

					first += second;
					AssertThat(first.sof_comments, Is().EqualToContainer(vector<string>{"first", "one", "second", "two"}));
				});

				it("appends properties", [&] {
					configuration first, second;
					first.get("first", "one");
					first.get("a:first", "a:one");
					second.get("second", "two");
					second.get("b:second", "b:two");

					first += second;
					AssertThat(first.get("first").textual(), Is().EqualTo("one"));
					AssertThat(first.get("a:first").textual(), Is().EqualTo("a:one"));
					AssertThat(first.get("second").textual(), Is().EqualTo("two"));
					AssertThat(first.get("b:second").textual(), Is().EqualTo("b:two"));
				});
			});

			describe("-=", [&] {
				it("removes comments", [&] {
					configuration first, second;
					first.sof_comments  = {"first", "one"};
					second.sof_comments = {"first", "two"};

					first -= second;
					AssertThat(first.sof_comments, Is().EqualToContainer(vector<string>{"one"}));
				});

				it("removes properties", [&] {
					configuration first, second;
					first.get("first", "one");
					first.get("a:first", "a:one");
					second.get("first", "two");
					second.get("a:first", "a:two");

					first -= second;
					AssertThat(first.get("first").textual(), Is().EqualTo(""));
					AssertThat(first.get("a:first").textual(), Is().EqualTo(""));
				});
			});

			describe("+", [&] {
				it("appends comments", [&] {
					configuration first, second;
					first.sof_comments  = {"first", "one"};
					second.sof_comments = {"second", "two"};

					auto added = first + second;
					AssertThat(added.sof_comments, Is().EqualToContainer(vector<string>{"first", "one", "second", "two"}));
				});

				it("appends properties", [&] {
					configuration first, second;
					first.get("first", "one");
					first.get("a:first", "a:one");
					second.get("second", "two");
					second.get("b:second", "b:two");

					auto added = first + second;
					AssertThat(added.get("first").textual(), Is().EqualTo("one"));
					AssertThat(added.get("a:first").textual(), Is().EqualTo("a:one"));
					AssertThat(added.get("second").textual(), Is().EqualTo("two"));
					AssertThat(added.get("b:second").textual(), Is().EqualTo("b:two"));
				});
			});

			describe("-", [&] {
				it("removes comments", [&] {
					configuration first, second;
					first.sof_comments  = {"first", "one"};
					second.sof_comments = {"first", "two"};

					auto substracted = first - second;
					AssertThat(substracted.sof_comments, Is().EqualToContainer(vector<string>{"one"}));
				});

				it("removes properties", [&] {
					configuration first, second;
					first.get("first", "one");
					first.get("a:first", "a:one");
					second.get("first", "two");
					second.get("a:first", "a:two");

					auto substracted = first - second;
					AssertThat(substracted.get("first").textual(), Is().EqualTo(""));
					AssertThat(substracted.get("a:first").textual(), Is().EqualTo(""));
				});
			});
		});
	});
});
