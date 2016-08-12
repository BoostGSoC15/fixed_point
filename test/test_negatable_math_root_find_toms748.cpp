
///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the math root-finding utility function TOMS748 of (fixed_point).

#include <utility>

#define BOOST_TEST_MODULE test_negatable_math_root_find_toms748
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/math/tools/roots.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_math_root_find_toms748)
{
  typedef boost::fixed_point::negatable<        std::numeric_limits<int>::digits,
                                        -(255 - std::numeric_limits<int>::digits)>
  fixed_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 8);

  BOOST_CONSTEXPR boost::uintmax_t maximum_allowed_iterations = UINTMAX_C(32);

  boost::uintmax_t iterations = maximum_allowed_iterations;

  // Find the first positive real root of [cos(x) - sqrt(x)] = 0,
  // which is near x = 0.64171437087288265839856530031652237185271781360384.

  using boost::math::tools::toms748_solve;

  const std::pair<fixed_point_type, fixed_point_type> root =
    toms748_solve([](const fixed_point_type& x) -> fixed_point_type
                  {
                    return (cos(x) - sqrt(x));
                  },
                  ldexp(fixed_point_type(1), -1),
                  fixed_point_type(1),
                  [&tol](const fixed_point_type& a, const fixed_point_type& b) -> bool
                  {
                    using std::fabs;

                    return (fabs(a - b) < tol);
                  },
                  iterations);

  // Verify the root with a tight tolerance.

  BOOST_CHECK_CLOSE_FRACTION(cos(root.first), sqrt(root.first), tol * 2);

  BOOST_CHECK(iterations < maximum_allowed_iterations);
}
