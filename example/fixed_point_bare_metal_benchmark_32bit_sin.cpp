
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

//! \brief Example program showing a bare metal real-time performance measurement of sin(negatable), 32-bit.

#include <cmath>
#include <mcal_benchmark.h>
#include <mcal_cpu.h>
#include <mcal_irq.h>
#include <mcal_port.h>

#define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION // Do not use Boost.Multiprecision.
#define BOOST_FIXED_POINT_DISABLE_IOSTREAM       // Do not use I/O streaming.

#include <boost/fixed_point/fixed_point.hpp>

typedef boost::fixed_point::negatable<7, -24> numeric_type;
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

numeric_type x = numeric_type(1) / 2;
numeric_type y;

void app::benchmark::task_init()
{
  port_type::set_direction_output();
}


void app::benchmark::task_func()
{
  using std::sin;

  mcal::irq::disable_all();
  port_type::set_pin_high();

  y = sin(x);

  port_type::set_pin_low();
  mcal::irq::enable_all();

  // sin(1/2) = approx. 0.4794255386042030
  const bool value_is_ok =    (y > (numeric_type(47) / 100))
                           && (y < (numeric_type(48) / 100));

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
