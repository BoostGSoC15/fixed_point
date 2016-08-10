///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2016.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <iomanip>
#include <iostream>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/special_functions/bessel.hpp>

int main()
{
  // Ensure that the built-in types have sufficient digits.
  static_assert(std::numeric_limits<long double>::digits > std::numeric_limits<int>::digits + 7,
                "Error: Not enough digits are available in long double for this example");

  // Use as many range digits as are available in built-in int.
  typedef boost::fixed_point::negatable<
    std::numeric_limits<int>::digits,        // As many range digits as digits in int.
    -(  std::numeric_limits<long double>::digits  // The remaining resolution digits from long double.
      - std::numeric_limits<int>::digits)>
  fixed_point_type;

  const fixed_point_type v = fixed_point_type(3) / 7;
  const fixed_point_type x = fixed_point_type(1) / 4;

  const fixed_point_type jv = boost::math::cyl_bessel_j(v, x);

  // 0.457863092422485
  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << std::fixed
            << jv
            << std::endl;
}
