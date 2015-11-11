///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2013 - 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is a partial reference implementation for the proposed
// "C++ binary fixed-point arithmetic" as specified in N3352.
// See: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html

/*!
  \file
  \brief Implement most fixed-point negatable \<cmath\> functions.
*/

#ifndef FIXED_POINT_NEGATABLE_CMATH_2015_08_21_HPP_
  #define FIXED_POINT_NEGATABLE_CMATH_2015_08_21_HPP_

  namespace boost { namespace fixed_point {

  /*! Absolute function. 
  \sa http://en.cppreference.com/w/cpp/numeric/math/abs
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> abs(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    return ((x.data < 0) ? -x : x);
  }

  /*! @c std::fabs function (identical to @c abs and provided for completeness).
    \sa http://en.cppreference.com/w/cpp/numeric/math/abs
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> fabs(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    return ((x.data < 0) ? -x : x);
  }
  /*! Floor function, computes largest integer value not greater than arg.
  \sa http://en.cppreference.com/w/cpp/numeric/math/floor
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> floor(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    const local_unsigned_small_type integral_part_mask((local_unsigned_small_type(((std::numeric_limits<local_negatable_type>::max)()).data) >> (-FractionalResolution)) << (-FractionalResolution));

    if(x.data < 0)
    {
      const local_unsigned_small_type u_mask(local_unsigned_small_type(-x.data) & integral_part_mask);

      return ((local_unsigned_small_type(-x.data) == u_mask) ? x : (local_negatable_type(local_nothing(), -local_value_type(u_mask)) - 1));
    }
    else if(x.data > 0)
    {
      const local_unsigned_small_type u_mask(local_unsigned_small_type(x.data) & integral_part_mask);

      return local_negatable_type(local_nothing(), u_mask);
    }
    else
    {
      return local_negatable_type(0U);
    }
  }
  /*! Ceiling function computes the smallest integer value not less than arg.
    \sa http://en.cppreference.com/w/cpp/numeric/math/ceil
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> ceil(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    const local_unsigned_small_type integral_part_mask((local_unsigned_small_type(((std::numeric_limits<local_negatable_type>::max)()).data) >> (-FractionalResolution)) << (-FractionalResolution));

    if(x.data < 0)
    {
      const local_unsigned_small_type u_mask(local_unsigned_small_type(-x.data) & integral_part_mask);

      return local_negatable_type(local_nothing(), -local_value_type(u_mask));
    }
    else if(x.data > 0)
    {
      const local_unsigned_small_type u(x.data);

      const local_unsigned_small_type u_mask(u & integral_part_mask);

      return ((u == u_mask) ? x : (local_negatable_type(local_nothing(), local_value_type(u_mask)) + 1));
    }
    else
    {
      return local_negatable_type(0U);
    }
  }
  /*! truncation function computes the nearest integer not greater in magnitude than arg.
  \sa  http://en.cppreference.com/w/cpp/numeric/math/trunc
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> trunc(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    return ((x.data < 0) ? -floor(-x) : floor(x));
  }

  /*! @c std::frexp function \<cmath\> implementation for negatable types.\n

  TBD examples.\n
      \param x Fixed-point value to decompose into fraction and integral power of 2.
      \param exp2 Pointer to integer value to store the exponent to. 
      \returns fraction part as a @c negatable type.
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> frexp(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, int* exp2)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x.data == 0)
    {
      *exp2 = 0;

      return local_negatable_type(0U);
    }
    else
    {
      const bool is_neg = (x.data < 0);

      // Extract the unsigned representation of the data field.
      local_unsigned_small_type result((!is_neg) ? local_unsigned_small_type(x.data) : local_unsigned_small_type(-x.data));

      boost::uint_fast16_t msb;

      {
        // Use a binary-halving mechanism to obtain the most significant bit.
        // This will subsequently be used for determination of the binary exponent.
        BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t unsigned_small_digits =
          static_cast<boost::uint_fast16_t>(std::numeric_limits<local_unsigned_small_type>::digits);

        local_unsigned_small_type unsigned_small_mask((std::numeric_limits<local_unsigned_small_type>::max)());

        local_unsigned_small_type tmp(result);

        msb = detail::msb_helper(tmp, unsigned_small_mask, unsigned_small_digits);
      }

      // Evaluate the necessary amount of right-shift.
      const int total_right_shift = (int(msb) - local_negatable_type::radix_split) + 1;

      // Compute the (still unsigned) data field and the exponent.
      result = detail::right_shift_helper(result, total_right_shift);

      *exp2 = total_right_shift;

      // Create and return the signed fixed-point result.
      return local_negatable_type(local_nothing(), local_value_type((!is_neg) ?  local_value_type(result)
                                                                              : -local_value_type(result)));
    }
  }

  /*! @c std::ldexp function \<cmath\> implementation for negatable type.\n
      Multiplies a floating point value x by the number 2 raised to the exp2 power.\n
      TBD examples.\n

      \param x Fixed-point value to multiply by @c exp2 integral power of 2.
      \param exp2 power of 2 to use to multiply.
      \return x * 2^exp2.
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> ldexp(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, int exp2)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(exp2 > 0)
    {
      return local_negatable_type(local_nothing(), local_value_type(x.data << exp2));
    }
    else if(exp2 < 0)
    {
      const bool is_neg = (x.data < 0);

      local_unsigned_small_type result((!is_neg) ? local_unsigned_small_type(x.data) : local_unsigned_small_type(-x.data));

      result >>= -exp2;

      return local_negatable_type(local_nothing(), ((!is_neg) ?  local_value_type(result)
                                                              : -local_value_type(result)));
    }
    else
    {
      return x;
    }
  }
  /*! Computes the fixed_point remainder of the division operation.
   \sa http://en.cppreference.com/w/cpp/numeric/math/fmod
   */

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> fmod(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> y)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(y == 0)
    {
      return local_negatable_type(0U);
    }

    // Calculate the fractional part of x such that:
    //   x = (integer_part * y) + fractional_part,
    // where
    //   |fractional_part| < |y|,
    // and fractional_part has the same sign as x.

    const local_negatable_type integer_part = floor(x / y);

    local_negatable_type fractional_part = x - (integer_part * y);

    if((x.data < 0) != (y < 0))
    {
      fractional_part -= y;
    }

    return fractional_part;
  }
  /*! Computes square root for fractional resolution < 11 bits.
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sqrt(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(11) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle arguments negative, zero, or unity.
    if(x <= 0)
    {
      return local_negatable_type(0U);
    }
    else if(x == local_negatable_type(1U))
    {
      return local_negatable_type(1U);
    }

    int n;

    // Use range reduction for (x < +1/2) or (x > 1).
    if((x < ldexp(local_negatable_type(1U), -1)) || (x > 1))
    {
      x = frexp(x, &n);
    }
    else
    {
      n = 0;
    }

    local_negatable_type px_numerator;
    local_negatable_type qx_denominator;

    // Here we break the reduced argument range yet further in half.
    // This results in two sub-ranges:
    //  the *lower* half of the range with (x >= 1/2 && x < 3/4)
    // and
    //  the *upper* half of the range with (x >= 3/4 && x < 1).

    if(x < local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0600) >> (11 + FractionalResolution))))
    {
      // Compute the sqrt(x) in the lower half of the range with (x >= 1/2 && x < 3/4).

      // Use an order 2/1 Pade approximation at (x = 5/8) resulting in:
      //  sqrt(x) = approx. P(x) / Q(x),
      // where
      //  P(x) =   0.7905694150420948330
      //         + 1.2649110640673517328 (x - (5/8))
      //         + 0.2529822128134703466 (x - (5/8))^2
      // and
      //  Q(x) =   1.0000000000000000000
      //         + 0.8000000000000000000 (x - (5/8))
      // in the range +1/2 <= x <= +3/4. These coefficients
      // have been specifically derived for this work.

      // Set the variable xp = x - (5/8).
      const local_negatable_type xp = x - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0500) >> (11 + FractionalResolution)));

      px_numerator   = ((       local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0206) >> (11 + FractionalResolution)))   // 0.2529822128134703466
                         * xp + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0A1E) >> (11 + FractionalResolution))))  // 1.2649110640673517328
                         * xp + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0653) >> (11 + FractionalResolution)))); // 0.7905694150420948330

      qx_denominator = (        local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0666) >> (11 + FractionalResolution)))   // 0.8000000000000000000
                         * xp + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0800) >> (11 + FractionalResolution)))); // 1.0000000000000000000
    }
    else
    {
      // Compute the sqrt(x) in the upper half of the range with (x >= 3/4 && x < 1).

      // Use an order 2/1 Pade approximation at (x = 7/8) resulting in:
      //  sqrt(x) = approx. P(x) / Q(x),
      // where
      //  P(x) =   0.9354143466934853464
      //         + 1.0690449676496975387 (x - (7/8))
      //         + 0.1527207096642425055 (x - (7/8))^2
      // and
      //  Q(x) =   1.0000000000000000000
      //         + 0.5714285714285714286 (x - (7/8))
      // in the range +3/4 <= x <= +1. These coefficients
      // have been specifically derived for this work.

      // Set the variable xp = x - (7/8).
      const local_negatable_type xp = x - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0700) >> (11 + FractionalResolution)));

      px_numerator   = ((       local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0138) >> (11 + FractionalResolution)))   // 0.2529822128134703466
                         * xp + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x088D) >> (11 + FractionalResolution))))  // 1.2649110640673517328
                         * xp + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x077B) >> (11 + FractionalResolution)))); // 0.7905694150420948330

      qx_denominator = (        local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0492) >> (11 + FractionalResolution)))   // 0.8000000000000000000
                         * xp + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0800) >> (11 + FractionalResolution)))); // 1.0000000000000000000
    }

    local_negatable_type result = px_numerator / qx_denominator;

    const bool is_odd_scaling = ((boost::int_fast8_t(n) & INT8_C(1)) != INT8_C(0));

    // Rescale the result. In certain cases there is one extra
    // power of two. If so, either multiply with or divide by
    // the square root of 2 in order to complete the rescaling
    // of the result.
    if(n > 0)
    {
      if(is_odd_scaling)
      {
        // Multiply with sqrt(2).
        BOOST_CONSTEXPR_OR_CONST local_negatable_type root_two(local_nothing(), local_value_type(UINT16_C(0x0B50) >> (11 + FractionalResolution)));

        result *= root_two;
      }

      // Left-shift the result by 1/2 of the even factors of 2.
      result.data <<= (n / 2);
    }
    else if(n < 0)
    {
      if(is_odd_scaling)
      {
        // Divide by sqrt(2), which is actually accomplished via multiplication by [1 / sqrt(2)].
        BOOST_CONSTEXPR_OR_CONST local_negatable_type one_over_root_two(local_nothing(), local_value_type(UINT16_C(0x05A8) >> (11 + FractionalResolution)));

        result *= one_over_root_two;
      }

      // Right-shift the result by 1/2 of the even factors of 2.
      result.data = local_value_type(local_unsigned_small_type(result.data) >> (-n / 2));
    }

    return result;
  }
 
  /*! Computes square root for fractional resolution < 24 bits.
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sqrt(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<(int(24) >= (-FractionalResolution)) && (int(11) < (-FractionalResolution))>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle arguments negative, zero, or unity.
    if(x <= 0)
    {
      return local_negatable_type(0U);
    }
    else if(x == 1)
    {
      return local_negatable_type(1U);
    }

    int n;

    // Use range reduction for (x < +1/2) or (x > 1).
    if((x < ldexp(local_negatable_type(1U), -1)) || (x > 1))
    {
      x = frexp(x, &n);
    }
    else
    {
      n = 0;
    }

    // Use an order 4 Pade approximation at (x = 3/4) resulting in:
    //  sqrt(x) = approx. P(x) / Q(x),
    // where
    //  P(x) =   0.8660254037844386468
    //         + 2.5980762113533159403 (x - 0.75)
    //         + 2.5980762113533159403 (x - 0.75)^2
    //         + 0.9622504486493762742 (x - 0.75)^3
    //         + 0.0962250448649376274 (x - 0.75)^4
    // and
    //  Q(x) =   1.0000000000000000000
    //         + 2.3333333333333333333 (x - 0.75)
    //         + 1.6666666666666666667 (x - 0.75)^2
    //         + 0.3703703703703703704 (x - 0.75)^3
    //         + 0.0123456790123456790 (x - 0.75)^4
    // in the range +1/2 <= x <= +1. These coefficients
    // have been specifically derived for this work.

    // Set the variable xp = x - (3/4).
    const local_negatable_type xp = x - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00C00000) >> (24 + FractionalResolution)));

    const local_negatable_type px_numerator =
      ((((       local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0018A234) >> (24 + FractionalResolution)))   // 0.0962250448649376274
          * xp + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00F6560B) >> (24 + FractionalResolution))))  // 0.9622504486493762742
          * xp + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x02991B85) >> (24 + FractionalResolution))))  // 2.5980762113533159403
          * xp + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x02991B85) >> (24 + FractionalResolution))))  // 2.5980762113533159403
          * xp + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00DDB3D7) >> (24 + FractionalResolution)))); // 0.8660254037844386468

    const local_negatable_type qx_denominator =
      ((((       local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00032916) >> (24 + FractionalResolution)))   // 0.0123456790123456790
          * xp + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x005ED097) >> (24 + FractionalResolution))))  // 0.3703703703703703704
          * xp + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x01AAAAAA) >> (24 + FractionalResolution))))  // 1.6666666666666666667
          * xp + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x02555555) >> (24 + FractionalResolution))))  // 2.3333333333333333333
          * xp + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x01000000) >> (24 + FractionalResolution)))); // 1.0000000000000000000

    local_negatable_type result = px_numerator / qx_denominator;

    const bool is_odd_scaling = ((boost::int_fast8_t(n) & INT8_C(1)) != INT8_C(0));

    // Rescale the result. For n odd, there is one extra
    // factor of two in the scaling. If so, either multiply with
    // or divide by the square root of 2 in order to complete
    // the rescaling of the result.
    if(n > 0)
    {
      if(is_odd_scaling)
      {
        // Multiply with sqrt(2).
        BOOST_CONSTEXPR_OR_CONST local_negatable_type root_two(local_nothing(), local_value_type(UINT32_C(0x016A09E6) >> (24 + FractionalResolution)));

        result *= root_two;
      }

      // Left-shift the result by 1/2 of the even factors of 2.
      result.data <<= (n / 2);
    }
    else if(n < 0)
    {
      if(is_odd_scaling)
      {
        // Divide by sqrt(2), which is actually accomplished via multiplication by [1 / sqrt(2)].
        BOOST_CONSTEXPR_OR_CONST local_negatable_type one_over_root_two(local_nothing(), local_value_type(UINT32_C(0x00B504F3) >> (24 + FractionalResolution)));

        result *= one_over_root_two;
      }

      // Right-shift the result by 1/2 of the even factors of 2.
      result.data = local_value_type(local_unsigned_small_type(result.data) >> (-n / 2));
    }

    return result;
  }

  /*! Computes sqrt for fractional resolution > 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sqrt(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;

    // Handle arguments negative, zero, or unity.
    if(x <= 0)
    {
      return local_negatable_type(0U);
    }
    else if(x == local_negatable_type(1U))
    {
      return local_negatable_type(1U);
    }

    // Find the most significant bit in order to perform range reduction.

    // Evaluate the necessary amount of right-shift for computing
    // the initial guess of the square root. For this we need to
    // compute the reduced argument as x = [2^n * a] and subsequently
    // remove the powers of 2.
    int n2;
    local_negatable_type a = frexp(x, &n2);

    // Get the initial estimate of the square root.

    // Use the reduced argument (a) in order to create an
    // estimate for the initial guess of the square root of x.
    // Here we use:
    //  sqrt(a) = approx. (a/2) + [8^(1/4) - 1]^2
    //          = approx. (a/2) + 0.4648
    //          = approx. (a/2) + (1/2) [via naive simplification].

    // See, for example, J. F. Hart et al., Computer Approximations
    // (John Wiley and Sons, Inc., 1968), Eq. 2.4.3 on page 27.

    // Compute the reduced argument (a) and divide the value
    // of (a) once again by 2 in order to compute the first part
    // of the initial guess.
    a.data = local_value_type(local_unsigned_small_type(a.data) >> 1);

    // Here is the addition of (1/2) to complete the initial guess.
    a += ldexp(local_negatable_type(1U), -1);

    // Remove the scaling from the reduced guess of the result
    // and use this as the proper initial guess of sqrt(x).
    if((n2 / 2) > 0)
    {
      a.data <<= (n2 / 2);
    }
    else if((n2 / 2) < 0)
    {
      a.data = local_value_type(local_unsigned_small_type(a.data) >> (-n2 / 2));
    }

    if((n2 % 2) != 0)
    {
      // There is one extra power of two. Either multiply with
      // or divide by the square root of 2 in order to complete
      // the rescaling of the initial guess.
      if(n2 > 0)
      {
        a *= local_negatable_type::value_root_two();
      }
      else
      {
        a /= local_negatable_type::value_root_two();
      }
    }

    // Estimate the zero'th term of the iteration with [1 / (2 * result)].
    local_negatable_type vi = local_negatable_type(1U) / (a * local_negatable_type(2U));

    // Compute the square root of x using coupled Newton iteration.
    // More precisely, this is the Schoenhage variation thereof.
    // We begin with an estimate of 1 binary digit of precision and
    // double the number of binary digits of precision with each iteration.

    for(boost::uint_fast16_t i = UINT16_C(1); i <= boost::uint_fast16_t(local_negatable_type::all_bits / 2); i *= UINT16_C(2))
    {
      // Perform the next iteration of vi.
      vi += vi * (-((a * vi) * 2U) + 1U);

      // Perform the next iteration of the result.
      a += (vi * (-((a) * (a)) + x));
    }

    return a;
  }
  /*! Computes exponent for fractional resolution < 11 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> exp(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(11) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle zero argument.
    if(x.data == 0)
    {
      return local_negatable_type(1U);
    }

    // Handle unity argument.
    if(x == local_negatable_type(1U))
    {
      return local_negatable_type::value_e();
    }

    // Handle reflection for negative arguments.
    if(x.data < 0)
    {
      return 1 / exp(-x);
    }

    int nf = 0;

    if(x > local_negatable_type::value_ln_two())
    {
      nf = int(x / local_negatable_type::value_ln_two());

      x -= (local_negatable_type::value_ln_two() * nf);
    }

    // Use a polynomial approximation.
    // exp(x) - 1 = approx. + 0.99792060811054451 x
    //                      + 0.49948832659208106 x^2
    //                      + 0.17621882066892929 x^3
    //                      + 0.04348249331418186 x^4,
    // in the range -1 <= x <= +1. These coefficients
    // have been specifically derived for this work.

    // Perform the polynomial approximation using a coefficient
    // expansion via the method of Horner.
    local_negatable_type result =
      (((      local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0059) >> (11 + FractionalResolution)))   // 0.04348249331418186
         * x + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0168) >> (11 + FractionalResolution))))  // 0.17621882066892929
         * x + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x03FE) >> (11 + FractionalResolution))))  // 0.49948832659208106
         * x + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x07FB) >> (11 + FractionalResolution))))  // 0.99792060811054451
         * x;

    ++result;

    if(nf > 0)
    {
      result.data <<= nf;
    }

    return result;
  }

  /*! Computes exponent for fractional resolution  24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> exp(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<(int(24) >= (-FractionalResolution)) && (int(11) < (-FractionalResolution))>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle zero argument.
    if(x.data == 0)
    {
      return local_negatable_type(1U);
    }

    // Handle unity argument.
    if(x == local_negatable_type(1U))
    {
      return local_negatable_type::value_e();
    }

    // Handle reflection for negative arguments.
    if(x.data < 0)
    {
      return 1 / exp(-x);
    }

    int nf = 0;

    if(x > local_negatable_type::value_ln_two())
    {
      nf = int(x / local_negatable_type::value_ln_two());

      x -= (local_negatable_type::value_ln_two() * nf);
    }

    // Use a polynomial approximation.
    // exp(x) - 1 = approx. + 0.9999999247233815 x
    //                      + 0.5000038123608412 x^2
    //                      + 0.1666677606703556 x^3
    //                      + 0.0416417643582255 x^4
    //                      + 0.0083291093882400 x^5
    //                      + 0.0014345483118123 x^6
    //                      + 0.0002043732656744 x^7,
    // in the range -1 <= x <= +1. These coefficients
    // have been specifically derived for this work.

    // Perform the polynomial approximation using a coefficient
    // expansion via the method of Horner.
    local_negatable_type result =
      ((((((      local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00000D64) >> (24 + FractionalResolution)))   // 0.0002043732656744
            * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00005E03) >> (24 + FractionalResolution))))  // 0.0014345483118123
            * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000221DB) >> (24 + FractionalResolution))))  // 0.0083291093882400
            * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000AA908) >> (24 + FractionalResolution))))  // 0.0416417643582255
            * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x002AAABD) >> (24 + FractionalResolution))))  // 0.1666677606703556
            * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0080003F) >> (24 + FractionalResolution))))  // 0.5000038123608412
            * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00FFFFFE) >> (24 + FractionalResolution))))  // 0.9999999247233815
            * x;

    ++result;

    if(nf > 0)
    {
      result.data <<= nf;
    }

    return result;
  }

  /*! Computes exponent for fractional resolution > 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> exp(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Handle zero argument.
    if(x.data == 0)
    {
      return local_negatable_type(1U);
    }

    // Handle reflection for negative arguments.
    if(x.data < 0)
    {
      return 1 / exp(-x);
    }

    int nf = 0;

    if(x > local_negatable_type::value_ln_two())
    {
      nf = int(x / local_negatable_type::value_ln_two());

      x -= (local_negatable_type::value_ln_two() * nf);
    }

    local_negatable_type result = detail::hypergeometric_0f0(x);

    if(nf > 0)
    {
      result.data <<= nf;
    }

    return result;
  }

  /*! Computes natural log for fractional resolution < 11 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(11) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    local_negatable_type result;

    if(x < 1)
    {
      result = ((x.data > 0) ? -log(1 / x) : -local_negatable_type::value_max());
    }
    else if(x > 1)
    {
      int n = 0;

      if(x > 2)
      {
        // Use a binary-halving mechanism to obtain the most significant bit.
        // This will subsequently be used for argument reduction below.

        BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t unsigned_small_digits =
          static_cast<boost::uint_fast16_t>(std::numeric_limits<local_unsigned_small_type>::digits);

        local_unsigned_small_type unsigned_small_mask((std::numeric_limits<local_unsigned_small_type>::max)());

        local_unsigned_small_type tmp = static_cast<local_unsigned_small_type>(x.data);

        const boost::uint_fast16_t msb = detail::msb_helper(tmp, unsigned_small_mask, unsigned_small_digits);

        // Evaluate the necessary amount of right-shift.
        n = int(msb) - local_negatable_type::radix_split;

        x.data = local_value_type(local_unsigned_small_type(x.data) >> n);
      }

      // Use a polynomial approximation of the base-2 logarithm.
      // log2(x + 1) = approx. + 1.4385022043619629 x
      //                       - 0.6777697968275845 x^2
      //                       + 0.3230078286559936 x^3
      //                       - 0.0839120707923452 x^4,
      // in the range 0 <= x <= 1. These coefficients
      // have been specifically derived for this work.

      // Note that the result of the polynomial aproximation
      // is a base-2 logarithm.

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      const local_negatable_type z = (x - 1);

      const local_negatable_type polynomial_approximation =
        (((    - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x00AB) >> (11 + FractionalResolution)))   // 0.0839120707923452
           * z + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0295) >> (11 + FractionalResolution))))  // 0.3230078286559936
           * z - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x056C) >> (11 + FractionalResolution))))  // 0.6777697968275845
           * z + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0B82) >> (11 + FractionalResolution))))  // 1.4385022043619629
           * z;

      // Scale the result to a base-e logarithm.
      const local_negatable_type log_value = polynomial_approximation * local_negatable_type::value_ln_two();

      // Scale with the logarithms of the powers of 2 if necessary.
      result = ((n == 0) ? log_value : (log_value + (n * local_negatable_type::value_ln_two())));
    }
    else
    {
      // The argument is exactly equal to 1.
      result = local_negatable_type(0U);
    }

    return result;
  }

  /*! Computes natural log for fractional resolution < 24 bits. */

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<(int(24) >= (-FractionalResolution)) && (int(11) < (-FractionalResolution))>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    local_negatable_type result;

    if(x < 1)
    {
      result = ((x.data > 0) ? -log(1 / x) : -local_negatable_type::value_max());
    }
    else if(x > 1)
    {
      int n = 0;

      if(x > 2)
      {
        // Use a binary-halving mechanism to obtain the most significant bit.
        // This will subsequently be used for argument reduction below.

        BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t unsigned_small_digits =
          static_cast<boost::uint_fast16_t>(std::numeric_limits<local_unsigned_small_type>::digits);

        local_unsigned_small_type unsigned_small_mask((std::numeric_limits<local_unsigned_small_type>::max)());

        local_unsigned_small_type tmp = static_cast<local_unsigned_small_type>(x.data);

        const boost::uint_fast16_t msb = detail::msb_helper(tmp, unsigned_small_mask, unsigned_small_digits);

        // Evaluate the necessary amount of right-shift.
        n = int(msb) - local_negatable_type::radix_split;

        x.data = local_value_type(local_unsigned_small_type(x.data) >> n);
      }

      // Use a polynomial approximation of the base-2 logarithm.
      // log2(x + 1) = approx. + 1.44265859709491 x
      //                       - 0.72044516000712 x^2
      //                       + 0.47280304531019 x^3
      //                       - 0.32400619521798 x^4
      //                       + 0.19183861358090 x^5
      //                       - 0.07798258678209 x^6
      //                       + 0.01513421407398 x^7,
      // in the range 0 <= x <= 1. These coefficients
      // have been specifically derived for this work.

      // Note that the result of the polynomial aproximation
      // is a base-2 logarithm.

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      const local_negatable_type z = (x - 1);

      const local_negatable_type polynomial_approximation =
        ((((((      local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0003DFD5) >> (24 + FractionalResolution)))   // 0.01513421407398
              * z - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0013F6AA) >> (24 + FractionalResolution))))  // 0.07798258678209
              * z + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00311C55) >> (24 + FractionalResolution))))  // 0.19183861358090
              * z - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0052F211) >> (24 + FractionalResolution))))  // 0.32400619521798
              * z + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0079099E) >> (24 + FractionalResolution))))  // 0.47280304531019
              * z - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00B86F18) >> (24 + FractionalResolution))))  // 0.72044516000712
              * z + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x01715212) >> (24 + FractionalResolution))))  // 1.44265859709491
              * z;

      // Scale the result to a base-e logarithm.
      const local_negatable_type log_value = polynomial_approximation * local_negatable_type::value_ln_two();

      // Scale with the logarithms of the powers of 2 if necessary.
      result = ((n == 0) ? log_value : (log_value + (n * local_negatable_type::value_ln_two())));
    }
    else
    {
      // The argument is exactly equal to 1.
      result = local_negatable_type(0U);
    }

    return result;
  }

  /*! Computes natural log for fractional resolution > 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;

    local_negatable_type result;

    if(x < 1)
    {
      result = ((x.data > 0) ? -log(1 / x) : -local_negatable_type::value_max());
    }
    else if(x > 1)
    {
      int n = 0;

      if(x > 2)
      {
        // Use a binary-halving mechanism to obtain the most significant bit.
        // This will subsequently be used for argument reduction below.

        BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t unsigned_small_digits =
          static_cast<boost::uint_fast16_t>(std::numeric_limits<local_unsigned_small_type>::digits);

        local_unsigned_small_type unsigned_small_mask((std::numeric_limits<local_unsigned_small_type>::max)());

        local_unsigned_small_type tmp = static_cast<local_unsigned_small_type>(x.data);

        const boost::uint_fast16_t msb = detail::msb_helper(tmp, unsigned_small_mask, unsigned_small_digits);

        // Evaluate the necessary amount of right-shift.
        n = int(msb) - local_negatable_type::radix_split;

        x.data = local_value_type(local_unsigned_small_type(x.data) >> n);
      }
      else
      {
        n = 0;
      }

      local_negatable_type log_val;

      {
        // Obtain an initial estimate of the log(x) using
        // two terms in the Taylor series.
        const local_negatable_type z = x - 1;

        log_val = z * (1 - (z / 2));
      }

      // Use Newton-Raphson iteration to compute the log(x).
      for(boost::uint_fast16_t i = UINT16_C(1); i <= boost::uint_fast16_t(local_negatable_type::all_bits / 2); i *= UINT16_C(2))
      {
        const local_negatable_type exp_minus_log = detail::hypergeometric_0f0(-log_val);

        log_val += ((x * exp_minus_log) - 1);
      }

      // Obtain the result and scale it with the logarithms
      // of the factors of 2 (if necessary).
      result = ((n == 0) ? log_val : (log_val + (n * local_negatable_type::value_ln_two())));
    }
    else
    {
      // The argument is exactly equal to 1.
      result = local_negatable_type(0U);
    }

    return result;
  }

  /*! Computes log to base 2. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log2(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    return log(x) / local_negatable_type::value_ln_two();
  }

  /*! Computes log to base 10. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log10(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Consider warm-caching log(10) as a constant value.
    return log(x) / log(local_negatable_type(10));
  }

  /*! Computes power x ^ a. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> pow(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> a)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x.data == 0)
    {
      return local_negatable_type(0U);
    }
    else if(a.data == 0)
    {
      return local_negatable_type(1U);
    }
    else
    {
      return exp(a * log(x));
    }
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> pow(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, int n)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(n == 0)
    {
      return local_negatable_type(1U);
    }
    else if(n < 0)
    {
      return 1 / pow(x, -n);
    }
    else
    {
      // The variable xn stores the binary powers of x.
      local_negatable_type result(((n % 2) != 0) ? x : local_negatable_type(1U));

      while((n /= 2) != 0)
      {
        // Square x for each binary power.
        x *= x;

        const bool has_binary_power = ((n % 2) != 0);

        if(has_binary_power)
        {
          // Multiply the result with each binary power contained in the exponent.
          result *= x;
        }
      }

      return result;
    }
  }

  /*! Computes sine function. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(11) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle reflection for negative arguments.
    if(x.data < 0)
    {
      return -sin(-x);
    }

    int n = 0;

    // Reduce the argument to the range 0 <= x <= +pi/2.
    if(x > local_negatable_type::value_pi())
    {
      n = int(x / local_negatable_type::value_pi());

      x -= (n * local_negatable_type::value_pi());
    }

    local_negatable_type result;

    if(x > local_negatable_type::value_pi_half())
    {
      result = sin(local_negatable_type::value_pi() - x);
    }
    else
    {
      if(x == local_negatable_type::value_pi_half())
      {
        result = local_negatable_type(1U);
      }
      else
      {
        // Use a polynomial approximation.
        // sin(x) = approx. + 0.99976073735983227 x
        //                  - 0.16580121984779175 x^3
        //                  + 0.00756279111686865 x^5
        // in the range -pi/2 <= x <= +pi/2. These coefficients
        // have been specifically derived for this work.

        const local_negatable_type x2 = (x * x);

        // Perform the polynomial approximation using a coefficient
        // expansion via the method of Horner.
        result = ((       local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x000F) >> (11 + FractionalResolution)))   // 0.00756279111686865
                   * x2 - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0153) >> (11 + FractionalResolution))))  // 0.16580121984779175
                   * x2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x07FF) >> (11 + FractionalResolution))))  // 0.99976073735983227
                   * x;
      }
    }

    return (((n % 2) == 0) ? result : -result);
  }

  /*! Computes sine function for fractional resolution < 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<(int(24) >= (-FractionalResolution)) && (int(11) < (-FractionalResolution))>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle reflection for negative arguments.
    if(x.data < 0)
    {
      return -sin(-x);
    }

    int n = 0;

    // Reduce the argument to the range 0 <= x <= +pi/2.
    if(x > local_negatable_type::value_pi())
    {
      n = int(x / local_negatable_type::value_pi());

      x -= (n * local_negatable_type::value_pi());
    }

    local_negatable_type result;

    if(x > local_negatable_type::value_pi_half())
    {
      result = sin(local_negatable_type::value_pi() - x);
    }
    else
    {
      if(x == local_negatable_type::value_pi_half())
      {
        result = local_negatable_type(1U);
      }
      else
      {
        // Use a polynomial approximation.
        // sin(x) = approx. + 0.9999999815561047 x
        //                  - 0.1666665066192447 x^3
        //                  + 0.0083329484215251 x^5
        //                  - 0.0001980373563403 x^7
        //                  + 0.0000025959374407 x^9,
        // in the range -pi/2 <= x <= +pi/2. These coefficients
        // have been specifically derived for this work.

        const local_negatable_type x2 = (x * x);

        // Perform the polynomial approximation using a coefficient
        // expansion via the method of Horner.
        result = ((((       local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0000002B) >> (24 + FractionalResolution)))   // 0.9999999815561047
                     * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00000CFA) >> (24 + FractionalResolution))))  // 0.1666665066192447
                     * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0002221B) >> (24 + FractionalResolution))))  // 0.0083329484215251
                     * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x002AAAA7) >> (24 + FractionalResolution))))  // 0.0001980373563403
                     * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00FFFFFF) >> (24 + FractionalResolution))))  // 0.0000025959374407
                     * x;
      }
    }

    return (((n % 2) == 0) ? result : -result);
  }

  /*! Computes sine function for fractional resolution > 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Handle reflection for negative arguments.
    if(x.data < 0)
    {
      return -sin(-x);
    }

    int n = 0;

    // Reduce the argument to the range 0 <= x <= +pi/2.
    if(x > local_negatable_type::value_pi())
    {
      n = int(x / local_negatable_type::value_pi());

      x -= (n * local_negatable_type::value_pi());
    }

    local_negatable_type result;

    if(x > local_negatable_type::value_pi_half())
    {
      result = sin(local_negatable_type::value_pi() - x);
    }
    else
    {
      if(x <= ldexp(local_negatable_type::value_pi(), -2))
      {
        // Use the Taylor series representation of sin(x) near x = 0.
        local_negatable_type x_squared = (x * x);
        local_negatable_type term      = x;
        bool term_is_negative          = true;
        local_negatable_type sum       = term;

        BOOST_CONSTEXPR_OR_CONST boost::uint32_t maximum_number_of_iterations = UINT32_C(10000);

        for(boost::uint32_t k = UINT32_C(1); k < maximum_number_of_iterations; k += UINT32_C(2))
        {
          term *= x_squared;

          term /= (k + 1U);
          term /= (k + 2U);

          const bool minimum_number_of_iterations_is_complete = (k > UINT32_C(5));

          if(   minimum_number_of_iterations_is_complete
             && (term <= std::numeric_limits<local_negatable_type>::epsilon()))
          {
            break;
          }

          ((!term_is_negative) ? sum += term : sum -= term);

          term_is_negative = (!term_is_negative);
        }

        result = sum;
      }
      else
      {
        // Use an angle-doubling identity to reduce the argument to less than +pi/4.
        const local_negatable_type half_x = x / 2;

        const local_negatable_type sin_half_x = sin(half_x);

        result = (2 * (sin(half_x) * cos(half_x)));
      }
    }

    return (((n % 2) == 0) ? result : -result);
  }

  /*! Computes cosine function for fractional resolution < 11 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(11) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle reflection for negative arguments.
    if(x.data < 0)
    {
      x = -x;
    }

    // Reduce the argument to the range 0 <= x <= +pi/2.
    int n = 0;

    if(x >= local_negatable_type::value_pi())
    {
      n = int(x / local_negatable_type::value_pi());

      x -= (n * local_negatable_type::value_pi());
    }

    local_negatable_type result;

    const local_negatable_type delta = local_negatable_type::value_pi_half() - x;

    if(fabs(delta) < ldexp(local_negatable_type(1U), -2))
    {
      const local_negatable_type delta2 = (delta * delta);

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      result = ((         + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0011) >> (11 + FractionalResolution)))   // 0.9999999815561047
                 * delta2 - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0155) >> (11 + FractionalResolution))))  // 0.1666665066192447
                 * delta2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0800) >> (11 + FractionalResolution))))  // 0.1666665066192447
                 * delta;
    }
    else if(x > local_negatable_type::value_pi_half())
    {
      if(x == local_negatable_type::value_pi())
      {
        result = local_negatable_type(-1);
      }
      else
      {
        result = -cos(local_negatable_type::value_pi() - x);
      }
    }
    else
    {
      if(x.data == 0)
      {
        result = local_negatable_type(1U);
      }
      else if(x == local_negatable_type::value_pi_half())
      {
        result = local_negatable_type(0U);
      }
      else
      {
        // Use a polynomial approximation.
        // cos(x) = approx. + 0.99999499214619638
        //                  - 0.49992775493529621 x^2
        //                  + 0.04150705495821635 x^4
        //                  - 0.00127707709747365 x^6
        // in the range -pi/2 <= x <= +pi/2. These coefficients
        // have been specifically derived for this work.

        const local_negatable_type x2 = (x * x);

        // Perform the polynomial approximation using a coefficient
        // expansion via the method of Horner.
        result = (((     - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0002) >> (11 + FractionalResolution)))    // 0.99999499214619638
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0055) >> (11 + FractionalResolution))))   // 0.49992775493529621
                    * x2 - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x03FF) >> (11 + FractionalResolution))))   // 0.04150705495821635
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x07FF) >> (11 + FractionalResolution))));  // 0.00127707709747365
      }
    }

    return (((n % 2) == 0) ? result : -result);
  }

  /*! Computes cosine function for fractional resolution < 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<(int(24) >= (-FractionalResolution)) && (int(11) < (-FractionalResolution))>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle reflection for negative arguments.
    if(x.data < 0)
    {
      x = -x;
    }

    // Reduce the argument to the range 0 <= x <= +pi/2.
    int n = 0;

    if(x >= local_negatable_type::value_pi())
    {
      n = int(x / local_negatable_type::value_pi());

      x -= (n * local_negatable_type::value_pi());
    }

    local_negatable_type result;

    const local_negatable_type delta = local_negatable_type::value_pi_half() - x;

    if(fabs(delta) < ldexp(local_negatable_type(1U), -2))
    {
      const local_negatable_type delta2 = (delta * delta);

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      result = (((         - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00000D00) >> (24 + FractionalResolution)))   // 0.9999999815561047
                  * delta2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00022222) >> (24 + FractionalResolution))))  // 0.1666665066192447
                  * delta2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x002AAAAA) >> (24 + FractionalResolution))))  // 0.1666665066192447
                  * delta2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x01000000) >> (24 + FractionalResolution))))  // 0.0000025959374407
                  * delta;
    }
    else if(x > local_negatable_type::value_pi_half())
    {
      if(x == local_negatable_type::value_pi())
      {
        result = local_negatable_type(-1);
      }
      else
      {
        result = -cos(local_negatable_type::value_pi() - x);
      }
    }
    else
    {
      if(x.data == 0)
      {
        result = local_negatable_type(1U);
      }
      else if(x == local_negatable_type::value_pi_half())
      {
        result = local_negatable_type(0U);
      }
      else
      {
        // Use a polynomial approximation.
        // cos(x) = approx. + 0.9999999998329485
        //                  - 0.4999999947816307 x^2
        //                  + 0.0416666405328244 x^4
        //                  - 0.0013888413877237 x^6
        //                  + 0.0000247627549571 x^8
        //                  - 0.0000002609587957 x^10
        // in the range -pi/2 <= x <= +pi/2. These coefficients
        // have been specifically derived for this work.

        const local_negatable_type x2 = (x * x);

        // Perform the polynomial approximation using a coefficient
        // expansion via the method of Horner.
        result = (((((     - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00000003) >> (24 + FractionalResolution)))    // 0.00000022171564942
                      * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00000196) >> (24 + FractionalResolution))))   // 0.00002424851890444
                      * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00005AD9) >> (24 + FractionalResolution))))   // 0.00138624146943166
                      * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000AAA4a) >> (24 + FractionalResolution))))   // 0.04166094016112324
                      * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x007FFFB4) >> (24 + FractionalResolution))))   // 0.49999549949242657
                      * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00FFFFF6) >> (24 + FractionalResolution))));  // 0.99999943833186084
      }
    }

    return (((n % 2) == 0) ? result : -result);
  }

  /*! Computes cosine function for fractional resolution > 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Handle reflection for negative arguments.
    if(x.data < 0)
    {
      x = -x;
    }

    // Reduce the argument to the range 0 <= x <= +pi/2.
    int n = 0;

    if(x > local_negatable_type::value_pi())
    {
      n = int(x / local_negatable_type::value_pi());

      x -= (n * local_negatable_type::value_pi());
    }

    local_negatable_type result;

    if(x > local_negatable_type::value_pi_half())
    {
      result = -cos(local_negatable_type::value_pi() - x);
    }
    else
    {
      if(x <= ldexp(local_negatable_type::value_pi(), -2))
      {
        // Use the Taylor series representation of cos(x) near x = 0.
        local_negatable_type x_squared = (x * x);
        local_negatable_type term      = x_squared / 2;
        bool term_is_negative          = false;
        local_negatable_type sum       = 1 - term;

        BOOST_CONSTEXPR_OR_CONST boost::uint32_t maximum_number_of_iterations = UINT32_C(10000);

        for(boost::uint32_t k = UINT32_C(2); k < maximum_number_of_iterations; k += UINT32_C(2))
        {
          term *= x_squared;

          term /= (k + 1U);
          term /= (k + 2U);

          const bool minimum_number_of_iterations_is_complete = (k > UINT32_C(6));

          if(   (minimum_number_of_iterations_is_complete)
             && (term <= std::numeric_limits<local_negatable_type>::epsilon()))
          {
            break;
          }

          ((!term_is_negative) ? sum += term : sum -= term);

          term_is_negative = (!term_is_negative);
        }

        result = sum;
      }
      else
      {
        // Use an angle-doubling identity to reduce the argument to less than +pi/4.
        const local_negatable_type half_x = x / 2;

        const local_negatable_type cos_half_x = cos(half_x);

        result = (2 * (cos_half_x * cos_half_x)) - 1;
      }
    }

    return (((n % 2) == 0) ? result : -result);
  }

  /*! Computes tangent function for fractional resolution < 11 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> tan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(11) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle negative arguments, zero argument and argument pi/2.
    if(x.data < 0)
    {
      return -tan(-x);
    }
    else if(x.data == 0)
    {
      return local_negatable_type(0U);
    }
    else if(x == local_negatable_type::value_pi_half())
    {
      return local_negatable_type::value_max();
    }

    // Reduce the argument to the range 0 <= x <= +pi/2.
    int n = 0;

    if(x > local_negatable_type::value_pi())
    {
      n = int(x / local_negatable_type::value_pi());

      x -= (n * local_negatable_type::value_pi());
    }

    local_negatable_type result;

    if(x > local_negatable_type::value_pi_half())
    {
      result = -tan(local_negatable_type::value_pi() - x);
    }
    else
    {
      const local_negatable_type pi_over_four = local_negatable_type::value_pi() / 4;

      if(x > pi_over_four)
      {
        const local_negatable_type tan_value_shifted = tan(x - pi_over_four);

        result = (1 + tan_value_shifted) / (1 - tan_value_shifted);
      }
      else if(x < pi_over_four)
      {
        // Use a polynomial approximation.
        // tan(x) / x = approx. + 0.999966509199007
        //                      + 0.335162805277695 x^2
        //                      + 0.118177026215436 x^4
        //                      + 0.091380425311140 x^6,
        // in the range 0 <= x <= +pi/4. These coefficients
        // have been specifically derived for this work.

        const local_negatable_type x2 = (x * x);

        // Perform the polynomial approximation using a coefficient
        // expansion via the method of Horner.
        result = (((       local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x00BB) >> (11 + FractionalResolution)))   // 0.01987397353189
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x00F2) >> (11 + FractionalResolution))))  // 0.01326787053456
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x02AE) >> (11 + FractionalResolution))))  // 0.33335465351938
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x07FF) >> (11 + FractionalResolution))))  // 0.99999982053522
                    * x;
      }
      else
      {
        // The argument is exactly +pi/4.
        result = local_negatable_type(1U);
      }
    }

    return result;
  }

  /*! Computes tangent function for fractional resolution < 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> tan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<(int(24) >= (-FractionalResolution)) && (int(11) < (-FractionalResolution))>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle negative arguments, zero argument and argument pi/2.
    if(x.data < 0)
    {
      return -tan(-x);
    }
    else if(x.data == 0)
    {
      return local_negatable_type(0U);
    }
    else if(x == local_negatable_type::value_pi_half())
    {
      return local_negatable_type::value_max();
    }

    // Reduce the argument to the range 0 <= x <= +pi/2.
    int n = 0;

    if(x > local_negatable_type::value_pi())
    {
      n = int(x / local_negatable_type::value_pi());

      x -= (n * local_negatable_type::value_pi());
    }

    local_negatable_type result;

    if(x > local_negatable_type::value_pi_half())
    {
      result = -tan(local_negatable_type::value_pi() - x);
    }
    else
    {
      const local_negatable_type pi_over_four = local_negatable_type::value_pi() / 4;

      if(x > pi_over_four)
      {
        const local_negatable_type tan_value_shifted = tan(x - pi_over_four);

        result = (1 + tan_value_shifted) / (1 - tan_value_shifted);
      }
      else if(x < pi_over_four)
      {
        // Use a polynomial approximation.
        // tan(x) / x = approx. + 0.99999982053522
        //                      + 0.33335465351938 x^2
        //                      + 0.13292151590120 x^4
        //                      + 0.05681862499001 x^6
        //                      + 0.01326787053456 x^8
        //                      + 0.01987397353189 x^10,
        // in the range 0 <= x <= +pi/4. These coefficients
        // have been specifically derived for this work.

        const local_negatable_type x2 = (x * x);

        // Perform the polynomial approximation using a coefficient
        // expansion via the method of Horner.
        result = (((((       local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00051675) >> (24 + FractionalResolution)))   // 0.01987397353189
                      * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00036585) >> (24 + FractionalResolution))))  // 0.01326787053456
                      * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000E8BAA) >> (24 + FractionalResolution))))  // 0.05681862499001
                      * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00220724) >> (24 + FractionalResolution))))  // 0.13292151590120
                      * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x005556BB) >> (24 + FractionalResolution))))  // 0.33335465351938
                      * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00FFFFFC) >> (24 + FractionalResolution))))  // 0.99999982053522
                      * x;
      }
      else
      {
        // The argument is exactly +pi/4.
        result = local_negatable_type(1U);
      }
    }

    return result;
  }

  /*! Computes tangent function for fractional resolution > 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> tan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Handle negative arguments, zero argument and argument pi/2.
    if(x.data < 0)
    {
      return -tan(-x);
    }
    else if(x.data == 0)
    {
      return local_negatable_type(0U);
    }
    else if(x == local_negatable_type::value_pi_half())
    {
      return local_negatable_type::value_max();
    }

    // Use a relatively lazy calculation for tan(x) here.
    // Since there is no support for the potentially supported
    // function sincos(negatable), we simply use the result
    // of the division tan(x) = [sin(x) / cos(x)] for tangent.
    // Here, sin(x) and cos(x) are computed separately.

    return sin(x) / cos(x);
  }

  /*! Computes arc sine function for fractional resolution < 11 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> asin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(11) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x.data < 0)
    {
      return -asin(-x);
    }

    local_negatable_type result;

    if(x.data == 0)
    {
      result = local_negatable_type(0U);
    }
    else if(x == 1)
    {
      result = local_negatable_type::value_pi_half();
    }
    else if(x > 1)
    {
      result = local_negatable_type(0U);
    }
    else
    {
      if(x < ldexp(local_negatable_type(1U), -2))
      {
        const local_negatable_type x2 = (x * x);

        // Use the Taylor series representation for small argument.
        // asin(x) = approx. x + (x^3)/6 + 3(x^5)/40 + 5(x^7)/112

        result = (((     + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x005B) >> (11 + FractionalResolution)))
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0099) >> (11 + FractionalResolution))))
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0155) >> (11 + FractionalResolution))))
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0800) >> (11 + FractionalResolution))))
                    * x;
      }
      else
      {
        // Use a polynomial approximation.
        // asin(x) = approx. pi/2 - [sqrt(1 - x) * (+ 1.5707288
        //                                          - 0.2121144 x^1
        //                                          + 0.0742610 x^2
        //                                          - 0.0187293 x^3],
        // in the range 0 <= x <= +1. These coefficients
        // have been taken from Eq. 4.4.45 in Abramowitz & Stegun,
        // "Handbook of Mathematical Functions", 0009-Revised edition
        // (Dover Publications, 1965).
        // The coefficients originate from C. Hastings, "Approximations
        // for Digital Computers" (Princeton Univ. Press, 1955).

        // Perform the polynomial approximation using a coefficient
        // expansion via the method of Horner.
        const local_negatable_type polynomial_approximation =
          (((    - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0026) >> (11 + FractionalResolution)))   // 0.0187293
             * x + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0098) >> (11 + FractionalResolution))))  // 0.0742610
             * x - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x01B2) >> (11 + FractionalResolution))))  // 0.2121144
             * x + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0C90) >> (11 + FractionalResolution)))); // 1.5707288

        result = local_negatable_type::value_pi_half() - (sqrt(1 - x) * polynomial_approximation);
      }
    }

    return result;
  }

  /*! Computes arc sine function for fractional resolution < 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> asin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<(int(24) >= (-FractionalResolution)) && (int(11) < (-FractionalResolution))>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x.data < 0)
    {
      return -asin(-x);
    }

    local_negatable_type result;

    if(x.data == 0)
    {
      result = local_negatable_type(0U);
    }
    else if(x == 1)
    {
      result = local_negatable_type::value_pi_half();
    }
    else if(x > 1)
    {
      result = local_negatable_type(0U);
    }
    else
    {
      // Use a polynomial approximation.
      // asin(x) = approx. pi/2 - [sqrt(1 - x) * (+ 1.5707962797298
      //                                          - 0.2145976333738 x^1
      //                                          + 0.0889661326639 x^2
      //                                          - 0.0501164255199 x^3
      //                                          + 0.0307646026592 x^4
      //                                          - 0.0169450735826 x^5
      //                                          + 0.0065926664377 x^6
      //                                          - 0.0012470498685 x^7)],
      // in the range 0 <= x <= +1. These coefficients
      // have been specifically derived for this work.

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      const local_negatable_type polynomial_approximation =
        (((((((    - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000051BA) >> (24 + FractionalResolution)))   // 0.0012470498685
               * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0001B00E) >> (24 + FractionalResolution))))  // 0.0065926664377
               * x - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00045683) >> (24 + FractionalResolution))))  // 0.0169450735826
               * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0007E030) >> (24 + FractionalResolution))))  // 0.0307646026592
               * x - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000CD46E) >> (24 + FractionalResolution))))  // 0.0501164255199
               * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0016C67C) >> (24 + FractionalResolution))))  // 0.0889661326639
               * x - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0036EFDE) >> (24 + FractionalResolution))))  // 0.2145976333738
               * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x01921FB4) >> (24 + FractionalResolution)))); // 1.5707962797298

      result = local_negatable_type::value_pi_half() - (sqrt(1 - x) * polynomial_approximation);
    }

    return result;
  }

  /*! Computes arc sine function for fractional resolution > 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> asin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x.data < 0)
    {
      return -asin(-x);
    }

    local_negatable_type result;

    if(x.data == 0)
    {
      result = local_negatable_type(0U);
    }
    else if(x == 1)
    {
      result = local_negatable_type::value_pi_half();
    }
    else if(x > 1)
    {
      result = local_negatable_type(0U);
    }
    else
    {
      const local_negatable_type one_half     = ldexp(local_negatable_type(1U), -1);
      const local_negatable_type three_halves = ldexp(local_negatable_type(3U), -1);

      if(x < one_half)
      {
        result = x * detail::hypergeometric_2f1(one_half, one_half, three_halves, (x * x));
      }
      else
      {
        const local_negatable_type delta = (1 - x);

        result =   local_negatable_type::value_pi_half()
                 - (  sqrt(delta * 2)
                    * detail::hypergeometric_2f1(one_half, one_half, three_halves, delta / 2));
      }
    }

    return result;
  }

  /*! Computes arc cosine function for fractional resolution < 11 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> acos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(11) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x.data < 0)
    {
      return local_negatable_type::value_pi() - acos(-x);
    }

    local_negatable_type result;

    if(x.data == 0)
    {
      result = local_negatable_type::value_pi_half();
    }
    else if(x == 1)
    {
      result = local_negatable_type(0U);
    }
    else if(x > 1)
    {
      result = local_negatable_type(0U);
    }
    else
    {
      // Use a polynomial approximation.
      // acos(x) = approx. sqrt(1 - x) * (+ 1.5707962797298
      //                                  - 0.2145976333738 x^1
      //                                  + 0.0889661326639 x^2
      //                                  - 0.0501164255199 x^3)],
      // in the range 0 <= x <= +1. These coefficients
      // have been taken from Eq. 4.4.45 in Abramowitz & Stegun,
      // "Handbook of Mathematical Functions", 0009-Revised edition
      // (Dover Publications, 1965).
      // The coefficients originate from C. Hastings, "Approximations
      // for Digital Computers" (Princeton Univ. Press, 1955).

      // These are the same coefficients that are used
      // for asin(x) above.

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      const local_negatable_type polynomial_approximation =
        (((    - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0026) >> (11 + FractionalResolution)))   // 0.0187293
           * x + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0098) >> (11 + FractionalResolution))))  // 0.0742610
           * x - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x01B2) >> (11 + FractionalResolution))))  // 0.2121144
           * x + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0C90) >> (11 + FractionalResolution)))); // 1.5707288

      result = (sqrt(1 - x) * polynomial_approximation);
    }

    return result;
  }

  /*! Computes arc cosine function for fractional resolution < 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> acos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<(int(24) >= (-FractionalResolution)) && (int(11) < (-FractionalResolution))>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x.data < 0)
    {
      return local_negatable_type::value_pi() - acos(-x);
    }

    local_negatable_type result;

    if(x.data == 0)
    {
      result = local_negatable_type::value_pi_half();
    }
    else if(x == 1)
    {
      result = local_negatable_type(0U);
    }
    else if(x > 1)
    {
      result = local_negatable_type(0U);
    }
    else
    {
      // Use a polynomial approximation.
      // acos(x) = approx. [sqrt(1 - x) * (+ 1.5707962797298
      //                                   - 0.2145976333738 x^1
      //                                   + 0.0889661326639 x^2
      //                                   - 0.0501164255199 x^3
      //                                   + 0.0307646026592 x^4
      //                                   - 0.0169450735826 x^5
      //                                   + 0.0065926664377 x^6
      //                                   - 0.0012470498685 x^7)],
      // in the range 0 <= x <= +1. These coefficients
      // have been specifically derived for this work.
      // These are the same coefficients that are used
      // for asin(x) above.

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      const local_negatable_type polynomial_approximation =
        (((((((    - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000051BA) >> (24 + FractionalResolution)))   // 0.0012470498685
               * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0001B00E) >> (24 + FractionalResolution))))  // 0.0065926664377
               * x - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00045683) >> (24 + FractionalResolution))))  // 0.0169450735826
               * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0007E030) >> (24 + FractionalResolution))))  // 0.0307646026592
               * x - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000CD46E) >> (24 + FractionalResolution))))  // 0.0501164255199
               * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0016C67C) >> (24 + FractionalResolution))))  // 0.0889661326639
               * x - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0036EFDE) >> (24 + FractionalResolution))))  // 0.2145976333738
               * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x01921FB4) >> (24 + FractionalResolution)))); // 1.5707962797298

      result = (sqrt(1 - x) * polynomial_approximation);
    }

    return result;
  }

  /*! Computes arc cosine function for fractional resolution > 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> acos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x.data < 0)
    {
      return local_negatable_type::value_pi() - acos(-x);
    }

    local_negatable_type result;

    if(x.data == 0)
    {
      result = local_negatable_type::value_pi_half();
    }
    else if(x == 1)
    {
      result = local_negatable_type(0U);
    }
    else if(x > 1)
    {
      result = local_negatable_type(0U);
    }
    else
    {
      result = local_negatable_type::value_pi_half() - asin(x);
    }

    return result;
  }

  /*! Computes arc or inverse tangent function atan.
  \sa http://en.cppreference.com/w/cpp/numeric/math/atan
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(11) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x.data < 0)
    {
      return -atan(-x);
    }

    local_negatable_type result;

    if(x.data == 0)
    {
      result = local_negatable_type(0U);
    }
    else if(x == local_negatable_type(1U))
    {
      result = ldexp(local_negatable_type::value_pi(), -2);
    }
    else if(x > local_negatable_type(1U))
    {
      result = local_negatable_type::value_pi_half() - atan(1 / x);
    }
    else
    {
      // Use a polynomial approximation.
      // atan(x) / x = approx. + 0.9998329769337240
      //                       - 0.3268824051434949 x^2
      //                       + 0.1590294514698240 x^4
      //                       - 0.0469818803609288 x^6,
      // in the range 0 <= x <= 1. These coefficients
      // have been specifically derived for this work.
      const local_negatable_type x2 = (x * x);

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      result = (((     - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0060) >> (11 + FractionalResolution)))   // 0.014236215058440372
                  * x2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x0145) >> (11 + FractionalResolution))))  // 0.059227914090755230
                  * x2 - local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x029D) >> (11 + FractionalResolution))))  // 0.122639496662731095
                  * x2 + local_negatable_type(local_nothing(), local_value_type(UINT16_C(0x07FF) >> (11 + FractionalResolution))))  // 0.999996396967490826
                  * x;
    }

    return result;
  }

  /*! Computes arc tangent function for fractional resolution < 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<(int(24) >= (-FractionalResolution)) && (int(11) < (-FractionalResolution))>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x.data < 0)
    {
      return -atan(-x);
    }

    local_negatable_type result;

    if(x.data == 0)
    {
      result = local_negatable_type(0U);
    }
    else if(x == local_negatable_type(1U))
    {
      result = ldexp(local_negatable_type::value_pi(), -2);
    }
    else if(x > local_negatable_type(1U))
    {
      result = local_negatable_type::value_pi_half() - atan(1 / x);
    }
    else
    {
      // Use a polynomial approximation.
      // atan(x) / x = approx. + 0.999996396967490826
      //                       - 0.333043397362190827 x^2
      //                       + 0.196084683947839669 x^4
      //                       - 0.122639496662731095 x^6
      //                       + 0.059227914090755230 x^8
      //                       - 0.014236215058440372 x^10,
      // in the range 0 <= x <= 1. These coefficients
      // have been specifically derived for this work.
      const local_negatable_type x2 = (x * x);

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      result = (((((     - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0003A4FC) >> (24 + FractionalResolution)))   // 0.014236215058440372
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000F298F) >> (24 + FractionalResolution))))  // 0.059227914090755230
                    * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x001F654D) >> (24 + FractionalResolution))))  // 0.122639496662731095
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0032329B) >> (24 + FractionalResolution))))  // 0.196084683947839669
                    * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00554255) >> (24 + FractionalResolution))))  // 0.333043397362190827
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00FFFFC3) >> (24 + FractionalResolution))))  // 0.999996396967490826
                    * x;
    }

    return result;
  }

  /*! Computes arc tangent function for fractional resolution > 24 bits. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x.data < 0)
    {
      return -atan(-x);
    }

    local_negatable_type result;

    if(x.data == 0)
    {
      result = local_negatable_type(0U);
    }
    else if(x == local_negatable_type(1U))
    {
      result = ldexp(local_negatable_type::value_pi(), -2);
    }
    else if(x > local_negatable_type(1U))
    {
      result = local_negatable_type::value_pi_half() - atan(1 / x);
    }
    else if(x < ldexp(local_negatable_type(1U), -3))
    {
      const local_negatable_type one_half     = ldexp(local_negatable_type(1U), -1);
      const local_negatable_type three_halves = ldexp(local_negatable_type(3U), -1);

      // Use a hypergeometric series expansion for small argument.
      result = x * detail::hypergeometric_2f1( local_negatable_type(1U),
                                               one_half,
                                               three_halves,
                                              -(x * x));
    }
    else
    {
      // Use Newton-Raphson iteration for atan.

      // Obtain an initial guess using a two-term Pade approximation
      // for atan(x). The coefficients for this Pade approximation
      // have been specifically derived for this work.
      result = (x * 3U) / (3U + (x * x));

      // Do the Newton-Raphson iteration. Start with four binary digits
      // of precision obtained from the initial guess above.
      for(boost::uint_fast16_t i = UINT16_C(4); i <= boost::uint_fast16_t(local_negatable_type::all_bits / 2); i *= UINT16_C(2))
      {
        const local_negatable_type c = cos(result);
        const local_negatable_type s = sin(result);

        result += (c * ((x * c) - s));
      }
    }

    return result;
  }

  /*! Computes the arc tangent of y/x using the signs of arguments to determine the correct quadrant.for any fractional resolution. 
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atan2(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> y, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Handle the negative arguments and zero arguments.
    const bool x_is_neg = (x.data < 0);

    if(y == 0)
    {
      return ((!x_is_neg) ? local_negatable_type(0U) : local_negatable_type::value_pi());
    }

    const bool y_is_neg = (y < 0);

    if(x.data == 0)
    {
      return ((!y_is_neg) ? local_negatable_type::value_pi_half() : -local_negatable_type::value_pi_half());
    }

    // Compute atan(y / x), thereby ignoring the sign of the arguments.
    const local_negatable_type atan_term(atan(y / x));

    // Determine the proper quadrant based on signs of x and y.
    return ((y_is_neg == x_is_neg) ? ((!x_is_neg) ? atan_term : (atan_term - local_negatable_type::value_pi()))
                                   : ((!x_is_neg) ? atan_term : (atan_term + local_negatable_type::value_pi())));
  }

  /*! Compute hyperbolic sine sinh. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sinh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    const local_negatable_type ep = exp(x);
    const local_negatable_type em = 1U / ep;

    return (ep - em) / 2U;
  }

  /*! Compute hyperbolic cosine cosh. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cosh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    const local_negatable_type ep = exp(x);
    const local_negatable_type em = 1U / ep;

    return (ep + em) / 2U;
  }

  /*! Compute hyperbolic tangent tanh. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> tanh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    const local_negatable_type ep = exp(x);
    const local_negatable_type em = 1 / ep;

    return (ep - em) / (ep + em);
  }

  /*! Compute inverse hyperbolic sine asinh. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> asinh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x.data < 0)
    {
      // Handle negative argument.
      return -asinh(-x);
    }

    local_negatable_type result;

    if(x.data == 0)
    {
      result = local_negatable_type(0U);
    }
    else
    {
      if(x < ldexp(local_negatable_type(1U), -3))
      {
        // Handle arguments greater than 0 but less than 1/8.
        // Use a hypergeometric series representation here.
        const local_negatable_type one_half     = ldexp(local_negatable_type(1U), -1);
        const local_negatable_type three_halves = ldexp(local_negatable_type(3U), -1);

        result = x * detail::hypergeometric_2f1(one_half, one_half, three_halves, -(x * x));
      }
      else
      {
        // Handle standard arguments greater than 0.
        result = log(x + sqrt((x * x) + 1));
      }
    }

    return result;
  }

  /*! Compute inverse hyperbolic cosine cosh. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> acosh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    local_negatable_type result;

    if(x <= 1)
    {
      // Handle arguments less than or equal to 1.
      result = local_negatable_type(0U);
    }
    else
    {
      // Handle arguments greater than 1 but less than (1 + 1/8).
      // Use a hypergeometric series representation here.
      const local_negatable_type x_minus_one = x - 1;

      if(x_minus_one < ldexp(local_negatable_type(1U), -3))
      {
        const local_negatable_type one_half     = ldexp(local_negatable_type(1U), -1);
        const local_negatable_type three_halves = ldexp(local_negatable_type(3U), -1);

        result =   sqrt(2 * x_minus_one)
                 * detail::hypergeometric_2f1(one_half, one_half, three_halves, -x_minus_one / 2);
      }
      else
      {
        // Handle standard arguments greater than 1.
        result = log(x + sqrt((x * x) - 1));
      }
    }

    return result;
  }

  /*! Compute inverse hyperbolic tan atanh. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atanh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x.data < 0)
    {
      // Handle negative argument.
      return -atanh(-x);
    }

    local_negatable_type result;

    if(x.data == 0)
    {
      // Handle arguments identically equal to 0.
      result = local_negatable_type(0U);
    }
    else if(x >= 1)
    {
      // Handle arguments greater than or equal to 1.
      result = local_negatable_type::value_max();
    }
    else
    {
      if(x < ldexp(local_negatable_type(1U), -3))
      {
        // Handle small arguments greater than 0 but less than 1/8.
        // Use a hypergeometric series representation here.
        const local_negatable_type one_half     = ldexp(local_negatable_type(1U), -1);
        const local_negatable_type three_halves = ldexp(local_negatable_type(3U), -1);

        result = x * detail::hypergeometric_2f1(local_negatable_type(1U), one_half, three_halves, (x * x));
      }
      else
      {
        // Handle standard arguments greater than 0.
        result = (log(1 + x) - log(1 - x)) / 2;
      }
    }

    return result;
  }

  /*! Compute hypotenuse function. */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> hypot(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> y)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    const local_negatable_type x2(x * x);
    const local_negatable_type y2(y * y);

    return sqrt(x2 + y2);
  }

  /*! copy sign function.
  \return fixed_point value with the magnitude of x and the sign of y. 
  \param x Magnitude.
  \param y Sign.
    \sa http://en.cppreference.com/w/cpp/numeric/math/copysign
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> copysign(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> y)
  {
    return (((x.data < 0) == (y.data < 0)) ? x : -x);
  }

  /*! Rounds the floating-point argument arg to an integer value in floating-point format, using the current rounding mode.
    \sa http://en.cppreference.com/w/cpp/numeric/math/nearbyint
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> nearbyint(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    const bool is_neg = (x.data < 0);

    local_unsigned_small_type u_round = ((!is_neg) ? local_unsigned_small_type( x.data)
                                                   : local_unsigned_small_type(-x.data));

    // Shift to the right. The amount of right shift is one bit less than
    // the fractional resolution. This reserves one bit for rounding.
    BOOST_CONSTEXPR int total_right_shift = (-FractionalResolution) - 1;

    u_round = detail::right_shift_helper(u_round, total_right_shift);

    const boost::int_fast8_t rounding_result = local_negatable_type::binary_round(u_round);

    // Round the result.
    local_unsigned_small_type result = local_unsigned_small_type(local_value_type(u_round) + rounding_result);

    // Shift the result back to the left.
    result = detail::left_shift_helper(result, -FractionalResolution);

    const local_unsigned_small_type integral_part_mask((local_unsigned_small_type(((std::numeric_limits<local_negatable_type>::max)()).data) >> (-FractionalResolution)) << (-FractionalResolution));

    // Mask out the fractional part (after rounding), leaving the nearby integer value.
    result &= local_value_type(integral_part_mask);

    // Return the result as a signed negatable object.
    return ((!is_neg) ? local_negatable_type(local_nothing(),  local_value_type(result))
                      : local_negatable_type(local_nothing(), -local_value_type(result)));
  }

  /*! Returns the next representable value of from in the direction.
    \sa http://en.cppreference.com/w/cpp/numeric/math/nextafter
    \note nextafter((std::numeric_limits<>::max)(), (std::numeric_limits<>::max)()) returns (std::numeric_limits<>::max)();
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> nextafter(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> y)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    static_assert(is_fixed_point<local_negatable_type>::value,
                  "Error: fixed_next can only be instantiated with a fixed-point negatable type.");

    local_negatable_type result(x);

    const local_negatable_type dx = ldexp(local_negatable_type(1), local_negatable_type::resolution);

    if(y > x)
    {
      if(x < (std::numeric_limits<local_negatable_type>::max)())
      {
        result += dx;
      }
    }
    else if(y < x)
    {
      if(x > std::numeric_limits<local_negatable_type>::lowest())
      {
        result -= dx;
      }
    }

    return result;
  }
  } } // namespace boost::fixed_point

#endif // FIXED_POINT_NEGATABLE_CMATH_2015_08_21_HPP_
