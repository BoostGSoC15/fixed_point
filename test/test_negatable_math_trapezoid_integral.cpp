
///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for computing the value of a Bessel function using a trapezoid integration of (fixed_point).

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_math_trapezoid_integral
#define BOOST_LIB_DIAGNOSTIC

#include <boost/cstdint.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  template<typename real_value_type,
           typename real_function_type>
  real_value_type integral(const real_value_type& a,
                           const real_value_type& b,
                           const real_value_type& tol,
                           real_function_type real_function)
  {
    boost::uint_fast32_t n2(1);

    real_value_type step = ((b - a) / 2U);

    real_value_type result = (real_function(a) + real_function(b)) * step;

    const boost::uint_fast8_t k_max = UINT8_C(16);

    for(boost::uint_fast8_t k = UINT8_C(0); k < k_max; ++k)
    {
      real_value_type sum(0);

      for(boost::uint_fast32_t j(0U); j < n2; ++j)
      {
        const boost::uint_fast32_t two_j_plus_one = (j * UINT32_C(2)) + UINT32_C(1);

        sum += real_function(a + (step * two_j_plus_one));
      }

      const real_value_type tmp = result;

      result = (result / 2U) + (step * sum);

      using std::fabs;
      const real_value_type ratio = fabs(tmp / result);
      const real_value_type delta = fabs(ratio - 1U);

      if((k > UINT8_C(1)) && (delta < tol))
      {
        break;
      }

      n2 *= 2U;

      step /= 2U;
    }

    return result;
  }

  template<typename float_type>
  float_type cyl_bessel_j(const boost::uint_fast8_t n,
                          const float_type& x)
  {
    using std::sqrt;
    const float_type tol = sqrt(std::numeric_limits<float_type>::epsilon());

    const float_type jn =
      local::integral(float_type(0),
                      boost::math::constants::pi<float_type>(),
                      tol,
                      [&x, &n](const float_type& t) -> float_type
                      {
                        using std::cos;
                        using std::sin;

                        return cos(x * sin(t) - (t * n));
                      }) / boost::math::constants::pi<float_type>();

    return jn;
  }
}

BOOST_AUTO_TEST_CASE(test_negatable_math_trapezoid_integral)
{
  typedef boost::fixed_point::negatable<15, -240> fixed_point_type;

  typedef fixed_point_type::float_type float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 8);

  // Compute y = cyl_bessel_j(2, 123 / 100) = approx. 0.166369383786814073512678524315131594371033482453328555149562207827319927054822411949870923
  const fixed_point_type j2 = local::cyl_bessel_j(UINT8_C(2), fixed_point_type(123) / 100);

  const float_point_type control = float_point_type("0.166369383786814073512678524315131594371033482453328555149562207827319927054822411949870923");

  BOOST_CHECK_CLOSE_FRACTION(j2, control, tol);
}
