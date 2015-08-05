///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//! \file
//!\brief Tests for sqrt(fixed_point). Along the way, also test numerous fixed_point arithmetic operations.

#include <cmath>
#include <iomanip>
#include <iostream>

#define BOOST_TEST_MODULE fixed_point_sqrt
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/special_functions/cbrt.hpp>
#include <boost/test/included/unit_test.hpp>

//#define ENABLE_LOCAL_TEST_DEBUG_MESSAGES

namespace local
{
  // Patch BOOST_CHECK_CLOSE_FRACTION because of disagreements
  // among compilers, the implementation of fixed_point and Boost.Test.

  template<typename T>
  void boost_check_close_fraction(const T& left, const T& right, const T& tolerance)
  {
    using std::fabs;

    const T fraction = fabs(left / right);

    const bool the_comparison_result = (fabs(T(1) - fraction) <= tolerance);

    BOOST_CHECK_EQUAL(the_comparison_result, true);
  }

  #undef  BOOST_CHECK_CLOSE_FRACTION
  #define BOOST_CHECK_CLOSE_FRACTION(L, R, T) local::boost_check_close_fraction(L, R, T);

  template<typename FixedPointType>
  const FixedPointType& tolerance_maker(const int fuzzy_bits)
  {
    static const FixedPointType the_tolerance = ldexp(FixedPointType(1), FixedPointType::resolution + fuzzy_bits);

    return the_tolerance;
  }

  template<typename FixedPointType,
           typename FloatPointType = typename FixedPointType::float_type>
  void test_sqrt(const int fuzzy_bits)
  {
    // Use at least 6 resolution bits.
    // Use at least 8 range bits.

    BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 6);
    BOOST_STATIC_ASSERT( FixedPointType::range      >= 8);

    using std::sqrt;

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)

      std::cout << "Testing sqrt for negatable<"
                << std::setw(4)
                << FixedPointType::range
                << ", "
                << std::setw(5)
                << FixedPointType::resolution
                << ">."
                << std::endl;

    #endif // ENABLE_LOCAL_TEST_DEBUG_MESSAGES

    const FixedPointType a1 (  2L    );                                      const FloatPointType b1(  2L    );
    const FixedPointType a2 (  3L    );                                      const FloatPointType b2(  3L    );
    const FixedPointType a3 (  8.375L);                                      const FloatPointType b3(  8.375L);
    const FixedPointType a4 ( 64.125L);                                      const FloatPointType b4( 64.125L);
    const FixedPointType a5 (100.875L);                                      const FloatPointType b5(100.875L);
    const FixedPointType a6 (FixedPointType(  1) /  3);                      const FloatPointType b6(FloatPointType(  1) /  3);
    const FixedPointType a7 (FixedPointType( 12) / 10);                      const FloatPointType b7(FloatPointType( 12) / 10);
    const FixedPointType a8 (FixedPointType(111) / 10);                      const FloatPointType b8(FloatPointType(111) / 10);
    const FixedPointType a9 (boost::math::constants::phi<FixedPointType>()); const FloatPointType b9(boost::math::constants::phi<FloatPointType>());

    BOOST_CHECK_CLOSE_FRACTION(sqrt(a1), FixedPointType(sqrt(b1)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a2), FixedPointType(sqrt(b2)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a3), FixedPointType(sqrt(b3)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a4), FixedPointType(sqrt(b4)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a5), FixedPointType(sqrt(b5)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a6), FixedPointType(sqrt(b6)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a7), FixedPointType(sqrt(b7)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a8), FixedPointType(sqrt(b8)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a9), FixedPointType(sqrt(b9)), tolerance_maker<FixedPointType>(fuzzy_bits));
  }

  template<typename FixedPointType,
           typename FloatPointType = typename FixedPointType::float_type>
  void test_big_sqrt_zeta_three(const int fuzzy_bits)
  {
    // Use at least 6000 resolution bits.
    // Use at least 2000 range bits.

    BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 6000);
    BOOST_STATIC_ASSERT( FixedPointType::range      >= 2000);

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)

      std::cout << "Testing sqrt for negatable<"
                << std::setw(4)
                << FixedPointType::range
                << ", "
                << std::setw(5)
                << FixedPointType::resolution
                << ">."
                << std::endl;

    #endif // ENABLE_LOCAL_TEST_DEBUG_MESSAGES

    const FixedPointType a(boost::math::constants::zeta_three<FixedPointType>());
    const FloatPointType b(boost::math::constants::zeta_three<FloatPointType>());

    BOOST_CHECK_CLOSE_FRACTION(sqrt(a), FixedPointType(sqrt(b)), tolerance_maker<FixedPointType>(fuzzy_bits));
  }
}

BOOST_AUTO_TEST_CASE(fixed_point_sqrt)
{
  // Test sqrt() for negatable in various key digit regions
  // such as 16, 24, 32, 53, 64, 113, 128, and also in randomly
  // selected higher digit regions.

  { typedef boost::fixed_point::negatable<   8,    -7, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,    -7, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,   -15, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,   -15, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,   -23, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,   -23, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,   -44, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,   -44, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,   -55, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,   -55, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,  -104, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,  -104, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,  -119, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<   8,  -119, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<  20,   -10, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<  20,   -10, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable< 100,   -10, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable< 100,   -10, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<  20,  -100, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<  20,  -100, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable< 100,  -100, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable< 100,  -100, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable< 119,  -120, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(8); }
  { typedef boost::fixed_point::negatable< 119,  -120, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(8); }
  { typedef boost::fixed_point::negatable<  20,  -491, boost::fixed_point::round::fastest>      fixed_point_type; local::test_sqrt<fixed_point_type>(8); }
  { typedef boost::fixed_point::negatable<  20,  -491, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(8); }

  // Test sqrt() for negatable in a large digit range using the result
  // of a non-trivial calculation (in this case zeta_three). This also
  // exercises negatable with numerous arithmetic operations.
  { typedef boost::fixed_point::negatable<2000, -6191, boost::fixed_point::round::fastest>      fixed_point_type; local::test_big_sqrt_zeta_three<fixed_point_type>(fixed_point_type::range); }
  { typedef boost::fixed_point::negatable<2000, -6191, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_big_sqrt_zeta_three<fixed_point_type>(fixed_point_type::range); }
}
