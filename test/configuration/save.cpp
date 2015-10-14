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


static bool exists(const char * fname) {
	FILE * f = fopen(fname, "r");
	if(f)
		fclose(f);
	return f;
}


go_bandit([] {
	describe("configuration", [&] {
		describe("save", [&] {
			before_each([&] { remove("emptest.cfg"); });
			after_each([&] { remove("emptest.cfg"); });


			it("returns false with empty `filename`", [&] {
				AssertThat(configuration().save(), Is().EqualTo(false));
				AssertThat(configuration("").save(), Is().EqualTo(false));
				AssertThat(configuration().save(""), Is().EqualTo(false));
				AssertThat(configuration("").save(""), Is().EqualTo(false));
			});

			it("creates nonexistant files", [&] {
				configuration("conftest.cfg").save();
				AssertThat(exists("conftest.cfg"), Is().EqualTo(true));
				remove("conftest.cfg");

				configuration().save("conftest.cfg");
				AssertThat(exists("conftest.cfg"), Is().EqualTo(true));
				remove("conftest.cfg");

				configuration().save("conftest.cfg");
				AssertThat(exists("conftest.cfg"), Is().EqualTo(true));
				remove("conftest.cfg");

				configuration("conftest.cfg").save("conftest.cfg");
				AssertThat(exists("conftest.cfg"), Is().EqualTo(true));
				remove("conftest.cfg");
			});

			it("writes \"\" on nothing", [&] {
				ostringstream strm;
				configuration().save(strm);
				AssertThat(strm.str(), Is().Empty());
			});

			it("writes SOF comments", [&] {
				configuration c;
				c.sof_comments = {"A", "B", "C"};

				ostringstream strm;
				c.save(strm);

				AssertThat(strm.str(), Is().EqualTo("# A\n"
				                                    "# B\n"
				                                    "# C\n"
				                                    "\n"
				                                    "\n"));
			});

			it("writes local datetime", [&] {
				const auto datetime_footer_type     = configuration::datetime_footer_type;
				configuration::datetime_footer_type = configuration::dt_m::local;

				configuration c;

				ostringstream strm;
				c.save(strm);

				AssertThat(strm.str(), Is().StartingWith("\n#  ").And().Not().Containing("GMT"));

				configuration::datetime_footer_type = datetime_footer_type;
			});

			it("writes GMT datetime", [&] {
				const auto datetime_footer_type     = configuration::datetime_footer_type;
				configuration::datetime_footer_type = configuration::dt_m::gmt;

				configuration c;

				ostringstream strm;
				c.save(strm);

				AssertThat(strm.str(), Is().StartingWith("\n#  ").And().Containing("GMT"));

				configuration::datetime_footer_type = datetime_footer_type;
			});

			it("correctly doesn't prefix the brace with ' '", [&] {
				configuration c;
				c.get("", "");

				ostringstream strm;
				c.save(strm);

				AssertThat(strm.str(), Is().StartingWith("{"));
			});

			it("correctly prefixes the brace with ' '", [&] {
				configuration c;
				c.get("a:", "");

				ostringstream strm;
				c.save(strm);

				AssertThat(strm.str(), Is().StartingWith("a {"));
			});
		});
	});
});
