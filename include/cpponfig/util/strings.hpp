// The MIT License (MIT)

// Copyright (c) 2014 nabijaczleweli

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
#ifndef STRINGS_HPP
#define STRINGS_HPP


#include <algorithm>
#include <cctype>


namespace cpponfiguration {
	namespace util {
		namespace {
			const static auto whitespace_selector = [](char c) { return !std::isspace(c); };
		}

		// Stolen from http://stackoverflow.com/a/217605/2851815
		static inline std::string & ltrim(std::string & s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), whitespace_selector));
			return s;
		}

		// Stolen from http://stackoverflow.com/a/217605/2851815
		static inline std::string & rtrim(std::string & s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), whitespace_selector).base(), s.end());
			return s;
		}

		// Stolen from http://stackoverflow.com/a/217605/2851815
		static inline std::string & trim(std::string & s) {
			return ltrim(rtrim(s));
		}

		static inline std::string ltrim(std::string && s) {
			s.erase(s.begin(), std::find_if(s.begin(), s.end(), whitespace_selector));
			return std::move(s);
		}

		static inline std::string rtrim(std::string && s) {
			s.erase(std::find_if(s.rbegin(), s.rend(), whitespace_selector).base(), s.end());
			return std::move(s);
		}

		static inline std::string trim(std::string && s) {
			return ltrim(rtrim(move(s)));
		}

		static inline std::string ltrim(const std::string & s) {
			std::string news(s);
			ltrim(news);
			return news;
		}

		static inline std::string rtrim(const std::string & s) {
			std::string news(s);
			rtrim(news);
			return news;
		}

		static inline std::string trim(const std::string & s) {
			std::string news(s);
			trim(news);
			return news;
		}
	}
}

namespace cpponfig = cpponfiguration;


#endif  // STRINGS_HPP
