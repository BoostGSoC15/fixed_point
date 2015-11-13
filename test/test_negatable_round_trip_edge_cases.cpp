///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for various edge cases of round-tripping for fixed_point for 15 binary digits of precison.

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#define BOOST_TEST_MODULE test_negatable_round_trip_edge_cases
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_round_trip_edge_cases)
{
  // Use round mode nearest_even becaust this is what
  // the compiler uses for its default rounding mode
  // for built-in floating-point types.
  typedef boost::fixed_point::negatable<0, -15, boost::fixed_point::round::nearest_even> fixed_point_type;

  const fixed_point_type x(0.9375F);

  std::stringstream stream1;

  // Here we use Kahans formula for digits10.
  //   digits10 = ((digits - 1) * 301) / 1000 = 4.2... = approx. 4
  stream1 << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
          << std::fixed
          << x;

  std::stringstream stream2(stream1.str());

  fixed_point_type y;
  stream2 >> y;

  BOOST_CHECK_EQUAL(x, y);
}
