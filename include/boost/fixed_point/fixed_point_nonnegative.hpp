///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2013 - 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// This file is a partial reference implementation for the proposed
// "C++ binary fixed-point arithmetic" as specified in N3352.
// See: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html
// In this particular file, we implement a prototype for the proposed
// nonnegative template class.

// ----------------------------------------------------------------
// This file contains only the nonnegative class.
// ----------------------------------------------------------------

#ifndef FIXED_POINT_NONNEGATIVE_2015_08_01_HPP_
  #define FIXED_POINT_NONNEGATIVE_2015_08_01_HPP_

  // There is optional support for certain variations of fixed_point
  // using preprocessor definitions. Not all of these are supported
  // at the moment. The potential options include:

  // Is supported now     : #define BOOST_FIXED_POINT_DISABLE_IOSTREAM
  // Is supported now     : #define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION
  // Is not yet supported : #define BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH
  // Is not yet supported : #define BOOST_FIXED_POINT_DISABLE_CPP11

  // With BOOST_FIXED_POINT_DISABLE_IOSTREAM, all I/O streaming
  // is disabled, as is the inclusion of associated standard
  // library headers. This option eliminates all I/O stream
  // overhead, in particular for bare-metal microcontroller projects.
  // Disabling I/O streaming requires simultaneous disabling
  // of multiprecision.

  // With BOOST_FIXED_POINT_DISABLE_MULTIPRECISION,
  // the use of Boost.Multiprecision for back-ends
  // of the fixed-point classes is disabled.

  // With BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH,
  // fixed_point avoids using the unsigned_large_type.
  // This option is intended for systems with limited
  // integer widths such as bare-metal microcontrollers.
  // When used in combination with BOOST_FIXED_POINT_DISABLE_MULTIPRECISION,
  // this option provides fixed-point representations with
  // up to 64-bits (if 64-bit integral types are available)
  // without requiring any of Boost.Multiprecision.
  // Otherwise, 32-bit internal representations would
  // have the largest possible widths.

  // With BOOST_FIXED_POINT_DISABLE_CPP11, an optional
  // back-port to C++03 is supported. This eliminates
  // the use of all C++11 language elements. This might send
  // the wrong message about language technology, but could
  // increase the range of potential target compilers
  // (especially for embedded systems).

  #if defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM)

    // When I/O streaming is disabled:
    //   * We must eliminate use of Boost.Multiprecision.
    //   * We must eliminate use of Boost.Math.Constants (because these require istreaming).
    //   * We must eliminate the parts of <boost/cstdfloat.hpp> that require I/O streams.

    #if !defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)
      #error Error: BOOST_FIXED_POINT_DISABLE_IOSTREAM can not be set without also setting BOOST_FIXED_POINT_DISABLE_MULTIPRECISION.
    #endif

    #define BOOST_CSTDFLOAT_NO_LIBQUADMATH_COMPLEX
    #define BOOST_CSTDFLOAT_NO_LIBQUADMATH_IOSTREAM

    #include <algorithm>
    #include <cmath>
    #include <limits>
    #include <type_traits>
    #include <boost/cstdfloat.hpp>
    #include <boost/cstdint.hpp>

  #elif defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION) && !defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM)

    // When multiprecision is disabled but I/O streaming is enabled:
    //   * We must eliminate Boost.Multiprecision.

    #include <algorithm>
    #include <cmath>
    #include <iomanip>
    #include <istream>
    #include <limits>
    #include <ostream>
    #include <sstream>
    #include <string>
    #include <type_traits>
    #include <boost/cstdfloat.hpp>
    #include <boost/cstdint.hpp>
    #include <boost/lexical_cast.hpp>
    #include <boost/math/constants/constants.hpp>

  #else

    // When multiprecision and I/O streaming are enabled:
    //   * We eliminate nothing and include all overhead from
    //     both Boost.Multiprecision as well as I/O streaming.

    #include <algorithm>
    #include <cmath>
    #include <iomanip>
    #include <istream>
    #include <limits>
    #include <ostream>
    #include <sstream>
    #include <string>
    #include <type_traits>
    #include <boost/cstdfloat.hpp>
    #include <boost/cstdint.hpp>
    #include <boost/lexical_cast.hpp>
    #include <boost/math/constants/constants.hpp>
    #include <boost/multiprecision/cpp_bin_float.hpp>
    #include <boost/multiprecision/cpp_int.hpp>

  #endif

  #include <boost/fixed_point/detail/fixed_point_detail.hpp>
  #include <boost/fixed_point/fixed_point_overflow.hpp>
  #include <boost/fixed_point/fixed_point_round.hpp>

  static_assert(std::numeric_limits<boost::uint8_t >::digits ==  8, "Configuration error: the size of boost::uint8_t  must be 8  bits!");
  static_assert(std::numeric_limits<boost::uint16_t>::digits == 16, "Configuration error: the size of boost::uint16_t must be 16 bits!");
  static_assert(std::numeric_limits<boost::uint32_t>::digits == 32, "Configuration error: the size of boost::uint32_t must be 32 bits!");
  static_assert(std::numeric_limits<boost::uint64_t>::digits == 64, "Configuration error: the size of boost::uint64_t must be 64 bits!");

  namespace boost { namespace fixed_point {

  // Forward declaration of the nonnegative class.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  class nonnegative;

  // What follows are forward declarations of elementary transcendental functions mainly from <cmath>.

  // Forward declaration of abs.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> abs(nonnegative<IntegralRange,
                                                   FractionalResolution,
                                                   RoundMode,
                                                   OverflowMode> x);

  // Forward declaration of fabs.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fabs(nonnegative<IntegralRange,
                                                    FractionalResolution,
                                                    RoundMode,
                                                    OverflowMode> x);

  // Forward declaration of frexp.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> frexp(nonnegative<IntegralRange,
                                                     FractionalResolution,
                                                     RoundMode,
                                                     OverflowMode> x, int* expptr);

  // Forward declaration of ldexp.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> ldexp(nonnegative<IntegralRange,
                                                     FractionalResolution,
                                                     RoundMode,
                                                     OverflowMode> x, int exp);

  // Forward declaration of fixed_next.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fixed_next(nonnegative<IntegralRange,
                                              FractionalResolution,
                                              RoundMode,
                                              OverflowMode> x);

  // Forward declaration of fixed_prior.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fixed_prior(nonnegative<IntegralRange,
                                                           FractionalResolution,
                                                           RoundMode,
                                                           OverflowMode> x);

  // Forward declaration of fixed_distance.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fixed_distance(nonnegative<IntegralRange,
                                                              FractionalResolution,
                                                              RoundMode,
                                                              OverflowMode> x);

  // Forward declaration of fixed_advance.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fixed_advance(nonnegative<IntegralRange,
                                                             FractionalResolution,
                                                             RoundMode,
                                                             OverflowMode> x,
                                                             int distance);

  // Forward declaration of fixed_nextafter.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fixed_nextafter(nonnegative<IntegralRange,
                                                               FractionalResolution,
                                                               RoundMode,
                                                               OverflowMode> x);

  // Forward declaration of sqrt.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> sqrt(nonnegative<IntegralRange,
                                                    FractionalResolution,
                                                    RoundMode,
                                                    OverflowMode> x);

  } } // namespace boost::fixed_point: End of forward declaration of transcendental and cmath functions.

  namespace std
  {
    // Forward declaration of the specialization of std::numeric_limits<nonnegative>.
    template<const int IntegralRange,
             const int FractionalResolution,
             typename RoundMode,
             typename OverflowMode>
    class numeric_limits<boost::fixed_point::nonnegative<IntegralRange,
                                                         FractionalResolution,
                                                         RoundMode,
                                                         OverflowMode>>;
  }

  namespace boost { namespace fixed_point {

  // We will now begin the implementation of the nonnegative class.

  /*!
    \brief Fixed_point nonnegative class used for unsigned fractional arithmetic.
    \details This is a partial reference implementation for the proposed by
      Lawrence Crowl, "C++ binary fixed-point arithmetic" as specified in N3352.\n
      In this particular file, we implement a prototype for the nonnegative template class.\n
      Example: @c boost::fixed_point::nonnegative<3, -5> @c x;\n
      TODO  some more examples here?
    \tparam IntegralRange Integer integer >= 0 defines a range of unsigned number n that is 2^-IntegralRange < n < 2^IntegralRange.
    \tparam FractionalResolution integer <= -1 defines resolution.
      The resolution of a fractional number is 2^FractionalResolution.
    \tparam RoundMode struct defining the rounding behaviour, default round::fastest.\n
    \note  Not all rounding or all overflow modes proposed in N3352 are yet implemented.
    \tparam OverflowMode struct defining the behaviour from rounding, default overflow::undefined.
    \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html
  */

  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode = round::fastest,
           typename OverflowMode = overflow::undefined>
  class nonnegative
  {
  public:
  };

  } } // namespace boost::fixed_point

#endif // FIXED_POINT_NONNEGATIVE_2015_08_01_HPP_
