///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//! \file
//!\brief Tests for cbrt(fixed_point). Along the way, also test numerous fixed_point arithmetic operations.

#include <cmath>
#include <iomanip>
#include <iostream>

#define BOOST_TEST_MODULE fixed_point_cbrt
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/special_functions/cbrt.hpp>
#include <boost/test/included/unit_test.hpp>

//#define ENABLE_LOCAL_TEST_DEBUG_MESSAGES

namespace local
{
  template<typename FixedPointType>
  const FixedPointType& tolerance_maker(const int fuzzy_bits)
  {
    static const FixedPointType the_tolerance = ldexp(FixedPointType(1), FixedPointType::resolution + fuzzy_bits);

    return the_tolerance;
  }

  template<typename FixedPointType,
           typename FloatPointType = typename FixedPointType::float_type>
  void test_cbrt(const int fuzzy_bits)
  {
    // Use at least 6 resolution bits.
    // Use at least 8 range bits.

    BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 6);
    BOOST_STATIC_ASSERT( FixedPointType::range      >= 8);

    using boost::math::cbrt;

    #if defined(ENABLE_LOCAL_TEST_DEBUG_MESSAGES)

      std::cout << "Testing cbrt for negatable<"
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

    const FixedPointType my_tolerance = tolerance_maker<FixedPointType>(fuzzy_bits);

    BOOST_CHECK_CLOSE_FRACTION(cbrt(a1), cbrt(b1), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cbrt(a2), cbrt(b2), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cbrt(a3), cbrt(b3), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cbrt(a4), cbrt(b4), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cbrt(a5), cbrt(b5), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cbrt(a6), cbrt(b6), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cbrt(a7), cbrt(b7), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cbrt(a8), cbrt(b8), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(cbrt(a9), cbrt(b9), my_tolerance);
  }
}

BOOST_AUTO_TEST_CASE(fixed_point_cbrt)
{
  // Test cbrt() for negatable in various key digit regions
  // such as 16, 24, 32, 53, 64, 113, 128, etc.

  { typedef boost::fixed_point::negatable<   8,    -7, boost::fixed_point::round::fastest>      fixed_point_type; local::test_cbrt<fixed_point_type>(6); }
  { typedef boost::fixed_point::negatable<   8,    -7, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(6); }
  { typedef boost::fixed_point::negatable<  11,   -12, boost::fixed_point::round::fastest>      fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<  11,   -12, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<  15,   -16, boost::fixed_point::round::fastest>      fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<  15,   -16, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<  26,   -26, boost::fixed_point::round::fastest>      fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<  26,   -26, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<  31,   -32, boost::fixed_point::round::fastest>      fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<  31,   -32, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<  56,   -56, boost::fixed_point::round::fastest>      fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<  56,   -56, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<  63,   -64, boost::fixed_point::round::fastest>      fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<  63,   -64, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable< 127,  -128, boost::fixed_point::round::fastest>      fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable< 127,  -128, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable< 255,  -256, boost::fixed_point::round::fastest>      fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable< 255,  -256, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<2047, -2048, boost::fixed_point::round::fastest>      fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
  { typedef boost::fixed_point::negatable<2047, -2048, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 1) * 3) / 2); }
}
