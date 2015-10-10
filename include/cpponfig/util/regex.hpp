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


#pragma once


#define CPPONFIG_CHARCACHED_REGEX_TEMPLATE(args, mapt, keyass, formula)                                                  \
	[] args {                                                                                                              \
		static std::map<mapt, std::regex> regices;                                                                           \
                                                                                                                         \
		const auto key = keyass;                                                                                             \
		auto itr = regices.find(key);                                                                                        \
		if(itr == regices.end())                                                                                             \
			itr = regices.emplace(key, regex(formula, std::regex_constants::optimize)).first; \
		return itr->second;                                                                                                  \
	}
#define CPPONFIG_CHARCACHED_REGEX(formula) CPPONFIG_CHARCACHED_REGEX_TEMPLATE((char chr), char, chr, formula)
#define CPPONFIG_TWOCHARCACHED_REGEX(formula) \
	CPPONFIG_CHARCACHED_REGEX_TEMPLATE((char chr1, char chr2), uint16_t, chr1 | (static_cast<uint16_t>(chr2) >> 8), formula)
