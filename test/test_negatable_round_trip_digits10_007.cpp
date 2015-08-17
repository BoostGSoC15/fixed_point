///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests round-trip for fixed_point negatable with 7 decimal digits.

#define BOOST_TEST_MODULE test_negatable_round_trip_digits10_007
#define BOOST_LIB_DIAGNOSTIC

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/cstdint.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  // Define a binary fixed-point type with 7 decimal digits of precision.
  typedef
  boost::fixed_point::negatable<0,
                                -25,
                                boost::fixed_point::round::nearest_even>
  fixed_point_type;

  bool round_trip(const fixed_point_type& x);
}

bool local::round_trip(const local::fixed_point_type& x)
{
  using local::fixed_point_type;

  std::stringstream ss1;

  ss1 << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
      << std::fixed
      << x;

  std::stringstream ss2(ss1.str());

  fixed_point_type y;
  ss2 >> y;

  const bool b(x == y);

  return b;
}

BOOST_AUTO_TEST_CASE(test_negatable_round_trip_digits10_007)
{
  using local::fixed_point_type;

  typedef fixed_point_type::float_type floating_point_type;

  boost::uint_fast32_t count;

  BOOST_CONSTEXPR_OR_CONST boost::uint_fast32_t range_of_test_cases = UINT32_C(9999996);

  bool b = true;

  // Test every 17th value with 7 decimal digits of precision
  // ranging from 0.0000001, 0.00000018, 0.0000035, ... 0.9999996.
  for(count = UINT32_C(1); ((count < range_of_test_cases) && b); count += UINT32_C(17))
  {
    std::stringstream ss1;

    ss1 << count;

    std::string str(ss1.str());

    str.insert(std::string::size_type(0U),
               std::string::size_type(7U) - ((std::min)(std::string::size_type(7U), str.length())),
               char('0'));

    const fixed_point_type x(boost::lexical_cast<floating_point_type>(str.insert(std::string::size_type(0U), "0.")));

    const bool next_test_result = local::round_trip(local::fixed_point_type(x));

    b = (b && next_test_result);
  }

  BOOST_CHECK_EQUAL(count, range_of_test_cases);

  BOOST_CHECK_EQUAL(b, true);
}
