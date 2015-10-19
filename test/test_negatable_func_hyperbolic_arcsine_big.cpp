///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the hyperbolic arcsine function of (fixed_point) for a big digit range.

#include <array>
#include <cmath>
#include <string>

#define BOOST_TEST_MODULE test_negatable_func_hyperbolic_arcsine_big
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  const std::array<std::string, 16U> reference =
  {{
    std::string("0.0"),
    std::string("0.313165880450868375871869308265705931525"),
    std::string("0.599971479517856975792313697585794732617"),
    std::string("0.849142301064005897792415416257825875565"),
    std::string("1.062028775243083076923217531618126451719"),
    std::string("1.244496012648449596774577575831508656924"),
    std::string("1.402580998949182131717853001146571301862"),
    std::string("1.541255536739363518029978030875422612076"),
    std::string("1.664356062671940966228932564674639275681"),
    std::string("1.774799028245939273997930913891718748936"),
    std::string("1.874810232032097314614107595146386120132"),
    std::string("1.966107412504205716295922790449411957594"),
    std::string("2.050034326160032850372752472639371293144"),
    std::string("2.127656910626462325673707338485745821428"),
    std::string("2.199832127374864729221420351308227981705"),
    std::string("2.267257608970758470429696365156947858698")
  }};
}

BOOST_AUTO_TEST_CASE(test_negatable_func_hyperbolic_arcsine_big)
{
  typedef boost::fixed_point::negatable<10, -117> fixed_point_type;
  typedef fixed_point_type::float_type            float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 8);

  const fixed_point_type local_pi = boost::math::constants::pi<fixed_point_type>();

  // Check positive arguments.
  for(int i = 0; i < 16; ++i)
  {
    const fixed_point_type x = asinh(fixed_point_type(i) / local_pi);
    const float_point_type y = float_point_type(local::reference[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  // Check negative arguments.
  for(int i = 0; i < 16; ++i)
  {
    const fixed_point_type x = asinh(fixed_point_type(-i) / local_pi);
    const float_point_type y = -float_point_type(local::reference[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  fixed_point_type x;
  float_point_type y;

  // Check the valid zero argument.
  x = asinh(0);
  y = float_point_type(0);

  BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);

  // Check a positive argument clost to 0.
  x = asinh(fixed_point_type(10) / 97);
  y = float_point_type("0.1029110376180219711525763826671313903819");

  BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);

  // Check a negative argument clost to 0.
  x = asinh(fixed_point_type(-10) / 97);
  y = -float_point_type("0.1029110376180219711525763826671313903819");

  BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
}
