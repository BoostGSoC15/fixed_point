///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Tests for sqrt(fixed_point) for a big digit range. Along the way, also test numerous fixed_point arithmetic operations.

#include <cmath>

#define BOOST_TEST_MODULE test_negatable_func_sqrt_big
#define BOOST_LIB_DIAGNOSTIC

#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  const std::string& str_pi_reference()
  {
    static const std::string str_pi =
      std::string("3.")
    + std::string("1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679")
    + std::string("8214808651328230664709384460955058223172535940812848111745028410270193852110555964462294895493038196")
    + std::string("4428810975665933446128475648233786783165271201909145648566923460348610454326648213393607260249141273")
    + std::string("7245870066063155881748815209209628292540917153643678925903600113305305488204665213841469519415116094")
    + std::string("3305727036575959195309218611738193261179310511854807446237996274956735188575272489122793818301194912")
    + std::string("9833673362440656643086021394946395224737190702179860943702770539217176293176752384674818467669405132")
    + std::string("0005681271452635608277857713427577896091736371787214684409012249534301465495853710507922796892589235")
    + std::string("4201995611212902196086403441815981362977477130996051870721134999999837297804995105973173281609631859")
    + std::string("5024459455346908302642522308253344685035261931188171010003137838752886587533208381420617177669147303")
    + std::string("5982534904287554687311595628638823537875937519577818577805321712268066130019278766111959092164201989")
    + std::string("3809525720106548586327886593615338182796823030195203530185296899577362259941389124972177528347913151")
    + std::string("5574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012")
    + std::string("8583616035637076601047101819429555961989467678374494482553797747268471040475346462080466842590694912")
    + std::string("9331367702898915210475216205696602405803815019351125338243003558764024749647326391419927260426992280")
    ;

    return str_pi;
  }

  template<typename FixedPointType>
  const FixedPointType& tolerance_maker(const int fuzzy_bits)
  {
    static const FixedPointType the_tolerance = ldexp(FixedPointType(1), FixedPointType::resolution + fuzzy_bits);

    return the_tolerance;
  }

  template<typename FixedPointType,
           typename FloatPointType = typename FixedPointType::float_type>
  void test_big_sqrt_pi(const int fuzzy_bits)
  {
    const FixedPointType a(boost::math::constants::pi<FixedPointType>());
    const FloatPointType b(str_pi_reference().c_str());

    BOOST_CHECK_CLOSE_FRACTION(sqrt(a), FixedPointType(sqrt(b)), tolerance_maker<FixedPointType>(fuzzy_bits));
  }
}

BOOST_AUTO_TEST_CASE(test_negatable_func_sqrt_big)
{
  // Test sqrt() for negatable in a large digit range using the result
  // of a non-trivial calculation (in this case pi). This also
  // exercises negatable with numerous arithmetic operations.

  { typedef boost::fixed_point::negatable<10, -4085, boost::fixed_point::round::fastest>      fixed_point_type; local::test_big_sqrt_pi<fixed_point_type>(fixed_point_type::range); }
  { typedef boost::fixed_point::negatable<10, -4085, boost::fixed_point::round::nearest_even> fixed_point_type; local::test_big_sqrt_pi<fixed_point_type>(fixed_point_type::range); }
}
