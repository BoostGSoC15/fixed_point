//  Copyright Paul A. Bristow 2015.
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)


//! \file
//!\brief Test numeric_limits for various negatable fixed_point types.

#define BOOST_TEST_MODULE test_negatable_basic_limits
#define BOOST_LIB_DIAGNOSTIC

#include <limits>
#include <iostream>
#include <type_traits>

#include <boost/test/included/unit_test.hpp>
#include <boost/fixed_point/fixed_point.hpp>

//#define ENABLE_LOCAL_TEST_DEBUG_MESSAGES

using namespace boost::fixed_point;

BOOST_AUTO_TEST_CASE(test_negatable_basic_limits_min)
{
  #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
  std::cout.setf(std::ios_base::boolalpha | std::ios_base::showpoint); // Show any trailing zeros.
  #endif

  {//! small range 2 and resolution 5 + sign = 8 bit, default round and overflow.

    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;
    fixed_point_type_2m5 x = (std::numeric_limits<fixed_point_type_2m5>::min)();
    BOOST_CHECK_EQUAL(x.all_bits, 8);
    BOOST_CHECK_EQUAL(x.range, 2);
    BOOST_CHECK_EQUAL(x.resolution, -5);
    fixed_point_type_2m5 m = x; // = 0.03125;

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << m.bit_pattern() << std::endl;
    #endif

    BOOST_CHECK_EQUAL(m.bit_pattern(), "00000001");
    BOOST_CHECK_EQUAL(m.bit_pattern().size(), std::size_t(x.range + (-x.resolution) + 1)); // 8 bits.
    BOOST_CHECK_EQUAL(x, m);
    int e;
    fixed_point_type_2m5 s = frexp(x, &e);

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << s << " " << e << std::endl;
    #endif

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

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << m.bit_pattern() << std::endl;
    #endif

    BOOST_CHECK_EQUAL(m.bit_pattern(), "00000000000000000000000000000001");
    BOOST_CHECK_EQUAL(m.bit_pattern().size(), std::size_t(x.range + (-x.resolution) + 1)); // 32 bits.
    BOOST_CHECK_EQUAL(x, m);
    int e;
    fixed_point_type_15m16 s = frexp(x, &e);

    BOOST_CHECK_EQUAL(e, -16 + 1);
    BOOST_CHECK_EQUAL(s, 0.5);
  }
} // BOOST_AUTO_TEST_CASE(test_negatable_numeric_limits_min)


BOOST_AUTO_TEST_CASE(test_negatable_basic_limits_max)
{
  #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
  std::cout.setf(std::ios_base::boolalpha | std::ios_base::showpoint); // Show any trailing zeros.
  #endif

  { //! small range 2 and resolution 5 + sign = 8 bit, default round and overflow.
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout.precision(std::numeric_limits<fixed_point_type_2m5>::digits10);
    #endif

    fixed_point_type_2m5 x = (std::numeric_limits<fixed_point_type_2m5>::max)();
    BOOST_CHECK_EQUAL(x.all_bits, 8); // 8 bits used.
    fixed_point_type_2m5 m = 3.969; // 

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << m.bit_pattern() << std::endl;
    #endif

    BOOST_CHECK_EQUAL(m.bit_pattern(), "01111111"); // All except sign bit set.
    BOOST_CHECK_EQUAL(m.bit_pattern().size(), std::size_t(x.range + (-x.resolution) + 1)); // 8 bits.
    BOOST_CHECK_EQUAL(x, m);
    int e;
    fixed_point_type_2m5 s = frexp(x, &e);
    fixed_point_type_2m5 eps = std::numeric_limits<fixed_point_type_2m5>::epsilon();
    fixed_point_type_2m5 ulp = eps/2;
    BOOST_CHECK_EQUAL(ulp, 0.03125);

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << "frac " << s << ", exp =  " << e <<", eps = " << eps << ", ulp = " << ulp << std::endl;
    #endif

    fixed_point_type_2m5 max = 1 - eps/2;

    BOOST_CHECK_EQUAL(e, 2);
    BOOST_CHECK_EQUAL(s, 1 - ulp);
    BOOST_CHECK_EQUAL(s, 1 - 0.03125);
    //BOOST_CHECK_EQUAL(s, 0.96875);
  }

  { //range 15 and resolution 16 + sign = 32 bit, default round and overflow.
    typedef boost::fixed_point::negatable<15, -16> fixed_point_type_15m16;

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout.precision(std::numeric_limits<fixed_point_type_15m16>::digits10);
    #endif

    fixed_point_type_15m16 x;
    x = (std::numeric_limits<fixed_point_type_15m16>::max)();
    BOOST_CHECK_EQUAL(x.all_bits, 32);
    fixed_point_type_15m16 m = x; // 

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << m.bit_pattern() << std::endl;
    #endif

    BOOST_CHECK_EQUAL(m.bit_pattern(), "01111111111111111111111111111111"); // All except sign bit set.
    BOOST_CHECK_EQUAL(m.bit_pattern().size(), std::size_t(x.range + (-x.resolution) + 1)); // 32 bits.
    BOOST_CHECK_EQUAL(x, m);
    fixed_point_type_15m16 eps = std::numeric_limits<fixed_point_type_15m16>::epsilon();
    fixed_point_type_15m16 ulp = eps / 2;

    int e;
    fixed_point_type_15m16 s = frexp(x, &e); 

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << "frac " << s << ", exp =  " << e << ", eps = " << eps << ", ulp = " << ulp << std::endl;
    #endif

    fixed_point_type_15m16 max = 1 - eps / 2;
    BOOST_CHECK_EQUAL(e, 15);
    BOOST_CHECK_EQUAL(s, 1 - ulp);
  }
} // BOOST_AUTO_TEST_CASE(test_negatable_numeric_limits_max)


BOOST_AUTO_TEST_CASE(test_negatable_basic_limits_lowest)
{ //! small range 2 and resolution 5 + sign = 8 bit, default round and overflow.
  {
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;

    fixed_point_type_2m5 x = (std::numeric_limits<fixed_point_type_2m5>::lowest)();
    fixed_point_type_2m5 m = (std::numeric_limits<fixed_point_type_2m5>::max)();

    BOOST_CHECK_EQUAL(x, -m);

    //BOOST_CHECK_EQUAL(x.all_bits, 8);
    //fixed_point_type_2m5 m = 3.969; // 

    //#if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    //std::cout << m.bit_pattern() << std::endl;
    //#endif

    //BOOST_CHECK_EQUAL(m.bit_pattern(), "01111111"); // All except sign bit set.
    //BOOST_CHECK_EQUAL(m.bit_pattern().size(), std::size_t(x.range + (-x.resolution) + 1)); // 8 bits.
    //BOOST_CHECK_EQUAL(x, m);
    //int e;
    //fixed_point_type_2m5 s = frexp(x, &e);

    //#if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    //std::cout << s << " " << e << std::endl;
    //#endif

    //BOOST_CHECK_EQUAL(e, 2);
    //BOOST_CHECK_EQUAL(s, 1 - 0.03125);
    //BOOST_CHECK_EQUAL(s, 0.96875);
  }
} // BOOST_AUTO_TEST_CASE(test_negatable_numeric_limits_lowest)


BOOST_AUTO_TEST_CASE(test_negatable_basic_limits_misc)
{
  {
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;

    fixed_point_type_2m5 y(1);
    int e;
    fixed_point_type_2m5 s = frexp(y, &e);

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << s << " " << e << std::endl; // 0.5 1
    #endif
  }
  {
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;
    fixed_point_type_2m5 y;
    BOOST_CHECK_EQUAL(y, 0);

    y = ldexp(2, 2);

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << y << std::endl;
    #endif
  }
} // BOOST_AUTO_TEST_CASE(test_negatable_basic_limits_misc)

BOOST_AUTO_TEST_CASE(test_negatable_basic_limits_value_float)
{ // Test the types for value_type and float_type for some fixed_point types.
  { // negatable<2, -5> using eight bits.
    typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5;

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << typeid(fixed_point_type_2m5::value_type).name() << std::endl;
    #endif

    bool type_traits_result;

    type_traits_result = std::is_same<fixed_point_type_2m5::value_type, signed char>::value;

    BOOST_CHECK_EQUAL(type_traits_result, true);

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << typeid(fixed_point_type_2m5::float_type).name() << std::endl;
    #endif

    type_traits_result = std::is_same<fixed_point_type_2m5::float_type, float>::value;

    BOOST_CHECK_EQUAL(type_traits_result, true);
  }

  { // negatable<0, -31>
    typedef boost::fixed_point::negatable<0, -31> fixed_point_type_0m31;

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << typeid(fixed_point_type_0m31::value_type).name() << std::endl;
    #endif

    bool type_traits_result;

    type_traits_result = std::is_same<fixed_point_type_0m31::value_type, int>::value;

    BOOST_CHECK_EQUAL(type_traits_result, true);

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << typeid(fixed_point_type_0m31::float_type).name() << std::endl;
    #endif

    type_traits_result = std::is_same<fixed_point_type_0m31::float_type, double>::value;

    BOOST_CHECK_EQUAL(type_traits_result, true);
  }
  // More example here needed.

} // BOOST_AUTO_TEST_CASE(test_negatable_numeric_limits_value_float)

BOOST_AUTO_TEST_CASE(test_negatable_basic_limits_epsilon)
{ // Test epsilon is 2^-(significand bits - 1)
  {
    typedef boost::fixed_point::negatable<0, -31> fixed_point_type_0m31;

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << std::numeric_limits<fixed_point_type_0m31>::epsilon() << std::endl;  // 2^-(31-1)
    #endif
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
    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    std::cout << std::numeric_limits<negatable<0, -1>>::epsilon() << std::endl;  // 1
    #endif

    // BOOST_CHECK_EQUAL((std::numeric_limits<negatable<0, -1> >::epsilon()), 1); // 2^-(1-1) = 2^0 = 1 
    // error : in "fixed_point_numeric_limits_epsilon": check (std::numeric_limits<negatable<0, -1> >::epsilon()) == 1 has failed [1 != 1]
    // negatable<0, -1> x(1);

    //#if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)
    //std::cout << x << std::endl; // 0 so 1 cannot be represented in this type?
    //#endif

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
    // Wolfram alpha 2^-254 = 3.454467422037777850154540745120159828446400145774512554 × 10^-77
    typedef negatable<0, -255>                 large_fixed_point_type;
    typedef large_fixed_point_type::float_type large_float_point_type;

    const large_fixed_point_type e(large_float_point_type("3.45446742203777785015454074512015982844640014577451255400948138806743672126497e-77"));

    BOOST_CHECK_EQUAL((std::numeric_limits<large_fixed_point_type>::epsilon()), e);
    BOOST_CHECK_EQUAL((std::numeric_limits<large_fixed_point_type>::epsilon()), large_float_point_type("3.45446742203777785015454074512015982844640014577451255400948138806743672126497e-77"));
    // 2^-(255-1) = 2^254 = 
  }

  { // Default constructor clears data member so value is zero.
    negatable<0, -255> x;
    BOOST_CHECK_EQUAL(x, 0);
  }
} // BOOST_AUTO_TEST_CASE(test_negatable_numeric_limits_epsilon)
