//  Copyright Paul A. Bristow 2015.
//  Copyright Christopher Kormanyos 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Test numeric_limits lowest for a list of negatable fixed_point types.

//! \sa http://www.boost.org/doc/libs/1_59_0/libs/test/doc/html/boost_test/tests_organization/test_cases/test_organization_templates.html#ref_BOOST_AUTO_TEST_CASE_TEMPLATE

#define BOOST_TEST_MODULE test_negatable_basic_limits
#define BOOST_LIB_DIAGNOSTIC
//#define ENABLE_LOCAL_TEST_DEBUG_MESSAGES

#include <boost/test/included/unit_test.hpp>
#include <boost/mpl/list.hpp> // for mpl::list
#include <boost/fixed_point/fixed_point.hpp>

#include <limits>
#include <iostream>
#include <type_traits>

using namespace boost::fixed_point;

// List of fixed_point types to use in testing (with rationale).
// All have default rounding and default overflow undefined.
// Split are chosen to be as close as possible to analogous IEEE floating-point layout,
// but can't match exactly because that format has an implicit hidden bit for fraction part,
// and exponent part has a sign bit, so use one less than the exponent bits.
// For a start, cover the edge case of all resolution bits, zero range, 
// so can only represent a fraction -1..0 .. 0.999....
typedef boost::fixed_point::negatable<0, -7> fixed_point_type_0m7; // 8-bit Fraction only.
typedef boost::fixed_point::negatable<2, -5> fixed_point_type_2m5; // 8-bit split, -4 to +3.999
// 8-bit types.
typedef boost::fixed_point::negatable<0, -15> fixed_point_type_0m15; // 16-bit Fraction only.
typedef boost::fixed_point::negatable<4, -11> fixed_point_type_4m11; // 16-bit split (as IEEE binary16).
// 32-bit types.
typedef boost::fixed_point::negatable<0, -31> fixed_point_type_0m31; // 32-bit Fraction only.
typedef boost::fixed_point::negatable<7, -24> fixed_point_type_7m24; // 32-bit split (as IEEE binary32 float).
// 64-bit types.
typedef boost::fixed_point::negatable<0, -63> fixed_point_type_0m63; // 64-bit Fraction only.
typedef boost::fixed_point::negatable<10, -53> fixed_point_type_10m53; // 64-bit split (as IEEE binary64 double).

// 80-bit type
typedef boost::fixed_point::negatable<15, -64> fixed_point_type_15m64; // 80-bit split (as IEEE X86 extended long double).
// Can't match exactly here as 64th bit is used to show that value is denormal).

// 128-bit types that will use float128 where available, else Boost.Multiprecision.
typedef boost::fixed_point::negatable<0, -127> fixed_point_type_0m127; // 128-bit Fraction only.
typedef boost::fixed_point::negatable<14, -113> fixed_point_type_14m113; // 128-bit split (as IEEE binary128).

// 256-bit types that must use Boost.Multiprecision.
typedef boost::fixed_point::negatable<0, -255> fixed_point_type_0m255; // 128-bit Fraction only.
typedef boost::fixed_point::negatable<15, -240> fixed_point_type_15m240; // 128-bit split (as IEEE binary128).


typedef boost::mpl::list<
  fixed_point_type_0m7,  fixed_point_type_2m5,  // 8-bit types.
  fixed_point_type_0m15, fixed_point_type_4m11, // 16-bit types.
  fixed_point_type_0m31, fixed_point_type_7m24, // 32-bit types.
  fixed_point_type_0m63, fixed_point_type_10m53, // 64-bit types.
  fixed_point_type_15m64, // 80-bit type.
  fixed_point_type_0m127, fixed_point_type_14m113, // 128-bit types.
  fixed_point_type_0m255, fixed_point_type_15m240 // 256-bit types.
  > test_types;

BOOST_AUTO_TEST_CASE_TEMPLATE(lowest_test, T, test_types)
{
  T x = (std::numeric_limits<T>::lowest)();
  T mm = -(std::numeric_limits<T>::max)();

  BOOST_CHECK(x < mm);
  BOOST_CHECK(mm > x);
} // BOOST_AUTO_TEST_CASE_TEMPLATE(lowest_test, T, test_types)

BOOST_AUTO_TEST_CASE_TEMPLATE(min_test, T, test_types)
{
  T z; // Default constructor.
  BOOST_CHECK_EQUAL(z, T(0)); // Check that initial value is zero.

  T m((std::numeric_limits<T>::min)()); // Constructor.
  T x = m; // Use assignment operator.

  std::string s = m.bit_pattern(); // "00...001"
  std::size_t d = s.find('1');
  BOOST_CHECK_EQUAL(d, x.all_bits-1);
} // BOOST_AUTO_TEST_CASE_TEMPLATE(mintest_test, T, test_types)


BOOST_AUTO_TEST_CASE_TEMPLATE(max_test, T, test_types)
{
  T z; // Default constructor.
  BOOST_CHECK_EQUAL(z, T(0)); // Check that initial value is zero.

  T m((std::numeric_limits<T>::max)()); // Constructor.
  T x = m; // Use assignment operator.

  std::string s = m.bit_pattern(); // "00...001" 
  std::string e("0");
  e.append(m.all_bits - 1, '1');  // Expected string "011...111.
  BOOST_CHECK_EQUAL(s, e); // Not sign bit.
} // BOOST_AUTO_TEST_CASE_TEMPLATE(maxtest_test, T, test_types)


/*
1>  test_fixed_point_types_limits.cpp
1>  test_fixed_point_types_limits.vcxproj -> J:\Cpp\fixed_point\Debug\test_fixed_point_types_limits.exe
1>
1>  Running 39 test cases...
1>  *** No errors detected

*/