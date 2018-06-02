
///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the cylindrical bessel function of (fixed_point) for a big digit range.

#define BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH

#include <array>
#include <string>

#define BOOST_TEST_MODULE test_negatable_math_cyl_bessel_j_big
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/math/special_functions/bessel.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  const std::array<std::string, 5U> reference =
  {{
    std::string("0.0"),
    std::string("0.016726927567521798895947345127480423249960286674824225897992605881766669511923437788365135"),
    std::string("0.064666212504061584587968242077162622540030349155902860156298466433309840096265240063862130"),
    std::string("0.137374802005576240926664325571145301398081392926590576536639459606152507342193024109550044"),
    std::string("0.225017239291023040505824175275621082335681422506071825322128695931202761647524569984079873")
  }};
}

BOOST_AUTO_TEST_CASE(test_negatable_math_cyl_bessel_j_big)
{
  typedef boost::fixed_point::negatable<  std::numeric_limits<int>::digits,
                                        -(255 - std::numeric_limits<int>::digits),
                                        boost::fixed_point::round::nearest_even>
  fixed_point_type;

  typedef fixed_point_type::float_type float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 6);

  // Check small arguments in the region of Taylor series expansion.
  for(int i = 1; i < int(local::reference.size()); ++i)
  {
    // Use an integer-valued order to avoid potentially
    // lossy tgamma calculations (requiring Bernoulli numbers).

    const fixed_point_type x = boost::math::cyl_bessel_j(fixed_point_type(2), fixed_point_type(i) / boost::math::constants::e<fixed_point_type>());
    const float_point_type y = float_point_type(local::reference[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }
}
