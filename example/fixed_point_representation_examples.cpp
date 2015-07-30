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

#include <boost/fixed_point/fixed_point.hpp>




//[fixed_point_type_representation_example_3

template <typename T>
void show_representation_types(std::ostream& os = std::cout)
{
  os << "Fixed_point Type "
    << typeid(T).name() 
    // class boost::fixed_point::negatable<7,-1,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined>
    << " with range " << T::range << ", resolution " << T::resolution << std::endl;

  os << "value_type is " << typeid(T::value_type).name() << std::endl; // signed char
  os << "float_type is " << typeid(T::float_type).name() << std::endl; // float
} // template <typename T>void show_representation_types()

//] [/fixed_point_type_representation_example_3]


int main()
{
  using boost::fixed_point::negatable;

  typedef negatable<7, -1> fixed_point_type_7m1;
  typedef negatable<15, -16> fixed_point_type_15m16;
  typedef negatable<0, -15> fixed_point_type_0m15;
  typedef negatable<0, -31> fixed_point_type_0m31;
  typedef negatable<10, -21> fixed_point_type_10m21;
  typedef negatable<0, -63> fixed_point_type_0m63;
  typedef negatable<10, -53> fixed_point_type_10m53;
  typedef negatable<2, -78> fixed_point_type_2m78; 
  typedef negatable<0, -126> fixed_point_type_0m126; // 
  typedef negatable<10, -116> fixed_point_type_10m116; // 
  typedef negatable<0, -254> fixed_point_type_0m254;

  try
  {
    std::cout.setf(std::ios_base::boolalpha | std::ios_base::showpoint); // Show any trailing zeros.
    std::cout << std::endl;

//[fixed_point_type_representation_example_1
     std::cout << typeid(negatable<0, -7>::value_type).name() << std::endl;
//] [/fixed_point_type_representation_example_1]

//[fixed_point_type_representation_example_2
     std::cout << typeid(negatable<0, -7>::float_type).name() << std::endl;
//] [/fixed_point_type_representation_example_2]

   show_representation_types<fixed_point_type_7m1>();
   show_representation_types<fixed_point_type_15m16>();
   show_representation_types<fixed_point_type_0m15>();
   show_representation_types<fixed_point_type_0m31>();
   show_representation_types<fixed_point_type_10m21>();
   show_representation_types<fixed_point_type_0m63>();
   show_representation_types<fixed_point_type_10m53>();
   show_representation_types<fixed_point_type_2m78>();
   show_representation_types<fixed_point_type_0m126>();
   show_representation_types<fixed_point_type_10m116>();
   show_representation_types<fixed_point_type_0m254>();

  }
  catch (std::exception ex)
  {
    std::cout << ex.what() << std::endl;
  }
}

/*
//[fixed_point_representation_examples_output_1

GCC5.1.0

signed char
float
Fixed_point Type class boost::fixed_point::negatable<7,-1,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 7, resolution -1
value_type is short
float_type is float
Fixed_point Type class boost::fixed_point::negatable<15,-16,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 15, resolution -16
value_type is int
float_type is double
Fixed_point Type class boost::fixed_point::negatable<0,-15,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 0, resolution -15
value_type is short
float_type is float
Fixed_point Type class boost::fixed_point::negatable<0,-31,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 0, resolution -31
value_type is int
float_type is double
Fixed_point Type class boost::fixed_point::negatable<10,-21,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 10, resolution -21
value_type is int
float_type is double
Fixed_point Type class boost::fixed_point::negatable<0,-63,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 0, resolution -63
value_type is __int64
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<63,2,void,int,0,0>,0>
Fixed_point Type class boost::fixed_point::negatable<10,-53,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 10, resolution -53
value_type is __int64
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<63,2,void,int,0,0>,0>
Fixed_point Type class boost::fixed_point::negatable<2,-78,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 2, resolution -78
value_type is class boost::multiprecision::number<struct boost::multiprecision::backends::cpp_int_backend<128,128,1,0,void>,0>
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<80,2,void,int,0,0>,0>
Fixed_point Type class boost::fixed_point::negatable<0,-126,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 0, resolution -126
value_type is class boost::multiprecision::number<struct boost::multiprecision::backends::cpp_int_backend<128,128,1,0,void>,0>
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<126,2,void,int,0,0>,0>
Fixed_point Type class boost::fixed_point::negatable<10,-116,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 10, resolution -116
value_type is class boost::multiprecision::number<struct boost::multiprecision::backends::cpp_int_backend<128,128,1,0,void>,0>
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<126,2,void,int,0,0>,0>
Fixed_point Type class boost::fixed_point::negatable<0,-254,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 0, resolution -254
value_type is class boost::multiprecision::number<struct boost::multiprecision::backends::cpp_int_backend<256,256,1,0,void>,0>
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<254,2,void,int,0,0>,0>

//] [/fixed_point_representation_examples_output_1]

//[fixed_point_representation_examples_output_2

VS 2013

Fixed_point Type class boost::fixed_point::negatable<7,-1,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 7, resolution -1
value_type is short
float_type is float
Fixed_point Type class boost::fixed_point::negatable<15,-16,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 15, resolution -16
value_type is int
float_type is double
Fixed_point Type class boost::fixed_point::negatable<0,-15,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 0, resolution -15
value_type is short
float_type is float
Fixed_point Type class boost::fixed_point::negatable<0,-31,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 0, resolution -31
value_type is int
float_type is double
Fixed_point Type class boost::fixed_point::negatable<10,-21,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 10, resolution -21
value_type is int
float_type is double
Fixed_point Type class boost::fixed_point::negatable<0,-63,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 0, resolution -63
value_type is __int64
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<63,2,void,int,0,0>,0>
Fixed_point Type class boost::fixed_point::negatable<10,-53,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 10, resolution -53
value_type is __int64
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<63,2,void,int,0,0>,0>
Fixed_point Type class boost::fixed_point::negatable<2,-78,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 2, resolution -78
value_type is class boost::multiprecision::number<struct boost::multiprecision::backends::cpp_int_backend<128,128,1,0,void>,0>
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<80,2,void,int,0,0>,0>
Fixed_point Type class boost::fixed_point::negatable<0,-126,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 0, resolution -126
value_type is class boost::multiprecision::number<struct boost::multiprecision::backends::cpp_int_backend<128,128,1,0,void>,0>
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<126,2,void,int,0,0>,0>
Fixed_point Type class boost::fixed_point::negatable<10,-116,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 10, resolution -116
value_type is class boost::multiprecision::number<struct boost::multiprecision::backends::cpp_int_backend<128,128,1,0,void>,0>
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<126,2,void,int,0,0>,0>
Fixed_point Type class boost::fixed_point::negatable<0,-254,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 0, resolution -254
value_type is class boost::multiprecision::number<struct boost::multiprecision::backends::cpp_int_backend<256,256,1,0,void>,0>
float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<254,2,void,int,0,0>,0>


//] [/fixed_point_representation_examples_output_2]


*/
