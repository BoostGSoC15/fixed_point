///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//! \file
//!\brief Test for a successful bare-metal configuration of fixed_point.

// For further mathematical details pertaining to this file,
// see C.M. Kormanyos, Real-Time C++: Efficient Object-Oriented and
// Template Microcontroller Programming (Springer, Heidelberg, 2013).
// in Section 12.7 and Chapter 13.
// See http://link.springer.com/chapter/10.1007/978-3-642-34688-0_13

#define BOOST_TEST_MODULE test_negatable_bare_metal_config
#define BOOST_LIB_DIAGNOSTIC

// Configure Boost.Fixed_point for a bare-metal system.

#define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION // Do not use Boost.Multiprecision.
#define BOOST_FIXED_POINT_DISABLE_IOSTREAM       // Do not use I/O streaming.

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  template<typename RealValueType,
           typename RealFunctionType>
  RealValueType first_derivative(const RealValueType& x,
                                 const RealValueType& dx,
                                 RealFunctionType real_function)
  {
    // Evaluate first derivative of real_function using
    // a three-point central-difference rule of O(dx^6).

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
} // namespace local

BOOST_AUTO_TEST_CASE(test_negatable_bare_metal_config)
{
  typedef boost::fixed_point::negatable<6, -9> fixed_point_type;

  const fixed_point_type a = fixed_point_type(12) / 10;
  const fixed_point_type b = fixed_point_type(34) / 10;
  const fixed_point_type c = fixed_point_type(56) / 10;

  // Compute the approximate derivative of [(a * x^2) + (b * x) + c]
  // evaluated at 1/2, where the approximate values of the coefficients
  // are: a = 1.2, b = 3.4, and c = 5.6. The numerical tolerance is set
  // to a value of approximately 1/4.

  const fixed_point_type d =
    local::first_derivative(fixed_point_type(1) / 2,  // x
                            fixed_point_type(1) / 4,  // Step size dx.
                            [&a, &b, &c](const fixed_point_type& x) -> fixed_point_type
                            {
                              return (((a * x) + b) * x) + c;
                            });

  // The expected result is ((2 * a) + b) = (1.2 + 3.4) = 4.6 (exact).
  // We obtain a fixed-point result of approximately 4.5938.

  // Verify that the result lies within (4.5 < result < 4.7).
  // The expected result is 4.6, so this is a wide tolerance.

  // This is the test of the result that has been used
  // on the bare-metal system.
  const bool result_is_ok = ((d > (fixed_point_type(45) / 10)) && (d < (fixed_point_type(47) / 10)));

  BOOST_CHECK_EQUAL(result_is_ok, true);

  // Now we perform a second test of the result based on a
  // close-fraction test. We do not use BOOST_CHECK_CLOSE_FRACTION()
  // here becasue that would require I/O streaming, which is
  // disabled in this bare-metal configuration.
  const fixed_point_type fraction = fabs(d / fixed_point_type(4.6F));
  const fixed_point_type delta    = fabs(1 - fraction);

  const bool result_is_within_tolerance = (delta <= ldexp(fixed_point_type(1), fixed_point_type::resolution + 2));

  BOOST_CHECK_EQUAL(result_is_within_tolerance, true);
}
