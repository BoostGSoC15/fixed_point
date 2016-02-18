///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015 - 2016.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// fixed_point_detail_constants.hpp implements templates
// for computing fixed-point representations of the
// mathematical constants sqrt(2), pi, log(2) and e.

#ifndef FIXED_POINT_DETAIL_CONSTANTS_2015_08_16_HPP_
  #define FIXED_POINT_DETAIL_CONSTANTS_2015_08_16_HPP_

  #include <cmath>
  #include <limits>

  namespace boost { namespace fixed_point { namespace detail {

  template<typename NumericType>
  NumericType calculate_root_two()
  {
    // Provide a very rough initial guess of 4/3, which is about
    // 5 percent off of the actual value. Use small integers here
    // because the underlying NumericType might have limited range.
    NumericType a(NumericType(4U) / 3U);

    // Estimate the zero'th term of the iteration with [1 / (2 * result)].
    NumericType vi(NumericType(1U) / (a * 2U));

    // Compute the square root of x using coupled Newton iteration.
    // More precisely, this is the Schoenhage variation thereof.
    // We begin with an estimate of 1 binary digit of precision and
    // double the number of binary digits of precision with each iteration.

    for(boost::uint_fast16_t i = UINT16_C(1); i <= boost::uint_fast16_t(std::numeric_limits<NumericType>::digits); i *= UINT16_C(2))
    {
      // Perform the next iteration of vi.
      vi += vi * (-((a * vi) * 2U) + NumericType(1U));

      // Perform the next iteration of the result.
      a += (vi * (-((a) * (a)) + NumericType(2U)));
    }

    return a;
  }

  template<typename NumericType>
  NumericType calculate_pi()
  {
    // Use a quadratically converging Gauss-AGM method to compute pi.

    using std::fabs;
    using std::ldexp;
    using std::sqrt;

    NumericType val_pi;

    NumericType a (1U);
    NumericType bB(NumericType(1U) / 2U);
    NumericType s (NumericType(1U) / 2U);
    NumericType t (NumericType(3U) / 8U);

    // This loop is designed for computing a maximum of a few million
    // decimal digits of pi. The number of digits roughly doubles
    // with each iteration of the loop. After 20 iterations,
    // the precision is about 2.8 million decimal digits.
    // We are, however, not using that many digits in this
    // application --- rarely more than a few thousand at most.

    const NumericType tolerance = ldexp(NumericType(1U), -int((long(std::numeric_limits<NumericType>::digits) * 3L) / 4L));

    for(boost::uint_least8_t k = UINT8_C(1); k < UINT8_C(32); ++k)
    {
      // Perform the iteration steps of the Gauss AGM.

      a      += sqrt(bB);
      a      /= 2U;
      val_pi  = (a * a);
      bB      = (val_pi - t) * 2U;

      const NumericType iterate_term((bB - val_pi) * (UINT32_C(1) << k));

      s += iterate_term;

      const bool minimum_number_of_iterations_is_complete = (k > UINT8_C(4));

      if((minimum_number_of_iterations_is_complete) && (fabs(iterate_term) <= tolerance))
      {
        break;
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

    using std::ldexp;
    using std::sqrt;

    // Choose m > (N * 1.661), where N is the number of decimal digits requested.
    BOOST_CONSTEXPR_OR_CONST int m((long(std::numeric_limits<NumericType>::digits10) * 17L) / 10L);

    // Set a0 = 1.
    // Set b0 = 4 / (2^m) = 1 / 2^(m - 2).
    NumericType ak(1);

    NumericType bk(ldexp(NumericType(1), -(m - 2)));

    const NumericType tolerance = ldexp(NumericType(1U), -int((long(std::numeric_limits<NumericType>::digits) * 3L) / 4L));

    for(boost::uint_least8_t k = UINT8_C(0); k < UINT8_C(32); ++k)
    {
      const NumericType a(ak);
      ak += bk;
      ak /= 2U;
      bk  = sqrt(bk * a);

      const bool minimum_number_of_iterations_is_complete = (k > UINT8_C(4));

      if((minimum_number_of_iterations_is_complete) && (fabs(ak - bk) <= tolerance))
      {
        break;
      }
    }

    // Define the large floating-point type that corresponds to the
    // large fixed-point type.

    // The iteration is finished: Compute ln2 = pi / [AGM(1, 4 / 2^m) * 2m].
    // Note that the result of the AGM iteration is: ak = bk = AGM(...).
    const NumericType val_ln_two = calculate_pi<NumericType>() / (ak * (2U * m));

    return val_ln_two;
  }

  template<typename NumericType>
  NumericType calculate_e()
  {
    NumericType term(1U);
    NumericType sum (2U);

    BOOST_CONSTEXPR_OR_CONST boost::uint32_t maximum_number_of_iterations = UINT32_C(10000);

    // Perform the Taylor series expansion of Euler's constant, e = exp(1).
    for(boost::uint32_t n = UINT32_C(2); n < maximum_number_of_iterations; ++n)
    {
      term /= n;

      const bool minimum_number_of_iterations_is_complete = (n > UINT32_C(4));

      using std::fabs;

      if(   (minimum_number_of_iterations_is_complete)
         && (fabs(term) <= std::numeric_limits<NumericType>::epsilon()))
      {
        break;
      }

      sum += term;
    }

    return sum;
  }
  } } } // namespace boost::fixed_point::detail

#endif // FIXED_POINT_DETAIL_CONSTANTS_2015_08_16_HPP_
