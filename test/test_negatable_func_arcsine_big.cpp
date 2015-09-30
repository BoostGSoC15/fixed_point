///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the trogonometric arcsine function of (fixed_point) for a big digit range.

#include <array>
#include <string>

#define BOOST_TEST_MODULE test_negatable_func_arcsine_big
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  // Table[N[ArcSin[n / 32], 40], {n, 0, 32, 1}]
  const std::array<std::string, 33U> data =
  {{
    std::string("0.0"),
    std::string("0.031255088499495154684091469893328549743"),
    std::string("0.062540761796491390800601636896380760041"),
    std::string("0.093887875107516479038772681531645732633"),
    std::string("0.125327831168065396874566986357084718048"),
    std::string("0.156892871020461201601510457095059377061"),
    std::string("0.188616386175404096108407045251964148337"),
    std::string("0.220533260920833324813209161050975413690"),
    std::string("0.252680255142078653485657436993710972252"),
    std::string("0.285096440252746221643356999076349556392"),
    std::string("0.317823703927880753183417470636359278116"),
    std::string("0.350907343591081136340143326722579034065"),
    std::string("0.384396774495639083038194872967046973753"),
    std::string("0.418346386443468135010793598527137029606"),
    std::string("0.452816594744925571639331361949700290223"),
    std::string("0.487875147540292923629030054596412386779"),
    std::string("0.523598775598298873077107230546583814033"),
    std::string("0.560075306226582005288691943664907553090"),
    std::string("0.597406416645350214303810366284248643977"),
    std::string("0.635711285401302134662119415819314034175"),
    std::string("0.675131532937031647209056265294388014204"),
    std::string("0.715838060225111221024158781675914223009"),
    std::string("0.758040765426235960456972310981132916757"),
    std::string("0.802002777803618453154931945705657494593"),
    std::string("0.848062078981481008052944338998418080073"),
    std::string("0.896665820127581441689178560865579056749"),
    std::string("0.948427838239875981326771980337367864561"),
    std::string("1.004231996173818509979557825486239442130"),
    std::string("1.065435816510739312260006817652329497594"),
    std::string("1.134327298059977363645134328532349782111"),
    std::string("1.215375125104673126492867008366670870489"),
    std::string("1.320140664458765825482187718925582006002"),
    std::string("1.570796326794896619231321691639751442099")
  }};
}

BOOST_AUTO_TEST_CASE(test_negatable_func_arcsine_big)
{
  typedef boost::fixed_point::negatable<10, -117> fixed_point_type;
  typedef fixed_point_type::float_type            float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 12);

  // Check positive arguments.
  for(int i = 0; i < int(local::data.size() - 1); ++i)
  {
    const fixed_point_type x = asin(fixed_point_type(i) / int(local::data.size() - 1));
    const float_point_type y = float_point_type(local::data[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  // Check negative arguments.
  for(int i = 0; i < int(local::data.size() - 1); ++i)
  {
    const fixed_point_type x = asin(fixed_point_type(-i) / int(local::data.size() - 1));
    const float_point_type y = -float_point_type(local::data[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }
}
