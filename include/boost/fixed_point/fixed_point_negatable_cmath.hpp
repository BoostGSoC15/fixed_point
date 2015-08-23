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

  template<const int Crng, const int Crsl, typename Crnd, typename Covf>
  negatable<Crng, Crsl, Crnd, Covf> abs(negatable<Crng, Crsl, Crnd, Covf> x)
  {
    return ((x.data < 0) ? -x : x);
  }

  //! @c std::fabs function (identical to abs and provided for completeness).
  template<const int Crng, const int Crsl, typename Crnd, typename Covf>
  negatable<Crng, Crsl, Crnd, Covf> fabs(negatable<Crng, Crsl, Crnd, Covf> x)
  {
    return ((x.data < 0) ? -x : x);
  }

  template<const int Crng, const int Crsl, typename Crnd, typename Covf>
  negatable<Crng, Crsl, Crnd, Covf> floor(negatable<Crng, Crsl, Crnd, Covf> x)
  {
    typedef negatable<Crng, Crsl, Crnd, Covf>                  local_negatable_type;
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

  template<const int Crng, const int Crsl, typename Crnd, typename Covf>
  negatable<Crng, Crsl, Crnd, Covf> ceil(negatable<Crng, Crsl, Crnd, Covf> x)
  {
    typedef negatable<Crng, Crsl, Crnd, Covf>                  local_negatable_type;
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

  template<const int Crng, const int Crsl, typename Crnd, typename Covf>
  negatable<Crng, Crsl, Crnd, Covf> trunc(negatable<Crng, Crsl, Crnd, Covf> x)
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
  template<const int Crng, const int Crsl, typename Crnd, typename Covf>
  negatable<Crng, Crsl, Crnd, Covf> frexp(negatable<Crng, Crsl, Crnd, Covf> x, int* exp2)
  {
    typedef negatable<Crng, Crsl, Crnd, Covf>                  local_negatable_type;
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
  template<const int Crng, const int Crsl, typename Crnd, typename Covf>
  negatable<Crng, Crsl, Crnd, Covf> ldexp(negatable<Crng, Crsl, Crnd, Covf> x, int exp2)
  {
    typedef negatable<Crng, Crsl, Crnd, Covf>                  local_negatable_type;
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

      return local_negatable_type(local_nothing(), local_value_type((!is_neg)
        ?  local_value_type(result)
        : -local_value_type(result)));
    }
    else
    {
      return x;
    }
  }

  template<const int Crng, const int Crsl, typename Crnd, typename Covf>
  negatable<Crng, Crsl, Crnd, Covf> sqrt(negatable<Crng, Crsl, Crnd, Covf> x)
  {
    typedef negatable<Crng, Crsl, Crnd, Covf>                  local_negatable_type;
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

  template<const int Crng, const int Crsl, typename Crnd, typename Covf>
  negatable<Crng, Crsl, Crnd, Covf> exp(negatable<Crng, Crsl, Crnd, Covf> x)
  {
    typedef negatable<Crng, Crsl, Crnd, Covf> local_negatable_type;

    const bool is_neg = (x < 0);

    if(is_neg)
    {
      return exp(1 / x);
    }

    local_negatable_type xx = ((!is_neg) ? x : -x);

    local_negatable_type exp_series;

    if(xx > local_negatable_type::value_ln_two())
    {
      const boost::uint_fast16_t nf = floor(xx / local_negatable_type::value_ln_two());

      const local_negatable_type scaled_argument = xx - (local_negatable_type::value_ln_two() * nf);

      exp_series = detail::hypergeometric_0f0(scaled_argument) * detail::power_of_two_helper<local_negatable_type>(int(nf));
    }
    else
    {
      exp_series = detail::hypergeometric_0f0(xx);
    }

    return ((!is_neg) ? exp_series : (1 / exp_series));
  }

  template<const int Crng, const int Crsl, typename Crnd, typename Covf>
  negatable<Crng, Crsl, Crnd, Covf> log(negatable<Crng, Crsl, Crnd, Covf> x)
  {
    typedef negatable<Crng, Crsl, Crnd, Covf> local_negatable_type;

    if(x < 0)
    {
      return local_negatable_type(0);
    }
    else if(x < 1)
    {
      return -log(1 / x);
    }
    else if(x > 1)
    {
      // Use frexp() to reduce the argument to x = y * 2^n. See also the eval_log()
      // function in Boost.Multiprecision.
      // Then use log(x) = (n * log(2)) + [(y - 1) - (y - 1)^2 / 2 + (y - 1)^3 / 3 ...].

      int n;
      const local_negatable_type y = frexp(x, &n);

      BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t maximum_number_of_iterations = UINT16_C(10000);

      const local_negatable_type y_minus_one = y - 1;

      local_negatable_type y_minus_one_pow_n = y_minus_one;
      local_negatable_type log_series        = y_minus_one;

      bool term_is_negative = true;

      const local_negatable_type tolerance = ldexp(local_negatable_type(1), -std::numeric_limits<local_negatable_type>::digits + 2);

      // Perform the series expansion of the logarithmic function.

      // TBD: It is probably more efficient here to use Newton
      // iteration in combination with the exponenticl function.
      // Consider determining the optimization potential here.
      for(n = UINT16_C(2); n < maximum_number_of_iterations; ++n)
      {
        y_minus_one_pow_n *= y_minus_one;

        const local_negatable_type term = y_minus_one_pow_n / n;

        ((!term_is_negative) ? (log_series += term) : (log_series -= term));

        const bool minimum_number_of_iterations_is_complete = (n > UINT16_C(4));

        if(minimum_number_of_iterations_is_complete)
        {
          if(fabs(term) < tolerance)
          {
            break;
          }
        }

        term_is_negative = (!term_is_negative);
      }

      return log_series + (n * local_negatable_type::value_ln_two());
    }
    else
    {
      return local_negatable_type(0);
    }
  }

  template<const int Crng, const int Crsl, typename Crnd, typename Covf>
  negatable<Crng, Crsl, Crnd, Covf> acos(negatable<Crng, Crsl, Crnd, Covf> x)
  {
    typedef negatable<Crng, Crsl, Crnd, Covf> local_negatable_type;

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
  } } // namespace boost::fixed_point

  //! Effectively inject all defined cmath functions into the global namespace.
  //! So users can write sqrt(negatable) without requiring any namespace decoration.

  using boost::fixed_point::abs;
  using boost::fixed_point::fabs;
  using boost::fixed_point::frexp;
  using boost::fixed_point::ldexp;
  using boost::fixed_point::floor;
  using boost::fixed_point::ceil;
  using boost::fixed_point::trunc;
  using boost::fixed_point::sqrt;
  using boost::fixed_point::exp;
  using boost::fixed_point::log;
  using boost::fixed_point::acos;

  // TBD: Ensure that ALL std:: added functions are also listed in this section.

#endif // FIXED_POINT_NEGATABLE_CMATH_2015_08_21_HPP_
