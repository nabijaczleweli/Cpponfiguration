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


#ifndef SWAPPABLE_HPP
#define SWAPPABLE_HPP


namespace cpponfiguration {
	// T must interhit swappable<T>, as in `class foo : swappable<foo> {};`
	template <class T>
	class swappable {
	protected:
		virtual void swap(T & other) = 0;

	public:
		inline void swap(swappable<T> & other) {
			swap(dynamic_cast<T &>(other));
		}

		inline virtual ~swappable() noexcept = default;
	};
}

namespace cpponfig = cpponfiguration;


namespace std {
	template <class T>
	inline void swap(cpponfig::swappable<T> & lhs, cpponfig::swappable<T> & rhs) {
		lhs.swap(rhs);
	}
}


#endif  // SWAPPABLE_HPP
