///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the hyperbolic sine function of (fixed_point) for a big digit range.

#include <array>
#include <string>

#define BOOST_TEST_MODULE test_negatable_func_hyperbolic_sine_big
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  // Table[N[Sinh[Pi / i], 40], {i, 1, 32, 1}]
  const std::array<std::string, 33U> reference =
  {{
    std::string("0.0"),
    std::string("11.548739357257748377977334315388409684500"),
    std::string("2.3012989023072948734630400234344271781780"),
    std::string("1.2493670505239752649541953019279756678010"),
    std::string("0.8686709614860096098969241822753544020225"),
    std::string("0.6704839982471175496972808047369168875077"),
    std::string("0.5478534738880398084757156477174314033528"),
    std::string("0.4640176304929909429808718482622380724268"),
    std::string("0.4028703819170663771015869226825362962574"),
    std::string("0.3561979324000116964713479786377145235264"),
    std::string("0.3193525397881006369819458982539545230796"),
    std::string("0.2894977765418925655585037023549118363477"),
    std::string("0.2648002276022707576980965439494055417277"),
    std::string("0.2440200191004625748942172434829174950780"),
    std::string("0.2262874998305143706957746056749300859577"),
    std::string("0.2109740461465752670709421699118722034840"),
    std::string("0.1976136237368815547798364823495941906360"),
    std::string("0.1858532100433430740697195019675876611939"),
    std::string("0.1754203719360101465712815371913544050051"),
    std::string("0.1661014330795159641984424431232557069927"),
    std::string("0.1577263941715942605580204945807930961666"),
    std::string("0.1501582829910762363022949083475987598270"),
    std::string("0.1432854835283961396262086788722319626378"),
    std::string("0.1370161132076128374437796227034715604396"),
    std::string("0.1312738361127429026421596436542152715101"),
    std::string("0.1259947009965176486874650488220748915826"),
    std::string("0.1211247223685934007765568893288498633977"),
    std::string("0.1166180082580018531257644435489552481008"),
    std::string("0.1124352955051855189452985800008362146193"),
    std::string("0.1085427925538633879191208173230258894093"),
    std::string("0.1049112568617188590673036451243211430523"),
    std::string("0.1015152531699330034105466037995362303199"),
    std::string("0.0983325525214278607270528337843244375196")
  }};
}

BOOST_AUTO_TEST_CASE(test_negatable_func_hyperbolic_sine_big)
{
  typedef boost::fixed_point::negatable<10, -117> fixed_point_type;
  typedef fixed_point_type::float_type            float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 8);

  const fixed_point_type local_pi = boost::math::constants::pi<fixed_point_type>();

  // Check positive arguments.
  for(int i = 1; i < int(local::reference.size()); ++i)
  {
    const fixed_point_type x = sinh(local_pi / fixed_point_type(i));
    const float_point_type y = float_point_type(local::reference[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  // Check negative arguments.
  for(int i = 1; i < int(local::reference.size()); ++i)
  {
    const fixed_point_type x = sinh(-local_pi / fixed_point_type(i));
    const float_point_type y = -float_point_type(local::reference[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }
}
