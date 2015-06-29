
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 2015.
// Copyright Christopher Kormanyos 2015.
// Copyright Nikhar Agrawal 

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
#include <boost/math/special_functions/pow.hpp> 
// file:///I:/modular-boost/libs/math/doc/html/math_toolkit/powers/ct_pow.html

//typedef boost::fixed_point::negatable<5, -2> fixed_point_type;
typedef boost::fixed_point::negatable<0, -7> fixed_point_type;

int main()
{
  try
  {
    std::cout.precision(std::numeric_limits<fixed_point_type>::digits10);
    std::cout.precision(4);

    std::cout << "Numeric_limits:"
      << " digits10 = " << std::numeric_limits<fixed_point_type>::digits10
      << "\n max_digits10 = " << std::numeric_limits<fixed_point_type>::max_digits10
      << "\n radix = " << std::numeric_limits<fixed_point_type>::digits
      << "\n epsilon = " << std::numeric_limits<fixed_point_type>::epsilon()
      << "\n max = " << std::numeric_limits<fixed_point_type>::max()
      << "\n min = " << std::numeric_limits<fixed_point_type>::min();
      std::cout << std::endl;

    std::cout << "Number of possible value is range + abs(resolution) = 2^" << 7 << " = " << boost::math::pow<7>(2) << std::endl;
    if (std::numeric_limits<fixed_point_type>::has_infinity)
    {
      std::cout << "\n infinity = " << std::numeric_limits<fixed_point_type>::infinity();
    }
    if (std::numeric_limits<fixed_point_type>::has_quiet_NaN)
    {
      std::cout << "\n NaN = " << std::numeric_limits<fixed_point_type>::quiet_NaN();
    }
    std::cout << std::endl;

    std::cout << std::boolalpha << std::showpoint 
      //<< std::scientific 
      << std::fixed 
      << std::endl;

    std::cout.precision(std::numeric_limits<fixed_point_type>::max_digits10);

    int p = 2;
    std::cout << "Output ss.precision() = " << p << std::endl;
    std::cout.precision(p);
    std::cout << "std::cout.precision() = " << std::cout.precision() << std::endl; 

    typedef fixed_point_type::float_type floating_point_type;
    int fails = 0;
    int count = 0;
    fixed_point_type x = 0;
    do
    { // Round-trip for all possible fixed-point values *of type negatable<5, -2> *
      std::stringstream ss;
      ss.precision(p);

      ss << std::fixed;
      ss << std::showpoint; // trailing zeros

      ss << x; // write out as string.

      std::cout << "Count " << count << ", precision " << ss.precision() << " wrote = " << x << " " << x.bit_pattern() << " as " << ss.str() << std::flush;

      fixed_point_type y;
      ss >> y; // Read back in.
      std::cout.precision(std::numeric_limits<fixed_point_type>::max_digits10);
      //std::cout << "precision " << std::cout.precision() << std::endl;
      std::cout << std::showpoint << ", read = " << y << " " << y.bit_pattern() << std::endl;

      if (x != y)
      {
        fails++;
        std::cout << "Fail for " << count << " wrote = " << x  << ", read = " << y << std::endl;
      }
      if (x.bit_pattern() != y.bit_pattern())
      {
        fails++;
        std::cout.precision(std::numeric_limits<fixed_point_type>::digits10);
        std::cout << "Fail for " << count << "wrote = " << x << " " << x.bit_pattern();
        std::cout.precision(std::numeric_limits<fixed_point_type>::digits10);
        std::cout << ", read = " << y << " " << y.bit_pattern() << std::endl;
      }


      count++;
      x += std::numeric_limits<fixed_point_type>::epsilon();
    }
    while (x > 0);  // Assumes will roll over to binary 1000000
    
    if (fails != 0)
    {
      std::cout << fails << " fails for precision " << p << std::endl;
    }
  }
  catch (std::exception ex)
  {
    std::cout << ex.what() << std::endl;
  }
}



/*



*/