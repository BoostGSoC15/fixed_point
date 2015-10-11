///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the trogonometric cosine function of (fixed_point) for a small digit range.

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_func_cosine_small
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_func_cosine_small)
{
  typedef boost::fixed_point::negatable<7, -24> fixed_point_type;
  typedef fixed_point_type::float_type          float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 7);

  // Check positive arguments.
  for(int i = 1; i < 64; ++i)
  {
    const fixed_point_type x = cos(fixed_point_type(i) / 10);

    using std::cos;
    const float_point_type y = cos(float_point_type(i) / 10);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  // Check negative arguments.
  for(int i = 1; i < 64; ++i)
  {
    const fixed_point_type x = cos(fixed_point_type(-i) / 10);

    using std::cos;
    const float_point_type y = cos(float_point_type(-i) / 10);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  const fixed_point_type local_pi_half = boost::fixed_point::negatable_constants<fixed_point_type>::pi_half();

  BOOST_CHECK_EQUAL(cos(+local_pi_half), fixed_point_type(0));
  BOOST_CHECK_EQUAL(cos(-local_pi_half), fixed_point_type(0));

  BOOST_CHECK_EQUAL(cos(fixed_point_type(0)), fixed_point_type(1));

  const fixed_point_type local_pi = boost::fixed_point::negatable_constants<fixed_point_type>::pi();

  BOOST_CHECK_EQUAL(cos(+local_pi), fixed_point_type(0));
  BOOST_CHECK_EQUAL(cos(-local_pi), fixed_point_type(0));
}
