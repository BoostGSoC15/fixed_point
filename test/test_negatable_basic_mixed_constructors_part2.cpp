///////////////////////////////////////////////////////////////////////////////
//  Copyright Nikhar Agrawal 2015.
//  Copyright Christopher Kormanyos 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)


//! \file
//!\brief Mixed constructor tests for fixed_point negatable.

#define BOOST_TEST_MODULE test_negatable_basic_mixed_constructors_part2
#define BOOST_LIB_DIAGNOSTIC

#include <iomanip>
#include <iostream>
#include <sstream>

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(test_negatable_basic_mixed_constructors_part2)
{
  // This test uses types which would have the underlying representation as built-in types 
  // and rounding mode is round::nearest_even. 
  {
    typedef boost::fixed_point::negatable<3,-4, boost::fixed_point::round::nearest_even> type_from;
    typedef boost::fixed_point::negatable<4,-3, boost::fixed_point::round::nearest_even> type_to;

    type_to a = 6.25;
    type_to b = 6.1875;
    type_from c = 6.1875;
    type_to d (c);

    BOOST_CHECK_EQUAL(d, a);
    BOOST_CHECK_EQUAL(d, b);
  }

  // This test uses types which would have the underlying representation as built-in types 
  // but these built-in types would supposedly be different from each other and rounding mode is round::fastest. 
  {
    typedef boost::fixed_point::negatable<20,-43> type_from; // use int64
    typedef boost::fixed_point::negatable<23,-8> type_to;   // use int32

    type_to a = 999.03125L;
    type_to b = 999.02734375L;
    type_to c = 999.029541015625L;
    type_from d = 999.029541015625L;
    type_to e (d);

    BOOST_CHECK_EQUAL ((e == a || e == b), true);
    BOOST_CHECK_EQUAL (e, c);
  }

  // This test uses types which would have the underlying representation as built-in types 
  // but these built-in types would supposedly be different from each other and rounding mode is round::nearest_even.
  {
    typedef boost::fixed_point::negatable<20, -43, boost::fixed_point::round::nearest_even> type_from; // use int64
    typedef boost::fixed_point::negatable<23, -8, boost::fixed_point::round::nearest_even> type_to;   // use int32

    type_to a = 999.03125L;
    type_to b = 999.029541015625L;
    type_from c = 999.029541015625L;
    type_to d (c);

    BOOST_CHECK_EQUAL (d, a);
    BOOST_CHECK_EQUAL (d, b);
  }

  // This test uses types where one would have built-in type and other would have multiprecision type
  // as underlying representation and rounding mode is round::fastest.
  {
    typedef boost::fixed_point::negatable<16, -420> fixed_point_type_from; // use multiprecision backend
    typedef boost::fixed_point::negatable<21, -10> fixed_point_type_to;   // use int32

    typedef fixed_point_type_from::float_type floating_point_type_from;

    floating_point_type_from big_float_from ("1024.499511718750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003025093114331318471207856075024462507576719799239315603180910706763469153143705915278456705185225159003629620616853584634302761167921671050495463099756392848967738514960376530192422564155070725513250306166850322074879148855369981176783751956850300945259051553504203724287435761652886867523193359375");

    fixed_point_type_to a = 1024.5L;
    fixed_point_type_to b = 1024.4990234375L;
    fixed_point_type_from c (big_float_from);
    fixed_point_type_to d (c);

    BOOST_CHECK_EQUAL ((d == a || d == b), true);
  }

  // This test uses types where one would have built-in type and other would have multiprecision type
  // as underlying representation and rounding mode is round::nearest_even.
  {
    typedef boost::fixed_point::negatable<16, -420, boost::fixed_point::round::nearest_even> fixed_point_type_from; // use multiprecision backend
    typedef boost::fixed_point::negatable<20, -10, boost::fixed_point::round::nearest_even> fixed_point_type_to;   // use int32

    typedef fixed_point_type_from::float_type floating_point_type_from;

    floating_point_type_from big_float_from ("1024.499511718750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003025093114331318471207856075024462507576719799239315603180910706763469153143705915278456705185225159003629620616853584634302761167921671050495463099756392848967738514960376530192422564155070725513250306166850322074879148855369981176783751956850300945259051553504203724287435761652886867523193359375");

    fixed_point_type_to a = 1024.5;
    fixed_point_type_from b (big_float_from);
    fixed_point_type_to c (b);

    BOOST_CHECK_EQUAL (c, a);
  }

  // This test uses types with multiprecision types as underlying representation and 
  // rounding mode is round::fastest.
  {
    typedef boost::fixed_point::negatable<470, -420> fixed_point_type_from;
    typedef boost::fixed_point::negatable<480, -417> fixed_point_type_to;

    typedef fixed_point_type_from::float_type floating_point_type_from;
    typedef fixed_point_type_to::float_type floating_point_type_to;

    floating_point_type_to big_float_to_higher ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.49951171875000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000302546243347602990063908643225496238091423374378811786366231479792532527195131716004897049552891765383441995508402692776513346593671277368400181097707293007187690317599260218963940002997904277663344481847379292375014161731451481941157520400812082350672227448740159161388874053955078125");
    floating_point_type_to big_float_to_lower ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.499511718750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003022507880318338465189243574410963394214122187397699779811082068630476321490427201884860172779377695188097279346640182718487827786383952615951027958540632288603585887734796914063580303387266367671888720018252110348041869641356113533243587597956643015016491016222488497078302316367626190185546875");
    floating_point_type_from big_float_from ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.499511718750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003025093114331318471207856075024462507576719799239315603180910706763469153143705915278456705185225159003629620616853584634302761167921671050495463099756392848967738514960376530192422564155070725513250306166850322074879148855369981176783751956850300945259051553504203724287435761652886867523193359375");

    fixed_point_type_to a (big_float_to_higher);
    fixed_point_type_to b (big_float_to_lower);
    fixed_point_type_from c (big_float_from);
    fixed_point_type_to d (c);

    BOOST_CHECK_EQUAL ((d == a || d == b), true);
  }

  // This test uses types with multiprecision types as underlying representation and
  // rounding mode is round::nearest_even.
  {
    typedef boost::fixed_point::negatable<470, -420, boost::fixed_point::round::nearest_even> fixed_point_type_from;
    typedef boost::fixed_point::negatable<480, -417, boost::fixed_point::round::nearest_even> fixed_point_type_to;

    typedef fixed_point_type_from::float_type floating_point_type_from;
    typedef fixed_point_type_to::float_type floating_point_type_to;

    floating_point_type_to big_float_to ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.49951171875000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000302546243347602990063908643225496238091423374378811786366231479792532527195131716004897049552891765383441995508402692776513346593671277368400181097707293007187690317599260218963940002997904277663344481847379292375014161731451481941157520400812082350672227448740159161388874053955078125");
    floating_point_type_from big_float_from ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.499511718750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003025093114331318471207856075024462507576719799239315603180910706763469153143705915278456705185225159003629620616853584634302761167921671050495463099756392848967738514960376530192422564155070725513250306166850322074879148855369981176783751956850300945259051553504203724287435761652886867523193359375");

    fixed_point_type_to a (big_float_to);
    fixed_point_type_from b (big_float_from);
    fixed_point_type_to c (b);

    BOOST_CHECK_EQUAL (c, a);
  }
}

BOOST_AUTO_TEST_CASE(test_negatable_basic_construct_larger_range_larger_resolution)
{
  // Since the only overflow mode supported is overflow::undefined, all the tests below 
  // would only have numbers that could also fit in smaller range.

  // This test uses types which would have the underlying representation as built-in types 
  // and rounding mode is round::fastest. 
  {
    typedef boost::fixed_point::negatable<5,-4> type_from;
    typedef boost::fixed_point::negatable<4,-3> type_to;

    type_to a = 6.125F;
    type_to b = 6.25F;
    type_to c = 6.1875F;
    type_from d = 6.1875F;
    type_to e (d);

    BOOST_CHECK_EQUAL ((e == a || e == b), true);
    BOOST_CHECK_EQUAL (e, c);
  }

  // This test uses types which would have the underlying representation as built-in types 
  // and rounding mode is round::nearest_even. 
  {
    typedef boost::fixed_point::negatable<5,-4, boost::fixed_point::round::nearest_even> type_from;
    typedef boost::fixed_point::negatable<4,-3, boost::fixed_point::round::nearest_even> type_to;

    type_to a = 6.25F;
    type_to b = 6.1875F;
    type_from c = 6.1875F;
    type_to d (c);

    BOOST_CHECK_EQUAL(d, a);
    BOOST_CHECK_EQUAL(d, b);
  }

  // This test uses types which would have the underlying representation as built-in types 
  // but these built-in types would supposedly be different from each other and rounding mode is round::fastest. 
  {
    typedef boost::fixed_point::negatable<24,-39> type_from; // use int64
    typedef boost::fixed_point::negatable<23,-8> type_to;   // use int32

    type_to a = 999.03125L;
    type_to b = 999.02734375L;
    type_to c = 999.029541015625L;
    type_from d = 999.029541015625L;
    type_to e (d);

    BOOST_CHECK_EQUAL ((e == a || e == b), true);
    BOOST_CHECK_EQUAL (e, c);
  }

  // This test uses types which would have the underlying representation as built-in types 
  // but these built-in types would supposedly be different from each other and rounding mode is round::nearest_even.
  {
    typedef boost::fixed_point::negatable<24, -39, boost::fixed_point::round::nearest_even> type_from; // use int64
    typedef boost::fixed_point::negatable<23, -8, boost::fixed_point::round::nearest_even> type_to;   // use int32

    type_to a = 999.03125L;
    type_to b = 999.029541015625L;
    type_from c = 999.029541015625L;
    type_to d (c);

    BOOST_CHECK_EQUAL (d, a);
    BOOST_CHECK_EQUAL (d, b);
  }

  // This test uses types where one would have built-in type and other would have multiprecision type
  // as underlying representation and rounding mode is round::fastest.
  {
    typedef boost::fixed_point::negatable<25, -420> fixed_point_type_from; // use multiprecision backend
    typedef boost::fixed_point::negatable<21, -10> fixed_point_type_to;   // use int32

    typedef fixed_point_type_from::float_type floating_point_type_from;

    floating_point_type_from big_float_from ("1024.499511718750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003025093114331318471207856075024462507576719799239315603180910706763469153143705915278456705185225159003629620616853584634302761167921671050495463099756392848967738514960376530192422564155070725513250306166850322074879148855369981176783751956850300945259051553504203724287435761652886867523193359375");

    fixed_point_type_to a = 1024.5L;
    fixed_point_type_to b = 1024.4990234375L;
    fixed_point_type_from c (big_float_from);
    fixed_point_type_to d (c);

    BOOST_CHECK_EQUAL ((d == a || d == b), true);
  }

  // This test uses types where one would have built-in type and other would have multiprecision type
  // as underlying representation and rounding mode is round::nearest_even.
  {
    typedef boost::fixed_point::negatable<24, -420, boost::fixed_point::round::nearest_even> fixed_point_type_from; // use multiprecision backend
    typedef boost::fixed_point::negatable<20, -10, boost::fixed_point::round::nearest_even> fixed_point_type_to;   // use int32

    typedef fixed_point_type_from::float_type floating_point_type_from;

    floating_point_type_from big_float_from ("1024.499511718750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003025093114331318471207856075024462507576719799239315603180910706763469153143705915278456705185225159003629620616853584634302761167921671050495463099756392848967738514960376530192422564155070725513250306166850322074879148855369981176783751956850300945259051553504203724287435761652886867523193359375");

    fixed_point_type_to a = 1024.5;
    fixed_point_type_from b (big_float_from);
    fixed_point_type_to c (b);

    BOOST_CHECK_EQUAL (c, a);
  }

  // This test uses types with multiprecision types as underlying representation and 
  // rounding mode is round::fastest.
  {
    typedef boost::fixed_point::negatable<490, -420> fixed_point_type_from;
    typedef boost::fixed_point::negatable<480, -417> fixed_point_type_to;

    typedef fixed_point_type_from::float_type floating_point_type_from;
    typedef fixed_point_type_to::float_type floating_point_type_to;

    floating_point_type_to big_float_to_higher ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.49951171875000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000302546243347602990063908643225496238091423374378811786366231479792532527195131716004897049552891765383441995508402692776513346593671277368400181097707293007187690317599260218963940002997904277663344481847379292375014161731451481941157520400812082350672227448740159161388874053955078125");
    floating_point_type_to big_float_to_lower ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.499511718750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003022507880318338465189243574410963394214122187397699779811082068630476321490427201884860172779377695188097279346640182718487827786383952615951027958540632288603585887734796914063580303387266367671888720018252110348041869641356113533243587597956643015016491016222488497078302316367626190185546875");
    floating_point_type_from big_float_from ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.499511718750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003025093114331318471207856075024462507576719799239315603180910706763469153143705915278456705185225159003629620616853584634302761167921671050495463099756392848967738514960376530192422564155070725513250306166850322074879148855369981176783751956850300945259051553504203724287435761652886867523193359375");

    fixed_point_type_to a (big_float_to_higher);
    fixed_point_type_to b (big_float_to_lower);
    fixed_point_type_from c (big_float_from);
    fixed_point_type_to d (c);

    BOOST_CHECK_EQUAL ((d == a || d == b), true);
  }

  // This test uses types with multiprecision types as underlying representation and
  // rounding mode is round::nearest_even.
  {
    typedef boost::fixed_point::negatable<490, -420, boost::fixed_point::round::nearest_even> fixed_point_type_from;
    typedef boost::fixed_point::negatable<480, -417, boost::fixed_point::round::nearest_even> fixed_point_type_to;

    typedef fixed_point_type_from::float_type floating_point_type_from;
    typedef fixed_point_type_to::float_type floating_point_type_to;

    floating_point_type_to big_float_to ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.49951171875000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000302546243347602990063908643225496238091423374378811786366231479792532527195131716004897049552891765383441995508402692776513346593671277368400181097707293007187690317599260218963940002997904277663344481847379292375014161731451481941157520400812082350672227448740159161388874053955078125");
    floating_point_type_from big_float_from ("82631996098781074868989413504096379978550585370535152410581099409300723904538918228148651304964410605948901538312291876864.499511718750000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000003025093114331318471207856075024462507576719799239315603180910706763469153143705915278456705185225159003629620616853584634302761167921671050495463099756392848967738514960376530192422564155070725513250306166850322074879148855369981176783751956850300945259051553504203724287435761652886867523193359375");

    fixed_point_type_to a (big_float_to);
    fixed_point_type_from b (big_float_from);
    fixed_point_type_to c (b);

    BOOST_CHECK_EQUAL (c, a);
  }
}
