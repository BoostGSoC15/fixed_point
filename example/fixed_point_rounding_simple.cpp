///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2016.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Test the fixed_point::negatable class.

#include <iomanip>
#include <iostream>
#include <boost/fixed_point/fixed_point.hpp>

typedef boost::fixed_point::negatable<2, -2, boost::fixed_point::round::fastest>      fixed_point_type_round_fastest;
typedef boost::fixed_point::negatable<2, -2, boost::fixed_point::round::nearest_even> fixed_point_type_round_nearest_even;

int main()
{
  fixed_point_type_round_fastest      x;
  fixed_point_type_round_nearest_even y;

  for(float input = 1.0F; input < 2.125F; input += 0.125F)
  {
    y = input;
    x = input;

    std::cout << "Input: "
              << std::setprecision(3)
              << std::fixed
              << input
              << 'F'
              << " : fastest "
              << std::setprecision(2)
              << x
              << " : nearest_even "
              << y
              << std::endl;

  }
}

// The output of the program is:
// 
// Input: 1.000F : fastest 1.00 : nearest_even 1.00
// Input: 1.125F : fastest 1.00 : nearest_even 1.00
// Input: 1.250F : fastest 1.25 : nearest_even 1.25
// Input: 1.375F : fastest 1.25 : nearest_even 1.50
// Input: 1.500F : fastest 1.50 : nearest_even 1.50
// Input: 1.625F : fastest 1.50 : nearest_even 1.50
// Input: 1.750F : fastest 1.75 : nearest_even 1.75
// Input: 1.875F : fastest 1.75 : nearest_even 2.00
// Input: 2.000F : fastest 2.00 : nearest_even 2.00
