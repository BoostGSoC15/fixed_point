///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2013 - 2016.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

/*!
  \file

 \brief This file is a partial reference implementation for the proposed
 "C++ binary fixed-point arithmetic" as specified in N3352.\n

 \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html \n

 In this file, we include subsidiary files that implement the
 proposed specified fixed-point types,
 and explain macros that may be optionally defined.

 \detail
   There is optional support for certain variations of fixed_point
   using preprocessor definitions. Not all of these are supported
   at the moment. The potential options include:

   Is supported now  : #define BOOST_FIXED_POINT_DISABLE_IOSTREAM\n
   Is supported now  : #define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION\n
   Not yet supported : #define BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH\n
   Not yet supported : #define BOOST_FIXED_POINT_DISABLE_CPP11\n
   Is now supported  : #define BOOST_FIXED_POINT_ENABLE_GMP_BACKENDS\n
*/

#ifndef FIXED_POINT_2015_03_06_HPP_
  #define FIXED_POINT_2015_03_06_HPP_

#ifdef BOOST_DOXYGEN_GENERATION
// These items below document the function of optional macros,
// but are processed only if the doxyfile or jamfile contains
// <doxygen:param>PREDEFINED=BOOST_DOXYGEN_GENERATION

  /*!
  \brief All I/O streaming is disabled, as is the inclusion of associated standard
  library headers. This option eliminates all I/O stream overhead,
  in particular for bare-metal microcontroller projects.
  Disabling I/O streaming requires simultaneous disabling of multiprecision by defining
  BOOST_FIXED_POINT_DISABLE_MULTIPRECISION
  */
#  define BOOST_FIXED_POINT_DISABLE_IOSTREAM

  /*! \def BOOST_FIXED_POINT_DISABLE_MULTIPRECISION
  \brief The use of Boost.Multiprecision for back-ends
  of the fixed-point classes is disabled.
  */

#  define BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH
/*! \def BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH
   \brief fixed_point avoids using the @c unsigned_large_type.\n
   This option is intended for systems with limited
   integer widths such as bare-metal microcontrollers.\n
   When used in combination with BOOST_FIXED_POINT_DISABLE_MULTIPRECISION,
   this option provides fixed-point representations with
   up to 64-bits (if 64-bit integral types are available)
   without requiring any of Boost.Multiprecision.
   Otherwise, 32-bit internal representations would
   have the largest possible widths.
*/
#  define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION

/*!
   \brief With BOOST_FIXED_POINT_DISABLE_CPP11, an optional
   back-port to C++03 is supported. This eliminates
   the use of all C++11 language elements. This might send
   the wrong message about language technology, but could
   increase the range of potential target compilers
   (especially for embedded systems).
*/

#define BOOST_FIXED_POINT_DISABLE_CPP11
/*!
  \brief Multiple-precision backends (when used) will use the ones
   based on GMP instead of Boost.Multiprecision's self-written
   multiple-precision backends.
   \sa https://gmplib.org
 */

#define BOOST_FIXED_POINT_ENABLE_GMP_BACKENDS

#endif BOOST_DOXYGEN_GENERATION

#include <boost/fixed_point/fixed_point_negatable.hpp>

#endif // FIXED_POINT_2015_03_06_HPP_
