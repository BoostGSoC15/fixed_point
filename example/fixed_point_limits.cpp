
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

#include <iostream>
#include <iomanip>
#include <exception>
#include <typeinfo>
#include <limits>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/special_functions/pow.hpp> 
// file:///I:/modular-boost/libs/math/doc/html/math_toolkit/powers/ct_pow.html

//typedef boost::fixed_point::negatable<5, -2> fixed_point_type;
typedef boost::fixed_point::negatable<15, -15> fixed_point_type;

template <typename T>
void show_fixed_point()
{
  std::cout.precision(std::numeric_limits<T>::max_digits10);
  //std::cout.precision(4);

  std::cout << "Numeric_limits of type:"
    << typeid(fixed_point_type).name() 
    << "\n digits10 = " << std::numeric_limits<T>::digits10
    << "\n max_digits10 = " << std::numeric_limits<T>::max_digits10
    << "\n radix = " << std::numeric_limits<T>::digits
    << "\n epsilon = " << std::numeric_limits<T>::epsilon()
    << "\n lowest = " << std::numeric_limits<fixed_point_type>::lowest()
    << "\n min = " << std::numeric_limits<T>::min()
    << "\n max = " << std::numeric_limits<T>::max();

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
  };
  std::cout << std::endl;

} // template <typename T> void show_fixed_point


int main()
{
  try
  {
    std::cout.precision(std::numeric_limits<fixed_point_type>::max_digits10);
    //std::cout.precision(4);

    // std::cout << "Number of possible value is range + abs(resolution) = 2^" << 15 << " = " << boost::math::pow<15>(2) << std::endl;
    std::cout << "Number of possible value is range + abs(resolution) = 2^" << 15 << " = " << boost::math::pow<15>(2) << std::endl;
    if (std::numeric_limits<fixed_point_type>::has_infinity)
    {
      std::cout << "\n infinity = " << std::numeric_limits<fixed_point_type>::infinity();
    }
    if (std::numeric_limits<fixed_point_type>::has_quiet_NaN)
    {
      std::cout << "\n NaN = " << std::numeric_limits<fixed_point_type>::quiet_NaN();
    }
    std::cout << std::endl;

    show_fixed_point<fixed_point_type>();

    std::cout << std::boolalpha << std::showpoint 
      //<< std::scientific 
      << std::fixed 
      << std::endl;
  }
  catch (std::exception ex)
  {
    std::cout << ex.what() << std::endl;
  }
}



/*



*/