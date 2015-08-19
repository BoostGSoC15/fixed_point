
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

//! \brief Example program showing non-trivial use of negatable with math special functions.

// Below are snippets of code that are included into Quickbook file fixed_point.qbk.


#include <algorithm>
#include <limits>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/special_functions/bernoulli.hpp>
#include <boost/multiprecision/cpp_bin_float.hpp>
// http://www.boost.org/doc/libs/release/libs/multiprecision/doc/html/boost_multiprecision/tut/floats/cpp_bin_float.html

int main()
{
  // Test bernoulli_b2n() for negatable. Compute an array of
  // fixed-point Bernoulli numbers and test them with
  // multiprecision floating-point control values.

  BOOST_CONSTEXPR_OR_CONST std::size_t number_of_bernoulli_b2n = 22U;

  BOOST_CONSTEXPR_OR_CONST int resolution_of_bernoulli_b2n = -280;

  // Make a very large fixed-point type with an unusually asymetric large integral range.
  typedef boost::fixed_point::negatable<2047 + resolution_of_bernoulli_b2n,
                                        resolution_of_bernoulli_b2n,
                                        boost::fixed_point::round::nearest_even>
  fixed_point_type;

  // Define a fixed-point vector for storing the Bernoulli numbers.
  std::vector<fixed_point_type> b2n_fixed;

  // Fill the the fixed-point vector with even Bernoulli numbers.
  boost::math::bernoulli_b2n<fixed_point_type>(0, number_of_bernoulli_b2n, std::back_inserter(b2n_fixed));

  // Define a less-wide fixed-point type for manipulating the results.
  typedef boost::fixed_point::negatable<511 + resolution_of_bernoulli_b2n,
                                        resolution_of_bernoulli_b2n,
                                        boost::fixed_point::round::nearest_even>
  fixed_point_result_type;

  // Define a less-wide floating-point type for verifying the results.
  typedef fixed_point_type::float_type float_point_result_type;

  std::vector<fixed_point_result_type> b2n_fixed_result(number_of_bernoulli_b2n);

  // Define storage for multiprecision floating-point control values.
  std::vector<float_point_result_type> b2n_float_control;

  // Fill the the floating-point vector with control values.
  boost::math::bernoulli_b2n<float_point_result_type>(0, number_of_bernoulli_b2n, std::back_inserter(b2n_float_control));

  // Transform the Bernoulli number arrays. This tests the fixed-point
  // class populating an STL container in combination with an algorithm
  // and a lambda function.
  std::transform(b2n_fixed.cbegin(),
                 b2n_fixed.cend(),
                 b2n_fixed_result.begin(),
                 [](const fixed_point_type& b2n) -> fixed_point_result_type
                 {
                   return fixed_point_result_type(b2n);
                 });

  // Set a judiciously selected precision and fixed format on the output stream.

  BOOST_CONSTEXPR_OR_CONST std::streamsize precision_of_ostream =
    std::streamsize(((long(-resolution_of_bernoulli_b2n) - 1L) * 301L) / 1000L);

  std::cout.precision(precision_of_ostream);
  std::cout.setf(std::ios::fixed);

  // Print the fixed-point results to the output stream.
  std::cout << std::endl << "Fixed-point results:" << std::endl << std::endl;
  std::copy(b2n_fixed_result.cbegin(),
            b2n_fixed_result.cend(),
            std::ostream_iterator<fixed_point_result_type>(std::cout, "\n"));

  // Print the floating-point control values to the output stream.
  std::cout << std::endl << "Floating-point-point control values:" << std::endl << std::endl;
  std::copy(b2n_float_control.cbegin(),
            b2n_float_control.cend(),
            std::ostream_iterator<float_point_result_type>(std::cout, "\n"));
}
