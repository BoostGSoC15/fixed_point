
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

//! \brief Example program showing a polynomial approximation of tgamma(negatable).

// Below are snippets of code that are included into Quickbook file fixed_point.qbk.

#include <iomanip>
#include <iostream>

#include <boost/array.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/tools/rational.hpp>

namespace local
{
  template<typename NumericType>
  NumericType tgamma(const NumericType& x)
  {
    // This subroutine computes tgamma(x - 2) to approximately
    // order 7 using a polynomial approximation.

    // The coefficients originate from J. F. Hart et al.,
    // Computer Approximations (John Wiley and Sons, Inc., 1968).
    // See Chap. 7, Tables of Coefficients, Table 5206 on page 244.
    BOOST_CONSTEXPR boost::array<NumericType, 8U> coefs =
    {
      NumericType(0.9999999757437L),
      NumericType(0.4227874604607L),
      NumericType(0.4117741970939L),
      NumericType(0.0821117276973L),
      NumericType(0.0721101941645L),
      NumericType(0.00445108786245L),
      NumericType(0.005159029832L),
      NumericType(0.0016063028892L),
    };

    return boost::math::tools::evaluate_polynomial(coefs, x - 2);
  }
}

int main()
{
  // This example performs computations of the tgamma(x)
  // function for x = 1/2 for a fixed-point negatable
  // type and built-in float.

  // A 32-bit fixed-point type is used, as might be
  // well-suited for a high-performance 32-bit embedded
  // system that does not have or does not use an FPU.

  typedef boost::fixed_point::negatable<7, -24> fixed_point_type;
  typedef fixed_point_type::float_type float_point_type;

  // Here we compute tgamma(5/2) and subsequently perform
  // two iterations of downward recursion via division with
  // (3/2) * (1/2) = (3/4). The result is tgamma(1/2), which
  // has a known closed-form value equal to sqrt(pi).

  const fixed_point_type x = fixed_point_type(5) / 2;
  const fixed_point_type g = (local::tgamma(x) * 4) / 3;

  std::cout << std::setprecision(6)
            << std::fixed
            << g
            << std::endl;

  using std::sqrt;

  // Compare with the control value of sqrt(pi) computed
  // with a built-in floating-point type.
  std::cout << std::setprecision(6)
            << std::fixed
            << sqrt(boost::math::constants::pi<float_point_type>())
            << std::endl;
}
