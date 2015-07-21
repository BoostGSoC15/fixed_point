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

//! Program to Generate a table of numeric_limits of fixed_point types in 
//! Quickbook format in file modular-boost\libs\fixed_point\doc\fixed_point_types_table.qbk

#include <boost/cstdint.hpp>
#include <boost/version.hpp> 
#include <boost/config.hpp> 
#include <boost/cstdlib.hpp>
#include <boost/fixed_point/fixed_point.hpp>

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <fstream>
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
  static int get_range() { return 0; }
  static int get_resolution() { return 0; }
};

/*! Deduce fixed-point if @c std::is_class (so exclude @c bool, @c int...) and not arithmetic (exclude @c float, @c double...).
*/
template <typename NumericalType>
struct numerical_details<NumericalType,
  typename std::enable_if<   (std::is_arithmetic<NumericalType>::value == false)
  && (std::is_class<NumericalType>::value == true)>::type>
{
  static int get_range() { return NumericalType::range; }
  static int get_resolution() { return NumericalType::resolution; }
};

/*! Deduce fundamental floating-point type @c float, @c double or @c long double.
*/

template <typename NumericalType>
struct numerical_details<NumericalType,
  typename std::enable_if<std::is_floating_point<NumericalType>::value>::type>
{
  static int get_range() { return 0; }
  static int get_resolution() { return std::numeric_limits<NumericalType>::digits; }
};

/*! Deduce fundamental integral type.
*/
template <typename NumericalType>
struct numerical_details<NumericalType,
  typename std::enable_if<std::is_integral<NumericalType>::value>::type>
{
  static int get_range() { return std::numeric_limits<NumericalType>::digits; }
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
    os << "\n exact        = " << std::numeric_limits<T>::is_exact;
  }

  // Avoid any char values showing as a character or squiggle.
  BOOST_CONSTEXPR_OR_CONST bool is_any_character_type = (std::is_same<T, signed char>::value
    || std::is_same<T, unsigned char>::value
    || std::is_same<T, char16_t>::value
    || std::is_same<T, char32_t>::value);

  os << "\n lowest       = ";
  if (is_any_character_type)
  {
    os << static_cast<boost::int32_t>(std::numeric_limits<T>::lowest());
  }
  else
  {
    os << std::numeric_limits<T>::lowest();
  }

  BOOST_CONSTEXPR_OR_CONST bool is_8bit_character_type = (std::is_same<T, signed char>::value
    || std::is_same<T, unsigned char>::value);

  os << "\n min          = ";
  if (is_8bit_character_type)
  {
    os << static_cast<boost::int32_t>((std::numeric_limits<T>::min)());
  }
  else
  {
    os << (std::numeric_limits<T>::min)();
  }

  os << "\n max          = ";
  if (is_8bit_character_type)
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

/*! \brief Output a Quickbook table line of limits for the fixed_point type.
    \details Might construct the typedef from the string name?
    \tparam T Fixed_point type, for example: negatable<15, -16> 
    \tparam name Short name of fixed_point, for example: "negatable<15, -16>"
*/
template <const int res, const int ran>
void fixed_point_limits_line(std::ostream& os = std::cout)
{
  using boost::fixed_point::negatable;

  typedef negatable<res, ran> T; // fixed_point_type; // 

  std::ostringstream s;
  s << "<" << res << "," << ran << ">" << std::flush; // For example: "<0,-30>"
  std::string name = s.str();

  // Avoid any char values showing as a character or squiggle.
  BOOST_CONSTEXPR_OR_CONST bool is_any_character_type = (std::is_same<T, signed char>::value
  || std::is_same<T, unsigned char>::value
  || std::is_same<T, char16_t>::value
  || std::is_same<T, char32_t>::value);

  //os.precision(std::numeric_limits<T>::digits10); // Will overflow page width for high precision.
  os.precision(6);
  //  "[[negatable] [range] [resolution] [bits] [digits] [epsilon] [lowest] [min] [max] ]" << std::endl; // Headings.


  os << "[[" << name << "] [" // For example: "negatable<29, -2>".
    << numerical_details<T>::get_range()
    << "] [" << numerical_details<T>::get_resolution()
    << "] [" << std::numeric_limits<T>::digits // digits.
    << "] [" << std::numeric_limits<T>::digits + 1 // Total bits (+1 for sign bit).
    << "] [" << std::setprecision(3) << std::numeric_limits<T>::epsilon() << std::setprecision(6)
    << "] [";

  // os << "\n lowest       = ";
  if (is_any_character_type)
  {
    os << static_cast<boost::int32_t>(std::numeric_limits<T>::lowest()) << "] [";
  }
  else
  {
    os << std::numeric_limits<T>::lowest() << "] [";
  }

  BOOST_CONSTEXPR_OR_CONST bool is_8bit_character_type = (std::is_same<T, signed char>::value
    || std::is_same<T, unsigned char>::value);

  os.precision(3);
  //os << "\n min          = ";
  if (is_8bit_character_type)
  {
    os << std::setprecision(3)
      << static_cast<boost::int32_t>((std::numeric_limits<T>::min)()) << "] [";
  }
  else
  {
    os << (std::numeric_limits<T>::min)() << "] [";
  }
  os.precision(6);
  //os << "\n max          = ";
  if (is_8bit_character_type)
  {
    os << static_cast<boost::int32_t>((std::numeric_limits<T>::max)()) << "]";
  }
  else
  {
    os << (std::numeric_limits<T>::max)() << "]";
  }

  os << "]" << std::endl;  // End of a table line for a fixed_point type.
} // template <typename T> void fixed_point_limits_line()


void show_floating_point_limits(
  std::string name, // "half", float", double" ...
  int radix, // Assumed for all these types (often called b).
  int bits, // For floating_point types = sign + exponent_bits + fraction_bits + implicit;
  int exponent_bits, // Exponent bits (often called e, or K by Kahan).
  int exponent_bias, // Bias of offset of exponent, 127 for float, 1023 for double.
  int fraction_bits, // Stored fraction bits (excluding implicit).
  int sign, // 0 if signed, else zero if unsigned.
  int implicit  // 1 if implicit, unstored bit, else 0;
  )
{
 // http://www.cs.berkeley.edu/~wkahan/ieee754status/IEEE754.PDF
  // W Kahan Lecture Notes on the Status of IEEE 754 October 1, 1997
  double log10_2 = log10(2.); // == 3010/10000 == 0.3010
  // T log10_2 = log10(static_cast<T>(2));

  int digits; // Total fractional significand bits, including any implicit bit (often called p or N by Kahan).
  digits = fraction_bits + implicit;

  double significand_precision; // log10(2^digits),  = log10(2^11) == 3.311 for 16, log10(2^24) = 7.225 for 32=bit float
  significand_precision = log10(pow(2., digits));
  int digits10; // 
  int max_digits10;
  int max_exponent;
  int max_exponent10;
  int min_exponent;
  int min_exponent10;

  double epsilon;
  double lowest;
  double min;
  // http://stackoverflow.com/questions/4610999/how-to-calculate-double-float-precision
  // where e = bits in exponent or exponent_bits and p = total fractional significand bits,
  double max; // 2 ^ (2^(e-1) * (1 - 2^-p)

  BOOST_ASSERT_MSG(bits == (sign + exponent_bits + fraction_bits + implicit), "bits must be sum of sign + exponent_bits + fraction_bits + implicit!");

  std::cout << "floating-point type " << name << std::endl;
  std::cout << "radix = " << radix << std::endl;
  std::cout << "exponent_bits =  " << exponent_bits << std::endl;
  std::cout << "fraction_bits =  " << fraction_bits << std::endl;
  std::cout << "implicit =  " << implicit << std::endl;
  std::cout << "sign =  " << sign << std::endl;
  std::cout << "digits =  " << digits << std::endl;
  std::cout << "exponent bias =  " << exponent_bias << std::endl;

  max_exponent = static_cast<int>(pow(2., exponent_bits)) - 1;
  std::cout << "biased exponent from 0 to " << max_exponent << std::endl;
  max_exponent =- exponent_bias; // 
  std::cout << "unbiased exponent from " << 1 + exponent_bias << " to " << max_exponent << std::endl;
  std::cout << "max_exponent = " << max_exponent << std::endl;

  min_exponent = pow(2., -exponent_bits); //  
  std::cout << "min_exponent = " << min_exponent << std::endl;
  min_exponent =- exponent_bias -1; // 
  std::cout << "min_exponent = " << min_exponent << std::endl;


  max_exponent10 = log10(pow(2., fraction_bits)); // log10(2^p)
  min_exponent10 = -log10(pow(2., fraction_bits));

  // floor((digits-1) * log10_2);
  std::cout << " At least " << (digits - 1) * log10_2 << " decimal digits" << std::endl;
  digits10 = floor(((digits - 1) * log10_2));  //= log10(2^11) == 3.311 for 16 = 3  
  std::cout << "digits10 = " << digits10 << std::endl;
  // Usually computed at compile-time using this formula:
  digits10 = (digits - 1) * 3010U / 10000U;
  std::cout << "static digits10 = " << digits10 << std::endl;
  // This avoids using floating-point that is not available at compile-time.

  // ceil(((digits) * log10_2) + 2)
  std::cout << " At most " << 1 + digits * log10_2 << " decimal digits" << std::endl;
  max_digits10 = ceil(1 + digits * log10_2); // float = 9
  std::cout << "max_digits10 " << max_digits10 << std::endl;
  // Usually computed at compile-time using this formula:
  max_digits10 = 2 + digits * 3010U / 10000U;
  std::cout << "static max_digits10 = " << max_digits10 << std::endl;
  // This avoids using floating-point that is not available at compile-time.

  epsilon = pow(2., (-digits)); // 2 ^ (N); float = 5.96046e-008
  std::cout << "epsilon =  " << epsilon << std::endl;

  min = pow(2., (2 - pow(2., exponent_bits - exponent_bias)));  // 
  std::cout << "min =  " << min << std::endl;
  // half == 6.10 * 10-5; //  ;
  max = 2 - pow(2., -fraction_bits) * pow(2., exponent_bias); // (2 - 2^-10) * 2^15 = 65504;
  std::cout << "max =  " << max << std::endl;

  std::cout << "lowest =  " << lowest << std::endl;
}



/*! \brief Output a Quickbook table line of limits for a floating_point type.
\details Might construct the typedef from the string name?
\tparam T Floating_point type, for example: float, double.
\tparam name Short name of Floating_point, for example: "double"
*/
template <typename T>
void floating_point_limits_line(std::string name, std::ostream& os = std::cout)
{
  // Avoid any char values showing as a character or squiggle.
  BOOST_CONSTEXPR_OR_CONST bool is_any_character_type = (std::is_same<T, signed char>::value
    || std::is_same<T, unsigned char>::value
    || std::is_same<T, char16_t>::value
    || std::is_same<T, char32_t>::value);

  BOOST_CONSTEXPR_OR_CONST bool is_8bit_character_type = (std::is_same<T, signed char>::value
    || std::is_same<T, unsigned char>::value);

  int exponent_bias;
  int radix = 2; // Assumed for all these types (often called b).
  int bits; // For floating_point types = sign + range + resolution + implicit;
  int range; // exponent bits (often called e).
  int resolution; // Stored fraction bits (excluding implicit).
  int digits; // total fractional significand bits, including any implicit bit Often called p).
  double significand_precision; // log10(2^digits),  = log10(2^11) == 3.311 for 16, log10(2^24) = 7.225 for 32=bit float
  int sign; // 0 if signed, else zero if unsigned.
  int implicit;  // 1 if implicit, unstored bit, else 0;
  int digits10
  int digits10;
  double epsilon;
  double lowest;
  double min;
  // http://stackoverflow.com/questions/4610999/how-to-calculate-double-float-precision
  // where e = bits in exponent or range and p = total fractional significand bits,
  double max; // 2 ^ (2^(e-1) * (1 - 2^-p)

  double log10_2 = 0.3010;
  // See P J Plauger, The Standard C Library, ISBN 0-13-131509-9, Chapter 4 on float.h, page 60;

  if (name == "half")
  {

    significand_precision = log10(pow(2., digits));

    std::cout << significand_precision << " " << floor(significand_precision) << std::endl;

    exponent_bias = 15;
    bits = 16;
    sign = 1;
    range = 5;
    resolution = 10;
    digits = 11; // Including 1 implicit.
    BOOST_STATIC_ASSERT_MSG(bits == (sign + range + resolution + implicit), "bits must be sum of sign + range + resolution + implicit!");

    // floor((digits-1) * log10_2);
    digits10 = floor(((digits - 1) * 301 / 1000));  //= log10(2^11) == 3.311 for 16 = 3  
    // ceil(((digits - 1) * log10_2) + 2)
    max_digits10 = ceil(((digits - 1) * 301 / 1000) + 2); // = 5
    // max_exponent = radix ^ max_exponent
    // max_exponent = 2 ^ 4
    // max_exponent = // digits ^ max_exponent;
    max_exponent = 15; // 15
    min_exponent = -16;
    max_exponent10 = 15; // 15
    min_exponent10 = -16;
    epsilon = pow(2., (15 - resolution)); // 2 ^ (e-10);
    lowest = ; // -3.4e+38;
    min = pow(2., -14);// == 6.10 * 10-5; //  ;
    max = 2 - pow(2., -resolution) * pow(2., exponent_bias); // (2 - 2^-10) * 2^15 = 65504;
  }
  else if (name == "float")
  {
    bits = 32;
    range = 8;
    resolution = 24;
    sign = 1;
    digits = 24;
    digits10 = ((digits - 1) * 301 / 1000;  // = 6
    max_digits10 = ((digits - 1) * 301 / 1000 + 2; // = 9

    epsilon = 1.19e-7;
    lowest = std::numeric_limits<T>::lowest(); //  -3.4e+38;
    min = (std::numeric_limits<T>::min)()); //  1.18e-38;
    max = (std::numeric_limits<T>::max)()); //  3.4 + 38;
  }
  else if (name == "double")
  {
    bits = 64;
  }
  else if (name == "long double")
  {
    bits = 80;
  }
  else if (name == "quad")
  {
    bits = 128;
  }
  //os.precision(std::numeric_limits<T>::digits10); // Will overflow page width for high precision.
  os.precision(3);
  //  "[[negatable] [range] [resolution] [bits] [epsilon] [lowest] [min] [max] ]" << std::endl; // Headings.

  os << "[[" << name
    << "] [" // For example: "double".
    << std::numeric_limits<T>::max_exponent  // == range in bits.
    //  << "] [" << std::numeric_limits<T>::digits + 1  // +1 for hidden implicit bit.
    << "] [" << std::numeric_limits<T>::digits // resolution.
    << "] [" << std::numeric_limits<T>::digits // digits.
    << "] [" << bits  // Total bits.
    << "] [" << std::numeric_limits<T>::epsilon()
    << "] [";

  // os << "\n lowest       = ";
  if (is_any_character_type)
  {
    os << static_cast<boost::int32_t>(std::numeric_limits<T>::lowest()) << "] [";
  }
  else
  {
    os << std::numeric_limits<T>::lowest() << "] [";
  }

  //os << "\n min          = ";
  if (is_8bit_character_type)
  {
    os << static_cast<boost::int32_t>((std::numeric_limits<T>::min)()) << "] [";
  }
  else
  {
    os << os << (std::numeric_limits<T>::min)();
    << "] [";
  }

  //os << "\n max          = ";
  if (is_8bit_character_type)
  {
    os << static_cast<boost::int32_t>((std::numeric_limits<T>::max)()) << "]";
  }
  else
  {
    os << (std::numeric_limits<T>::max)() << "]";
  }
  os << "]" << std::endl;  // End of a table line for a floating_point type.

} // template <typename T> void floating_point_limits_line()


std::string versions()
{ // Build a string of info about Boost, platform, STL, etc.
  std::stringstream mess;
  //mess << "\n" << "Program:\n\" " __FILE__  << "\"\n"; // \n is mis-interpreted!
  mess << "\n" << "Program:\n numeric_limits_qbk.cpp \n";
#ifdef __TIMESTAMP__
  mess << __TIMESTAMP__;
#endif
  mess << "\nBuildInfo:\n" "  Platform " << BOOST_PLATFORM;
  mess << "\n  Compiler " BOOST_COMPILER;
#ifdef _MSC_FULL_VER
  mess << "\n  MSVC version " << BOOST_STRINGIZE(_MSC_FULL_VER) << ".";
#endif
  mess << "\n  STL " BOOST_STDLIB;
  mess << "\n  Boost version " << BOOST_VERSION / 100000 << "." << BOOST_VERSION / 100 % 1000 << "." << BOOST_VERSION % 100 << std::endl;
  return mess.str();
} // std::string versions()

// Quickbook format table file.
std::string fixed_table_filename = "fixed_point_types_table.qbk";
std::string floating_table_filename = "floating_point_types_table.qbk";


int main()
{
  show_floating_point_limits("float", 2, 32, 8, 127, 23, 1, 1);


} // int main()


/*

int main()
{
  using namespace boost::multiprecision;

  using boost::fixed_point::negatable;

  // Several 32-bit splits 
  typedef negatable<15, -16> fixed_point_type_15m16; // 32-bit even split.
  typedef negatable<11, -20> fixed_point_type_11m20; // 32-bit higher resolution.
  typedef negatable< 0, -31> fixed_point_type_0m31; // 32-bit all resolution (no range bit).
  typedef negatable<29, -2> fixed_point_type_29m2; // 32-bit nearly all range.

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
  typedef negatable<0, -254> fixed_point_type_0m254;  // 256-bit very high precision (no range).
  typedef negatable<200, -54> fixed_point_type_200m54; // 256-bit high range.

  try
  {
    std::cout.setf(std::ios_base::boolalpha | std::ios_base::showpoint); // Show any trailing zeros.
    std::cout << std::endl;

    //[fixed_example_1


    // Show all the significant digits for this particular type.

    // Fundamental (built-in) integral types.
    show_fixed_point<bool>();
    show_fixed_point<signed char>();
    show_fixed_point<unsigned char>();
    show_fixed_point<char16_t>(); // Shows as type unsigned short.
    show_fixed_point<char32_t>(); // Shows as type unsigned int.
    show_fixed_point<short int>();
    show_fixed_point<unsigned short int>();
    show_fixed_point<int>();
    show_fixed_point<unsigned int>();

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
    show_fixed_point<fixed_point_type_15m16>(); // Even split bits between range and resolution. 
    show_fixed_point<fixed_point_type_11m20>(); // More resolution than range.
    show_fixed_point<fixed_point_type_0m31>(); // All bits used for resolution.
    show_fixed_point<fixed_point_type_29m2>(); // Most bits used for range.
    show_fixed_point<fixed_point_type_15m240>(); // 256-bit high precision.
    show_fixed_point<fixed_point_type_0m254>();  // 256-bit very high precision (no range).
    show_fixed_point<fixed_point_type_200m54>(); // 256-bit high range.

    {
      // Output table of numeric_limits for various fixed_point types.
    std::cout << versions() << std::endl; // Boost version.

    std::ofstream fout(fixed_table_filename, std::ios_base::out);
    if (!fout.is_open())
    {
      std::cout << "Unable to open file " << fixed_table_filename << " for output.\n" << std::endl;
      return boost::exit_failure;
    }
    fout <<
      "[/""\n"
      "Copyright 2015 Paul A. Bristow.""\n"
      "Copyright 2015 Christopher Kormanyos.""\n"
      "Distributed under the Boost Software License, Version 1.0.""\n"
      "(See accompanying file LICENSE_1_0.txt or copy at""\n"
      "http://www.boost.org/LICENSE_1_0.txt).""\n"
      "]""\n"
      << std::endl;

    fout << "[section:fixed_point_limits" << " Numeric limits for various fixed_point types]\n" << std::endl;

    fout << "[table:fixed_point_types_table Numeric limits for Fixed_point types" "\n";

    fout << "[[negatable] [range] [resolution] [digits] [bits] [epsilon] [lowest] [min] [max]]" << std::endl; // headings.
    fout.precision(3);

    fixed_point_limits_line<15, -16>(fout);
    fixed_point_limits_line<11, -20>(fout);
    fixed_point_limits_line<0, -31>(fout);
    fixed_point_limits_line<29, -2>(fout);
    fixed_point_limits_line<15, -240>(fout);
    fixed_point_limits_line<0, -254>(fout);
    fixed_point_limits_line<200, -54>(fout);
    fixed_point_limits_line<0, -7>(fout);
    fixed_point_limits_line<4, -11>(fout);
    fixed_point_limits_line<7, -24>(fout);
    fixed_point_limits_line<10, -53>(fout);
    fixed_point_limits_line<14, -113>(fout);


    fout << "]" " [/table:fixed_point_types_table]\n";  // end of table;

    fout << "\n"
      "[endsect] [/section:fixed_poin_limits  Numeric limits for various fixed_point types]" "\n" << std::endl;

    fout.close();
    }
    {
      // Repeat for some floating-point IEEE754 formats.

      std::ofstream fout(floating_table_filename, std::ios_base::out);
      if (!fout.is_open())
      {
        std::cout << "Unable to open file " << floating_table_filename << " for output.\n" << std::endl;
        return boost::exit_failure;
      }
      fout <<
        "[/""\n"
        "Copyright 2015 Paul A. Bristow.""\n"
        "Copyright 2015 Christopher Kormanyos.""\n"
        "Distributed under the Boost Software License, Version 1.0.""\n"
        "(See accompanying file LICENSE_1_0.txt or copy at""\n"
        "http://www.boost.org/LICENSE_1_0.txt).""\n"
        "]""\n"
        << std::endl;

      fout << "[section:floating_point_limits  Numeric limits for IEE754 floating_point types]\n" << std::endl;

      fout << "[table:floating_point_types_table Numeric limits for IEEE754 floating_point types" "\n";

      fout << "[[negatable] [range] [resolution] [digits] [bits] [epsilon] [lowest] [min] [max] ]" << std::endl; // Headings.
      fout.precision(3);

      floating_point_limits_line<float>("float", fout);
      floating_point_limits_line<double>("double", fout);
      floating_point_limits_line<long double>("long double", fout);

      fout << "]" " [/table:floating_point_types_table]\n";  // end of table;

      fout << "\n"
        "[endsect] [/section:floating_point_limits  Numeric limits for IEE754 floating_point types]" "\n" << std::endl;

      fout.close();
    }
  }
  catch (std::exception ex)
  {
    std::cout << ex.what() << std::endl;
  }
} // int main()


*/


/*
//[fixed_point_type_examples_output_1


//] [/fixed_point_type_examples_output_1]
*/
