///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//! \file
//!\brief Perform approximate computations of pi for fixed_point. Use a quadratically convergent Gauss AGM method.

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <type_traits>

#define BOOST_TEST_MODULE fixed_point_local_pi
#define BOOST_LIB_DIAGNOSTIC

#include <boost/cstdfloat.hpp>
#include <boost/cstdint.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  template<typename FixedPointType>
  static const FixedPointType& tolerance_maker(const int fuzzy_bits)
  {
    static const FixedPointType the_tolerance = ldexp(FixedPointType(1), FixedPointType::resolution + fuzzy_bits);

    return the_tolerance;
  }

  template<typename FixedPointType,
           typename FloatPointType,
           typename EnableType = void>
  struct pi_helper
  {
    static void test_pi(const int fuzzy_bits)
    {
      // Use at least as many bits as the sum of (bits of float32_t + 1).
      // Use at least 4 range bits.

      BOOST_STATIC_ASSERT((FixedPointType::range - FixedPointType::resolution) >= (std::numeric_limits<boost::float32_t>::digits + 1));
      BOOST_STATIC_ASSERT( FixedPointType::range >= 4);

      FixedPointType val_pi;

      FixedPointType a (1);
      FixedPointType bB(FixedPointType(1) / 2);
      FixedPointType s (FixedPointType(1) / 2);
      FixedPointType t (FixedPointType(3) / 8);

      // This loop is designed for computing a maximum of a few million
      // decimal digits of pi. The number of digits roughly doubles
      // with each iteration of the loop. After 20 iterations,
      // the precision is about 2.8 million decimal digits.
      // We are not using that many digits in these tests,
      // only up to a few thousand at most.

      for(boost::uint_least8_t k = UINT8_C(1); k < UINT8_C(32); ++k)
      {
        // Perform the iteration steps of the Gauss AGM.

        a      += sqrt(bB);
        a      /= 2U;
        val_pi  = (a * a);
        bB      = (val_pi - t) * 2U;

        const FixedPointType iterate_term((bB - val_pi) * (UINT32_C(1) << k));

        s += iterate_term;

        const bool minimum_iterations_are_complete = (k > UINT8_C(2));

        if(minimum_iterations_are_complete)
        {
          // Extract the exponent of the iteration term in order to
          // obtain a rough estimate of the number of base-2 digits
          // that have been obtained in this iteration.

          int exp2;
          static_cast<void>(frexp(iterate_term, &exp2));

          BOOST_ASSERT_MSG((exp2 != 0) && (iterate_term != 0),
                           "Error in pi test: The iteration term should not be zero");

          const bool precision_goal_has_been_reached = (exp2 <= ((FixedPointType::resolution - 1) / 2));

          if(precision_goal_has_been_reached)
          {
            break;
          }
        }

        t = (val_pi + bB) / 4U;
      }

      val_pi += bB;
      val_pi /= s;

      BOOST_CHECK_CLOSE_FRACTION(val_pi,
                                 boost::math::constants::pi<FloatPointType>(),
                                 tolerance_maker<FixedPointType>(fuzzy_bits));
    }
  };

  template<typename FixedPointType,
           typename FloatPointType>
  struct pi_helper<FixedPointType,
                   FloatPointType,
                   typename std::enable_if<std::is_floating_point<FloatPointType>::value>::type>
  {
    static void test_pi(const int fuzzy_bits)
    {
      // Use at least 3 resolution bits.
      // Use at least 2 range bits.

      BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 3);
      BOOST_STATIC_ASSERT( FixedPointType::range      >= 2);

      static const FixedPointType val_pi(3.1415926535897932384626433832795028841972L);

      BOOST_CHECK_CLOSE_FRACTION(val_pi,
                                 boost::math::constants::pi<FloatPointType>(),
                                 tolerance_maker<FixedPointType>(fuzzy_bits));
    }
  };
}

BOOST_AUTO_TEST_CASE(fixed_point_local_pi)
{
  { typedef boost::fixed_point::negatable< 2,    -3> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(1); }
  { typedef boost::fixed_point::negatable< 2,    -5> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(1); }
  { typedef boost::fixed_point::negatable< 2,   -13> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(2); }
  { typedef boost::fixed_point::negatable< 4,   -19> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(4); }
  { typedef boost::fixed_point::negatable< 4,   -27> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(4); }
  { typedef boost::fixed_point::negatable< 8,   -44> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(((-fixed_point_type::resolution + 9) * 2) / 10); }
  { typedef boost::fixed_point::negatable< 8,   -55> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(((-fixed_point_type::resolution + 9) * 2) / 10); }
  { typedef boost::fixed_point::negatable<16,   -96> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(((-fixed_point_type::resolution + 9) * 2) / 10); }
  { typedef boost::fixed_point::negatable<16,  -111> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(((-fixed_point_type::resolution + 9) * 2) / 10); }
  { typedef boost::fixed_point::negatable<16,  -223> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(((-fixed_point_type::resolution + 9) * 2) / 10); }
  { typedef boost::fixed_point::negatable<16,  -495> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(((-fixed_point_type::resolution + 9) * 1) / 10); }
  { typedef boost::fixed_point::negatable<16, -1007> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(((-fixed_point_type::resolution + 9) * 1) / 10); }
  { typedef boost::fixed_point::negatable<16, -2031> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(((-fixed_point_type::resolution + 9) * 1) / 10); }
  { typedef boost::fixed_point::negatable<16, -4079> fixed_point_type; local::pi_helper<fixed_point_type, fixed_point_type::float_type>::test_pi(((-fixed_point_type::resolution + 9) * 1) / 10); }
}
