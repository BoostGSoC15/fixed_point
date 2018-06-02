///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests basic mixed-math fixed_point negatable.

#define BOOST_TEST_MODULE test_negatable_basic_mixed_math
#define BOOST_LIB_DIAGNOSTIC

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

typedef boost::fixed_point::negatable<12,  -8> fixed_point_type_more_range;
typedef boost::fixed_point::negatable<10, -10> fixed_point_type_more_resol;


BOOST_AUTO_TEST_CASE(test_negatable_basic_mixed_math)
{
  {
    const fixed_point_type_more_range a(4.25F);
    const fixed_point_type_more_resol b(2.5F);

    const auto c = a + b;

    const decltype(c) control(6.75F);

    BOOST_CHECK_EQUAL(c, control);
  }

  {
    const fixed_point_type_more_resol a(4.25F);
    const fixed_point_type_more_range b(2.5F);

    const auto c = a + b;

    const decltype(c) control(6.75F);

    BOOST_CHECK_EQUAL(c, control);
  }

  {
    const fixed_point_type_more_range a(4.25F);
    const fixed_point_type_more_resol b(2.5F);

    const auto c = a - b;

    const decltype(c) control(1.75F);

    BOOST_CHECK_EQUAL(c, control);
  }

  {
    const fixed_point_type_more_resol a(4.25F);
    const fixed_point_type_more_range b(2.5F);

    const auto c = a - b;

    const decltype(c) control(1.75F);

    BOOST_CHECK_EQUAL(c, control);
  }

  {
    const fixed_point_type_more_range a(4.25F);
    const fixed_point_type_more_resol b(2.5F);

    const auto c = a * b;

    const decltype(c) control(10.625F);

    BOOST_CHECK_EQUAL(c, control);
  }

  {
    const fixed_point_type_more_resol a(4.25F);
    const fixed_point_type_more_range b(2.5F);

    const auto c = a * b;

    const decltype(c) control(10.625F);

    BOOST_CHECK_EQUAL(c, control);
  }

  {
    const fixed_point_type_more_range a(4.5F);
    const fixed_point_type_more_resol b(2.25F);

    const auto c = a / b;

    const decltype(c) control(2.0F);

    BOOST_CHECK_EQUAL(c, control);
  }

  {
    const fixed_point_type_more_resol a(4.5F);
    const fixed_point_type_more_range b(2.25F);

    const auto c = a / b;

    const decltype(c) control(2.0F);

    BOOST_CHECK_EQUAL(c, control);
  }
}
