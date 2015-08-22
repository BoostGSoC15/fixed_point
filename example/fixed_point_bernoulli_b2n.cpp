
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

//! \brief Example program showing non-trivial use of negatable with a special function from Boost.Math.

// Below are snippets of code that are included into Quickbook file fixed_point.qbk.


#include <array>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <iterator>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/special_functions/bernoulli.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
// http://www.boost.org/doc/libs/release/libs/multiprecision/doc/html/boost_multiprecision/tut/floats/cpp_bin_float.html

int main()
{
  // Example: Compute an array of fixed-point Bernoulli numbers.

  BOOST_CONSTEXPR_OR_CONST std::size_t number_of_bernoulli_b2n = 22U;
  BOOST_CONSTEXPR_OR_CONST int         resolution_of_bernoulli_b2n = -280;

  // Make a very large fixed-point type with an asymetrically large integral range.
  typedef boost::fixed_point::negatable<2047 + resolution_of_bernoulli_b2n,
                                        resolution_of_bernoulli_b2n,
                                        boost::fixed_point::round::nearest_even>
  fixed_point_type;

  // Define a fixed-point array for storing the Bernoulli numbers.
  std::array<fixed_point_type, number_of_bernoulli_b2n> b2n_fixed;

  // Fill the fixed-point array with even Bernoulli numbers.
  for(int i = 0U; i < int(number_of_bernoulli_b2n); ++i)
  {
    b2n_fixed[i] = boost::math::bernoulli_b2n<fixed_point_type>(i);
  }

  // Set a judiciously selected precision and also fixed format
  // on the output stream.

  BOOST_CONSTEXPR_OR_CONST std::streamsize precision_of_ostream =
    std::streamsize(((long(-resolution_of_bernoulli_b2n) - 1L) * 301L) / 1000L);

  std::cout.precision(precision_of_ostream);
  std::cout.setf(std::ios::fixed);

  // Print the fixed-point results to the output stream.
  std::cout << std::endl << "Fixed-point results of bernoulli_b2n:" << std::endl << std::endl;
  std::copy(b2n_fixed.cbegin(),
            b2n_fixed.cend(),
            std::ostream_iterator<fixed_point_type>(std::cout, "\n"));

  // Define a floating-point array for storing the control values.
  std::array<fixed_point_type::float_type, number_of_bernoulli_b2n> b2n_float;

  // Fill the floating-point array with even Bernoulli numbers.
  for(int i = 0U; i < int(number_of_bernoulli_b2n); ++i)
  {
    b2n_float[i] = boost::math::bernoulli_b2n<fixed_point_type::float_type>(i);
  }

  // Print the fixed-point results to the output stream.
  std::cout << std::endl << "Floating-point results of bernoulli_b2n:" << std::endl << std::endl;
  std::copy(b2n_float.cbegin(),
            b2n_float.cend(),
            std::ostream_iterator<fixed_point_type::float_type>(std::cout, "\n"));
}
