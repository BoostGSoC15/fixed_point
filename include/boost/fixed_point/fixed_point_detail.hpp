///////////////////////////////////////////////////////////////////////////////
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Copyright Christopher Kormanyos 2014 - 2015.
//  Copyright Nikhar Agrawal 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// This file contains fixed_point details.

#ifndef FIXED_POINT_DETAIL_2015_05_23_HPP_
  #define FIXED_POINT_DETAIL_2015_05_23_HPP_

  #include <sstream>
  #include <type_traits>
  #include <boost/cstdfloat.hpp>
  #include <boost/cstdint.hpp>
  #include <boost/lexical_cast.hpp>
  #include <boost/multiprecision/cpp_bin_float.hpp>
  #include <boost/multiprecision/cpp_int.hpp>

  // Do not produce Doxygen indexing of items in namespace detail unless specifically required.
  // The section between \cond and \endcond can be included by adding its section label DETAIL
  // to the ENABLED_SECTIONS configuration option. 
  // If the section label is omitted, the section will be excluded from processing unconditionally.

  //! \cond DETAIL

  namespace boost { namespace fixed_point { namespace detail {

  template<typename unsigned_integral_type,
           const unsigned bit_pos,
           const unsigned bit_cnt,
           typename enable_type = void>
  struct bit_mask_helper
  {
    // Ensure that the requested bit mask is in range.
    static_assert((bit_pos + bit_cnt) <= unsigned(std::numeric_limits<unsigned_integral_type>::digits),
                  "the requested bit_mask value exceeds the maximum value of the unsigned_integral_type");

    static const unsigned_integral_type& value()
    {
      static const unsigned_integral_type the_bit_mask =
        static_cast<unsigned_integral_type>(static_cast<unsigned_integral_type>(static_cast<unsigned_integral_type>(~static_cast<unsigned_integral_type>(0U)) >> (std::numeric_limits<unsigned_integral_type>::digits - bit_cnt)) << bit_pos);

      return the_bit_mask;
    }
  };

  template<typename unsigned_integral_type,
           const unsigned bit_pos,
           const unsigned bit_cnt>
  struct bit_mask_helper<unsigned_integral_type,
                         bit_pos,
                         bit_cnt,
                         typename std::enable_if<   std::is_integral<unsigned_integral_type>::value
                                                 && std::is_unsigned<unsigned_integral_type>::value>::type>
  {
    // Ensure that the requested bit mask is in range.
    static_assert((bit_pos + bit_cnt) <= unsigned(std::numeric_limits<unsigned_integral_type>::digits),
                  "the requested bit_mask value exceeds the maximum value of the unsigned_integral_type");

    static unsigned_integral_type value() BOOST_NOEXCEPT
    {
      return static_cast<unsigned_integral_type>(static_cast<unsigned_integral_type>(static_cast<unsigned_integral_type>(~static_cast<unsigned_integral_type>(0U)) >> (std::numeric_limits<unsigned_integral_type>::digits - bit_cnt)) << bit_pos);
    }
  };

  template<const unsigned bit_count,
            typename enable_type = void>
  struct integer_type_helper
  {
  private:
    typedef boost::multiprecision::cpp_int_backend<bit_count,
                                                   bit_count,
                                                   boost::multiprecision::signed_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    signed_integral_backend_type;

    typedef boost::multiprecision::cpp_int_backend<bit_count,
                                                   bit_count,
                                                   boost::multiprecision::unsigned_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    unsigned_integral_backend_type;

  public:
    typedef boost::multiprecision::number<signed_integral_backend_type,
                                          boost::multiprecision::et_off> exact_signed_type;

    typedef boost::multiprecision::number<unsigned_integral_backend_type,
                                          boost::multiprecision::et_off> exact_unsigned_type;
  };

  template<const unsigned bit_count>
  struct integer_type_helper<bit_count,
                              typename std::enable_if<(bit_count <= 8U)>::type>
  {
    typedef boost::int8_t  exact_signed_type;
    typedef boost::uint8_t exact_unsigned_type;
  };

  template<const unsigned bit_count>
  struct integer_type_helper<bit_count,
                              typename std::enable_if<   (bit_count >   8U)
                                                      && (bit_count <= 16U)>::type>
  {
    typedef boost::int16_t  exact_signed_type;
    typedef boost::uint16_t exact_unsigned_type;
  };

  template<const unsigned bit_count>
  struct integer_type_helper<bit_count,
                              typename std::enable_if<   (bit_count >  16U)
                                                      && (bit_count <= 32U)>::type>
  {
    typedef boost::int32_t  exact_signed_type;
    typedef boost::uint32_t exact_unsigned_type;
  };

  template<const unsigned bit_count>
  struct integer_type_helper<bit_count,
                              typename std::enable_if<   (bit_count >  32U)
                                                      && (bit_count <= 64U)>::type>
  {
    typedef boost::int64_t  exact_signed_type;
    typedef boost::uint64_t exact_unsigned_type;
  };

  template<const unsigned bit_count>
  struct integer_type_helper<bit_count,
                              typename std::enable_if<   (bit_count >   64U)
                                                      && (bit_count <= 128U)>::type>
  {
  private:
    typedef boost::multiprecision::cpp_int_backend<128U,
                                                   128U,
                                                   boost::multiprecision::signed_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    signed_integral_backend_type;

    typedef boost::multiprecision::cpp_int_backend<128U,
                                                   128U,
                                                   boost::multiprecision::unsigned_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    unsigned_integral_backend_type;

  public:
    typedef boost::multiprecision::number<signed_integral_backend_type,
                                          boost::multiprecision::et_off> exact_signed_type;

    typedef boost::multiprecision::number<unsigned_integral_backend_type,
                                          boost::multiprecision::et_off> exact_unsigned_type;
  };

  template<const unsigned bit_count>
  struct integer_type_helper<bit_count,
                              typename std::enable_if<   (bit_count >  128U)
                                                      && (bit_count <= 256U)>::type>
  {
  private:
    typedef boost::multiprecision::cpp_int_backend<256U,
                                                   256U,
                                                   boost::multiprecision::signed_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    signed_integral_backend_type;

    typedef boost::multiprecision::cpp_int_backend<256U,
                                                   256U,
                                                   boost::multiprecision::unsigned_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    unsigned_integral_backend_type;

  public:
    typedef boost::multiprecision::number<signed_integral_backend_type,
                                          boost::multiprecision::et_off> exact_signed_type;

    typedef boost::multiprecision::number<unsigned_integral_backend_type,
                                          boost::multiprecision::et_off> exact_unsigned_type;
  };

  template<const unsigned bit_count>
  struct integer_type_helper<bit_count,
                              typename std::enable_if<   (bit_count >  256U)
                                                      && (bit_count <= 512U)>::type>
  {
  private:
    typedef boost::multiprecision::cpp_int_backend<512U,
                                                   512U,
                                                   boost::multiprecision::signed_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    signed_integral_backend_type;

    typedef boost::multiprecision::cpp_int_backend<512U,
                                                   512U,
                                                   boost::multiprecision::unsigned_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    unsigned_integral_backend_type;

  public:
    typedef boost::multiprecision::number<signed_integral_backend_type,
                                          boost::multiprecision::et_off> exact_signed_type;

    typedef boost::multiprecision::number<unsigned_integral_backend_type,
                                          boost::multiprecision::et_off> exact_unsigned_type;
  };

  template<const unsigned bit_count>
  struct integer_type_helper<bit_count,
                              typename std::enable_if<   (bit_count >   512U)
                                                      && (bit_count <= 1024U)>::type>
  {
  private:
    typedef boost::multiprecision::cpp_int_backend<1024U,
                                                   1024U,
                                                   boost::multiprecision::signed_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    signed_integral_backend_type;

    typedef boost::multiprecision::cpp_int_backend<1024U,
                                                   1024U,
                                                   boost::multiprecision::unsigned_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    unsigned_integral_backend_type;

  public:
    typedef boost::multiprecision::number<signed_integral_backend_type,
                                          boost::multiprecision::et_off> exact_signed_type;

    typedef boost::multiprecision::number<unsigned_integral_backend_type,
                                          boost::multiprecision::et_off> exact_unsigned_type;
  };

  template<const unsigned bit_count>
  struct integer_type_helper<bit_count,
                              typename std::enable_if<   (bit_count >  1024U)
                                                      && (bit_count <= 2048U)>::type>
  {
  private:
    typedef boost::multiprecision::cpp_int_backend<2048U,
                                                   2048U,
                                                   boost::multiprecision::signed_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    signed_integral_backend_type;

    typedef boost::multiprecision::cpp_int_backend<2048U,
                                                   2048U,
                                                   boost::multiprecision::unsigned_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    unsigned_integral_backend_type;

  public:
    typedef boost::multiprecision::number<signed_integral_backend_type,
                                          boost::multiprecision::et_off> exact_signed_type;

    typedef boost::multiprecision::number<unsigned_integral_backend_type,
                                          boost::multiprecision::et_off> exact_unsigned_type;
  };

  template<const unsigned bit_count>
  struct integer_type_helper<bit_count,
                              typename std::enable_if<   (bit_count >  2048U)
                                                      && (bit_count <= 4096U)>::type>
  {
  private:
    typedef boost::multiprecision::cpp_int_backend<4096U,
                                                   4096U,
                                                   boost::multiprecision::signed_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    signed_integral_backend_type;

    typedef boost::multiprecision::cpp_int_backend<4096U,
                                                   4096U,
                                                   boost::multiprecision::unsigned_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    unsigned_integral_backend_type;

  public:
    typedef boost::multiprecision::number<signed_integral_backend_type,
                                          boost::multiprecision::et_off> exact_signed_type;

    typedef boost::multiprecision::number<unsigned_integral_backend_type,
                                          boost::multiprecision::et_off> exact_unsigned_type;
  };

  template<const unsigned bit_count>
  struct integer_type_helper<bit_count,
                              typename std::enable_if<   (bit_count >  4096U)
                                                      && (bit_count <= 8192U)>::type>
  {
  private:
    typedef boost::multiprecision::cpp_int_backend<8192U,
                                                   8192U,
                                                   boost::multiprecision::signed_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    signed_integral_backend_type;

    typedef boost::multiprecision::cpp_int_backend<8192U,
                                                   8192U,
                                                   boost::multiprecision::unsigned_magnitude,
                                                   boost::multiprecision::unchecked,
                                                   void>
    unsigned_integral_backend_type;

  public:
    typedef boost::multiprecision::number<signed_integral_backend_type,
                                          boost::multiprecision::et_off> exact_signed_type;

    typedef boost::multiprecision::number<unsigned_integral_backend_type,
                                          boost::multiprecision::et_off> exact_unsigned_type;
  };

  template<const unsigned bit_count,
            typename enable_type = void>
  struct float_type_helper
  {
    typedef boost::multiprecision::number<
              boost::multiprecision::backends::cpp_bin_float<bit_count,
                                                             boost::multiprecision::backends::digit_base_2>,
            boost::multiprecision::et_off>
    exact_float_type;
  };

  template<const unsigned bit_count>
  struct float_type_helper<bit_count,
                           typename std::enable_if<(bit_count <= 24U)>::type>
  {
    typedef boost::float32_t exact_float_type;
  };

  template<const unsigned bit_count>
  struct float_type_helper<bit_count,
                           typename std::enable_if<   (bit_count >  24U)
                                                   && (bit_count <= 53U)>::type>
  {
    typedef boost::float64_t exact_float_type;
  };

  template<typename arithmetic_type,
           const int radix_split,
           typename enable_type = void>
  struct radix_split_maker
  {
    static const arithmetic_type& value()
    {
      static bool is_init = bool();

      arithmetic_type local_result = arithmetic_type();

      if(is_init == false)
      {
        is_init = true;

        // The variable xn stores the binary powers of x.
        local_result = arithmetic_type(((radix_split % 2) != 0) ? arithmetic_type(2) : arithmetic_type(1));

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
            local_result *= xn;
          }
        }
      }

      static const arithmetic_type the_result(local_result);

      return the_result;
    }
  };

  template<typename arithmetic_type,
            const int radix_split>
  struct radix_split_maker<arithmetic_type,
                           radix_split,
                           typename std::enable_if<   std::is_integral<arithmetic_type>::value
                                                   && (radix_split < 8)>::type>
  {
    static const arithmetic_type& value()
    {
      static const arithmetic_type& the_result(UINT8_C(1) << radix_split);
      return the_result;
    }
  };

  template<typename arithmetic_type,
            const int radix_split>
  struct radix_split_maker<arithmetic_type,
                           radix_split,
                           typename std::enable_if<   std::is_integral<arithmetic_type>::value
                                                   && (radix_split >=  8)
                                                   && (radix_split <  16)>::type>
  {
    static const arithmetic_type& value()
    {
      static const arithmetic_type the_result(UINT16_C(1) << radix_split);
      return the_result;
    }
  };

  template<typename arithmetic_type,
            const int radix_split>
  struct radix_split_maker<arithmetic_type,
                           radix_split,
                           typename std::enable_if<   std::is_integral<arithmetic_type>::value
                                                   && (radix_split >= 16)
                                                   && (radix_split <  32)>::type>
  {
    static const arithmetic_type& value()
    {
      static const arithmetic_type the_result(UINT32_C(1) << radix_split);
      return the_result;
    }
  };

  template<typename arithmetic_type,
            const int radix_split>
  struct radix_split_maker<arithmetic_type,
                           radix_split,
                           typename std::enable_if<   std::is_integral<arithmetic_type>::value
                                                   && (radix_split >= 32)
                                                   && (radix_split <  64)>::type>
  {
    static const arithmetic_type& value()
    {
      static const arithmetic_type the_result(UINT64_C(1) << radix_split);
      return the_result;
    }
  };

  template<typename unsigned_integral_type,
           typename floating_point_type,
           typename enable_type = void>
  struct conversion_helper
  {
    static void convert_floating_point_to_unsigned_integer(const floating_point_type& floating_point_source,
                                                           unsigned_integral_type& unsigned_destination)
    {
      unsigned_destination = floating_point_source.template convert_to<unsigned_integral_type>();
    }
  };

  template<typename floating_point_type>
  struct conversion_helper<boost::uint64_t,
                           floating_point_type,
                           typename std::enable_if<std::is_floating_point<floating_point_type>::value == false>::type>
  {
    static void convert_floating_point_to_unsigned_integer(const floating_point_type& floating_point_source,
                                                           boost::uint64_t& unsigned_destination)
    {
      // TBD: Here is a big workaround for the conversion of cpp_bin_float to uint64_t.
      // TBD: It is for cases when the digits in cpp_bin_float are fewer than
      // the digits in uint64_t, but larger than the digits in float64_t.
      // TBD: See the TODO in the comment at line 1113 of cpp_bin_float.hpp.

      std::stringstream ss;

      ss << std::fixed << floating_point_source;

      std::string str(ss.str());

      const std::string::size_type position_of_dot = str.find(".");

      if(position_of_dot != std::string::npos)
      {
        str = str.substr(std::string::size_type(0U), position_of_dot);
      }

      unsigned_destination = boost::lexical_cast<boost::uint64_t>(str);
    }
  };

  template<typename unsigned_integral_type,
           typename floating_point_type>
  struct conversion_helper<unsigned_integral_type,
                           floating_point_type,
                           typename std::enable_if<std::is_floating_point<floating_point_type>::value>::type>
  {
    static void convert_floating_point_to_unsigned_integer(const floating_point_type& floating_point_source,
                                                           unsigned_integral_type& unsigned_destination)
    {
      unsigned_destination = static_cast<unsigned_integral_type>(floating_point_source);
    }
  };
  } } } // namespace boost::fixed_point::detail
  //! \endcond // DETAIL

#endif // FIXED_POINT_DETAIL_2015_05_23_HPP_
