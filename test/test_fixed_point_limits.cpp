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

using namespace boost::fixed_point;


BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_min)
{ 
  std::cout.setf(std::ios_base::boolalpha | std::ios_base::showpoint); // Show any trailing zeros.
  {//! small range 2 and resolution 5 + sign = 8 bit, default round and overflow.

    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;
    fixed_point_type_2m5 x = (std::numeric_limits<fixed_point_type_2m5>::min)();
    BOOST_CHECK_EQUAL(x.all_bits, 8);
    BOOST_CHECK_EQUAL(x.range, 2);
    BOOST_CHECK_EQUAL(x.resolution, -5);
    fixed_point_type_2m5 m = x; // = 0.03125;
    std::cout << m.bit_pattern() << std::endl;
    BOOST_CHECK_EQUAL(m.bit_pattern(), "00000001");
    BOOST_CHECK_EQUAL(m.bit_pattern().size(), x.range + (-x.resolution) + 1); // 8 bits.
    BOOST_CHECK_EQUAL(x, m);
    int e;
    fixed_point_type_2m5 s = frexp(x, &e);

    //std::cout << s << " " << e << std::endl;
    BOOST_CHECK_EQUAL(e, -5 + 1);
    BOOST_CHECK_EQUAL(s, 0.5);
  }
  { //range 15 and resolution 16 + sign = 32 bit, default round and overflow.
    typedef boost::fixed_point::negatable<15, -16> fixed_point_type_15m16;

    fixed_point_type_15m16 x = (std::numeric_limits<fixed_point_type_15m16>::min)();
    BOOST_CHECK_EQUAL(x.all_bits, 32);
    BOOST_CHECK_EQUAL(x.range, 15);
    BOOST_CHECK_EQUAL(x.resolution, -16);
    fixed_point_type_15m16 m = x;
    std::cout << m.bit_pattern() << std::endl;
    BOOST_CHECK_EQUAL(m.bit_pattern(), "00000000000000000000000000000001");
    BOOST_CHECK_EQUAL(m.bit_pattern().size(), x.range + (-x.resolution) + 1); // 32 bits.
    BOOST_CHECK_EQUAL(x, m);
    int e;
    fixed_point_type_15m16 s = frexp(x, &e);

    BOOST_CHECK_EQUAL(e, -16 + 1);
    BOOST_CHECK_EQUAL(s, 0.5);
  }


} // BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_min)


BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_max)
{
  std::cout.setf(std::ios_base::boolalpha | std::ios_base::showpoint); // Show any trailing zeros.


  { //! small range 2 and resolution 5 + sign = 8 bit, default round and overflow.
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;
    std::cout.precision(std::numeric_limits<fixed_point_type_2m5>::digits10);

    fixed_point_type_2m5 x = (std::numeric_limits<fixed_point_type_2m5>::max)();
    BOOST_CHECK_EQUAL(x.all_bits, 8); // 8 bits used.
    fixed_point_type_2m5 m = 3.969; // 
     std::cout << m.bit_pattern() << std::endl;
    BOOST_CHECK_EQUAL(m.bit_pattern(), "01111111"); // All except sign bit set.
    BOOST_CHECK_EQUAL(m.bit_pattern().size(), x.range + (-x.resolution) + 1); // 8 bits.
    BOOST_CHECK_EQUAL(x, m);
    int e;
    fixed_point_type_2m5 s = frexp(x, &e);
    fixed_point_type_2m5 eps = std::numeric_limits<fixed_point_type_2m5>::epsilon();
    fixed_point_type_2m5 ulp = eps/2;
    BOOST_CHECK_EQUAL(ulp, 0.03125);
    std::cout << "frac " << s << ", exp =  " << e <<", eps = " << eps << ", ulp = " << ulp << std::endl;
    fixed_point_type_2m5 max = 1 - eps/2;

    BOOST_CHECK_EQUAL(e, 2);
    BOOST_CHECK_EQUAL(s, 1 - ulp);
    BOOST_CHECK_EQUAL(s, 1 - 0.03125);
   // BOOST_CHECK_EQUAL(s, 0.96875);
  }

  { //range 15 and resolution 16 + sign = 32 bit, default round and overflow.
    typedef boost::fixed_point::negatable<15, -16> fixed_point_type_15m16;

    std::cout.precision(std::numeric_limits<fixed_point_type_15m16>::digits10);

    fixed_point_type_15m16 x;
    x = (std::numeric_limits<fixed_point_type_15m16>::max)();
    BOOST_CHECK_EQUAL(x.all_bits, 32);
    fixed_point_type_15m16 m = x; // 
    std::cout << m.bit_pattern() << std::endl;
    BOOST_CHECK_EQUAL(m.bit_pattern(), "01111111111111111111111111111111"); // All except sign bit set.
    BOOST_CHECK_EQUAL(m.bit_pattern().size(), x.range + (-x.resolution) + 1); // 32 bits.
    BOOST_CHECK_EQUAL(x, m);
    fixed_point_type_15m16 eps = std::numeric_limits<fixed_point_type_15m16>::epsilon();
    fixed_point_type_15m16 ulp = eps / 2;

    int e;
    fixed_point_type_15m16 s = frexp(x, &e); 
    std::cout << "frac " << s << ", exp =  " << e << ", eps = " << eps << ", ulp = " << ulp << std::endl;
    fixed_point_type_15m16 max = 1 - eps / 2;
    BOOST_CHECK_EQUAL(e, 15);
    BOOST_CHECK_EQUAL(s, 1 - ulp);
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
{ // Test the types for value_type and float_type for some fixed_point types.
  { // negatable<2, -5> using eight bits.
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;

    //std::cout << typeid(fixed_point_type_2m5::value_type).name() << std::endl;
    BOOST_CHECK_EQUAL(typeid(fixed_point_type_2m5::value_type).name(), "signed char");

    //std::cout << typeid(fixed_point_type_2m5::float_type).name() << std::endl;
    BOOST_CHECK_EQUAL(typeid(fixed_point_type_2m5::float_type).name(), "float");
  }

  { // negatable<0, -31>
    typedef boost::fixed_point::negatable<0, -31> fixed_point_type_0m31;

    //std::cout << typeid(fixed_point_type_0m31::value_type).name() << std::endl;
    BOOST_CHECK_EQUAL(typeid(fixed_point_type_0m31::value_type).name(), "int");

   // std::cout << typeid(fixed_point_type_0m31::float_type).name() << std::endl;
    BOOST_CHECK_EQUAL(typeid(fixed_point_type_0m31::float_type).name(), "double");
  }
// More example here needed.

} // BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_value_float)


BOOST_AUTO_TEST_CASE(fixed_point_numeric_limits_epsilon)
{ // Test epsilon is 2^-(significand bits - 1)
  {
    typedef boost::fixed_point::negatable<0, -31> fixed_point_type_0m31;

   // std::cout << std::numeric_limits<fixed_point_type_0m31>::epsilon() << std::endl;  // 2^-(31-1)
    BOOST_CHECK_EQUAL(std::numeric_limits<fixed_point_type_0m31>::epsilon(), 9.3132257462e-010);
  }

  {
    typedef boost::fixed_point::negatable<0, -7> fixed_point_type_0m7;
    BOOST_CHECK_EQUAL(std::numeric_limits<fixed_point_type_0m7>::epsilon(), 0.015625); // 2^-(7-1) = 2^-6
  }
  {
    typedef boost::fixed_point::negatable<1, -6> fixed_point_type_1m6;
    BOOST_CHECK_EQUAL(std::numeric_limits<fixed_point_type_1m6>::epsilon(), 0.03125); // 2^-(6-1) = 2^-5
  }
  { // Note that if do not use a typedef then
    // extra brackets needed to avoid confusing BOOST_CHECK_EQUAL claiming wrong number of arguments.
    BOOST_CHECK_EQUAL((std::numeric_limits<negatable<1, -6> >::epsilon()), 0.03125); // 2^-(6-1) = 2^-5
  }
  { // Edge cases
    std::cout << std::numeric_limits<negatable<0, -1>>::epsilon() << std::endl;  // 1

    // BOOST_CHECK_EQUAL((std::numeric_limits<negatable<0, -1> >::epsilon()), 1); // 2^-(1-1) = 2^0 = 1 
    // error : in "fixed_point_numeric_limits_epsilon": check (std::numeric_limits<negatable<0, -1> >::epsilon()) == 1 has failed [1 != 1]
    // negatable<0, -1> x(1);
    // std::cout << x << std::endl; // 0 so 1 cannot be represented in this type?
    // BOOST_CHECK_EQUAL((std::numeric_limits<negatable<0, -1> >::epsilon()), x); // 2^-(1-1) = 2^0 = 1 
    // error : in "fixed_point_numeric_limits_epsilon": check (std::numeric_limits<negatable<0, -1> >::epsilon()) == x has failed [1 != 0]

    BOOST_CHECK_EQUAL((std::numeric_limits<negatable<1, -1> >::epsilon()), 1); // 2^-(1-1) = 2^0 = 1
    BOOST_CHECK_EQUAL((std::numeric_limits<negatable<2, -1> >::epsilon()), 1); // 
    BOOST_CHECK_EQUAL((std::numeric_limits<negatable<3, -1> >::epsilon()), 1); // 
    BOOST_CHECK_EQUAL((std::numeric_limits<negatable<4, -1> >::epsilon()), 1); //
    BOOST_CHECK_EQUAL((std::numeric_limits<negatable<5, -1> >::epsilon()), 1); //
    BOOST_CHECK_EQUAL((std::numeric_limits<negatable<6, -1> >::epsilon()), 1); //
  }
  { // Big resolutions that will use multiprecision.
    // Wolfram alpha 2^-2543.454467422037777850154540745120159828446400145774512554 × 10^-77
    negatable<0, -255> e;
    e = 3.454467422037777850154540745120159828446400145774512554e-77;

    BOOST_CHECK_EQUAL((std::numeric_limits<negatable<0, -255> >::epsilon()), e);
    BOOST_CHECK_EQUAL((std::numeric_limits<negatable<0, -255> >::epsilon()), 3.45446742203777785015454074512015982844640014577451255400948138806743672126497e-77); // 2^-(255-1) = 2^254 = 
    
  }
  { // Default constructor clears data member so value is zero.
    negatable<0, -255> x;
    BOOST_CHECK_EQUAL(x, 0);
  
  
  }


}