///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the trogonometric arccosine function of (fixed_point) for a small digit range.

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_trig_func_arccosine_small
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_trig_func_arccosine_small)
{
  typedef boost::fixed_point::negatable<7, -24> fixed_point_type;
  typedef fixed_point_type::float_type          float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 9);

  // Check positive arguments.
  for(int i = 0; i <= 32; ++i)
  {
    const fixed_point_type x = acos(fixed_point_type(i) / 32);

    using std::acos;
    const float_point_type y = acos(float_point_type(i) / 32);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  // Check negative arguments.
  for(int i = 0; i <= 32; ++i)
  {
    const fixed_point_type x = acos(fixed_point_type(-i) / 32);

    using std::acos;
    const float_point_type y = acos(float_point_type(-i) / 32);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }
}
