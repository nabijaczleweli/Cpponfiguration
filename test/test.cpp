// The MIT License (MIT)

// Copyright (c) 2015 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "../include/cpponfig/cpponfig_version.hpp"
#include "../include/cpponfig/configuration.hpp"
#include "../include/cpponfig/util/strings.hpp"
#include <sstream>
#include <iostream>
#include <exception>


using namespace std;
using namespace cpponfig;
using namespace cpponfig::util;


int main() {
	cout << "Cpponfiguration v. " << cpponfiguration_version << "\n\n";

	configuration::datetime_footer_type = configuration::datetime_mode::gmt;
	property::floating_precision        = 10;

	configuration cfg;
	istringstream input_stream(R"TEXT(
	                           # This is a test
	                           # SOF comment

	                           { # This is a category
	                             mo0 = asdf
	                             asdf=m#00
	                             moo=asdf # asdf moo
	                             #moo=asdf
	                             m0o=[0,1,2,3,4]
	                             0m0=[false,true,false,false,true]
	                             m00=[1.25,3.7,233.7,20,1.99090909909]
	                           }
	                           )TEXT");
	try {
		cfg.load(input_stream);
		cout << "Loading succeeded!\n";
	} catch(...) {
		const char * what = nullptr;
		const auto exc = current_exception();
		if(exc)
			try {
				rethrow_exception(exc);
			} catch(const exception & e) {
				what = e.what();
			} catch(...) {
			}
		cout << "Loading failed!" << (what ? (" What: "s + what + '.') : "") << '\n';
	}
	cfg.save(cout);

	cfg.get("0m0").boolean_list().emplace_back(false);
	cfg.get("m00").floating_list().emplace_back(3.14159);
	cfg.get("0m0").update_from_boolean_list();
	cfg.get("m00").update_from_floating_list();

	cout << "\n\"";
	if(cfg.save(cout))
		cout << "\"\nSaved properly.\n";
	else
		cout << "Didn\'t save.\n";
}
