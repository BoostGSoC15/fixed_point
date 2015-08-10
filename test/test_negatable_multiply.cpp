///////////////////////////////////////////////////////////////////////////////
//  Copyright Paul Bristow 2015.
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//! \file
//!\brief Tests multiplication for fixed_point negatable.

#define BOOST_TEST_MODULE test_negatable_multiply
#define BOOST_LIB_DIAGNOSTIC

#include <iomanip>
#include <iostream>
#include <sstream>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_multiply)
{
  //! small range and resolution
  {
    typedef boost::fixed_point::negatable<4, -2, boost::fixed_point::round::fastest> fixed_point_type_fastest_round;
    fixed_point_type_fastest_round x = fixed_point_type_fastest_round (-1.25) * fixed_point_type_fastest_round (1.5);
    fixed_point_type_fastest_round y = fixed_point_type_fastest_round(-2);
    fixed_point_type_fastest_round z = fixed_point_type_fastest_round(-1.75);

    BOOST_CHECK_EQUAL(x == y || x == z, true);

    x = fixed_point_type_fastest_round (1.25) * fixed_point_type_fastest_round (1.5);
    y = fixed_point_type_fastest_round (1.75);
    z = fixed_point_type_fastest_round (2);

    BOOST_CHECK_EQUAL(x == y || x == z, true);
  }

  //! larger range and small resolution
  {
    typedef boost::fixed_point::negatable<400, -2, boost::fixed_point::round::fastest> fixed_point_type_fastest_round;
    fixed_point_type_fastest_round x = fixed_point_type_fastest_round (-1.25) * fixed_point_type_fastest_round (1.5);
    fixed_point_type_fastest_round y = fixed_point_type_fastest_round(-2);
    fixed_point_type_fastest_round z = fixed_point_type_fastest_round(-1.75);

    BOOST_CHECK_EQUAL(x == y || x == z, true);

    x = fixed_point_type_fastest_round (1.25) * fixed_point_type_fastest_round (1.5);
    y = fixed_point_type_fastest_round (1.75);
    z = fixed_point_type_fastest_round (2);

    BOOST_CHECK_EQUAL(x == y || x == z, true);
  }

  //! larger range and larger resolution
  {
    typedef boost::fixed_point::negatable<87, -4, boost::fixed_point::round::fastest> fixed_point_type_fastest_round;
    fixed_point_type_fastest_round x = fixed_point_type_fastest_round (800.4375) * fixed_point_type_fastest_round (-7.5625);
    fixed_point_type_fastest_round y = fixed_point_type_fastest_round(-6053.3125);
    fixed_point_type_fastest_round z = fixed_point_type_fastest_round(-6053.25);

    BOOST_CHECK_EQUAL(x == y || x == z, true);

    x = fixed_point_type_fastest_round (800.4375) * fixed_point_type_fastest_round (7.5625);
    y = fixed_point_type_fastest_round (6053.3125);
    z = fixed_point_type_fastest_round (6053.25);

    BOOST_CHECK_EQUAL(x == y || x == z, true);
  }
}
