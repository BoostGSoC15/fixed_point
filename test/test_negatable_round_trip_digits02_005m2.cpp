///////////////////////////////////////////////////////////////////////////////
//  Copyright Paul Bristow 2015.
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests round-trip for fixed_point negatable with 5.2 binary digits.

#define BOOST_TEST_MODULE test_negatable_round_trip_digits02_005m2
#define BOOST_LIB_DIAGNOSTIC

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  // Define a binary fixed-point type with resolution 5 range -2
  typedef boost::fixed_point::negatable<5, -2> fixed_point_type;

  bool round_trip(const fixed_point_type& x);

  // Here we need to invert Kahan's formula to ensure that all of
  // the fractional digits can be printed in decimal form.
  // In this case, we need to print two decimal digits10,
  // even tough we are only have two binary digits2.

  // Begin with
  //   digits2 = 8 (what we would have needed for two decimal digits)
  // This gives
  //  digits10 = [(8 - 1) * 301] / 1000
  //  digits10 = 2107 / 1000 = 2

  // In this case, we simply hard-code the value of 2 for decimal digits
  // in the fixed-point field.
  BOOST_CONSTEXPR_OR_CONST int digits10 = (-fixed_point_type::resolution);
}

bool local::round_trip(const local::fixed_point_type& x)
{
  typedef local::fixed_point_type fixed_point_type;

  std::stringstream ss1;

  ss1.precision(local::digits10);

  ss1 << std::fixed
      << x;

  std::stringstream ss2(ss1.str());

  fixed_point_type y;
  ss2 >> y;

  const bool b(x == y);

  return b;
}

BOOST_AUTO_TEST_CASE(test_negatable_round_trip_digits02_005m2)
{
  typedef local::fixed_point_type      fixed_point_type;
  typedef fixed_point_type::float_type floating_point_type;

  boost::uint_fast16_t count;

  BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t number_of_test_cases = UINT16_C(40);

  bool b = true;

  // Test sequential values with 5.2 binary digits of precision
  // ranging from 0.25, 0.50, 0.75, ... 9.75.
  for(count = UINT16_C(1); ((count < number_of_test_cases) && b); ++count)
  {
    std::stringstream ss1;

    ss1 << std::fixed
        << std::setprecision(local::digits10)
        << (float(count) / 4.0F);

    std::string str(ss1.str());

    const fixed_point_type x(boost::lexical_cast<floating_point_type>(str));

    const bool next_test_result = local::round_trip(local::fixed_point_type(x));

    b = (b && next_test_result);
  }

  BOOST_CHECK_EQUAL(count, number_of_test_cases);

  BOOST_CHECK_EQUAL(b, true);
}
