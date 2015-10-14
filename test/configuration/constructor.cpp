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
		describe("constructor", [&] {
			it("is empty by default", [&] {
				configuration c;
				AssertThat(c.empty(), Is().EqualTo(true));
				AssertThat(c.sof_comments, Is().Empty());
			});

			it("is empty when constructed with filename", [&] {
				const auto save_on_destruction     = configuration::save_on_destruction;
				configuration::save_on_destruction = false;

				{
					configuration c("AAAAAAAAAA.BBBBBBBBBB");
					AssertThat(c.empty(), Is().EqualTo(true));
					AssertThat(c.sof_comments, Is().Empty());
				}

				configuration::save_on_destruction = save_on_destruction;
			});

			it("copies properties", [&] {
				configuration full;
				full.get("asdf", "fdsa");

				configuration empty(full);
				AssertThat(empty.empty(), Is().EqualTo(false));
				AssertThat(empty.get("asdf").textual(), Is().EqualTo("fdsa"));
			});

			it("copies comments", [&] {
				configuration full;
				full.sof_comments = {"asdf", "fdsa"};

				configuration empty(full);
				AssertThat(empty.empty(), Is().EqualTo(false));
				AssertThat(empty.sof_comments, Is().EqualToContainer(vector<string>{"asdf", "fdsa"}));
			});

			it("moves properties and empties", [&] {
				configuration full;
				full.get("asdf", "fdsa");

				configuration empty(move(full));
				AssertThat(full.empty(), Is().EqualTo(true));
				AssertThat(empty.empty(), Is().EqualTo(false));
				AssertThat(empty.get("asdf").textual(), Is().EqualTo("fdsa"));
			});

			it("moves comments and empties", [&] {
				configuration full;
				full.sof_comments = {"asdf", "fdsa"};

				configuration empty(move(full));
				AssertThat(full.empty(), Is().EqualTo(true));
				AssertThat(empty.empty(), Is().EqualTo(false));
				AssertThat(empty.sof_comments, Is().EqualToContainer(vector<string>{"asdf", "fdsa"}));
			});
		});
	});
});
