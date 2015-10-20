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
#include <fstream>


using namespace std;
using namespace cpponfig;


TEST_CASE("Cannot load from empty filename", "[configuration] [load]") {
	CHECK_FALSE(configuration().load());
	CHECK_FALSE(configuration("").load());
	CHECK_FALSE(configuration().load(""));
	CHECK_FALSE(configuration("").load(""));
}

TEST_CASE("Cannot load from nonexistant file", "[configuration] [load]") {
	const static string filename = "NkKVhzgfmSdLlzvsZxGF.LtCzOgFHxxnUlFwNVNgf";

	const auto save_on_destruction     = configuration::save_on_destruction;
	configuration::save_on_destruction = false;

	CHECK_FALSE(configuration(filename).load());
	CHECK_FALSE(configuration().load(filename));
	CHECK_FALSE(configuration(filename).load(filename));

	configuration::save_on_destruction = save_on_destruction;
}

TEST_CASE("Reads from empty file", "[configuration] [load]") {
	const static string filename = "BdPlUUSHRLUSwvyOZunFIzF.CeluUepoeCPBfeDSHqF";


	ofstream{filename};

	const auto save_on_destruction     = configuration::save_on_destruction;
	configuration::save_on_destruction = false;

	CHECK(configuration(filename).load());
	CHECK(configuration().load(filename));
	CHECK(configuration(filename).load(filename));

	configuration::save_on_destruction = save_on_destruction;

	remove(filename.c_str());
}

TEST_CASE("Reads from empty istream", "[configuration] [load]") {
	istringstream ss;
	CHECK(configuration().load(ss));
}

TEST_CASE("Doesn't read from failed istream", "[configuration] [load]") {
	istringstream ss;
	ss.setstate(ios::failbit);
	CHECK_FALSE(configuration().load(ss));
}
