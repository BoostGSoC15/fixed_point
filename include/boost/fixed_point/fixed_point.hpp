///////////////////////////////////////////////////////////////////////////////
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Copyright Christopher Kormanyos 2014 - 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// This file is a partial reference implementation for the proposed
// "C++ binary fixed-point arithmetic" as specified in N3352.
// See: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html

#ifndef FIXED_POINT_2015_03_06_HPP_
  #define FIXED_POINT_2015_03_06_HPP_

  #include <algorithm>
  #include <cmath>
  #include <iomanip>
  #include <limits>
  #include <ostream>
  #include <sstream>
  #include <string>
  #include <type_traits>

  #include <boost/cstdfloat.hpp>
  #include <boost/cstdint.hpp>
  #include <boost/multiprecision/cpp_bin_float.hpp>
  #include <boost/multiprecision/cpp_int.hpp>

  namespace boost { namespace fixed_point {

  namespace round
  {
    typedef enum enum_round_type
    {
      fastest,       // Speed is more important than the choice in value.
      negative,      // Round towards negative infinity. This mode is useful in interval arithmetic.
      truncated,     // Round towards zero. This mode is useful in implementing integral arithmetic.
      positive,      // Round towards positive infinity. This mode is useful in interval arithmetic.
      classic,       // Round towards the nearest value, but exactly-half values are rounded towards maximum magnitude. This mode is the standard school algorithm.
      nearest_even,  // Round towards the nearest value, but exactly-half values are rounded towards even values. This mode has more balance than the classic mode.
      nearest_odd,   // Round towards the nearest value, but exactly-half values are rounded towards odd values. This mode has as much balance as the near_even mode, but preserves more information.
    }
    round_type;
  }

  namespace overflow
  {
    typedef enum enum_overflow_type
    {
      impossible, // Programmer analysis of the program has determined that overflow cannot occur. Uses of this mode should be accompanied by an argument supporting the conclusion.
      undefined,  // Programmers are willing to accept undefined behavior in the event of an overflow.
      modulus,    // The assigned value is the dynamic value mod the range of the variable. This mode makes sense only with unsigned numbers. It is useful for angular measures.
      saturate,   // If the dynamic value exceeds the range of the variable, assign the nearest representable value.
      exception   // If the dynamic value exceeds the range of the variable, throw an exeception of type std::overflow_error.
    }
    overflow_type;
  }
  } } // namespace boost::fixed_point

  // Forward declaration of the negatable class.
  namespace boost { namespace fixed_point {
    template<const int integral_range,
             const int decimal_resolution,
             const fixed_point::round::round_type round_mode,
             const fixed_point::overflow::overflow_type overflow_mode>
    class negatable;
  } }
  // namespace boost::fixed_point

  namespace std
  {
    // Forward declaration of the specialization of std::numeric_limits<negatable>.
    template<const int integral_range,
             const int decimal_resolution,
             const boost::fixed_point::round::round_type round_mode,
             const boost::fixed_point::overflow::overflow_type overflow_mode>
    class numeric_limits<boost::fixed_point::negatable<integral_range, decimal_resolution, round_mode, overflow_mode> >;
  }

  namespace boost { namespace fixed_point {

  namespace detail
  {
    namespace mp = boost::multiprecision;

    template<const unsigned bit_count> struct integer_type_helper
    {
      typedef detail::mp::number<mp::cpp_int_backend<bit_count,
                                                     bit_count,
                                                     detail::mp::signed_magnitude,
                                                     detail::mp::unchecked,
                                                     void> > exact_signed_type;

      typedef detail::mp::number<mp::cpp_int_backend<bit_count,
                                                     bit_count,
                                                     detail::mp::unsigned_magnitude,
                                                     detail::mp::unchecked,
                                                     void> > exact_unsigned_type;
    };

    template<> struct integer_type_helper< 0U> { typedef boost::int8_t  exact_signed_type; typedef boost::uint8_t  exact_unsigned_type; };
    template<> struct integer_type_helper< 1U> { typedef boost::int8_t  exact_signed_type; typedef boost::uint8_t  exact_unsigned_type; };
    template<> struct integer_type_helper< 2U> { typedef boost::int8_t  exact_signed_type; typedef boost::uint8_t  exact_unsigned_type; };
    template<> struct integer_type_helper< 3U> { typedef boost::int8_t  exact_signed_type; typedef boost::uint8_t  exact_unsigned_type; };
    template<> struct integer_type_helper< 4U> { typedef boost::int8_t  exact_signed_type; typedef boost::uint8_t  exact_unsigned_type; };
    template<> struct integer_type_helper< 5U> { typedef boost::int8_t  exact_signed_type; typedef boost::uint8_t  exact_unsigned_type; };
    template<> struct integer_type_helper< 6U> { typedef boost::int8_t  exact_signed_type; typedef boost::uint8_t  exact_unsigned_type; };
    template<> struct integer_type_helper< 7U> { typedef boost::int8_t  exact_signed_type; typedef boost::uint8_t  exact_unsigned_type; };
    template<> struct integer_type_helper< 8U> { typedef boost::int8_t  exact_signed_type; typedef boost::uint8_t  exact_unsigned_type; };

    template<> struct integer_type_helper< 9U> { typedef boost::int16_t exact_signed_type; typedef boost::uint16_t exact_unsigned_type; };
    template<> struct integer_type_helper<10U> { typedef boost::int16_t exact_signed_type; typedef boost::uint16_t exact_unsigned_type; };
    template<> struct integer_type_helper<11U> { typedef boost::int16_t exact_signed_type; typedef boost::uint16_t exact_unsigned_type; };
    template<> struct integer_type_helper<12U> { typedef boost::int16_t exact_signed_type; typedef boost::uint16_t exact_unsigned_type; };
    template<> struct integer_type_helper<13U> { typedef boost::int16_t exact_signed_type; typedef boost::uint16_t exact_unsigned_type; };
    template<> struct integer_type_helper<14U> { typedef boost::int16_t exact_signed_type; typedef boost::uint16_t exact_unsigned_type; };
    template<> struct integer_type_helper<15U> { typedef boost::int16_t exact_signed_type; typedef boost::uint16_t exact_unsigned_type; };
    template<> struct integer_type_helper<16U> { typedef boost::int16_t exact_signed_type; typedef boost::uint16_t exact_unsigned_type; };

    template<> struct integer_type_helper<17U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<18U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<19U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<20U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<21U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<22U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<23U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<24U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<25U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<26U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<27U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<28U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<29U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<30U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<31U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };
    template<> struct integer_type_helper<32U> { typedef boost::int32_t exact_signed_type; typedef boost::uint32_t exact_unsigned_type; };

    template<> struct integer_type_helper<33U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<34U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<35U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<36U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<37U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<38U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<39U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<40U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<41U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<42U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<43U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<44U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<45U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<46U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<47U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<48U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<49U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<50U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<51U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<52U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<53U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<54U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<55U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<56U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<57U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<58U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<59U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<60U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<61U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<62U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<63U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };
    template<> struct integer_type_helper<64U> { typedef boost::int64_t exact_signed_type; typedef boost::uint64_t exact_unsigned_type; };

    template<const unsigned bit_count>
    struct float_type_helper
    {
      typedef
      detail::mp::number<detail::mp::backends::cpp_bin_float<bit_count,
                                                             detail::mp::backends::digit_base_2> >
      exact_float_type;
    };

    template<> struct float_type_helper< 0U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper< 1U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper< 2U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper< 3U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper< 4U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper< 5U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper< 6U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper< 7U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper< 8U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper< 9U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<10U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<11U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<12U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<13U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<14U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<15U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<16U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<17U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<18U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<19U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<20U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<21U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<22U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<23U> { typedef boost::float32_t exact_float_type; };
    template<> struct float_type_helper<24U> { typedef boost::float32_t exact_float_type; };

    template<> struct float_type_helper<25U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<26U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<27U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<28U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<29U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<30U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<31U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<32U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<33U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<34U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<35U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<36U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<37U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<38U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<39U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<40U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<41U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<42U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<43U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<44U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<45U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<46U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<47U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<48U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<49U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<50U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<51U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<52U> { typedef boost::float64_t exact_float_type; };
    template<> struct float_type_helper<53U> { typedef boost::float64_t exact_float_type; };

    template<typename integral_source_type,
             typename other_destination_type>
    other_destination_type convert_to(const integral_source_type& source)
    {
      return static_cast<other_destination_type>(source);
    }

    template<typename arithmetic_type,
             const int radix_split>
    struct radix_split_maker
    {
      static arithmetic_type value()
      {
        // TBD: Store the result as a static variable
        // that only needs to be computed *once*.

        // The variable xn stores the binary powers of x.
        arithmetic_type result(((radix_split % 2) != 0) ? arithmetic_type(2) : arithmetic_type(1));

        arithmetic_type xn(2);

        int p2 = radix_split;

        while((p2 /= 2) != 0)
        {
          // Square xn for each binary power.
          xn *= xn;

          const bool has_binary_power = ((p2 % 2) != 0);

          if(has_binary_power)
          {
            // Multiply the result with each binary power contained in the exponent.
            result *= xn;
          }
        }

        return result;
      }
    };

    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type,  0> { static arithmetic_type value() { return arithmetic_type(UINT32_C(         1)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type,  1> { static arithmetic_type value() { return arithmetic_type(UINT32_C(         2)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type,  2> { static arithmetic_type value() { return arithmetic_type(UINT32_C(         4)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type,  3> { static arithmetic_type value() { return arithmetic_type(UINT32_C(         8)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type,  4> { static arithmetic_type value() { return arithmetic_type(UINT32_C(        16)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type,  5> { static arithmetic_type value() { return arithmetic_type(UINT32_C(        32)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type,  6> { static arithmetic_type value() { return arithmetic_type(UINT32_C(        64)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type,  7> { static arithmetic_type value() { return arithmetic_type(UINT32_C(       128)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type,  8> { static arithmetic_type value() { return arithmetic_type(UINT32_C(       256)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type,  9> { static arithmetic_type value() { return arithmetic_type(UINT32_C(       512)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 10> { static arithmetic_type value() { return arithmetic_type(UINT32_C(      1024)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 11> { static arithmetic_type value() { return arithmetic_type(UINT32_C(      2048)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 12> { static arithmetic_type value() { return arithmetic_type(UINT32_C(      4096)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 13> { static arithmetic_type value() { return arithmetic_type(UINT32_C(      8192)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 14> { static arithmetic_type value() { return arithmetic_type(UINT32_C(     16384)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 15> { static arithmetic_type value() { return arithmetic_type(UINT32_C(     32768)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 16> { static arithmetic_type value() { return arithmetic_type(UINT32_C(     65536)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 17> { static arithmetic_type value() { return arithmetic_type(UINT32_C(    131072)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 18> { static arithmetic_type value() { return arithmetic_type(UINT32_C(    262144)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 19> { static arithmetic_type value() { return arithmetic_type(UINT32_C(    524288)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 20> { static arithmetic_type value() { return arithmetic_type(UINT32_C(   1048576)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 21> { static arithmetic_type value() { return arithmetic_type(UINT32_C(   2097152)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 22> { static arithmetic_type value() { return arithmetic_type(UINT32_C(   4194304)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 23> { static arithmetic_type value() { return arithmetic_type(UINT32_C(   8388608)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 24> { static arithmetic_type value() { return arithmetic_type(UINT32_C(  16777216)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 25> { static arithmetic_type value() { return arithmetic_type(UINT32_C(  33554432)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 26> { static arithmetic_type value() { return arithmetic_type(UINT32_C(  67108864)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 27> { static arithmetic_type value() { return arithmetic_type(UINT32_C( 134217728)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 28> { static arithmetic_type value() { return arithmetic_type(UINT32_C( 268435456)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 29> { static arithmetic_type value() { return arithmetic_type(UINT32_C( 536870912)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 30> { static arithmetic_type value() { return arithmetic_type(UINT32_C(1073741824)); } };
    template<typename arithmetic_type> struct radix_split_maker<arithmetic_type, 31> { static arithmetic_type value() { return arithmetic_type(UINT32_C(2147483648)); } };
  }
  // namespace boost::fixed_point::detail

  // We will now begin the implementation of the negatable class.
  template<const int integral_range,
           const int decimal_resolution,
           const round::round_type round_mode = round::fastest,
           const overflow::overflow_type overflow_mode = overflow::undefined>
  class negatable
  {
  private:
    static const int range      = integral_range - decimal_resolution;
    static const int resolution = decimal_resolution;

    static_assert( resolution < 0,
                  "Error: the negatable class resolution must be fractional (negative).");
    static_assert(-resolution < range - 1,
                  "Error: the negatable class resolution exceeds the available range.");

    template<typename T>
    void print_bits (T num)
    {
      std::string ans = "";
      size_t bits = range;
      T mask = T(1);
      for (int i = 0; i < bits; i++)
      {
        if (num & mask) ans+="1";
        else ans+="0";
        mask = mask << 1;
      }
      std::reverse (ans.begin (), ans.end ());
      std::cout<<ans<<"\n";
    }

  public:
    typedef typename detail::integer_type_helper<range>::exact_signed_type value_type;

    negatable() : data() { }

    template<typename signed_integral_type>
    negatable(const signed_integral_type& n,
              const typename std::enable_if<   std::is_same<char,       signed_integral_type>::value
                                            || std::is_same<short,      signed_integral_type>::value
                                            || std::is_same<int,        signed_integral_type>::value
                                            || std::is_same<long,       signed_integral_type>::value
                                            || std::is_same<long long,  signed_integral_type>::value
                                            || std::is_same<value_type, signed_integral_type>::value>::type* = nullptr) : data(n * radix_split_value<value_type>())
    {
      std::cout<<typeid(signed_integral_type).name() <<"\n";
      std::cout<<sizeof(signed_integral_type)<<"\n";
      std::cout<<typeid(value_type).name() <<"\n";
      std::cout<<sizeof(value_type)<<std::endl;
      std::cout<<data<<"\n";
      print_bits(data);
    }

    template<typename unsigned_integral_type>
    negatable(const unsigned_integral_type& u,
              const typename std::enable_if<   std::is_same<unsigned char,      unsigned_integral_type>::value
                                            || std::is_same<unsigned short,     unsigned_integral_type>::value
                                            || std::is_same<unsigned int,       unsigned_integral_type>::value
                                            || std::is_same<unsigned long,      unsigned_integral_type>::value
                                            || std::is_same<unsigned long long, unsigned_integral_type>::value>::type* = nullptr) : data(value_type(u) << radix_split) { }

    template<typename floating_point_type>
    negatable(const floating_point_type& f,
              const typename std::enable_if<   std::is_same<float,       floating_point_type>::value
                                            || std::is_same<double,      floating_point_type>::value
                                            || std::is_same<long double, floating_point_type>::value>::type* = nullptr) : data(value_type(f * radix_split_value<floating_point_type>()))
    {
      std::cout<<typeid(floating_point_type).name() <<"\n";
      std::cout<<sizeof(floating_point_type)<<"\n";
      std::cout<<typeid(value_type).name() <<"\n";
      std::cout<<sizeof(value_type)<<std::endl;
      std::cout<<data<<"\n";
      print_bits(data);
    }

    negatable(const negatable& v) : data(v.data) { }

    ~negatable() { }

    negatable& operator=(const negatable& v)
    {
      if(this != (&v)) { data = v.data; }

      return *this;
    }

    negatable& operator=(const char& n)               { data = value_type(n * radix_split_value<value_type>()); return *this; }
    negatable& operator=(const short& n)              { data = value_type(n * radix_split_value<value_type>()); return *this; }
    negatable& operator=(const int& n)                { data = value_type(n * radix_split_value<value_type>()); return *this; }
    negatable& operator=(const long& n)               { data = value_type(n * radix_split_value<value_type>()); return *this; }
    negatable& operator=(const long long& n)          { data = value_type(n * radix_split_value<value_type>()); return *this; }

    negatable& operator=(const unsigned char& u)      { data = value_type(u) << radix_split; return *this; }
    negatable& operator=(const unsigned short& u)     { data = value_type(u) << radix_split; return *this; }
    negatable& operator=(const unsigned int& u)       { data = value_type(u) << radix_split; return *this; }
    negatable& operator=(const unsigned long& u)      { data = value_type(u) << radix_split; return *this; }
    negatable& operator=(const unsigned long long& u) { data = value_type(u) << radix_split; return *this; }

    negatable& operator=(const float& f)              { data = value_type(f * radix_split_value<float>()); return *this; }
    negatable& operator=(const double& f)             { data = value_type(f * radix_split_value<double>()); return *this; }
    negatable& operator=(const long double& f)        { data = value_type(f * radix_split_value<long double>()); return *this; }

    negatable& operator++()   { data += value_type(unsigned_small_type(1) << radix_split); return *this; }
    negatable& operator--()   { data -= value_type(unsigned_small_type(1) << radix_split); return *this; }

    negatable operator++(int) { const negatable tmp(*this); data += value_type(unsigned_small_type(1) << radix_split); return tmp; }
    negatable operator--(int) { const negatable tmp(*this); data -= value_type(unsigned_small_type(1) << radix_split); return tmp; }

    negatable& operator+=(const negatable& v)
    {
      if(is_quiet_nan(*this) || is_quiet_nan(v))
      {
        data = value_quiet_nan().data;
        return *this;
      }

      if(is_infinity(*this))
      {
        return *this;
      }

      if(is_infinity(v))
      {
        data = v.data;
      }
      else
      {
        const bool has_potential_overflow = ((data > 0) && (v.data > 0));

        data += v.data;

        if(has_potential_overflow && ((data > value_max().data) || (data < 0)))
        {
          data = value_infinity().data;
        }
      }

      return *this;
    }

    negatable& operator-=(const negatable& v)
    {
      if(is_quiet_nan(*this) || is_quiet_nan(v))
      {
        data = value_quiet_nan().data;
        return *this;
      }

      if(is_infinity(*this))
      {
        return *this;
      }

      if(is_infinity(v))
      {
        data = -v.data;
      }
      else
      {
        const bool has_potential_overflow = ((data < 0) && (v.data < 0));

        data -= v.data;

        if(has_potential_overflow && ((data < -value_max().data) || (data > 0)))
        {
          data = value_infinity().data;
        }
      }

      return *this;
    }

    negatable& operator*=(const negatable& v)
    {
      const bool u_is_neg      = (  data < 0);
      const bool v_is_neg      = (v.data < 0);
      const bool result_is_neg = (u_is_neg != v_is_neg);

      if(is_quiet_nan(*this) || is_quiet_nan(v))
      {
        data = value_quiet_nan().data;
        return *this;
      }

      if(is_infinity(*this) || is_infinity(v))
      {
        data = value_infinity().data;
      }
      else
      {
        unsigned_large_type result((!u_is_neg) ? data : -data);

        result *= ((!v_is_neg) ? unsigned_large_type(v.data) : unsigned_large_type(-v.data));

        result >>= radix_split;

        if(result > unsigned_large_type(value_max().data))
        {
          data = value_infinity().data;
        }
        else
        {
          data = detail::convert_to<unsigned_large_type, value_type>(result);
        }
      }

      if(result_is_neg) { data = -data; }

      return *this;
    }

    negatable& operator/=(const negatable& v)
    {
      const bool u_is_neg      = (  data < 0);
      const bool v_is_neg      = (v.data < 0);
      const bool result_is_neg = (u_is_neg != v_is_neg);

      if(is_quiet_nan(*this) || is_quiet_nan(v))
      {
        data = value_quiet_nan().data;
        return *this;
      }

      if(is_infinity(*this))
      {
        data = (result_is_neg ? value_infinity().data : -value_infinity().data);
        return *this;
      }

      if(v.data == 0)
      {
        data = (u_is_neg ? value_infinity().data : -value_infinity().data);
        return *this;
      }

      if(is_infinity(v))
      {
        data = 0;
      }
      else
      {
        unsigned_large_type result((!u_is_neg) ? data : -data);

        result <<= radix_split;

        result /= ((!v_is_neg) ? unsigned_large_type(v.data) : unsigned_large_type(-v.data));

        if(result > unsigned_large_type(value_max().data))
        {
          data = value_infinity().data;
        }
        else
        {
          data = detail::convert_to<unsigned_large_type, value_type>(result);
        }

        if(result_is_neg) { data = -data; }
      }

      return *this;
    }

    negatable& operator+=(const char& n)               { return (*this) += negatable(n); }
    negatable& operator+=(const short& n)              { return (*this) += negatable(n); }
    negatable& operator+=(const int& n)                { return (*this) += negatable(n); }
    negatable& operator+=(const long& n)               { return (*this) += negatable(n); }
    negatable& operator+=(const long long& n)          { return (*this) += negatable(n); }
    negatable& operator+=(const unsigned char& u)      { return (*this) += negatable(u); }
    negatable& operator+=(const unsigned short& u)     { return (*this) += negatable(u); }
    negatable& operator+=(const unsigned int& u)       { return (*this) += negatable(u); }
    negatable& operator+=(const unsigned long& u)      { return (*this) += negatable(u); }
    negatable& operator+=(const unsigned long long& u) { return (*this) += negatable(u); }
    negatable& operator+=(const float& f)              { return (*this) += negatable(f); }
    negatable& operator+=(const double& f)             { return (*this) += negatable(f); }
    negatable& operator+=(const long double& f)        { return (*this) += negatable(f); }

    negatable& operator-=(const char& n)               { return (*this) -= negatable(n); }
    negatable& operator-=(const short& n)              { return (*this) -= negatable(n); }
    negatable& operator-=(const int& n)                { return (*this) -= negatable(n); }
    negatable& operator-=(const long& n)               { return (*this) -= negatable(n); }
    negatable& operator-=(const long long& n)          { return (*this) -= negatable(n); }
    negatable& operator-=(const unsigned char& u)      { return (*this) -= negatable(u); }
    negatable& operator-=(const unsigned short& u)     { return (*this) -= negatable(u); }
    negatable& operator-=(const unsigned int& u)       { return (*this) -= negatable(u); }
    negatable& operator-=(const unsigned long& u)      { return (*this) -= negatable(u); }
    negatable& operator-=(const unsigned long long& u) { return (*this) -= negatable(u); }
    negatable& operator-=(const float& f)              { return (*this) -= negatable(f); }
    negatable& operator-=(const double& f)             { return (*this) -= negatable(f); }
    negatable& operator-=(const long double& f)        { return (*this) -= negatable(f); }

    negatable& operator*=(const char& n)               { return (*this) *= negatable(n); }
    negatable& operator*=(const short& n)              { return (*this) *= negatable(n); }
    negatable& operator*=(const int& n)                { return (*this) *= negatable(n); }
    negatable& operator*=(const long& n)               { return (*this) *= negatable(n); }
    negatable& operator*=(const long long& n)          { return (*this) *= negatable(n); }
    negatable& operator*=(const unsigned char& u)      { return (*this) *= negatable(u); }
    negatable& operator*=(const unsigned short& u)     { return (*this) *= negatable(u); }
    negatable& operator*=(const unsigned int& u)       { return (*this) *= negatable(u); }
    negatable& operator*=(const unsigned long& u)      { return (*this) *= negatable(u); }
    negatable& operator*=(const unsigned long long& u) { return (*this) *= negatable(u); }
    negatable& operator*=(const float& f)              { return (*this) *= negatable(f); }
    negatable& operator*=(const double& f)             { return (*this) *= negatable(f); }
    negatable& operator*=(const long double& f)        { return (*this) *= negatable(f); }

    negatable& operator/=(const char& n)               { return (*this) /= negatable(n); }
    negatable& operator/=(const short& n)              { return (*this) /= negatable(n); }
    negatable& operator/=(const int& n)                { return (*this) /= negatable(n); }
    negatable& operator/=(const long& n)               { return (*this) /= negatable(n); }
    negatable& operator/=(const long long& n)          { return (*this) /= negatable(n); }
    negatable& operator/=(const unsigned char& u)      { return (*this) /= negatable(u); }
    negatable& operator/=(const unsigned short& u)     { return (*this) /= negatable(u); }
    negatable& operator/=(const unsigned int& u)       { return (*this) /= negatable(u); }
    negatable& operator/=(const unsigned long& u)      { return (*this) /= negatable(u); }
    negatable& operator/=(const unsigned long long& u) { return (*this) /= negatable(u); }
    negatable& operator/=(const float& f)              { return (*this) /= negatable(f); }
    negatable& operator/=(const double& f)             { return (*this) /= negatable(f); }
    negatable& operator/=(const long double& f)        { return (*this) /= negatable(f); }

    operator char()                                    { return static_cast<char>      (data / radix_split_value<value_type>()); }
    operator short()                                   { return static_cast<short>     (data / radix_split_value<value_type>()); }
    operator int()                                     { return static_cast<int>       (data / radix_split_value<value_type>()); }
    operator long()                                    { return static_cast<long>      (data / radix_split_value<value_type>()); }
    operator long long()                               { return static_cast<long long> (data / radix_split_value<value_type>()); }

    operator unsigned char()                           { return static_cast<unsigned char>     (unsigned_small_type(data) >> radix_split); }
    operator unsigned short()                          { return static_cast<unsigned short>    (unsigned_small_type(data) >> radix_split); }
    operator unsigned int()                            { return static_cast<unsigned int>      (unsigned_small_type(data) >> radix_split); }
    operator unsigned long()                           { return static_cast<unsigned long>     (unsigned_small_type(data) >> radix_split); }
    operator unsigned long long()                      { return static_cast<unsigned long long>(unsigned_small_type(data) >> radix_split); }

    operator float()
    {
      const bool is_neg = (data < static_cast<value_type>(0));

      const unsigned_small_type x = static_cast<unsigned_small_type>((!is_neg) ? data : -data);

      const unsigned_small_type integer_part = (x >> radix_split);
      const unsigned_small_type decimal_part = x - (integer_part << radix_split);

      const float f = static_cast<float>(integer_part) + (static_cast<float>(decimal_part) / radix_split_value<float>());

      return ((!is_neg) ? f : -f);
    }

    operator double()
    {
      const bool is_neg = (data < static_cast<value_type>(0));

      const unsigned_small_type x = static_cast<unsigned_small_type>((!is_neg) ? data : -data);

      const unsigned_small_type integer_part = (x >> radix_split);
      const unsigned_small_type decimal_part = x - (integer_part << radix_split);

      const double f = static_cast<double>(integer_part) + (static_cast<double>(decimal_part) / radix_split_value<double>());

      return ((!is_neg) ? f : -f);
    }

    operator long double()
    {
      const bool is_neg = (data < static_cast<value_type>(0));

      const unsigned_small_type x = static_cast<unsigned_small_type>((!is_neg) ? data : -data);

      const unsigned_small_type integer_part = (x >> radix_split);
      const unsigned_small_type decimal_part = x - (integer_part << radix_split);

      const long double f = static_cast<long double>(integer_part) + (static_cast<long double>(decimal_part) / radix_split_value<long double>());

      return ((!is_neg) ? f : -f);
    }

  private:
    value_type data;

    static const int radix_split = -resolution;
    static const int total_range = range;

    typedef typename detail::integer_type_helper<range * 1>::exact_unsigned_type unsigned_small_type;
    typedef typename detail::integer_type_helper<range * 2>::exact_unsigned_type unsigned_large_type;

    template<typename arithmetic_type>
    static arithmetic_type radix_split_value()
    {
      return fixed_point::detail::radix_split_maker<arithmetic_type, radix_split>::value();
    }

    struct nothing { };

    template<typename signed_integral_type>
    negatable(const nothing&,
              const signed_integral_type& n,
              const typename std::enable_if<   std::is_same<char,       signed_integral_type>::value
                                            || std::is_same<short,      signed_integral_type>::value
                                            || std::is_same<int,        signed_integral_type>::value
                                            || std::is_same<long,       signed_integral_type>::value
                                            || std::is_same<long long,  signed_integral_type>::value
                                            || std::is_same<value_type, signed_integral_type>::value>::type* = nullptr) : data(n) { }

    template<typename unsigned_integral_type>
    negatable(const nothing&,
              const unsigned_integral_type& u,
              const typename std::enable_if<   std::is_same<unsigned char,      unsigned_integral_type>::value
                                            || std::is_same<unsigned short,     unsigned_integral_type>::value
                                            || std::is_same<unsigned int,       unsigned_integral_type>::value
                                            || std::is_same<unsigned long,      unsigned_integral_type>::value
                                            || std::is_same<unsigned long long, unsigned_integral_type>::value>::type* = nullptr) : data(u) { }

    static bool is_quiet_nan(const negatable& x) { return (x.data  == value_quiet_nan()); }
    static bool is_infinity (const negatable& x) { using std::abs; return (abs(x.data) == value_infinity()); }

    static value_type make_unsigned_constant(const value_type& x)
    {
      // TBD: Provide support for smaller ranges and larger resolutions.
      // TBD: This will involve the generation of constant coefficients used for transcendentals.
      // TBD: The magic number *24* represents the number of binary digits for which
      // the rationalized coefficients have been optimized in transcendentals.
      // TBD: Herein lies a big chunk of development. We need to derive various
      // sets of rationalized coefficients for different ranges of precision
      // and also find a sensible point above which we switch to multiprecision.
      static_assert(range > 24,
                    "Error: the negatable class does not yet support such small range for generation of constants.");

      static_assert(radix_split <= 24,
                    "Error: the negatable class does not yet support such large resolution for generation of constants.");

      return x >> (24 - radix_split);
    }

    static negatable value_epsilon()
    {
      value_type r10  = radix_split_value<value_type>();

      if(r10 <= 10)
      {
        return negatable(nothing(), 1);
      }
      else
      {
        // TBD: Consider using template metaprogramming instead of a loop here.
        while(r10 > 10)
        {
          r10 = (r10 + 9) / 10;
        }

        return negatable(nothing(), r10);
      }
    }

    static negatable value_min()       { return negatable(nothing(), 1); }
    static negatable value_max()       { return negatable(nothing(), (std::numeric_limits<value_type>::max)() - 3) ; }
    static negatable value_infinity()  { return negatable(nothing(), value_max().data - 2) ; }
    static negatable value_quiet_nan() { return negatable(nothing(), value_max().data - 1) ; }

    friend class ::std::numeric_limits<negatable>;

    template<typename char_type, class traits_type>
    friend inline std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& os, const negatable& x)
    {
      std::basic_ostringstream<char_type, traits_type> ostr;
      ostr.flags(os.flags());
      ostr.imbue(os.getloc());

      // TBD: There must be a better way to extract the string
      // other than using an intermediate stringstream object?
      std::stringstream ss;
      ss << x.data;

      namespace fp = boost::fixed_point;

      typedef typename fp::detail::float_type_helper<total_range>::exact_float_type float_type;

      float_type v;
      ss >> v;

      v /= fp::detail::radix_split_maker<float_type, radix_split>::value();

      // TBD: Use the proper precision value from the ostream object.
      ostr.precision((radix_split * 301) / 1000);
      ostr << std::fixed << v;

      return (os << ostr.str());
    }

    // Implementations of global unary plus and minus.
    friend inline negatable operator+ (const negatable& left)                                      { return negatable(left); }
    friend inline negatable operator- (const negatable& left)                                      { negatable tmp(left); tmp.data = -tmp.data; return tmp; }

    // Implementations of global add, sub, mul, div of [lhs(negatable)] operator [rhs(negatable)].
    friend inline negatable operator+ (const negatable& u,            const negatable& v)          { return negatable(u) += v; }
    friend inline negatable operator- (const negatable& u,            const negatable& v)          { return negatable(u) -= v; }
    friend inline negatable operator* (const negatable& u,            const negatable& v)          { return negatable(u) *= v; }
    friend inline negatable operator/ (const negatable& u,            const negatable& v)          { return negatable(u) /= v; }

    // Implementations of global add, sub, mul, div of [lhs(negatable)] operator [rhs(arithmetic_type)].
    friend inline negatable operator+ (const negatable& u,            const char& n)               { return negatable(u) += n; }
    friend inline negatable operator+ (const negatable& u,            const short& n)              { return negatable(u) += n; }
    friend inline negatable operator+ (const negatable& u,            const int& n)                { return negatable(u) += n; }
    friend inline negatable operator+ (const negatable& u,            const long& n)               { return negatable(u) += n; }
    friend inline negatable operator+ (const negatable& u,            const long long& n)          { return negatable(u) += n; }
    friend inline negatable operator+ (const negatable& u,            const unsigned char& n)      { return negatable(u) += n; }
    friend inline negatable operator+ (const negatable& u,            const unsigned short& n)     { return negatable(u) += n; }
    friend inline negatable operator+ (const negatable& u,            const unsigned int& n)       { return negatable(u) += n; }
    friend inline negatable operator+ (const negatable& u,            const unsigned long& n)      { return negatable(u) += n; }
    friend inline negatable operator+ (const negatable& u,            const unsigned long long& n) { return negatable(u) += n; }
    friend inline negatable operator+ (const negatable& u,            const float& f)              { return negatable(u) += f; }
    friend inline negatable operator+ (const negatable& u,            const double& f)             { return negatable(u) += f; }
    friend inline negatable operator+ (const negatable& u,            const long double& f)        { return negatable(u) += f; }

    friend inline negatable operator- (const negatable& u,            const char& n)               { return negatable(u) -= n; }
    friend inline negatable operator- (const negatable& u,            const short& n)              { return negatable(u) -= n; }
    friend inline negatable operator- (const negatable& u,            const int& n)                { return negatable(u) -= n; }
    friend inline negatable operator- (const negatable& u,            const long& n)               { return negatable(u) -= n; }
    friend inline negatable operator- (const negatable& u,            const long long& n)          { return negatable(u) -= n; }
    friend inline negatable operator- (const negatable& u,            const unsigned char& n)      { return negatable(u) -= n; }
    friend inline negatable operator- (const negatable& u,            const unsigned short& n)     { return negatable(u) -= n; }
    friend inline negatable operator- (const negatable& u,            const unsigned int& n)       { return negatable(u) -= n; }
    friend inline negatable operator- (const negatable& u,            const unsigned long& n)      { return negatable(u) -= n; }
    friend inline negatable operator- (const negatable& u,            const unsigned long long& n) { return negatable(u) -= n; }
    friend inline negatable operator- (const negatable& u,            const float& f)              { return negatable(u) -= f; }
    friend inline negatable operator- (const negatable& u,            const double& f)             { return negatable(u) -= f; }
    friend inline negatable operator- (const negatable& u,            const long double& f)        { return negatable(u) -= f; }

    friend inline negatable operator* (const negatable& u,            const char& n)               { return negatable(u) *= n; }
    friend inline negatable operator* (const negatable& u,            const short& n)              { return negatable(u) *= n; }
    friend inline negatable operator* (const negatable& u,            const int& n)                { return negatable(u) *= n; }
    friend inline negatable operator* (const negatable& u,            const long& n)               { return negatable(u) *= n; }
    friend inline negatable operator* (const negatable& u,            const long long& n)          { return negatable(u) *= n; }
    friend inline negatable operator* (const negatable& u,            const unsigned char& n)      { return negatable(u) *= n; }
    friend inline negatable operator* (const negatable& u,            const unsigned short& n)     { return negatable(u) *= n; }
    friend inline negatable operator* (const negatable& u,            const unsigned int& n)       { return negatable(u) *= n; }
    friend inline negatable operator* (const negatable& u,            const unsigned long& n)      { return negatable(u) *= n; }
    friend inline negatable operator* (const negatable& u,            const unsigned long long& n) { return negatable(u) *= n; }
    friend inline negatable operator* (const negatable& u,            const float& f)              { return negatable(u) *= f; }
    friend inline negatable operator* (const negatable& u,            const double& f)             { return negatable(u) *= f; }
    friend inline negatable operator* (const negatable& u,            const long double& f)        { return negatable(u) *= f; }

    friend inline negatable operator/ (const negatable& u,            const char& n)               { return negatable(u) /= n; }
    friend inline negatable operator/ (const negatable& u,            const short& n)              { return negatable(u) /= n; }
    friend inline negatable operator/ (const negatable& u,            const int& n)                { return negatable(u) /= n; }
    friend inline negatable operator/ (const negatable& u,            const long& n)               { return negatable(u) /= n; }
    friend inline negatable operator/ (const negatable& u,            const long long& n)          { return negatable(u) /= n; }
    friend inline negatable operator/ (const negatable& u,            const unsigned char& n)      { return negatable(u) /= n; }
    friend inline negatable operator/ (const negatable& u,            const unsigned short& n)     { return negatable(u) /= n; }
    friend inline negatable operator/ (const negatable& u,            const unsigned int& n)       { return negatable(u) /= n; }
    friend inline negatable operator/ (const negatable& u,            const unsigned long& n)      { return negatable(u) /= n; }
    friend inline negatable operator/ (const negatable& u,            const unsigned long long& n) { return negatable(u) /= n; }
    friend inline negatable operator/ (const negatable& u,            const float& f)              { return negatable(u) /= f; }
    friend inline negatable operator/ (const negatable& u,            const double& f)             { return negatable(u) /= f; }
    friend inline negatable operator/ (const negatable& u,            const long double& f)        { return negatable(u) /= f; }

    // Implementations of global add, sub, mul, div of [lhs(arithmetic_type)] operator [rhs(negatable)].
    friend inline negatable operator+ (const char& n,                 const negatable& u)          { return negatable(n) += u; }
    friend inline negatable operator+ (const short& n,                const negatable& u)          { return negatable(n) += u; }
    friend inline negatable operator+ (const int& n,                  const negatable& u)          { return negatable(n) += u; }
    friend inline negatable operator+ (const long& n,                 const negatable& u)          { return negatable(n) += u; }
    friend inline negatable operator+ (const long long& n,            const negatable& u)          { return negatable(n) += u; }
    friend inline negatable operator+ (const unsigned char& n,        const negatable& u)          { return negatable(n) += u; }
    friend inline negatable operator+ (const unsigned short& n,       const negatable& u)          { return negatable(n) += u; }
    friend inline negatable operator+ (const unsigned int& n,         const negatable& u)          { return negatable(n) += u; }
    friend inline negatable operator+ (const unsigned long& n,        const negatable& u)          { return negatable(n) += u; }
    friend inline negatable operator+ (const unsigned long long& n,   const negatable& u)          { return negatable(n) += u; }
    friend inline negatable operator+ (const float& f,                const negatable& u)          { return negatable(f) += u; }
    friend inline negatable operator+ (const double& f,               const negatable& u)          { return negatable(f) += u; }
    friend inline negatable operator+ (const long double& f,          const negatable& u)          { return negatable(f) += u; }

    friend inline negatable operator- (const char& n,                 const negatable& u)          { return negatable(n) -= u; }
    friend inline negatable operator- (const short& n,                const negatable& u)          { return negatable(n) -= u; }
    friend inline negatable operator- (const int& n,                  const negatable& u)          { return negatable(n) -= u; }
    friend inline negatable operator- (const long& n,                 const negatable& u)          { return negatable(n) -= u; }
    friend inline negatable operator- (const long long& n,            const negatable& u)          { return negatable(n) -= u; }
    friend inline negatable operator- (const unsigned char& n,        const negatable& u)          { return negatable(n) -= u; }
    friend inline negatable operator- (const unsigned short& n,       const negatable& u)          { return negatable(n) -= u; }
    friend inline negatable operator- (const unsigned int& n,         const negatable& u)          { return negatable(n) -= u; }
    friend inline negatable operator- (const unsigned long& n,        const negatable& u)          { return negatable(n) -= u; }
    friend inline negatable operator- (const unsigned long long& n,   const negatable& u)          { return negatable(n) -= u; }
    friend inline negatable operator- (const float& f,                const negatable& u)          { return negatable(f) -= u; }
    friend inline negatable operator- (const double& f,               const negatable& u)          { return negatable(f) -= u; }
    friend inline negatable operator- (const long double& f,          const negatable& u)          { return negatable(f) -= u; }

    friend inline negatable operator* (const char& n,                 const negatable& u)          { return negatable(n) *= u; }
    friend inline negatable operator* (const short& n,                const negatable& u)          { return negatable(n) *= u; }
    friend inline negatable operator* (const int& n,                  const negatable& u)          { return negatable(n) *= u; }
    friend inline negatable operator* (const long& n,                 const negatable& u)          { return negatable(n) *= u; }
    friend inline negatable operator* (const long long& n,            const negatable& u)          { return negatable(n) *= u; }
    friend inline negatable operator* (const unsigned char& n,        const negatable& u)          { return negatable(n) *= u; }
    friend inline negatable operator* (const unsigned short& n,       const negatable& u)          { return negatable(n) *= u; }
    friend inline negatable operator* (const unsigned int& n,         const negatable& u)          { return negatable(n) *= u; }
    friend inline negatable operator* (const unsigned long& n,        const negatable& u)          { return negatable(n) *= u; }
    friend inline negatable operator* (const unsigned long long& n,   const negatable& u)          { return negatable(n) *= u; }
    friend inline negatable operator* (const float& f,                const negatable& u)          { return negatable(f) *= u; }
    friend inline negatable operator* (const double& f,               const negatable& u)          { return negatable(f) *= u; }
    friend inline negatable operator* (const long double& f,          const negatable& u)          { return negatable(f) *= u; }

    friend inline negatable operator/ (const char& n,                 const negatable& u)          { return negatable(n) /= u; }
    friend inline negatable operator/ (const short& n,                const negatable& u)          { return negatable(n) /= u; }
    friend inline negatable operator/ (const int& n,                  const negatable& u)          { return negatable(n) /= u; }
    friend inline negatable operator/ (const long& n,                 const negatable& u)          { return negatable(n) /= u; }
    friend inline negatable operator/ (const long long& n,            const negatable& u)          { return negatable(n) /= u; }
    friend inline negatable operator/ (const unsigned char& n,        const negatable& u)          { return negatable(n) /= u; }
    friend inline negatable operator/ (const unsigned short& n,       const negatable& u)          { return negatable(n) /= u; }
    friend inline negatable operator/ (const unsigned int& n,         const negatable& u)          { return negatable(n) /= u; }
    friend inline negatable operator/ (const unsigned long& n,        const negatable& u)          { return negatable(n) /= u; }
    friend inline negatable operator/ (const unsigned long long& n,   const negatable& u)          { return negatable(n) /= u; }
    friend inline negatable operator/ (const float& f,                const negatable& u)          { return negatable(f) /= u; }
    friend inline negatable operator/ (const double& f,               const negatable& u)          { return negatable(f) /= u; }
    friend inline negatable operator/ (const long double& f,          const negatable& u)          { return negatable(f) /= u; }

    // Implementations of global equality.
    friend inline bool      operator==(const negatable& u,            const negatable& v)          { return ((u.data == v.data) && (!(negatable::is_quiet_nan(u) && negatable::is_quiet_nan(v)))); }

    friend inline bool      operator==(const negatable& u,            const char& v)               { return (u.data == negatable(v).data); }
    friend inline bool      operator==(const negatable& u,            const short& v)              { return (u.data == negatable(v).data); }
    friend inline bool      operator==(const negatable& u,            const int& v)                { return (u.data == negatable(v).data); }
    friend inline bool      operator==(const negatable& u,            const long& v)               { return (u.data == negatable(v).data); }
    friend inline bool      operator==(const negatable& u,            const long long& v)          { return (u.data == negatable(v).data); }

    friend inline bool      operator==(const negatable& u,            const unsigned char& v)      { return (u.data == negatable(v).data); }
    friend inline bool      operator==(const negatable& u,            const unsigned short& v)     { return (u.data == negatable(v).data); }
    friend inline bool      operator==(const negatable& u,            const unsigned int& v)       { return (u.data == negatable(v).data); }
    friend inline bool      operator==(const negatable& u,            const unsigned long& v)      { return (u.data == negatable(v).data); }
    friend inline bool      operator==(const negatable& u,            const unsigned long long& v) { return (u.data == negatable(v).data); }

    friend inline bool      operator==(const negatable& u,            const float& v)              { return ((u.data == negatable(v).data) && (!(negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v))))); }
    friend inline bool      operator==(const negatable& u,            const double& v)             { return ((u.data == negatable(v).data) && (!(negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v))))); }
    friend inline bool      operator==(const negatable& u,            const long double& v)        { return ((u.data == negatable(v).data) && (!(negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v))))); }

    friend inline bool      operator==(const char& u,                 const negatable& v)          { return (negatable(u).data == v.data); }
    friend inline bool      operator==(const short& u,                const negatable& v)          { return (negatable(u).data == v.data); }
    friend inline bool      operator==(const int& u,                  const negatable& v)          { return (negatable(u).data == v.data); }
    friend inline bool      operator==(const long& u,                 const negatable& v)          { return (negatable(u).data == v.data); }
    friend inline bool      operator==(const long long& u,            const negatable& v)          { return (negatable(u).data == v.data); }

    friend inline bool      operator==(const unsigned char& u,        const negatable& v)          { return (negatable(u).data == v.data); }
    friend inline bool      operator==(const unsigned short& u,       const negatable& v)          { return (negatable(u).data == v.data); }
    friend inline bool      operator==(const unsigned int& u,         const negatable& v)          { return (negatable(u).data == v.data); }
    friend inline bool      operator==(const unsigned long& u,        const negatable& v)          { return (negatable(u).data == v.data); }
    friend inline bool      operator==(const unsigned long long& u,   const negatable& v)          { return (negatable(u).data == v.data); }

    friend inline bool      operator==(const float& u,                const negatable& v)          { return ((negatable(u).data == v.data) && (!((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v)))); }
    friend inline bool      operator==(const double& u,               const negatable& v)          { return ((negatable(u).data == v.data) && (!((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v)))); }
    friend inline bool      operator==(const double long& u,          const negatable& v)          { return ((negatable(u).data == v.data) && (!((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v)))); }

    // Implementations of global inequality.
    friend inline bool      operator!=(const negatable& u,            const negatable& v)          { return ((u.data != v.data) || (negatable::is_quiet_nan(u) && negatable::is_quiet_nan(v))); }

    friend inline bool      operator!=(const negatable& u,            const char& v)               { return (u.data != negatable(v).data); }
    friend inline bool      operator!=(const negatable& u,            const short& v)              { return (u.data != negatable(v).data); }
    friend inline bool      operator!=(const negatable& u,            const int& v)                { return (u.data != negatable(v).data); }
    friend inline bool      operator!=(const negatable& u,            const long& v)               { return (u.data != negatable(v).data); }
    friend inline bool      operator!=(const negatable& u,            const long long& v)          { return (u.data != negatable(v).data); }

    friend inline bool      operator!=(const negatable& u,            const unsigned char& v)      { return (u.data != negatable(v).data); }
    friend inline bool      operator!=(const negatable& u,            const unsigned short& v)     { return (u.data != negatable(v).data); }
    friend inline bool      operator!=(const negatable& u,            const unsigned int& v)       { return (u.data != negatable(v).data); }
    friend inline bool      operator!=(const negatable& u,            const unsigned long& v)      { return (u.data != negatable(v).data); }
    friend inline bool      operator!=(const negatable& u,            const unsigned long long& v) { return (u.data != negatable(v).data); }

    friend inline bool      operator!=(const negatable& u,            const float& v)              { return ((u.data != negatable(v).data) || (negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v)))); }
    friend inline bool      operator!=(const negatable& u,            const double& v)             { return ((u.data != negatable(v).data) || (negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v)))); }
    friend inline bool      operator!=(const negatable& u,            const long double& v)        { return ((u.data != negatable(v).data) || (negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v)))); }

    friend inline bool      operator!=(const char& u,                 const negatable& v)          { return (negatable(u).data != v.data); }
    friend inline bool      operator!=(const short& u,                const negatable& v)          { return (negatable(u).data != v.data); }
    friend inline bool      operator!=(const int& u,                  const negatable& v)          { return (negatable(u).data != v.data); }
    friend inline bool      operator!=(const long& u,                 const negatable& v)          { return (negatable(u).data != v.data); }
    friend inline bool      operator!=(const long long& u,            const negatable& v)          { return (negatable(u).data != v.data); }

    friend inline bool      operator!=(const unsigned char& u,        const negatable& v)          { return (negatable(u).data != v.data); }
    friend inline bool      operator!=(const unsigned short& u,       const negatable& v)          { return (negatable(u).data != v.data); }
    friend inline bool      operator!=(const unsigned int& u,         const negatable& v)          { return (negatable(u).data != v.data); }
    friend inline bool      operator!=(const unsigned long& u,        const negatable& v)          { return (negatable(u).data != v.data); }
    friend inline bool      operator!=(const unsigned long long& u,   const negatable& v)          { return (negatable(u).data != v.data); }

    friend inline bool      operator!=(const float& u,                const negatable& v)          { return ((negatable(u).data != v.data) || ((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v))); }
    friend inline bool      operator!=(const double& u,               const negatable& v)          { return ((negatable(u).data != v.data) || ((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v))); }
    friend inline bool      operator!=(const double long& u,          const negatable& v)          { return ((negatable(u).data != v.data) || ((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v))); }

    // Implementations of global operators >, <, >=, <=.
    friend inline bool      operator> (const negatable& u,            const negatable& v)          { return ((u.data > v.data) && (!(negatable::is_quiet_nan(u) && negatable::is_quiet_nan(v)))); }

    friend inline bool      operator> (const negatable& u,            const char& v)               { return (u.data > negatable(v).data); }
    friend inline bool      operator> (const negatable& u,            const short& v)              { return (u.data > negatable(v).data); }
    friend inline bool      operator> (const negatable& u,            const int& v)                { return (u.data > negatable(v).data); }
    friend inline bool      operator> (const negatable& u,            const long& v)               { return (u.data > negatable(v).data); }
    friend inline bool      operator> (const negatable& u,            const long long& v)          { return (u.data > negatable(v).data); }

    friend inline bool      operator> (const negatable& u,            const unsigned char& v)      { return (u.data > negatable(v).data); }
    friend inline bool      operator> (const negatable& u,            const unsigned short& v)     { return (u.data > negatable(v).data); }
    friend inline bool      operator> (const negatable& u,            const unsigned int& v)       { return (u.data > negatable(v).data); }
    friend inline bool      operator> (const negatable& u,            const unsigned long& v)      { return (u.data > negatable(v).data); }
    friend inline bool      operator> (const negatable& u,            const unsigned long long& v) { return (u.data > negatable(v).data); }

    friend inline bool      operator> (const negatable& u,            const float& v)              { return ((u.data > negatable(v).data) && (!(negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v))))); }
    friend inline bool      operator> (const negatable& u,            const double& v)             { return ((u.data > negatable(v).data) && (!(negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v))))); }
    friend inline bool      operator> (const negatable& u,            const long double& v)        { return ((u.data > negatable(v).data) && (!(negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v))))); }

    friend inline bool      operator> (const char& u,                 const negatable& v)          { return (negatable(u).data > v.data); }
    friend inline bool      operator> (const short& u,                const negatable& v)          { return (negatable(u).data > v.data); }
    friend inline bool      operator> (const int& u,                  const negatable& v)          { return (negatable(u).data > v.data); }
    friend inline bool      operator> (const long& u,                 const negatable& v)          { return (negatable(u).data > v.data); }
    friend inline bool      operator> (const long long& u,            const negatable& v)          { return (negatable(u).data > v.data); }

    friend inline bool      operator> (const unsigned char& u,        const negatable& v)          { return (negatable(u).data > v.data); }
    friend inline bool      operator> (const unsigned short& u,       const negatable& v)          { return (negatable(u).data > v.data); }
    friend inline bool      operator> (const unsigned int& u,         const negatable& v)          { return (negatable(u).data > v.data); }
    friend inline bool      operator> (const unsigned long& u,        const negatable& v)          { return (negatable(u).data > v.data); }
    friend inline bool      operator> (const unsigned long long& u,   const negatable& v)          { return (negatable(u).data > v.data); }

    friend inline bool      operator> (const float& u,                const negatable& v)          { return ((negatable(u).data > v.data) && (!((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v)))); }
    friend inline bool      operator> (const double& u,               const negatable& v)          { return ((negatable(u).data > v.data) && (!((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v)))); }
    friend inline bool      operator> (const double long& u,          const negatable& v)          { return ((negatable(u).data > v.data) && (!((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v)))); }

    friend inline bool      operator< (const negatable& u,            const negatable& v)          { return ((u.data < v.data) || (negatable::is_quiet_nan(u) && negatable::is_quiet_nan(v))); }

    friend inline bool      operator< (const negatable& u,            const char& v)               { return (u.data < negatable(v).data); }
    friend inline bool      operator< (const negatable& u,            const short& v)              { return (u.data < negatable(v).data); }
    friend inline bool      operator< (const negatable& u,            const int& v)                { return (u.data < negatable(v).data); }
    friend inline bool      operator< (const negatable& u,            const long& v)               { return (u.data < negatable(v).data); }
    friend inline bool      operator< (const negatable& u,            const long long& v)          { return (u.data < negatable(v).data); }

    friend inline bool      operator< (const negatable& u,            const unsigned char& v)      { return (u.data < negatable(v).data); }
    friend inline bool      operator< (const negatable& u,            const unsigned short& v)     { return (u.data < negatable(v).data); }
    friend inline bool      operator< (const negatable& u,            const unsigned int& v)       { return (u.data < negatable(v).data); }
    friend inline bool      operator< (const negatable& u,            const unsigned long& v)      { return (u.data < negatable(v).data); }
    friend inline bool      operator< (const negatable& u,            const unsigned long long& v) { return (u.data < negatable(v).data); }

    friend inline bool      operator< (const negatable& u,            const float& v)              { return ((u.data < negatable(v).data) || (negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v)))); }
    friend inline bool      operator< (const negatable& u,            const double& v)             { return ((u.data < negatable(v).data) || (negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v)))); }
    friend inline bool      operator< (const negatable& u,            const long double& v)        { return ((u.data < negatable(v).data) || (negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v)))); }

    friend inline bool      operator< (const char& u,                 const negatable& v)          { return (negatable(u).data < v.data); }
    friend inline bool      operator< (const short& u,                const negatable& v)          { return (negatable(u).data < v.data); }
    friend inline bool      operator< (const int& u,                  const negatable& v)          { return (negatable(u).data < v.data); }
    friend inline bool      operator< (const long& u,                 const negatable& v)          { return (negatable(u).data < v.data); }
    friend inline bool      operator< (const long long& u,            const negatable& v)          { return (negatable(u).data < v.data); }

    friend inline bool      operator< (const unsigned char& u,        const negatable& v)          { return (negatable(u).data < v.data); }
    friend inline bool      operator< (const unsigned short& u,       const negatable& v)          { return (negatable(u).data < v.data); }
    friend inline bool      operator< (const unsigned int& u,         const negatable& v)          { return (negatable(u).data < v.data); }
    friend inline bool      operator< (const unsigned long& u,        const negatable& v)          { return (negatable(u).data < v.data); }
    friend inline bool      operator< (const unsigned  long long& u,  const negatable& v)          { return (negatable(u).data < v.data); }

    friend inline bool      operator< (const float& u,                const negatable& v)          { return ((negatable(u).data < v.data) || ((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v))); }
    friend inline bool      operator< (const double& u,               const negatable& v)          { return ((negatable(u).data < v.data) || ((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v))); }
    friend inline bool      operator< (const double long& u,          const negatable& v)          { return ((negatable(u).data < v.data) || ((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v))); }

    friend inline bool      operator>=(const negatable& u,            const negatable& v)          { return ((u.data >= v.data) && (!(negatable::is_quiet_nan(u) && negatable::is_quiet_nan(v)))); }

    friend inline bool      operator>=(const negatable& u,            const char& v)               { return (u.data >= negatable(v).data); }
    friend inline bool      operator>=(const negatable& u,            const short& v)              { return (u.data >= negatable(v).data); }
    friend inline bool      operator>=(const negatable& u,            const int& v)                { return (u.data >= negatable(v).data); }
    friend inline bool      operator>=(const negatable& u,            const long& v)               { return (u.data >= negatable(v).data); }
    friend inline bool      operator>=(const negatable& u,            const long long& v)          { return (u.data >= negatable(v).data); }

    friend inline bool      operator>=(const negatable& u,            const unsigned char& v)      { return (u.data >= negatable(v).data); }
    friend inline bool      operator>=(const negatable& u,            const unsigned short& v)     { return (u.data >= negatable(v).data); }
    friend inline bool      operator>=(const negatable& u,            const unsigned int& v)       { return (u.data >= negatable(v).data); }
    friend inline bool      operator>=(const negatable& u,            const unsigned long& v)      { return (u.data >= negatable(v).data); }
    friend inline bool      operator>=(const negatable& u,            const unsigned long long& v) { return (u.data >= negatable(v).data); }

    friend inline bool      operator>=(const negatable& u,            const float& v)              { return ((u.data >= negatable(v).data) && (!(negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v))))); }
    friend inline bool      operator>=(const negatable& u,            const double& v)             { return ((u.data >= negatable(v).data) && (!(negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v))))); }
    friend inline bool      operator>=(const negatable& u,            const long double& v)        { return ((u.data >= negatable(v).data) && (!(negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v))))); }

    friend inline bool      operator>=(const char& u,                 const negatable& v)          { return (negatable(u).data >= v.data); }
    friend inline bool      operator>=(const short& u,                const negatable& v)          { return (negatable(u).data >= v.data); }
    friend inline bool      operator>=(const int& u,                  const negatable& v)          { return (negatable(u).data >= v.data); }
    friend inline bool      operator>=(const long& u,                 const negatable& v)          { return (negatable(u).data >= v.data); }
    friend inline bool      operator>=(const long long& u,            const negatable& v)          { return (negatable(u).data >= v.data); }

    friend inline bool      operator>=(const unsigned char& u,        const negatable& v)          { return (negatable(u).data >= v.data); }
    friend inline bool      operator>=(const unsigned short& u,       const negatable& v)          { return (negatable(u).data >= v.data); }
    friend inline bool      operator>=(const unsigned int& u,         const negatable& v)          { return (negatable(u).data >= v.data); }
    friend inline bool      operator>=(const unsigned long& u,        const negatable& v)          { return (negatable(u).data >= v.data); }
    friend inline bool      operator>=(const unsigned long long& u,   const negatable& v)          { return (negatable(u).data >= v.data); }

    friend inline bool      operator>=(const float& u,                const negatable& v)          { return ((negatable(u).data >= v.data) && (!((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v)))); }
    friend inline bool      operator>=(const double& u,               const negatable& v)          { return ((negatable(u).data >= v.data) && (!((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v)))); }
    friend inline bool      operator>=(const double long& u,          const negatable& v)          { return ((negatable(u).data >= v.data) && (!((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v)))); }

    friend inline bool      operator<=(const negatable& u,            const negatable& v)          { return ((u.data <= v.data) || (negatable::is_quiet_nan(u) && negatable::is_quiet_nan(v))); }

    friend inline bool      operator<=(const negatable& u,            const char& v)               { return (u.data <= negatable(v).data); }
    friend inline bool      operator<=(const negatable& u,            const short& v)              { return (u.data <= negatable(v).data); }
    friend inline bool      operator<=(const negatable& u,            const int& v)                { return (u.data <= negatable(v).data); }
    friend inline bool      operator<=(const negatable& u,            const long& v)               { return (u.data <= negatable(v).data); }
    friend inline bool      operator<=(const negatable& u,            const long long& v)          { return (u.data <= negatable(v).data); }

    friend inline bool      operator<=(const negatable& u,            const unsigned char& v)      { return (u.data <= negatable(v).data); }
    friend inline bool      operator<=(const negatable& u,            const unsigned short& v)     { return (u.data <= negatable(v).data); }
    friend inline bool      operator<=(const negatable& u,            const unsigned int& v)       { return (u.data <= negatable(v).data); }
    friend inline bool      operator<=(const negatable& u,            const unsigned long& v)      { return (u.data <= negatable(v).data); }
    friend inline bool      operator<=(const negatable& u,            const unsigned long long& v) { return (u.data <= negatable(v).data); }

    friend inline bool      operator<=(const negatable& u,            const float& v)              { return ((u.data <= negatable(v).data) || (negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v)))); }
    friend inline bool      operator<=(const negatable& u,            const double& v)             { return ((u.data <= negatable(v).data) || (negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v)))); }
    friend inline bool      operator<=(const negatable& u,            const long double& v)        { return ((u.data <= negatable(v).data) || (negatable::is_quiet_nan(u) && (negatable::is_quiet_nan(v)))); }

    friend inline bool      operator<=(const char& u,                 const negatable& v)          { return (negatable(u).data <= v.data); }
    friend inline bool      operator<=(const short& u,                const negatable& v)          { return (negatable(u).data <= v.data); }
    friend inline bool      operator<=(const int& u,                  const negatable& v)          { return (negatable(u).data <= v.data); }
    friend inline bool      operator<=(const long& u,                 const negatable& v)          { return (negatable(u).data <= v.data); }
    friend inline bool      operator<=(const long long& u,            const negatable& v)          { return (negatable(u).data <= v.data); }

    friend inline bool      operator<=(const unsigned char& u,        const negatable& v)          { return (negatable(u).data <= v.data); }
    friend inline bool      operator<=(const unsigned short& u,       const negatable& v)          { return (negatable(u).data <= v.data); }
    friend inline bool      operator<=(const unsigned int& u,         const negatable& v)          { return (negatable(u).data <= v.data); }
    friend inline bool      operator<=(const unsigned long& u,        const negatable& v)          { return (negatable(u).data <= v.data); }
    friend inline bool      operator<=(const unsigned long long& u,   const negatable& v)          { return (negatable(u).data <= v.data); }

    friend inline bool      operator<=(const float& u,                const negatable& v)          { return ((negatable(u).data <= v.data) || ((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v))); }
    friend inline bool      operator<=(const double& u,               const negatable& v)          { return ((negatable(u).data <= v.data) || ((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v))); }
    friend inline bool      operator<=(const double long& u,          const negatable& v)          { return ((negatable(u).data <= v.data) || ((negatable::is_quiet_nan(u)) && negatable::is_quiet_nan(v))); }
  };

  } }
  // namespace boost::fixed_point

  namespace std
  {
    // Provide a specialization of std::numeric_limits<negatable>.
    template<const int integral_range,
             const int decimal_resolution,
             const boost::fixed_point::round::round_type round_mode,
             const boost::fixed_point::overflow::overflow_type overflow_mode>
    class numeric_limits<boost::fixed_point::negatable<integral_range, decimal_resolution, round_mode, overflow_mode> >
    {
    private:
      typedef boost::fixed_point::negatable<integral_range,
                                                  decimal_resolution,
                                                  round_mode,
                                                  overflow_mode> negatable_type;

      static const int range      = integral_range - decimal_resolution;
      static const int resolution = decimal_resolution;

    public:
      static const bool                    is_specialized    = true;
      static const int                     digits            = std::numeric_limits<typename negatable_type::value_type>::digits;
      static const int                     digits10          = std::numeric_limits<typename negatable_type::value_type>::digits;
      static const int                     max_digits10      = std::numeric_limits<typename negatable_type::value_type>::max_digits10;
      static const bool                    is_signed         = true;
      static const bool                    is_integer        = false;
      static const bool                    is_exact          = true;
      static const int                     radix             = 2;
      static const int                     min_exponent      = -negatable_type::radix_split;
      static const int                     min_exponent10    = static_cast<int>((static_cast<long>(min_exponent) * 301L) / 1000L);
      static const int                     max_exponent      = std::numeric_limits<typename negatable_type::value_type>::digits - negatable_type::radix_split;
      static const int                     max_exponent10    = static_cast<int>((static_cast<long>(max_exponent) * 301L) / 1000L);
      static const bool                    has_infinity      = true;
      static const bool                    has_quiet_NaN     = true;
      static const bool                    has_signaling_NaN = false;
      static const std::float_denorm_style has_denorm        = std::denorm_absent;
      static const bool                    has_denorm_loss   = false;
      static const bool                    is_iec559         = false;
      static const bool                    is_bounded        = true;
      static const bool                    is_modulo         = false;
      static const bool                    traps             = false;
      static const bool                    tinyness_before   = false;
      static const std::float_round_style  round_style       = std::round_toward_zero;

      static negatable_type (min)() throw()                  { return negatable_type::value_min(); }
      static negatable_type (max)() throw()                  { return negatable_type::value_max(); }
      static negatable_type lowest() throw()                 { return -(max)(); }
      static negatable_type epsilon() throw()                { return negatable_type::value_epsilon(); }
      static negatable_type round_error() throw()            { return negatable_type(1) / 2; }
      static negatable_type infinity() throw()               { return negatable_type::value_infinity(); }
      static negatable_type quiet_NaN() throw()              { return negatable_type::value_quiet_nan(); }
    };
  }
  // namespace std

#endif // FIXED_POINT_2015_03_06_HPP_

/*
///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Test the fixed_point class.

#include <iomanip>
#include <iostream>
#include "fixed_point.hpp"

typedef boost::fixed_point::negatable<32, -24> fixed_point_type;

int main()
{
  fixed_point_type x = fixed_point_type(10) / 3;
  float            f = 10.0F / 3.0F;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << x
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << f
            << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << cosh(x)
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << cosh(f)
            << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << cos(x)
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << cos(f)
            << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << log(x)
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << log(f)
            << std::endl;
}
*/
