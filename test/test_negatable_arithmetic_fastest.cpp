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

#define BOOST_TEST_MODULE test_negatable_arithmetic
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

//use 6 fractinal bits at least
template <typename fixed_point_type>
void test ()
{
  fixed_point_type a(   8.375L);
  fixed_point_type b(  64.125L);
  fixed_point_type c( 500.5L);
  fixed_point_type d(1024.875L);

  BOOST_CHECK_EQUAL(a + b , fixed_point_type (72.5L));
  // also checks == operator with right argument as float
  BOOST_CHECK_EQUAL(a + b ,  72.5L);

  a += b;
  BOOST_CHECK_EQUAL(a , fixed_point_type(72.5L));
  BOOST_CHECK_EQUAL(a , 72.5L);

  BOOST_CHECK_EQUAL(a - b ,  fixed_point_type(8.375L));
  BOOST_CHECK_EQUAL(a - b ,  8.375L);

  a -= b;
  BOOST_CHECK_EQUAL(a ,  fixed_point_type(8.375));
  BOOST_CHECK_EQUAL(a ,  8.375);

  BOOST_CHECK_EQUAL(a * b ,  fixed_point_type(8.375L * 64.125L));
  BOOST_CHECK_EQUAL(a * b ,  8.375L * 64.125L);
  BOOST_CHECK_EQUAL(a * b ,  fixed_point_type(537.046875L));
  BOOST_CHECK_EQUAL(a * b ,  537.046875L);

  a *= b;
  BOOST_CHECK_EQUAL(a ,  fixed_point_type(8.375L * 64.125L));
  BOOST_CHECK_EQUAL(a ,  8.375L * 64.125L);
  BOOST_CHECK_EQUAL(a ,  fixed_point_type(537.046875L));
  BOOST_CHECK_EQUAL(a ,  537.046875L);

  BOOST_CHECK_EQUAL(a / b ,  fixed_point_type(537.046875L / 64.125L));
  BOOST_CHECK_EQUAL(a / b ,  537.046875L / 64.125L);
  BOOST_CHECK_EQUAL(a / b ,  fixed_point_type(8.375L));
  BOOST_CHECK_EQUAL(a / b ,  8.375L);

  a /= b;
  BOOST_CHECK_EQUAL(a ,  fixed_point_type(537.046875L / 64.125L));
  BOOST_CHECK_EQUAL(a ,  537.046875L / 64.125L);
  BOOST_CHECK_EQUAL(a ,  fixed_point_type(8.375L));
  BOOST_CHECK_EQUAL(a ,  8.375L);

  fixed_point_type ac(a);
  BOOST_CHECK_EQUAL(ac ,  a);
  ac = a * c;
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type (8.375L * 500.5L));
  BOOST_CHECK_EQUAL(ac ,  8.375L * 500.5L);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type (4191.6875L));
  BOOST_CHECK_EQUAL(ac ,  4191.6875L);

  ac = 8.375L * 500.5L;
  ac = ac + b + c;
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type((8.375L * 500.5L) + 64.125L + 500.5L));
  BOOST_CHECK_EQUAL(ac ,  (8.375L * 500.5L) + 64.125L + 500.5L);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(4756.3125L));
  BOOST_CHECK_EQUAL(ac ,  4756.3125L);

  ac = a;
  ac = b + c + ac;
  BOOST_CHECK_EQUAL(ac ,  8.375L + 64.125L + 500.5L);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(8.375L + 64.125L + 500.5L));
  BOOST_CHECK_EQUAL(ac ,  573.0L);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(573.0L));

  ac = ac - b + c;
  BOOST_CHECK_EQUAL(ac ,  8.375L + 64.125L + 500.5L - 64.125L + 500.5L);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(8.375L + 64.125L + 500.5L - 64.125L + 500.5L));
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(1009.375L));
  BOOST_CHECK_EQUAL(ac ,  1009.375L);

  ac = a;
  ac = b + c - ac;
  BOOST_CHECK_EQUAL(ac ,  -8.375L + 64.125L + 500.5L);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(-8.375L + 64.125L + 500.5L));
  BOOST_CHECK_EQUAL(ac ,  556.25L);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(556.25L));

  ac = a;
  ac = ac * b;
  BOOST_CHECK_EQUAL(ac ,  8.375L*64.125L);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(8.375L * 64.125L));
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(537.046875L));
  BOOST_CHECK_EQUAL(ac ,  537.046875L);

  ac = a;
  ac *= b * ac;
  BOOST_CHECK_EQUAL(ac ,  8.375L*8.375L*64.125L);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type((8.375L * 8.375L) * 64.125L));
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(4497.767578125L));
  BOOST_CHECK_EQUAL(ac ,  4497.767578125L);

  b = 57.578125L;
  BOOST_CHECK_EQUAL (b, fixed_point_type(57.578125L));

  ac = b;
  ac = ac / a;
  BOOST_CHECK_EQUAL(ac ,  57.578125L / 8.375L);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(57.578125L / 8.375L));
  BOOST_CHECK_EQUAL(ac ,  6.875L);

  ac = b;
  ac /= ac / a;
  BOOST_CHECK_EQUAL(ac ,  57.578125L / (57.578125L / 8.375L));
  BOOST_CHECK_EQUAL(ac ,  a);

  ac = a;
  ac = b * (ac + a);
  BOOST_CHECK_EQUAL(ac ,  57.578125L * (16.75L));
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(964.43359375L));

  ac = a;
  ac = b / (ac * 1);
  BOOST_CHECK_EQUAL(ac ,  57.578125L / 8.375L);
  BOOST_CHECK_EQUAL(ac, fixed_point_type(6.875L));

  ac = a;
  ac = ac + b;
  BOOST_CHECK_EQUAL(ac ,  8.375L + 57.578125L);

  ac = a;
  ac = a + ac;
  BOOST_CHECK_EQUAL(ac ,  16.75L);

  ac = a;
  ac = a - ac;
  BOOST_CHECK_EQUAL(ac ,  0.0L);

  ac = a;
  ac += a + b;
  BOOST_CHECK_EQUAL(ac ,  74.328125L);

  ac = a;
  ac += b + a;
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(57.578125L + 8.375L*2.0L));
  BOOST_CHECK_EQUAL(ac ,  74.328125L);

  ac = +a;
  BOOST_CHECK_EQUAL(ac ,  8.375L);
  BOOST_CHECK_EQUAL(ac ,  a);

  ac = -a;
  BOOST_CHECK_EQUAL(ac ,  -8.375L);
  BOOST_CHECK_EQUAL(ac ,  -1*a);

  ac = 8.375L;
  ac = a * ac;
  BOOST_CHECK_EQUAL(ac ,  8.375L*8.375L);

  ac = a;
  ac += +a;
  BOOST_CHECK_EQUAL(ac ,  16.75L);

  ac = a;
  ac += -a;
  BOOST_CHECK_EQUAL(ac ,  0.0L);

  ac = a;
  ac -= +a;
  BOOST_CHECK_EQUAL(ac ,  0.0L);

  ac = a;
  ac -= -a;
  BOOST_CHECK_EQUAL(ac ,  16.75L);

  ac = a;
  ac += b - a;
  BOOST_CHECK_EQUAL(ac ,  8.375L + 57.578125L - 8.375L);
  BOOST_CHECK_EQUAL(ac ,  b);

  ac = a;
  ac += b*c;
  BOOST_CHECK_EQUAL(ac ,  8.375L + (57.578125L * 500.5L));
  BOOST_CHECK_EQUAL(ac, fixed_point_type(28826.2265625L));

  ac = a;
  ac -= c - b;
  BOOST_CHECK_EQUAL(ac ,  8.375L - (500.5L - 57.578125L));
  BOOST_CHECK_EQUAL(ac , fixed_point_type(-434.546875L));
  
  ac = a;
  ac -= b*c;
  BOOST_CHECK_EQUAL(ac ,  8.375L - (500.5L * 57.578125L));
  BOOST_CHECK_EQUAL(ac , fixed_point_type(-28809.4765625L));

  ac = a;
  ac -= ac * b;
  BOOST_CHECK_EQUAL(ac ,  8.375L - (8.375L * 57.578125L));
  BOOST_CHECK_EQUAL(ac , fixed_point_type(-473.841796875L));

  ac = a * 8.375L;
  ac *= +a;
  BOOST_CHECK_EQUAL(ac ,  70.140625L * 8.375L);
  BOOST_CHECK_EQUAL(ac ,  fixed_point_type(587.427734375L));

  ac = a;
  ac *= b * c;
  BOOST_CHECK_EQUAL(ac ,  241349.5068359375L);

  ac = a;
  ac *= b / a;
  BOOST_CHECK_EQUAL(ac ,  (8.375L * 57.578125L) / 8.375L);

  ac = a;
  ac *= b + c;
  BOOST_CHECK_EQUAL(ac ,  8.375L * (57.578125L + 500.5L));

  ac = b;
  ac /= +a;
  BOOST_CHECK_EQUAL(ac ,  6.875L);

  ac = b;
  ac /= b / a;
  BOOST_CHECK_EQUAL(ac ,  57.578125L / (57.578125L / 8.375L));

  ac = b;
  ac /= a + fixed_point_type(0);
  BOOST_CHECK_EQUAL(ac ,  6.875L);

  //
  // simple tests with immediate values, these calls can be optimised in many backends:
  //
  ac = a + b;
  BOOST_CHECK_EQUAL(ac ,  65.953125L);
  ac = a + +b;
  BOOST_CHECK_EQUAL(ac ,  65.953125L);
  ac = +a + b;
  BOOST_CHECK_EQUAL(ac ,  65.953125L);
  ac = +a + +b;
  BOOST_CHECK_EQUAL(ac ,  65.953125L);
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

  BOOST_CHECK_EQUAL((8.375L == b + a) ,  false);
  BOOST_CHECK_EQUAL((8.375L != b + a) ,  true);
  BOOST_CHECK_EQUAL((8.375L <= b + a) ,  true);
  BOOST_CHECK_EQUAL((8.375L <  b + a) ,  true);
  BOOST_CHECK_EQUAL((8.375L >= b + a) ,  false);
  BOOST_CHECK_EQUAL((8.375L >  b + a) ,  false);

  BOOST_CHECK_EQUAL((800.0L == b + a) ,  false);
  BOOST_CHECK_EQUAL((800.0L != b + a) ,  true);
  BOOST_CHECK_EQUAL((800.0L >= b + a) ,  true);
  BOOST_CHECK_EQUAL((800.0L >  b + a) ,  true);
  BOOST_CHECK_EQUAL((800.0L <= b + a) ,  false);
  BOOST_CHECK_EQUAL((800.0L <  b + a) ,  false);

  BOOST_CHECK_EQUAL((65.953125L == b + a) ,  true);
  BOOST_CHECK_EQUAL((65.953125L != b + a) ,  false);
  BOOST_CHECK_EQUAL((65.953125L <= b + a) ,  true);
  BOOST_CHECK_EQUAL((65.953125L <  b + a) ,  false);
  BOOST_CHECK_EQUAL((65.953125L >= b + a) ,  true);
  BOOST_CHECK_EQUAL((65.953125L >  b + a) ,  false);
}

BOOST_AUTO_TEST_CASE(test_negatable_arithmetic)
{
  test<boost::fixed_point::negatable< 18,  -13, boost::fixed_point::round::fastest> >();
  test<boost::fixed_point::negatable< 18,  -34, boost::fixed_point::round::fastest> >();
  test<boost::fixed_point::negatable< 18,  -45, boost::fixed_point::round::fastest> >();
  test<boost::fixed_point::negatable< 20,  -11, boost::fixed_point::round::fastest> >();
  test<boost::fixed_point::negatable< 24,   -7, boost::fixed_point::round::fastest> >();
  test<boost::fixed_point::negatable<100,  -12, boost::fixed_point::round::fastest> >();
  test<boost::fixed_point::negatable< 22,  -90, boost::fixed_point::round::fastest> >();
  test<boost::fixed_point::negatable<100, -139, boost::fixed_point::round::fastest> >();
}
