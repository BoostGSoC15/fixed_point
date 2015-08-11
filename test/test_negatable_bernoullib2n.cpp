///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for bernoulli_b2n(fixed_point). Along the way, also test numerous fixed_point arithmetic operations.

#define BOOST_TEST_MODULE test_negatable_bernoullib2n
#define BOOST_LIB_DIAGNOSTIC

#include <cmath>
#include <iomanip>
#include <iostream>
#include <vector>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/special_functions/bernoulli.hpp>
#include <boost/test/included/unit_test.hpp>

//#define ENABLE_LOCAL_TEST_DEBUG_MESSAGES
#define ENABLE_LOCAL_PATCH_OF_BOOST_TEST

#if defined(ENABLE_LOCAL_PATCH_OF_BOOST_TEST)

  // Patch BOOST_CHECK_CLOSE_FRACTION because of disagreements
  // regarding syntax among various compilers, the implementation
  // of fixed_point, and of Boost.Test.

  template<typename T>
  void boost_check_close_fraction(const T& left, const T& right, const T& tolerance)
  {
    using std::fabs;

    const T fraction = fabs(left / right);
    const T delta    = fabs(1 - fraction);

    const bool the_comparison_result = (delta <= tolerance);

    BOOST_CHECK_EQUAL(the_comparison_result, true);
  }

  #undef  BOOST_CHECK_CLOSE_FRACTION
  #define BOOST_CHECK_CLOSE_FRACTION(LEFT, RIGHT, TOLERANCE) local::boost_check_close_fraction((LEFT), (RIGHT), (TOLERANCE));

#endif // ENABLE_LOCAL_PATCH_OF_BOOST_TEST

BOOST_AUTO_TEST_CASE(test_negatable_bernoullib2n)
{
  // Test bernoulli_b2n() for negatable in one single digit range.
  // Compute 32 fixed-point Bernoulli numbers.
  // Start with Bernoulli number 0.

  typedef boost::fixed_point::negatable<230, -24> fixed_point_type;

  std::vector<fixed_point_type::float_type> bn_fp(32U);

  // Fill the vectors with even Bernoulli numbers.
  boost::math::bernoulli_b2n<fixed_point_type::float_type>(0, 32, std::back_inserter(bn_fp));

  for(size_t i = 0; i < bn_fp.size(); i++)
  {
    std::cout << std::setprecision(std::numeric_limits<fixed_point_type::float_type>::digits10)
              << i*2
              << ",\t"
              << bn_fp[i]
              << std::endl;
  }
}
