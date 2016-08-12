
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 2015 - 2016.
// Copyright Christopher Kormanyos 2015 - 2016.

// This program computes fixed-point negatable limits for various
// precisions and stores them in partial format for a Quickbook table.

#include <iomanip>
#include <iostream>
#include <limits>

#include <boost/fixed_point/fixed_point.hpp>

template<typename fixed_point_type>
void print_limits()
{
  std::cout << std::setprecision(4)
            << "["
            << "[<" << fixed_point_type::range << ", " << fixed_point_type::resolution << ">] "
            << "["  << fixed_point_type::range << "] "
            << "["  << fixed_point_type::resolution << "] "
            << "["  << std::numeric_limits<fixed_point_type>::digits << "] "
            << "["  << std::numeric_limits<fixed_point_type>::digits +1 << "] "
            << "["  << std::numeric_limits<fixed_point_type>::epsilon() << "] "
            << "["  << std::numeric_limits<fixed_point_type>::lowest() << "] "
            << "["  << (std::numeric_limits<fixed_point_type>::min)() << "] "
            << "["  << (std::numeric_limits<fixed_point_type>::max)() << "]"
            << "]"
            << std::endl
            ;
}

int main()
{
  print_limits<boost::fixed_point::negatable< 15,  -16>>();
  print_limits<boost::fixed_point::negatable< 11,  -20>>();
  print_limits<boost::fixed_point::negatable<  0,  -31>>();
  print_limits<boost::fixed_point::negatable< 30,   -1>>();
  print_limits<boost::fixed_point::negatable< 15, -240>>();
  print_limits<boost::fixed_point::negatable<  0, -255>>();
  print_limits<boost::fixed_point::negatable<200,  -55>>();
  print_limits<boost::fixed_point::negatable<  7,   -8>>();
  print_limits<boost::fixed_point::negatable<  0,   -7>>();
  print_limits<boost::fixed_point::negatable<  2,   -5>>();
  print_limits<boost::fixed_point::negatable<  4,  -11>>();
  print_limits<boost::fixed_point::negatable<  7,  -24>>();
  print_limits<boost::fixed_point::negatable< 10,  -53>>();
  print_limits<boost::fixed_point::negatable< 14, -113>>();
}
