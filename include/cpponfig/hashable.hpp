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


#ifndef HASHABLE_HPP
#define HASHABLE_HPP


#include <functional>


namespace cpponfiguration {
	// T must publicly interhit hashable<T>, as in `class foo : public hashable<foo> {};`
	template <class T>
	class hashable {
		friend std::hash<hashable<T>>;

	protected:
		virtual size_t hash_code() const = 0;

		inline virtual ~hashable() noexcept = default;
	};
}

namespace cpponfig = cpponfiguration;


namespace std {
	template <class T>
	class hash<cpponfig::hashable<T>> {
	public:
		inline size_t operator()(const cpponfig::hashable<T> & tohash) const {
			return tohash.hash_code();
		}
	};

	template <class T>
	class hash : public hash<cpponfig::hashable<T>> {};
}


#endif  // HASHABLE_HPP
