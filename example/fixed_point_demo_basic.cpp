
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 2015 - 2016.
// Copyright Christopher Kormanyos 2015 - 2016.

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
#include <iomanip>
#include <iostream>
#include <limits>
#include <typeinfo>

//[fixed_point_include_1
#include <boost/fixed_point/fixed_point.hpp>
//] [/fixed_point_include_1]

//[fixed_point_typedef_1
typedef boost::fixed_point::negatable<15, -16> fixed_point_type;
//] [/fixed_point_typedef_1]

int main()
{
  try
  {
    std::cout.precision(std::numeric_limits<fixed_point_type>::digits10 + 1);

//[show_numeric_limits_1

    std::cout << "Numeric_limits for type"                                                 << std::endl
              << typeid(fixed_point_type).name()                                           << std::endl
              << "digits10     = " <<  std::numeric_limits<fixed_point_type>::digits10     << std::endl
              << "max_digits10 = " <<  std::numeric_limits<fixed_point_type>::max_digits10 << std::endl
              << "radix        = " <<  std::numeric_limits<fixed_point_type>::radix        << std::endl
              << "epsilon      = " <<  std::numeric_limits<fixed_point_type>::epsilon()    << std::endl
              << "max          = " << (std::numeric_limits<fixed_point_type>::max)()       << std::endl
              << "min          = " << (std::numeric_limits<fixed_point_type>::min)()       << std::endl
              << "lowest       = " <<  std::numeric_limits<fixed_point_type>::lowest()     << std::endl
              ;

    if(std::numeric_limits<fixed_point_type>::has_infinity)
    {
      std::cout << "infinity = " << std::numeric_limits<fixed_point_type>::infinity() << std::endl;
    }
    else
    {
      std::cout << "Type does not have an infinity" << std::endl;
    }

    if (std::numeric_limits<fixed_point_type>::has_quiet_NaN)
    {
      std::cout << "NaN = " << std::numeric_limits<fixed_point_type>::quiet_NaN() << std::endl;
    }
    else
    {
      std::cout << "Type does not have a NaN" << std::endl;
    }
//] [/show_numeric_limits_1]

    std::cout.setf(std::ios::boolalpha | std::ios::showpoint); // Show any trailing zeros.
    std::cout << std::endl;

//[fixed_example_1

    fixed_point_type x = fixed_point_type(123) /   100;
    fixed_point_type y = fixed_point_type(456) / 10000;

    // Show all the significant digits for this particular floating-point type.
    std::cout.precision(std::numeric_limits<fixed_point_type>::digits10);

    std::cout << "x = fixed_point_type(123) /   100 = " 
              << x // 1.22999573 is the nearest representation of decimal digit string 1.23.
              << std::endl;

    std::cout << "y = fixed_point_type(456) / 10000 = "
              << y // 0.0455932617 is the nearest representation of decimal digit string 0.0456
              << std::endl;

    x = -x / 2; // Do some trivial arithmetic.

    std::cout <<"x = -x / 2 = " 
              << x // -0.614990234  is the nearest representation of decimal digit string -0.615
              << std::endl;

//] [/fixed_example_1]

//[fixed_example_functions

    int exponential;

    x = frexp(x, &exponential);

    std::cout << "x = frexp(x, &exponential) = "
              << x // 0.614990234
              << " exponential = "
              << exponential
              << std::endl; // 0

    exponential++; // double the value.

    std::cout << "double the value" << std::endl;

    x = ldexp(x, exponential);

    // Show the fraction and exponent parts after changing the exponent.
    std::cout << "x = frexp(x, &exponential) = "
              << frexp(x, &exponential) // -0.614990234
              << " exponential = "
              << exponential
              << std::endl; // 1

    // Other C numeric math functions (cmath) are available of course, for example:
    std::cout << "ldexp(x, exponential); = "
              << x
              << std::endl; // -1.22998047

    std::cout << "abs  (x)       = "
              <<  abs(x)
              << std::endl; // 1.22998047

    std::cout << "fabs (x)       = " << fabs(x) << std::endl; // 1.22998047

    std::cout << "sqrt (fabs(x)) = "
              <<  sqrt(fabs(x))
              << std::endl; //  = 0.

    std::cout << "sqrt (y)       = "
              <<  sqrt(y)
              << std::endl; //  = 0.213516235

//] [/fixed_example_functions]
  }
  catch(const std::exception& ex)
  {
    std::cout << ex.what() << std::endl;
  }
}


/*
//[numeric_limits_output_1
Numeric_limits for type
class boost::fixed_point::negatable<15,-16,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined>
digits10     = 9
max_digits10 = 11
radix        = 2
epsilon      = 3.051757813e-005
max          = 32767.99998
min          = 1.525878906e-005
lowest       = -32768
Type does not have an infinity
Type does not have a NaN

x = fixed_point_type(123) /   100 = 1.22999573
y = fixed_point_type(456) / 10000 = 0.0455932617
x = -x / 2 = -0.614990234
x = frexp(x, &exponential) = -0.614990234 exponential = 0
double the value
x = frexp(x, &exponential) = -0.614990234 exponential = 1
ldexp(x, exponential); = -1.22998047
abs  (x)       = 1.22998047
fabs (x)       = 1.22998047
sqrt (fabs(x)) = 1.10902405
sqrt (y)       = 0.213516235

//[fixed_example_functions
abs  (x)       = 1.22998047
fabs (x)       = 1.22998047
sqrt (fabs(x)) = 1.10902405
sqrt (y)       = 0.213516235
//] [/fixed_example_functions]

*/
