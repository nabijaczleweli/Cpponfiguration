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


static bool exists(const char * fname) {
	FILE * f = fopen(fname, "r");
	if(f)
		fclose(f);
	return f;
}


TEST_CASE("Saving fails with empty filename", "[configuration] [save]") {
	CHECK_FALSE(configuration().save());
	CHECK_FALSE(configuration("").save());
	CHECK_FALSE(configuration().save(""));
	CHECK_FALSE(configuration("").save(""));
}

TEST_CASE("Saving creates file with nonexistant file", "[configuration] [save]") {
	configuration("conftest.cfg").save();
	CHECK(exists("conftest.cfg"));
	remove("conftest.cfg");

	configuration().save("conftest.cfg");
	CHECK(exists("conftest.cfg"));
	remove("conftest.cfg");

	configuration().save("conftest.cfg");
	CHECK(exists("conftest.cfg"));
	remove("conftest.cfg");

	configuration("conftest.cfg").save("conftest.cfg");
	CHECK(exists("conftest.cfg"));
	remove("conftest.cfg");
}

TEST_CASE("Saving yields \"\" empty config", "[configuration] [save]") {
	ostringstream strm;
	configuration().save(strm);
	REQUIRE(strm.str().find_first_not_of("\n") == string::npos);
}

TEST_CASE("SOF comments are saved", "[configuration] [save]") {
	configuration c;
	c.sof_comments = {"A", "B", "C"};

	ostringstream strm;
	c.save(strm);

	REQUIRE(strm.str() == "# A\n"
	                      "# B\n"
	                      "# C\n"
	                      "\n"
	                      "\n");
}

TEST_CASE("Local datetime is saved", "[configuration] [save]") {
	const auto datetime_footer_type     = configuration::datetime_footer_type;
	configuration::datetime_footer_type = configuration::dt_m::local;

	configuration c;

	ostringstream strm;
	c.save(strm);

	REQUIRE(strm.str().find("\n#  ") == 0);
	REQUIRE(strm.str().find("GMT") == string::npos);

	configuration::datetime_footer_type = datetime_footer_type;
}

TEST_CASE("GMT datetime is saved", "[configuration] [save]") {
	const auto datetime_footer_type     = configuration::datetime_footer_type;
	configuration::datetime_footer_type = configuration::dt_m::gmt;

	configuration c;

	ostringstream strm;
	c.save(strm);

	REQUIRE(strm.str().find("\n#  ") == 0);
	REQUIRE(strm.str().find("GMT") != string::npos);

	configuration::datetime_footer_type = datetime_footer_type;
}

TEST_CASE("Nameless category brace isn't prefixed by ' '", "[configuration] [save]") {
	configuration c;
	c.get("", "");

	ostringstream strm;
	c.save(strm);

	REQUIRE(strm.str()[0] == '{');
}

TEST_CASE("Named category brace is prefixed by ' '", "[configuration] [save]") {
	configuration c;
	c.get("a:", "");

	ostringstream strm;
	c.save(strm);

	REQUIRE(strm.str().find("a {") == 0);
}
