//  Copyright Paul A. Bristow 2015.
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)


//! \file
//!\brief Test numeric_limits for same fixed_point types.

#define BOOST_TEST_MODULE fixed_point_basic
#define BOOST_LIB_DIAGNOSTIC
//  Linking to lib file: libboost_unit_test_framework-vc120-mt-gd-1_59.lib

//#include <boost/test/included/unit_test.hpp> // No Boost.Test library.
#include <boost/test/unit_test.hpp> // USe Boost.Test library.

#include <iostream>
#include <iomanip>
#include <sstream>

#include "boost/fixed_point/fixed_point.hpp"

BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_min)
{ 
  {//! small range 2 and resolution 5 + sign = 8 bit, default round and overflow.
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;
    fixed_point_type_2m5 x = (std::numeric_limits<fixed_point_type_2m5>::min)();
    BOOST_CHECK_EQUAL(x.all_bits, 8);
    fixed_point_type_2m5 m = 0.03125;
    // std::cout << m.bit_pattern() << std::endl;
    BOOST_CHECK_EQUAL(m.bit_pattern(), "00000001");
    BOOST_CHECK_EQUAL(m.bit_pattern().size(), x.range + (-x.resolution) + 1); // 8 bits.
    BOOST_CHECK_EQUAL(x, m);
    int e;
    fixed_point_type_2m5 s = frexp(x, &e);

    //std::cout << s << " " << e << std::endl;
    BOOST_CHECK_EQUAL(e, -5 + 1);
    BOOST_CHECK_EQUAL(s, 0.5);
  }
} // BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_min)


BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_max)
{ //! small range 2 and resolution 5 + sign = 8 bit, default round and overflow.
  {
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;

    fixed_point_type_2m5 x = (std::numeric_limits<fixed_point_type_2m5>::max)();
    BOOST_CHECK_EQUAL(x.all_bits, 8);
    fixed_point_type_2m5 m = 3.969; // 
     std::cout << m.bit_pattern() << std::endl;
    BOOST_CHECK_EQUAL(m.bit_pattern(), "01111111"); // All except sign bit set.
    BOOST_CHECK_EQUAL(m.bit_pattern().size(), x.range + (-x.resolution) + 1); // 8 bits.
    BOOST_CHECK_EQUAL(x, m);
    int e;
    fixed_point_type_2m5 s = frexp(x, &e);

    std::cout << s << " " << e << std::endl;
    BOOST_CHECK_EQUAL(e, 2);
    BOOST_CHECK_EQUAL(s, 1 - 0.03125);
   // BOOST_CHECK_EQUAL(s, 0.96875);
  }
} // BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_max)


BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_lowest)
{ //! small range 2 and resolution 5 + sign = 8 bit, default round and overflow.
  {
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;

    fixed_point_type_2m5 x = (std::numeric_limits<fixed_point_type_2m5>::lowest)();
    fixed_point_type_2m5 m = (std::numeric_limits<fixed_point_type_2m5>::max)();

    BOOST_CHECK_EQUAL(x, -m);

    //BOOST_CHECK_EQUAL(x.all_bits, 8);
    //fixed_point_type_2m5 m = 3.969; // 
    //std::cout << m.bit_pattern() << std::endl;
    //BOOST_CHECK_EQUAL(m.bit_pattern(), "01111111"); // All except sign bit set.
    //BOOST_CHECK_EQUAL(m.bit_pattern().size(), x.range + (-x.resolution) + 1); // 8 bits.
    //BOOST_CHECK_EQUAL(x, m);
    //int e;
    //fixed_point_type_2m5 s = frexp(x, &e);

    //std::cout << s << " " << e << std::endl;
    //BOOST_CHECK_EQUAL(e, 2);
    //BOOST_CHECK_EQUAL(s, 1 - 0.03125);
    //// BOOST_CHECK_EQUAL(s, 0.96875);
  }
} // BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_lowest)


BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_misc)
{
  {
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;

    fixed_point_type_2m5 y(1);
      int e;
    fixed_point_type_2m5 s = frexp(y, &e);
    std::cout << s << " " << e << std::endl; // 0.5 1
  }
  {
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;
    fixed_point_type_2m5 y;
    BOOST_CHECK_EQUAL(y, 0);

    y = ldexp(2, 2);
    std::cout << y << std::endl;
  }
} // BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_misc)



BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_value_float)
{
  {
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;

    std::cout << typeid(fixed_point_type_2m5::value_type).name() << std::endl;
    BOOST_CHECK_EQUAL(typeid(fixed_point_type_2m5::value_type).name(), "signed char");

    std::cout << typeid(fixed_point_type_2m5::float_type).name() << std::endl;
    BOOST_CHECK_EQUAL(typeid(fixed_point_type_2m5::float_type).name(), "float");
  }


} // BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_value_float)


