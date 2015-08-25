///////////////////////////////////////////////////////////////////////////////
//  Copyright Nikhar Agrawal 2015.
//  Copyright Christopher Kormanyos 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)


//! \file
//!\brief Mixed constructor tests for fixed_point negatable.

#define BOOST_TEST_MODULE test_negatable_mixed_constructors_part1
#define BOOST_LIB_DIAGNOSTIC

#include <iomanip>
#include <iostream>
#include <sstream>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_mixed_constructors_part1)
{
  // This test uses types which would have the underlying representation as built-in types 
  // and rounding mode is round::fastest.
  {
    typedef boost::fixed_point::negatable<3,-2> type_from;
    typedef boost::fixed_point::negatable<4,-3> type_to;

    type_to a = 6.25F;
    type_from b = 6.25F;
    type_to c (b);
    type_to d (type_from(6.25F));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,6.25F);
    BOOST_CHECK_EQUAL (d,6.25F);
  }

  // This test uses types which would have the underlying representation as built-in types 
  // and rounding mode is round::nearest_even. 
  {
    typedef boost::fixed_point::negatable<3,-2, boost::fixed_point::round::nearest_even> type_from;
    typedef boost::fixed_point::negatable<4,-3, boost::fixed_point::round::nearest_even> type_to;

    type_to a = 6.25F;
    type_from b = 6.25F;
    type_to c (b);
    type_to d (type_from(6.25F));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);

    BOOST_CHECK_EQUAL (c,6.25F);
    BOOST_CHECK_EQUAL (d,6.25F);
  }

  // This test uses types which would have the underlying representation as built-in types 
  // but these built-in types would supposedly be different from each other and rounding mode is round::fastest. 
  {
    typedef boost::fixed_point::negatable<16,-10> type_from; // use int32
    typedef boost::fixed_point::negatable<17,-20> type_to;   // use int64

    type_to a = 1099.5126953125L;
    type_from b = 1099.5126953125L;
    type_to c (b);
    type_to d (type_from(1099.5126953125L));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125L);
    BOOST_CHECK_EQUAL (d,1099.5126953125L);
  }

  // This test uses types which would have the underlying representation as built-in types 
  // but these built-in types would supposedly be different from each other and rounding mode is round::nearest_even.
  {
    typedef boost::fixed_point::negatable<16, -10, boost::fixed_point::round::nearest_even> type_from; // use int32
    typedef boost::fixed_point::negatable<17, -20, boost::fixed_point::round::nearest_even> type_to;   // use int64

    type_to a = 1099.5126953125L;
    type_from b = 1099.5126953125L;
    type_to c (b);
    type_to d (type_from(1099.5126953125L));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125L);
    BOOST_CHECK_EQUAL (d,1099.5126953125L);
  }

  // This test uses types where one would have built-in type and other would have multiprecision type
  // as underlying representation and rounding mode is round::fastest.
  {
    typedef boost::fixed_point::negatable<16, -10> type_from; // use int32
    typedef boost::fixed_point::negatable<417, -420> type_to;   // use multiprecision backend

    type_to a = 1099.5126953125L;
    type_from b = 1099.5126953125L;
    type_to c (b);
    type_to d (type_from(1099.5126953125L));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125L);
    BOOST_CHECK_EQUAL (d,1099.5126953125L);
  }

  // This test uses types where one would have built-in type and other would have multiprecision type
  // as underlying representation and rounding mode is round::nearest_even.
  {
    typedef boost::fixed_point::negatable<16, -10, boost::fixed_point::round::nearest_even> type_from; // use int32
    typedef boost::fixed_point::negatable<417, -420, boost::fixed_point::round::nearest_even> type_to;   // use multiprecision backend

    type_to a = 1099.5126953125L;
    type_from b = 1099.5126953125L;
    type_to c (b);
    type_to d (type_from(1099.5126953125L));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125L);
    BOOST_CHECK_EQUAL (d,1099.5126953125L);
  }

  // This test uses types with multiprecision types as underlying representation and 
  // rounding mode is round::fastest.
  {
    typedef boost::fixed_point::negatable<430, -424> fixed_point_type_from;
    typedef boost::fixed_point::negatable<440, -430> fixed_point_type_to;

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
  }

  // This test uses types with multiprecision types as underlying representation and
  // rounding mode is round::nearest_even.
  {

    typedef boost::fixed_point::negatable<430, -424, boost::fixed_point::round::nearest_even> fixed_point_type_from;
    typedef boost::fixed_point::negatable<440, -430, boost::fixed_point::round::nearest_even> fixed_point_type_to;

    typedef fixed_point_type_from::float_type floating_point_type_from;
    typedef fixed_point_type_to::float_type floating_point_type_to;

    floating_point_type_to big_float_to ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");
    floating_point_type_from big_float_from ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");

    fixed_point_type_to a (big_float_to);
    fixed_point_type_from b (big_float_from);
    fixed_point_type_to c (b);
    fixed_point_type_to d ((fixed_point_type_from)(big_float_from));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
  }
}

BOOST_AUTO_TEST_CASE(test_negatable_construct_larger_range_smaller_resolution)
{
  // Since the only overflow mode supported is overflow::undefined, all the tests below 
  // would only have numbers that could also fit in smaller range.

  // This test uses types which would have the underlying representation as built-in types 
  // and rounding mode is round::fastest. 
  {
    typedef boost::fixed_point::negatable<7,-2> type_from;
    typedef boost::fixed_point::negatable<4,-3> type_to;

    type_to a = 6.25F;
    type_from b = 6.25F;
    type_to c (b);
    type_to d (type_from(6.25F));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,6.25F);
    BOOST_CHECK_EQUAL (d,6.25F);
  }

  // This test uses types which would have the underlying representation as built-in types 
  // and rounding mode is round::nearest_even. 
  {
    typedef boost::fixed_point::negatable<6,-2, boost::fixed_point::round::nearest_even> type_from;
    typedef boost::fixed_point::negatable<4,-3, boost::fixed_point::round::nearest_even> type_to;

    type_to a = 6.25F;
    type_from b = 6.25F;
    type_to c (b);
    type_to d (type_from(6.25F));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,6.25F);
    BOOST_CHECK_EQUAL (d,6.25F);
  }

  // This test uses types which would have the underlying representation as built-in types 
  // but these built-in types would supposedly be different from each other and rounding mode is round::fastest.
  {
    typedef boost::fixed_point::negatable<18,-10> type_from; // use int32
    typedef boost::fixed_point::negatable<17,-20> type_to;   // use int64

    type_to a = 1099.5126953125L;
    type_from b = 1099.5126953125L;
    type_to c (b);
    type_to d (type_from(1099.5126953125L));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125L);
    BOOST_CHECK_EQUAL (d,1099.5126953125L);
  }

  // This test uses types which would have the underlying representation as built-in types 
  // but these built-in types would supposedly be different from each other and rounding mode is round::nearest_even.
  {
    typedef boost::fixed_point::negatable<20, -10, boost::fixed_point::round::nearest_even> type_from; // use int32
    typedef boost::fixed_point::negatable<17, -20, boost::fixed_point::round::nearest_even> type_to;   // use int64

    type_to a = 1099.5126953125L;
    type_from b = 1099.5126953125L;
    type_to c (b);
    type_to d (type_from(1099.5126953125L));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
    BOOST_CHECK_EQUAL (c,1099.5126953125L);
    BOOST_CHECK_EQUAL (d,1099.5126953125L);
  }

  // This test uses types with multiprecision types as underlying representation and 
  // rounding mode is round::fastest.
  {
    typedef boost::fixed_point::negatable<470, -424> fixed_point_type_from;
    typedef boost::fixed_point::negatable<440, -430> fixed_point_type_to;

    typedef fixed_point_type_from::float_type floating_point_type_from;
    typedef fixed_point_type_to::float_type floating_point_type_to;

    floating_point_type_to big_float_to ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");
    floating_point_type_from big_float_from ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");

    fixed_point_type_to a (big_float_to);
    fixed_point_type_from b (big_float_from);
    fixed_point_type_to c (b);
    fixed_point_type_to d ((fixed_point_type_from)(big_float_from));

    BOOST_CHECK_EQUAL (c,a);
    BOOST_CHECK_EQUAL (d,a);
  }

    // This test uses types with multiprecision types as underlying representation and 
    // rounding mode is round::nearest_even.
  {
    typedef boost::fixed_point::negatable<470, -424, boost::fixed_point::round::nearest_even> fixed_point_type_from;
    typedef boost::fixed_point::negatable<440, -430, boost::fixed_point::round::nearest_even> fixed_point_type_to;  

    typedef fixed_point_type_from::float_type floating_point_type_from;
    typedef fixed_point_type_to::float_type floating_point_type_to;

    floating_point_type_to big_float_to ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");
    floating_point_type_from big_float_from ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.125000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000053224498000101883998875671453091360603082459178883520011563126148080110359319401703562961046591961274351135564659444067264932271199483366269899638994133157747123111848537123809031017110820488360041179034278286297428295359424055277486331760883331298828125");
  }
}  

BOOST_AUTO_TEST_CASE(test_negatable_construct_smaller_range_larger_resolution)
{
  // This test uses types which would have the underlying representation as built-in types 
  // and rounding mode is round::fastest. 
  {
    typedef boost::fixed_point::negatable<3,-4> type_from;
    typedef boost::fixed_point::negatable<4,-3> type_to;

    type_to a = 6.125F;
    type_to b = 6.25F;
    type_to c = 6.1875F;
    type_from d = 6.1875F;
    type_to e (d);

    BOOST_CHECK_EQUAL ((e == a || e == b), true);
    BOOST_CHECK_EQUAL (e, c);
  }
}
