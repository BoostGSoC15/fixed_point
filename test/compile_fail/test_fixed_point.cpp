///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Test the fixed_point::negatable class.

#include <iomanip>
#include <iostream>
#include "boost/math/fixed_point/fixed_point.hpp"

typedef boost::math::fixed_point::negatable<28, -4> fixed_point_type;

int main()
{
  fixed_point_type x = fixed_point_type(10)/3;
  double           d = 10.0 / 3.0;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << x
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<double>::digits10)
            << std::fixed << d
            << std::endl;

}
