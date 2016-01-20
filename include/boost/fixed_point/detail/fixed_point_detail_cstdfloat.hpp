///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2013 - 2016.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// fixed_point_detail_cstdfloat.hpp implements a subset of
// floating-point typedefs having specified widths,
// as described in N3626 (proposed for C++14).
// See: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3626.pdf
// This file has been adapted from <boost/cstdfloat.hpp>.
// Support for float128_t has been removed.


#ifndef FIXED_POINT_DETAIL_CSTDFLOAT_2015_08_14_HPP_
  #define FIXED_POINT_DETAIL_CSTDFLOAT_2015_08_14_HPP_

  #include <limits>
  #include <boost/fixed_point/detail/fixed_point_detail_stdfloat.hpp>

  // Here, we define floating-point typedefs having specified widths
  // within the namespace boost::fixed_point::detail.

  // In addition, a clear assessment of IEEE-754 comformance is
  // carried out using compile-time assertion.

  namespace boost { namespace fixed_point { namespace detail {

  #if(BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE == 1)
    static_assert(   (std::numeric_limits<float32_t>::is_iec559    == true)
                  && (std::numeric_limits<float32_t>::radix        ==    2)
                  && (std::numeric_limits<float32_t>::digits       ==   24)
                  && (std::numeric_limits<float32_t>::max_exponent ==  128),
                  "The 32-bit floating-point type in <cstdfloat> is not IEEE-754 conformant.");
  #endif

  #if(BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE == 1)
    static_assert(   (std::numeric_limits<float64_t>::is_iec559    == true)
                  && (std::numeric_limits<float64_t>::radix        ==    2)
                  && (std::numeric_limits<float64_t>::digits       ==   53)
                  && (std::numeric_limits<float64_t>::max_exponent == 1024),
                  "The 64-bit floating-point type in <cstdfloat> is not IEEE-754 conformant.");
  #endif

  #if(BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE == 1)
    static_assert(   (std::numeric_limits<float80_t>::is_iec559    ==  true)
                  && (std::numeric_limits<float80_t>::radix        ==     2)
                  && (std::numeric_limits<float80_t>::digits       ==    64)
                  && (std::numeric_limits<float80_t>::max_exponent == 16384),
                  "The 80-bit floating-point type in <cstdfloat> is not IEEE-754 conformant.");
  #endif

  #if(BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 1)
    static_assert(   (std::numeric_limits<float128_t>::is_iec559    ==  true)
                  && (std::numeric_limits<float128_t>::radix        ==     2)
                  && (std::numeric_limits<float128_t>::digits       ==   113)
                  && (std::numeric_limits<float128_t>::max_exponent == 16384),
                  "The 128-bit floating-point type in <cstdfloat> is not IEEE-754 conformant.");
  #endif

  #if(   (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE  == 1) \
      || (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE  == 1) \
      || (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE  == 1) \
      || (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 1))
  #else
    #error The maximum available floating-point width for <boost/fixed_point/detail/fixed_point_detailc_stdfloat.hpp> is undefined.
  #endif

  #undef BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE
  #undef BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE
  #undef BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE
  #undef BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE

  } } } // namespace boost::fixed_point::detail

#endif // FIXED_POINT_DETAIL_CSTDFLOAT_2015_08_14_HPP_
