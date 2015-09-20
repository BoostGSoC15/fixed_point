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
    typedef typename local_negatable_type::unsigned_small_type local_unsigned_small_type;
    typedef typename local_negatable_type::value_type          local_value_type;
    typedef typename local_negatable_type::nothing             local_nothing;

    if(x < 0)
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
    typedef typename local_negatable_type::value_type          local_value_type;
    typedef typename local_negatable_type::unsigned_small_type local_unsigned_small_type;
    typedef typename local_negatable_type::nothing             local_nothing;

    if(x < 0)
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
    const bool is_neg = (x.data < 0);

    return ((!is_neg) ? floor(x) : -floor(-x));
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
    typedef typename local_negatable_type::unsigned_small_type local_unsigned_small_type;
    typedef typename local_negatable_type::value_type          local_value_type;
    typedef typename local_negatable_type::nothing             local_nothing;

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

      local_unsigned_small_type unsigned_small_mask((std::numeric_limits<local_unsigned_small_type>::max)());

      BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t unsigned_small_digits =
        static_cast<boost::uint_fast16_t>(std::numeric_limits<local_unsigned_small_type>::digits);

      boost::uint_fast16_t msb;

      {
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
    typedef typename local_negatable_type::unsigned_small_type local_unsigned_small_type;
    typedef typename local_negatable_type::value_type          local_value_type;
    typedef typename local_negatable_type::nothing             local_nothing;

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
    typedef typename local_negatable_type::unsigned_small_type local_unsigned_small_type;

    // TBD: This implementation of square root seems to fail
    // for low digit counts.

    // TBD: This implementation of square root may be too inefficient
    // for low digit counts such as 5...16 digits. Consider optimization
    // (such as table-lookup of initial root estimate) for low digit counts.

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

      // TBD: Is there a more efficient way to do this?
      // TBD: Is this initial guess accurate enough in all situations?
      // TBD: Compute some initial guesses and tabulate them as constant values.

      int exp_value;
      local_negatable_type mantissa = frexp(x, &exp_value);

      if((exp_value & 1) != 0)
      {
        mantissa.data <<= 1;
        --exp_value;
      }

      local_negatable_type result(ldexp(mantissa, exp_value / 2));

      // Estimate the zeroth term of the iteration = 1 / (2 * result).
      local_negatable_type vi = 1U / (result * 2U);

      // Compute the square root of x using coupled Newton iteration.
      // Here we use a conservative calculation of the digits of precision.
      // We begin with an estimate of 1/2 digit of precision and double
      // the number of digits with each iteration.

      // TBD: Here we are using too many Newton-Raphson steps.
      // TBD: Improve the accuracy of the initial estimate of the
      // square root and subsequently use a tighter tolerance
      // on the number of iterations in the Newton-Raphson loop.

      for(boost::uint_fast16_t i = UINT16_C(1); boost::uint_fast16_t(i / 2U) <= boost::uint_fast16_t(local_negatable_type::all_bits); i *= UINT16_C(2))
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
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> exp(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
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

    local_negatable_type xx = ((!is_neg) ? x : -x);

    local_negatable_type exp_series;

    if(xx > local_negatable_type::value_ln_two())
    {
      const boost::uint_fast16_t nf = static_cast<boost::uint_fast16_t>(floor(xx / local_negatable_type::value_ln_two()));

      const local_negatable_type scaled_argument = xx - (local_negatable_type::value_ln_two() * nf);

      exp_series = detail::hypergeometric_0f0(scaled_argument) * detail::power_of_two_helper<local_negatable_type>(int(nf));
    }
    else
    {
      exp_series = detail::hypergeometric_0f0(xx);
    }

    return ((!is_neg) ? exp_series : (1 / exp_series));
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
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

      BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t maximum_number_of_iterations = UINT16_C(10000);

      const local_negatable_type y_minus_one = y - 1;

      local_negatable_type y_minus_one_pow_n = y_minus_one;
      local_negatable_type log_series        = y_minus_one;

      bool term_is_negative = true;

      const local_negatable_type tolerance = ldexp(local_negatable_type(1), local_negatable_type::resolution + 1);

      // Perform the series expansion of the logarithmic function.

      // TBD: It is probably more efficient here to use Newton
      // iteration in combination with the exponential function.
      // Consider determining the optimization potential here.
      for(boost::uint_fast16_t n = UINT16_C(2); n < maximum_number_of_iterations; ++n)
      {
        y_minus_one_pow_n *= y_minus_one;

        const local_negatable_type term = y_minus_one_pow_n / n;

        ((!term_is_negative) ? (log_series += term) : (log_series -= term));

        const bool minimum_number_of_iterations_is_complete = (n > UINT16_C(4));

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
    else
    {
      return exp(a * log(x));
    }
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sin(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    return local_negatable_type(0);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cos(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    return local_negatable_type(0);
  }

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> tan(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    return local_negatable_type(0);
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
      return local_negatable_type::value_pi() / 2;
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
