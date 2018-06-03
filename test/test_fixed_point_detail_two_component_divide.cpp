///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2018.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for the two_component_divide() of (fixed_point::detail).

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
  // Create a 64-bit random number generator.
  std::mt19937_64 random_generator(std::uint64_t(std::time(nullptr)));

  // Create a 64-bit random distribution.
  std::uniform_int_distribution<std::uint64_t> uint64_distribution(0U, (std::numeric_limits<std::uint64_t>::max)());

  bool test_divide_64_by_32();
}

bool local::test_divide_64_by_32()
{
  bool test_result_is_ok = true;

  my_concurrency::parallel_for
  (
    std::uint64_t(0U),
    std::uint64_t(100000000ULL),
    [&test_result_is_ok](std::uint64_t count)
    {
      const std::uint64_t a = local::uint64_distribution(local::random_generator);
            std::uint32_t b;

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

BOOST_AUTO_TEST_CASE(test_fixed_point_detail_two_component_divide)
{
  const bool test_divide_64_by_32_is_ok = local::test_divide_64_by_32();

  BOOST_CHECK_EQUAL(test_divide_64_by_32_is_ok, true);
}
