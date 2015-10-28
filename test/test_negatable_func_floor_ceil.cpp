///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests floor and ceil for fixed_point negatable.

#define BOOST_TEST_MODULE test_negatable_func_floor_ceil
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  template<typename FixedPointType,
           typename FloatPointType = typename FixedPointType::float_type>
  void test_floor_ceil()
  {
    using std::ceil;
    using std::floor;

    const FixedPointType a1(FixedPointType( 21) / 10); const FloatPointType b1(FloatPointType( 21) / 10);
    const FixedPointType a2(FixedPointType(-21) / 10); const FloatPointType b2(FloatPointType(-21) / 10);
    const FixedPointType a3(FixedPointType( 17) /  2); const FloatPointType b3(FloatPointType( 17) /  2);
    const FixedPointType a4(FixedPointType(-14) /  3); const FloatPointType b4(FloatPointType(-14) /  3);
    const FixedPointType a5(FixedPointType( -7) / 10); const FloatPointType b5(FloatPointType( -7) / 10);
    const FixedPointType a6(FixedPointType(  7) / 10); const FloatPointType b6(FloatPointType(  7) / 10);
    const FixedPointType a7(FixedPointType(  1.23L));  const FloatPointType b7(FloatPointType(  1.23L));
    const FixedPointType a8(FixedPointType(  3.14L));  const FloatPointType b8(FloatPointType(  3.14L));
    const FixedPointType a9(FixedPointType( -3.14L));  const FloatPointType b9(FloatPointType( -3.14L));

    BOOST_CHECK_EQUAL(floor(a1), FixedPointType(floor(b1)));
    BOOST_CHECK_EQUAL(floor(a2), FixedPointType(floor(b2)));
    BOOST_CHECK_EQUAL(floor(a3), FixedPointType(floor(b3)));
    BOOST_CHECK_EQUAL(floor(a4), FixedPointType(floor(b4)));
    BOOST_CHECK_EQUAL(floor(a5), FixedPointType(floor(b5)));
    BOOST_CHECK_EQUAL(floor(a6), FixedPointType(floor(b6)));
    BOOST_CHECK_EQUAL(floor(a7), FixedPointType(floor(b7)));
    BOOST_CHECK_EQUAL(floor(a8), FixedPointType(floor(b8)));
    BOOST_CHECK_EQUAL(floor(a9), FixedPointType(floor(b9)));

    BOOST_CHECK_EQUAL(ceil(a1), FixedPointType(ceil(b1)));
    BOOST_CHECK_EQUAL(ceil(a2), FixedPointType(ceil(b2)));
    BOOST_CHECK_EQUAL(ceil(a3), FixedPointType(ceil(b3)));
    BOOST_CHECK_EQUAL(ceil(a4), FixedPointType(ceil(b4)));
    BOOST_CHECK_EQUAL(ceil(a5), FixedPointType(ceil(b5)));
    BOOST_CHECK_EQUAL(ceil(a6), FixedPointType(ceil(b6)));
    BOOST_CHECK_EQUAL(ceil(a7), FixedPointType(ceil(b7)));
    BOOST_CHECK_EQUAL(ceil(a8), FixedPointType(ceil(b8)));
    BOOST_CHECK_EQUAL(ceil(a9), FixedPointType(ceil(b9)));
  }
}

BOOST_AUTO_TEST_CASE(test_negatable_func_floor_ceil)
{
  { typedef boost::fixed_point::negatable<6,  -17> fixed_point_type; static_cast<void>(local::test_floor_ceil<fixed_point_type>()); }
  { typedef boost::fixed_point::negatable<6,  -25> fixed_point_type; static_cast<void>(local::test_floor_ceil<fixed_point_type>()); }
  { typedef boost::fixed_point::negatable<6,  -46> fixed_point_type; static_cast<void>(local::test_floor_ceil<fixed_point_type>()); }
  { typedef boost::fixed_point::negatable<6,  -57> fixed_point_type; static_cast<void>(local::test_floor_ceil<fixed_point_type>()); }
  { typedef boost::fixed_point::negatable<6, -121> fixed_point_type; static_cast<void>(local::test_floor_ceil<fixed_point_type>()); }
  { typedef boost::fixed_point::negatable<6, -249> fixed_point_type; static_cast<void>(local::test_floor_ceil<fixed_point_type>()); }
}
