///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests round-trip for fixed_point negatable with 1 decimal digit.

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
<<<<<<< HEAD
    boost::fixed_point::negatable<12,  -8> a(4.25);
    boost::fixed_point::negatable<10, -10> b(2.5);
=======
    const fixed_point_type_more_range a(4.25F);
    const fixed_point_type_more_resol b(2.5F);
>>>>>>> develop

    const auto c = a + b;

<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> result(6.75);
=======
    const decltype(c) control(6.75F);
>>>>>>> develop

    BOOST_CHECK_EQUAL(c, control);
  }

  {
<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> a(4.25);
    boost::fixed_point::negatable<12,  -8> b(2.5);
=======
    const fixed_point_type_more_resol a(4.25F);
    const fixed_point_type_more_range b(2.5F);
>>>>>>> develop

    const auto c = a + b;

<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> result(6.75);
=======
    const decltype(c) control(6.75F);
>>>>>>> develop

    BOOST_CHECK_EQUAL(c, control);
  }

  {
<<<<<<< HEAD
    boost::fixed_point::negatable<12,  -8> a(4.25);
    boost::fixed_point::negatable<10, -10> b(2.5);
=======
    const fixed_point_type_more_range a(4.25F);
    const fixed_point_type_more_resol b(2.5F);
>>>>>>> develop

    const auto c = a - b;

<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> result(1.75);
=======
    const decltype(c) control(1.75F);
>>>>>>> develop

    BOOST_CHECK_EQUAL(c, control);
  }

  {
<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> a(4.25);
    boost::fixed_point::negatable<12,  -8> b(2.5);
=======
    const fixed_point_type_more_resol a(4.25F);
    const fixed_point_type_more_range b(2.5F);
>>>>>>> develop

    const auto c = a - b;

<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> result(1.75);
=======
    const decltype(c) control(1.75F);
>>>>>>> develop

    BOOST_CHECK_EQUAL(c, control);
  }

  {
<<<<<<< HEAD
    boost::fixed_point::negatable<12,  -8> a(4.25);
    boost::fixed_point::negatable<10, -10> b(2.5);
=======
    const fixed_point_type_more_range a(4.25F);
    const fixed_point_type_more_resol b(2.5F);
>>>>>>> develop

    const auto c = a * b;

<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> result(10.625);
=======
    const decltype(c) control(10.625F);
>>>>>>> develop

    BOOST_CHECK_EQUAL(c, control);
  }

  {
<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> a(4.25);
    boost::fixed_point::negatable<12,  -8> b(2.5);
=======
    const fixed_point_type_more_resol a(4.25F);
    const fixed_point_type_more_range b(2.5F);
>>>>>>> develop

    const auto c = a * b;

<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> result(10.625);
=======
    const decltype(c) control(10.625F);
>>>>>>> develop

    BOOST_CHECK_EQUAL(c, control);
  }

  {
<<<<<<< HEAD
    boost::fixed_point::negatable<12,  -8> a(4.5);
    boost::fixed_point::negatable<10, -10> b(2.25);
=======
    const fixed_point_type_more_range a(4.5F);
    const fixed_point_type_more_resol b(2.25F);
>>>>>>> develop

    const auto c = a / b;

<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> result(2);
=======
    const decltype(c) control(2.0F);
>>>>>>> develop

    BOOST_CHECK_EQUAL(c, control);
  }

  {
<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> a(4.5);
    boost::fixed_point::negatable<12,  -8> b(2.25);
=======
    const fixed_point_type_more_resol a(4.5F);
    const fixed_point_type_more_range b(2.25F);
>>>>>>> develop

    const auto c = a / b;

<<<<<<< HEAD
    boost::fixed_point::negatable<10, -10> result(2);
=======
    const decltype(c) control(2.0F);
>>>>>>> develop

    BOOST_CHECK_EQUAL(c, control);
  }
}
