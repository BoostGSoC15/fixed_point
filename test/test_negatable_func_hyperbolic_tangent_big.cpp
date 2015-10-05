///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the hyperbolic tangent function of (fixed_point) for a big digit range.

#include <array>
#include <string>

#define BOOST_TEST_MODULE test_negatable_func_hyperbolic_tangent_big
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  // Table[N[Cosh[Pi / i], 40], {i, 1, 32, 1}]
  const std::array<std::string, 33U> reference =
  {{
    std::string("0.0"),
    std::string("0.9962720762207499442646905800125367118969"),
    std::string("0.9171523356672743463730929214426187753679"),
    std::string("0.7807144353592677128562290348274513189461"),
    std::string("0.6557942026326724356531211426917828879855"),
    std::string("0.5568933069002105824038512757448591302757"),
    std::string("0.4804727781564516058608452870752432259909"),
    std::string("0.4209112410485349131553275757241471123486"),
    std::string("0.3736847479012153464986673462504492005224"),
    std::string("0.3355468322066388105716227372182108589146"),
    std::string("0.3042161928867445404843270396307853144729"),
    std::string("0.2780794292958502937349945221840481925972"),
    std::string("0.2559777892456845394596178407666614764464"),
    std::string("0.2370639933552018379236545395808869652950"),
    std::string("0.2207072715109626261135947866928098681616"),
    std::string("0.2064299564374271058080557115042494704691"),
    std::string("0.1938645581270888369518664141017635607366"),
    std::string("0.1827242238784265934255926563635158280606"),
    std::string("0.1727820635163637785814642364638528895605"),
    std::string("0.1638564385470952152995606520575881880600"),
    std::string("0.1558003292216189618363622875956733359441"),
    std::string("0.1484935349353371070227567679656048669762"),
    std::string("0.1418368737418978150710124512173645908031"),
    std::string("0.1357478130764942764442424718435348514265"),
    std::string("0.1301571392426363984966900036150852438633"),
    std::string("0.1250063905604036165999528592670949238261"),
    std::string("0.1202458587041477629190637448920594964910"),
    std::string("0.1158330175555412903549342094518656086535"),
    std::string("0.1117312771125830093760666538045474033886"),
    std::string("0.1079089869832625386892612772861998907408"),
    std::string("0.1043386332805962385569908029433301399012"),
    std::string("0.1009961866764863912306387222131519518252"),
    std::string("0.0978605695552026229237379567530236424382")
  }};
}

BOOST_AUTO_TEST_CASE(test_negatable_func_hyperbolic_tangent_big)
{
  typedef boost::fixed_point::negatable<10, -117> fixed_point_type;
  typedef fixed_point_type::float_type            float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 8);

  const fixed_point_type local_pi = boost::math::constants::pi<fixed_point_type>();

  // Check positive arguments.
  for(int i = 1; i < int(local::reference.size()); ++i)
  {
    const fixed_point_type x = tanh(local_pi / fixed_point_type(i));
    const float_point_type y = float_point_type(local::reference[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  // Check negative arguments.
  for(int i = 1; i < int(local::reference.size()); ++i)
  {
    const fixed_point_type x = tanh(-local_pi / fixed_point_type(i));
    const float_point_type y = -float_point_type(local::reference[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }
}
