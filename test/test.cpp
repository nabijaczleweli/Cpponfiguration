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


#include "configuration.hpp"
#include "util/strings.hpp"
#include <sstream>
#include <iostream>
#include <exception>


using namespace std;
using namespace cpponfig;


class dumping_configuration : public configuration {
	public:
		dumping_configuration() : configuration() {}
		dumping_configuration(const std::string & name) : configuration(name) {}

		void dump(ostream & stream) {
			for(const auto & cmt : sof_comments)
				stream << comment_character << ' ' << cmt << '\n';
			stream << (sof_comments.empty() ? "" : "\n");
			if(!properties.empty()) {
				for(const auto & prop : properties)
					stream << boolalpha << '<' << prop.first << ",property(" << prop.second.textual() <<
					                                                            (prop.second.comment.empty() ? "" : ',' + prop.second.comment) << ")>\n";
			} else
				stream << "<<NO ENTRIES>>\n";
		}
};


int main() {
	configuration::add_datetime_to_footer = configuration::datetime_mode::gmt;
	property::floating_precision = 10;

	dumping_configuration cfg;
	istringstream input_stream(
	                          "# This is a test" "\n"
	                          "# SOF comment" "\n"
	                          "\n"
	                          "mo0 = asdf" "\n"
	                          "asdf=m#00" "\n"
	                          "moo=asdf # asdf moo" "\n"
	                          "#moo=asdf" "\n"
	                          "m0o=[0,1,2,3,4]" "\n"
	                          "0m0=[false,true,false,false,true]" "\n"
	                          "m00=[1.25,3.7,233.7,20,1.99090909909]" "\n"
	                          );
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
			} catch(...) {}
		cout << "Loading failed!" << (what ? (string(" What: ") + what + '.') : "") << '\n';
	}
	cfg.dump(cout);

	cfg.get("0m0").boolean_list().emplace_back(false);
	cfg.get("m00").floating_list().emplace_back(3.14159);
	cfg.get("0m0").update_from_boolean_list();
	cfg.get("m00").update_from_floating_list();

	cout << '\n';
	stringstream output_stream;
	if(cfg.save(output_stream)) {
		ostringstream temp_stream;
		for(string temp; getline(output_stream, temp);)
			if(!temp.empty())
				temp_stream << temp << '\n';
		cout << "Saved: \"" << rtrim(temp_stream.str()) << "\".\n";
	} else
		cout << "Didn\'t save.\n";
}
