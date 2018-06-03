///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2016 - 2018.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <cstdint>
#include <limits>
#include <random>

template<typename unsigned_integral_type = std::uint64_t,
         const std::uint_fast16_t number_of_bits = std::numeric_limits<unsigned_integral_type>::digits>
unsigned_integral_type random_unsigned_variate_generator()
{
  static_assert(   std::numeric_limits<unsigned_integral_type>::is_integer == true
                && std::numeric_limits<unsigned_integral_type>::is_signed  == false,
                "error: the unsigned_integral_type must be an true unsigned integral type");

  static_assert(number_of_bits <= std::numeric_limits<unsigned_integral_type>::digits,
                "error: the number_of_bits does not fit in unsigned_integral_type");

  static std::random_device my_random_device;
  static std::mt19937       my_generator(my_random_device());

  std::uniform_int_distribution<> bit_distribution(0, 1);

  unsigned_integral_type unsigned_integral_result(0U);

  const std::uint_fast16_t my_number_of_bits =
    (std::max)(number_of_bits, std::uint_fast16_t(1U));

  unsigned_integral_result = bit_distribution(my_generator);

  for(std::uint_fast16_t i = 1U; i < number_of_bits; ++i)
  {
    unsigned_integral_result <<= 1U;

    unsigned_integral_result |= bit_distribution(my_generator);
  }

  return unsigned_integral_result;
}
