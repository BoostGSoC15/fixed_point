///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for various edge cases of round-tripping for fixed_point for 15 binary digits of precison.

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#define BOOST_TEST_MODULE test_negatable_round_trip_edge_cases
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  template<typename FixedPointType,
           typename FloatPointType>
  FixedPointType test_round_trip(const FloatPointType f)
  {
    const FixedPointType x(static_cast<FloatPointType>(f));

    std::stringstream stream1;

    // Here we use Kahan's formula for digits10.
    //       digits10 =  ((digits - 1) * 301) / 1000
    //   max_digits10 = (((digits - 0) * 301) / 1000) + 1
    stream1 << std::setprecision(std::numeric_limits<FixedPointType>::max_digits10)
            << std::fixed
            << x;

    std::stringstream stream2(stream1.str());

    FixedPointType y;
    stream2 >> y;

    return y;
  }
}

BOOST_AUTO_TEST_CASE(test_negatable_round_trip_edge_cases)
{
  // Use round mode nearest_even becaust this is what
  // the compiler uses for the default rounding mode
  // of its built-in floating-point types.

  {
    // Define the smallest negatable type having 1 decimal digits10.
    typedef boost::fixed_point::negatable<0, -5, boost::fixed_point::round::nearest_even> fixed_point_type;
    typedef fixed_point_type::float_type                                                  float_point_type;

    const float_point_type x = static_cast<float_point_type>(0.9L);
    const fixed_point_type y = local::test_round_trip<fixed_point_type>(x);

    BOOST_CHECK_EQUAL(fixed_point_type(x), y);
  }

  {
    // Define the smallest negatable type having 2 decimal digits10.
    typedef boost::fixed_point::negatable<0, -8, boost::fixed_point::round::nearest_even> fixed_point_type;
    typedef fixed_point_type::float_type                                                  float_point_type;

    const float_point_type x = static_cast<float_point_type>(0.99L);
    const fixed_point_type y = local::test_round_trip<fixed_point_type>(x);

    BOOST_CHECK_EQUAL(fixed_point_type(x), y);
  }

  {
    // Define the smallest negatable type having 4 decimal digits10.
    typedef boost::fixed_point::negatable<0, -15, boost::fixed_point::round::nearest_even> fixed_point_type;
    typedef fixed_point_type::float_type                                                   float_point_type;

    const float_point_type x = static_cast<float_point_type>(0.9999L);
    const fixed_point_type y = local::test_round_trip<fixed_point_type>(x);

    BOOST_CHECK_EQUAL(fixed_point_type(x), y);
  }

  {
    // Define the smallest negatable type having 9 decimal digits10.
    typedef boost::fixed_point::negatable<0, -31, boost::fixed_point::round::nearest_even> fixed_point_type;
    typedef fixed_point_type::float_type                                                   float_point_type;

    const float_point_type x = static_cast<float_point_type>(0.999999999L);
    const fixed_point_type y = local::test_round_trip<fixed_point_type>(x);

    BOOST_CHECK_EQUAL(fixed_point_type(x), y);
  }

  {
    // Define the smallest negatable type having 37 decimal digits10.
    typedef boost::fixed_point::negatable<0, -124, boost::fixed_point::round::nearest_even> fixed_point_type;
    typedef fixed_point_type::float_type                                                    float_point_type;

    const float_point_type x = static_cast<float_point_type>("0.9999999999999999999999999999999999999");
    const fixed_point_type y = local::test_round_trip<fixed_point_type>(x);

    BOOST_CHECK_EQUAL(fixed_point_type(x), y);
  }

  {
    // Define the smallest negatable type having 100 decimal digits10.
    typedef boost::fixed_point::negatable<0, -334, boost::fixed_point::round::nearest_even> fixed_point_type;
    typedef fixed_point_type::float_type                                                    float_point_type;

    const float_point_type x = static_cast<float_point_type>("0.9999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999999");
    const fixed_point_type y = local::test_round_trip<fixed_point_type>(x);

    BOOST_CHECK_EQUAL(fixed_point_type(x), y);
  }
}
