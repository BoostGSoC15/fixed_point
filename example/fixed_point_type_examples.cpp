
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

/*
! Show numeric_limits values for a type.
\tparam T floating-point, fixed-point type.
\param os std::ostream, default @c std::cout
*/

template <typename NumericalType,
          typename EnableType = void>
struct numerical_details
{
  static int get_range     () { return 0; }
  static int get_resolution() { return 0; }
};

template <typename NumericalType>
struct numerical_details<NumericalType,
                         typename std::enable_if<   (std::is_arithmetic<NumericalType>::value == false)
                                                 && (std::is_class<NumericalType>::value      == true)>::type>
{
  static int get_range     () { return NumericalType::range; }
  static int get_resolution() { return NumericalType::resolution; }
};

template <typename NumericalType>
struct numerical_details<NumericalType,
                         typename std::enable_if<std::is_floating_point<NumericalType>::value>::type>
{
  static int get_range     () { return 0; }
  static int get_resolution() { return std::numeric_limits<NumericalType>::digits; }
};

template <typename NumericalType>
struct numerical_details<NumericalType,
                         typename std::enable_if<std::is_integral<NumericalType>::value>::type>
{
  static int get_range     () { return std::numeric_limits<NumericalType>::digits; }
  static int get_resolution() { return 0; }
};

template <typename T>
void show_fixed_point(std::ostream& os = std::cout)
{
  using boost::fixed_point::negatable;

  os.precision(std::numeric_limits<T>::max_digits10);

  os << "Numeric_limits of type: "
     << typeid(T).name()
     << "\n range        = " << numerical_details<T>::get_range()
     << "\n resolution   = " << numerical_details<T>::get_resolution()
     << "\n radix        = " << std::numeric_limits<T>::radix  // Always 2 for fixed-point.
     << "\n digits       = " << std::numeric_limits<T>::digits // Does not include any sign bit.
     << "\n epsilon      = " << std::numeric_limits<T>::epsilon()
     << "\n lowest       = " << std::numeric_limits<T>::lowest()
     << "\n min          = " << std::numeric_limits<T>::min()
     << "\n max          = " << std::numeric_limits<T>::max()
     << "\n max_exponent = " << std::numeric_limits<T>::max_exponent
     << "\n min_exponent = " << std::numeric_limits<T>::min_exponent
     << "\n digits10     = " << std::numeric_limits<T>::digits10
     << "\n max_digits10 = " << std::numeric_limits<T>::max_digits10
     << "\n"
     << std::endl;
} // template <typename T> void show_fixed_point

int main()
{
  using boost::fixed_point::negatable;

  typedef negatable<15,  -16> fixed_point_type_15m16;
  typedef negatable<11,  -20> fixed_point_type_11m20;
  typedef negatable< 0,  -30> fixed_point_type_0m30;
  typedef negatable<29,   -2> fixed_point_type_29m2;
  typedef negatable<0,  -168> fixed_point_type_0m168;
  typedef negatable<20, -148> fixed_point_type_20m148;

  try
  {
    std::cout.setf(std::ios_base::boolalpha | std::ios_base::showpoint); // Show any trailing zeros.
    std::cout << std::endl;

//[fixed_example_1


    // Show all the significant digits for this particular floating-point type.

    show_fixed_point<float>();
    // digits 24 (leaving 8 for decimal exponent).
    // epsilon 1.2e-7.

    show_fixed_point<double>();
    // digits 53 (leaving 10 for decimal exponent).
    // epsilon 2.2e-16.

//] [/fixed_example_1]

//[fixed_point_15m16
    show_fixed_point<fixed_point_type_15m16> ();
    show_fixed_point<fixed_point_type_11m20> ();
    show_fixed_point<fixed_point_type_0m30>  ();
    show_fixed_point<fixed_point_type_29m2>  ();
    show_fixed_point<fixed_point_type_0m168> ();
    show_fixed_point<fixed_point_type_20m148>();
    show_fixed_point<bool>                   ();

    //std::cout << "fixed_point_type(123) / 100 = "
    //  << x // 1.22999573 is the nearest representation of decimal digit string 1.23.
    //  << std::endl;
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
