///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for log(fixed_point) round::nearest_even.

#define BOOST_TEST_MODULE test_negatable_func_log_nearest_even
#define BOOST_LIB_DIAGNOSTIC

#include <cmath>

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
  void test_log(const int fuzzy_bits)
  {
    // Use at least 10 resolution bits.
    // Use at least  5 range bits.

    BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 10);
    BOOST_STATIC_ASSERT( FixedPointType::range      >=  5);

    const FixedPointType a1(+2L    );                                      const FloatPointType b1(+2L    );
    const FixedPointType a2(+3L    );                                      const FloatPointType b2(+3L    );
    const FixedPointType a3(+4.375L);                                      const FloatPointType b3(+4.375L);
    const FixedPointType a4(+1.125L);                                      const FloatPointType b4(+1.125L);
    const FixedPointType a5(+0.125L);                                      const FloatPointType b5(+0.125L);
    const FixedPointType a6(+0.875L);                                      const FloatPointType b6(+0.875L);
    const FixedPointType a7(FixedPointType( 1) /  3);                      const FloatPointType b7(FloatPointType( 1) /  3);
    const FixedPointType a8(FixedPointType(21) / 10);                      const FloatPointType b8(FloatPointType(21) / 10);
    const FixedPointType a9(boost::math::constants::pi<FixedPointType>()); const FloatPointType b9(boost::math::constants::pi<FloatPointType>());

    using std::log;

    BOOST_CHECK_CLOSE_FRACTION(log(a1), FixedPointType(log(b1)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(log(a2), FixedPointType(log(b2)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(log(a3), FixedPointType(log(b3)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(log(a4), FixedPointType(log(b4)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(log(a5), FixedPointType(log(b5)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(log(a6), FixedPointType(log(b6)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(log(a7), FixedPointType(log(b7)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(log(a8), FixedPointType(log(b8)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(log(a9), FixedPointType(log(b9)), tolerance_maker<FixedPointType>(fuzzy_bits));
  }
}

BOOST_AUTO_TEST_CASE(test_negatable_func_log_nearest_even)
{
  // Test log() for negatable round::nearest_even in various key digit regions.

  { typedef boost::fixed_point::negatable< 5,  -10, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_log<fixed_point_type>( 6); }
  { typedef boost::fixed_point::negatable< 7,  -24, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_log<fixed_point_type>( 8); }
  { typedef boost::fixed_point::negatable<10, -117, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_log<fixed_point_type>(14); }
}
