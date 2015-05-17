///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Test the fixed_point::negatable class.

#include <iomanip>
#include <iostream>
#include "fixed_point.hpp"

typedef boost::math::fixed_point::negatable<8, -24> fixed_point_type;

int main()
{
  fixed_point_type x = fixed_point_type(10) / 3;
  double           d = 10.0 / 3.0;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << x
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << d
            << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << cosh(x)
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << std::cosh(d)
            << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << cos(x)
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << std::cos(d)
            << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << log(x)
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << std::log(d)
            << std::endl;
}
