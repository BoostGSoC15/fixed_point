// Copyright Paul A. Bristow 2015
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)


//! \file
//!\brief Basic tests for fixed_point.

#define BOOST_TEST_MODULE fixed_point_basic
#define BOOST_LIB_DIAGNOSTIC
//  Linking to lib file: libboost_unit_test_framework-vc120-mt-gd-1_59.lib

#include <boost/test/included/unit_test.hpp> // No Boost.Test library.
//#include <boost/test/unit_test.hpp> // USe Boost.Test library.

#include <iostream>
#include <iomanip>
#include <sstream>

#include "boost/fixed_point/fixed_point.hpp"

/*BOOST_AUTO_TEST_CASE(fixed_point_multiply_fastest_round)
{
  //! small range and resolution	
  {
  	typedef boost::fixed_point::negatable<4, -2, boost::fixed_point::round::fastest> fixed_point_type_fastest_round;
    fixed_point_type_fastest_round x = fixed_point_type_fastest_round (-1.25) * fixed_point_type_fastest_round (1.5);
    fixed_point_type_fastest_round y = fixed_point_type_fastest_round(-2);
    fixed_point_type_fastest_round z = fixed_point_type_fastest_round(-1.75);

    BOOST_CHECK_EQUAL(x == y || x == z, true);

    x = fixed_point_type_fastest_round (1.25) * fixed_point_type_fastest_round (1.5);
    y = fixed_point_type_fastest_round (1.75);
    z = fixed_point_type_fastest_round (2);

    BOOST_CHECK_EQUAL(x == y || x == z, true);
  }

  //! larger range and small resolution
  {
  	typedef boost::fixed_point::negatable<400, -2, boost::fixed_point::round::fastest> fixed_point_type_fastest_round;
    fixed_point_type_fastest_round x = fixed_point_type_fastest_round (-1.25) * fixed_point_type_fastest_round (1.5);
    fixed_point_type_fastest_round y = fixed_point_type_fastest_round(-2);
    fixed_point_type_fastest_round z = fixed_point_type_fastest_round(-1.75);

    BOOST_CHECK_EQUAL(x == y || x == z, true);

    x = fixed_point_type_fastest_round (1.25) * fixed_point_type_fastest_round (1.5);
    y = fixed_point_type_fastest_round (1.75);
    z = fixed_point_type_fastest_round (2);

    BOOST_CHECK_EQUAL(x == y || x == z, true);
  }

  //! larger range and larger resolution
  {
  	typedef boost::fixed_point::negatable<87, -4, boost::fixed_point::round::fastest> fixed_point_type_fastest_round;
    fixed_point_type_fastest_round x = fixed_point_type_fastest_round (800.4375) * fixed_point_type_fastest_round (-7.5625);
    fixed_point_type_fastest_round y = fixed_point_type_fastest_round(-6053.3125);
    fixed_point_type_fastest_round z = fixed_point_type_fastest_round(-6053.25);

    BOOST_CHECK_EQUAL(x == y || x == z, true);

    x = fixed_point_type_fastest_round (800.4375) * fixed_point_type_fastest_round (7.5625);
    y = fixed_point_type_fastest_round (6053.3125);
    z = fixed_point_type_fastest_round (6053.25);

    BOOST_CHECK_EQUAL(x == y || x == z, true);
  }  
}*/

//use 6 fractinal bits at least
template <typename fixed_point_type>
void test ()
{
   fixed_point_type a(8.375);
   fixed_point_type b(64.125);
   fixed_point_type c(500.5);
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
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(8.375*500.5+64.125+500.5));
   BOOST_CHECK_EQUAL(ac ,  8.375*500.5+64.125+500.5);
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(4756.3125));
   BOOST_CHECK_EQUAL(ac ,  4756.3125);

   ac = a;
   ac = b + c + ac;
   BOOST_CHECK_EQUAL(ac ,  8.375+64.125+500.5);
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(8.375+64.125+500.5));
   BOOST_CHECK_EQUAL(ac ,  573);
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(573));

   ac = ac - b + c;
   BOOST_CHECK_EQUAL(ac ,  8.375+64.125+500.5-64.125+500.5);
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(8.375+64.125+500.5-64.125+500.5));
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(1009.375));
   BOOST_CHECK_EQUAL(ac ,  1009.375);

   ac = a;
   ac = b + c - ac;
   BOOST_CHECK_EQUAL(ac ,  -8.375+64.125+500.5);
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(-8.375+64.125+500.5));
   BOOST_CHECK_EQUAL(ac ,  556.25);
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(556.25));

   ac = a;
   ac = ac * b;
   BOOST_CHECK_EQUAL(ac ,  8.375*64.125);
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(8.375*64.125));
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(537.046875));
   BOOST_CHECK_EQUAL(ac ,  537.046875);

   ac = a;
   ac *= b * ac;
   BOOST_CHECK_EQUAL(ac ,  8.375*8.375*64.125);
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(8.375*8.375*64.125));
   BOOST_CHECK_EQUAL(ac ,  fixed_point_type(4497.767578125));
   BOOST_CHECK_EQUAL(ac ,  4497.767578125);

   
   /*ac = b;
   ac = ac / a;
   BOOST_CHECK_EQUAL(ac ,  64.125/8.375);

   ac = b;
   ac /= ac / a;
   BOOST_CHECK_EQUAL(ac ,  64.125 / (64.125/8.375));
   ac = a;
   ac = b + ac * a;
   BOOST_CHECK_EQUAL(ac ,  64.125 * 2);
   ac = a;
   ac = b - ac * a;
   BOOST_CHECK_EQUAL(ac ,  0);
   ac = a;
   ac = b * (ac + a);
   BOOST_CHECK_EQUAL(ac ,  64.125 * (16));
   ac = a;
   ac = b / (ac * 1);
   BOOST_CHECK_EQUAL(ac ,  64.125 / 8.375);
   ac = a;
   ac = ac + b;
   BOOST_CHECK_EQUAL(ac ,  8.375 + 64.125);
   ac = a;
   ac = a + ac;
   BOOST_CHECK_EQUAL(ac ,  16);
   ac = a;
   ac = a - ac;
   BOOST_CHECK_EQUAL(ac ,  0);
   ac = a;
   ac += a + b;
   BOOST_CHECK_EQUAL(ac ,  80);
   ac = a;
   ac += b + a;
   BOOST_CHECK_EQUAL(ac ,  80);
   ac = +a;
   BOOST_CHECK_EQUAL(ac ,  8.375);
   ac = 8.375;
   ac = a * ac;
   BOOST_CHECK_EQUAL(ac ,  8.375*8.375);
   ac = a;
   ac = a;
   ac += +a;
   BOOST_CHECK_EQUAL(ac ,  16);
   ac = a;
   ac += b - a;
   BOOST_CHECK_EQUAL(ac ,  8.375 + 64.125-8.375);
   ac = a;
   ac += b*c;
   BOOST_CHECK_EQUAL(ac ,  8.375 + 64.125 * 500.5);
   ac = a;
   ac = a;
   ac -= +a;
   BOOST_CHECK_EQUAL(ac ,  0);
   ac = a;
   if(std::numeric_limits<Real>::is_signed || is_twos_complement_integer<Real>::value)
   {
      ac = a;
      ac -= c - b;
      BOOST_CHECK_EQUAL(ac ,  8.375 - (500.5-64.125));
      ac = a;
      ac -= b*c;
      BOOST_CHECK_EQUAL(ac ,  8.375 - 500.5*64.125);
   }
   ac = a;
   ac += ac * b;
   BOOST_CHECK_EQUAL(ac ,  8.375 + 8.375 * 64.125);
   if(std::numeric_limits<Real>::is_signed || is_twos_complement_integer<Real>::value)
   {
      ac = a;
      ac -= ac * b;
      BOOST_CHECK_EQUAL(ac ,  8.375 - 8.375 * 64.125);
   }
   ac = a * 8.375;
   ac *= +a;
   BOOST_CHECK_EQUAL(ac ,  64.125 * 8.375);
   ac = a;
   ac *= b * c;
   BOOST_CHECK_EQUAL(ac ,  8.375 * 64.125 * 500.5);
   ac = a;
   ac *= b / a;
   BOOST_CHECK_EQUAL(ac ,  8.375 * 64.125 / 8.375);
   ac = a;
   ac *= b + c;
   BOOST_CHECK_EQUAL(ac ,  8.375 * (64.125 + 500.5));
   ac = b;
   ac /= +a;
   BOOST_CHECK_EQUAL(ac ,  8.375);
   ac = b;
   ac /= b / a;
   BOOST_CHECK_EQUAL(ac ,  64.125 / (64.125/8.375));
   ac = b;
   ac /= a + Real(0);
   BOOST_CHECK_EQUAL(ac ,  8.375);
   //
   // simple tests with immediate values, these calls can be optimised in many backends:
   //
   ac = a + b;
   BOOST_CHECK_EQUAL(ac ,  72);
   ac = a + +b;
   BOOST_CHECK_EQUAL(ac ,  72);
   ac = +a + b;
   BOOST_CHECK_EQUAL(ac ,  72);
   ac = +a + +b;
   BOOST_CHECK_EQUAL(ac ,  72);
   ac = a;
   ac = b / ac;
   BOOST_CHECK_EQUAL(ac ,  b / a);
   //
   // Comparisons:
   //
   BOOST_CHECK_EQUAL((a == b) ,  false);
   BOOST_CHECK_EQUAL((a != b) ,  true);
   BOOST_CHECK_EQUAL((a <= b) ,  true);
   BOOST_CHECK_EQUAL((a < b) ,  true);
   BOOST_CHECK_EQUAL((a >= b) ,  false);
   BOOST_CHECK_EQUAL((a > b) ,  false);

   BOOST_CHECK_EQUAL((a+b == b) ,  false);
   BOOST_CHECK_EQUAL((a+b != b) ,  true);
   BOOST_CHECK_EQUAL((a+b >= b) ,  true);
   BOOST_CHECK_EQUAL((a+b > b) ,  true);
   BOOST_CHECK_EQUAL((a+b <= b) ,  false);
   BOOST_CHECK_EQUAL((a+b < b) ,  false);

   BOOST_CHECK_EQUAL((a == b+a) ,  false);
   BOOST_CHECK_EQUAL((a != b+a) ,  true);
   BOOST_CHECK_EQUAL((a <= b+a) ,  true);
   BOOST_CHECK_EQUAL((a < b+a) ,  true);
   BOOST_CHECK_EQUAL((a >= b+a) ,  false);
   BOOST_CHECK_EQUAL((a > b+a) ,  false);

   BOOST_CHECK_EQUAL((a+b == b+a) ,  true);
   BOOST_CHECK_EQUAL((a+b != b+a) ,  false);
   BOOST_CHECK_EQUAL((a+b <= b+a) ,  true);
   BOOST_CHECK_EQUAL((a+b < b+a) ,  false);
   BOOST_CHECK_EQUAL((a+b >= b+a) ,  true);
   BOOST_CHECK_EQUAL((a+b > b+a) ,  false);

   BOOST_CHECK_EQUAL((8.375 == b+a) ,  false);
   BOOST_CHECK_EQUAL((8.375 != b+a) ,  true);
   BOOST_CHECK_EQUAL((8.375 <= b+a) ,  true);
   BOOST_CHECK_EQUAL((8.375 < b+a) ,  true);
   BOOST_CHECK_EQUAL((8.375 >= b+a) ,  false);
   BOOST_CHECK_EQUAL((8.375 > b+a) ,  false);
   BOOST_CHECK_EQUAL((800 == b+a) ,  false);
   BOOST_CHECK_EQUAL((800 != b+a) ,  true);
   BOOST_CHECK_EQUAL((800 >= b+a) ,  true);
   BOOST_CHECK_EQUAL((800 > b+a) ,  true);
   BOOST_CHECK_EQUAL((800 <= b+a) ,  false);
   BOOST_CHECK_EQUAL((800 < b+a) ,  false);
   BOOST_CHECK_EQUAL((72 == b+a) ,  true);
   BOOST_CHECK_EQUAL((72 != b+a) ,  false);
   BOOST_CHECK_EQUAL((72 <= b+a) ,  true);
   BOOST_CHECK_EQUAL((72 < b+a) ,  false);
   BOOST_CHECK_EQUAL((72 >= b+a) ,  true);
   BOOST_CHECK_EQUAL((72 > b+a) ,  false);

   test_members(a);
   //
   // Use in Boolean context:
   //
   a = 0;
   if(a)
   {
      BOOST_ERROR("Unexpected non-zero result");
   }
   if(!a){}
   else
   {
      BOOST_ERROR("Unexpected zero result");
   }
   b = 2;
   if(!b)
   {
      BOOST_ERROR("Unexpected zero result");
   }
   if(b){}
   else
   {
      BOOST_ERROR("Unexpected non-zero result");
   }
   if(a && b)
   {
      BOOST_ERROR("Unexpected zero result");
   }
   if(!(a || b))
   {
      BOOST_ERROR("Unexpected zero result");
   }
   if(a + b){}
   else
   {
      BOOST_ERROR("Unexpected zero result");
   }
   if(b - 2)
   {
      BOOST_ERROR("Unexpected non-zero result");
   }
   //
   // Test iostreams:
   //
   std::stringstream ss;
   a = 20;
   b = 2;
   ss << a;
   ss >> c;
   BOOST_CHECK_EQUAL(a ,  c);
   ss.clear();
   ss << a + b;
   ss >> c;
   BOOST_CHECK_EQUAL(c ,  22);
   BOOST_CHECK_EQUAL(c ,  a + b);
   //
   // More cases for complete code coverage:
   //
   a = 20;
   b = 30;
   swap(a, b);
   BOOST_CHECK_EQUAL(a ,  30);
   BOOST_CHECK_EQUAL(b ,  20);
   a = 20;
   b = 30;
   std::swap(a, b);
   BOOST_CHECK_EQUAL(a ,  30);
   BOOST_CHECK_EQUAL(b ,  20);
   a = 20;
   b = 30;
   a = a + b * 2;
   BOOST_CHECK_EQUAL(a ,  20 + 30 * 2);
   a = 100;
   a = a - b * 2;
   BOOST_CHECK_EQUAL(a ,  100 - 30 * 2);
   a = 20;
   a = a * (b + 2);
   BOOST_CHECK_EQUAL(a ,  20 * (32));
   a = 20;
   a = (b + 2) * a;
   BOOST_CHECK_EQUAL(a ,  20 * (32));
   a = 90;
   b = 2;
   a = a / (b + 0);
   BOOST_CHECK_EQUAL(a ,  45);
   a = 20;
   b = 30;
   c = (a * b) + 22;
   BOOST_CHECK_EQUAL(c ,  20 * 30 + 22);
   c = 22 + (a * b);
   BOOST_CHECK_EQUAL(c ,  20 * 30 + 22);
   c = 10;
   ac = a + b * c;
   BOOST_CHECK_EQUAL(ac ,  20 + 30 * 10);
   ac = b * c + a;
   BOOST_CHECK_EQUAL(ac ,  20 + 30 * 10);
   a = a + b * c;
   BOOST_CHECK_EQUAL(a ,  20 + 30 * 10);
   a = 20;
   b = a + b * c;
   BOOST_CHECK_EQUAL(b ,  20 + 30 * 10);
   b = 30;
   c = a + b * c;
   BOOST_CHECK_EQUAL(c ,  20 + 30 * 10);
   c = 10;
   c = a + b / c;
   BOOST_CHECK_EQUAL(c ,  20 + 30 / 10);

   //
   // Test conditionals:
   //
   a = 20;
   test_conditional(a, +a);
   test_conditional(a, (a + 0));

   test_signed_ops<Real>(boost::mpl::bool_<std::numeric_limits<Real>::is_signed>());
   //
   // Test move:
   //
#ifndef BOOST_NO_CXX11_RVALUE_REFERENCES
   Real m(static_cast<Real&&>(a));
   BOOST_CHECK_EQUAL(m, 20);
   // Move from already moved from object:
   Real m2(static_cast<Real&&>(a));
   // assign from moved from object 
   // (may result in "a" being left in valid state as implementation artifact):
   c = static_cast<Real&&>(a);
   // assignment to moved-from objects:
   c = static_cast<Real&&>(m);
   BOOST_CHECK_EQUAL(c, 20);
   m2 = c;
   BOOST_CHECK_EQUAL(c, 20);
   // Destructor of "a" checks destruction of moved-from-object...
   Real m3(static_cast<Real&&>(a));
#endif
}*/
}

BOOST_AUTO_TEST_CASE(fixed_point_arithmetic)
{
	test<boost::fixed_point::negatable<10, -10, boost::fixed_point::round::fastest> >();
	test<boost::fixed_point::negatable<100, -10, boost::fixed_point::round::fastest> >();
	test<boost::fixed_point::negatable<10, -100, boost::fixed_point::round::fastest> >();
	test<boost::fixed_point::negatable<100, -100, boost::fixed_point::round::fastest> >();

	test<boost::fixed_point::negatable<10, -10, boost::fixed_point::round::nearest_even> >();
	test<boost::fixed_point::negatable<100, -10, boost::fixed_point::round::nearest_even> >();
	test<boost::fixed_point::negatable<10, -100, boost::fixed_point::round::nearest_even> >();
	test<boost::fixed_point::negatable<100, -100, boost::fixed_point::round::nearest_even> >();
}