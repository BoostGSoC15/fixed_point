
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Christopher Kormanyos 2016.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

// This file also includes Doxygen-style documentation about the function of the code.
// See http://www.doxygen.org for details.

//! \file

//! \brief Example program showing a bare metal real-time performance measurement of a derivative(negatable), 32-bit.

#include <mcal_benchmark.h>
#include <mcal_cpu.h>
#include <mcal_irq.h>
#include <mcal_port.h>

#define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION // Do not use Boost.Multiprecision.
#define BOOST_FIXED_POINT_DISABLE_IOSTREAM       // Do not use I/O streaming.

#include <boost/fixed_point/fixed_point.hpp>

typedef boost::fixed_point::negatable<6, -9> numeric_type;
//typedef float numeric_type;

namespace app
{
  namespace benchmark
  {
    void task_init();
    void task_func();
  }

  typedef mcal::benchmark::benchmark_port_type port_type;
}

namespace local
{
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
}

numeric_type a;
numeric_type b;
numeric_type c;
numeric_type d;

void app::benchmark::task_init()
{
  port_type::set_direction_output();

  a = numeric_type(12U) / 10U;
  b = numeric_type(34U) / 10U;
  c = numeric_type(56U) / 10U;
}

void app::benchmark::task_func()
{
  // Compute the approximate derivative of (a * x^2) + (b * x) + c
  // evaluated at 1/2, where the approximate values of the coefficients
  // are: a = 1.2, b = 3.4, and c = 5.6. The step-size for evaluating
  // the derivative is set to a value of approximately 1/4.

  mcal::irq::disable_all();
  port_type::set_pin_high();

  d = local::first_derivative(numeric_type(1U) / 2U, // x-value
                              numeric_type(1U) / 4U, // step size dx
                              [](const numeric_type& x) -> numeric_type
                              {
                                return (((a * x) + b) * x) + c;
                              });

  port_type::set_pin_low();
  mcal::irq::enable_all();

  // The expected result is ((2 * a) + b) = (2.4 + 3.4) = 4.6 (exact).
  // We obtain a fixed-point result of approximately 4.5938.

  // Verify that the result lies within (4.5 < result < 4.7).
  // The expected result is 4.6, so this is a wide tolerance.

  const bool value_is_ok =    (d > numeric_type(4))
                           && (d < numeric_type(5));

  if(value_is_ok)
  {
    // The benchmark is OK.
    // Perform one nop and leave.

    mcal::cpu::nop();
  }
  else
  {
    // The benchmark result is not OK!
    // Remain in a blocking loop and crash the system.

    for(;;) { mcal::cpu::nop(); }
  }
}
