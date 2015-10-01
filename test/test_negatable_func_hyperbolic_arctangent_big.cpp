
///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the hyperbolic arctangent function of (fixed_point) for a big digit range.

#include <array>
#include <cmath>
#include <string>

#define BOOST_TEST_MODULE test_negatable_func_hyperbolic_arctangent_big
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  const std::array<std::string, 32U> data =
  {{
    std::string("0.0"),
    std::string("0.03126017849066699476401224517264889700963"),
    std::string("0.06258157147700300712676502386220659570756"),
    std::string("0.09402611575146982626142002215372788183409"),
    std::string("0.12565721414045303884256886520093583982895"),
    std::string("0.15754052331994768509117998013193502497880"),
    std::string("0.18974481085245186197776999516126746621606"),
    std::string("0.22234291063072283912360700601773451520078"),
    std::string("0.25541281188299534160275704815183096743906"),
    std::string("0.28903892538757905653000527061360573496702"),
    std::string("0.32331358246252622621932720120028806723143"),
    std::string("0.35833883898506971348612226649007080063728"),
    std::string("0.39422868018213508473059212236947083014805"),
    std::string("0.43111175530193964869111118759169275579160"),
    std::string("0.46913481929646515400813117898257235457370"),
    std::string("0.50846712882692125328570802594952358665392"),
    std::string("0.54930614433405484569762261846126285232375"),
    std::string("0.59188504850420827210735045836882305755054"),
    std::string("0.63648283790644372204808296150459777470706"),
    std::string("0.68343813763139451779564620661516681771521"),
    std::string("0.73316853439671352232912110232139645007889"),
    std::string("0.78619832037687564504126278053196423526914"),
    std::string("0.84319947678511434979248818877069473720844"),
    std::string("0.90505430394812576793610621867313276502616"),
    std::string("0.97295507452765665255267637172158986481854"),
    std::string("1.04857055938961842314945996268359975612389"),
    std::string("1.13434177065918216789401339771969295042351"),
    std::string("1.23404976573580953800764552024675499226887"),
    std::string("1.35402510055110503299800228507435667208655"),
    std::string("1.50613078775260077867807193325154452083410"),
    std::string("1.71699360224257312296458216227117860522497"),
    std::string("2.07156736319576634394792160864411556946603")
  }};
}

BOOST_AUTO_TEST_CASE(test_negatable_func_hyperbolic_arctangent_big)
{
  typedef boost::fixed_point::negatable<10, -117> fixed_point_type;
  typedef fixed_point_type::float_type            float_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 8);

  BOOST_CONSTEXPR int i_max = 32;

  // Check positive arguments.
  for(int i = 1; i < i_max; ++i)
  {
    const fixed_point_type x = atanh(fixed_point_type(i) / i_max);
    const float_point_type y = float_point_type(local::data[i]);

    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  // Check negative arguments.
  for(int i = 1; i < i_max; ++i)
  {
    const fixed_point_type x = atanh(fixed_point_type(-i) / i_max);
    const float_point_type y = -float_point_type(local::data[i]);
  
    BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
  }

  fixed_point_type x;
  float_point_type y;

  // Check the valid zero argument.
  x = atanh(0);
  y = float_point_type(0);

  BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);

  // Check a positive argument clost to 0.
  x = atanh(fixed_point_type(10) / 97);
  y = float_point_type("0.10346035790366122736294077386941984571057");

  BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);

  // Check a negative argument clost to 0.
  x = atanh(fixed_point_type(-10) / 97);
  y = -float_point_type("0.10346035790366122736294077386941984571057");

  BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);

  // Check a positive argument clost to 1.
  x = atanh(1 - (fixed_point_type(1) / 97));
  y = float_point_type("2.63134509445244277592746552669152938209503");

  BOOST_CHECK_CLOSE_FRACTION(x, fixed_point_type(y), tol);
}
