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
  \brief Implement most fixed-point negatable <cmath> functions.
*/

#ifndef FIXED_POINT_NEGATABLE_CMATH_2015_08_21_HPP_
  #define FIXED_POINT_NEGATABLE_CMATH_2015_08_21_HPP_

  namespace boost { namespace fixed_point {

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> abs(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    return ((x.data < 0) ? -x : x);
  }

  //! @c std::fabs function (identical to abs and provided for completeness).
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> fabs(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    return ((x.data < 0) ? -x : x);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> floor(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x.data < 0)
    {
      const local_unsigned_small_type u(-x.data);

      const local_unsigned_small_type u_mask(u & local_negatable_type::unsigned_integer_part_mask());

      return ((u == u_mask) ? x : (local_negatable_type(local_nothing(), -local_value_type(u_mask)) - 1));
    }
    else
    {
      const local_unsigned_small_type u_mask(local_unsigned_small_type(x.data) & local_negatable_type::unsigned_integer_part_mask());

      return local_negatable_type(local_nothing(), u_mask);
    }
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> ceil(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x.data < 0)
    {
      const local_unsigned_small_type u_mask(local_unsigned_small_type(-x.data) & local_negatable_type::unsigned_integer_part_mask());

      return local_negatable_type(local_nothing(), -local_value_type(u_mask));
    }
    else
    {
      const local_unsigned_small_type u(x.data);

      const local_unsigned_small_type u_mask(u & local_negatable_type::unsigned_integer_part_mask());

      return ((u == u_mask) ? x : (local_negatable_type(local_nothing(), local_value_type(u_mask)) + 1));
    }
  }

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

      return local_negatable_type(0);
    }
    else
    {
      const bool is_neg = (x.data < 0);

      // Extract the unsigned representation of the data field.
      local_unsigned_small_type result((!is_neg) ? local_unsigned_small_type(x.data) : local_unsigned_small_type(-x.data));

      // Use a binary-halving mechanism to obtain the most significant bit.
      // This will subsequently be used for determination of the binary exponent.
      boost::uint_fast16_t msb;

      {
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

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> fmod(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> y)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(y == 0)
    {
      return local_negatable_type(0);
    }

    // Calculate the fractional part of x such that:
    //   x = (integer_part * y) + fractional_part,
    // where
    //   |fractional_part| < |y|,
    // and fractional_part has the same sign as x.

    const local_negatable_type integer_part = floor(x / y);

    local_negatable_type fractional_part = x - (integer_part * y);

    if((x < 0) != (y < 0))
    {
      fractional_part -= y;
    }

    return fractional_part;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sqrt(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::unsigned_small_type                      local_unsigned_small_type;
    typedef typename local_negatable_type::value_type                               local_value_type;

    if(x.data <= 0)
    {
      return local_negatable_type(0);
    }

    // Get the initial estimate of the square root.
    boost::uint_fast16_t msb;

    // Use a binary-halving mechanism to obtain the most significant bit.
    // This will subsequently be used for range reduction.
    {
      BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t unsigned_small_digits =
        static_cast<boost::uint_fast16_t>(std::numeric_limits<local_unsigned_small_type>::digits);

      local_unsigned_small_type unsigned_small_mask((std::numeric_limits<local_unsigned_small_type>::max)());

      local_unsigned_small_type tmp(x.data);

      msb = detail::msb_helper(tmp, unsigned_small_mask, unsigned_small_digits);
    }

    // Evaluate the necessary amount of right-shift for computing
    // the initial guess of the square root. For this we need to
    // compute the reduced argument as x = [2^n * a] and subsequently
    // remove even powers of 2.
    int n = int(msb) - local_negatable_type::radix_split;

    // Ensure that n is an even such that argument scaling is
    // always carried out with an even number of powers of two.
    if((n & 1) != 0)
    {
      --n;
    }

    local_negatable_type result;

    if(n < 0)
    {
      // Use special handling for arguments less than 1.

      // Shift the data field of the argument up, resulting in
      // a value of x that is greater than or equal to 1.
      x.data = local_value_type(local_unsigned_small_type(x.data)) << -n;

      // Perform the square root calculation on the scaled-up argument.
      result = sqrt(x);

      // Scale the result down.
      result.data = local_value_type(local_unsigned_small_type(result.data) >> (-n / 2));
    }
    else
    {
      // Use the reduced argument (a).
      // Here we estimate the initial guess with:
      //  sqrt(a) = approx. (a/2) + [8^(1/4) - 1]^2
      //          = approx. (a/2) + 0.4648
      //          = approx. (a/2) + (1/2) [via naive simplification].

      // See, for example, J. F. Hart et al., Computer Approximations
      // (John Wiley and Sons, Inc., 1968), Eq. 2.4.3 on page 27.

      // Compute the reduced argument (a) and divide the value
      // of (a) once again by 2 in order to compute the first part
      // of the initial guess.
      local_unsigned_small_type a(x.data);

      a = detail::right_shift_helper(a, n + 1);

      // Here is the addition of (1/2) to complete the initial guess.
      a += (local_unsigned_small_type(1) << (local_negatable_type::radix_split - 1));

      // Remove the scaling from the reduced guess of the result
      // and use this as the proper initial guess of sqrt(x).
      result.data = local_value_type(a << (n / 2));

      // Estimate the zero'th term of the iteration with [1 / (2 * result)].
      local_negatable_type vi = 1U / (result * 2U);

      // Compute the square root of x using coupled Newton iteration.
      // More precisely, this is the Schoenhage variation thereof.
      // We begin with an estimate of 1 binary digit of precision and
      // double the number of binary digits of precision with each iteration.
      // The last iteration is only performed with half of the necessary
      // precision because the precision of the result is subsequently
      // doubled during the course of the last iteration.

      for(boost::uint_fast16_t i = UINT16_C(1); i <= boost::uint_fast16_t(local_negatable_type::all_bits / 2); i *= UINT16_C(2))
      {
        // Perform the next iteration of vi.
        vi += vi * (-((result * vi) * 2U) + 1U);

        // Perform the next iteration of the result.
        result += (vi * (-((result) * (result)) + x));
      }
    }

    return result;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> exp(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle the zero argument.
    if(x == 0)
    {
      return local_negatable_type(1);
    }

    // Handle reflection for negative arguments.
    const bool is_neg = (x < 0);

    if(is_neg)
    {
      return 1 / exp(-x);
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

    const int nf = ((x > local_negatable_type::value_ln_two()) ? int(x / local_negatable_type::value_ln_two()) : 0);

    if(nf > 0)
    {
      x -= (local_negatable_type::value_ln_two() * nf);
    }

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

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> exp(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Handle the zero argument.
    if(x == 0)
    {
      return local_negatable_type(1);
    }

    // Handle reflection for negative arguments.
    const bool is_neg = (x < 0);

    if(is_neg)
    {
      return 1 / exp(-x);
    }

    // Use range reduction and scaling with powers of 2
    // in order to compute exp(x).
    const int nf = ((x > local_negatable_type::value_ln_two()) ? int(x / local_negatable_type::value_ln_two()) : 0);

    if(nf > 0)
    {
      x -= (local_negatable_type::value_ln_two() * nf);
    }

    local_negatable_type result = detail::hypergeometric_0f0(x);

    if(nf > 0)
    {
      result.data <<= nf;
    }

    return result;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    local_negatable_type result;

    if(x < 1)
    {
      result = ((x > 0) ? -log(1 / x) : local_negatable_type(0));
    }
    else if(x > 1)
    {
      // Use frexp() to reduce the argument to 1 <= x <= 2 and
      // store the factors of 2 in an integral variable n.

      int n;

      if(x > 2)
      {
        x = frexp(x, &n);

        x.data <<= 1;
        --n;
      }
      else
      {
        n = 0;
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

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      const local_negatable_type z = x - 1;

      const local_negatable_type polynomial_approximation =
        ((((((      local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0003DFD5) >> (24 + FractionalResolution)))   // 0.01513421407398
              * z - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0013F6AA) >> (24 + FractionalResolution))))  // 0.07798258678209
              * z + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00311C55) >> (24 + FractionalResolution))))  // 0.19183861358090
              * z - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0052F211) >> (24 + FractionalResolution))))  // 0.32400619521798
              * z + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0079099E) >> (24 + FractionalResolution))))  // 0.47280304531019
              * z - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00B86F18) >> (24 + FractionalResolution))))  // 0.72044516000712
              * z + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x01715212) >> (24 + FractionalResolution))))  // 1.44265859709491
              * z;

      // Obtain the result and scale it with the logarithms
      // of the powers of two (if necessary). Note that this
      // result is still a base-2 logarithm.
      const local_negatable_type log_value = polynomial_approximation * local_negatable_type::value_ln_two();

      result = ((n == 0) ? log_value : (log_value + (n * local_negatable_type::value_ln_two())));
    }
    else
    {
      // The argument is exactly 1.
      result = local_negatable_type(0);
    }

    return result;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    local_negatable_type result;

    if(x < 1)
    {
      result = ((x > 0) ? -log(1 / x) : local_negatable_type(0));
    }
    else if(x > 1)
    {
      // Use frexp() to reduce the argument to 1 <= x <= 2 and
      // store the factors of 2 in an integral variable n.

      int n;

      if(x > 2)
      {
        x = frexp(x, &n);

        x.data <<= 1;
        --n;
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
      // The argument is exactly 1.
      result = local_negatable_type(0);
    }

    return result;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log2(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    return log(x) / local_negatable_type::value_ln_two();
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log10(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Consider warm-caching log(10) as a constant value.
    return log(x) / log(local_negatable_type(10));
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> pow(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> a)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x == 0)
    {
      return local_negatable_type(0);
    }
    else if(a == 0)
    {
      return local_negatable_type(1);
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
      return local_negatable_type(1);
    }
    else if(n < 0)
    {
      return 1 / pow(x, -n);
    }
    else
    {
      // The variable xn stores the binary powers of x.
      local_negatable_type result(((n % 2) != 0) ? x : local_negatable_type(1));

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

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) >= (-FractionalResolution)>::type const*)
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

    return (((n % 2) == 0) ? result : -result);
  }

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

        const local_negatable_type sin_half_x = sin(half_x);

        result = (2 * (sin(half_x) * cos(half_x)));
      }
    }

    return (((n % 2) == 0) ? result : -result);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle reflection for negative arguments.
    if(x < 0)
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
      // Use a polynomial approximation.
      // cos(x) = approx. + 0.999999966710846081
      //                  - 0.499999245786563211 x^2
      //                  + 0.041664023481461532 x^4
      //                  - 0.001385684919213699 x^6
      //                  + 0.000023223269589427 x^8
      // in the range -pi/2 <= x <= +pi/2. These coefficients
      // have been specifically derived for this work.

      const local_negatable_type x2 = (x * x);

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      result = ((((       local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00000185) >> (24 + FractionalResolution)))    // 0.999999966710846081
                   * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00005ACE) >> (24 + FractionalResolution))))   // 0.499999245786563211
                   * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000AAA7E) >> (24 + FractionalResolution))))   // 0.041664023481461532
                   * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x007FFFF3) >> (24 + FractionalResolution))))   // 0.001385684919213699
                   * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00FFFFFF) >> (24 + FractionalResolution))));  // 0.000023223269589427
    }

    return (((n % 2) == 0) ? result : -result);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) < (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Handle reflection for negative arguments.
    if(x < 0)
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

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> tan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Handle reflection for negative arguments.
    if(x < 0)
    {
      return -tan(-x);
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
      const local_negatable_type pi_over_four = ldexp(local_negatable_type::value_pi(), -2);

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
        result = local_negatable_type(1);
      }
    }

    return result;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> tan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) < (-FractionalResolution)>::type const*)
  {
    return sin(x) / cos(x);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> asin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(24) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x < 0)
    {
      return -asin(-x);
    }

    local_negatable_type result;

    if(x == 0)
    {
      result = local_negatable_type(0);
    }
    else if(x == 1)
    {
      result = local_negatable_type::value_pi_half();
    }
    else if(x > 1)
    {
      result = local_negatable_type(0);
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
               * x + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0001b00E) >> (24 + FractionalResolution))))  // 0.0065926664377
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

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> asin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(24) < (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x < 0)
    {
      return -asin(-x);
    }

    local_negatable_type result;

    if(x == 0)
    {
      result = local_negatable_type(0);
    }
    else if(x == 1)
    {
      result = local_negatable_type::value_pi_half();
    }
    else if(x > 1)
    {
      result = local_negatable_type(0);
    }
    else
    {
      const local_negatable_type one_half     = ldexp(local_negatable_type(1), -1);
      const local_negatable_type three_halves = local_negatable_type(3) / 2;

      if(x < one_half)
      {
        const local_negatable_type half = local_negatable_type(1) / 2;

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

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> acos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x < 0)
    {
      return local_negatable_type::value_pi_half() - asin(x);
    }

    local_negatable_type result;

    if(x == 0)
    {
      result = local_negatable_type::value_pi_half();
    }
    else if(x == 1)
    {
      result = local_negatable_type(0);
    }
    else if(x > 1)
    {
      result = local_negatable_type(0);
    }
    else
    {
      result = local_negatable_type::value_pi_half() - asin(x);
    }

    return result;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(24) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x < 0)
    {
      return -atan(-x);
    }

    local_negatable_type result;

    if(x == 0)
    {
      result = local_negatable_type(0);
    }
    else if(x == 1)
    {
      result = ldexp(local_negatable_type::value_pi(), -2);
    }
    else if(x > 1)
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
      result = (((((     - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0003A4FC) >> (24 + FractionalResolution)))    // 0.014236215058440372
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000F298F) >> (24 + FractionalResolution))))   // 0.059227914090755230
                    * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x001F654D) >> (24 + FractionalResolution))))   // 0.122639496662731095
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x0032329B) >> (24 + FractionalResolution))))   // 0.196084683947839669
                    * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00554255) >> (24 + FractionalResolution))))   // 0.333043397362190827
                    * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00FFFFC3) >> (24 + FractionalResolution))));  // 0.999996396967490826

      result *= x;
    }

    return result;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                               typename std::enable_if<int(24) < (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x < 0)
    {
      return -atan(-x);
    }

    local_negatable_type result;

    if(x == 0)
    {
      result = local_negatable_type(0);
    }
    else if(x == 1)
    {
      result = ldexp(local_negatable_type::value_pi(), -2);
    }
    else if(x > 1)
    {
      result = local_negatable_type::value_pi_half() - atan(1 / x);
    }
    else if(x < ldexp(local_negatable_type(1), -3))
    {
      // Use a hypergeometric series expansion for small argument.
      result = x * detail::hypergeometric_2f1( local_negatable_type(1),
                                               local_negatable_type(1) / 2,
                                               local_negatable_type(3) / 2,
                                              -(x * x));
    }
    else
    {
      // Use Newton-Raphson iteration for atan.

      // Obtain an initial guess using a two-term Pade-like approximation.
      // See Abramowitz & Stegun, Eq. 4.4.48.
      result = x / (1 + ((7 * (x * x)) / 25));

      // Do the Newton-Raphson iteration. Start with four binary digits
      // of precision obtained from the initial estimate above.
      for(boost::uint_fast16_t i = UINT16_C(4); i <= boost::uint_fast16_t(local_negatable_type::all_bits / 2); i *= UINT16_C(2))
      {
        const local_negatable_type c = cos(result);
        const local_negatable_type s = sin(result);

        result += (c * ((x * c) - s));
      }
    }

    return result;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atan2(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> y, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Handle the negative arguments and zero arguments.
    const bool x_is_neg = (x < 0);
    const bool y_is_neg = (y < 0);

    if(y == 0)
    {
      return ((!x_is_neg) ? local_negatable_type(0) : local_negatable_type::value_pi());
    }

    if(x == 0)
    {
      return ((!y_is_neg) ? local_negatable_type::value_pi_half() : -local_negatable_type::value_pi_half());
    }

    // Compute atan(y / x), thereby ignoring the sign of the arguments.
    const local_negatable_type atan_term(atan(y / x));

    // Determine the proper quadrant based on signs of x and y.
    return ((y_is_neg == x_is_neg) ? ((!x_is_neg) ? atan_term : atan_term - local_negatable_type::value_pi())
                                   : ((!x_is_neg) ? atan_term : atan_term + local_negatable_type::value_pi()));
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sinh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    const local_negatable_type ep = exp(x);
    const local_negatable_type em = 1 / ep;

    return (ep - em) / 2;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cosh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    const local_negatable_type ep = exp(x);
    const local_negatable_type em = 1 / ep;

    return (ep + em) / 2;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> tanh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    const local_negatable_type ep = exp(x);
    const local_negatable_type em = 1 / ep;

    return (ep - em) / (ep + em);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> asinh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x < 0)
    {
      // Handle negative argument.
      return -asinh(-x);
    }

    local_negatable_type result;

    if(x == 0)
    {
      result = local_negatable_type(0);
    }
    else
    {
      if(x < ldexp(local_negatable_type(1), -3))
      {
        // Handle arguments greater than 0 but near 0.
        // Use a hypergeometric series representation here.
        const local_negatable_type one_half = ldexp(local_negatable_type(1), -1);

        result = x * detail::hypergeometric_2f1(one_half, one_half, local_negatable_type(3) / 2, -(x * x));
      }
      else
      {
        // Handle standard arguments greater than 0.
        result = log(x + sqrt((x * x) + 1));
      }
    }

    return result;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> acosh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    local_negatable_type result;

    if(x <= 1)
    {
      // Handle arguments less than or equal to 1.
      result = local_negatable_type(0);
    }
    else
    {
      // Handle arguments greater than 1 but near 1.
      // Use a hypergeometric series representation here.
      const local_negatable_type x_minus_one = x - 1;

      if(x_minus_one < ldexp(local_negatable_type(1), -3))
      {
        const local_negatable_type one_half = ldexp(local_negatable_type(1), -1);

        result =   sqrt(2 * x_minus_one)
                 * detail::hypergeometric_2f1(one_half, one_half, local_negatable_type(3) / 2, -x_minus_one / 2);
      }
      else
      {
        // Handle standard arguments greater than 1.
        result = log(x + sqrt((x * x) - 1));
      }
    }

    return result;
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atanh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x < 0)
    {
      // Handle negative argument.
      return -atanh(-x);
    }

    local_negatable_type result;

    if((x == 0) || (x >= 1))
    {
      // Handle arguments identically equal to 0
      // and arguments greater than or equal to 1.
      result = local_negatable_type(0);
    }
    else
    {
      if(x < ldexp(local_negatable_type(1), -3))
      {
        // Handle small arguments greater than 0.
        // Use a hypergeometric series representation here.
        const local_negatable_type one_half = ldexp(local_negatable_type(1), -1);

        result = x * detail::hypergeometric_2f1(local_negatable_type(1), one_half, local_negatable_type(3) / 2, (x * x));
      }
      else
      {
        // Handle standard arguments greater than 0.
        result = (log(1 + x) - log(1 - x)) / 2;
      }
    }

    return result;
  }
  } } // namespace boost::fixed_point

#endif // FIXED_POINT_NEGATABLE_CMATH_2015_08_21_HPP_
