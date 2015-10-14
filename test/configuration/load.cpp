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
		describe("load", [&] {
			before_each([&] { ofstream("emptest.cfg"); });
			after_each([&] { remove("emptest.cfg"); });


			it("returns false with empty `filename`", [&] {
				AssertThat(configuration().load(), Is().EqualTo(false));
				AssertThat(configuration("").load(), Is().EqualTo(false));
				AssertThat(configuration().load(""), Is().EqualTo(false));
				AssertThat(configuration("").load(""), Is().EqualTo(false));
			});

			it("returns false with nonexistant file", [&] {
				const auto save_on_destruction     = configuration::save_on_destruction;
				configuration::save_on_destruction = false;

				AssertThat(configuration("AAAAAAAAAA.BBBBBBBBBB").load(), Is().EqualTo(false));
				AssertThat(configuration().load("AAAAAAAAAA.BBBBBBBBBB"), Is().EqualTo(false));
				AssertThat(configuration("AAAAAAAAAA.BBBBBBBBBB").load("AAAAAAAAAA.BBBBBBBBBB"), Is().EqualTo(false));

				configuration::save_on_destruction = save_on_destruction;
			});

			it("returns true with empty file", [&] {
				const auto save_on_destruction     = configuration::save_on_destruction;
				configuration::save_on_destruction = false;

				AssertThat(configuration("emptest.cfg").load(), Is().EqualTo(true));
				AssertThat(configuration().load("emptest.cfg"), Is().EqualTo(true));
				AssertThat(configuration("emptest.cfg").load("emptest.cfg"), Is().EqualTo(true));

				configuration::save_on_destruction = save_on_destruction;
			});

			it("returns true with empty istream", [&] {
				istringstream ss;
				AssertThat(configuration().load(ss), Is().EqualTo(true));
			});

			it("returns true with failed istream", [&] {
				istringstream ss;
				ss.setstate(ios::failbit);
				AssertThat(configuration().load(ss), Is().EqualTo(false));
			});
		});
	});
});
