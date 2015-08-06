///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//! \file
//!\brief Perform local computations of pi and ln_two for fixed_point. Use quadratically convergent Gauss AGM methods.

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <type_traits>

#define BOOST_TEST_MODULE fixed_point_local_constants
#define BOOST_LIB_DIAGNOSTIC

#include <boost/cstdfloat.hpp>
#include <boost/cstdint.hpp>
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

  template<typename FixedPointType,
           typename FloatingPointType,
           typename EnableType = void>
  struct constants_helper
  {
  public:
    static FixedPointType pi(const int fuzzy_bits)
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

        const bool minimum_number_of_iterations_are_complete = (k > UINT8_C(2));

        if(minimum_number_of_iterations_are_complete)
        {
          // Extract the exponent of the iteration term in order to
          // obtain a rough estimate of the number of base-2 digits
          // that have been obtained in this iteration.

          int exp2;
          static_cast<void>(frexp(iterate_term, &exp2));

          const bool iteration_term_is_zero = ((exp2 == 0) || (iterate_term == 0));

          const bool precision_goal_has_been_reached = (exp2 <= ((FixedPointType::resolution - 1) / 2));

          if(precision_goal_has_been_reached || iteration_term_is_zero)
          {
            break;
          }
        }

        t = (val_pi + bB) / 4U;
      }

      val_pi += bB;
      val_pi /= s;

      const FixedPointType control_value(boost::math::constants::pi<FloatingPointType>());

      BOOST_CHECK_CLOSE_FRACTION(val_pi, control_value, tolerance_maker(fuzzy_bits));

      return val_pi;
    }

    static FixedPointType ln_two(const int fuzzy_bits)
    {
      // The calculation of ln_two requires significantly more precision
      // in both the range as well as the resolution in order to be
      // calculated with the Gauss AGM method. For this reasn, we create
      // a larger local fixed-point type that is large enough to compensate
      // for all the digit loss in the calculation.

      typedef boost::fixed_point::negatable<(-FixedPointType::resolution) * 2,
                                              FixedPointType::resolution  * 2>
      large_fixed_point_type;

      // Use at least as many bits as the sum of (bits of float32_t + 1).
      // Use at least 2 range bits.

      BOOST_STATIC_ASSERT((FixedPointType::range - FixedPointType::resolution) >= (std::numeric_limits<boost::float32_t>::digits + 1));
      BOOST_STATIC_ASSERT( FixedPointType::range >= 2);

      // Choose m > (N * 1.661), where N is the number of decimal digits requested.
      const int m = static_cast<int>((((-large_fixed_point_type::resolution) * 301) / 1000) * 1.7F);

      // Set a0 = 1.
      // Set b0 = 4 / (2^m) = 1 / 2^(m - 2).
      large_fixed_point_type ak(1);

      large_fixed_point_type bk(ldexp(large_fixed_point_type(1), -(m - 2)));

      for(boost::uint_least8_t k = UINT8_C(0); k < UINT8_C(32); ++k)
      {
        const large_fixed_point_type a(ak);
        ak += bk;
        ak /= 2U;
        bk  = sqrt(bk * a);

        const bool minimum_number_of_iterations_are_complete = (k > UINT8_C(4));

        if(minimum_number_of_iterations_are_complete)
        {
          const large_fixed_point_type delta_ak_bk = fabs(ak - bk);

          // Extract the exponent of the iteration term in order to
          // obtain a rough estimate of the number of base-2 digits
          // that have been obtained in this iteration.

          int exp2;
          static_cast<void>(frexp(delta_ak_bk, &exp2));

          const bool delta_ak_bk_is_zero = ((exp2 == 0) || (delta_ak_bk == 0));

          const bool precision_goal_has_been_reached = (exp2 <= ((large_fixed_point_type::resolution * 3) / 4));

          if(precision_goal_has_been_reached || delta_ak_bk_is_zero)
          {
            break;
          }
        }
      }

      // The iteration is finished: Compute ln2 = pi / [2m * AGM(1, 4 / 2^m)].
      // Note that ak = bk = AGM(...).

      const large_fixed_point_type large_val_ln_two =
        large_fixed_point_type(boost::math::constants::pi<typename large_fixed_point_type::float_type>()) / (ak * (2U * m));

      const FixedPointType val_ln_two(large_val_ln_two);

      const FixedPointType control_value(private_ln_two_avoid_problem_with_boost_math_constants(std::numeric_limits<FloatingPointType>::digits));

      BOOST_CHECK_CLOSE_FRACTION(val_ln_two, control_value, tolerance_maker(fuzzy_bits));

      return val_ln_two;
    }

  private:
    static FixedPointType tolerance_maker(const int fuzzy_bits)
    {
      return ldexp(FixedPointType(1),
                   FixedPointType::resolution + fuzzy_bits);
    }

    static FloatingPointType private_ln_two_avoid_problem_with_boost_math_constants(const int digits2)
    {
      // Here we implement a local private generator of ln_two in order to avoid
      // what appears to be defects in multiprecision for certain compilers
      // or minor compiler defects.

      // TBD: Try to characterize and correct the (potential) defects
      // in multiprecision, if there prove to be any (which is unclear
      // at the moment).

      using std::fabs;
      using std::frexp;
      using std::ldexp;

      const int digits10 = static_cast<int>((static_cast<long>(digits2) * 301L) / 1000L);

      // Choose m > (N * 1.661), where N is the number of decimal digits requested.
      const int m = static_cast<int>(digits10 * 1.7F);

      // Set a0 = 1.
      // Set b0 = 4 / (2^m) = 1 / 2^(m - 2).
      FloatingPointType ak(1);

      FloatingPointType bk(ldexp(FloatingPointType(1), -(m - 2)));

      for(boost::uint_least8_t k = UINT8_C(0); k < UINT8_C(32); ++k)
      {
        const FloatingPointType a(ak);
        ak += bk;
        ak /= 2U;
        bk  = sqrt(bk * a);

        const bool minimum_number_of_iterations_are_complete = (k > UINT8_C(4));

        if(minimum_number_of_iterations_are_complete)
        {
          const FloatingPointType delta_ak_bk = fabs(ak - bk);

          // Extract the exponent of the iteration term in order to
          // obtain a rough estimate of the number of base-2 digits
          // that have been obtained in this iteration.

          int exp2;
          static_cast<void>(frexp(delta_ak_bk, &exp2));

          const bool delta_ak_bk_is_zero = ((exp2 == 0) || (delta_ak_bk == 0));

          const bool precision_goal_has_been_reached = (exp2 <= (((-digits2) * 3) / 4));

          if(precision_goal_has_been_reached || delta_ak_bk_is_zero)
          {
            break;
          }
        }
      }

      // The iteration is finished: Compute ln2 = pi / [2m * AGM(1, 4 / 2^m)].
      // Note that ak = bk = AGM(...).

      const FloatingPointType val_ln_two =
        (boost::math::constants::pi<FloatingPointType>()) / (ak * (2U * m));

      return val_ln_two;
    }
  };

  template<typename FixedPointType,
           typename FloatingPointType>
  struct constants_helper<FixedPointType,
                          FloatingPointType,
                          typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type>
  {
  public:
    static FixedPointType pi(const int fuzzy_bits)
    {
      // Use at least 3 resolution bits.
      // Use at least 2 range bits.

      BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 3);
      BOOST_STATIC_ASSERT( FixedPointType::range      >= 2);

      static const FixedPointType val_pi(3.1415926535897932384626433832795028841972L);

      static const FixedPointType control_value(boost::math::constants::pi<FloatingPointType>());

      BOOST_CHECK_CLOSE_FRACTION(val_pi, control_value, tolerance_maker(fuzzy_bits));

      return val_pi;
    }

    static FixedPointType ln_two(const int fuzzy_bits)
    {
      // Use at least 3 resolution bits.
      // Use at least 0 range bits.

      BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 3);
      BOOST_STATIC_ASSERT( FixedPointType::range      >= 0);

      static const FixedPointType val_ln_two(0.6931471805599453094172321214581765680755L);

      static const FixedPointType control_value(boost::math::constants::ln_two<FloatingPointType>());

      BOOST_CHECK_CLOSE_FRACTION(val_ln_two, control_value, tolerance_maker(fuzzy_bits));

      return val_ln_two;
    }

  private:
    static FixedPointType tolerance_maker(const int fuzzy_bits)
    {
      return ldexp(FixedPointType(1),
                   FixedPointType::resolution + fuzzy_bits);
    }
  };
}

BOOST_AUTO_TEST_CASE(fixed_point_local_constants)
{
  { typedef boost::fixed_point::negatable< 2,    -3> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(1)); }
  { typedef boost::fixed_point::negatable< 2,    -5> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(1)); }
  { typedef boost::fixed_point::negatable< 2,   -13> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(2)); }
  { typedef boost::fixed_point::negatable< 4,   -19> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(4)); }
  { typedef boost::fixed_point::negatable< 4,   -27> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(4)); }
  { typedef boost::fixed_point::negatable< 8,   -44> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable< 8,   -55> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<16,   -96> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<16,  -111> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<16,  -223> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<16,  -495> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<16, -1007> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<16, -2031> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<16, -4079> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 1) / 10)); }

  { typedef boost::fixed_point::negatable<0,    -5> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(1)); }
  { typedef boost::fixed_point::negatable<0,    -7> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(1)); }
  { typedef boost::fixed_point::negatable<1,   -14> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(2)); }
  { typedef boost::fixed_point::negatable<1,   -21> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(4)); }
  { typedef boost::fixed_point::negatable<2,   -29> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(4)); }
  { typedef boost::fixed_point::negatable<2,   -50> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2,   -61> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2,  -110> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2,  -125> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2,  -253> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2,  -509> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<2, -1021> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<2, -2045> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<2, -4093> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 1) / 10)); }
}
