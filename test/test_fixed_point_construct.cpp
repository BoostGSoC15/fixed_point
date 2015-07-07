// Copyright Paul A. Bristow 2015
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


//! \file
//!\brief Basic tests for fixed_point.

#define BOOST_TEST_MODULE fixed_point_basic
#define BOOST_LIB_DIAGNOSTIC
//  Linking to lib file: libboost_unit_test_framework-vc120-mt-gd-1_59.lib

#include <boost/test/included/unit_test.hpp> // No Boost.Test library.
//#include <boost/test/unit_test.hpp> // USe Boost.Test library.

#include <iostream>
#include <iomanip>
#include <sstream>

#include "boost/fixed_point/fixed_point.hpp"

BOOST_AUTO_TEST_CASE(fixed_point_construct_nearest_even_round)
{
  //! small range and resolution
  {
  	typedef boost::fixed_point::negatable<4, -2, boost::fixed_point::round::nearest_even> fixed_point_type_nearest_even_round;
    fixed_point_type_nearest_even_round x = fixed_point_type_nearest_even_round (-1.375);
    fixed_point_type_nearest_even_round y = fixed_point_type_nearest_even_round(-1.5);    

    BOOST_CHECK_EQUAL(x, y);

    x = fixed_point_type_nearest_even_round (1.375);
    y = fixed_point_type_nearest_even_round (1.5);

    BOOST_CHECK_EQUAL(x, y);

    BOOST_CHECK_EQUAL(fixed_point_type_nearest_even_round::all_bits, 7);
    BOOST_CHECK_EQUAL(fixed_point_type_nearest_even_round::range, 4);
    BOOST_CHECK_EQUAL(fixed_point_type_nearest_even_round::resolution, -2);
    BOOST_CHECK_EQUAL(x.all_bits, 7);
    BOOST_CHECK_EQUAL(y.range, 4);
    BOOST_CHECK_EQUAL(x.resolution, -2);
  }

  //! larger range and small resolution
  {
    typedef boost::fixed_point::negatable<400, -2, boost::fixed_point::round::nearest_even> fixed_point_type_nearest_even_round;
    fixed_point_type_nearest_even_round x = fixed_point_type_nearest_even_round (-1.375);
    fixed_point_type_nearest_even_round y = fixed_point_type_nearest_even_round(-1.5);    

    BOOST_CHECK_EQUAL(x, y);

    x = fixed_point_type_nearest_even_round (1.375);
    y = fixed_point_type_nearest_even_round (1.5);

    BOOST_CHECK_EQUAL(x, y);

    BOOST_CHECK_EQUAL(fixed_point_type_nearest_even_round::all_bits, 403);
    BOOST_CHECK_EQUAL(fixed_point_type_nearest_even_round::range, 400);
    BOOST_CHECK_EQUAL(fixed_point_type_nearest_even_round::resolution, -2);
  }

  //! larger range and larger resolution
  {
  	typedef boost::fixed_point::negatable<87, -4, boost::fixed_point::round::nearest_even> fixed_point_type_nearest_even_round;
    fixed_point_type_nearest_even_round x = fixed_point_type_nearest_even_round (-605.28125);
    fixed_point_type_nearest_even_round y = fixed_point_type_nearest_even_round(-605.25);

    BOOST_CHECK_EQUAL(x, y);

    x = fixed_point_type_nearest_even_round (605.28125);
    y = fixed_point_type_nearest_even_round (605.25);

    BOOST_CHECK_EQUAL(x, y);

    BOOST_CHECK_EQUAL(fixed_point_type_nearest_even_round::all_bits, 92);
    BOOST_CHECK_EQUAL(fixed_point_type_nearest_even_round::range, 87);
    BOOST_CHECK_EQUAL(fixed_point_type_nearest_even_round::resolution, -4);
  }  
}

BOOST_AUTO_TEST_CASE(fixed_point_construct_fastest_round)
{
  //! small range and resolution
  {
  	typedef boost::fixed_point::negatable<4, -2, boost::fixed_point::round::fastest> fixed_point_type_fastest_round;
    fixed_point_type_fastest_round x = fixed_point_type_fastest_round (-1.33);
    fixed_point_type_fastest_round y = fixed_point_type_fastest_round(-1.25);
    fixed_point_type_fastest_round z = fixed_point_type_fastest_round(-1.5);

    BOOST_CHECK_EQUAL(x == y || x == z, true);

    x = fixed_point_type_fastest_round (1.33);
    y = fixed_point_type_fastest_round (1.25);
    z = fixed_point_type_fastest_round (1.5);

    BOOST_CHECK_EQUAL(x == y || x == z, true);
  }

  //! larger range and small resolution
  {
    typedef boost::fixed_point::negatable<400, -2, boost::fixed_point::round::fastest> fixed_point_type_fastest_round;
    fixed_point_type_fastest_round x = fixed_point_type_fastest_round (-1.33);
    fixed_point_type_fastest_round y = fixed_point_type_fastest_round(-1.25);
    fixed_point_type_fastest_round z = fixed_point_type_fastest_round(-1.5);

    BOOST_CHECK_EQUAL(x == y || x == z, true);

    x = fixed_point_type_fastest_round (1.33);
    y = fixed_point_type_fastest_round (1.25);
    z = fixed_point_type_fastest_round (1.5);

    BOOST_CHECK_EQUAL(x == y || x == z, true);
  }

  //! larger range and larger resolution
  {
  	typedef boost::fixed_point::negatable<87, -4, boost::fixed_point::round::fastest> fixed_point_type_fastest_round;
    fixed_point_type_fastest_round x = fixed_point_type_fastest_round (-6053.2777);
    fixed_point_type_fastest_round y = fixed_point_type_fastest_round(-6053.3125);
    fixed_point_type_fastest_round z = fixed_point_type_fastest_round(-6053.25);

    BOOST_CHECK_EQUAL(x == y || x == z, true);

    x = fixed_point_type_fastest_round (6053.2777);
    y = fixed_point_type_fastest_round (6053.3125);
    z = fixed_point_type_fastest_round (6053.25);

    BOOST_CHECK_EQUAL(x == y || x == z, true);
  }  
}
