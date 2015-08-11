///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests round-trip for fixed_point negatable with 999 decimal digits.

#define BOOST_TEST_MODULE test_negatable_round_trip_digits10_999
#define BOOST_LIB_DIAGNOSTIC

#include <algorithm>
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
  fixed_point_type;

  BOOST_CONSTEXPR_OR_CONST std::string::size_type digits10_string_length =
    ((long(std::numeric_limits<fixed_point_type>::digits) - 2L) * 301L) / 1000L;

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

BOOST_AUTO_TEST_CASE(test_negatable_round_trip_digits10_999)
{
  using local::fixed_point_type;

  typedef fixed_point_type::float_type floating_point_type;

  typedef boost::mt19937 random_generator_type;

  const boost::uniform_int<boost::uint_fast8_t> uniform_bit_range(UINT8_C(0), UINT8_C(1));

  boost::variate_generator<random_generator_type,
                           boost::uniform_int<boost::uint_fast8_t>>
  radom_bit_maker(random_generator_type(), uniform_bit_range);

  boost::uint_fast32_t count = UINT32_C(0);

  BOOST_CONSTEXPR_OR_CONST boost::uint_fast32_t number_of_test_cases = UINT16_C(20000);

  bool b = true;

  // Test random values with 999 decimal digits of precision.
  for( ; ((count < number_of_test_cases) && b); ++count)
  {
    typedef
      boost::fixed_point::detail::integer_type_helper<fixed_point_type::all_bits>::exact_unsigned_type
    unsigned_integral_type;

    unsigned_integral_type u(0);

    for(int i = 0; i < int(local::digits10_string_length); ++i)
    {
      u |= unsigned_integral_type(radom_bit_maker()) << i;
    }

    std::stringstream ss1;

    ss1 << u;

    std::string str(ss1.str());

    str.insert(std::string::size_type(0U),
               local::digits10_string_length - (std::min)(local::digits10_string_length, str.length()),
               char('0'));

    const fixed_point_type x(boost::lexical_cast<floating_point_type>(str.insert(std::string::size_type(0U), "0.")));

    const bool next_test_result = local::round_trip(fixed_point_type(x));

    b = (b && next_test_result);
  }

  BOOST_CHECK_EQUAL(count, number_of_test_cases);

  BOOST_CHECK_EQUAL(b, true);
}
