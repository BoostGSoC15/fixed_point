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
//! Example of listing the numeric_limits of any fixed_point type.
//! (Also works for other types including fundamental floating-point and integral types).

#include <boost/cstdint.hpp>

// Pick out Quickbook code snippets:
//[fixed_point_include_1
#include <boost/fixed_point/fixed_point.hpp>
//] [/fixed_point_include_1]


#include <iostream>
#include <iomanip>
#include <exception>
#include <typeinfo>
#include <limits>
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

  os.precision(std::numeric_limits<T>::digits10);

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
    std::streamsize p = os.precision(3);
    os << "\n epsilon      = " << std::numeric_limits<T>::epsilon();
    //os.precision(p);
  }
  else
  { // is_exact (for example, any integral type) so epsilon has no meaning (and is zero).
    os << "\n exact        = "  << std::numeric_limits<T>::is_exact;
  }

  // Avoid any char values showing as a character or squiggle.
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

  // Several 32-bit splits 
  typedef negatable<15,  -16> fixed_point_type_15m16; // 32-bit even split.
  typedef negatable<11,  -20> fixed_point_type_11m20; // 32-bit higher resolution.
  typedef negatable< 0,  -31> fixed_point_type_0m31; // 32-bit all resolution (no range bit).
  typedef negatable<29,   -2> fixed_point_type_29m2; // 32-bit nearly all range.

  // Types that correspond to key IEEE745 types binary16, 32, 64, 128.
  typedef  negatable<4, -11> fixed_point_type_4m11; // 16-bit IEEE half-precision float.
  // https://en.wikipedia.org/wiki/Half-precision_floating-point_format

  typedef  negatable<7, -24> fixed_point_type_7m24;  //  32-bit float.
  // https://en.wikipedia.org/wiki/Single-precision_floating-point_format

  typedef  negatable<10, -53> fixed_point_type_10m53; // 64-bit double.
  // https://en.wikipedia.org/wiki/Double-precision_floating-point_format

  typedef   negatable<14, -113> fixed_point_type_14m113;  // 128-bit quad type C++ __float128
  // https://en.wikipedia.org/wiki/Quadruple-precision_floating-point_format

  // Some fixed_point types using only a single 8-bit byte (signed char).
  // Popular low digit 8-bit and 16-bit cases.
  typedef   negatable<0, -7> fixed_point_type_0m7; // All 7 bits used for resolution.
  typedef   negatable<2, -5> fixed_point_type_2m5; // 8 bit even split range and resolution.
  typedef  negatable<6, -9>  fixed_point_type_6m9; // 16-bit, even split range and resolution.

  // High precision, high digit count example.
  typedef negatable<15, -240> fixed_point_type_15m240; // 256-bit high precision.
  typedef negatable<0,  -254> fixed_point_type_0m254;  // 256-bit very high precision (no range).
  typedef negatable<200, -54> fixed_point_type_200m54; // 256-bit high range.

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


//[fixed_point_15m16

    // Some fixed_point types using 32 bits, and more.
    show_fixed_point<fixed_point_type_15m16> (); // Even split bits between range and resolution. 
    show_fixed_point<fixed_point_type_11m20> (); // More resolution than range.
    show_fixed_point<fixed_point_type_0m31>  (); // All bits used for resolution.
    show_fixed_point<fixed_point_type_29m2>  (); // Most bits used for range.
    show_fixed_point<fixed_point_type_15m240>(); // 256-bit high precision.
    show_fixed_point<fixed_point_type_0m254>();  // 256-bit very high precision (no range).
    show_fixed_point<fixed_point_type_200m54>(); // 256-bit high range.

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
