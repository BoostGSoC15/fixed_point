///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Test sqrt() for negatable in a tiny digit region.

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_func_sqrt_tiny
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
    // Use at least 8 resolution bits.
    // Use at least 4 range bits.

    BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 8);
    BOOST_STATIC_ASSERT( FixedPointType::range      >= 4);

    using std::sqrt;

    const FixedPointType a1 (FixedPointType(1) /  2);                       const FloatPointType b1(FloatPointType(1) /  2);
    const FixedPointType a2 (FixedPointType(6) / 10);                       const FloatPointType b2(FloatPointType(6) / 10);
    const FixedPointType a3 (FixedPointType(3) /  2);                       const FloatPointType b3(FloatPointType(3) /  2);
    const FixedPointType a4 (2L);                                           const FloatPointType b4(2L);
    const FixedPointType a5 (FixedPointType(5) /  2);                       const FloatPointType b5(FloatPointType(5) /  2);
    const FixedPointType a6 (3L);                                           const FloatPointType b6(3L);
    const FixedPointType a7 (FixedPointType(1) /  3);                       const FloatPointType b7(FloatPointType(1) /  3);
    const FixedPointType a8 (boost::math::constants::pi<FixedPointType>()); const FloatPointType b8(boost::math::constants::pi<FloatPointType>());
    const FixedPointType a9 (11L);                                          const FloatPointType b9(11L);

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

BOOST_AUTO_TEST_CASE(test_negatable_func_sqrt_tiny)
{
  { typedef boost::fixed_point::negatable<4, -11> fixed_point_type; local::test_sqrt<fixed_point_type>(1); }
}
