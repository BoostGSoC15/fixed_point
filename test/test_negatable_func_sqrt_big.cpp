///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for sqrt(fixed_point) for a big digit range. Along the way, also test numerous fixed_point arithmetic operations.

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_func_sqrt_big
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
  void test_big_sqrt_pi(const int fuzzy_bits)
  {
    const FixedPointType a(boost::math::constants::pi<FixedPointType>());
    const FloatPointType b(boost::math::constants::pi<FloatPointType>());

    BOOST_CHECK_CLOSE_FRACTION(sqrt(a), FixedPointType(sqrt(b)), tolerance_maker<FixedPointType>(fuzzy_bits));
  }
}

BOOST_AUTO_TEST_CASE(test_negatable_func_sqrt_big)
{
  // Test sqrt() for negatable in a large digit range using the result
  // of a non-trivial calculation (in this case pi). This also
  // exercises negatable with numerous arithmetic operations.

  { typedef boost::fixed_point::negatable<10, -4085, boost::fixed_point::round::fastest>      fixed_point_type; local::test_big_sqrt_pi<fixed_point_type>(fixed_point_type::range); }
  { typedef boost::fixed_point::negatable<10, -4085, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_big_sqrt_pi<fixed_point_type>(fixed_point_type::range); }
}
