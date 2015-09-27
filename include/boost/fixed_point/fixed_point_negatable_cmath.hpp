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
  \brief Fixed_point negatable <cmath> functions.
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
    return ((!(x.data < 0)) ? floor(x) : -floor(-x));
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

      boost::uint_fast16_t msb;

      {
        BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t unsigned_small_digits =
          static_cast<boost::uint_fast16_t>(std::numeric_limits<local_unsigned_small_type>::digits);

        local_unsigned_small_type unsigned_small_mask((std::numeric_limits<local_unsigned_small_type>::max)());

        local_unsigned_small_type tmp(result);

        // Use a binary-halving mechanism to obtain the most significant bit.
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

      result = (result >> -exp2);

      return local_negatable_type(local_nothing(), local_value_type((!is_neg) ?  local_value_type(result)
                                                                              : -local_value_type(result)));
    }
    else
    {
      return x;
    }
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
    else if(local_unsigned_small_type(x.data) < local_negatable_type::radix_split_value())
    {
      return 1U / (sqrt(1U / x));
    }
    else
    {
      // Get the initial estimate of the square root.
      boost::uint_fast16_t msb;

      {
        // Find the most significant bit of the argument.
        BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t unsigned_small_digits =
          static_cast<boost::uint_fast16_t>(std::numeric_limits<local_unsigned_small_type>::digits);

        local_unsigned_small_type unsigned_small_mask((std::numeric_limits<local_unsigned_small_type>::max)());

        local_unsigned_small_type tmp(x.data);

        // Use a binary-halving mechanism to obtain the most significant bit.
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

      // Use the reduced argument (a). Here we estimate the initial guess:
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

      local_negatable_type result;

      // Remove the scaling from the reduced guess of the result
      // and use this as the proper initial guess of sqrt(x).
      result.data = local_value_type(a << (n / 2));

      // Estimate the zero'th term of the iteration with [1 / (2 * result)].
      local_negatable_type vi = 1 / result;
      vi.data >>= 1;

      // Compute the square root of x using coupled Newton iteration.
      // More precisely, this is the Schoenhage variation thereof.
      // We begin with an estimate of 1 binary digit of precision and
      // double the number of binary digits of precision with each iteration.
      // The last iteration is only be performed with half of the necessary
      // precision because the precision of the result is subsequently
      // doubled during the course of the last iteration.

      for(boost::uint_fast16_t i = UINT16_C(1); i <= boost::uint_fast16_t(local_negatable_type::all_bits / 2); i *= UINT16_C(2))
      {
        // Perform the next iteration of vi.
        vi += vi * (-((result * vi) * 2U) + 1U);

        // Perform the next iteration of the result.
        result += (vi * (-((result) * (result)) + x));
      }

      return result;
    }
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> exp(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    if(x == 0)
    {
      return local_negatable_type(1);
    }

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
    // in the range -1 <= x <= +1.

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

    if(x == 0)
    {
      return local_negatable_type(1);
    }

    const bool is_neg = (x < 0);

    if(is_neg)
    {
      return 1 / exp(-x);
    }

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
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    // TBD: The tolerance of the log function can potentially be improved.

    // TBD: Make a log function for small digit range using:
    // log2(x + 1) = approx. + 1.44265859709491 x
    //                       - 0.72044516000712 x^2
    //                       + 0.47280304531019 x^3
    //                       - 0.32400619521798 x^4
    //                       + 0.19183861358090 x^5
    //                       - 0.07798258678209 x^6
    //                       + 0.01513421407398 x^7

    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    if(x <= 0)
    {
      return local_negatable_type(0);
    }
    else if(x < 1)
    {
      return -log(1 / x);
    }
    else if(x > 1)
    {
      // Use frexp() to reduce the argument to x = y * 2^n, where 0.5 <= y < 1.
      // Then use log(x) = [n * log(2)] + [(y - 1) - (y - 1)^2 / 2 + (y - 1)^3 / 3 ...].
      // This method is similar to the eval_log() function in Boost.Multiprecision.

      int exp2;
      const local_negatable_type y = frexp(x, &exp2);

      BOOST_CONSTEXPR boost::uint32_t maximum_number_of_iterations = UINT32_C(10000);

      const local_negatable_type y_minus_one = y - 1;

      local_negatable_type y_minus_one_pow_n = y_minus_one;
      local_negatable_type log_series        = y_minus_one;

      bool term_is_negative = true;

      const local_negatable_type tolerance = ldexp(local_negatable_type(1), local_negatable_type::resolution + 1);

      // Perform the series expansion of the logarithmic function.

      // TBD: It may more efficient here to use Newton iteration
      // in combination with the exponential function in higher
      // digit ranges. Consider investigating any optimization
      // potential here.

      for(boost::uint32_t n = UINT32_C(2); n < maximum_number_of_iterations; ++n)
      {
        y_minus_one_pow_n *= y_minus_one;

        const local_negatable_type term = y_minus_one_pow_n / n;

        ((!term_is_negative) ? (log_series += term) : (log_series -= term));

        const bool minimum_number_of_iterations_is_complete = (n > UINT32_C(4));

        if((minimum_number_of_iterations_is_complete) && (fabs(term) <= tolerance))
        {
          break;
        }

        term_is_negative = (!term_is_negative);
      }

      return log_series + (exp2 * local_negatable_type::value_ln_two());
    }
    else
    {
      // The argument is exactly 1.
      return local_negatable_type(0);
    }
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

    // TBD: Consider warm-caching ln_ten as a constant value.
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
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Reduce the argument to the range 0 <= x <= +pi/2.
    const bool is_neg = (x < 0);

    if(is_neg)
    {
      x = -x;
    }

    const int n = ((x > local_negatable_type::value_pi()) ? int(x / local_negatable_type::value_pi()) : 0);

    const bool negate_result = (is_neg != ((n % 2) != 0));

    if(n > 0)
    {
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
      // in the range -pi/2 <= x <= +pi/2.

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

    return ((!negate_result) ? result : -result);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) <  (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Reduce the argument to the range 0 <= x <= +pi/2.
    const bool is_neg = (x < 0);

    if(is_neg)
    {
      x = -x;
    }

    const int n = ((x > local_negatable_type::value_pi()) ? int(x / local_negatable_type::value_pi()) : 0);

    const bool negate_result = (is_neg != ((n % 2) != 0));

    if(n > 0)
    {
      x -= (n * local_negatable_type::value_pi());
    }

    local_negatable_type result;

    if(x > local_negatable_type::value_pi_half())
    {
      result = sin(local_negatable_type::value_pi() - x);
    }
    else
    {
      if(x <= (local_negatable_type::value_pi() / 4))
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
        // Use an angle doubling identity to reduce the argument to less than +pi/4.
        const local_negatable_type half_x = x / 2;

        const local_negatable_type sin_half_x = sin(half_x);

        result = (2 * (sin(half_x) * cos(half_x)));
      }
    }

    return ((!negate_result) ? result : -result);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) >= (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    // Reduce the argument to the range 0 <= x <= +pi/2.
    const bool is_neg = (x < 0);

    if(is_neg)
    {
      x = -x;
    }

    const int n = ((x > local_negatable_type::value_pi()) ? int(x / local_negatable_type::value_pi()) : 0);

    const bool negate_result = ((n % 2) != 0);

    if(n > 0)
    {
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
      // cos(x) = approx. + 0.9999999667108460810462
      //                  - 0.4999992457865632108920 x^2
      //                  + 0.0416640234814615320276 x^4
      //                  - 0.0013856849192136990974 x^6
      //                  + 0.0000232232695894268079 x^8
      // in the range -pi/2 <= x <= +pi/2.

      const local_negatable_type x2 = (x * x);

      // Perform the polynomial approximation using a coefficient
      // expansion via the method of Horner.
      result = ((((       local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00000185) >> (24 + FractionalResolution)))    // 0.9999999667108460810462
                   * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00005ACE) >> (24 + FractionalResolution))))   // 0.4999992457865632108920
                   * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x000AAA7E) >> (24 + FractionalResolution))))   // 0.0416640234814615320276
                   * x2 - local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x007FFFF3) >> (24 + FractionalResolution))))   // 0.0013856849192136990974
                   * x2 + local_negatable_type(local_nothing(), local_value_type(UINT32_C(0x00FFFFFF) >> (24 + FractionalResolution))));  // 0.0000232232695894268079
    }

    return ((!negate_result) ? result : -result);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                                                                              typename std::enable_if<int(24) < (-FractionalResolution)>::type const*)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // Reduce the argument to the range 0 <= x <= +pi/2.
    const bool is_neg = (x < 0);

    if(is_neg)
    {
      x = -x;
    }

    const int n = ((x > local_negatable_type::value_pi()) ? int(x / local_negatable_type::value_pi()) : 0);

    const bool negate_result = ((n % 2) != 0);

    if(n > 0)
    {
      x -= (n * local_negatable_type::value_pi());
    }

    local_negatable_type result;

    if(x > local_negatable_type::value_pi_half())
    {
      result = -cos(local_negatable_type::value_pi() - x);
    }
    else
    {
      if(x <= (local_negatable_type::value_pi() / 4))
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
        // Use an angle doubling identity to reduce the argument to less than +pi/4.
        const local_negatable_type half_x = x / 2;

        const local_negatable_type cos_half_x = cos(half_x);

        result = (2 * (cos_half_x * cos_half_x)) - 1;
      }
    }

    return ((!negate_result) ? result : -result);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> tan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    return sin(x) / cos(x);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> asin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    return local_negatable_type(0);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> acos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    // TBD: This version of acos is essentially non-functional because
    // it merely returns a sensible value for one and only one argument
    // (at x == 0). This result is subsequently used in Boost.Math.Constants
    // for the calculation of pi.

    // TBD: Make a complete (and efficient) version of acos.

    if(x == 0)
    {
      return local_negatable_type::value_pi_half();
    }
    else
    {
      return local_negatable_type(0);
    }
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    return local_negatable_type(0);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atan2(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> y, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    return local_negatable_type(0);
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

    return local_negatable_type(0);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> acosh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    return local_negatable_type(0);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atanh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    return local_negatable_type(0);
  }
  } } // namespace boost::fixed_point

#endif // FIXED_POINT_NEGATABLE_CMATH_2015_08_21_HPP_
