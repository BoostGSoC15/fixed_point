///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2013 - 2016.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// This file contains fixed_point details.

#ifndef FIXED_POINT_DETAIL_2015_05_23_HPP_
  #define FIXED_POINT_DETAIL_2015_05_23_HPP_

  #include <cstdint>

  #include <boost/config.hpp>
  #include <boost/fixed_point/detail/fixed_point_detail_cstdfloat.hpp>

  #if (   !defined(BOOST_FIXED_POINT_FLOAT32_C)  \
       && !defined(BOOST_FIXED_POINT_FLOAT64_C)  \
       && !defined(BOOST_FIXED_POINT_FLOAT80_C)  \
       && !defined(BOOST_FIXED_POINT_FLOAT128_C))
    #error Configuration error: Sorry, fixed_point can not detect any IEEE-754 built-in floating-point types!
  #endif

  // Do not produce Doxygen indexing of items in namespace detail unless specifically required.
  // The section between \cond and \endcond can be included by adding its section label DETAIL
  // to the ENABLED_SECTIONS configuration option. 
  // If the section label is omitted, the section will be excluded from processing unconditionally.

  //! \cond DETAIL

  namespace boost { namespace fixed_point { namespace detail {

  template<typename UnsignedIntegralType>
  UnsignedIntegralType left_shift_helper(const UnsignedIntegralType& u, const int shift_count)
  {
    #if !defined(BOOST_FIXED_POINT_ENABLE_GMP_BACKENDS)

      static_assert(    (std::numeric_limits<UnsignedIntegralType>::is_integer == true)
                     && (std::numeric_limits<UnsignedIntegralType>::is_signed  == false),
                     "The UnsignedIntegralType for left shift must be an unsigned integral type.");

    #endif

    return ((shift_count > 0) ? UnsignedIntegralType(u << +shift_count)
                              : UnsignedIntegralType(u >> -shift_count));
  }

  template<typename UnsignedIntegralType>
  UnsignedIntegralType right_shift_helper(const UnsignedIntegralType& u, const int shift_count)
  {
    #if !defined(BOOST_FIXED_POINT_ENABLE_GMP_BACKENDS)

      static_assert(    (std::numeric_limits<UnsignedIntegralType>::is_integer == true)
                     && (std::numeric_limits<UnsignedIntegralType>::is_signed  == false),
                     "The UnsignedIntegralType for right shift must be an unsigned integral type.");

    #endif

    return ((shift_count > 0) ? UnsignedIntegralType(u >> +shift_count)
                              : UnsignedIntegralType(u << -shift_count));
  }

  #if !defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)

    template<const std::uint32_t BitCount,
             typename EnableType = void>
    struct integer_type_helper
    {
    private:
      static BOOST_CONSTEXPR_OR_CONST std::uint32_t bit_count_nearest_power_of_two =
        (BitCount <= std::uint32_t(UINT32_C(1) <<  7)) ? std::uint32_t(UINT32_C(1) <<  7) :
        (BitCount <= std::uint32_t(UINT32_C(1) <<  8)) ? std::uint32_t(UINT32_C(1) <<  8) :
        (BitCount <= std::uint32_t(UINT32_C(1) <<  9)) ? std::uint32_t(UINT32_C(1) <<  9) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 10)) ? std::uint32_t(UINT32_C(1) << 10) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 11)) ? std::uint32_t(UINT32_C(1) << 11) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 12)) ? std::uint32_t(UINT32_C(1) << 12) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 13)) ? std::uint32_t(UINT32_C(1) << 13) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 14)) ? std::uint32_t(UINT32_C(1) << 14) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 15)) ? std::uint32_t(UINT32_C(1) << 15) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 16)) ? std::uint32_t(UINT32_C(1) << 16) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 17)) ? std::uint32_t(UINT32_C(1) << 17) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 18)) ? std::uint32_t(UINT32_C(1) << 18) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 19)) ? std::uint32_t(UINT32_C(1) << 19) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 20)) ? std::uint32_t(UINT32_C(1) << 20) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 21)) ? std::uint32_t(UINT32_C(1) << 21) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 22)) ? std::uint32_t(UINT32_C(1) << 22) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 23)) ? std::uint32_t(UINT32_C(1) << 23) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 24)) ? std::uint32_t(UINT32_C(1) << 24) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 25)) ? std::uint32_t(UINT32_C(1) << 25) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 26)) ? std::uint32_t(UINT32_C(1) << 26) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 27)) ? std::uint32_t(UINT32_C(1) << 27) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 28)) ? std::uint32_t(UINT32_C(1) << 28) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 29)) ? std::uint32_t(UINT32_C(1) << 29) :
        (BitCount <= std::uint32_t(UINT32_C(1) << 30)) ? std::uint32_t(UINT32_C(1) << 30) :
                    (std::uint32_t(UINT32_C(1) << 31));

      #if defined(BOOST_FIXED_POINT_ENABLE_GMP_BACKENDS)

        typedef boost::multiprecision::gmp_int signed_integral_backend_type;
        typedef boost::multiprecision::gmp_int unsigned_integral_backend_type;

      #else

      typedef boost::multiprecision::cpp_int_backend<unsigned(bit_count_nearest_power_of_two),
                                                     unsigned(bit_count_nearest_power_of_two),
                                                     boost::multiprecision::signed_magnitude,
                                                     boost::multiprecision::unchecked,
                                                     void>
      signed_integral_backend_type;

      typedef boost::multiprecision::cpp_int_backend<unsigned(bit_count_nearest_power_of_two),
                                                     unsigned(bit_count_nearest_power_of_two),
                                                     boost::multiprecision::unsigned_magnitude,
                                                     boost::multiprecision::unchecked,
                                                     void>
      unsigned_integral_backend_type;

      #endif

    public:
      typedef boost::multiprecision::number<signed_integral_backend_type,
                                            boost::multiprecision::et_off> exact_signed_type;

      typedef boost::multiprecision::number<unsigned_integral_backend_type,
                                            boost::multiprecision::et_off> exact_unsigned_type;
    };

  #endif // !BOOST_FIXED_POINT_DISABLE_MULTIPRECISION

  #if defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)

    template<const std::uint32_t BitCount,
             typename EnableType = void>
    struct integer_type_helper
    {
    private:
      typedef signed long long   exact_signed_type;
      typedef unsigned long long exact_unsigned_type;
    };

  #endif // BOOST_FIXED_POINT_DISABLE_MULTIPRECISION

  template<const std::uint32_t BitCount>
  struct integer_type_helper<BitCount,
                             typename std::enable_if<(BitCount <= UINT32_C(8))>::type>
  {
    typedef std::int8_t  exact_signed_type;
    typedef std::uint8_t exact_unsigned_type;
  };

  template<const std::uint32_t BitCount>
  struct integer_type_helper<BitCount,
                             typename std::enable_if<   (BitCount >  UINT32_C( 8))
                                                     && (BitCount <= UINT32_C(16))>::type>
  {
    typedef std::int16_t  exact_signed_type;
    typedef std::uint16_t exact_unsigned_type;
  };

  template<const std::uint32_t BitCount>
  struct integer_type_helper<BitCount,
                             typename std::enable_if<   (BitCount >  UINT32_C(16))
                                                     && (BitCount <= UINT32_C(32))>::type>
  {
    typedef std::int32_t  exact_signed_type;
    typedef std::uint32_t exact_unsigned_type;
  };

  template<const std::uint32_t BitCount>
  struct integer_type_helper<BitCount,
                             typename std::enable_if<   (BitCount >  UINT32_C(32))
                                                     && (BitCount <= UINT32_C(64))>::type>
  {
    typedef std::int64_t  exact_signed_type;
    typedef std::uint64_t exact_unsigned_type;
  };

  #if !defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)

    template<const std::uint32_t BitCount,
             typename EnableType = void>
    struct float_type_helper
    {
    private:

      #if defined(BOOST_FIXED_POINT_ENABLE_GMP_BACKENDS)

        typedef boost::multiprecision::gmp_float<unsigned((static_cast<long long>(static_cast<long long>(BitCount) * 301LL) + 500LL) / 1000LL)> floating_point_backend_type;

      #else

        typedef boost::multiprecision::backends::cpp_bin_float<unsigned(BitCount),
                                                               boost::multiprecision::backends::digit_base_2>
        floating_point_backend_type;

      #endif

    public:
      typedef boost::multiprecision::number<floating_point_backend_type,
                                            boost::multiprecision::et_off> exact_float_type;
    };

  #endif // !BOOST_FIXED_POINT_DISABLE_MULTIPRECISION

  #if defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)

    template<const std::uint32_t BitCount,
             typename EnableType = void>
    struct float_type_helper
    {
      typedef long double exact_float_type;
    };

  #endif

  #if defined(BOOST_FIXED_POINT_FLOAT32_C)

    template<const std::uint32_t BitCount>
    struct float_type_helper<BitCount,
                             typename std::enable_if<(BitCount <= UINT32_C(24))>::type>
    {
      typedef boost::fixed_point::detail::float32_t exact_float_type;
    };

  #endif // BOOST_FIXED_POINT_FLOAT32_C

  #if defined(BOOST_FIXED_POINT_FLOAT64_C)

    // For 64-bit float, we have two cases.
    //
    // * !float32_t : float64_t covers bits  0...53
    // *  float32_t : float64_t covers bits 24...53

    template<const std::uint32_t BitCount>
    struct float_type_helper<BitCount,
    #if defined(BOOST_FIXED_POINT_FLOAT32_C)
                             typename std::enable_if<   (BitCount >  UINT32_C(24))
                                                     && (BitCount <= UINT32_C(53))>::type>
    #else
                             typename std::enable_if<   (BitCount <= UINT32_C(53))>::type>
    #endif
    {
      typedef boost::fixed_point::detail::float64_t exact_float_type;
    };

  #endif // BOOST_FIXED_POINT_FLOAT64_C

  #if defined(BOOST_FIXED_POINT_FLOAT80_C)

    // For 80-bit float, we have four cases (2^2 = 4). There are
    // two redundant cases.
    //
    // * !float32_t && !float64_t : float80_t covers bits  0...64
    // *  float32_t && !float64_t : float80_t covers bits 24...64
    // *  float32_t &&  float64_t : float80_t covers bits 53...64
    // * !float32_t &&  float64_t : float80_t covers bits 53...64

    template<const std::uint32_t BitCount>
    struct float_type_helper<BitCount,
    #if   (!defined(BOOST_FIXED_POINT_FLOAT32_C) && !defined(BOOST_FIXED_POINT_FLOAT64_C))
                             typename std::enable_if<   (BitCount <= UINT32_C(64))>::type>
    #elif ( defined(BOOST_FIXED_POINT_FLOAT32_C) && !defined(BOOST_FIXED_POINT_FLOAT64_C))
                             typename std::enable_if<   (BitCount >  UINT32_C(24))
                                                     && (BitCount <= UINT32_C(64))>::type>
    #else
                             typename std::enable_if<   (BitCount >  UINT32_C(53))
                                                     && (BitCount <= UINT32_C(64))>::type>
    #endif
    {
      typedef boost::fixed_point::detail::float80_t exact_float_type;
    };

  #endif // BOOST_FIXED_POINT_FLOAT80_C

  #if defined(BOOST_FIXED_POINT_FLOAT128_C)

    // For 128-bit float, we have eight cases (2^3 = 8). There are two groups
    // of redundancies. These contain two and four redundant results, respectively.
    //
    // * !float32_t && !float64_t && !float80_t : float128_t covers bits  0...113
    // *  float32_t && !float64_t && !float80_t : float128_t covers bits 24...113
    // * !float32_t &&  float64_t && !float80_t : float128_t covers bits 53...113
    // *  float32_t &&  float64_t && !float80_t : float128_t covers bits 53...113
    // * !float32_t && !float64_t &&  float80_t : float128_t covers bits 64...113
    // *  float32_t && !float64_t &&  float80_t : float128_t covers bits 64...113
    // * !float32_t &&  float64_t &&  float80_t : float128_t covers bits 64...113
    // *  float32_t &&  float64_t &&  float80_t : float128_t covers bits 64...113

    template<const std::uint32_t BitCount>
    struct float_type_helper<BitCount,
    #if   (!defined(BOOST_FIXED_POINT_FLOAT32_C) && !defined(BOOST_FIXED_POINT_FLOAT64_C) && !defined(BOOST_FIXED_POINT_FLOAT80_C))
                             typename std::enable_if<   (BitCount <= UINT32_C(113))>::type>
    #elif ( defined(BOOST_FIXED_POINT_FLOAT32_C) && !defined(BOOST_FIXED_POINT_FLOAT64_C) && !defined(BOOST_FIXED_POINT_FLOAT80_C))
                             typename std::enable_if<   (BitCount >  UINT32_C( 24))
                                                     && (BitCount <= UINT32_C(113))>::type>
    #elif ( defined(BOOST_FIXED_POINT_FLOAT64_C) && !defined(BOOST_FIXED_POINT_FLOAT80_C))
                             typename std::enable_if<   (BitCount >  UINT32_C( 53))
                                                     && (BitCount <= UINT32_C(113))>::type>
    #else
                             typename std::enable_if<   (BitCount >  UINT32_C( 64))
                                                     && (BitCount <= UINT32_C(113))>::type>
    #endif
    {
      typedef boost::fixed_point::detail::float128_t exact_float_type;
    };

  #endif // BOOST_FIXED_POINT_FLOAT128_C

  #if !defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)

    template<typename UnsignedIntegralType,
             typename FloatingPointType,
             typename EnableType = void>
    struct conversion_helper
    {
      static void convert_floating_point_to_unsigned_integer(const FloatingPointType& floating_point_source,
                                                             UnsignedIntegralType& unsigned_destination)
      {
        unsigned_destination = floating_point_source.template convert_to<UnsignedIntegralType>();
      }
    };

    template<typename UnsignedIntegralType,
             typename FloatingPointType>
    struct conversion_helper<UnsignedIntegralType,
                             FloatingPointType,
                             typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type>
    {
      static void convert_floating_point_to_unsigned_integer(const FloatingPointType& floating_point_source,
                                                             UnsignedIntegralType& unsigned_destination)
      {
        unsigned_destination = static_cast<UnsignedIntegralType>(floating_point_source);
      }
    };

    #if !defined(BOOST_FIXED_POINT_FLOAT80_C)

      // Here is a somewhat significant work-around for the conversion of
      // cpp_bin_float to uint64_t. It is used for cases when the digits
      // when float80_t is not available and we are trying to convert
      // a small multiprecision floating point type to uint64_t.

      // Say, for instsance, that float64_t is the largest width built-in
      // floating-point type. It only has, however, 53 bits of precision.
      // In this case, we might use a multiprecision type for precision
      // ranging from 54...64 bits such as cpp_bin_float<64, digit_base_2>.
      // When being converted to uint64_t in this precision range,
      // multiprecision does not yet handle this case. In particular,
      // see the TODO in the comment at line 1113 of cpp_bin_float.hpp
      // from Boost 1.58. Hence we need this work-around.

      template<typename FloatingPointType>
      struct conversion_helper<std::uint64_t,
                               FloatingPointType,
                               typename std::enable_if<std::is_floating_point<FloatingPointType>::value == false>::type>
      {
      private:
        typedef boost::fixed_point::detail::float_type_helper<UINT32_C(64)>::exact_float_type floating_point_type;

      public:
        static void convert_floating_point_to_unsigned_integer(const floating_point_type& floating_point_source,
                                                               std::uint64_t& unsigned_destination)
        {
          std::stringstream ss;

          ss << std::fixed << floating_point_source;

          std::string str(ss.str());

          const std::string::size_type position_of_dot = str.find(".");

          if(position_of_dot != std::string::npos)
          {
            str = str.substr(std::string::size_type(0U), position_of_dot);
          }

          unsigned_destination = boost::lexical_cast<std::uint64_t>(str);
        }
      };

    #endif // !BOOST_FIXED_POINT_FLOAT80_C

  #endif // !BOOST_FIXED_POINT_DISABLE_MULTIPRECISION

  #if defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)

    template<typename UnsignedIntegralType,
             typename FloatingPointType,
             typename EnableType = void>
    struct conversion_helper
    {
      static void convert_floating_point_to_unsigned_integer(const FloatingPointType& floating_point_source,
                                                             UnsignedIntegralType& unsigned_destination)
      {
        unsigned_destination = static_cast<UnsignedIntegralType>(floating_point_source);
      }
    };

  #endif // BOOST_FIXED_POINT_DISABLE_MULTIPRECISION

  template<typename UnsignedIntegralType>
  std::uint_fast16_t msb_helper(UnsignedIntegralType& u,
                                UnsignedIntegralType& mask,
                                const std::uint_fast16_t bit_count)
  {
    // Use O(log2[N]) binary-halving in an unrolled loop to find the msb.
    // The binary-halving search uses a recursive function call.

    static_assert(std::numeric_limits<UnsignedIntegralType>::is_signed == false,
                  "The UnsignedIntegralType for msb_meta_helper_nonconstant must be an unsigned (integral) type.");

    if(u < 2U)
    {
      return UINT32_C(0);
    }

    const UnsignedIntegralType hi_part = (u >> (bit_count / 2U));

    mask = (mask >> (bit_count / 2U));

    if((hi_part & mask) != 0)
    {
      u = hi_part;

      return (bit_count / 2U) + msb_helper(u, mask, std::uint_fast16_t(bit_count / 2U));
    }
    else
    {
      u = (u & mask);

      return msb_helper(u, mask, (bit_count / 2U));
    }
  }

  // Make a template specialization of msb_helper() for std::uint32_t.
  template<>
  std::uint_fast16_t msb_helper(std::uint32_t& u,
                                std::uint32_t&,
                                const std::uint_fast16_t)
  {
    std::uint_fast8_t r(0);

    // Use O(log2[N]) binary-halving in an unrolled loop to find the msb.
    if((u & UINT32_C(0xFFFF0000)) != UINT32_C(0)) { u >>= 16; r |= UINT8_C(16); }
    if((u & UINT32_C(0x0000FF00)) != UINT32_C(0)) { u >>=  8; r |= UINT8_C( 8); }
    if((u & UINT32_C(0x000000F0)) != UINT32_C(0)) { u >>=  4; r |= UINT8_C( 4); }
    if((u & UINT32_C(0x0000000C)) != UINT32_C(0)) { u >>=  2; r |= UINT8_C( 2); }
    if((u & UINT32_C(0x00000002)) != UINT32_C(0)) { u >>=  1; r |= UINT8_C( 1); }

    return std::uint_fast16_t(r);
  }

  // Make a template specialization of msb_helper() for std::uint16_t.
  template<>
  std::uint_fast16_t msb_helper(std::uint16_t& u,
                                std::uint16_t&,
                                const std::uint_fast16_t)
  {
    std::uint_fast8_t r(0);

    // Use O(log2[N]) binary-halving in an unrolled loop to find the msb.
    if((u & UINT16_C(0xFF00)) != UINT16_C(0)) { u >>= 8; r |= UINT8_C(8); }
    if((u & UINT16_C(0x00F0)) != UINT16_C(0)) { u >>= 4; r |= UINT8_C(4); }
    if((u & UINT16_C(0x000C)) != UINT16_C(0)) { u >>= 2; r |= UINT8_C(2); }
    if((u & UINT16_C(0x0002)) != UINT16_C(0)) { u >>= 1; r |= UINT8_C(1); }

    return std::uint_fast16_t(r);
  }

  // Make a template specialization of msb_helper() for std::uint8_t.
  template<>
  std::uint_fast16_t msb_helper(std::uint8_t& u,
                                std::uint8_t&,
                                const std::uint_fast16_t)
  {
    std::uint_fast8_t r(0);

    // Use O(log2[N]) binary-halving in an unrolled loop to find the msb.
    if((u & UINT8_C(0xF0)) != UINT8_C(0)) { u >>= 4; r |= UINT8_C(4); }
    if((u & UINT8_C(0x0C)) != UINT8_C(0)) { u >>= 2; r |= UINT8_C(2); }
    if((u & UINT8_C(0x02)) != UINT8_C(0)) { u >>= 1; r |= UINT8_C(1); }

    return std::uint_fast16_t(r);
  }

  template<typename ArithmeticType>
  ArithmeticType power_of_two_helper(int p2)
  {
    if(p2 == 0)
    {
      return ArithmeticType(1);
    }
    else if(p2 < 0)
    {
      return ArithmeticType(1) / power_of_two_helper<ArithmeticType>(-p2);
    }
    else
    {
      // The variable xn stores the binary powers of x.
      ArithmeticType the_result(((p2 % 2) != 0) ? ArithmeticType(2) : ArithmeticType(1));

      ArithmeticType xn(2);

      while((p2 /= 2) != 0)
      {
        // Square xn for each binary power.
        xn *= xn;

        const bool has_binary_power = ((p2 % 2) != 0);

        if(has_binary_power)
        {
          // Multiply the result with each binary power contained in the exponent.
          the_result *= xn;
        }
      }

      return the_result;
    }
  }

  template<typename UnsignedLargeType,
           typename UnsignedSmallType = typename integer_type_helper<std::numeric_limits<UnsignedLargeType>::digits / 2>::exact_unsigned_type>
  UnsignedSmallType lo_part(const UnsignedLargeType& lt)
  {
    static_assert(    (std::numeric_limits<UnsignedLargeType>::is_integer == true)
                    && (std::numeric_limits<UnsignedLargeType>::is_signed  == false),
                    "The UnsignedLargeType for lo_part must be an unsigned integral type.");

    static_assert(    (std::numeric_limits<UnsignedSmallType>::is_integer == true)
                    && (std::numeric_limits<UnsignedSmallType>::is_signed  == false),
                    "The UnsignedSmallType for lo_part must be an unsigned integral type.");

    return UnsignedSmallType(lt);
  }

  template<typename UnsignedLargeType,
           typename UnsignedSmallType = typename integer_type_helper<std::numeric_limits<UnsignedLargeType>::digits / 2>::exact_unsigned_type>
  UnsignedSmallType hi_part(const UnsignedLargeType& lt)
  {
    static_assert(    (std::numeric_limits<UnsignedLargeType>::is_integer == true)
                    && (std::numeric_limits<UnsignedLargeType>::is_signed  == false),
                    "The UnsignedLargeType for hi_part must be an unsigned integral type.");

    static_assert(    (std::numeric_limits<UnsignedSmallType>::is_integer == true)
                    && (std::numeric_limits<UnsignedSmallType>::is_signed  == false),
                    "The UnsignedSmallType for hi_part must be an unsigned integral type.");

    return UnsignedSmallType(lt >> std::numeric_limits<UnsignedSmallType>::digits);
  }

  template<typename UnsignedLargeType,
           typename UnsignedSmallType = typename integer_type_helper<std::numeric_limits<UnsignedLargeType>::digits / 2>::exact_unsigned_type>
  UnsignedLargeType make_large(const UnsignedSmallType& lo, const UnsignedSmallType& hi)
  {
    static_assert(    (std::numeric_limits<UnsignedLargeType>::is_integer == true)
                    && (std::numeric_limits<UnsignedLargeType>::is_signed  == false),
                    "The UnsignedLargeType for make_large must be an unsigned integral type.");

    static_assert(    (std::numeric_limits<UnsignedSmallType>::is_integer == true)
                    && (std::numeric_limits<UnsignedSmallType>::is_signed  == false),
                    "The UnsignedSmallType for make_large must be an unsigned integral type.");

    return (UnsignedLargeType(hi) << std::numeric_limits<UnsignedSmallType>::digits) | lo;
  }

  template<typename UnsignedSmallType,
           typename UnsignedHalfType = typename integer_type_helper<std::numeric_limits<UnsignedSmallType>::digits / 2>::exact_unsigned_type>
  void two_component_multiply(const UnsignedSmallType& u,
                              const UnsignedSmallType& v,
                                    UnsignedSmallType& result_lo,
                                    UnsignedSmallType& result_hi)
  {
    // Multiply u * v, where u and v are both unsigned
    // and both have 2^n bits. The result has 2*(2^n) bits.
    // The result is stored in a pair of two unsigned integers,
    // result_lo and result_hi.

    // Use an elementary school multiplication algorithm.

    // For example, multiply:
    //  uint32_t * uint32_t --> uint64_t result.

    // The result of the multiplication of (u * v) is:
    //   (u_hi_v_hi << n) + (uv_cross << (n/2)) + u_lo_v_lo,
    // where
    //   u_hi_v_hi = (u_hi * v_hi),
    // and
    //   uv_cross  = (u_lo * v_hi) + (u_hi * v_lo),
    // and
    //   u_lo_v_lo = (u_lo * v_lo).

    // Care is taken to properly handle shifts and carries.

    typedef UnsignedHalfType  local_unsigned_half_type;
    typedef UnsignedSmallType local_unsigned_small_type;

    BOOST_CONSTEXPR_OR_CONST int digits_half = std::numeric_limits<local_unsigned_half_type>::digits;

    const local_unsigned_half_type u_lo(static_cast<local_unsigned_half_type>(u));
    const local_unsigned_half_type v_lo(static_cast<local_unsigned_half_type>(v));

    const local_unsigned_half_type u_hi(static_cast<local_unsigned_half_type>(u >> digits_half));
    const local_unsigned_half_type v_hi(static_cast<local_unsigned_half_type>(v >> digits_half));

    const local_unsigned_small_type u_lo_v_lo(u_lo * local_unsigned_small_type(v_lo));
    const local_unsigned_small_type u_hi_v_lo(u_hi * local_unsigned_small_type(v_lo));
    const local_unsigned_small_type u_lo_v_hi(u_lo * local_unsigned_small_type(v_hi));

    const local_unsigned_small_type uv_cross_lo =
          local_unsigned_small_type(   local_unsigned_half_type (u_lo_v_hi))
        + local_unsigned_small_type(   local_unsigned_half_type (u_hi_v_lo)
                                    +  local_unsigned_small_type(u_lo_v_lo >> digits_half));

    const local_unsigned_small_type uv_cross_hi =
          local_unsigned_small_type(   local_unsigned_half_type (u_hi_v_lo   >> digits_half))
        + local_unsigned_small_type(   local_unsigned_half_type (u_lo_v_hi   >> digits_half)
                                    +  local_unsigned_small_type(uv_cross_lo >> digits_half));

    result_hi = local_unsigned_small_type(local_unsigned_small_type(u_hi * local_unsigned_small_type(v_hi)) + uv_cross_hi);

    result_lo = local_unsigned_small_type(  local_unsigned_small_type(uv_cross_lo << digits_half)
                                          | local_unsigned_half_type (u_lo_v_lo));
  }

  template<typename UnsignedSmallType,
           typename UnsignedHalfType = typename integer_type_helper<std::numeric_limits<UnsignedSmallType>::digits / 2>::exact_unsigned_type>
  void two_component_divide(const UnsignedSmallType& u_lo,
                            const UnsignedSmallType& u_hi,
                            const UnsignedSmallType& v_lo,
                                  UnsignedSmallType& result_lo,
                                  UnsignedSmallType& result_hi)
  {
    typedef UnsignedSmallType local_unsigned_small_type;
    typedef UnsignedHalfType  local_unsigned_half_type;

    // Divide the local_unsigned_small_type pair (u_lo, u_hi)
    // by v_lo and set the result in the local_unsigned_small_type
    // pair (result_lo, result_hi).

    // The Knuth long division algorithm is used.
    // The loop-ordering of Knuth's algorithm
    // has been reversed due to the little endian
    // data format used here.

    const std::array<local_unsigned_half_type, 4U> u =
    {{
      lo_part(u_lo),
      hi_part(u_lo),
      lo_part(u_hi),
      hi_part(u_hi),
    }};

    const std::array<local_unsigned_half_type, 2U> v =
    {{
      lo_part(v_lo),
      hi_part(v_lo)
    }};

    std::array<local_unsigned_half_type, 4U> result =
    {{
      local_unsigned_half_type(0U),
      local_unsigned_half_type(0U),
      local_unsigned_half_type(0U),
      local_unsigned_half_type(0U)
    }};

    // Handling zero numerator and/or zero denominator is done
    // in the function that calls this subroutine.

    if(v[1U] == 0U)
    {
      // The absolute value of the denominator has one single limb.
      // Use a simplified linear division algorithm.
      // The loop has been unrolled.

      local_unsigned_small_type lt = u[3U];

      result[3U] = u[3U] / v[0U];

      lt          = u[2U] + local_unsigned_small_type(local_unsigned_small_type(lt - local_unsigned_small_type(local_unsigned_small_type(v[0U]) * result[3U])) << std::numeric_limits<local_unsigned_half_type>::digits);
      result[2U]  = lo_part(local_unsigned_small_type(lt / v[0U]));

      lt          = u[1U] + local_unsigned_small_type(local_unsigned_small_type(lt - local_unsigned_small_type(local_unsigned_small_type(v[0U]) * result[2U])) << std::numeric_limits<local_unsigned_half_type>::digits);
      result[1U]  = lo_part(local_unsigned_small_type(lt / v[0U]));

      lt          = u[0U] + local_unsigned_small_type(local_unsigned_small_type(lt - local_unsigned_small_type(local_unsigned_small_type(v[0U]) * result[1U])) << std::numeric_limits<local_unsigned_half_type>::digits);
      result[0U]  = lo_part(local_unsigned_small_type(lt / v[0U]));

      result_lo = make_large<local_unsigned_small_type>(result[0U], result[1U]);
      result_hi = make_large<local_unsigned_small_type>(result[2U], result[3U]);

      return;
    }

    // Calculate the number of significant limbs in u.
    std::uint_fast8_t sig_limbs_u = 4U;

    for(std::uint_fast8_t i = std::uint_fast8_t(4U - 1U); (std::int_fast8_t(i) >= std::int_fast8_t(0)) && (u[i] == 0U); --i)
    {
      --sig_limbs_u;
    }

    // The result of the division is zero if the denominator
    // is larger than the numerator.

    // At this point in the subroutine, we know that v has
    // exactly two significant limbs.

    bool b_zero = false;

    if(sig_limbs_u < 2U)
    {
      b_zero = true;
    }
    else if(sig_limbs_u == 2U)
    {
      // Check the highest non-zero limb for zero result.
      // This check is equivalent to u < v.

      if(u[sig_limbs_u - 1U] < v[1U])
      {
        b_zero = true;
      }
      else if(u[sig_limbs_u - 1U] == v[1U])
      {
        // In this case, the numerator is equal to the denominator and
        // the result of the division is one with remainder of zero.
        result_lo = 1U;
        result_hi = 0U;

        return;
      }
    }

    if(b_zero)
    {
      // The result is zero.
      result_lo = 0U;
      result_hi = 0U;

      return;
    }

    // Now use the Knuth long division algorithm.

    // Compute the normalization factor d.
    const local_unsigned_half_type d_norm = lo_part(local_unsigned_small_type(local_unsigned_small_type(1) << std::numeric_limits<local_unsigned_half_type>::digits) / (local_unsigned_small_type(v[1U]) + 1U));

    // Step D1(b). Normalize u -> u * d = uu.
    // Note the added digit in uu and that the data uu
    // are not initialized.
    std::array<local_unsigned_half_type, 4U + 1U> uu;

    if(d_norm == local_unsigned_half_type(1U))
    {
      // The normalization is one.
      // In this case uu = u.
      std::copy(u.cbegin(), u.cend(), uu.begin());

      uu.back() = local_unsigned_half_type(0U);
    }
    else
    {
      // Multiply u by d.
      local_unsigned_half_type carry(0U);

      std::uint_fast8_t i;

      for(i = 0U; i < sig_limbs_u; ++i)
      {
        const local_unsigned_small_type lt(local_unsigned_small_type(u[i] * local_unsigned_small_type(d_norm)) + carry);

        uu[i] = lo_part(lt);
        carry = hi_part(lt);
      }

      uu[i] = carry;
    }

    // Step D1(c). Normalize v -> v * d = vv.
    // The data of vv are not initialized.
    std::array<local_unsigned_half_type, 2U> vv;

    if(d_norm == local_unsigned_half_type(1U))
    {
      // The normalization is one: In this case vv = v.
      std::copy(v.cbegin(), v.cend(), vv.begin());
    }
    else
    {
      // Multiply v by d.
      // The loop has been unrolled.

      local_unsigned_half_type carry(0U);

      local_unsigned_small_type lt;

      lt = local_unsigned_small_type(v[0U] * local_unsigned_small_type(d_norm)) + carry;

      vv[0U] = lo_part(lt);
      carry  = hi_part(lt);

      lt = local_unsigned_small_type(v[1U] * local_unsigned_small_type(d_norm)) + carry;

      vv[1U] = lo_part(lt);
    }

    // D2: Initialize j.
    // D7: Loop on j from 0 to m.
    for(std::uint_fast8_t j = 0U; j <= (sig_limbs_u - 2U); ++j)
    {
      // * D3 [Calculate q^]
      //    if u[j] == v[1]
      //      set q^ = b - 1
      //    else
      //      set q^ = (u[j] * b + u[j+1]) / v[1]

      const std::uint_fast8_t uj(sig_limbs_u - j);

      const local_unsigned_small_type u_j_j1((local_unsigned_small_type(uu[uj]) << std::numeric_limits<local_unsigned_half_type>::digits) + uu[uj - 1U]);

      local_unsigned_small_type q_hat =
        ((uu[uj] == vv[1U]) ? local_unsigned_small_type((std::numeric_limits<local_unsigned_half_type>::max)())
                            : local_unsigned_small_type(u_j_j1 / vv[1U]));

      // Decrease q^ if necessary. In this step, q^ must be decreased
      // if the expression "(u[uj] * b + u[uj - 1] - q^ * v[vj0 - 1]) * b"
      // exceeds the size of local_unsigned_small_type.
      for(;;)
      {
        local_unsigned_small_type lt(u_j_j1 - local_unsigned_small_type(q_hat * vv[1U]));

        if(hi_part<local_unsigned_small_type, local_unsigned_half_type>(lt) != 0U)
        {
          break;
        }

        if(local_unsigned_small_type(vv[0U] * q_hat) <= local_unsigned_small_type(local_unsigned_small_type(lt << std::numeric_limits<local_unsigned_half_type>::digits) + uu[uj - 2U]))
        {
          break;
        }

        --q_hat;
      }

      // D4: Multiply and subtract.
      // Replace u[j..j+n] by u[j..j+n] - q^ * v[1..n].

      // Set nv = q^ * (v[1..n]).

      std::array<local_unsigned_half_type, 2U + 1U> nv;

      {
        // The loop has been unrolled.

        local_unsigned_half_type carry(0U);

        local_unsigned_small_type lt;

        lt     = local_unsigned_small_type(vv[0U] * q_hat) + carry;
        nv[0U] = lo_part(lt);
        carry  = hi_part(lt);

        lt     = local_unsigned_small_type(vv[1U] * q_hat) + carry;
        nv[1U] = lo_part(lt);
        carry  = hi_part(lt);

        nv[2U] = carry;
      }

      // Subtract nv[0..n] from u[j..j+n].

      local_unsigned_half_type borrow(0U);

      {
        // The loop has been unrolled.

        local_unsigned_small_type lt;

        lt          = local_unsigned_small_type(local_unsigned_small_type(uu[uj - 2U]) - nv[0U]) - borrow;
        uu[uj - 2U] = lo_part(lt);
        borrow      = ((hi_part(lt) != 0U) ? 1U : 0U);

        lt          = local_unsigned_small_type(local_unsigned_small_type(uu[uj - 1U]) - nv[1U]) - borrow;
        uu[uj - 1U] = lo_part(lt);
        borrow      = ((hi_part(lt) != 0U) ? 1U : 0U);

        lt          = local_unsigned_small_type(local_unsigned_small_type(uu[uj - 0U]) - nv[2U]) - borrow;
        uu[uj - 0U] = lo_part(lt);
        borrow      = ((hi_part(lt) != 0U) ? 1U : 0U);
      }

      // Get the result data.
      result[(sig_limbs_u - 2U) - j] = lo_part(q_hat);

      // D5: Test the remainder
      //     Set the result value: Set result.m_data[m - j] = q^
      //     If u[j] < 0, (the value of borrow is non-zero),
      //     then step D6 must be performed.
      if(borrow != 0U)
      {
        // D6: Add back
        //     Add v[1..n] back to u[j..j+n]
        //     and decrease the result by 1.

        // The loop has been unrolled.

        local_unsigned_half_type carry(0U);

        local_unsigned_small_type lt;

        lt          = local_unsigned_small_type(local_unsigned_small_type(uu[uj - 2U]) + nv[0U]) + carry;
        uu[uj - 2U] = lo_part(lt);
        carry       = ((hi_part(lt) != 0U) ? 1U : 0U);

        lt          = local_unsigned_small_type(local_unsigned_small_type(uu[uj - 1U]) + nv[1U]) + carry;
        uu[uj - 1U] = lo_part(lt);
        carry       = ((hi_part(lt) != 0U) ? 1U : 0U);

        lt          = local_unsigned_small_type(local_unsigned_small_type(uu[uj - 0U]) + nv[2U]) + carry;
        uu[uj - 0U] = lo_part(lt);
        carry       = ((hi_part(lt) != 0U) ? 1U : 0U);

        uu[(sig_limbs_u - 2U) - j] += carry;

        --result[(sig_limbs_u - 2U) - j];
      }
    }

    // Compute the low and high parts of the result.
    result_lo = make_large<local_unsigned_small_type>(result[0U], result[1U]);
    result_hi = make_large<local_unsigned_small_type>(result[2U], result[3U]);
  }

  } } } // namespace boost::fixed_point::detail
  //! \endcond // DETAIL

#endif // FIXED_POINT_DETAIL_2015_05_23_HPP_
