///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//! \file
//!\brief Perform local computations of log(2) for fixed_point.\n
//        Use a quadratically convergent Gauss-AGM method for high digit counts\n
//        or a constant value for low digit counts.

#define BOOST_TEST_MODULE test_negatable_constants_local_ln_two
#define BOOST_LIB_DIAGNOSTIC

#include <type_traits>

#include <boost/cstdint.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  const std::string& ln_two_helper_string()
  {
    // Store 1000 decimal digits of ln_two in the form of a static constant string.
    static const std::string str_ln_two =
        std::string("0.6931471805599453094172321214581765680755001343602552541206800094933936219696947156058633269964186875")
      + std::string(  "4200148102057068573368552023575813055703267075163507596193072757082837143519030703862389167347112335")
      + std::string(  "0115364497955239120475172681574932065155524734139525882950453007095326366642654104239157814952043740")
      + std::string(  "4303855008019441706416715186447128399681717845469570262716310645461502572074024816377733896385506952")
      + std::string(  "6066834113727387372292895649354702576265209885969320196505855476470330679365443254763274495125040606")
      + std::string(  "9438147104689946506220167720424524529612687946546193165174681392672504103802546259656869144192871608")
      + std::string(  "2938031727143677826548775664850856740776484514644399404614226031930967354025744460703080960850474866")
      + std::string(  "3852313818167675143866747664789088143714198549423151997354880375165861275352916610007105355824987941")
      + std::string(  "4729509293113897155998205654392871700072180857610252368892132449713893203784393530887748259701715591")
      + std::string(  "0708823683627589842589185353024363421436706118923678919237231467232172053401649256872747782344535347")
      ;

    return str_ln_two;
  }

  template<typename FixedPointType,
           typename FloatingPointType,
           typename EnableType = void>
  struct constants_helper
  {
  public:
    static FixedPointType ln_two(const int fuzzy_bits)
    {
      // Use at least 15 total significant bits.
      // Use at least  8 resolution bits.

      BOOST_STATIC_ASSERT((FixedPointType::range - FixedPointType::resolution) >= 15);
      BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 8);

      const FixedPointType val_ln_two = boost::fixed_point::negatable_constants<FixedPointType>::ln_two();

      const FixedPointType reference_value  = FixedPointType(FloatingPointType(ln_two_helper_string()));

      BOOST_CHECK_CLOSE_FRACTION(val_ln_two, reference_value, tolerance_maker(fuzzy_bits));

      return val_ln_two;
    }

  private:
    static FixedPointType tolerance_maker(const int fuzzy_bits)
    {
      return ldexp(FixedPointType(0.5F),
                   FixedPointType::resolution + (fuzzy_bits + 1));
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
    static FixedPointType ln_two(const int fuzzy_bits)
    {
      BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 3);
      BOOST_STATIC_ASSERT( FixedPointType::range      >= 0);

      const FixedPointType val_ln_two = boost::fixed_point::negatable_constants<FixedPointType>::ln_two();

      const FloatingPointType reference_value(FloatingPointType("0.6931471805599453094172321214581765680755001343602552541206800094933936219696947156058633269964186875"));

      BOOST_CHECK_CLOSE_FRACTION(val_ln_two, reference_value, tolerance_maker(fuzzy_bits));

      return val_ln_two;
    }

  private:
    static FixedPointType tolerance_maker(const int fuzzy_bits)
    {
      return ldexp(FixedPointType(0.5F),
                   FixedPointType::resolution + (fuzzy_bits + 1));
    }
  };

  template<typename FixedPointType,
           typename FloatingPointType>
  struct constants_helper<FixedPointType,
                          FloatingPointType,
                          typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type>
  {
  public:
    static FixedPointType ln_two(const int fuzzy_bits)
    {
      // Use at least 3 resolution bits.
      // Use at least 0 range bits.

      BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 3);
      BOOST_STATIC_ASSERT( FixedPointType::range      >= 0);

      const FixedPointType val_ln_two = boost::fixed_point::negatable_constants<FixedPointType>::ln_two();

      const FloatingPointType reference_value(FloatingPointType(0.6931471805599453094172321214581765680755L));

      BOOST_CHECK_CLOSE_FRACTION(val_ln_two, reference_value, tolerance_maker(fuzzy_bits));

      return val_ln_two;
    }

  private:
    static FixedPointType tolerance_maker(int fuzzy_bits)
    {
      return ldexp(FixedPointType(0.5F),
                   FixedPointType::resolution + (fuzzy_bits + 1));
    }
  };
}

BOOST_AUTO_TEST_CASE(test_negatable_constants_local_ln_two)
{
  { typedef boost::fixed_point::negatable<0,    -7> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(1)); }
  { typedef boost::fixed_point::negatable<1,   -14> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(2)); }
  { typedef boost::fixed_point::negatable<2,   -22> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(4)); }
  { typedef boost::fixed_point::negatable<2,   -29> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(4)); }
  { typedef boost::fixed_point::negatable<2,   -51> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2,  -125> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2, -2045> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
}
