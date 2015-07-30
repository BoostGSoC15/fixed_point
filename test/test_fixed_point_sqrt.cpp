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
#include <boost/math/constants/constants.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  template<typename FixedPointType,
           typename FloatPointType = typename FixedPointType::float_type>
  void test(const FixedPointType& my_tolerance = ldexp(FixedPointType(1), FixedPointType::resolution + 3))
  {
    // Use at least 6 resolution bits.
    // Use at least 8 range bits.

    BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 6);
    BOOST_STATIC_ASSERT( FixedPointType::range      >= 8);

    using std::sqrt;

    /*
    std::cout << "Testing sqrt for negatable<"
              << std::setw(3)
              << FixedPointType::range
              << ", "
              << std::setw(4)
              << FixedPointType::resolution
              << ">."
              << std::endl;
    */

    const FixedPointType a1(  2    );                                       const FloatPointType b1(  2    );
    const FixedPointType a2(  3    );                                       const FloatPointType b2(  3    );
    const FixedPointType a3(  8.375);                                       const FloatPointType b3(  8.375);
    const FixedPointType a4( 64.125);                                       const FloatPointType b4( 64.125);
    const FixedPointType a5(100.875);                                       const FloatPointType b5(100.875);
    const FixedPointType a6(FixedPointType(  1) /  3);                      const FloatPointType b6(FloatPointType(  1) /  3);
    const FixedPointType a7(FixedPointType( 12) / 10);                      const FloatPointType b7(FloatPointType( 12) / 10);
    const FixedPointType a8(FixedPointType(111) / 10);                      const FloatPointType b8(FloatPointType(111) / 10);
    const FixedPointType a9(boost::math::constants::phi<FixedPointType>()); const FloatPointType b9(boost::math::constants::phi<FloatPointType>());

    BOOST_CHECK_CLOSE_FRACTION(sqrt(a1), FixedPointType(sqrt(b1)), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a2), FixedPointType(sqrt(b2)), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a3), FixedPointType(sqrt(b3)), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a4), FixedPointType(sqrt(b4)), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a5), FixedPointType(sqrt(b5)), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a6), FixedPointType(sqrt(b6)), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a7), FixedPointType(sqrt(b7)), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a8), FixedPointType(sqrt(b8)), my_tolerance);
    BOOST_CHECK_CLOSE_FRACTION(sqrt(a9), FixedPointType(sqrt(b9)), my_tolerance);
  }
}

BOOST_AUTO_TEST_CASE(fixed_point_sqrt)
{
  { typedef boost::fixed_point::negatable<  8,   -7, boost::fixed_point::round::fastest> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable<  8,  -15, boost::fixed_point::round::fastest> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable<  8,  -44, boost::fixed_point::round::fastest> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable<  8,  -55, boost::fixed_point::round::fastest> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable< 20,  -10, boost::fixed_point::round::fastest> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable<100,  -10, boost::fixed_point::round::fastest> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable< 20, -100, boost::fixed_point::round::fastest> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable<100, -100, boost::fixed_point::round::fastest> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable< 20, -491, boost::fixed_point::round::fastest> fixed_point_type; local::test<fixed_point_type>(ldexp(fixed_point_type(1), fixed_point_type::resolution + 16)); }

  { typedef boost::fixed_point::negatable<  8,   -7, boost::fixed_point::round::nearest_even> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable<  8,  -15, boost::fixed_point::round::nearest_even> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable<  8,  -44, boost::fixed_point::round::nearest_even> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable<  8,  -55, boost::fixed_point::round::nearest_even> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable< 20,  -10, boost::fixed_point::round::nearest_even> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable<100,  -10, boost::fixed_point::round::nearest_even> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable< 20, -100, boost::fixed_point::round::nearest_even> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable<100, -100, boost::fixed_point::round::nearest_even> fixed_point_type; local::test<fixed_point_type>(); }
  { typedef boost::fixed_point::negatable< 20, -491, boost::fixed_point::round::nearest_even> fixed_point_type; local::test<fixed_point_type>(ldexp(fixed_point_type(1), fixed_point_type::resolution + 16)); }
}
