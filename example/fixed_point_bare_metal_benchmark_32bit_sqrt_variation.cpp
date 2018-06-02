
///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2016.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// Example program showing a bare metal real-time performance measurement
// of a shift-and-add variation of sqrt(negatable), 32-bit.

#include <cstdint>
#include <limits>

#include <mcal_benchmark.h>
#include <mcal_cpu.h>
#include <mcal_irq.h>

#define BOOST_FIXED_POINT_DISABLE_IOSTREAM
#define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION

#include <boost/fixed_point/fixed_point.hpp>

typedef mcal::benchmark::benchmark_port_type port_type;

typedef boost::fixed_point::negatable<7, -24> fixed_point_type;

extern const fixed_point_type x;
extern       fixed_point_type y;

namespace shift
{
  fixed_point_type sqrt(const fixed_point_type& x);

  // Computes the fixed-point square root using a shift-and-add algorithm.
  // Adapted from sqrt found here: http://codepad.org/CqvPx5u0
  // And this has beem modified from Ken Turkowski's algorithm.
  // Despite the somewhat confusing description of the result in the white paper,
  // the algorithm builds a M.F fixed-point square root.
  // @see http://www.realitypixels.com/turk/computergraphics/FixedSqrt.pdf
  fixed_point_type sqrt(const fixed_point_type& x)
  {
    typedef fixed_point_type::unsigned_small_type local_unsigned_small_type;
    typedef fixed_point_type::nothing             local_nothing;

    constexpr std::uint_fast8_t total_bits      = static_cast<std::uint_fast8_t>(fixed_point_type::all_bits);
    constexpr std::uint_fast8_t fractional_bits = static_cast<std::uint_fast8_t>(fixed_point_type::radix_split);

    local_unsigned_small_type root  = static_cast<local_unsigned_small_type>(0U);                  // Clear the root.
    local_unsigned_small_type remHi = static_cast<local_unsigned_small_type>(0U);                  // Clear the high part of the partial remainder.
    local_unsigned_small_type remLo = static_cast<local_unsigned_small_type>(x.crepresentation()); // Get the argument into the low part of the partial remainder.

    for(std::uint_fast8_t count = (total_bits >> 1) + (fractional_bits >> 1); count > 0U; --count)
    {
      // Get 2 bits of the argument.
      remHi = (remHi << 2) | (remLo >> (total_bits - 2));
      remLo <<= 2;

      // Get ready for the next bit in the root.
      root <<= 1;

      // Test the radical.
      const local_unsigned_small_type testDiv = (root << 1) + 1;

      if(remHi >= testDiv)
      {
        remHi = remHi - testDiv;

        ++root;
      }
    }

    return fixed_point_type(local_nothing(), root);
  }

} // namespace shift

namespace app
{
  namespace benchmark
  {
    void task_init();
    void task_func();
  }
}

void app::benchmark::task_init()
{
  port_type::set_direction_output();
}

void app::benchmark::task_func()
{
  mcal::irq::disable_all();
  port_type::set_pin_high();

  // Use either one of the following two lines (but not both)
  // to benchmark variations of square root algorithms.

  y = shift::sqrt(x);
  //y = boost::fixed_point::sqrt(x);

  port_type::set_pin_low();

  mcal::irq::enable_all();
}

const fixed_point_type x(fixed_point_type(6U) /  10U);
      fixed_point_type y;
