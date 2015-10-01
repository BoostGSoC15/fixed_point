
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
  typedef boost::fixed_point::negatable<  std::numeric_limits<int>::digits,
                                        -(255 - std::numeric_limits<int>::digits)>
  fixed_point_type;

  const fixed_point_type tol = ldexp(fixed_point_type(1), fixed_point_type::resolution + 12);

  BOOST_CONSTEXPR boost::uintmax_t maximum_allowed_iterations = UINTMAX_C(32);

  boost::uintmax_t max_iter = maximum_allowed_iterations;

  // Find the root of [cos(x) - x] near 0.7 and verify
  // the root to within a reasonable tolerance.

  const std::pair<fixed_point_type, fixed_point_type> root_pair =
   boost::math::tools::toms748_solve([](const fixed_point_type& x) -> fixed_point_type
                                     {
                                       return cos(x) - x;
                                     },
                                     fixed_point_type(1) / 2,
                                     fixed_point_type(2),
                                     [&tol](const fixed_point_type& a, const fixed_point_type& b) -> bool
                                     {
                                       return fabs(a - b) < tol;
                                     },
                                     max_iter);

  BOOST_CHECK_CLOSE_FRACTION(cos(root_pair.first), root_pair.first, tol * 2);

  BOOST_CHECK(max_iter < maximum_allowed_iterations);
}
