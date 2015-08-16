///////////////////////////////////////////////////////////////////////////////
// Copyright Christopher Kormanyos 2014.
// Copyright John Maddock 2014.
// Copyright Paul Bristow 2014.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// fixed_point_detail_constants.hpp implements templates
// for computing fixed-point representations of mathematical
// constants such as pi and log(2).

#ifndef FIXED_POINT_DETAIL_CONSTANTS_2015_08_16_HPP_
  #define FIXED_POINT_DETAIL_CONSTANTS_2015_08_16_HPP_

  #include <cmath>
  #include <limits>

  namespace boost { namespace fixed_point { namespace detail {

  template<typename NumericType>
  NumericType calculate_pi()
  {
    // Use a quadratically converging Gauss-AGM method for computing pi.

    using std::frexp;
    using std::sqrt;

    NumericType val_pi;

    NumericType a (1);
    NumericType bB(NumericType(1) / 2);
    NumericType s (NumericType(1) / 2);
    NumericType t (NumericType(3) / 8);

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

      const NumericType iterate_term((bB - val_pi) * (UINT32_C(1) << k));

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

        BOOST_CONSTEXPR_OR_CONST int precision_goal = -int((long(std::numeric_limits<NumericType>::digits) * 3L) / 4L);

        const bool precision_goal_has_been_reached = (exp2 <= precision_goal);

        if(precision_goal_has_been_reached || iteration_term_is_zero)
        {
          break;
        }
      }

      t = (val_pi + bB) / 4U;
    }

    val_pi += bB;
    val_pi /= s;

    return val_pi;
  }

  template<typename NumericType>
  NumericType calculate_ln_two()
  {
    // Use a quadratically converging Gauss-AGM method for computing log(2).

    using std::frexp;
    using std::sqrt;

    // Choose m > (N * 1.661), where N is the number of decimal digits requested.
    BOOST_CONSTEXPR_OR_CONST int m = int((long(std::numeric_limits<NumericType>::digits10) * 17L) / 10L);

    // Set a0 = 1.
    // Set b0 = 4 / (2^m) = 1 / 2^(m - 2).
    NumericType ak(1);

    NumericType bk(ldexp(NumericType(1), -(m - 2)));

    for(boost::uint_least8_t k = UINT8_C(0); k < UINT8_C(32); ++k)
    {
      const NumericType a(ak);
      ak += bk;
      ak /= 2U;
      bk  = sqrt(bk * a);

      const bool minimum_number_of_iterations_are_complete = (k > UINT8_C(4));

      if(minimum_number_of_iterations_are_complete)
      {
        const NumericType delta_ak_bk = fabs(ak - bk);

        // Extract the exponent of the iteration term in order to
        // obtain a rough estimate of the number of base-2 digits
        // that have been obtained in this iteration.

        int exp2;
        static_cast<void>(frexp(delta_ak_bk, &exp2));

        const bool delta_ak_bk_is_zero = ((exp2 == 0) || (delta_ak_bk == 0));

        BOOST_CONSTEXPR_OR_CONST int precision_goal = -int((long(std::numeric_limits<NumericType>::digits) * 3L) / 4L);

        const bool precision_goal_has_been_reached = (exp2 <= precision_goal);

        if(precision_goal_has_been_reached || delta_ak_bk_is_zero)
        {
          break;
        }
      }
    }

    // Define the large floating-point type that corresponds to the
    // large fixed-point type.

    // The iteration is finished: Compute ln2 = pi / [AGM(1, 4 / 2^m) * 2m].
    // Note that ak = bk = AGM(...).
    const NumericType val_ln_two = calculate_pi<NumericType>() / (ak * (2 * m));

    return val_ln_two;
  }
  } } } // namespace boost::fixed_point::detail

#endif // FIXED_POINT_DETAIL_CONSTANTS_2015_08_16_HPP_
