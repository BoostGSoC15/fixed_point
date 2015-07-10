///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Configure Boost.Fixed_point for a bare-metal system
// Do not use multiprecision and do not use I/O streaming.

#define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION
#define BOOST_FIXED_POINT_DISABLE_IOSTREAM

#include <boost/fixed_point/fixed_point.hpp>

namespace local {

template<typename real_value_type,
         typename real_function_type>
real_value_type derivative(const real_value_type& x,
                           const real_value_type& dx,
                           real_function_type real_function)
{
  // Compute the first derivative of the input function
  // using a three-point central difference rule of O(dx^6).

  const real_value_type dx2(dx  + dx);
  const real_value_type dx3(dx2 + dx);

  const real_value_type m1((  real_function(x + dx)
                            - real_function(x - dx))  / 2);
  const real_value_type m2((  real_function(x + dx2)
                            - real_function(x - dx2)) / 4);
  const real_value_type m3((  real_function(x + dx3)
                            - real_function(x - dx3)) / 6);

  const real_value_type fifteen_m1(m1 * 15);
  const real_value_type six_m2    (m2 *  6);
  const real_value_type ten_dx    (dx * 10);

  return ((fifteen_m1 - six_m2) + m3) / ten_dx;
}

template<typename arithmetic_type>
arithmetic_type compute_quadratic_derivative()
{
  const arithmetic_type a = arithmetic_type(12) / 10;
  const arithmetic_type b = arithmetic_type(34) / 10;
  const arithmetic_type c = arithmetic_type(56) / 10;

  // Compute the approximate derivative of (a * x^2) + (b * x) + c
  // evaluated at 1/2, where the approximate values of the coefficients
  // are a = 1.2, b = 3.4 and c = 5.6.

  // The expected answer is 4.6 (exact).

  return derivative(arithmetic_type(1) / 2,
                    arithmetic_type(1) / 8,
                    [&a, &b, &c](const arithmetic_type& x) -> arithmetic_type
                    {
                      return (((a * x) + b) * x) + c;
                    });
}
} // namespace local

bool global_result_is_ok;

extern "C" int main()
{
  typedef boost::fixed_point::negatable<6, -9> fixed_point_type;

  const fixed_point_type d = local::compute_quadratic_derivative<fixed_point_type>();

  global_result_is_ok = ((d > (fixed_point_type(45) / 10)) && (d < (fixed_point_type(47) / 10)));

  // The expected result is 4.6 (exact).
  // We obtain a fixed-point result of 4.5938.

  // We can not print the fixed-point number to the
  // output stream because output streaming is
  // disabled in the Boost configuration.

  //std::cout << std::setprecision(4) << std::fixed << d << std::endl;

  // Initialize the microcontroller abstraction layer.
  if(global_result_is_ok)
  {
    // The result is OK.
  }
}
