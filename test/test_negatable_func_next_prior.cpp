///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the fixed_point functions next, prior, distance, advance.

#define BOOST_TEST_MODULE test_negatable_func_next_prior
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/fixed_point/fixed_point_negatable_next.hpp> // Functions under test.

#include <boost/test/included/unit_test.hpp>

// Basic tests using any type and any value (except max, lowest or -max).
template<typename T>
void tests(T value)
{
  using namespace boost::fixed_point;

  T x(value);
  T xn = fixed_next(x);
  T xp = fixed_prior(x);

  BOOST_CHECK_EQUAL(fixed_prior(xn),  x); // next then prior should be x.
  BOOST_CHECK_EQUAL(fixed_next(xp), x);   // prior then next should be x.

  T xx = fixed_next(xp);

  const bool b = (xx == x);

  BOOST_CHECK_EQUAL(b, true);

  T np = fixed_next(xp);

  //std::cout
  //  << "\n x = " << x
  //  << "x = " << x.bit_pattern() << "\n"
  //  << "np = " << np.bit_pattern() << "\n" // expect same as x
  // // << "xp = " << xp.bit_pattern() << "\n"
  // // << "xn = " << xn.bit_pattern() << "\n"
  //  << std::endl;

  BOOST_CHECK_EQUAL(np.bit_pattern(), x.bit_pattern()); // prior then next should be x.
  BOOST_CHECK_EQUAL(np, x);
  BOOST_CHECK_EQUAL(fixed_distance(xp, xn), 2);
  BOOST_CHECK_EQUAL(fixed_advance(xp, 2), xn);
} // void tests.

BOOST_AUTO_TEST_CASE(test_negatable_func_next_prior)
{
  typedef boost::fixed_point::negatable< 7,   -8> fixed_point_type;        //  16-bit fixed_point type.
  typedef boost::fixed_point::negatable<15,  -48> fixed_point_type_15m48;  //  64-bit fixed_point type.
  typedef boost::fixed_point::negatable<15, -240> fixed_point_type_15m240; // 256-bit using multiprecision
  typedef boost::fixed_point::negatable<55, -200> big_fixed_point_type;    // 256-bit using multiprecision

  using std::nextafter;

  using boost::fixed_point::fixed_next;
  using boost::fixed_point::fixed_prior;
  using boost::fixed_point::fixed_advance;
  using boost::fixed_point::fixed_distance;

  // Simple test using unity and a single fixed_point type.
  fixed_point_type x(1);
  BOOST_CHECK_EQUAL(x.bit_pattern(), "0000000100000000");
  fixed_point_type xn = fixed_next(x);
  BOOST_CHECK_EQUAL(xn.bit_pattern(), "0000000100000001");
  fixed_point_type xp = fixed_prior(x);
  BOOST_CHECK_EQUAL(xp.bit_pattern(), "0000000011111111");
  BOOST_CHECK_EQUAL(fixed_prior(xn), x);
  BOOST_CHECK_EQUAL(fixed_next(xp), x);
  BOOST_CHECK_EQUAL(fixed_distance(xp, xn), 2);
  BOOST_CHECK_EQUAL(fixed_advance(xp, 2), xn);

  // Check that nextafter gives same results.
  // (Useful in case fixed_next etc are not implemented using nextafter).
  BOOST_CHECK_EQUAL(fixed_prior(x), nextafter(x, x-1));
  BOOST_CHECK_EQUAL(fixed_next(x), nextafter(x, x+1));

  // Repeat above tests using multiple types and multiple values.
  tests(fixed_point_type(1));
  tests(fixed_point_type(-1));
  tests(fixed_point_type(0));
  tests((std::numeric_limits<fixed_point_type>::min)()); // small value.

  tests(fixed_prior((std::numeric_limits<fixed_point_type>::max)()));   // penultimate value.
  tests(fixed_next ( std::numeric_limits<fixed_point_type>::lowest())); // penultimate value.

  //tests((std::numeric_limits<fixed_point_type>::max)());    // Expected to fail!
  //tests((std::numeric_limits<fixed_point_type>::lowest)()); // Expected to fail!

  // Use a 64-bit type  fixed_point_type_15m48

  tests(fixed_point_type_15m48(1));
  tests(fixed_point_type_15m48(-1));
  tests(fixed_point_type_15m48(0));
  tests((std::numeric_limits<fixed_point_type>::min)()); // small value.

  tests(fixed_prior((std::numeric_limits<fixed_point_type_15m48>::max)()));   // penultimate value.
  tests(fixed_next((std::numeric_limits<fixed_point_type_15m48>::lowest)())); // penultimate value.

    // Test using a big 256-bit type using multiprecision.
  tests(fixed_point_type_15m240(1));
  tests(fixed_point_type_15m240(-1));
  tests(fixed_point_type_15m240(0));
  // test_negatable_func_next_prior.cpp(31): error : in "test_negatable_func_next_prior": check fixed_next(xp) == x has failed [0 != 0]
  tests((std::numeric_limits<fixed_point_type_15m240>::min)()); // small value.

  tests(fixed_prior((std::numeric_limits<fixed_point_type_15m240>::max)()));   // penultimate value.
  tests(fixed_next((std::numeric_limits<fixed_point_type_15m240>::lowest)())); // penultimate value.

  // Test using a big 256-bit type using multiprecision.
  tests(big_fixed_point_type(1));
  tests(big_fixed_point_type(-1));
  tests(big_fixed_point_type(0));
  tests((std::numeric_limits<big_fixed_point_type>::min)()); // small value.

  tests(fixed_prior((std::numeric_limits<big_fixed_point_type>::max)()));   // penultimate value.
  tests(fixed_next((std::numeric_limits<big_fixed_point_type>::lowest)())); // penultimate value.
} // BOOST_AUTO_TEST_CASE(test_negatable_func_next_prior)
