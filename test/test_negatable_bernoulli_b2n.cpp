///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for bernoulli_b2n(fixed_point). Along the way, also test numerous fixed_point arithmetic operations.

#define BOOST_TEST_MODULE test_negatable_bernoulli_b2n
#define BOOST_LIB_DIAGNOSTIC

#include <array>
#include <cstddef>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/special_functions/bernoulli.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_bernoulli_b2n)
{
  // Test bernoulli_b2n() for negatable. Compute an array of
  // fixed-point Bernoulli numbers and test them with tabulated
  // control values.

  BOOST_CONSTEXPR_OR_CONST std::size_t number_of_bernoulli_b2n     =   22U;
  BOOST_CONSTEXPR_OR_CONST int         resolution_of_bernoulli_b2n = -280;

  // Here is a table of the text-based control values.
  const char* b2n_float_control[number_of_bernoulli_b2n] =
  {
    "+1.00000000000000000000000000000000000000000000000000000000000000000000000000000000000",
    "+0.16666666666666666666666666666666666666666666666666666666666666666666666666666666667",
    "-0.03333333333333333333333333333333333333333333333333333333333333333333333333333333333",
    "+0.02380952380952380952380952380952380952380952380952380952380952380952380952380952381",
    "-0.03333333333333333333333333333333333333333333333333333333333333333333333333333333333",
    "+0.07575757575757575757575757575757575757575757575757575757575757575757575757575757576",
    "-0.25311355311355311355311355311355311355311355311355311355311355311355311355311355311",
    "+1.16666666666666666666666666666666666666666666666666666666666666666666666666666666667",
    "-7.09215686274509803921568627450980392156862745098039215686274509803921568627450980392",
    "+54.9711779448621553884711779448621553884711779448621553884711779448621553884711779449",
    "-529.124242424242424242424242424242424242424242424242424242424242424242424242424242424",
    "+6192.12318840579710144927536231884057971014492753623188405797101449275362318840579710",
    "-86580.2531135531135531135531135531135531135531135531135531135531135531135531135531136",
    "+1425517.16666666666666666666666666666666666666666666666666666666666666666666666666667",
    "-27298231.0678160919540229885057471264367816091954022988505747126436781609195402298851",
    "+601580873.900642368384303868174835916771400642368384303868174835916771400642368384304",
    "-15116315767.0921568627450980392156862745098039215686274509803921568627450980392156863",
    "+429614643061.166666666666666666666666666666666666666666666666666666666666666666666667",
    "-13711655205088.3327721590879485616327721590879485616327721590879485616327721590879486",
    "+488332318973593.166666666666666666666666666666666666666666666666666666666666666666667",
    "-19296579341940068.1486326681448632668144863266814486326681448632668144863266814486327",
    "+841693047573682615.000553709856035437430786267995570321151716500553709856035437430786"
  };

  // Make a very large fixed-point type with an asymetrically large integral range.
  typedef boost::fixed_point::negatable<2047 + resolution_of_bernoulli_b2n,
                                        resolution_of_bernoulli_b2n,
                                        boost::fixed_point::round::nearest_even>
  fixed_point_type;

  // Define a fixed-point array for storing the Bernoulli numbers.
  std::array<fixed_point_type, number_of_bernoulli_b2n> b2n_fixed;

  // Fill the fixed-point array with even Bernoulli numbers.
  for(int i = 0U; i < int(number_of_bernoulli_b2n); ++i)
  {
    b2n_fixed[i] = boost::math::bernoulli_b2n<fixed_point_type>(i);
  }

  // Set a judiciously selected tolerance for the Bernoulli test.
  const fixed_point_type tolerance = ldexp(fixed_point_type(1), -160);

  // Search for a potential mismatch between the fixed-point
  // Bernoulli numbers and the text-based control values.
  // Here, the control values are first converted to a multiprecision
  // floating-point type and subsequently to a fixed-point type.

  for(std::size_t i = 0U; i < number_of_bernoulli_b2n; ++i)
  {
    typedef fixed_point_type::float_type float_point_type;

    const float_point_type b2n_float(b2n_float_control[i]);

    BOOST_CHECK_CLOSE_FRACTION(b2n_fixed[i], fixed_point_type(b2n_float), tolerance);
  }
}
