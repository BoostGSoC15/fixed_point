
///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the cylindrical bessel function of (fixed_point) for a small digit range.

#define BOOST_TEST_MODULE test_negatable_math_cyl_bessel_j_small
#define BOOST_LIB_DIAGNOSTIC

#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_math_cyl_bessel_j_small)
{
  typedef boost::fixed_point::negatable<  std::numeric_limits<int>::digits,
                                        -(std::numeric_limits<long double>::digits - std::numeric_limits<int>::digits)>
  fixed_point_type;

  typedef fixed_point_type::float_type float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 6);

  BOOST_CONSTEXPR int i_max = 4;

  // Check small arguments in the region of Taylor series expansion.
  for(int i = 1; i <= i_max; ++i)
  {
    // Use an integer-valued order wrapped in the fixed-point type in order
    // to force a high level of compiler work but simultaneously avoid
    // potentially lossy tgamma calculations (requiring Bernoulli numbers).

    const fixed_point_type x = boost::math::cyl_bessel_j(fixed_point_type(2), fixed_point_type(i) / boost::math::constants::e<fixed_point_type>());
    const fixed_point_type y = boost::math::cyl_bessel_j(float_point_type(2), float_point_type(i) / boost::math::constants::e<float_point_type>());

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }
}
