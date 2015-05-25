///////////////////////////////////////////////////////////////////////////////
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Copyright Christopher Kormanyos 2014 - 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// This file contains fixed_point details.

#ifndef FIXED_POINT_DETAIL_2015_05_23_HPP_
  #define FIXED_POINT_DETAIL_2015_05_23_HPP_

  #include <type_traits>
  #include <boost/cstdfloat.hpp>
  #include <boost/cstdint.hpp>
  #include <boost/multiprecision/cpp_bin_float.hpp>
  #include <boost/multiprecision/cpp_int.hpp>

  namespace boost { namespace fixed_point { namespace detail
  {
    template<const unsigned bit_count,
             typename enable_type = void>
    struct integer_type_helper
    {
      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<bit_count,
                                                       bit_count,
                                                       boost::multiprecision::signed_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_signed_type;

      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<bit_count,
                                                       bit_count,
                                                       boost::multiprecision::unsigned_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_unsigned_type;
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
      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<128U,
                                                       128U,
                                                       boost::multiprecision::signed_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_signed_type;

      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<128U,
                                                       128U,
                                                       boost::multiprecision::unsigned_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_unsigned_type;
    };

    template<const unsigned bit_count>
    struct integer_type_helper<bit_count,
                               typename std::enable_if<   (bit_count >  256U)
                                                       && (bit_count <= 512U)>::type>
    {
      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<512U,
                                                       512U,
                                                       boost::multiprecision::signed_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_signed_type;

      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<512U,
                                                       512U,
                                                       boost::multiprecision::unsigned_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_unsigned_type;
    };

    template<const unsigned bit_count>
    struct integer_type_helper<bit_count,
                               typename std::enable_if<   (bit_count >   512U)
                                                       && (bit_count <= 1024U)>::type>
    {
      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<1024U,
                                                       1024U,
                                                       boost::multiprecision::signed_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_signed_type;

      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<1024U,
                                                       1024U,
                                                       boost::multiprecision::unsigned_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_unsigned_type;
    };

    template<const unsigned bit_count>
    struct integer_type_helper<bit_count,
                               typename std::enable_if<   (bit_count >  1024U)
                                                       && (bit_count <= 2048U)>::type>
    {
      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<2048U,
                                                       2048U,
                                                       boost::multiprecision::signed_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_signed_type;

      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<2048U,
                                                       2048U,
                                                       boost::multiprecision::unsigned_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_unsigned_type;
    };

    template<const unsigned bit_count>
    struct integer_type_helper<bit_count,
                               typename std::enable_if<   (bit_count >  2048U)
                                                       && (bit_count <= 4096U)>::type>
    {
      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<4096U,
                                                       4096U,
                                                       boost::multiprecision::signed_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_signed_type;

      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<4096U,
                                                       4096U,
                                                       boost::multiprecision::unsigned_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_unsigned_type;
    };

    template<const unsigned bit_count>
    struct integer_type_helper<bit_count,
                               typename std::enable_if<   (bit_count >  4096U)
                                                       && (bit_count <= 8192U)>::type>
    {
      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<8192U,
                                                       8192U,
                                                       boost::multiprecision::signed_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_signed_type;

      typedef boost::multiprecision::number<
                boost::multiprecision::cpp_int_backend<8192U,
                                                       8192U,
                                                       boost::multiprecision::unsigned_magnitude,
                                                       boost::multiprecision::unchecked,
                                                       void>,
                boost::multiprecision::et_off>
      exact_unsigned_type;
    };

    template<const unsigned bit_count,
             typename enable_type = void>
    struct float_type_helper
    {
      typedef boost::multiprecision::number<
                boost::multiprecision::backends::cpp_bin_float<bit_count,
                                                               boost::multiprecision::backends::digit_base_2>>
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

    template<typename integral_source_type,
             typename other_destination_type>
    other_destination_type convert_to(const integral_source_type& source)
    {
      return static_cast<other_destination_type>(source);
    }

    template<typename arithmetic_type,
             const int radix_split,
             typename enable_type = void>
    struct radix_split_maker
    {
      static arithmetic_type& value()
      {
        static bool            is_init    = bool();
        static arithmetic_type the_result = arithmetic_type();

        if(is_init == false)
        {
          is_init = true;

          // The variable xn stores the binary powers of x.
          the_result = arithmetic_type(((radix_split % 2) != 0) ? arithmetic_type(2) : arithmetic_type(1));

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
              the_result *= xn;
            }
          }
        }

        return the_result;
      }
    };

    template<typename arithmetic_type,
             const int radix_split>
    struct radix_split_maker<arithmetic_type,
                             radix_split,
                             typename std::enable_if<(radix_split < 32)>::type>
    {
      static arithmetic_type& value()
      {
        static arithmetic_type the_result(UINT32_C(1) << radix_split);
        return the_result;
      }
    };

    template<typename arithmetic_type,
             const int radix_split>
    struct radix_split_maker<arithmetic_type,
                             radix_split,
                             typename std::enable_if<   (radix_split >= 32)
                                                     && (radix_split <  64)>::type>
    {
      static arithmetic_type& value()
      {
        static arithmetic_type the_result(UINT64_C(1) << radix_split);
        return the_result;
      }
    };
  } } } // namespace boost::fixed_point::detail

#endif // FIXED_POINT_DETAIL_2015_05_23_HPP_
