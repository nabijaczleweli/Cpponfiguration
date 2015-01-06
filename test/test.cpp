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


#include "../src/configuration.hpp"
#include <sstream>
#include <iostream>
#include <exception>


using namespace std;


class dumping_configuration : public configuration {
	public:
		dumping_configuration() : configuration() {}
		dumping_configuration(const std::string & name) : configuration(name) {}

		void dump(ostream & stream) {
			if(!properties.empty())
				for(const auto & prop : properties)
					stream << '<' << prop.first << ",property(" << prop.second.textual() << ")>\n";
			else
				stream << "<<NO ENTRIES>>\n";
		}
};


int main() {
	dumping_configuration cfg;
	stringstream stream(
	                    "m00 = asdf" "\n"
	                    "asdf=m#00" "\n"
	                    "moo=asdf # asdf moo" "\n"
	                    "#moo=asdf" "\n"
	                    "#moo=asdf" "\n"
	                    );
	try {
		cfg.load(stream);
		cout << "Loading succeeded!\n";
	} catch(...) {
		const char * what = nullptr;
		const auto exc = current_exception();
		if(exc)
			try {
				rethrow_exception(exc);
			} catch(const exception & e) {
				what = e.what();
			} catch(...) {}
		if(what)
			clog << "Loading failed! What: " << what << ".\n";
		else
			clog << "Loading failed!\n";
	}
	cfg.dump(cout);
}
