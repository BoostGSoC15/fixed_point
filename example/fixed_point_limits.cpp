
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 2015.
// Copyright Christopher Kormanyos 2015.
// Copyright Nikhar Agrawal 2015.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

// This file also includes Doxygen-style documentation about the function of the code.
// See http://www.doxygen.org for details.

//! \file

// Below are snippets of code that can be included into a Quickbook file.

#include <exception>
#include <iostream>
#include <iomanip>
#include <limits>
#include <typeinfo>
#include <type_traits>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/special_functions/pow.hpp>

template <typename T>
void show_fixed_point_limits()
{
  // Ensure that type T is a fixed_point type,
  // (although actually as written it will also work for floating-point types).
  BOOST_STATIC_ASSERT_MSG(boost::fixed_point::is_fixed_point<T>::value == true, "This function is designed for fixed_point types.");

  std::cout.precision(std::numeric_limits<T>::digits10);
  std::cout << std::boolalpha
            << std::showpoint
            << std::showpos;

  // Show the relevant numeric_limits.
  std::cout << "Numeric_limits of type:\n"
            << typeid(T).name()
            << "\n radix        = " <<  std::numeric_limits<T>::radix
            << "\n digits10     = " <<  std::numeric_limits<T>::digits10
            << "\n max_digits10 = " <<  std::numeric_limits<T>::max_digits10
            << "\n epsilon      = " <<  std::numeric_limits<T>::epsilon()
            << "\n lowest       = " <<  std::numeric_limits<T>::lowest()
            << "\n min          = " << (std::numeric_limits<T>::min)()
            << "\n max          = " << (std::numeric_limits<T>::max)();

  // If, most unexpectedly, type T has a representation for infinity or NaN, show them.
  if (std::numeric_limits<T>::has_infinity)
  {
    std::cout << "\n infinity = " << std::numeric_limits<T>::infinity();
  }
  else
  {
    std::cout << "\n Type does not have an infinity.";
  }
  if (std::numeric_limits<T>::has_quiet_NaN)
  {
    std::cout << "\n NaN = " << std::numeric_limits<T>::quiet_NaN();
  }
  else
  {
    std::cout << "\n Type does not have a NaN.";
  }
  std::cout << std::endl;
} // template <typename T> void show_fixed_point_limits

/*! As an example, define a local fixed-point negatable type using 31 + sign bits, 
splitting the bits equally to range and resolution.
*/
typedef boost::fixed_point::negatable<15, -16> fixed_point_type;

int main()
{
  try
  {
    //[fixed_point_limits_1
    std::cout << "Number of possible values is 2^[range + abs(resolution)] = 2^"
              << std::numeric_limits<fixed_point_type>::digits
              << " = " << static_cast<long>(boost::math::pow<std::numeric_limits<fixed_point_type>::digits>(2))
              << std::endl;
    //] [/fixed_point_limits_1]
    std::cout.precision(std::numeric_limits<fixed_point_type>::digits10); // Show all significant decimal digits.
    //[fixed_point_limits_2
    show_fixed_point_limits<fixed_point_type>();
    //] [/fixed_point_limits_2]
    std::cout << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
  }
} // int main()

/*
//[fixed_point_limits_output

Number of possible values is 2^[range + abs(resolution)] = 2^31 = -2147483648

Numeric_limits of type:
class boost::fixed_point::negatable<15,-16,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined>
radix        = +2
digits10     = +9
max_digits10 = +11
epsilon      = +3.05175781e-05
lowest       = -32768.0000
min          = +1.52587891e-05
max          = +32768.0000
Type does not have an infinity.
Type does not have a NaN.

Press any key to continue . . .

//] [/fixed_point_limits_output]

*/

