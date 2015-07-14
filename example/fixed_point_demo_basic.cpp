
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
//! Basic demonstration of use of fixed-point types
// Below are snippets of code that can be included into a Quickbook file.

#include <iostream>
#include <iomanip>
#include <exception>
#include <typeinfo>
#include <limits>

//[fixed_point_include_1
#include <boost/fixed_point/fixed_point.hpp>
//] [/fixed_point_include_1]

//[fixed_point_typedef_1
typedef boost::fixed_point::negatable<15, -16> fixed_point_type;
//] [/fixed_point_typedef_1]

//typedef boost::fixed_point::negatable<87, -2> fixed_point_type_fastest_round;

int main()
{
  try
  {
    std::cout.precision(std::numeric_limits<fixed_point_type>::digits10);
//[show_numeric_limits_1
    std::cout << "Numeric_limits for type\n "
      << typeid(fixed_point_type).name()
      << "\n digits10 = " << std::numeric_limits<fixed_point_type>::digits10
      << "\n max_digits10 = " << std::numeric_limits<fixed_point_type>::max_digits10
      << "\n radix = " << std::numeric_limits<fixed_point_type>::digits
      << "\n epsilon = " << std::numeric_limits<fixed_point_type>::epsilon()
      << "\n max = " << std::numeric_limits<fixed_point_type>::max()
      << "\n min = " << std::numeric_limits<fixed_point_type>::min()
      << "\n lowest = " << std::numeric_limits<fixed_point_type>::lowest();
    if (std::numeric_limits<fixed_point_type>::has_infinity)
    {
      std::cout << "\n infinity = " << std::numeric_limits<fixed_point_type>::infinity();
    }
    else
    {
      std::cout << "\n Type does not have an infinity!";
    }
    if (std::numeric_limits<fixed_point_type>::has_quiet_NaN)
    {
      std::cout << "\n NaN = " << std::numeric_limits<fixed_point_type>::quiet_NaN();
    }
    else
    {
      std::cout << "\n Type does not have a NaN!";
    }
    //] [/show_numeric_limits_1]

    std::cout.setf(std::ios_base::boolalpha | std::ios_base::showpoint); // Show any trailing zeros.
    std::cout << std::endl;

    // Note that using integral types to construct 
    // rather a floating-point float or double 1.23F
    // is usually more efficient.
//[fixed_example_1

    fixed_point_type x = fixed_point_type(123) / 100;
    fixed_point_type y = fixed_point_type(456) / 10000;

    // Show all the significant digits for this particular floating-point type.
    std::cout.precision(std::numeric_limits<fixed_point_type>::digits10);

    std::cout << "fixed_point_type(123) / 100 = " 
      << x // 1.22999573 is the nearest representation of decimal digit string 1.23.
      << std::endl;

    std::cout << "fixed_point_type(456) / 10000 = " 
      << y // 0.0455932617 is the nearest representation of decimal digit string 0.0456
      << std::endl;

    x = -x / 2; // Do some trivial arithmetic.

    std::cout <<"x = -x / 2 = " 
      << x // -0.614990234  is the nearest representation of decimal digit string -0.615
      << std::endl;

//] [/fixed_example_1]

//[fixed_example_functions
    int exponential;
    fixed_point_type xx = frexp(x, &exponential);
    std::cout << "frexp(x, &exponential) = " << xx // 0.614990234
      << " exponential = " << exponential << std::endl; // 0
    exponential++; // double the value because radix == 2.
    fixed_point_type x2 = ldexp(x, exponential);

    // Show how the fraction and exponent parts become after changing exponent.
    std::cout << "frexp(x, &exponential) = " << frexp(x2, &exponential) // -0.614990234
      << " exponential = " << exponential << std::endl; // 1

    // Other cmath functions are available of course, for example:
    std::cout << "ldexp(x, exponential); = " << x2 << std::endl; // -1.22998047
    std::cout << "abs(x2) = " << abs(x2) << std::endl; // 1.22998047
    std::cout << "fabs(x2) = " << fabs(x2) << std::endl; // 1.22998047
    std::cout << "sqrt(y) = " << sqrt(y) << std::endl; //  = 0.213516235

//] [/fixed_example_functions]

  }
  catch (std::exception ex)
  {
    std::cout << ex.what() << std::endl;
  }
}


/*
//[numeric_limits_output_1
Numeric_limits for type class boost::fixed_point::negatable<15,-16,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined>
digits10 = 9
max_digits10 = 11
radix = 31
epsilon = 0.000106811523
max = 32768
min = 1.52587891e-005
lowest = -32768
Type does not have an infinity!
Type does not have a NaN!
//] [/numeric_limits_output_1]

//[numeric_limits_output_2
fixed_point_type(123) / 100 = 1.22999573
fixed_point_type(456) / 10000 = 0.0455932617
x = -x / 2 = -0.614990234
//] [/numeric_limits_output_2]

//[numeric_limits_output_3
frexp(x, &exponential) = -0.614990234 exponential = 0
frexp(x, &exponential) = -0.614990234 exponential = 1
ldexp(x, exponential); = -1.22998047
abs(x2) = 1.22998047
fabs(x2) = 1.22998047
sqrt(y) = 0.2135162
//] [/numeric_limits_output_3]

*/