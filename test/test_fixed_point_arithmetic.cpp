///////////////////////////////////////////////////////////////////////////////
//  Copyright Paul Bristow 2015.
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

//! \file
//!\brief Basic tests for fixed_point.

#include <iomanip>
#include <iostream>

#define BOOST_TEST_MODULE fixed_point_basic
#define BOOST_LIB_DIAGNOSTIC
//  Linking to lib file: libboost_unit_test_framework-vc120-mt-gd-1_59.lib

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp> // No Boost.Test library.

//use 6 fractinal bits at least
template <typename fixed_point_type>
void test ()
{
  fixed_point_type a(   8.375);
  fixed_point_type b(  64.125);
  fixed_point_type c( 500.5);
  fixed_point_type d(1024.875);

  BOOST_CHECK_EQUAL(a + b , fixed_point_type (72.5));
  // also checks == operator with right argument as float
  BOOST_CHECK_EQUAL(a + b ,  72.5);

  a += b;
  BOOST_CHECK_EQUAL(a , fixed_point_type(72.5));
  BOOST_CHECK_EQUAL(a , 72.5);

  BOOST_CHECK_EQUAL(a - b ,  fixed_point_type(8.375));
  BOOST_CHECK_EQUAL(a - b ,  8.375);

  a -= b;
  BOOST_CHECK_EQUAL(a ,  fixed_point_type(8.375));
  BOOST_CHECK_EQUAL(a ,  8.375);


  BOOST_CHECK_EQUAL(a * b ,  fixed_point_type(8.375 * 64.125));
  BOOST_CHECK_EQUAL(a * b ,  8.375 * 64.125);
  BOOST_CHECK_EQUAL(a * b ,  fixed_point_type(537.046875));
  BOOST_CHECK_EQUAL(a * b ,  537.046875);

  a *= b;
  BOOST_CHECK_EQUAL(a ,  fixed_point_type(8.375 * 64.125));
  BOOST_CHECK_EQUAL(a ,  8.375 * 64.125);
  BOOST_CHECK_EQUAL(a ,  fixed_point_type(537.046875));
  BOOST_CHECK_EQUAL(a ,  537.046875);

  BOOST_CHECK_EQUAL(a / b ,  fixed_point_type(537.046875 / 64.125));
  BOOST_CHECK_EQUAL(a / b ,  537.046875 / 64.125);
  BOOST_CHECK_EQUAL(a / b ,  fixed_point_type(8.375));
  BOOST_CHECK_EQUAL(a / b ,  8.375);

  a /= b;
  BOOST_CHECK_EQUAL(a ,  fixed_point_type(537.046875 / 64.125));
  BOOST_CHECK_EQUAL(a ,  537.046875 / 64.125);
  BOOST_CHECK_EQUAL(a ,  fixed_point_type(8.375));
  BOOST_CHECK_EQUAL(a ,  8.375);

  fixed_point_type ac(a);
  BOOST_CHECK_EQUAL(ac ,  a);
  ac = a * c;
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type (8.375 * 500.5));
  BOOST_CHECK_EQUAL(ac ,  8.375 * 500.5);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type (4191.6875));
  BOOST_CHECK_EQUAL(ac ,  4191.6875);

  ac = 8.375 * 500.5;
  ac = ac + b + c;
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type((8.375 * 500.5) + 64.125 + 500.5));
  BOOST_CHECK_EQUAL(ac ,  (8.375 * 500.5) + 64.125 + 500.5);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(4756.3125));
  BOOST_CHECK_EQUAL(ac ,  4756.3125);

  ac = a;
  ac = b + c + ac;
  BOOST_CHECK_EQUAL(ac ,  8.375 + 64.125 + 500.5);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(8.375 + 64.125 + 500.5));
  BOOST_CHECK_EQUAL(ac ,  573);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(573));

  ac = ac - b + c;
  BOOST_CHECK_EQUAL(ac ,  8.375 + 64.125 + 500.5 - 64.125 + 500.5);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(8.375 + 64.125 + 500.5 - 64.125 + 500.5));
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(1009.375));
  BOOST_CHECK_EQUAL(ac ,  1009.375);

  ac = a;
  ac = b + c - ac;
  BOOST_CHECK_EQUAL(ac ,  -8.375 + 64.125 + 500.5);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(-8.375 + 64.125 + 500.5));
  BOOST_CHECK_EQUAL(ac ,  556.25);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(556.25));

  ac = a;
  ac = ac * b;
  BOOST_CHECK_EQUAL(ac ,  8.375*64.125);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(8.375 * 64.125));
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(537.046875));
  BOOST_CHECK_EQUAL(ac ,  537.046875);

  ac = a;
  ac *= b * ac;
  BOOST_CHECK_EQUAL(ac ,  8.375*8.375*64.125);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type((8.375 * 8.375) * 64.125));
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(4497.767578125));
  BOOST_CHECK_EQUAL(ac ,  4497.767578125);

  b = 57.578125;
  BOOST_CHECK_EQUAL (b, fixed_point_type(57.578125));

  ac = b;
  ac = ac / a;
  BOOST_CHECK_EQUAL(ac ,  57.578125 / 8.375);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(57.578125 / 8.375));
  BOOST_CHECK_EQUAL(ac ,  6.875);

  ac = b;
  ac /= ac / a;
  BOOST_CHECK_EQUAL(ac ,  57.578125 / (57.578125 / 8.375));
  BOOST_CHECK_EQUAL(ac ,  a);

  ac = a;
  ac = b * (ac + a);
  BOOST_CHECK_EQUAL(ac ,  57.578125 * (16.75));
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(964.43359375));

  ac = a;
  ac = b / (ac * 1);
  BOOST_CHECK_EQUAL(ac ,  57.578125 / 8.375);
  BOOST_CHECK_EQUAL(ac, fixed_point_type(6.875));

  ac = a;
  ac = ac + b;
  BOOST_CHECK_EQUAL(ac ,  8.375 + 57.578125);

  ac = a;
  ac = a + ac;
  BOOST_CHECK_EQUAL(ac ,  16.75);

  ac = a;
  ac = a - ac;
  BOOST_CHECK_EQUAL(ac ,  0);

  ac = a;
  ac += a + b;
  BOOST_CHECK_EQUAL(ac ,  74.328125);

  ac = a;
  ac += b + a;
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(57.578125 + 8.375*2));
  BOOST_CHECK_EQUAL(ac ,  74.328125);

  ac = +a;
  BOOST_CHECK_EQUAL(ac ,  8.375);
  BOOST_CHECK_EQUAL(ac ,  a);

  ac = -a;
  BOOST_CHECK_EQUAL(ac ,  -8.375);
  BOOST_CHECK_EQUAL(ac ,  -1*a);

  ac = 8.375;
  ac = a * ac;
  BOOST_CHECK_EQUAL(ac ,  8.375*8.375);

  ac = a;
  ac += +a;
  BOOST_CHECK_EQUAL(ac ,  16.75);

  ac = a;
  ac += -a;
  BOOST_CHECK_EQUAL(ac ,  0);

  ac = a;
  ac -= +a;
  BOOST_CHECK_EQUAL(ac ,  0);

  ac = a;
  ac -= -a;
  BOOST_CHECK_EQUAL(ac ,  16.75);

  ac = a;
  ac += b - a;
  BOOST_CHECK_EQUAL(ac ,  8.375 + 57.578125 - 8.375);
  BOOST_CHECK_EQUAL(ac ,  b);

  ac = a;
  ac += b*c;
  BOOST_CHECK_EQUAL(ac ,  8.375 + (57.578125 * 500.5));
  BOOST_CHECK_EQUAL(ac, fixed_point_type(28826.2265625));

  ac = a;
  ac -= c - b;
  BOOST_CHECK_EQUAL(ac ,  8.375 - (500.5 - 57.578125));
  BOOST_CHECK_EQUAL(ac , fixed_point_type(-434.546875));
  
  ac = a;
  ac -= b*c;
  BOOST_CHECK_EQUAL(ac ,  8.375 - (500.5 * 57.578125));
  BOOST_CHECK_EQUAL(ac , fixed_point_type(-28809.4765625));

  ac = a;
  ac -= ac * b;
  BOOST_CHECK_EQUAL(ac ,  8.375 - (8.375 * 57.578125));
  BOOST_CHECK_EQUAL(ac , fixed_point_type(-473.841796875));

  ac = a * 8.375;
  ac *= +a;
  BOOST_CHECK_EQUAL(ac ,  70.140625 * 8.375);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(587.427734375));
 
  ac = a;
  ac *= b * c;
  BOOST_CHECK_EQUAL(ac ,  241349.5068359375L);

  ac = a;
  ac *= b / a;
  BOOST_CHECK_EQUAL(ac ,  (8.375 * 57.578125) / 8.375);

  ac = a;
  ac *= b + c;
  BOOST_CHECK_EQUAL(ac ,  8.375 * (57.578125 + 500.5));

  ac = b;
  ac /= +a;
  BOOST_CHECK_EQUAL(ac ,  6.875);

  ac = b;
  ac /= b / a;
  BOOST_CHECK_EQUAL(ac ,  57.578125 / (57.578125 / 8.375));

  ac = b;
  ac /= a + fixed_point_type(0);
  BOOST_CHECK_EQUAL(ac ,  6.875);

  
  //
  // simple tests with immediate values, these calls can be optimised in many backends:
  //
  ac = a + b;
  BOOST_CHECK_EQUAL(ac ,  65.953125);
  ac = a + +b;
  BOOST_CHECK_EQUAL(ac ,  65.953125);
  ac = +a + b;
  BOOST_CHECK_EQUAL(ac ,  65.953125);
  ac = +a + +b;
  BOOST_CHECK_EQUAL(ac ,  65.953125);
  ac = a;
  ac = b / ac;
  BOOST_CHECK_EQUAL(ac ,  b / a);

  //
  // Comparisons:
  //
  BOOST_CHECK_EQUAL((a == b) ,  false);
  BOOST_CHECK_EQUAL((a != b) ,  true);
  BOOST_CHECK_EQUAL((a <= b) ,  true);
  BOOST_CHECK_EQUAL((a <  b) ,  true);
  BOOST_CHECK_EQUAL((a >= b) ,  false);
  BOOST_CHECK_EQUAL((a >  b) ,  false);

  BOOST_CHECK_EQUAL((a+b == b) ,  false);
  BOOST_CHECK_EQUAL((a+b != b) ,  true);
  BOOST_CHECK_EQUAL((a+b >= b) ,  true);
  BOOST_CHECK_EQUAL((a+b >  b) ,  true);
  BOOST_CHECK_EQUAL((a+b <= b) ,  false);
  BOOST_CHECK_EQUAL((a+b <  b) ,  false);

  BOOST_CHECK_EQUAL((a == b+a) ,  false);
  BOOST_CHECK_EQUAL((a != b+a) ,  true);
  BOOST_CHECK_EQUAL((a <= b+a) ,  true);
  BOOST_CHECK_EQUAL((a <  b+a) ,  true);
  BOOST_CHECK_EQUAL((a >= b+a) ,  false);
  BOOST_CHECK_EQUAL((a >  b+a) ,  false);

  BOOST_CHECK_EQUAL((a+b == b+a) ,  true);
  BOOST_CHECK_EQUAL((a+b != b+a) ,  false);
  BOOST_CHECK_EQUAL((a+b <= b+a) ,  true);
  BOOST_CHECK_EQUAL((a+b <  b+a) ,  false);
  BOOST_CHECK_EQUAL((a+b >= b+a) ,  true);
  BOOST_CHECK_EQUAL((a+b >  b+a) ,  false);

  BOOST_CHECK_EQUAL((8.375 == b + a) ,  false);
  BOOST_CHECK_EQUAL((8.375 != b + a) ,  true);
  BOOST_CHECK_EQUAL((8.375 <= b + a) ,  true);
  BOOST_CHECK_EQUAL((8.375 <  b + a) ,  true);
  BOOST_CHECK_EQUAL((8.375 >= b + a) ,  false);
  BOOST_CHECK_EQUAL((8.375 >  b + a) ,  false);

  BOOST_CHECK_EQUAL((800 == b + a) ,  false);
  BOOST_CHECK_EQUAL((800 != b + a) ,  true);
  BOOST_CHECK_EQUAL((800 >= b + a) ,  true);
  BOOST_CHECK_EQUAL((800 >  b + a) ,  true);
  BOOST_CHECK_EQUAL((800 <= b + a) ,  false);
  BOOST_CHECK_EQUAL((800 <  b + a) ,  false);

  BOOST_CHECK_EQUAL((65.953125 == b + a) ,  true);
  BOOST_CHECK_EQUAL((65.953125 != b + a) ,  false);
  BOOST_CHECK_EQUAL((65.953125 <= b + a) ,  true);
  BOOST_CHECK_EQUAL((65.953125 <  b + a) ,  false);
  BOOST_CHECK_EQUAL((65.953125 >= b + a) ,  true);
  BOOST_CHECK_EQUAL((65.953125 >  b + a) ,  false);
}

BOOST_AUTO_TEST_CASE(fixed_point_arithmetic)
{
  test<boost::fixed_point::negatable< 20,  -10, boost::fixed_point::round::fastest> >();
  test<boost::fixed_point::negatable<100,  -10, boost::fixed_point::round::fastest> >();
  test<boost::fixed_point::negatable< 20, -100, boost::fixed_point::round::fastest> >();
  test<boost::fixed_point::negatable<100, -100, boost::fixed_point::round::fastest> >();

  test<boost::fixed_point::negatable< 20,  -10, boost::fixed_point::round::nearest_even> >();
  test<boost::fixed_point::negatable<100,  -10, boost::fixed_point::round::nearest_even> >();
  test<boost::fixed_point::negatable< 20, -100, boost::fixed_point::round::nearest_even> >();
  test<boost::fixed_point::negatable<100, -100, boost::fixed_point::round::nearest_even> >();
}
