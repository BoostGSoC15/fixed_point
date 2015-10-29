///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the is_fixed_point structure of (boost::fixed_point).

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_basic_is_fixed_point
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_basic_is_fixed_point)
{
  typedef boost::fixed_point::negatable<7, -8> fixed_point_type;

  bool b;

  b = boost::fixed_point::is_fixed_point<fixed_point_type>::value;
  BOOST_CHECK_EQUAL(b, true);

  b = boost::fixed_point::is_fixed_point<int>::value;
  BOOST_CHECK_EQUAL(b, false);

  b = boost::fixed_point::is_fixed_point<float>::value;
  BOOST_CHECK_EQUAL(b, false);
}
