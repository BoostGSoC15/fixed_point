///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Perform local computations of pi for fixed_point.\n
//        Use a quadratically convergent Gauss-AGM method for high digit counts\n
//        or a constant value for low digit counts.

#define BOOST_TEST_MODULE test_negatable_constants_local_pi
#define BOOST_LIB_DIAGNOSTIC

#include <string>
#include <type_traits>

#include <boost/cstdint.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  const std::string& pi_helper_string()
  {
    // Store 1000 decimal digits of pi in the form of a static constant string.
    static const std::string str_pi =
        std::string("3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679")
      + std::string(  "8214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196")
      + std::string(  "4428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273")
      + std::string(  "7245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094")
      + std::string(  "3305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912")
      + std::string(  "9833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132")
      + std::string(  "0005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235")
      + std::string(  "4201995611212902196086403441815981362977477130996051870721134999999837297804995105973173281609631859")
      + std::string(  "5024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303")
      + std::string(  "5982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989")
      ;

    return str_pi;
  }

  template<typename FixedPointType,
           typename FloatingPointType,
           typename EnableType = void>
  struct constants_helper
  {
  public:
    static FixedPointType pi(const int fuzzy_bits)
    {
      // Use at least 15 total significant bits.
      // Use at least  4 range bits.
      // Use at least  8 resolution bits.

      BOOST_STATIC_ASSERT((FixedPointType::range - FixedPointType::resolution) >= 15);
      BOOST_STATIC_ASSERT( FixedPointType::range >= 4);
      BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 8);

      const FixedPointType val_pi = boost::fixed_point::negatable_constants<FixedPointType>::pi();

      const FixedPointType reference_value = FixedPointType(FloatingPointType(pi_helper_string()));

      BOOST_CHECK_CLOSE_FRACTION(val_pi, reference_value, tolerance_maker(fuzzy_bits));

      return val_pi;
    }

  private:
    static FixedPointType tolerance_maker(const int fuzzy_bits)
    {
      return ldexp(FixedPointType(1),
        FixedPointType::resolution + fuzzy_bits);
    }
  };

  template<typename FixedPointType,
           typename FloatingPointType>
  struct constants_helper<FixedPointType,
                          FloatingPointType,
                          typename std::enable_if<   (std::numeric_limits<FixedPointType>::digits == std::numeric_limits<boost::intmax_t>::digits)
                                                  &&  std::is_floating_point<FloatingPointType>::value == false>::type>
  {
  public:
    static FixedPointType pi(const int fuzzy_bits)
    {
      // Use at least 3 resolution bits.
      // Use at least 2 range bits.

      BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 3);
      BOOST_STATIC_ASSERT( FixedPointType::range      >= 2);

      const FixedPointType val_pi = boost::fixed_point::negatable_constants<FixedPointType>::pi();

      const FloatingPointType reference_value(FloatingPointType("3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170680"));

      BOOST_CHECK_CLOSE_FRACTION(val_pi, reference_value, tolerance_maker(fuzzy_bits));

      return val_pi;
    }

  private:
    static FixedPointType tolerance_maker(const int fuzzy_bits)
    {
      return ldexp(FixedPointType(1),
                   FixedPointType::resolution + fuzzy_bits);
    }
  };

  template<typename FixedPointType,
           typename FloatingPointType>
  struct constants_helper<FixedPointType,
                          FloatingPointType,
                          typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type>
  {
  public:
    static FixedPointType pi(const int fuzzy_bits)
    {
      // Use at least 3 resolution bits.
      // Use at least 2 range bits.

      BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 3);
      BOOST_STATIC_ASSERT( FixedPointType::range      >= 2);

      const FixedPointType val_pi = boost::fixed_point::negatable_constants<FixedPointType>::pi();

      const FloatingPointType reference_value(FloatingPointType(3.1415926535897932384626433832795028841972L));

      BOOST_CHECK_CLOSE_FRACTION(val_pi, reference_value, tolerance_maker(fuzzy_bits));

      return val_pi;
    }

  private:
    static FixedPointType tolerance_maker(const int fuzzy_bits)
    {
      return ldexp(FixedPointType(1),
                   FixedPointType::resolution + fuzzy_bits);
    }
  };
}

BOOST_AUTO_TEST_CASE(test_negatable_constants_local_pi)
{
  { typedef boost::fixed_point::negatable< 2,    -3> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(1)); }
  { typedef boost::fixed_point::negatable< 4,   -19> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(4)); }
  { typedef boost::fixed_point::negatable< 4,   -27> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(4)); }
  { typedef boost::fixed_point::negatable< 8,   -44> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable< 8,   -55> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<16,  -111> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<16,  -223> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<16, -1007> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 1) / 10)); }
}
