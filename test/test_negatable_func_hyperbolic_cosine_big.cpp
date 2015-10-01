///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the hyperbolic cosine function of (fixed_point) for a big digit range.

#include <array>
#include <string>

#define BOOST_TEST_MODULE test_negatable_func_hyperbolic_cosine_big
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/constants/constants.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  // Table[N[Cosh[Pi / i], 40], {i, 1, 32, 1}]
  const std::array<std::string, 33U> data =
  {{
    std::string("0.0"),
    std::string("11.59195327552152062775175205256013769577"),
    std::string("2.509178478658056782009995643269405948212"),
    std::string("1.600286857702386232519932017924928726163"),
    std::string("1.324609089252005846662845477003383821439"),
    std::string("1.203972089338220800873012107094841916599"),
    std::string("1.140238321076428792141131980379350890767"),
    std::string("1.102412065159090082692731141407961578750"),
    std::string("1.078102288572843594133654670048213751500"),
    std::string("1.061544613780336272437424329767014643661"),
    std::string("1.049755230836746247854541182066207802826"),
    std::string("1.041061459579932562885111960787758243500"),
    std::string("1.034465640095510565271865251179886560960"),
    std::string("1.029342396737737661072102339540621802866"),
    std::string("1.025283391350676781919795755925961741146"),
    std::string("1.022012743632611104779670039917994272201"),
    std::string("1.019338581770758737460540750224159372115"),
    std::string("1.017124090602230794933510501252664352402"),
    std::string("1.015269573507533089914200712250465117153"),
    std::string("1.013700984546759192998303899138682786342"),
    std::string("1.012362393324827689851650565480233030370"),
    std::string("1.011210912693701172426315778190061194188"),
    std::string("1.010213210065066552355973608857473991535"),
    std::string("1.009343061242569486478670216153496664775"),
    std::string("1.008579605211088572225974289984548575155"),
    std::string("1.007906079294693948506469919819238150235"),
    std::string("1.007308889253375362337268174117526806568"),
    std::string("1.006776916625556819987176815122603889990"),
    std::string("1.006300996558851863964359264187065436363"),
    std::string("1.005873519790331801002895864184636076234"),
    std::string("1.005488126143867276444166197308000332421"),
    std::string("1.005139466256377574488598763096161821470"),
    std::string("1.004823014707256478218988448213106514695")
  }};
}

BOOST_AUTO_TEST_CASE(test_negatable_func_hyperbolic_cosine_big)
{
  typedef boost::fixed_point::negatable<10, -117> fixed_point_type;
  typedef fixed_point_type::float_type            float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 8);

  const fixed_point_type local_pi = boost::math::constants::pi<fixed_point_type>();

  // Check positive arguments.
  for(int i = 1; i < int(local::data.size()); ++i)
  {
    const fixed_point_type x = cosh(local_pi / fixed_point_type(i));
    const float_point_type y = float_point_type(local::data[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  // Check negative arguments.
  for(int i = 1; i < int(local::data.size()); ++i)
  {
    const fixed_point_type x = cosh(-local_pi / fixed_point_type(i));
    const float_point_type y = float_point_type(local::data[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }
}
