///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2013 - 2016.
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
  \brief Fixed_point negatable specializations of std::numeric_limits<negatable>.
*/

#ifndef FIXED_POINT_NEGATABLE_LIMITS_2015_08_24_HPP_
  #define FIXED_POINT_NEGATABLE_LIMITS_2015_08_24_HPP_

  namespace std {

  //! Provide specializations of std::numeric_limits<negatable>.

  /*! \note Individual template specializations need to be provided
    for each different rounding mode and overflow mode.
    This might be 7 rounding * 5 overflow, a total of 35 specializations!
  */

  /*!template specialization of std::numeric_limits<negatable>
      for @c round::fastest and @c overflow::undefined.
  */
  template<const int IntegralRange, const int FractionalResolution>
  class numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>
  {
  private:
    typedef boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>
    local_negatable_type;

  public:
    BOOST_STATIC_CONSTEXPR bool                    is_specialized    = true;
    BOOST_STATIC_CONSTEXPR int                     digits            = local_negatable_type::all_bits - 1;
    BOOST_STATIC_CONSTEXPR int                     digits10          = static_cast<int>((static_cast<boost::uintmax_t>(digits - 1) * UINTMAX_C(301)) / UINTMAX_C(1000));
    BOOST_STATIC_CONSTEXPR int                     max_digits10      = static_cast<int>((static_cast<boost::uintmax_t>(digits - 0) * UINTMAX_C(301)) / UINTMAX_C(1000)) + 2;
    BOOST_STATIC_CONSTEXPR bool                    is_signed         = true;
    BOOST_STATIC_CONSTEXPR bool                    is_integer        = false;
    BOOST_STATIC_CONSTEXPR bool                    is_exact          = false;
    BOOST_STATIC_CONSTEXPR int                     radix             = 2;
    BOOST_STATIC_CONSTEXPR int                     min_exponent      = -local_negatable_type::radix_split;
    BOOST_STATIC_CONSTEXPR int                     min_exponent10    = -static_cast<int>((static_cast<boost::uintmax_t>(-min_exponent) * UINTMAX_C(301)) / UINTMAX_C(1000));
    BOOST_STATIC_CONSTEXPR int                     max_exponent      = digits - local_negatable_type::radix_split;
    BOOST_STATIC_CONSTEXPR int                     max_exponent10    = +static_cast<int>((static_cast<boost::uintmax_t>(+max_exponent) * UINTMAX_C(301)) / UINTMAX_C(1000));
    BOOST_STATIC_CONSTEXPR bool                    has_infinity      = false;
    BOOST_STATIC_CONSTEXPR bool                    has_quiet_NaN     = false;
    BOOST_STATIC_CONSTEXPR bool                    has_signaling_NaN = false;
    BOOST_STATIC_CONSTEXPR std::float_denorm_style has_denorm        = std::denorm_absent;
    BOOST_STATIC_CONSTEXPR bool                    has_denorm_loss   = false;
    BOOST_STATIC_CONSTEXPR bool                    is_iec559         = false;
    BOOST_STATIC_CONSTEXPR bool                    is_bounded        = true;
    BOOST_STATIC_CONSTEXPR bool                    is_modulo         = false;
    BOOST_STATIC_CONSTEXPR bool                    traps             = false;
    BOOST_STATIC_CONSTEXPR bool                    tinyness_before   = false;
    BOOST_STATIC_CONSTEXPR std::float_round_style  round_style       = std::round_indeterminate;

    BOOST_STATIC_CONSTEXPR local_negatable_type (min)        () BOOST_NOEXCEPT { return local_negatable_type::value_min(); }
    BOOST_STATIC_CONSTEXPR local_negatable_type (max)        () BOOST_NOEXCEPT { return local_negatable_type::value_max(); }
    BOOST_STATIC_CONSTEXPR local_negatable_type lowest       () BOOST_NOEXCEPT { return local_negatable_type::value_lowest(); }
    BOOST_STATIC_CONSTEXPR local_negatable_type epsilon      () BOOST_NOEXCEPT { return local_negatable_type::value_epsilon(); }
    BOOST_STATIC_CONSTEXPR local_negatable_type round_error  () BOOST_NOEXCEPT { return local_negatable_type(1); }
    BOOST_STATIC_CONSTEXPR local_negatable_type infinity     () BOOST_NOEXCEPT { return local_negatable_type(0); }
    BOOST_STATIC_CONSTEXPR local_negatable_type quiet_NaN    () BOOST_NOEXCEPT { return local_negatable_type(0); }
    BOOST_STATIC_CONSTEXPR local_negatable_type signaling_NaN() BOOST_NOEXCEPT { return local_negatable_type(0); }
    BOOST_STATIC_CONSTEXPR local_negatable_type denorm_min   () BOOST_NOEXCEPT { return (min)(); }
  };

  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_specialized;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::digits;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::digits10;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::max_digits10;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_signed;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_integer;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_exact;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::radix;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::min_exponent;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::min_exponent10;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::max_exponent;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::max_exponent10;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_infinity;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_quiet_NaN;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_signaling_NaN;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST std::float_denorm_style numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_denorm;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_denorm_loss;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_iec559;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_bounded;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_modulo;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::traps;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::tinyness_before;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST std::float_round_style  numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::round_style;

  /*!template specialization of std::numeric_limits<negatable>
      for @c round::nearest_even and @c overflow::undefined.
  */
  template<const int IntegralRange, const int FractionalResolution>
  class numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>
  {
  private:
    typedef boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>
    local_negatable_type;

  public:
    BOOST_STATIC_CONSTEXPR bool                    is_specialized    = true;
    BOOST_STATIC_CONSTEXPR int                     digits            = local_negatable_type::all_bits - 1;
    BOOST_STATIC_CONSTEXPR int                     digits10          = static_cast<int>((static_cast<boost::uintmax_t>(digits - 1) * UINTMAX_C(301)) / UINTMAX_C(1000));
    BOOST_STATIC_CONSTEXPR int                     max_digits10      = static_cast<int>((static_cast<boost::uintmax_t>(digits - 0) * UINTMAX_C(301)) / UINTMAX_C(1000)) + 2;
    BOOST_STATIC_CONSTEXPR bool                    is_signed         = true;
    BOOST_STATIC_CONSTEXPR bool                    is_integer        = false;
    BOOST_STATIC_CONSTEXPR bool                    is_exact          = false;
    BOOST_STATIC_CONSTEXPR int                     radix             = 2;
    BOOST_STATIC_CONSTEXPR int                     min_exponent      = -local_negatable_type::radix_split;
    BOOST_STATIC_CONSTEXPR int                     min_exponent10    = -static_cast<int>((static_cast<boost::uintmax_t>(-min_exponent) * UINTMAX_C(301)) / UINTMAX_C(1000));
    BOOST_STATIC_CONSTEXPR int                     max_exponent      = digits - local_negatable_type::radix_split;
    BOOST_STATIC_CONSTEXPR int                     max_exponent10    = +static_cast<int>((static_cast<boost::uintmax_t>(+max_exponent) * UINTMAX_C(301)) / UINTMAX_C(1000));
    BOOST_STATIC_CONSTEXPR bool                    has_infinity      = false;
    BOOST_STATIC_CONSTEXPR bool                    has_quiet_NaN     = false;
    BOOST_STATIC_CONSTEXPR bool                    has_signaling_NaN = false;
    BOOST_STATIC_CONSTEXPR std::float_denorm_style has_denorm        = std::denorm_absent;
    BOOST_STATIC_CONSTEXPR bool                    has_denorm_loss   = false;
    BOOST_STATIC_CONSTEXPR bool                    is_iec559         = false;
    BOOST_STATIC_CONSTEXPR bool                    is_bounded        = true;
    BOOST_STATIC_CONSTEXPR bool                    is_modulo         = false;
    BOOST_STATIC_CONSTEXPR bool                    traps             = false;
    BOOST_STATIC_CONSTEXPR bool                    tinyness_before   = false;
    BOOST_STATIC_CONSTEXPR std::float_round_style  round_style       = std::round_to_nearest;

    BOOST_STATIC_CONSTEXPR local_negatable_type (min)        () BOOST_NOEXCEPT { return local_negatable_type::value_min(); }
    BOOST_STATIC_CONSTEXPR local_negatable_type (max)        () BOOST_NOEXCEPT { return local_negatable_type::value_max(); }
    BOOST_STATIC_CONSTEXPR local_negatable_type lowest       () BOOST_NOEXCEPT { return  local_negatable_type::value_lowest(); }
    BOOST_STATIC_CONSTEXPR local_negatable_type epsilon      () BOOST_NOEXCEPT { return local_negatable_type::value_epsilon(); }
    BOOST_STATIC_CONSTEXPR local_negatable_type round_error  () BOOST_NOEXCEPT { return local_negatable_type(1) / 2; }
    BOOST_STATIC_CONSTEXPR local_negatable_type infinity     () BOOST_NOEXCEPT { return local_negatable_type(0); }
    BOOST_STATIC_CONSTEXPR local_negatable_type quiet_NaN    () BOOST_NOEXCEPT { return local_negatable_type(0); }
    BOOST_STATIC_CONSTEXPR local_negatable_type signaling_NaN() BOOST_NOEXCEPT { return local_negatable_type(0); }
    BOOST_STATIC_CONSTEXPR local_negatable_type denorm_min   () BOOST_NOEXCEPT { return (min)(); }
  };

  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_specialized;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::digits;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::digits10;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::max_digits10;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_signed;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_integer;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_exact;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::radix;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::min_exponent;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::min_exponent10;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::max_exponent;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::max_exponent10;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_infinity;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_quiet_NaN;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_signaling_NaN;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST std::float_denorm_style numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_denorm;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_denorm_loss;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_iec559;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_bounded;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_modulo;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::traps;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::tinyness_before;
  template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST std::float_round_style  numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::round_style;
  } // namespace std

#endif // FIXED_POINT_NEGATABLE_LIMITS_2015_08_24_HPP_
