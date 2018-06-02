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
  \brief Fixed_point namespace round and structs used for handling rounding as value for template parameter RoundMode, default @c round::fastest.
*/

#ifndef FIXED_POINT_ROUND_2015_08_01_HPP_
  #define FIXED_POINT_ROUND_2015_08_01_HPP_

  namespace boost { namespace fixed_point {

  namespace round
  { // Care - Doxygen is really picky about layout here - don't try to tidy it up!
    struct fastest      { }; //!< Template parameter for fixed_point types.\n Speed is more important than the choice of value.
    struct negative     { }; //!< Template parameter for fixed_point types.\n Round towards negative infinity. This mode is useful in interval arithmetic.
    struct truncated    { }; //!< Template parameter for fixed_point types.\n Round towards zero. This mode is useful in implementing integral arithmetic.
    struct positive     { }; //!< Template parameter for fixed_point types.\n Round towards positive infinity. This mode is useful in interval arithmetic.
    struct classic      { }; //!< Template parameter for fixed_point types.\n Round towards the nearest value, but exactly-half values are rounded towards maximum magnitude. This mode is the standard school algorithm.
    struct nearest_even { }; //!< Template parameter for fixed_point types.\n Round towards the nearest value, but exactly-half values are rounded towards even values. This mode has more balance than the classic mode.
    struct nearest_odd  { }; //!< Template parameter for fixed_point types.\n Round towards the nearest value, but exactly-half values are rounded towards odd values. This mode has as much balance as the near_even mode, but preserves more information.
  }

  // See also ISO/IEC 14882:2011 Paragraph 18.3.2.5.

  // 18.3.2.5 Type float_round_style
  //
  // namespace std
  // {
  //   enum float_round_style
  //   {
  //     round_indeterminate       = -1, : Potential interpretation: fastest
  //     round_toward_zero         =  0, : Potential interpretation: truncated (This might be the same as fastest.)
  //     round_to_nearest          =  1, : Potential interpretation: nearest_even
  //     round_toward_infinity     =  2, : Potential interpretation: positive (Round toward positive infinity.)
  //     round_toward_neg_infinity =  3  : Potential interpretation: negative (Round toward negative infinity.)
  //   };
  // }

  } } // namespace boost::fixed_point

#endif // FIXED_POINT_ROUND_2015_08_01_HPP_
