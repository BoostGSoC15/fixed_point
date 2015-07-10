///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Configure Boost.Fixed_point for a bare-metal system.

#define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION // Do not use Boost.Multiprecision.
#define BOOST_FIXED_POINT_DISABLE_IOSTREAM       // Do not use I/O streaming.

#include <boost/fixed_point/fixed_point.hpp>

namespace local
{
  template<typename real_value_type,
           typename real_function_type>
  real_value_type derivative(const real_value_type& x,
                             const real_value_type& dx,
                             real_function_type real_function)
  {
    // Compute the first derivative of the input function
    // using a three-point central-difference rule of O(dx^6).

    const real_value_type dx2(dx  + dx);
    const real_value_type dx3(dx2 + dx);

    const real_value_type m1((  real_function(x + dx)
                              - real_function(x - dx))  / 2U);
    const real_value_type m2((  real_function(x + dx2)
                              - real_function(x - dx2)) / 4U);
    const real_value_type m3((  real_function(x + dx3)
                              - real_function(x - dx3)) / 6U);

    const real_value_type fifteen_m1(m1 * 15U);
    const real_value_type six_m2    (m2 *  6U);
    const real_value_type ten_dx    (dx * 10U);

    return ((fifteen_m1 - six_m2) + m3) / ten_dx;
  }
} // namespace local

bool global_result_is_ok;

namespace mcal { namespace wdg {

void trigger()
{
  // Simulate a fake watchdog trigger mechanism.
}

} } // namespace mcal::wdg

extern "C" int main()
{
  typedef boost::fixed_point::negatable<6, -9> fixed_point_type;

  const fixed_point_type a = fixed_point_type(12) / 10;
  const fixed_point_type b = fixed_point_type(34) / 10;
  const fixed_point_type c = fixed_point_type(56) / 10;

  // Compute the approximate derivative of (a * x^2) + (b * x) + c
  // evaluated at 1/2, where the approximate values of the coefficients
  // are a = 1.2, b = 3.4, and c = 5.6. The numerical tolerance is set
  // to a value of approximately 1/8.

  const fixed_point_type d =
    local::derivative(fixed_point_type(1) / 2,
                      fixed_point_type(1) / 8,
                      [&a, &b, &c](const fixed_point_type& x) -> fixed_point_type
                      {
                        return (((a * x) + b) * x) + c;
                      });

  // The expected result is (2 * a) + b = 2.4 + 3.4 = 4.6 (exact).
  // We obtain a fixed-point result of approximately 4.5938.

  // Verify that the result lies within (4.5 < result < 4.7).
  // The expected result is 4.6, so this is a wide tolerance.
  global_result_is_ok = ((d > (fixed_point_type(45) / 10)) && (d < (fixed_point_type(47) / 10)));

  // We can not print the fixed-point number to the output stream
  // because I/O-streaming is disabled for fixed-point in this
  // Boost configuration.

  // But if we could print to the output stream, it might look
  // similar to the line below.

  //std::cout << std::setprecision(4) << std::fixed << d << std::endl;

  // Initialize the microcontroller abstraction layer.
  if(global_result_is_ok)
  {
    // The result is OK.

    // Here we could take some action in the microcontroller
    // such as toggle a digital output port to high, indicating
    // success of the test case.
  }

  // Do not return from main() in this bare-metal OS-less system.
  for(;;)
  {
    mcal::wdg::trigger();
  }
}
