// The MIT License (MIT)

// Copyright (c) 2015 nabijaczlweli

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
#include <istream>

using namespace std;


char configuration::comment_character = '#';
char configuration::assignment_character = '=';


void configuration::load_properties(istream & from) {
	for(string line; getline(from, line);) {
		size_t equals_idx;

		if(line.empty() || line[0] == comment_character || (equals_idx = line.find_first_of(assignment_character)) == string::npos)
			continue;

		ltrim(line);
		if(line.empty() || line[0] == comment_character)
			continue;

		if(const size_t comment_idx = line.find_first_of(comment_character) && comment_idx != string::npos) {
			line = line.substr(line.find_first_of(comment_character));
			rtrim(line);
			if(line.empty() || comment_idx > equals_idx)
				continue;
		} else
			rtrim(line);

		// TODO process things now, add to map, whatever
	}
}
