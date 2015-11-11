///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the nextafter function of (fixed_point) for 128 digit exponent (like in float32_t).

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_func_nextafter_big
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_func_nextafter_big)
{
  typedef boost::fixed_point::negatable<7, -120> fixed_point_type;

  using std::ldexp;

  const fixed_point_type increment = ldexp(fixed_point_type(1), -120);

  // Check positive argument.
  fixed_point_type x(0);
  fixed_point_type y = fixed_point_type(1) / 10;

  x = nextafter(x, y);

  BOOST_CHECK_EQUAL(x, increment);

  // Check negative argument.
  x = nextafter(fixed_point_type(0), -y);

  BOOST_CHECK_EQUAL(x, -increment);

  // Check zero argument.
  x = nextafter(fixed_point_type(0), fixed_point_type(0));

  BOOST_CHECK_EQUAL(x, fixed_point_type(0));

  // Check identical arguments.
  x = nextafter(fixed_point_type(0.25F), fixed_point_type(0.25F));

  BOOST_CHECK_EQUAL(x, fixed_point_type(0.25F));

  x = nextafter(fixed_point_type(-0.25F), fixed_point_type(-0.25F));

  BOOST_CHECK_EQUAL(x, fixed_point_type(-0.25F));

  // Check the prior to maximum argument.
  x = nextafter((std::numeric_limits<fixed_point_type>::max)() - fixed_point_type(increment),
                (std::numeric_limits<fixed_point_type>::max)());

  // Check the maximum argument, returns max.
  BOOST_CHECK_EQUAL(x, (std::numeric_limits<fixed_point_type>::max)());
}
