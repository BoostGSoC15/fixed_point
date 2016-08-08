///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015 - 2016.
//  Copyright Paul A. Bristow 2015 - 2016.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

// For additional details, see C.M. Kormanyos,
// Real-Time C++: Efficient Object-Oriented and
// Template Microcontroller Programming (Springer, Heidelberg, 2013).
// in Section 12.7 and Chapter 13.
// See http://link.springer.com/chapter/10.1007/978-3-642-34688-0_13

#define USE_BARE_METAL

#include <cstdint>

#if defined(USE_BARE_METAL)
#else
  #include <iostream>
  #include <iomanip>
#endif

// Configure Boost.Fixed_point for a bare-metal system.

#if defined(USE_BARE_METAL)
  #define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION // Do not use Boost.Multiprecision.
  #define BOOST_FIXED_POINT_DISABLE_IOSTREAM       // Do not use I/O streaming.
#endif

#include <boost/fixed_point/fixed_point.hpp>

namespace local
{
  /*! Evaluate first derivative of real_function
      using a three-point central-difference rule of O(dx^6), for more details
     \see http://www.boost.org/doc/libs/release/libs/multiprecision/doc/html/boost_multiprecision/tut/floats/fp_eg/nd.html.

    \tparam RealValueType Type of value, for example, a fixed_point_type.
    \tparam RealFunctionType Type of parameter @c real function.
    \param x Value of x.
    \param dx Step size.
    \param real_function Real Function.
    \return First derivative at x value.
  */

  //[fixed_point_derivative_function
  template<typename RealValueType,
           typename RealFunctionType>
  RealValueType first_derivative(const RealValueType& x,
                                 const RealValueType& dx,
                                 RealFunctionType real_function)
  {
    const RealValueType dx2(dx  + dx);
    const RealValueType dx3(dx2 + dx);

    const RealValueType m1((  real_function(x + dx)
                            - real_function(x - dx))  / 2U);
    const RealValueType m2((  real_function(x + dx2)
                            - real_function(x - dx2)) / 4U);
    const RealValueType m3((  real_function(x + dx3)
                            - real_function(x - dx3)) / 6U);

    const RealValueType fifteen_m1(m1 * 15U);
    const RealValueType six_m2    (m2 *  6U);
    const RealValueType ten_dx    (dx * 10U);

    return ((fifteen_m1 - six_m2) + m3) / ten_dx;
  }
//] [/fixed_point_derivative_function]
} // namespace local

// Implement a tiny simulated subset of the mcal (microcontroller abstraction layer).
namespace mcal
{
  namespace wdg
  {
    void trigger();
  }
} // namespace mcal::wdg

void mcal::wdg::trigger()
{
  // Simulate a fake watchdog trigger mechanism doing nothing here.
}

// Declare a global Boolean test variable.
bool global_result_is_ok;

extern "C" int main()
{
//[fixed_point_derivative_coeffic
  typedef boost::fixed_point::negatable<6, -9> fixed_point_type;

  const fixed_point_type a = fixed_point_type(12) / 10;
  const fixed_point_type b = fixed_point_type(34) / 10;
  const fixed_point_type c = fixed_point_type(56) / 10;

  // Compute the approximate derivative of (a * x^2) + (b * x) + c
  // evaluated at 1/2, where the approximate values of the coefficients
  // are: a = 1.2, b = 3.4, and c = 5.6. The numerical tolerance is set
  // to a value of approximately 1/4.
//] [/fixed_point_derivative_coeffic]
  // See http://link.springer.com/chapter/10.1007/978-3-642-34688-0_12 page 219-220.

//[fixed_point_derivative_evalution
  const fixed_point_type d =
    local::first_derivative(fixed_point_type(1) / 2, // x
                            fixed_point_type(1) / 4,  // Step size dx.
                            [&a, &b, &c](const fixed_point_type& x) -> fixed_point_type
                            {
                              return (((a * x) + b) * x) + c;
                            });
//] [/fixed_point_derivative_evalution]

  // The expected result is ((2 * a) + b) = (2.4 + 3.4) = 4.6 (exact).
  // We obtain a fixed-point result of approximately 4.5938.

  // Verify that the result lies within (4.5 < result < 4.7).
  // The expected result is 4.6, so this is a wide tolerance.

//[fixed_point_verify
  global_result_is_ok = ((d > (fixed_point_type(45) / 10)) && (d < (fixed_point_type(47) / 10)));
//] [/fixed_point_verify]

  #if defined(USE_BARE_METAL)
    // We can not print the fixed-point number to the output stream
    // because I/O-streaming is disabled for fixed-point in this
    // Boost configuration.
  #else
    // But if we could print to the output stream, it might look
    // similar to the lines below. When attempting to print to
    // the output stream, however, we would need to add <iostream>
    // and deactivate #define BOOST_FIXED_POINT_DISABLE_IOSTREAM.
    std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
              << std::fixed
              << d
              << std::endl;
  #endif

  // Is the result of taking the derivative of the quadratic function OK?
  if(global_result_is_ok)
  {
    // Here we could take some action in the microcontroller
    // such as toggle a digital output port to high, indicating
    // success of the test case.
  }

  #if defined(USE_BARE_METAL)
    // In this bare-metal OS-less system, do not return from main().
    for(;;)
    {
      mcal::wdg::trigger();
    }
  #endif
}
