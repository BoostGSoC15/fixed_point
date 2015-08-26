///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for cbrt(fixed_point) round::nearest_even. Along the way, also test numerous fixed_point arithmetic operations.

#define BOOST_TEST_MODULE test_negatable_cbrt_nearest_even
#define BOOST_LIB_DIAGNOSTIC

#include <cmath>
#include <iomanip>
#include <iostream>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/special_functions/cbrt.hpp>
#include <boost/test/included/unit_test.hpp>

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

    // Use boost::math::cbrt for fixed-point negatable because cbrt(negatable) is not yet implemented.
    // Do not use a using directive for boost::math::cbrt() until cbrt is implemented for fixed-point negatable.
    BOOST_CHECK_CLOSE_FRACTION(boost::math::cbrt(a1), FixedPointType(boost::math::cbrt(b1)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(boost::math::cbrt(a2), FixedPointType(boost::math::cbrt(b2)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(boost::math::cbrt(a3), FixedPointType(boost::math::cbrt(b3)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(boost::math::cbrt(a4), FixedPointType(boost::math::cbrt(b4)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(boost::math::cbrt(a5), FixedPointType(boost::math::cbrt(b5)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(boost::math::cbrt(a6), FixedPointType(boost::math::cbrt(b6)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(boost::math::cbrt(a7), FixedPointType(boost::math::cbrt(b7)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(boost::math::cbrt(a8), FixedPointType(boost::math::cbrt(b8)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(boost::math::cbrt(a9), FixedPointType(boost::math::cbrt(b9)), tolerance_maker<FixedPointType>(fuzzy_bits));
  }
}

BOOST_AUTO_TEST_CASE(test_negatable_cbrt)
{
  // Test cbrt() for negatable round::fastest in various key digit
  // regions such as 16, 24, 32, 53, 64, 113.

  { typedef boost::fixed_point::negatable< 8,  -7, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<11, -12, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 9) * 2) / 10); }
  { typedef boost::fixed_point::negatable<15, -16, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 9) * 2) / 10); }
  { typedef boost::fixed_point::negatable<26, -26, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 9) * 1) / 10); }
  { typedef boost::fixed_point::negatable<31, -32, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 9) * 1) / 10); }
  { typedef boost::fixed_point::negatable<56, -56, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_cbrt<fixed_point_type>(((-fixed_point_type::resolution + 9) * 1) / 10); }
}
