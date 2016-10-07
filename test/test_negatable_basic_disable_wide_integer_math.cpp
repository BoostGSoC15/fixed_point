
///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2016.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests long mul/div of fixed_point negatable for BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH.

#define BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH
#define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION
#define BOOST_FIXED_POINT_DISABLE_IOSTREAM

#define BOOST_TEST_MODULE test_negatable_basic_disable_wide_integer_math
#define BOOST_LIB_DIAGNOSTIC

#include <boost/cstdfloat.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_basic_disable_wide_integer_math)
{
  typedef boost::fixed_point::negatable< 10,
                                        -53,
                                        boost::fixed_point::round::classic> fixed_point_type;

  typedef boost::float64_t float_point_type;

  using std::ldexp;

  const float_point_type tol = ldexp(float_point_type(1), -15);

  const fixed_point_type x(fixed_point_type(100U) /  70U);
  const fixed_point_type y(fixed_point_type(456U) / 110U);

  const fixed_point_type z_mul(x * y);
  const fixed_point_type z_div(x / y);

  BOOST_CONSTEXPR float_point_type xd(BOOST_FLOAT64_C(100.0) / BOOST_FLOAT64_C( 70.0));
  BOOST_CONSTEXPR float_point_type yd(BOOST_FLOAT64_C(456.0) / BOOST_FLOAT64_C(110.0));

  const fixed_point_type z_mul_control(xd * yd);
  const fixed_point_type z_div_control(xd / yd);

  BOOST_CHECK_CLOSE_FRACTION(float_point_type(z_mul), float_point_type(z_mul_control), tol); // 5.9220779220779220779220779220779
  BOOST_CHECK_CLOSE_FRACTION(float_point_type(z_div), float_point_type(z_div_control), tol); // 0.34461152882205513784461152882206

  BOOST_CHECK_EQUAL(z_mul.crepresentation(), z_mul_control.crepresentation()); // UINT64_C(0x00BD81A98EF606A8)
  BOOST_CHECK_EQUAL(z_div.crepresentation(), z_div_control.crepresentation()); // UINT64_C(0x000B070EC1C3B071)

  BOOST_CHECK_EQUAL(z_mul.crepresentation(), UINT64_C(0x00BD81A98EF606A8));
  BOOST_CHECK_EQUAL(z_div.crepresentation(), UINT64_C(0x000B070EC1C3B071));
}
