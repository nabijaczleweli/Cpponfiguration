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


#ifndef SALT_HPP
#define SALT_HPP


#include <random>
#include <memory>


namespace cpponfiguration {
	// Here, because:
	//
	// "Hash functions are only required to produce the same result for the same input within a single execution of a program;
	// this allows salted hashes that prevent collision DoS attacks."
	// Source: http://en.cppreference.com/w/cpp/utility/hash
	//
	// Example usage:
	// size_t hash_code() {
	//   static const salt slt;
	//
	//   size_t result = /*prime*/;
	//   // Produce hash, store in result
	//   return result ^ slt;
	// }
	class salt {
	private:
		static std::mt19937 randomizer;
		static std::uniform_int_distribution<size_t> dist;

		const size_t value = randomize_salt ? dist(randomizer) : 0x249393A9;

	public:
		static bool randomize_salt;  // Useful for debugging


		constexpr inline operator size_t() const {
			return value;
		}
	};
}

namespace cpponfig = cpponfiguration;


#endif  // SALT_HPP
