///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for sqrt(fixed_point) with round::fastest. Along the way, also test numerous fixed_point arithmetic operations.

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_func_sqrt_small
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
    // Use at least 4 resolution bits.
    // Use at least 3 range bits.

    BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 4);
    BOOST_STATIC_ASSERT( FixedPointType::range      >= 3);

    using std::sqrt;

    const FixedPointType a1 (2L);                                           const FloatPointType b1(2L);
    const FixedPointType a2 (3L);                                           const FloatPointType b2(3L);
    const FixedPointType a3 (FixedPointType(1) / 3);                        const FloatPointType b3(FloatPointType(1) / 3);
    const FixedPointType a4 (boost::math::constants::pi<FixedPointType>()); const FloatPointType b4(boost::math::constants::pi<FloatPointType>());

    BOOST_CHECK_CLOSE_FRACTION(sqrt(a1), FixedPointType(sqrt(b1)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a2), FixedPointType(sqrt(b2)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a3), FixedPointType(sqrt(b3)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a4), FixedPointType(sqrt(b4)), tolerance_maker<FixedPointType>(fuzzy_bits));
  }
}

BOOST_AUTO_TEST_CASE(test_negatable_func_sqrt_small)
{
  // Test sqrt() round::fastest for negatable in a small digit region.

  { typedef boost::fixed_point::negatable<3, -4> fixed_point_type; local::test_sqrt<fixed_point_type>(1); }
}
