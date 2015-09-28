///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the trogonometric arc-cosine function of (fixed_point) for a big digit range.

#include <array>
#include <string>

#define BOOST_TEST_MODULE test_negatable_trig_func_arccosine_big
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  // Table[N[ArcSin[n / 32], 40], {n, 0, 32, 1}]
  const std::array<std::string, 33U> data =
  {{
    std::string("1.570796326794896619231321691639751442099"),
    std::string("1.539541238295401464547230221746422892356"),
    std::string("1.508255564998405228430720054743370682057"),
    std::string("1.476908451687380140192549010108105709466"),
    std::string("1.445468495626831222356754705282666724050"),
    std::string("1.413903455774435417629811234544692065037"),
    std::string("1.382179940619492523122914646387787293762"),
    std::string("1.350263065874063294418112530588776028408"),
    std::string("1.318116071652817965745664254646040469846"),
    std::string("1.285699886542150397587964692563401885707"),
    std::string("1.252972622867015866047904221003392163982"),
    std::string("1.219888983203815482891178364917172408034"),
    std::string("1.186399552299257536193126818672704468346"),
    std::string("1.152449940351428484220528093112614412493"),
    std::string("1.117979732049971047591990329690051151875"),
    std::string("1.082921179254603695602291637043339055320"),
    std::string("1.047197551196597746154214461093167628066"),
    std::string("1.010721020568314613942629747974843889009"),
    std::string("0.973389910149546404927511325355502798122"),
    std::string("0.935085041393594484569202275820437407924"),
    std::string("0.895664793857864972022265426345363427894"),
    std::string("0.854958266569785398207162909963837219090"),
    std::string("0.812755561368660658774349380658618525342"),
    std::string("0.768793548991278166076389745934093947506"),
    std::string("0.722734247813415611178377352641333362025"),
    std::string("0.674130506667315177542143130774172385349"),
    std::string("0.622368488555020637904549711302383577538"),
    std::string("0.566564330621078109251763866153511999968"),
    std::string("0.505360510284157306971314873987421944504"),
    std::string("0.436469028734919255586187363107401659987"),
    std::string("0.355421201690223492738454683273080571610"),
    std::string("0.250655662336130793749133972714169436096"),
    std::string("0.0")
  }};
}

BOOST_AUTO_TEST_CASE(test_negatable_trig_func_arccosine_big)
{
  typedef boost::fixed_point::negatable<10, -117> fixed_point_type;
  typedef fixed_point_type::float_type            float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 12);

  // Check positive arguments.
  for(int i = 0; i < int(local::data.size() - 1); ++i)
  {
    const fixed_point_type x = acos(fixed_point_type(i) / int(local::data.size() - 1));
    const float_point_type y = float_point_type(local::data[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  // Check negative arguments.
  for(int i = 0; i < int(local::data.size() - 1); ++i)
  {
    const fixed_point_type x = acos(fixed_point_type(-i) / int(local::data.size() - 1));
    const float_point_type y =   float_point_type("3.141592653589793238462643383279502884197")
                               - float_point_type(local::data[i]);
  
    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }
}
