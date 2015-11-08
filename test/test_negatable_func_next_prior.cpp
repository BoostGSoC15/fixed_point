///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the fixed_point functions next, prior, distance, advance.

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_func_nextafter
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/fixed_point/fixed_point_negatable_next.hpp>

#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_func_next_prior)
{
  typedef boost::fixed_point::negatable<7, -8> fixed_point_type;

  //using std::nextafter;
  using boost::fixed_point::fixed_next;
  using boost::fixed_point::fixed_prior;
  using boost::fixed_point::fixed_advance;
  using boost::fixed_point::fixed_distance;

  fixed_point_type x(1);
  fixed_point_type xn = fixed_next(x);
  fixed_point_type xp = fixed_prior(x);
  BOOST_CHECK(fixed_prior(xn) == x);
  BOOST_CHECK(fixed_next(xp) == x);
  BOOST_CHECK(fixed_distance(xp, xn) == 2);
  BOOST_CHECK(fixed_advance(xp, 2) == xn);

  // TODO some more tests needed here for edge cases, and also fixed using multiprecision types.

 /*
  // Check the maximum argument.
  x = nextafter((std::numeric_limits<fixed_point_type>::max)() - fixed_point_type(increment),
    (std::numeric_limits<fixed_point_type>::max)());
  BOOST_CHECK_EQUAL(x, (std::numeric_limits<fixed_point_type>::max)());

  BOOST_CHECK_EQUAL(nextafter((std::numeric_limits<fixed_point_type>::max)(),(std::numeric_limits<fixed_point_type>::max)()),
    (std::numeric_limits<fixed_point_type>::max)());

  BOOST_CHECK_EQUAL(nextafter((std::numeric_limits<fixed_point_type>::lowest)(),(std::numeric_limits<fixed_point_type>::lowest)()),
    (std::numeric_limits<fixed_point_type>::lowest)());

*/
}
