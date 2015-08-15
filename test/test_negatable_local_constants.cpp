///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

//! \file
//!\brief Perform local computations of pi and ln_two for fixed_point. Use quadratically convergent Gauss AGM methods.

#define BOOST_TEST_MODULE test_negatable_local_constants
#define BOOST_LIB_DIAGNOSTIC

#include <cmath>
#include <iomanip>
#include <iostream>
#include <limits>
#include <type_traits>

#include <boost/cstdint.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/test/included/unit_test.hpp>

namespace local
{
  const std::string pi_helper_string()
  {
    // Store 3000 decimal digits of pi in the form of a static constant string.
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
      + std::string(  "3809525720106548586327886593615338182796823030195203530185296899577362259941389124972177528347913151")
      + std::string(  "5574857242454150695950829533116861727855889075098381754637464939319255060400927701671139009848824012")
      + std::string(  "8583616035637076601047101819429555961989467678374494482553797747268471040475346462080466842590694912")
      + std::string(  "9331367702898915210475216205696602405803815019351125338243003558764024749647326391419927260426992279")
      + std::string(  "6782354781636009341721641219924586315030286182974555706749838505494588586926995690927210797509302955")
      + std::string(  "3211653449872027559602364806654991198818347977535663698074265425278625518184175746728909777727938000")
      + std::string(  "8164706001614524919217321721477235014144197356854816136115735255213347574184946843852332390739414333")
      + std::string(  "4547762416862518983569485562099219222184272550254256887671790494601653466804988627232791786085784383")
      + std::string(  "8279679766814541009538837863609506800642251252051173929848960841284886269456042419652850222106611863")
      + std::string(  "0674427862203919494504712371378696095636437191728746776465757396241389086583264599581339047802759009")
      + std::string(  "9465764078951269468398352595709825822620522489407726719478268482601476990902640136394437455305068203")
      + std::string(  "4962524517493996514314298091906592509372216964615157098583874105978859597729754989301617539284681382")
      + std::string(  "6868386894277415599185592524595395943104997252468084598727364469584865383673622262609912460805124388")
      + std::string(  "4390451244136549762780797715691435997700129616089441694868555848406353422072225828488648158456028506")
      + std::string(  "0168427394522674676788952521385225499546667278239864565961163548862305774564980355936345681743241125")
      + std::string(  "1507606947945109659609402522887971089314566913686722874894056010150330861792868092087476091782493858")
      + std::string(  "9009714909675985261365549781893129784821682998948722658804857564014270477555132379641451523746234364")
      + std::string(  "5428584447952658678210511413547357395231134271661021359695362314429524849371871101457654035902799344")
      + std::string(  "0374200731057853906219838744780847848968332144571386875194350643021845319104848100537061468067491927")
      + std::string(  "8191197939952061419663428754440643745123718192179998391015919561814675142691239748940907186494231962")
      ;

    return str_pi;
  }

  const std::string ln_two_helper_string()
  {
    // Store 3000 decimal digits of ln_two in the form of a static constant string.
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
      + std::string(  "6481149418642386776774406069562657379600867076257199184734022651462837904883062033061144630073719489")
      + std::string(  "0027436439650025809365194430411911506080948793067865158870900605203468429736193841289652556539686022")
      + std::string(  "1941229242075743217574890977067526871158170511370091589426654785959648906530584602586683829400228330")
      + std::string(  "0538207400567705304678700184162404418833232798386349001563121889560650553151272199398332030751408426")
      + std::string(  "0914790012651682434438935724727882054862715527418772430024897945401961872339808608316648114909306675")
      + std::string(  "1933931289043164137068139777649817697486890388778999129650361927071088926410523092478391737350122984")
      + std::string(  "2420499568935992206602204654941510613918788574424557751020683703086661948089641218680779020818158858")
      + std::string(  "0001688115973056186676199187395200766719214592236720602539595436541655311295175989940056000366513567")
      + std::string(  "5690512459268257439464831683326249018038242408242314523061409638057007025513877026817851630690255137")
      + std::string(  "0323405380214501901537402950994226299577964742713815736380172987394070424217997226696297993931270693")
      + std::string(  "5747240493386530879758721699645129446491883771156701678598804981838896784134938314014073166472765327")
      + std::string(  "6359192335112333893387095132090592721854713289754707978913844454666761927028855334234298993218037691")
      + std::string(  "5497334026754675887323677834291619181043011609169526554785973289176354555674286387746398710191243175")
      + std::string(  "4255888301206779210280341206879759143081283307230300883494705792496591005860012341561757413272465943")
      + std::string(  "0684354652111350215443415399553818565227502214245664400062761833032064727257219751529082785684213207")
      + std::string(  "9598863896727711955221881904660395700977470651261950527893229608893140562543344255239206203034394177")
      + std::string(  "7357945592125901992559114844024239012554259003129537051922061506434583787873002035414421785758013236")
      + std::string(  "4516607099143831450049858966885772221486528821694181270488607589722032166631283783291567630749872985")
      + std::string(  "7463892826937350984077804939500493399876264755070316221613903484529942491724837340613662263834936811")
      + std::string(  "1684167056925214751383930638455371862687797328895558871634429756244755392366369488877823890174981027")
      ;

    return str_ln_two;
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

      const FixedPointType val_pi        = boost::fixed_point::negatable_constants<FixedPointType>::pi();
      const FixedPointType control_value = FixedPointType(FloatingPointType(pi_helper_string()));

      BOOST_CHECK_CLOSE_FRACTION(val_pi, control_value, tolerance_maker(fuzzy_bits));

      return val_pi;
    }

    static FixedPointType ln_two(const int fuzzy_bits)
    {
      // Use at least 15 total significant bits.
      // Use at least  8 resolution bits.

      BOOST_STATIC_ASSERT((FixedPointType::range - FixedPointType::resolution) >= 15);
      BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 8);

      // The calculation of ln_two requires significantly more precision
      // in both the range as well as the resolution in order to be
      // calculated with the Gauss AGM method. For this reason, we create
      // a large local fixed-point type that is large enough to compensate
      // for all the digit loss in the calculation.

      BOOST_CONSTEXPR_OR_CONST int large_local_fixed_point_resolution = -(((-FixedPointType::resolution) >= 8) ? (-FixedPointType::resolution) : 8);

      BOOST_STATIC_ASSERT(large_local_fixed_point_resolution < 0);

      typedef boost::fixed_point::negatable<((-large_local_fixed_point_resolution) * 2) - 1,
                                               large_local_fixed_point_resolution  * 2>
      large_local_fixed_point_type;

      // Choose m > (N * 1.661), where N is the number of decimal digits requested.
      BOOST_CONSTEXPR_OR_CONST int m = static_cast<int>((((long(-large_local_fixed_point_resolution) * 301L) / 1000L) * 17L) / 10L);

      // Set a0 = 1.
      // Set b0 = 4 / (2^m) = 1 / 2^(m - 2).
      large_local_fixed_point_type ak(1);

      large_local_fixed_point_type bk(ldexp(large_local_fixed_point_type(1), -(m - 2)));

      for(boost::uint_least8_t k = UINT8_C(0); k < UINT8_C(32); ++k)
      {
        const large_local_fixed_point_type a(ak);
        ak += bk;
        ak /= 2U;
        bk  = sqrt(bk * a);

        const bool minimum_number_of_iterations_are_complete = (k > UINT8_C(4));

        if(minimum_number_of_iterations_are_complete)
        {
          const large_local_fixed_point_type delta_ak_bk = fabs(ak - bk);

          // Extract the exponent of the iteration term in order to
          // obtain a rough estimate of the number of base-2 digits
          // that have been obtained in this iteration.

          int exp2;
          static_cast<void>(frexp(delta_ak_bk, &exp2));

          const bool delta_ak_bk_is_zero = ((exp2 == 0) || (delta_ak_bk == 0));

          const bool precision_goal_has_been_reached = (exp2 <= ((large_local_fixed_point_resolution * 3) / 4));

          if(precision_goal_has_been_reached || delta_ak_bk_is_zero)
          {
            break;
          }
        }
      }

      // Define the large floating-point type that corresponds to the
      // large fixed-point type.
      typedef typename large_local_fixed_point_type::float_type large_local_float_point_type;

      // The iteration is finished: Compute ln2 = pi / [AGM(1, 4 / 2^m) * 2m].
      // Note that ak = bk = AGM(...).
      const FixedPointType val_ln_two =
        large_local_fixed_point_type(large_local_float_point_type(pi_helper_string())) / (ak * (2 * m));

      const FixedPointType control_value  = FixedPointType(FloatingPointType(ln_two_helper_string()));

      BOOST_CHECK_CLOSE_FRACTION(val_ln_two, control_value, tolerance_maker(fuzzy_bits));

      return val_ln_two;
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

      static const FixedPointType val_pi(3.1415926535897932384626433832795028841972L);

      static const FixedPointType control_value(boost::math::constants::pi<FloatingPointType>());

      BOOST_CHECK_CLOSE_FRACTION(val_pi, control_value, tolerance_maker(fuzzy_bits));

      return val_pi;
    }

    static FixedPointType ln_two(const int fuzzy_bits)
    {
      // Use at least 3 resolution bits.
      // Use at least 0 range bits.

      BOOST_STATIC_ASSERT(-FixedPointType::resolution >= 3);
      BOOST_STATIC_ASSERT( FixedPointType::range      >= 0);

      static const FixedPointType val_ln_two(0.6931471805599453094172321214581765680755L);

      static const FixedPointType control_value(boost::math::constants::ln_two<FloatingPointType>());

      BOOST_CHECK_CLOSE_FRACTION(val_ln_two, control_value, tolerance_maker(fuzzy_bits));

      return val_ln_two;
    }

  private:
    static FixedPointType tolerance_maker(const int fuzzy_bits)
    {
      return ldexp(FixedPointType(1),
                   FixedPointType::resolution + fuzzy_bits);
    }
  };
}

BOOST_AUTO_TEST_CASE(test_negatable_local_constants)
{
  { typedef boost::fixed_point::negatable< 2,    -3> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(1)); }
  { typedef boost::fixed_point::negatable< 2,    -5> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(1)); }
  { typedef boost::fixed_point::negatable< 2,   -13> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(2)); }
  { typedef boost::fixed_point::negatable< 4,   -19> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(4)); }
  { typedef boost::fixed_point::negatable< 4,   -27> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(4)); }
  { typedef boost::fixed_point::negatable< 8,   -44> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable< 8,   -55> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<16,   -96> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<16,  -111> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<16,  -223> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<16,  -495> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<16, -1007> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<16, -2031> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<16, -4079> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::pi(((-fixed_point_type::resolution + 9) * 1) / 10)); }

  { typedef boost::fixed_point::negatable<0,    -5> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(1)); }
  { typedef boost::fixed_point::negatable<0,    -7> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(1)); }
  { typedef boost::fixed_point::negatable<1,   -14> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(2)); }
  { typedef boost::fixed_point::negatable<1,   -21> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(4)); }
  { typedef boost::fixed_point::negatable<2,   -29> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(4)); }
  { typedef boost::fixed_point::negatable<2,   -50> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2,   -61> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2,  -110> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2,  -125> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2,  -253> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 2) / 10)); }
  { typedef boost::fixed_point::negatable<2,  -509> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<2, -1021> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<2, -2045> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 1) / 10)); }
  { typedef boost::fixed_point::negatable<2, -4093> fixed_point_type; static_cast<void>(local::constants_helper<fixed_point_type, fixed_point_type::float_type>::ln_two(((-fixed_point_type::resolution + 9) * 1) / 10)); }
}
