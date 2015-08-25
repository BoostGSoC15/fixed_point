///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests round-trip for fixed_point negatable with 1 decimal digit.

#define BOOST_TEST_MODULE test_negatable_mixed_math
#define BOOST_LIB_DIAGNOSTIC

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>


BOOST_AUTO_TEST_CASE(test_negatable_mixed_math)
{
  {
    boost::fixed_point::negatable<10,  -8> a(4.25);
    boost::fixed_point::negatable<20, -10> b(2.5);

    auto c = a + b;

    boost::fixed_point::negatable<20, -10> result(6.75);

    BOOST_CHECK_EQUAL(c, result);
  }

  {
    boost::fixed_point::negatable<100,  -8> a(4.25);
    boost::fixed_point::negatable< 20, -10> b(2.5);

    auto c = a + b;

    boost::fixed_point::negatable<20, -10> result(6.75);

    BOOST_CHECK_EQUAL(c, result);
  }

  {
    boost::fixed_point::negatable<10,  -8> a(4.25);
    boost::fixed_point::negatable<20, -10> b(2.5);

    auto c = a - b;

    boost::fixed_point::negatable<20, -10> result(1.75);

    BOOST_CHECK_EQUAL(c, result);
  }

  {
    boost::fixed_point::negatable<100,  -8> a(4.25);
    boost::fixed_point::negatable< 20, -10> b(2.5);

    auto c = a - b;

    boost::fixed_point::negatable<20, -10> result(1.75);

    BOOST_CHECK_EQUAL(c, result);
  }

  {
    boost::fixed_point::negatable<10,  -8> a(4.25);
    boost::fixed_point::negatable<20, -10> b(2.5);

    auto c = a * b;

    boost::fixed_point::negatable<20, -10> result(10.625);

    BOOST_CHECK_EQUAL(c, result);
  }

  {
    boost::fixed_point::negatable<100,  -8> a(4.25);
    boost::fixed_point::negatable< 20, -10> b(2.5);

    auto c = a * b;

    boost::fixed_point::negatable<20, -10> result(10.625);

    BOOST_CHECK_EQUAL(c, result);
  }

  {
    boost::fixed_point::negatable<10,  -8> a(4.5);
    boost::fixed_point::negatable<20, -10> b(2.25);

    auto c = a / b;

    boost::fixed_point::negatable<20, -10> result(2);

    BOOST_CHECK_EQUAL(c, result);
  }

  {
    boost::fixed_point::negatable<100,  -8> a(4.5);
    boost::fixed_point::negatable< 20, -10> b(2.25);

    auto c = a / b;

    boost::fixed_point::negatable<20, -10> result(2);

    BOOST_CHECK_EQUAL(c, result);
  }
}
