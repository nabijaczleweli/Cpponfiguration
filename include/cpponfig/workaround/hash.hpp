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


#ifndef CPPONFIG_WORKAROUND_HASH_HPP
#define CPPONFIG_WORKAROUND_HASH_HPP


// See http://www.boost.org/doc/libs/1_52_0/boost/config/select_stdlib_config.hpp
#include <cstddef>


#ifdef _LIBCPP_VERSION
#define CPPONFIGURATION_STDHASH std::__1::hash
#define CPPONFIGURATION_STDHASH_OVERLOAD_NAMESPACE_BEGIN _LIBCPP_BEGIN_NAMESPACE_STD
#define CPPONFIGURATION_STDHASH_OVERLOAD_NAMESPACE_END }}
#else
#define CPPONFIGURATION_STDHASH std::hash
#define CPPONFIGURATION_STDHASH_OVERLOAD_NAMESPACE_BEGIN namespace std {
#define CPPONFIGURATION_STDHASH_OVERLOAD_NAMESPACE_END }
#endif


#endif  // CPPONFIG_WORKAROUND_HASH_HPP
