///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2018.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the two_component_divide() of (fixed_point::detail).

#include <iomanip>
#include <iostream>
#include <random>

#define BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH
#define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION
#define BOOST_FIXED_POINT_DISABLE_IOSTREAM

#define BOOST_TEST_MODULE test_fixed_point_detail_two_component_divide
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/detail/fixed_point_detail.hpp>
#include <boost/fixed_point/detail/parallel_for.h>
#include <boost/fixed_point/detail/random_unsigned_variate_generator.h>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  template<const std::uint64_t number_of_trials>
  bool test_divide_64_by_32();

  template<const std::uint64_t      number_of_trials,
           const std::uint_fast16_t number_of_bits_left,
           const std::uint_fast16_t number_of_bits_right>
  bool test_divide_xx_by_yy();
}

template<const std::uint64_t number_of_trials>
bool local::test_divide_64_by_32()
{
  bool test_result_is_ok = true;

  // Create a 64-bit random number generator.
  std::mt19937_64 random_generator(std::uint64_t(std::time(nullptr)));

  // Create a 64-bit random distribution.
  std::uniform_int_distribution<std::uint64_t> uint64_distribution(0U, (std::numeric_limits<std::uint64_t>::max)());

  my_concurrency::parallel_for
  (
    std::uint64_t(0U),
    number_of_trials,
    [&test_result_is_ok, &random_generator, &uint64_distribution](std::uint64_t count)
    {
      std::uint64_t a;
      std::uint32_t b;

      for(;;)
      {
        a = uint64_distribution(random_generator);

        if(a != 0U)
        {
          break;
        }
      }

      for(;;)
      {
        b = random_unsigned_variate_generator<std::uint32_t, 32>();

        if(b != 0U)
        {
          break;
        }
      }

      std::uint32_t result_lo;
      std::uint32_t result_hi;

      boost::fixed_point::detail::two_component_divide<std::uint32_t>(std::uint32_t(a),
                                                                      std::uint32_t(a >> 32),
                                                                      b,
                                                                      result_lo,
                                                                      result_hi);

      const std::uint64_t control           = a / b;

      const std::uint64_t result_as_uint64  = result_lo | (std::uint64_t(result_hi) << 32);

      test_result_is_ok &= (control == result_as_uint64);
    }
  );

  return test_result_is_ok;
}

template<const std::uint64_t      number_of_trials,
         const std::uint_fast16_t number_of_bits_left,
         const std::uint_fast16_t number_of_bits_right>
bool local::test_divide_xx_by_yy()
{
  static_assert(number_of_bits_left <= 64U,
                "The number of bits on the left hand side of the division can not exceed 64.");

  static_assert(number_of_bits_right <= 32U,
                "The number of bits on the right hand side of the division can not exceed 32.");

  bool test_result_is_ok = true;

  my_concurrency::parallel_for
  (
    std::uint64_t(0U),
    number_of_trials,
    [&test_result_is_ok](std::uint64_t count)
    {
      std::uint64_t a;
      std::uint32_t b;

      for(;;)
      {
        a = random_unsigned_variate_generator<std::uint64_t, number_of_bits_left>();

        if(a != 0U)
        {
          break;
        }
      }

      for(;;)
      {
        b = random_unsigned_variate_generator<std::uint32_t, number_of_bits_right>();

        if(b != 0U)
        {
          break;
        }
      }

      std::uint32_t result_lo;
      std::uint32_t result_hi;

      boost::fixed_point::detail::two_component_divide<std::uint32_t>(std::uint32_t(a),
                                                                      std::uint32_t(a >> 32),
                                                                      b,
                                                                      result_lo,
                                                                      result_hi);

      const std::uint64_t control           = a / b;

      const std::uint64_t result_as_uint64  = result_lo | (std::uint64_t(result_hi) << 32);

      test_result_is_ok &= (control == result_as_uint64);
    }
  );

  return test_result_is_ok;
}

BOOST_AUTO_TEST_CASE(test_fixed_point_detail_two_component_divide)
{
  const bool test_divide_64_by_32_is_ok = local::test_divide_64_by_32<25000000ULL>();
  BOOST_CHECK_EQUAL(test_divide_64_by_32_is_ok, true);
  std::cout << "test_divide_64_by_32_is_ok: " << std::boolalpha << test_divide_64_by_32_is_ok << std::endl;

  const bool test_divide_64_by_31_is_ok = local::test_divide_xx_by_yy<25000000ULL, 64U, 31U>();
  BOOST_CHECK_EQUAL(test_divide_64_by_31_is_ok, true);
  std::cout << "test_divide_64_by_31_is_ok: " << std::boolalpha << test_divide_64_by_31_is_ok << std::endl;

  const bool test_divide_64_by_17_is_ok = local::test_divide_xx_by_yy<25000000ULL, 64U, 17U>();
  BOOST_CHECK_EQUAL(test_divide_64_by_17_is_ok, true);
  std::cout << "test_divide_64_by_17_is_ok: " << std::boolalpha << test_divide_64_by_17_is_ok << std::endl;

  const bool test_divide_64_by_16_is_ok = local::test_divide_xx_by_yy<25000000ULL, 64U, 16U>();
  BOOST_CHECK_EQUAL(test_divide_64_by_16_is_ok, true);
  std::cout << "test_divide_64_by_16_is_ok: " << std::boolalpha << test_divide_64_by_16_is_ok << std::endl;

  const bool test_divide_64_by_15_is_ok = local::test_divide_xx_by_yy<25000000ULL, 64U, 15U>();
  BOOST_CHECK_EQUAL(test_divide_64_by_15_is_ok, true);
  std::cout << "test_divide_64_by_15_is_ok: " << std::boolalpha << test_divide_64_by_15_is_ok << std::endl;

  const bool test_divide_64_by_09_is_ok = local::test_divide_xx_by_yy<25000000ULL, 64U, 9U>();
  BOOST_CHECK_EQUAL(test_divide_64_by_09_is_ok, true);
  std::cout << "test_divide_64_by_09_is_ok: " << std::boolalpha << test_divide_64_by_09_is_ok << std::endl;

  const bool test_divide_64_by_08_is_ok = local::test_divide_xx_by_yy<25000000ULL, 64U, 8U>();
  BOOST_CHECK_EQUAL(test_divide_64_by_08_is_ok, true);
  std::cout << "test_divide_64_by_08_is_ok: " << std::boolalpha << test_divide_64_by_08_is_ok << std::endl;

  const bool test_divide_64_by_07_is_ok = local::test_divide_xx_by_yy<25000000ULL, 64U, 7U>();
  BOOST_CHECK_EQUAL(test_divide_64_by_07_is_ok, true);
  std::cout << "test_divide_64_by_07_is_ok: " << std::boolalpha << test_divide_64_by_07_is_ok << std::endl;

  const bool test_divide_49_by_32_is_ok = local::test_divide_xx_by_yy<25000000ULL, 49U, 32U>();
  BOOST_CHECK_EQUAL(test_divide_49_by_32_is_ok, true);
  std::cout << "test_divide_49_by_32_is_ok: " << std::boolalpha << test_divide_49_by_32_is_ok << std::endl;

  const bool test_divide_49_by_31_is_ok = local::test_divide_xx_by_yy<25000000ULL, 49U, 31U>();
  BOOST_CHECK_EQUAL(test_divide_49_by_31_is_ok, true);
  std::cout << "test_divide_49_by_31_is_ok: " << std::boolalpha << test_divide_49_by_31_is_ok << std::endl;

  const bool test_divide_48_by_32_is_ok = local::test_divide_xx_by_yy<25000000ULL, 48U, 32U>();
  BOOST_CHECK_EQUAL(test_divide_48_by_32_is_ok, true);
  std::cout << "test_divide_48_by_32_is_ok: " << std::boolalpha << test_divide_48_by_32_is_ok << std::endl;

  const bool test_divide_48_by_31_is_ok = local::test_divide_xx_by_yy<25000000ULL, 48U, 31U>();
  BOOST_CHECK_EQUAL(test_divide_48_by_31_is_ok, true);
  std::cout << "test_divide_48_by_31_is_ok: " << std::boolalpha << test_divide_48_by_31_is_ok << std::endl;

  const bool test_divide_48_by_17_is_ok = local::test_divide_xx_by_yy<25000000ULL, 48U, 17U>();
  BOOST_CHECK_EQUAL(test_divide_48_by_17_is_ok, true);
  std::cout << "test_divide_48_by_17_is_ok: " << std::boolalpha << test_divide_48_by_17_is_ok << std::endl;

  const bool test_divide_48_by_16_is_ok = local::test_divide_xx_by_yy<25000000ULL, 48U, 16U>();
  BOOST_CHECK_EQUAL(test_divide_48_by_16_is_ok, true);
  std::cout << "test_divide_48_by_16_is_ok: " << std::boolalpha << test_divide_48_by_16_is_ok << std::endl;

  const bool test_divide_48_by_15_is_ok = local::test_divide_xx_by_yy<25000000ULL, 48U, 15U>();
  BOOST_CHECK_EQUAL(test_divide_48_by_15_is_ok, true);
  std::cout << "test_divide_48_by_15_is_ok: " << std::boolalpha << test_divide_48_by_15_is_ok << std::endl;

  const bool test_divide_33_by_32_is_ok = local::test_divide_xx_by_yy<25000000ULL, 33U, 32U>();
  BOOST_CHECK_EQUAL(test_divide_33_by_32_is_ok, true);
  std::cout << "test_divide_33_by_32_is_ok: " << std::boolalpha << test_divide_33_by_32_is_ok << std::endl;

  const bool test_divide_33_by_31_is_ok = local::test_divide_xx_by_yy<25000000ULL, 33U, 31U>();
  BOOST_CHECK_EQUAL(test_divide_33_by_31_is_ok, true);
  std::cout << "test_divide_33_by_31_is_ok: " << std::boolalpha << test_divide_33_by_31_is_ok << std::endl;

  const bool test_divide_32_by_32_is_ok = local::test_divide_xx_by_yy<25000000ULL, 32U, 32U>();
  BOOST_CHECK_EQUAL(test_divide_32_by_32_is_ok, true);
  std::cout << "test_divide_32_by_32_is_ok: " << std::boolalpha << test_divide_32_by_32_is_ok << std::endl;

  const bool test_divide_32_by_31_is_ok = local::test_divide_xx_by_yy<25000000ULL, 32U, 31U>();
  BOOST_CHECK_EQUAL(test_divide_32_by_31_is_ok, true);
  std::cout << "test_divide_32_by_31_is_ok: " << std::boolalpha << test_divide_32_by_31_is_ok << std::endl;

  const bool test_divide_32_by_17_is_ok = local::test_divide_xx_by_yy<25000000ULL, 32U, 17U>();
  BOOST_CHECK_EQUAL(test_divide_32_by_17_is_ok, true);
  std::cout << "test_divide_32_by_17_is_ok: " << std::boolalpha << test_divide_32_by_17_is_ok << std::endl;

  const bool test_divide_32_by_16_is_ok = local::test_divide_xx_by_yy<25000000ULL, 32U, 16U>();
  BOOST_CHECK_EQUAL(test_divide_32_by_16_is_ok, true);
  std::cout << "test_divide_32_by_16_is_ok: " << std::boolalpha << test_divide_32_by_16_is_ok << std::endl;

  const bool test_divide_32_by_15_is_ok = local::test_divide_xx_by_yy<25000000ULL, 32U, 15U>();
  BOOST_CHECK_EQUAL(test_divide_32_by_15_is_ok, true);
  std::cout << "test_divide_32_by_15_is_ok: " << std::boolalpha << test_divide_32_by_15_is_ok << std::endl;

  const bool test_divide_16_by_09_is_ok = local::test_divide_xx_by_yy<25000000ULL, 16U, 9U>();
  BOOST_CHECK_EQUAL(test_divide_16_by_09_is_ok, true);
  std::cout << "test_divide_32_by_09_is_ok: " << std::boolalpha << test_divide_16_by_09_is_ok << std::endl;

  const bool test_divide_16_by_08_is_ok = local::test_divide_xx_by_yy<25000000ULL, 16U, 8U>();
  BOOST_CHECK_EQUAL(test_divide_16_by_08_is_ok, true);
  std::cout << "test_divide_32_by_08_is_ok: " << std::boolalpha << test_divide_16_by_08_is_ok << std::endl;

  const bool test_divide_16_by_07_is_ok = local::test_divide_xx_by_yy<25000000ULL, 16U, 7U>();
  BOOST_CHECK_EQUAL(test_divide_16_by_07_is_ok, true);
  std::cout << "test_divide_32_by_07_is_ok: " << std::boolalpha << test_divide_16_by_07_is_ok << std::endl;
}
