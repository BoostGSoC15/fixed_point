///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//! \file
//!\brief Perform tests of frexp and ldexp for fixed_point.

#include <cmath>
#include <iomanip>
#include <iostream>

#define BOOST_TEST_MODULE fixed_point_frexp_ldexp
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/test/included/unit_test.hpp>

#define ENABLE_LOCAL_PATCH_OF_BOOST_TEST

namespace local
{
  #if defined(ENABLE_LOCAL_PATCH_OF_BOOST_TEST)

    // Patch BOOST_CHECK_CLOSE_FRACTION because of disagreements
    // regarding syntax among various compilers, the implementation
    // of fixed_point, and of Boost.Test.

    template<typename T>
    void boost_check_close_fraction(const T& left, const T& right, const T& tolerance)
    {
      using std::fabs;

      const T fraction = fabs(left / right);
      const T delta    = fabs(1 - fraction);

      const bool the_comparison_result = (delta <= tolerance);

      BOOST_CHECK_EQUAL(the_comparison_result, true);
    }

    #undef  BOOST_CHECK_CLOSE_FRACTION
    #define BOOST_CHECK_CLOSE_FRACTION(LEFT, RIGHT, TOLERANCE) local::boost_check_close_fraction((LEFT), (RIGHT), (TOLERANCE));

  #endif // ENABLE_LOCAL_PATCH_OF_BOOST_TEST

  template<typename FixedPointType>
  const FixedPointType& tolerance_maker(const int fuzzy_bits)
  {
    static const FixedPointType the_tolerance = ldexp(FixedPointType(1), FixedPointType::resolution + fuzzy_bits);

    return the_tolerance;
  }

  template<typename FixedPointType,
           typename FloatPointType = typename FixedPointType::float_type>
  void test_frexp_ldexp(const int fuzzy_bits)
  {
    // Use at least 17 resolution bits.
    // Use at least  6 range bits.

    BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 17);
    BOOST_STATIC_ASSERT( FixedPointType::range      >=  6);

    using std::ldexp;
    using std::frexp;

    const FixedPointType a1(boost::math::constants::pi        <FixedPointType>());      const FloatPointType b1(boost::math::constants::pi        <FloatPointType>());
    const FixedPointType a2(boost::math::constants::e         <FixedPointType>());      const FloatPointType b2(boost::math::constants::e         <FloatPointType>());
    const FixedPointType a3(boost::math::constants::ln_two    <FixedPointType>());      const FloatPointType b3(boost::math::constants::ln_two    <FloatPointType>());
    const FixedPointType a4(boost::math::constants::zeta_three<FixedPointType>());      const FloatPointType b4(boost::math::constants::zeta_three<FloatPointType>());
    const FixedPointType a5(boost::math::constants::pi        <FixedPointType>() * 11); const FloatPointType b5(boost::math::constants::pi        <FloatPointType>() * 11);
    const FixedPointType a6(boost::math::constants::pi        <FixedPointType>() / 11); const FloatPointType b6(boost::math::constants::pi        <FloatPointType>() / 11);
    const FixedPointType a7(boost::math::constants::ln_two    <FixedPointType>() /  5); const FloatPointType b7(boost::math::constants::ln_two    <FloatPointType>() /  5);
    const FixedPointType a8(boost::math::constants::phi       <FixedPointType>() / 11); const FloatPointType b8(boost::math::constants::phi       <FloatPointType>() / 11);

    FixedPointType c1; FixedPointType c2; FixedPointType c3; FixedPointType c4;
    FixedPointType c5; FixedPointType c6; FixedPointType c7; FixedPointType c8;

    int exp2a1; int exp2a2; int exp2a3; int exp2a4;
    int exp2a5; int exp2a6; int exp2a7; int exp2a8;

    int exp2b1; int exp2b2; int exp2b3; int exp2b4;
    int exp2b5; int exp2b6; int exp2b7; int exp2b8;

    BOOST_CHECK_CLOSE_FRACTION(c1 = frexp(a1, &exp2a1), FixedPointType(frexp(b1, &exp2b1)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(c2 = frexp(a2, &exp2a2), FixedPointType(frexp(b2, &exp2b2)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(c3 = frexp(a3, &exp2a3), FixedPointType(frexp(b3, &exp2b3)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(c4 = frexp(a4, &exp2a4), FixedPointType(frexp(b4, &exp2b4)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(c5 = frexp(a5, &exp2a5), FixedPointType(frexp(b5, &exp2b5)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(c6 = frexp(a6, &exp2a6), FixedPointType(frexp(b6, &exp2b6)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(c7 = frexp(a7, &exp2a7), FixedPointType(frexp(b7, &exp2b7)), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(c8 = frexp(a8, &exp2a8), FixedPointType(frexp(b8, &exp2b8)), tolerance_maker<FixedPointType>(fuzzy_bits));

    BOOST_CHECK_EQUAL(exp2a1, exp2b1);
    BOOST_CHECK_EQUAL(exp2a2, exp2b2);
    BOOST_CHECK_EQUAL(exp2a3, exp2b3);
    BOOST_CHECK_EQUAL(exp2a4, exp2b4);
    BOOST_CHECK_EQUAL(exp2a5, exp2b5);
    BOOST_CHECK_EQUAL(exp2a6, exp2b6);
    BOOST_CHECK_EQUAL(exp2a7, exp2b7);
    BOOST_CHECK_EQUAL(exp2a8, exp2b8);

    BOOST_CHECK_CLOSE_FRACTION(ldexp(c1, exp2a1), a1, tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c2, exp2a2), a2, tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c3, exp2a3), a3, tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c4, exp2a4), a4, tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c5, exp2a5), a5, tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c6, exp2a6), a6, tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c7, exp2a7), a7, tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c8, exp2a8), a8, tolerance_maker<FixedPointType>(fuzzy_bits));

    BOOST_CHECK_CLOSE_FRACTION(ldexp(c1, exp2a1), FixedPointType(b1), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c2, exp2a2), FixedPointType(b2), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c3, exp2a3), FixedPointType(b3), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c4, exp2a4), FixedPointType(b4), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c5, exp2a5), FixedPointType(b5), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c6, exp2a6), FixedPointType(b6), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c7, exp2a7), FixedPointType(b7), tolerance_maker<FixedPointType>(fuzzy_bits));
    BOOST_CHECK_CLOSE_FRACTION(ldexp(c8, exp2a8), FixedPointType(b8), tolerance_maker<FixedPointType>(fuzzy_bits));
  }
}

BOOST_AUTO_TEST_CASE(fixed_point_frexp_ldexp)
{
  { typedef boost::fixed_point::negatable<6,  -17> fixed_point_type; static_cast<void>(local::test_frexp_ldexp<fixed_point_type>(5)); }
  { typedef boost::fixed_point::negatable<6,  -25> fixed_point_type; static_cast<void>(local::test_frexp_ldexp<fixed_point_type>(5)); }
  { typedef boost::fixed_point::negatable<6,  -46> fixed_point_type; static_cast<void>(local::test_frexp_ldexp<fixed_point_type>(5)); }
  { typedef boost::fixed_point::negatable<6,  -57> fixed_point_type; static_cast<void>(local::test_frexp_ldexp<fixed_point_type>(5)); }
  { typedef boost::fixed_point::negatable<6, -121> fixed_point_type; static_cast<void>(local::test_frexp_ldexp<fixed_point_type>(5)); }
  { typedef boost::fixed_point::negatable<6, -233> fixed_point_type; static_cast<void>(local::test_frexp_ldexp<fixed_point_type>(5)); }
}
