
///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2016.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests long mul/div of fixed_point negatable with BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH.

#define BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH
#define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION
#define BOOST_FIXED_POINT_DISABLE_IOSTREAM

#define BOOST_TEST_MODULE test_negatable_basic_disable_wide_integer_math
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <boost/test/included/unit_test.hpp>

namespace
{
  typedef boost::fixed_point::negatable<10, -53> fixed_point_type;

  typedef fixed_point_type::float_type float_point_type;
}

BOOST_AUTO_TEST_CASE(test_negatable_basic_disable_wide_integer_math)
{
  using std::ldexp;
  const float_point_type tol = ldexp(float_point_type(1), -15);

  const fixed_point_type x(fixed_point_type(100U) /  70U);
  const fixed_point_type y(fixed_point_type(456U) / 110U);

  BOOST_CONSTEXPR float_point_type xd(BOOST_FIXED_POINT_FLOAT64_C(100.0) / BOOST_FIXED_POINT_FLOAT64_C( 70.0));
  BOOST_CONSTEXPR float_point_type yd(BOOST_FIXED_POINT_FLOAT64_C(456.0) / BOOST_FIXED_POINT_FLOAT64_C(110.0));

  // Check long mul/div for BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH.
  BOOST_CHECK_CLOSE_FRACTION(float_point_type(x * y), xd * yd, tol); // 5.9220779220779220779220779220779
  BOOST_CHECK_CLOSE_FRACTION(float_point_type(x / y), xd / yd, tol); // 0.34461152882205513784461152882206
}
