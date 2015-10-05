///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the hyperbolic arccosine function of (fixed_point) for a big digit range.

#include <array>
#include <cmath>
#include <string>

#define BOOST_TEST_MODULE test_negatable_func_hyperbolic_arccosine_big
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  const std::array<std::string, 16U> reference =
  {{
    std::string("0"),
    std::string("0.778106145550644377111366739615976447074"),
    std::string("1.075752488200409485802391555609856873626"),
    std::string("1.290536532304094183640829393540476233337"),
    std::string("1.462031046249798902436020652270040793246"),
    std::string("1.605910108033730176133032902447716460754"),
    std::string("1.730325807541958959036468259798836958994"),
    std::string("1.840160097979680118879378610736544890913"),
    std::string("1.938605491014298609459620748385845637186"),
    std::string("2.027880282537007725259350493624371150962"),
    std::string("2.109596135288051406880977578274834030021"),
    std::string("2.184964822221038876033477607418923366474"),
    std::string("2.254922631829586235095871486009826209805"),
    std::string("2.320209272625950561857648130716494090736"),
    std::string("2.381420078257271302754568903133336497487"),
    std::string("2.439041768004230859532267223518776047036"),
  }};
}

BOOST_AUTO_TEST_CASE(test_negatable_func_hyperbolic_arccosine_big)
{
  typedef boost::fixed_point::negatable<10, -117> fixed_point_type;
  typedef fixed_point_type::float_type            float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 8);

  const fixed_point_type local_pi = boost::math::constants::pi<fixed_point_type>();

  // Check positive arguments.
  for(int i = 0; i < 16; ++i)
  {
    const fixed_point_type x = acosh(1 + (fixed_point_type(i) / local_pi));
    const float_point_type y = float_point_type(local::reference[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  fixed_point_type x;
  float_point_type y;

  // Check a positive argument clost to 1.
  x = acosh(1 + (fixed_point_type(10) / 97));
  y = float_point_type("0.4502634120238865131827079648245141384230");

  BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
}
