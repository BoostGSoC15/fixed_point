
///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2016.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include <cmath>
#include <mcal_benchmark.h>
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
  using std::atan;

  mcal::irq::disable_all();
  port_type::set_pin_high();

  y = atan(x);

  port_type::set_pin_low();
  mcal::irq::enable_all();

  // atan(1/2) = approx. 0.4636476090008061
  const bool value_is_ok =    (y > (numeric_type(46) / 100))
                           && (y < (numeric_type(47) / 100));

  if(value_is_ok)
  {
  }
  else
  {
    for(;;) { ; }
  }
}
