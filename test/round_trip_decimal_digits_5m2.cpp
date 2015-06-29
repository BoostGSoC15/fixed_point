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

#define BOOST_TEST_MODULE round_trip_decimal_digits_5m2
#define BOOST_LIB_DIAGNOSTIC

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <boost/cstdfloat.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/lexical_cast.hpp>
//#include <boost/test/included/unit_test.hpp>
#include <boost/test/unit_test.hpp>

namespace local
{
  // Define a binary fixed-point type with resolution 5 range -2

  // decimal digits of precision.
  typedef
    boost::fixed_point::negatable<5, -2,
    boost::fixed_point::round::nearest_even>
    fixed_point_type_decimal_digits_5m2;

  bool round_trip(const fixed_point_type_decimal_digits_5m2& x);
}

bool local::round_trip(const local::fixed_point_type_decimal_digits_5m2& x)
{
  typedef local::fixed_point_type_decimal_digits_5m2 fixed_point_type;

  std::ostringstream ss1;
  ss1 << std::fixed << std::showpoint;
  ss1 << x;


  //int p = 4;
  int p = 4;
  std::cout.precision(p);

  // Output 
  // ss1.precision(4);  fails!!  operator <<' is ambiguous
  //C:\Program Files (x86)\Microsoft Visual Studio 12.0\VC\include\ostream(1026):
  //or       'std::basic_ostream<char,std::char_traits<char>> &std::operator <<
  // <char,std::char_traits<char>>(std::basic_ostream<char,std::char_traits<char>> &,const std::error_code &)'
  // while trying to match the argument list 
  // '(std::stringstream, const local::fixed_point_type_decimal_digits_5m2)'

  //ss1.precision(std::numeric_limits<fixed_point_type>::digits10); // is OK.

  // using boost::fixed_point::operator<<; //

  ss1 << x;

  std::stringstream ss2(ss1.str());

  fixed_point_type y;
  ss2 >> y;

  const bool b(x == y);

  return b;
}

BOOST_AUTO_TEST_CASE(round_trip_decimal_digits_5m2)
{
  typedef local::fixed_point_type_decimal_digits_5m2 fixed_point_type;
  typedef fixed_point_type::float_type floating_point_type;

  uint_fast16_t count;

  bool b = true; // If no failures.

  // Test every single value with p decimal digits of precision
  // ranging from 0.001, 0.002, 0.003, ... 0.999.
  // for (count = UINT16_C(1); ((count < UINT16_C(999)) && b); ++count)
  for (count = 1; ((count < 128) && b); ++count)
  {
    std::stringstream ss1;

    ss1 << count;

    std::string str(ss1.str());

    str.insert(std::string::size_type(0U),
      std::string::size_type(3U) - str.length(),
      char('0'));

    str = ("0." + str);

    const floating_point_type x = boost::lexical_cast<floating_point_type>(str);

    const fixed_point_type fx(x);

    const bool next_test_result =
      local::round_trip(local::fixed_point_type_decimal_digits_5m2(fx));

    b = (b && next_test_result);
  }

  BOOST_CHECK_EQUAL(count, 128);

  BOOST_CHECK_EQUAL(b, true);
} // BOOST_AUTO_TEST_CASE(round_trip_decimal_digits_5m2)
