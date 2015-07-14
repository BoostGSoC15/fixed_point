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
#include <boost/multiprecision/cpp_dec_float.hpp>

#include "boost/fixed_point/fixed_point.hpp"

BOOST_AUTO_TEST_CASE(fixed_point_construct_smaller_range_smaller_resolution)
{

  // built-in types as base, round::fastest	
  {
    typedef boost::fixed_point::negatable<3,-2> type_from;
    typedef boost::fixed_point::negatable<4,-3> type_to;

    type_to a = 6.25;
    type_from b = 6.25;
    type_to c (b);
    type_to d (type_from(6.25));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,6.25);
    BOOST_CHECK_EQUAL (d,6.25);
  }

  // built-in types as base, round::nearest_even
  {
    typedef boost::fixed_point::negatable<3,-2, boost::fixed_point::round::nearest_even> type_from;
    typedef boost::fixed_point::negatable<4,-3, boost::fixed_point::round::nearest_even> type_to;

    type_to a = 6.25;
    type_from b = 6.25;
    type_to c (b);
    type_to d (type_from(6.25));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,6.25);
    BOOST_CHECK_EQUAL (d,6.25);
  }

  // built-in types as base but supposedly different base types, round::fastest
  {
    typedef boost::fixed_point::negatable<16,-10> type_from; // use int32
    typedef boost::fixed_point::negatable<17,-20> type_to;   // use int64

    type_to a = 1099.5126953125;
    type_from b = 1099.5126953125;
    type_to c (b);
    type_to d (type_from(1099.5126953125));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125);
    BOOST_CHECK_EQUAL (d,1099.5126953125);
  }

  // built-in types as base but supposedly different base types, round::nearest_even
  {
    typedef boost::fixed_point::negatable<16, -10, boost::fixed_point::round::nearest_even> type_from; // use int32
    typedef boost::fixed_point::negatable<17, -20, boost::fixed_point::round::nearest_even> type_to;   // use int64

    type_to a = 1099.5126953125;
    type_from b = 1099.5126953125;
    type_to c (b);
    type_to d (type_from(1099.5126953125));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125);
    BOOST_CHECK_EQUAL (d,1099.5126953125);
  }

  // multiprecision and built-in types as base
  {
    typedef boost::fixed_point::negatable<16, -10> type_from; // use int32
    typedef boost::fixed_point::negatable<417, -420> type_to;   // use multiprecision backend

    type_to a = 1099.5126953125;
    type_from b = 1099.5126953125;
    type_to c (b);
    type_to d (type_from(1099.5126953125));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125);
    BOOST_CHECK_EQUAL (d,1099.5126953125);
  }

  // multiprecision and built-in types as base, round::nearest_even
  {
    typedef boost::fixed_point::negatable<16, -10, boost::fixed_point::round::nearest_even> type_from; // use int32
    typedef boost::fixed_point::negatable<417, -420, boost::fixed_point::round::nearest_even> type_to;   // use multiprecision backend

    type_to a = 1099.5126953125;
    type_from b = 1099.5126953125;
    type_to c (b);
    type_to d (type_from(1099.5126953125));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125);
    BOOST_CHECK_EQUAL (d,1099.5126953125);
  }

	 // multiprecision types as base
	  {
	    typedef boost::fixed_point::negatable<430, -424> fixed_point_type_from; // use int32
	    typedef boost::fixed_point::negatable<440, -430> fixed_point_type_to;   // use multiprecision backend

	    typedef fixed_point_type_from::float_type floating_point_type_from;
	    typedef fixed_point_type_to::float_type floating_point_type_to;

	    floating_point_type_to big_float_to ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");
	    floating_point_type_from big_float_from ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");

	    fixed_point_type_to a(big_float_to);
	    fixed_point_type_from b(big_float_from);
	    fixed_point_type_to c (b);
	    fixed_point_type_to d ((fixed_point_type_from)(big_float_from));

	    BOOST_CHECK_EQUAL (c,a);
	    BOOST_CHECK_EQUAL (d,a);
	   // BOOST_CHECK_EQUAL (c,big_float_to);
	  }

  // multiprecision types as base, round::nearest_even
  {

    typedef boost::fixed_point::negatable<430, -424, boost::fixed_point::round::nearest_even> fixed_point_type_from; // use int32
    typedef boost::fixed_point::negatable<440, -430, boost::fixed_point::round::nearest_even> fixed_point_type_to;   // use multiprecision backend

    typedef fixed_point_type_from::float_type floating_point_type_from;
    typedef fixed_point_type_from::float_type floating_point_type_to;

    floating_point_type_to big_float_to ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");
    floating_point_type_from big_float_from ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");

    fixed_point_type_to a (big_float_to);
    fixed_point_type_from b (big_float_from);
    fixed_point_type_to c (b);
    fixed_point_type_to d ((fixed_point_type_from)(big_float_from));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
  //  BOOST_CHECK_EQUAL (c,big_float_to);
  }
}

BOOST_AUTO_TEST_CASE(fixed_point_construct_larger_range_smaller_resolution)
{
  // since the only overflow mode supported is overflow::undefined, all the tests below would only have numbers that could also fit in smaller range

  // built-in types as base, round::fastest 
  {
    typedef boost::fixed_point::negatable<7,-2> type_from;
    typedef boost::fixed_point::negatable<4,-3> type_to;

    type_to a = 6.25;
    type_from b = 6.25;
    type_to c (b);
    type_to d (type_from(6.25));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,6.25);
    BOOST_CHECK_EQUAL (d,6.25);
  }

  // built-in types as base, round::nearest_even
  {
    typedef boost::fixed_point::negatable<6,-2, boost::fixed_point::round::nearest_even> type_from;
    typedef boost::fixed_point::negatable<4,-3, boost::fixed_point::round::nearest_even> type_to;

    type_to a = 6.25;
    type_from b = 6.25;
    type_to c (b);
    type_to d (type_from(6.25));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,6.25);
    BOOST_CHECK_EQUAL (d,6.25);
  }

  // built-in types as base but supposedly different base types, round::fastest
  {
    typedef boost::fixed_point::negatable<18,-10> type_from; // use int32
    typedef boost::fixed_point::negatable<17,-20> type_to;   // use int64

    type_to a = 1099.5126953125;
    type_from b = 1099.5126953125;
    type_to c (b);
    type_to d (type_from(1099.5126953125));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125);
    BOOST_CHECK_EQUAL (d,1099.5126953125);
  }

  // built-in types as base but supposedly different base types, round::nearest_even
  {
    typedef boost::fixed_point::negatable<20, -10, boost::fixed_point::round::nearest_even> type_from; // use int32
    typedef boost::fixed_point::negatable<17, -20, boost::fixed_point::round::nearest_even> type_to;   // use int64

    type_to a = 1099.5126953125;
    type_from b = 1099.5126953125;
    type_to c (b);
    type_to d (type_from(1099.5126953125));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125);
    BOOST_CHECK_EQUAL (d,1099.5126953125);
  }

  // multiprecision types as base
  {
    typedef boost::fixed_point::negatable<470, -424> fixed_point_type_from; // use int32
    typedef boost::fixed_point::negatable<440, -430> fixed_point_type_to;   // use multiprecision backend

    typedef fixed_point_type_from::float_type floating_point_type_from;
    typedef fixed_point_type_from::float_type floating_point_type_to;

    floating_point_type_to big_float_to ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");
    floating_point_type_from big_float_from ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");

    fixed_point_type_to a (big_float_to);
    fixed_point_type_from b (big_float_from);
    fixed_point_type_to c (b);
    fixed_point_type_to d ((fixed_point_type_from)(big_float_from));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    //BOOST_CHECK_EQUAL (c,big_float_to);
  }

  // multiprecision types as base, round::nearest_even
  {

    typedef boost::fixed_point::negatable<470, -424, boost::fixed_point::round::nearest_even> fixed_point_type_from; // use int32
    typedef boost::fixed_point::negatable<440, -430, boost::fixed_point::round::nearest_even> fixed_point_type_to;   // use multiprecision backend

    typedef fixed_point_type_from::float_type floating_point_type_from;
    typedef fixed_point_type_from::float_type floating_point_type_to;

    floating_point_type_to big_float_to ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");
    floating_point_type_from big_float_from ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");

    fixed_point_type_to a(big_float_to);
    fixed_point_type_from b(big_float_from);
    fixed_point_type_to c (b);
    fixed_point_type_to d ((fixed_point_type_from)(big_float_from));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
   // BOOST_CHECK_EQUAL (c,big_float_to);
  }

}

