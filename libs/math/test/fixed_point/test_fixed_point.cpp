///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Test the fixed_point class.

#include <iomanip>
#include <iostream>
#include <boost/math/fixed_point/fixed_point.hpp>

typedef boost::math::fixed_point::negatable<32, -24> fixed_point_type;

int main()
{
  fixed_point_type x = fixed_point_type(10) / 3;
  float            f = 10.0F / 3.0F;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << x
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << f
            << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << cosh(x)
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << cosh(f)
            << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << cos(x)
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << cos(f)
            << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << log(x)
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float>::digits10)
            << std::fixed << log(f)
            << std::endl;
}
