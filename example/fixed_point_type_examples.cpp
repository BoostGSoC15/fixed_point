
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

//[fixed_point_include_1
#include <boost/fixed_point/fixed_point.hpp>
//] [/fixed_point_include_1]

//[fixed_point_typedef_1
typedef boost::fixed_point::negatable<15, -16> fixed_point_type;
//] [/fixed_point_typedef_1]

//typedef boost::fixed_point::negatable<87, -2> fixed_point_type_fastest_round;


//typedef boost::fixed_point::negatable<5, -2> fixed_point_type;
//typedef boost::fixed_point::negatable<15, -15> fixed_point_type;

template <typename T>
void show_fixed_point()
{
  std::cout.precision(std::numeric_limits<T>::max_digits10);

  std::cout << "Numeric_limits of type:"
    << typeid(fixed_point_type).name()
    << "\n radix = " << std::numeric_limits<T>::digits
    << "\n epsilon = " << std::numeric_limits<T>::epsilon()
    << "\n lowest = " << std::numeric_limits<fixed_point_type>::lowest()
    << "\n min = " << std::numeric_limits<T>::min()
    << "\n max = " << std::numeric_limits<T>::max()
    << "\n digits10 = " << std::numeric_limits<T>::digits10
    << "\n max_digits10 = " << std::numeric_limits<T>::max_digits10

    ;
  // TODO add resolution and range to report.
  std::cout << std::endl;

} // template <typename T> void show_fixed_point


int main()
{
  try
  {
    std::cout.setf(std::ios_base::boolalpha | std::ios_base::showpoint); // Show any trailing zeros.
    std::cout << std::endl;

    using boost::fixed_point::negatable;

//[fixed_example_1


    // Show all the significant digits for this particular floating-point type.
    std::cout.precision(std::numeric_limits<fixed_point_type>::digits10);

    show_fixed_point<negatable<15, -15> >();

    //std::cout << "fixed_point_type(123) / 100 = "
    //  << x // 1.22999573 is the nearest representation of decimal digit string 1.23.
    //  << std::endl;


//] [/fixed_example_1]

  }
  catch (std::exception ex)
  {
    std::cout << ex.what() << std::endl;
  }
}



/*
//[fixed_point_type_examples_output_1


//] [/fixed_point_type_examples_output_1]


*/