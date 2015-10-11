///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for sqrt(fixed_point) with round::nearest_even.

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_func_sqrt_small_nearest_even
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
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
  void test_sqrt(const int fuzzy_bits)
  {
    // Use at least 6 resolution bits.
    // Use at least 8 range bits.

    BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 6);
    BOOST_STATIC_ASSERT( FixedPointType::range      >= 7);

    using std::sqrt;

    const FixedPointType a1 (  2L    );                                      const FloatPointType b1(  2L    );
    const FixedPointType a2 (  3L    );                                      const FloatPointType b2(  3L    );
    const FixedPointType a3 (  8.375L);                                      const FloatPointType b3(  8.375L);
    const FixedPointType a4 ( 64.125L);                                      const FloatPointType b4( 64.125L);
    const FixedPointType a5 (100.875L);                                      const FloatPointType b5(100.875L);
    const FixedPointType a6 (FixedPointType(  1) / 10);                      const FloatPointType b6(FloatPointType(  1) / 10);
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
}

BOOST_AUTO_TEST_CASE(test_negatable_func_sqrt_small_nearest_even)
{
  // Test sqrt() round::fastest for negatable in various key small digit ranges

  { typedef boost::fixed_point::negatable<7,  -8, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<7, -16, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }
  { typedef boost::fixed_point::negatable<7, -24, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_sqrt<fixed_point_type>(4); }

  { typedef boost::fixed_point::negatable<4, -11, boost::fixed_point::round::nearest_even> fixed_point_type; BOOST_CHECK_EQUAL(sqrt(fixed_point_type(1)), fixed_point_type(1)); }
  { typedef boost::fixed_point::negatable<7, -24, boost::fixed_point::round::nearest_even> fixed_point_type; BOOST_CHECK_EQUAL(sqrt(fixed_point_type(1)), fixed_point_type(1)); }
}
