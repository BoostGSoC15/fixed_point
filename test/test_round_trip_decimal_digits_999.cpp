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

#define BOOST_TEST_MODULE round_trip_decimal_digits_100
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
  // Define a binary fixed-point type with 999 decimal digits of precision.
  typedef
  boost::fixed_point::negatable<0,
                                -3320,
                                boost::fixed_point::round::nearest_even>
  fixed_point_type_decimal_digits_999;

  bool round_trip(const fixed_point_type_decimal_digits_999& x);
}

bool local::round_trip(const local::fixed_point_type_decimal_digits_999& x)
{
  typedef local::fixed_point_type_decimal_digits_999 fixed_point_type;

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

BOOST_AUTO_TEST_CASE(fixed_point_type_decimal_digits_999)
{
  typedef local::fixed_point_type_decimal_digits_999 fixed_point_type;
  typedef fixed_point_type::float_type floating_point_type;

  typedef boost::mt19937 random_generator_type;

  random_generator_type random_generator;

  boost::uniform_int<unsigned> uniform_bit_range(0U, 1U);

  boost::variate_generator<random_generator_type,
                           boost::uniform_int<unsigned>>
  radom_bit_maker(random_generator, uniform_bit_range);

  uint_fast32_t count;

  bool b = true;

  // Test random values with 999 decimal digits of precision.
  for(count = UINT32_C(1); ((count < UINT32_C(10000)) && b); ++count)
  {
    fixed_point_type::value_type v(0);

    for(int i = 0; i < std::numeric_limits<fixed_point_type>::digits - 2; ++i)
    {
      v |= fixed_point_type::value_type(radom_bit_maker()) << i;
    }

    std::stringstream ss1;

    ss1 << v;

    std::string str(ss1.str());

    if(str.length() < std::string::size_type(999U))
    {
      str.insert(std::string::size_type(0U),
                 std::string::size_type(999U) - str.length(),
                 char('0'));
    }

    str = ("0." + str);

    const floating_point_type x = floating_point_type(str);

    const fixed_point_type fx(x);

    const bool next_test_result =
      local::round_trip(local::fixed_point_type_decimal_digits_999(fx));

    b = (b && next_test_result);
  }

  BOOST_CHECK_EQUAL(count, UINT32_C(10000));

  BOOST_CHECK_EQUAL(b, true);
}
