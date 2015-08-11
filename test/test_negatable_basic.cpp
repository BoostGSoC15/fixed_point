// Copyright Paul A. Bristow 2015
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


//! \file
//!\brief Basic tests for fixed_point.

#include <iomanip>
#include <sstream>
#include <string>

#define BOOST_TEST_MODULE test_negatable_basic
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_basic)
{
  BOOST_TEST(true);

  {
    // Plain fixed-point
    typedef boost::fixed_point::negatable<32, -16> fixed_point_type;

    const fixed_point_type x = fixed_point_type(-100) / 100;

    std::ostringstream os;

    os << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
       << std::fixed
       << x;

    const std::string control_string =
        std::string("-1.")
      + std::string(std::string::size_type(std::numeric_limits<fixed_point_type>::digits10), char('0'));

    BOOST_CHECK_EQUAL(os.str(), control_string);
  }

  {
    // Plain fixed-point
    typedef boost::fixed_point::negatable<2, -13> fixed_point_type;

    const fixed_point_type x = fixed_point_type(-1) / 3;

    std::ostringstream os;

    os << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
       << std::fixed
       << x;

    const std::string control_string =
        std::string("-0.")
      + std::string(std::string::size_type(std::numeric_limits<fixed_point_type>::digits10), char('3'));

    BOOST_CHECK_EQUAL(os.str(), control_string);
  }

  {
    // fastest round
    typedef boost::fixed_point::negatable<13, -2> fixed_point_type_fastest_round;

    const fixed_point_type_fastest_round x = fixed_point_type_fastest_round(-1.26);

    std::ostringstream os;

    os << std::setprecision(std::numeric_limits<fixed_point_type_fastest_round>::digits10)
       << x;

    BOOST_CHECK_EQUAL(os.str(), std::string("-1.25"));
  }
}
