
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

numeric_type x = numeric_type(6) / 10;
numeric_type y;

void app::benchmark::task_init()
{
  port_type::set_direction_output();
}


void app::benchmark::task_func()
{
  using std::sqrt;

  mcal::irq::disable_all();
  port_type::set_pin_high();

  y = sqrt(x);

  port_type::set_pin_low();
  mcal::irq::enable_all();

  // sqrt(6/10) = approx. 0.7745966692414834
  const bool value_is_ok =    (y > (numeric_type(77) / 100))
                           && (y < (numeric_type(78) / 100));

  if(value_is_ok)
  {
  }
  else
  {
    for(;;) { ; }
  }
}
