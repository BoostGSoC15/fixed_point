///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the hyperbolic arccosine function of (fixed_point) for a small digit range.

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_func_hyperbolic_arccosine_small
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_func_hyperbolic_arccosine_small)
{
  typedef boost::fixed_point::negatable<7, -24> fixed_point_type;
  typedef fixed_point_type::float_type          float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 4);

  using std::acosh;

  // Check positive arguments.
  for(int i = 0; i < 16; ++i)
  {
    const fixed_point_type x = acosh(1 + (fixed_point_type(i) / fixed_point_type(3.1415926535897932385L)));
    const float_point_type y = acosh(1 + (float_point_type(i) / float_point_type(3.1415926535897932385L)));

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  fixed_point_type x;
  float_point_type y;

  // Check a positive argument clost to 1.
  x = acosh(1 + (1 / (fixed_point_type(97) / 10)));
  y = acosh(1 + (1 / (float_point_type(97) / 10)));

  BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);

  // Check an invalid zero argument.
  x = acosh(0);
  y = float_point_type(0);

  BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);

  // Check an invalid argument that lies between 0 < x < 1.
  x = acosh(fixed_point_type(1) / 2);
  y = float_point_type(0);

  BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
}
