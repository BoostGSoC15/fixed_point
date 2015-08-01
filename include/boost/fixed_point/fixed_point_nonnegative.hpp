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
// In this file, we include subsidiary files that implement the
// proposed specified fixed-point types.

#ifndef FIXED_POINT_NONNEGATIVE_2015_08_01_HPP_
  #define FIXED_POINT_NONNEGATIVE_2015_08_01_HPP_

  #include <boost/fixed_point/detail/fixed_point_detail.hpp>

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

    } // End of forward declaration of transcendental and cmath functions.
  } // namespace boost::fixed_point

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
      Example: @c boost::fixed_point::nonnegative<2, -5> @c x;\n
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
