///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the copysign function of (fixed_point).

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_func_copysign
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_func_copysign)
{
  typedef boost::fixed_point::negatable<7, -8> fixed_point_type;
  typedef fixed_point_type::float_type         float_point_type;

  using std::copysign;

  fixed_point_type x;
  float_point_type y;

  x = copysign(fixed_point_type(1), fixed_point_type(1));
  y = copysign(float_point_type(1), float_point_type(1));

  BOOST_CHECK_EQUAL(x, fixed_point_type(y));

  x = copysign(fixed_point_type(1), fixed_point_type(-1));
  y = copysign(float_point_type(1), float_point_type(-1));

  BOOST_CHECK_EQUAL(x, fixed_point_type(y));

  x = copysign(fixed_point_type(-1), fixed_point_type(1));
  y = copysign(float_point_type(-1), float_point_type(1));

  BOOST_CHECK_EQUAL(x, fixed_point_type(y));

  x = copysign(fixed_point_type(-1), fixed_point_type(-1));
  y = copysign(float_point_type(-1), float_point_type(-1));

  BOOST_CHECK_EQUAL(x, fixed_point_type(y));
}
