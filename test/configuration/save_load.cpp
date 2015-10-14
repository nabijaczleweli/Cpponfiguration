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
#include <sstream>


using namespace std;
using namespace bandit;
using namespace cpponfig;


go_bandit([] {
	describe("configuration", [&] {
		describe("save + load", [&] {
			it("carries over comments", [&] {
				configuration tosave, toload;
				tosave.sof_comments = {"asdf", "fdsa"};

				stringstream ss;
				tosave.save(ss);
				toload.load(ss);

				AssertThat(toload.sof_comments, Is().EqualToContainer(tosave.sof_comments));
			});

			it("carries over elements", [&] {
				configuration tosave, toload;
				tosave.get("asdf", "fdsa");
				tosave.get("asdf:fdsa", "asdf:fdsa");

				stringstream ss;
				tosave.save(ss);
				toload.load(ss);

				AssertThat(toload.get("asdf").textual(), Is().EqualTo(tosave.get("asdf").textual()));
				AssertThat(toload.get("asdf:fdsa").textual(), Is().EqualTo(tosave.get("asdf:fdsa").textual()));
			});
		});
	});
});
