
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 2015.
// Copyright Christopher Kormanyos 2015.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

// This file also includes Doxygen-style documentation about the function of the code.
// See http://www.doxygen.org for details.

//! \file

//! \brief Example program showing use of math constants.

// Below are snippets of code that can be included into a Quickbook file.

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
// http://www.boost.org/doc/libs/release/libs/multiprecision/doc/html/boost_multiprecision/tut/floats/cpp_bin_float.html

#include <iomanip>
#include <iostream>
#include <limits>
#include <cmath>
int main()
{
//[bin_float_50_pi
  // Construct a 50 decimal digit string version of pi for reference.
  using boost::multiprecision::cpp_bin_float_50;  // 50 decimal digits precision.

  std::cout << std::setprecision(std::numeric_limits<cpp_bin_float_50>::digits10)
    << std::fixed
    << boost::math::constants::pi<cpp_bin_float_50>()
    // 3.14159265358979323846264338327950288419716939937510
    << std::endl;
//] //[/bin_float_50_pi]

  // Use a rather precise fixed_point that will use a 64-bit integer as its underlying representation.
//[fixed_point__constant
  typedef boost::fixed_point::negatable<3, -60> fixed_point_type;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
    << std::fixed
    << boost::math::constants::pi<fixed_point_type>()
    // 3.141592653589793238
    << std::endl;
//] [/fixed_point__constant]

  // Use a tiny (and so very imprecise) fixed_point type that will fit into a single byte.
//[fixed_point_imprecise_constant
  typedef boost::fixed_point::negatable<2, -5> tiny_fixed_point_type;

  std::cout << std::setprecision(std::numeric_limits<tiny_fixed_point_type>::digits10)
    << std::fixed
    << boost::math::constants::pi<tiny_fixed_point_type>()
    // 3.1
    << std::endl;
//] [/fixed_point_imprecise_constant]

  // Use a precise fixed_point type that will fit into a 128-bit 16 byte item.
//[fixed_point_precise_constant

  typedef boost::fixed_point::negatable<2, -125> precise_fixed_point_type;

  std::cout << std::setprecision(std::numeric_limits<precise_fixed_point_type>::digits10)
    << std::fixed
    << boost::math::constants::pi<precise_fixed_point_type>()
    // 3.1415926535897932384626433832795028842
    << std::endl;
//] [/fixed_point_precise_constant]

} // int main()


/*

Note that large digit counts rely on the
arc-cosine function is used for pi(). And since this function
is not included in the negatable realm yet, the constant
pi() does not work for negatable in all digit ranges.


*/