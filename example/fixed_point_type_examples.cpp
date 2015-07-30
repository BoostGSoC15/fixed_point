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

#include <boost/cstdint.hpp>

//[fixed_point_include_1
#include <boost/fixed_point/fixed_point.hpp>
//] [/fixed_point_include_1]

/*
! Show numeric_limits values for a type.
\tparam T floating-point, fixed-point type.
\param os std::ostream, default @c std::cout
*/

// static int Functions to access template parameters for fixed_point 
// that are missing for fundamental integral and floating-point types.
// Version selected on whether is not arithmetic, is floating_point or is integral.

template <typename NumericalType,
          typename EnableType = void>
struct numerical_details
{
  static int get_range     () { return 0; }
  static int get_resolution() { return 0; }
};

/*! Deduce fixed-point if @c std::is_class (so exclude @c bool, @c int...) and not arithmetic (exclude @c float, @c double...).
*/
template <typename NumericalType>
struct numerical_details<NumericalType,
                         typename std::enable_if<   (std::is_arithmetic<NumericalType>::value == false)
                                                 && (std::is_class<NumericalType>::value      == true)>::type>
{
  static int get_range     () { return NumericalType::range; }
  static int get_resolution() { return NumericalType::resolution; }
};

/*! Deduce fundamental floating-point type @c float, @c double or @c long double. 
*/

template <typename NumericalType>
struct numerical_details<NumericalType,
                         typename std::enable_if<std::is_floating_point<NumericalType>::value>::type>
{
  static int get_range     () { return 0; }
  static int get_resolution() { return std::numeric_limits<NumericalType>::digits; }
};

/*! Deduce fundamental integral type.
*/
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
     << "\n range        = " << numerical_details<T>::get_range() // 
     << "\n resolution   = " << numerical_details<T>::get_resolution()
     << "\n radix        = " << std::numeric_limits<T>::radix  // Always 2 for fixed-point.
     << "\n digits       = " << std::numeric_limits<T>::digits; // Does not include any sign bit.

  if (std::is_signed<T>::value == true)
  {
    os << "\n signed "
       << "\n total bits =   " << std::numeric_limits<T>::digits + 1; // DOES include sign bit.
  }

  if (std::numeric_limits<T>::is_exact == false)
  { // epsilon has meaning.
    os << "\n epsilon      = " << std::numeric_limits<T>::epsilon();
  }
  else
  {
    os << "\n exact        = "  << std::numeric_limits<T>::is_exact;
  }

  // Avoid char values showing as a character or squiggle.
  BOOST_CONSTEXPR_OR_CONST bool is_any_character_type = (   std::is_same<T, signed char>::value 
                                                         || std::is_same<T, unsigned char>::value
                                                         || std::is_same<T, char16_t>::value
                                                         || std::is_same<T, char32_t>::value);

  os << "\n lowest       = ";
  if(is_any_character_type)
  {
    os << static_cast<boost::int32_t>(std::numeric_limits<T>::lowest());
  }
  else
  {
    os << std::numeric_limits<T>::lowest();
  }

  BOOST_CONSTEXPR_OR_CONST bool is_8bit_character_type = (   std::is_same<T, signed char>::value
                                                          || std::is_same<T, unsigned char>::value);

  os << "\n min          = ";
  if(is_8bit_character_type)
  {
    os << static_cast<boost::int32_t>((std::numeric_limits<T>::min)());
  }
  else
  {
    os << (std::numeric_limits<T>::min)();
  }

  os << "\n max          = ";
  if(is_8bit_character_type)
  {
    os << static_cast<boost::int32_t>((std::numeric_limits<T>::max)());
  }
  else
  {
    os << (std::numeric_limits<T>::max)();
  }

  os << "\n max_exponent = " << std::numeric_limits<T>::max_exponent
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
  typedef negatable< 0,  -31> fixed_point_type_0m31;
  typedef negatable<29,   -2> fixed_point_type_29m2;
  typedef negatable<0,  -168> fixed_point_type_0m168;
  typedef negatable<20, -148> fixed_point_type_20m148;

  try
  {
    std::cout.setf(std::ios_base::boolalpha | std::ios_base::showpoint); // Show any trailing zeros.
    std::cout << std::endl;

//[fixed_example_1


    // Show all the significant digits for this particular type.

    // Fundamental (built-in) integral types.
    show_fixed_point<bool>              ();
    show_fixed_point<signed char>       ();
    show_fixed_point<unsigned char>     ();
    show_fixed_point<char16_t>          (); // Shows as type unsigned short.
    show_fixed_point<char32_t>          (); // Shows as type unsigned int.
    show_fixed_point<short int>         ();
    show_fixed_point<unsigned short int>();
    show_fixed_point<int>               ();
    show_fixed_point<unsigned int>      ();

   // Fundamental (built-in) floating-point types.
    show_fixed_point<float>();
    // digits 24 (leaving 8 for decimal exponent).
    // epsilon 1.2e-7.

    show_fixed_point<double>();
    // digits 53 (leaving 10 for decimal exponent).
    // epsilon 2.2e-16.
    show_fixed_point<long double>();
    // Varies with compiler
    // Using MSVC double == long double

//] [/fixed_example_1]


// Some fixed_point types using only a single 8-bit byte (signed char).

//[fixed_point_15m16

    // Some fixed_point types using 32 bits, and more.
    show_fixed_point<fixed_point_type_15m16> (); // Even split bits between range and resolution. 
    show_fixed_point<fixed_point_type_11m20> (); // More resolution than range.
    show_fixed_point<fixed_point_type_0m31>  (); // All bits used for resolution.
    show_fixed_point<fixed_point_type_29m2>  (); // Most bits used for range.
    show_fixed_point<fixed_point_type_0m168> ();
    show_fixed_point<fixed_point_type_20m148>();

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
