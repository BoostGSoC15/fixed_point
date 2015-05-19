
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 2015.
// Copyright Christopher Kormanyos 2015.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

// This file also includes Doxygen-style documentation about the function of the code.
// See http://www.doxygen.org for details.

//! \file

// Below are snippets of code that can be included into a Quickbook program.

#include <iomanip>
#include <iostream>

#include <boost/fixed_point/fixed_point.hpp>

typedef boost::math::fixed_point::negatable<32, -16> fixed_point_type;

int main()
{
  std::cout.precision(std::numeric_limits<fixed_point_type>::digits10);

  std::cout << "Numeric_limits:"
    << " digits10 = " << std::numeric_limits<fixed_point_type>::digits10
    << "\n max_digits10 = " << std::numeric_limits<fixed_point_type>::max_digits10
    << "\n radix = " << std::numeric_limits<fixed_point_type>::digits
    << "\n epsilon = " << std::numeric_limits<fixed_point_type>::epsilon()
    << "\n max = " << std::numeric_limits<fixed_point_type>::max()
    << "\n min = " << std::numeric_limits<fixed_point_type>::min()
    << "\n infinity = " << std::numeric_limits<fixed_point_type>::infinity()
    << "\n NaN = " << std::numeric_limits<fixed_point_type>::quiet_NaN()
    << std::endl;


  std::cout << std::boolalpha << std::showpoint << std::endl;

  fixed_point_type x = fixed_point_type(123) / 100;
  fixed_point_type y = fixed_point_type(456) / 10000;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
    << x
    << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
    << y
    << std::endl;

  x = x / 2;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
    << x
    << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
    << sin(x)
    << std::endl;

  std::cout << std::setprecision(std::numeric_limits<double>::digits10)
    << sin(1.23)
    << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
    << acosh(x)
    << std::endl;

  std::cout << std::setprecision(std::numeric_limits<double>::digits10)
    << acosh(1.23)
    << std::endl;
}


/*

Numeric_limits: digits10 = 31
 max_digits10 = 0
 radix = 31
 epsilon = 0.0001068115234375
 max = 32767.999938964844
 min = 1.52587890625e-005
 infinity = 32767.999908447266
 NaN = 32767.999923706055

1.229995727539062500000000000000
0.04559326171875000000000000000000
0.6149902343750000000000000000000
0.5769500732421875000000000000000
0.942488801931697
32767.99992370605500000000000000
0.665863529156555

EXIT STATUS: 0

*/