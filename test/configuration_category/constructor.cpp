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


#include "configuration_category.hpp"
#include "configuration.hpp"
#include "catch.hpp"


using namespace std;
using namespace cpponfig;


static const property empty_prop("");
static const property test_prop("bEFHlZq", "nuyUbtZKvDcEvrLjO");


TEST_CASE("configuration_category - constructor") {
	SECTION("Empty is default state", "[configuration_category] [constructor]") {
		configuration_category c;
		CHECK(c.empty());
		CHECK(c.comment.empty());
	}

	SECTION("Properties are copied from iterators", "[configuration_category] [constructor]") {
		const vector<pair<string, property>> itrs({{"hjfiEdPteZGcU", {"THVCPYNGbXYUCwTuwYrH", "nvecxgkNF"}},
		                                           {"mcXgvgZxTuNglUC", {"cELUsPwEXzxxzD", "boNdFhhD"}},
		                                           {"NZGdEaJRydZJXl", {"bsKdbfBpgx", "Yvcw"}},
		                                           {"kjAeBDZQeBzHZWFslT", {"toQYD", "vCmpJlyk"}},
		                                           {"IEVgryZdVghatRUV", {"qyAZYuQgChLXuwubYXlH", "lmWVzLHIOTzPHdD"}},
		                                           {"YORuOocgsDnBrVyKplbY", {"KPINBd", "mcADchxGUyklPEhPGNh"}}});

		configuration_category c(itrs.begin(), itrs.end());

		CHECK_FALSE(c.empty());
		CHECK(c.comment.empty());
		for(const auto & pr : itrs)
			REQUIRE(c.get(pr.first, empty_prop) == pr.second);
	}

	SECTION("Properties and comment are copied from iterators", "[configuration_category] [constructor]") {
		const vector<pair<string, property>> itrs({{"hfRQVGRetAXeaxTKjr", {"VIdVUrRiSIVZ", "alSbNApKnXUthUzKMqv"}},
		                                           {"gboBumf", {"DGwVunAhAFVgPGbjTY", "WVWpnEEdCPhnYTkgkn"}},
		                                           {"BkkfVoYPM", {"JaZQahvOgQLzKiYCyW", "BVhWJarrsM"}},
		                                           {"chFcoDNfdX", {"UClrDvUkwXH", "DJXwkwwaaAIhnLLN"}},
		                                           {"stRhi", {"syNrmlVOimUcfOccFMKo", "oismHxQ"}},
		                                           {"fwaciCH", {"MLkGAhaI", "ZKRQtAhj"}}});
		const string cmt = "NIiZINASIQiVYw";

		configuration_category c(cmt, itrs.begin(), itrs.end());

		CHECK_FALSE(c.empty());
		REQUIRE(c.comment == cmt);
		for(const auto & pr : itrs)
			REQUIRE(c.get(pr.first, empty_prop) == pr.second);
	}

	SECTION("comment is copied", "[configuration_category] [constructor]") {
		const string cmt = "okKSatQBppqPZpxnbfsV";

		configuration_category c(cmt);

		CHECK(c.empty());
		REQUIRE(c.comment == cmt);
	}

	SECTION("Properties are copied", "[configuration_category] [constructor]") {
		configuration_category full;
		full.get("asdf", test_prop);

		configuration_category empty(full);
		CHECK_FALSE(empty.empty());
		REQUIRE(empty.get("asdf", empty_prop) == test_prop);
	}

	SECTION("comment is copied", "[configuration_category] [constructor]") {
		configuration_category full;
		full.comment = "vZWchIqpJQmGGrqhGlYL";

		configuration_category empty(full);
		CHECK(empty.empty());
		REQUIRE(empty.comment == full.comment);
	}

	SECTION("Properties are moved", "[configuration_category] [constructor]") {
		configuration_category full;
		full.get("asdf", test_prop);

		configuration_category empty(move(full));
		CHECK(full.empty());
		CHECK_FALSE(empty.empty());
		REQUIRE(empty.get("asdf", empty_prop) == test_prop);
	}

	SECTION("comment is moved", "[configuration_category] [constructor]") {
		const string cmt = "NaisiIWjYdHBGKGIhSx";

		configuration_category full;
		full.comment = cmt;

		configuration_category empty(move(full));
		CHECK(full.empty());
		CHECK(empty.empty());
		REQUIRE(empty.comment == cmt);
	}
}
