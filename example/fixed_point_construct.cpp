///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Test the fixed_point::negatable class.

#include <iomanip>
#include <iostream>
#include "boost/fixed_point/fixed_point.hpp"

typedef boost::fixed_point::negatable<87, -2> fixed_point_type_fastest_round;

int main()
{
  std::cout << "Round fastest: -1.26\n";
  fixed_point_type_fastest_round y = fixed_point_type_fastest_round (-1.26);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_fastest_round>::digits10)
            << y
            << std::endl;

  std::cout << "Round fastest: -1.24\n";
  y = fixed_point_type_fastest_round (-1.24);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_fastest_round>::digits10)
            << y
            << std::endl;

  std::cout << "Round fastest: 1.24\n";
  y = fixed_point_type_fastest_round (1.24);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_fastest_round>::digits10)
            << y
            << std::endl;

  std::cout << "Round fastest: 1.26\n";
  y = fixed_point_type_fastest_round (1.26);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_fastest_round>::digits10)
            << y
            << std::endl;

  /*static assert should fail*/
  /*fixed_point_type_positive_round z*/

}
