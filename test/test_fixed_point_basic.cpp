// Copyright Paul A. Bristow 2015
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


//! \file
//!\brief Basic tests for fixed_point.

#define BOOST_TEST_MODULE fixed_point_basic
#define BOOST_LIB_DIAGNOSTIC
//  Linking to lib file: libboost_unit_test_framework-vc120-mt-gd-1_59.lib
// boost_unit_test_framework-mgw48-mt-s-1_59

//#include <boost/test/included/unit_test.hpp> // No Boost.Test library.
#include <boost/test/unit_test.hpp> // USe Boost.Test library.

#include <iostream>
#include <iomanip>
#include <sstream>

#include "boost/fixed_point/fixed_point.hpp"

typedef boost::fixed_point::negatable<87, -2, boost::fixed_point::round::negative> fixed_point_type_negative_round;
typedef boost::fixed_point::negatable<87, -2> fixed_point_type_fastest_round;
typedef boost::fixed_point::negatable<87, -2, boost::fixed_point::round::positive> fixed_point_type_positive_round;

BOOST_AUTO_TEST_CASE(fixed_point_basic)
{

  BOOST_TEST(true);

  { // Plain fixed-point
    typedef boost::fixed_point::negatable<32, -16> fixed_point_type;

    fixed_point_type x123 = fixed_point_type(-100) / 100;

    std::cout << x123 << std::endl; // -1 with default ostream settings.
    std::ostringstream os;
    std::streamsize precision = os.precision(std::numeric_limits<fixed_point_type>::digits10);
    std::cout << "fixed_point_type precision digits10 = " << precision << std::endl;
    os << std::showpoint << x123; // Include any trailing zeros.
    BOOST_TEST_CHECK(os.str() == "-1.0000000000000");
  }
  {// fastest round

    fixed_point_type_fastest_round x = fixed_point_type_fastest_round(-1.26);
    std::cout << x << std::endl; //

    std::ostringstream os;
    os.precision(std::numeric_limits<fixed_point_type_fastest_round>::digits10);
    os << x;
    BOOST_TEST_CHECK(os.str() == "-1.25");
  }

}

/*

CustomBuildStep:
1>  Description: Autorun "J:\Cpp\fixed_point\Debug\test_fixed_point_basic.exe"
1>  Running 1 test case...
1>  11111111111111111111111111111111111111111111111111111111111111111111111111111111111111010
1>
1>  *** No errors detected

*/
