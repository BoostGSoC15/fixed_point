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
// In this file, we include subsidiary files that implement
// certain overflow modes of fixed-point types.

#ifndef FIXED_POINT_OVERFLOW_2015_08_01_HPP_
  #define FIXED_POINT_OVERFLOW_2015_08_01_HPP_

  namespace boost { namespace fixed_point {

  namespace overflow
  {
    struct impossible   { }; //!< Template parameter for fixed_point types.\n Programmer analysis of the program has determined that overflow cannot occur. Uses of this mode should be accompanied by an argument supporting the conclusion.
    struct undefined    { }; //!< Template parameter for fixed_point types.\n Programmers are willing to accept undefined behavior in the event of an overflow.
    struct modulus      { }; //!< Template parameter for fixed_point types.\n The assigned value is the dynamic value mod the range of the variable. This mode makes sense only with unsigned numbers. It is useful for angular measures.
    struct saturate     { }; //!< Template parameter for fixed_point types.\n If the dynamic value exceeds the range of the variable, assign the nearest representable value.
    struct exception    { }; //!< Template parameter for fixed_point types.\n If the dynamic value exceeds the range of the variable, throw an exeception of type `std::overflow_error`.
  }

  } } // namespace boost::fixed_point

#endif // FIXED_POINT_OVERFLOW_2015_08_01_HPP_
