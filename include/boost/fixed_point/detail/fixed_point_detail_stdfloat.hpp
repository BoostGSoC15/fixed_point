///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015 - 2016.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// fixed_point_detail_cstdfloat.hpp implements a subset of
// floating-point typedefs having specified widths,
// as described in N1703 (proposed for C via JTC1/SC22/WG14).
// See: http://www.open-std.org/jtc1/sc22/wg14/www/docs/n1703.pdf
// This file has been adapted from <boost/math/cstdfloat/cstdfloat_types.hpp>.
// Support for float16_t has been removed.


#ifndef BOOST_FIXED_POINT_STDFLOAT_2015_08_14_HPP_
  #define BOOST_FIXED_POINT_STDFLOAT_2015_08_14_HPP_

  #include <float.h>

  namespace boost { namespace fixed_point { namespace detail {

  // This is the beginning of the preamble.

  // In this preamble, the preprocessor is used to query certain
  // preprocessor definitions from <float.h>. Based on the results
  // of these queries, an attempt is made to automatically detect
  // the presence of built-in floating-point types having specified
  // widths. These are *thought* to be conformant with IEEE-754,
  // whereby an unequivocal test based on numeric_limits is
  // provided in the C++ header <cstdfloat>.

  // In addition, macros that are used for initializing floating-point
  // literal values and some basic min/max values are defined.

  // First, we will pre-load certain preprocessor definitions
  // with a dummy value.

  #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH  0

  #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE  0
  #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE  0
  #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE  0
  #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE 0

  // Ensure that the compiler has a radix-2 floating-point representation.
  #if (!defined(FLT_RADIX) || ((defined(FLT_RADIX) && (FLT_RADIX != 2))))
    #error The compiler does not support any of the radix-2 floating-point types required for <stdfloat.h>.
  #endif

  // Check if built-in float is equivalent to float32_t, float64_t, or float80_t.
  #if(defined(FLT_MANT_DIG) && defined(FLT_MAX_EXP))
    #if((FLT_MANT_DIG == 24) && (FLT_MAX_EXP == 128) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT32_NATIVE_TYPE float
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 32
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT32_C(x)  (x ## F)
      #define BOOST_FIXED_POINT_FLOAT_32_MIN  FLT_MIN
      #define BOOST_FIXED_POINT_FLOAT_32_MAX  FLT_MAX
    #elif((FLT_MANT_DIG == 53) && (FLT_MAX_EXP == 1024) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT64_NATIVE_TYPE float
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 64
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT64_C(x)  (x ##F)
      #define BOOST_FIXED_POINT_FLOAT_64_MIN  FLT_MIN
      #define BOOST_FIXED_POINT_FLOAT_64_MAX  FLT_MAX
    #elif((FLT_MANT_DIG == 64) && (FLT_MAX_EXP == 16384) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT80_NATIVE_TYPE float
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 80
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT80_C(x)  (x ## F)
      #define BOOST_FIXED_POINT_FLOAT_80_MIN  FLT_MIN
      #define BOOST_FIXED_POINT_FLOAT_80_MAX  FLT_MAX
    #elif((FLT_MANT_DIG == 113) && (FLT_MAX_EXP == 16384) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT128_NATIVE_TYPE float
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 128
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT128_C(x)  (x ## F)
      #define BOOST_FIXED_POINT_FLOAT_128_MIN  FLT_MIN
      #define BOOST_FIXED_POINT_FLOAT_128_MAX  FLT_MAX
    #endif
  #endif

  // Check if built-in double is equivalent to float32_t, float64_t, or float80_t.
  #if(defined(DBL_MANT_DIG) && defined(DBL_MAX_EXP))
    #if((DBL_MANT_DIG == 24) && (DBL_MAX_EXP == 128) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT32_NATIVE_TYPE double
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 32
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT32_C(x)  (x)
      #define BOOST_FIXED_POINT_FLOAT_32_MIN  DBL_MIN
      #define BOOST_FIXED_POINT_FLOAT_32_MAX  DBL_MAX
    #elif((DBL_MANT_DIG == 53) && (DBL_MAX_EXP == 1024) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT64_NATIVE_TYPE double
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 64
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT64_C(x)  (x)
      #define BOOST_FIXED_POINT_FLOAT_64_MIN  DBL_MIN
      #define BOOST_FIXED_POINT_FLOAT_64_MAX  DBL_MAX
    #elif((DBL_MANT_DIG == 64) && (DBL_MAX_EXP == 16384) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT80_NATIVE_TYPE double
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 80
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT80_C(x)  (x)
      #define BOOST_FIXED_POINT_FLOAT_80_MIN  DBL_MIN
      #define BOOST_FIXED_POINT_FLOAT_80_MAX  DBL_MAX
    #elif((DBL_MANT_DIG == 113) && (DBL_MAX_EXP == 16384) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT128_NATIVE_TYPE double
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 128
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT128_C(x)  (x)
      #define BOOST_FIXED_POINT_FLOAT_128_MIN  DBL_MIN
      #define BOOST_FIXED_POINT_FLOAT_128_MAX  DBL_MAX
    #endif
  #endif

  // Check if built-in long double is equivalent to float32_t, float64_t, or float80_t.
  #if(defined(LDBL_MANT_DIG) && defined(LDBL_MAX_EXP))
    #if((LDBL_MANT_DIG == 24) && (LDBL_MAX_EXP == 128) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT32_NATIVE_TYPE long double
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 32
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT32_C(x)  (x ## L)
      #define BOOST_FIXED_POINT_FLOAT_32_MIN  LDBL_MIN
      #define BOOST_FIXED_POINT_FLOAT_32_MAX  LDBL_MAX
    #elif((LDBL_MANT_DIG == 53) && (LDBL_MAX_EXP == 1024) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT64_NATIVE_TYPE long double
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 64
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT64_C(x)  (x ## L)
      #define BOOST_FIXED_POINT_FLOAT_64_MIN  LDBL_MIN
      #define BOOST_FIXED_POINT_FLOAT_64_MAX  LDBL_MAX
    #elif((LDBL_MANT_DIG == 64) && (LDBL_MAX_EXP == 16384) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT80_NATIVE_TYPE long double
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 80
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT80_C(x)  (x ## L)
      #define BOOST_FIXED_POINT_FLOAT_80_MIN  LDBL_MIN
      #define BOOST_FIXED_POINT_FLOAT_80_MAX  LDBL_MAX
    #elif((LDBL_MANT_DIG == 113) && (LDBL_MAX_EXP == 16384) && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 0))
      #define BOOST_FIXED_POINT_STDFLOAT_FLOAT128_NATIVE_TYPE long double
      #undef  BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH
      #define BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH 128
      #undef  BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE
      #define BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE  1
      #define BOOST_FIXED_POINT_FLOAT128_C(x)  (x ## L)
      #define BOOST_FIXED_POINT_FLOAT_128_MIN  LDBL_MIN
      #define BOOST_FIXED_POINT_FLOAT_128_MAX  LDBL_MAX
    #endif
  #endif

  // This is the end of the preamble. Now we use the results
  // of the queries that have been obtained in the preamble.

  // Ensure that the compiler has any suitable floating-point type whatsoever.
  #if (   (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE  == 0) \
       && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE  == 0) \
       && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE  == 0) \
       && (BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 0))
    #error The compiler does not support any of the floating-point types required for <boost/fixed_point/detail/fixed_point_detail_stdfloat.hpp>.
  #endif

  // Here, we define the floating-point typedefs having specified widths
  // based on the preprocessor analysis from the preamble above.

  // These type definitions are defined in the global namespace.

  // For simplicity, the least and fast types are type defined identically
  // as the corresponding fixed-width type. This behavior can, however,
  // be modified in order to be optimized for a given compiler implementation.

  // This section also contains the various min/max macros
  // for the least and fast types.

  #if(BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT32_NATIVE_TYPE == 1)
    typedef BOOST_FIXED_POINT_STDFLOAT_FLOAT32_NATIVE_TYPE  float32_t;
    typedef float32_t float_fast32_t;
    typedef float32_t float_least32_t;

    #define BOOST_FIXED_POINT_FLOAT_FAST32_MIN   BOOST_FIXED_POINT_FLOAT_32_MIN
    #define BOOST_FIXED_POINT_FLOAT_LEAST32_MIN  BOOST_FIXED_POINT_FLOAT_32_MIN
    #define BOOST_FIXED_POINT_FLOAT_FAST32_MAX   BOOST_FIXED_POINT_FLOAT_32_MAX
    #define BOOST_FIXED_POINT_FLOAT_LEAST32_MAX  BOOST_FIXED_POINT_FLOAT_32_MAX
  #endif

  #if(BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT64_NATIVE_TYPE == 1)
    typedef BOOST_FIXED_POINT_STDFLOAT_FLOAT64_NATIVE_TYPE  float64_t;
    typedef float64_t float_fast64_t;
    typedef float64_t float_least64_t;

    #define BOOST_FIXED_POINT_FLOAT_FAST64_MIN   BOOST_FIXED_POINT_FLOAT_64_MIN
    #define BOOST_FIXED_POINT_FLOAT_LEAST64_MIN  BOOST_FIXED_POINT_FLOAT_64_MIN
    #define BOOST_FIXED_POINT_FLOAT_FAST64_MAX   BOOST_FIXED_POINT_FLOAT_64_MAX
    #define BOOST_FIXED_POINT_FLOAT_LEAST64_MAX  BOOST_FIXED_POINT_FLOAT_64_MAX
  #endif

  #if(BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT80_NATIVE_TYPE == 1)
    typedef BOOST_FIXED_POINT_STDFLOAT_FLOAT80_NATIVE_TYPE  float80_t;
    typedef float80_t float_fast80_t;
    typedef float80_t float_least80_t;

    #define BOOST_FIXED_POINT_FLOAT_FAST80_MIN   BOOST_FIXED_POINT_FLOAT_80_MIN
    #define BOOST_FIXED_POINT_FLOAT_LEAST80_MIN  BOOST_FIXED_POINT_FLOAT_80_MIN
    #define BOOST_FIXED_POINT_FLOAT_FAST80_MAX   BOOST_FIXED_POINT_FLOAT_80_MAX
    #define BOOST_FIXED_POINT_FLOAT_LEAST80_MAX  BOOST_FIXED_POINT_FLOAT_80_MAX
  #endif

  #if(BOOST_FIXED_POINT_STDFLOAT_HAS_FLOAT128_NATIVE_TYPE == 1)
    typedef BOOST_FIXED_POINT_STDFLOAT_FLOAT128_NATIVE_TYPE  float128_t;
    typedef float128_t float_fast128_t;
    typedef float128_t float_least128_t;

    #define BOOST_FIXED_POINT_FLOAT_FAST128_MIN   BOOST_FIXED_POINT_FLOAT_128_MIN
    #define BOOST_FIXED_POINT_FLOAT_LEAST128_MIN  BOOST_FIXED_POINT_FLOAT_128_MIN
    #define BOOST_FIXED_POINT_FLOAT_FAST128_MAX   BOOST_FIXED_POINT_FLOAT_128_MAX
    #define BOOST_FIXED_POINT_FLOAT_LEAST128_MAX  BOOST_FIXED_POINT_FLOAT_128_MAX
  #endif

  // The following section contains macros that are used for
  // initializing floating-point literal values and various
  // min and max macros for the floatmax types.

  #if(BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH == 32)
    typedef float32_t     floatmax_t;
    #define BOOST_FIXED_POINT_FLOATMAX_C(x) BOOST_FIXED_POINT_FLOAT32_C(x)
    #define BOOST_FIXED_POINT_FLOATMAX_MIN  BOOST_FIXED_POINT_FLOAT_32_MIN
    #define BOOST_FIXED_POINT_FLOATMAX_MAX  BOOST_FIXED_POINT_FLOAT_32_MAX
  #elif(BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH == 64)
    typedef float64_t     floatmax_t;
    #define BOOST_FIXED_POINT_FLOATMAX_C(x) BOOST_FIXED_POINT_FLOAT64_C(x)
    #define BOOST_FIXED_POINT_FLOATMAX_MIN  BOOST_FIXED_POINT_FLOAT_64_MIN
    #define BOOST_FIXED_POINT_FLOATMAX_MAX  BOOST_FIXED_POINT_FLOAT_64_MAX
  #elif(BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH == 80)
    typedef float80_t     floatmax_t;
    #define BOOST_FIXED_POINT_FLOATMAX_C(x) BOOST_FIXED_POINT_FLOAT80_C(x)
    #define BOOST_FIXED_POINT_FLOATMAX_MIN  BOOST_FIXED_POINT_FLOAT_80_MIN
    #define BOOST_FIXED_POINT_FLOATMAX_MAX  BOOST_FIXED_POINT_FLOAT_80_MAX
  #elif(BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH == 128)
    typedef float128_t     floatmax_t;
    #define BOOST_FIXED_POINT_FLOATMAX_C(x) BOOST_FIXED_POINT_FLOAT128_C(x)
    #define BOOST_FIXED_POINT_FLOATMAX_MIN  BOOST_FIXED_POINT_FLOAT_128_MIN
    #define BOOST_FIXED_POINT_FLOATMAX_MAX  BOOST_FIXED_POINT_FLOAT_128_MAX
  #else
    #error The maximum available floating-point width for <boost/fixed_point/detail/fixed_point_detail_stdfloat.hpp> is undefined.
  #endif

  #undef BOOST_FIXED_POINT_STDFLOAT_MAXIMUM_AVAILABLE_WIDTH

  } } } // namespace boost::fixed_point::detail

#endif // BOOST_FIXED_POINT_STDFLOAT_2015_08_14_HPP_
