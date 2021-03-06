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

typedef boost::fixed_point::negatable<8, -2, boost::fixed_point::round::negative> fixed_point_type_negative_round;
typedef boost::fixed_point::negatable<8, -2> fixed_point_type_fastest_round;
typedef boost::fixed_point::negatable<8, -2, boost::fixed_point::round::positive> fixed_point_type_positive_round;

int main()
{
  std::cout << "Round towards negative infinity: -1.25 * 1.5\n";
  fixed_point_type_negative_round x = fixed_point_type_negative_round (-1.25) * fixed_point_type_negative_round (1.5);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_negative_round>::digits10)
            << x
            << std::endl;

  std::cout << "Round towards negative infinity: 1.25 * 1.5\n";
  x = fixed_point_type_negative_round (1.25) * fixed_point_type_negative_round (1.5);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_negative_round>::digits10)
            << x
            << std::endl;

  std::cout << "Round towards negative infinity: -1.25 * -1.5\n";
  x = fixed_point_type_negative_round (-1.25) * fixed_point_type_negative_round (-1.5);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_negative_round>::digits10)
            << x
            << std::endl;

  std::cout << "Round fastest: -1.25 * 1.5\n";
  fixed_point_type_fastest_round y = fixed_point_type_fastest_round (-1.25) * fixed_point_type_fastest_round (1.5);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_fastest_round>::digits10)
            << y
            << std::endl;

  std::cout << "Round fastest: 1.25 * 1.5\n";
  y = fixed_point_type_fastest_round (1.25) * fixed_point_type_fastest_round (1.5);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_fastest_round>::digits10)
            << y
            << std::endl;
  /*std::cout << "Round towards negative infinity: -1.24\n";
  x = fixed_point_type_negative_round (-1.24);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_negative_round>::digits10)
            << x
            << std::endl;

  std::cout << "Round towards negative infinity: 1.24\n";
  x = fixed_point_type_negative_round (1.24);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_negative_round>::digits10)
            << x
            << std::endl;*/

  /*std::cout << "Round towards negative infinity: 1.26\n";
  x = fixed_point_type_negative_round (1.26);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_negative_round>::digits10)
            << x
            << std::endl;

  std::cout << "Round towards negative infinity: -1.25\n";
  x = fixed_point_type_negative_round (-1.25);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_negative_round>::digits10)
            << x
            << std::endl;

  std::cout << "Round towards negative infinity: 1.25\n";
  x = fixed_point_type_negative_round (1.25);

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type_negative_round>::digits10)
            << x
            << std::endl;

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
            << std::endl;*/

  /*static assert should fail*/
  /*fixed_point_type_positive_round z*/

}
