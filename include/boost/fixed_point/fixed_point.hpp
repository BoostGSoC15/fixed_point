///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2013 - 2016.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// This file is a partial reference implementation for the proposed
// "C++ binary fixed-point arithmetic" as specified in N3352.
// See: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html
// In this file, we include subsidiary files that implement the
// proposed specified fixed-point types.

#ifndef FIXED_POINT_2015_03_06_HPP_
  #define FIXED_POINT_2015_03_06_HPP_

  // There is optional support for certain variations of fixed_point
  // using preprocessor definitions. Not all of these are supported
  // at the moment. The potential options include:

  // Is supported now  : #define BOOST_FIXED_POINT_DISABLE_IOSTREAM
  // Is supported now  : #define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION
  // Not yet supported : #define BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH
  // Not yet supported : #define BOOST_FIXED_POINT_DISABLE_CPP11
  // Not yet supported : #define BOOST_FIXED_POINT_ENABLE_GMP_BACKENDS

  // With BOOST_FIXED_POINT_DISABLE_IOSTREAM, all I/O streaming
  // is disabled, as is the inclusion of associated standard
  // library headers. This option eliminates all I/O stream
  // overhead, in particular for bare-metal microcontroller projects.
  // Disabling I/O streaming requires simultaneous disabling
  // of multiprecision.

  // With BOOST_FIXED_POINT_DISABLE_MULTIPRECISION,
  // the use of Boost.Multiprecision for back-ends
  // of the fixed-point classes is disabled.

  // With BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH,
  // fixed_point avoids using the unsigned_large_type.
  // This option is intended for systems with limited
  // integer widths such as bare-metal microcontrollers.
  // When used in combination with BOOST_FIXED_POINT_DISABLE_MULTIPRECISION,
  // this option provides fixed-point representations with
  // up to 64-bits (if 64-bit integral types are available)
  // without requiring any of Boost.Multiprecision.
  // Otherwise, 32-bit internal representations would
  // have the largest possible widths.

  // With BOOST_FIXED_POINT_DISABLE_CPP11, an optional
  // back-port to C++03 is supported. This eliminates
  // the use of all C++11 language elements. This might send
  // the wrong message about language technology, but could
  // increase the range of potential target compilers
  // (especially for embedded systems).

  // With BOOST_FIXED_POINT_ENABLE_GMP_BACKENDS,
  // multiple-precision backends (when used) will use the ones
  // based on GMP instead of Boost.Multiprecision's self-written
  // multiple-precision backends.

  #include <boost/fixed_point/fixed_point_negatable.hpp>

#endif // FIXED_POINT_2015_03_06_HPP_
