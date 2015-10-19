
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
  template<typename NumericType, typename RealFunctionType>
  NumericType integral(const NumericType& a,
                       const NumericType& b,
                       const NumericType& tol,
                       RealFunctionType real_function)
  {
    boost::uint_fast32_t n2(1);

    NumericType step = ((b - a) / 2U);

    NumericType result = (real_function(a) + real_function(b)) * step;

    const boost::uint_fast8_t k_max = UINT8_C(16);

    for(boost::uint_fast8_t k = UINT8_C(0); k < k_max; ++k)
    {
      NumericType sum(0);

      for(boost::uint_fast32_t j(0U); j < n2; ++j)
      {
        const boost::uint_fast32_t two_j_plus_one = (j * UINT32_C(2)) + UINT32_C(1);

        sum += real_function(a + (step * two_j_plus_one));
      }

      const NumericType tmp = result;

      result = (result / 2U) + (step * sum);

      using std::fabs;
      const NumericType ratio = fabs(tmp / result);
      const NumericType delta = fabs(ratio - 1U);

      if((k > UINT8_C(1)) && (delta < tol))
      {
        break;
      }

      n2 *= 2U;

      step /= 2U;
    }

    return result;
  }

  template<typename NumericType>
  NumericType cyl_bessel_j(const boost::uint_fast8_t n, const NumericType& x)
  {
    using std::sqrt;
    const NumericType tol = sqrt(std::numeric_limits<NumericType>::epsilon());

    const NumericType jn =
      local::integral(NumericType(0),
                      boost::math::constants::pi<NumericType>(),
                      tol,
                      [&x, &n](const NumericType& t) -> NumericType
                      {
                        using std::cos;
                        using std::sin;

                        return cos(x * sin(t) - (t * n));
                      }) / boost::math::constants::pi<NumericType>();

    return jn;
  }
}

BOOST_AUTO_TEST_CASE(test_negatable_math_trapezoid_integral)
{
  typedef boost::fixed_point::negatable<15, -240> fixed_point_type;

  typedef fixed_point_type::float_type float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 6);

  // Compute y = cyl_bessel_j(2, 123 / 100).
  const fixed_point_type j2   = local::cyl_bessel_j(UINT8_C(2), fixed_point_type(123) / 100);

  // Assign the known reference value of the Bessel function.
  const float_point_type reference = float_point_type("0.166369383786814073512678524315131594371033482453328555149562207827319927054822411949870923");

  BOOST_CHECK_CLOSE_FRACTION(j2, reference, tol);
}
