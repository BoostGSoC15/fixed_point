
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
#include <exception>

#include <boost/fixed_point/fixed_point.hpp>

typedef boost::fixed_point::negatable<32, -16> fixed_point_type;
typedef boost::fixed_point::negatable<87, -2> fixed_point_type_fastest_round;

int main()
{
  try
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


    fixed_point_type_fastest_round z = fixed_point_type_fastest_round(123) / 100;

    std::cout << z << std::endl;

  }
  catch (std::exception ex)
  {
    std::cout << ex.what() << std::endl;
  }
}



/*

1>  Description: Autorun "J:\Cpp\fixed_point\Debug\fixed_point_demo_basic.exe"
1>  Numeric_limits: digits10 = 14
1>   max_digits10 = 15
1>   radix = 47
1>   epsilon = 0.0001068115234375
1>   max = 1.4073748835533e+014
1>   min = 1.52587890625e-005
1>   infinity = 0
1>   NaN = 0
1>
1>  1.2299957275391
1>  0.045593261718750
1>  0.61499023437500



GCC  debug

Numeric_limits: digits10 = 14
max_digits10 = 15
radix = 47
epsilon = 0.0001068115234375
max = 1.4073748835533e+014
min = 1.52587890625e-005
infinity = 0
NaN = 0

1.2299957275391
0.045593261718750
0.61499023437500

RUN SUCCESSFUL (total time: 43ms)



GCC release

Numeric_limits: digits10 = 14
max_digits10 = 15
radix = 47
epsilon = 0.0001068115234375
max = 1.4073748835533e+014
min = 1.52587890625e-005
infinity = 0
NaN = 0

1.2299957275391
0.045593261718750
0.61499023437500

RUN SUCCESSFUL (total time: 43ms)




*/