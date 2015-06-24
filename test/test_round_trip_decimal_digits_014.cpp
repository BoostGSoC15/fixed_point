///////////////////////////////////////////////////////////////////////////////
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Copyright Christopher Kormanyos 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// This file does round-trip testing for
// "C++ binary fixed-point arithmetic" as specified in N3352.
// See: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html

#define BOOST_TEST_MODULE round_trip_decimal_digits_014
#define BOOST_LIB_DIAGNOSTIC

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/cstdfloat.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  // Define a binary fixed-point type with 14 decimal digits of precision.
  typedef
  boost::fixed_point::negatable<0,
                                -48,
                                boost::fixed_point::round::nearest_even>
  fixed_point_type_decimal_digits_014;

  bool round_trip(const fixed_point_type_decimal_digits_014& x);
}

bool local::round_trip(const local::fixed_point_type_decimal_digits_014& x)
{
  typedef local::fixed_point_type_decimal_digits_014 fixed_point_type;

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

BOOST_AUTO_TEST_CASE(round_trip_decimal_digits_014)
{
  typedef local::fixed_point_type_decimal_digits_014 fixed_point_type;
  typedef fixed_point_type::float_type floating_point_type;

  typedef boost::mt19937 random_generator_type;

  random_generator_type random_generator;

  boost::uniform_int<boost::uint64_t> uniform_bit_range(UINT64_C(1), (UINT64_C(1) << 46));

  boost::variate_generator<random_generator_type,
                           boost::uniform_int<boost::uint64_t>>
  unsigned_integral_maker(random_generator, uniform_bit_range);

  uint_fast32_t count;

  bool b = true;

  // Test random values with 14 decimal digits of precision within the
  // range of 0.00000000000001, 0.00000000000002, ... 0.50000000000000.
  for(count = UINT32_C(1); ((count < UINT32_C(1000000)) && b); ++count)
  {
    std::stringstream ss1;

    const boost::uint64_t u = unsigned_integral_maker();

    ss1 << u;

    std::string str(ss1.str());

    if(str.length() < std::string::size_type(14U))
    {
      str.insert(std::string::size_type(0U),
                 std::string::size_type(14U) - str.length(),
                 char('0'));
    }

    str = ("0." + str);

    const floating_point_type x = boost::lexical_cast<floating_point_type>(str);

    const fixed_point_type fx(x);

    const bool next_test_result =
      local::round_trip(local::fixed_point_type_decimal_digits_014(fx));

    b = (b && next_test_result);
  }

  BOOST_CHECK_EQUAL(count, UINT32_C(1000000));

  BOOST_CHECK_EQUAL(b, true);
}
