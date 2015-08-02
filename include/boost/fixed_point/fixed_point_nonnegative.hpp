///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2013 - 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// This file is a partial reference implementation for the proposed
// "C++ binary fixed-point arithmetic" as specified in N3352.
// See: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html
// In this particular file, we implement a prototype for the proposed
// nonnegative template class.

// ----------------------------------------------------------------
// This file contains only the nonnegative class.
// ----------------------------------------------------------------

#ifndef FIXED_POINT_NONNEGATIVE_2015_08_01_HPP_
  #define FIXED_POINT_NONNEGATIVE_2015_08_01_HPP_

  #if defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM)

    // When I/O streaming is disabled:
    //   * We must eliminate use of Boost.Multiprecision.
    //   * We must eliminate use of Boost.Math.Constants (because these require istreaming).
    //   * We must eliminate the parts of <boost/cstdfloat.hpp> that require I/O streams.

    #if !defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)
      #error Error: BOOST_FIXED_POINT_DISABLE_IOSTREAM can not be set without also setting BOOST_FIXED_POINT_DISABLE_MULTIPRECISION.
    #endif

    #define BOOST_CSTDFLOAT_NO_LIBQUADMATH_COMPLEX
    #define BOOST_CSTDFLOAT_NO_LIBQUADMATH_IOSTREAM

    #include <algorithm>
    #include <cmath>
    #include <limits>
    #include <type_traits>
    #include <boost/cstdfloat.hpp>
    #include <boost/cstdint.hpp>

  #elif defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION) && !defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM)

    // When multiprecision is disabled but I/O streaming is enabled:
    //   * We must eliminate Boost.Multiprecision.

    #include <algorithm>
    #include <cmath>
    #include <iomanip>
    #include <istream>
    #include <limits>
    #include <ostream>
    #include <sstream>
    #include <string>
    #include <type_traits>
    #include <boost/cstdfloat.hpp>
    #include <boost/cstdint.hpp>
    #include <boost/lexical_cast.hpp>
    #include <boost/math/constants/constants.hpp>

  #else

    // When multiprecision and I/O streaming are enabled:
    //   * We eliminate nothing and include all overhead from
    //     both Boost.Multiprecision as well as I/O streaming.

    #include <algorithm>
    #include <cmath>
    #include <iomanip>
    #include <istream>
    #include <limits>
    #include <ostream>
    #include <sstream>
    #include <string>
    #include <type_traits>
    #include <boost/cstdfloat.hpp>
    #include <boost/cstdint.hpp>
    #include <boost/lexical_cast.hpp>
    #include <boost/math/constants/constants.hpp>
    #include <boost/multiprecision/cpp_bin_float.hpp>
    #include <boost/multiprecision/cpp_int.hpp>

  #endif

  #include <boost/fixed_point/detail/fixed_point_detail.hpp>
  #include <boost/fixed_point/fixed_point_overflow.hpp>
  #include <boost/fixed_point/fixed_point_round.hpp>

  static_assert(std::numeric_limits<boost::uint8_t >::digits ==  8, "Configuration error: the size of boost::uint8_t  must be 8  bits!");
  static_assert(std::numeric_limits<boost::uint16_t>::digits == 16, "Configuration error: the size of boost::uint16_t must be 16 bits!");
  static_assert(std::numeric_limits<boost::uint32_t>::digits == 32, "Configuration error: the size of boost::uint32_t must be 32 bits!");
  static_assert(std::numeric_limits<boost::uint64_t>::digits == 64, "Configuration error: the size of boost::uint64_t must be 64 bits!");

  namespace boost { namespace fixed_point {

  // Forward declaration of the nonnegative class.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  class nonnegative;

  // What follows are forward declarations of elementary transcendental functions mainly from <cmath>.

  // Forward declaration of abs.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> abs(nonnegative<IntegralRange,
                                                   FractionalResolution,
                                                   RoundMode,
                                                   OverflowMode> x);

  // Forward declaration of fabs.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fabs(nonnegative<IntegralRange,
                                                    FractionalResolution,
                                                    RoundMode,
                                                    OverflowMode> x);

  // Forward declaration of frexp.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> frexp(nonnegative<IntegralRange,
                                                     FractionalResolution,
                                                     RoundMode,
                                                     OverflowMode> x, int* expptr);

  // Forward declaration of ldexp.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> ldexp(nonnegative<IntegralRange,
                                                     FractionalResolution,
                                                     RoundMode,
                                                     OverflowMode> x, int exp);

  // Forward declaration of fixed_next.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fixed_next(nonnegative<IntegralRange,
                                              FractionalResolution,
                                              RoundMode,
                                              OverflowMode> x);

  // Forward declaration of fixed_prior.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fixed_prior(nonnegative<IntegralRange,
                                                           FractionalResolution,
                                                           RoundMode,
                                                           OverflowMode> x);

  // Forward declaration of fixed_distance.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fixed_distance(nonnegative<IntegralRange,
                                                              FractionalResolution,
                                                              RoundMode,
                                                              OverflowMode> x);

  // Forward declaration of fixed_advance.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fixed_advance(nonnegative<IntegralRange,
                                                             FractionalResolution,
                                                             RoundMode,
                                                             OverflowMode> x,
                                                             int distance);

  // Forward declaration of fixed_nextafter.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> fixed_nextafter(nonnegative<IntegralRange,
                                                               FractionalResolution,
                                                               RoundMode,
                                                               OverflowMode> x);

  // Forward declaration of sqrt.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline nonnegative<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode> sqrt(nonnegative<IntegralRange,
                                                    FractionalResolution,
                                                    RoundMode,
                                                    OverflowMode> x);

  } } // namespace boost::fixed_point: End of forward declaration of transcendental and cmath functions.

  namespace std
  {
    // Forward declaration of the specialization of std::numeric_limits<nonnegative>.
    template<const int IntegralRange,
             const int FractionalResolution,
             typename RoundMode,
             typename OverflowMode>
    class numeric_limits<boost::fixed_point::nonnegative<IntegralRange,
                                                         FractionalResolution,
                                                         RoundMode,
                                                         OverflowMode>>;
  }

  namespace boost { namespace fixed_point {

  // We will now begin the implementation of the nonnegative class.

  /*!
    \brief Fixed_point nonnegative class used for unsigned fractional arithmetic.
    \details This is a partial reference implementation for the proposed by
      Lawrence Crowl, "C++ binary fixed-point arithmetic" as specified in N3352.\n
      In this particular file, we implement a prototype for the nonnegative template class.\n
      Example: @c boost::fixed_point::nonnegative<2, -5> @c x;\n
      TODO  some more examples here?
    \tparam IntegralRange Integer integer >= 0 defines a range of unsigned number n that is 2^-IntegralRange < n < 2^IntegralRange.
    \tparam FractionalResolution integer <= -1 defines resolution.
      The resolution of a fractional number is 2^FractionalResolution.
    \tparam RoundMode struct defining the rounding behaviour, default round::fastest.\n
    \note  Not all rounding or all overflow modes proposed in N3352 are yet implemented.
    \tparam OverflowMode struct defining the behaviour from rounding, default overflow::undefined.
    \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html
  */

  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode = round::fastest,
           typename OverflowMode = overflow::undefined>
  class nonnegative
  {
  public:
    // The nonnegative class must have at least one fractional digit.
    // Pure integer instantiations of nonnegative allowed
    static_assert(FractionalResolution < 0,
                  "Error: The fractional resolution of nonnegative must be negative and include at least 1 fractional bit.");

    // The nonnegative class can not have a negative integral range.
    static_assert(IntegralRange >= 0,
                  "Error: The integral range of nonnegative must be 0 or more.");

    static_assert(   std::is_same<RoundMode, round::fastest>::value
                  || std::is_same<RoundMode, round::nearest_even>::value,
                  "Error: Only fastest and nearest_even round modes are supported at the moment.");

    static_assert(   std::is_same<OverflowMode, overflow::undefined>::value,
                  "Error: Only undefined overflow mode is supported at the moment.");

    // Make the range, resolution and total number of bits available to the user.
    // These just echo the values of the template parameters.

    /*! Value of template parameter IntegralRange for the nonnegative type.\n
    Usage: boost::fixed_point::nonnegative<2, -5> x; x.range == 2;
    */
    static BOOST_CONSTEXPR_OR_CONST int range = IntegralRange;

    /*! Value of template parameter FractionalResolution for the nonnegative type.\n
    Usage: boost::fixed_point::nonnegative<2, -5> x; x.resolution == -5;
    \note The value of resolution is always negative.
    */
    static BOOST_CONSTEXPR_OR_CONST int resolution = FractionalResolution;

    /*! Total number of bits in the nonnegative type, unsigned without a sign bit.\n
        For example: @c boost::fixed_point::nonnegative<2, -5> @c x; @c int @c n=x.all_bits; @c n==7\n
        x.range + (-x.resolution) == 2 + (-(-5)) == 7.
    */
    static BOOST_CONSTEXPR_OR_CONST int all_bits = range + (-resolution);

    #if defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)
      static_assert(all_bits <= 31, "Error: the width of fixed_point nonnegative can not exceed 31 bits when multiprecision is disabled.");
    #endif

    //! \sa range and resolution, public static data.
    static BOOST_CONSTEXPR_OR_CONST int radix_split = -FractionalResolution;

    // Friend forward declaration of another nonnegative class
    // with different template parameters.

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution,
             typename OtherRoundMode,
             typename OtherOverflowMode>
    friend class nonnegative;

    // Here we declare two convenient class-local type definitions.
    //
    /*!
      The unsigned integer representation of the fixed-point nonnegative number.\n
      For low digit counts, this will be
      a built-in type such as @c uint8_t, @c uint16_t, @c uint32_t, @c uint64_t, etc.\n
      For larger digit counts, this will be a multiprecision unsigned integer type.
      One bit is added to the sum of al_bits == (range + (-resolution))
      in order to provide room for internal rounding mechanisms.
    */
    typedef typename detail::integer_type_helper<all_bits + 1>::exact_unsigned_type value_type;

    /*!
      The floating-point type that is guaranteed to be wide
      enough to represent the fixed-point nonnegative number in its entirety.\n
      For low digit counts, this will be a built-in type such as @c float, @c double or @c long @c double.\n
      Example: @c nonnegative<15, -16> using 32-bits\n
      \code
       Fixed_point Type class boost::fixed_point::nonnegative<15, -16, struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 15, resolution -16
       value_type is @c uint32_t
       float_type is double
      \endcode

      For larger digit counts, this will be a multiprecision floating-point type such as cpp_bin_float.\n
      Example: for a fixed_point type @c nonnegative<10, -53> using 64-bits
      \code
       Fixed_point Type class boost::fixed_point::nonnegative<10, -53, struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 10, resolution -53
       value_type is @c uint64_t
       float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<63, 2, void, int, 0, 0>, 0>

      \endcode

    */
   typedef typename detail::float_type_helper<all_bits>::exact_float_type float_type;

    // The public class constructors follow below.

    // The class default constructor is implemented below.

    /*! Default constructor.\n By design choice, this clears the data member.\n
        So after defining @c nonnegative<15, -16> @c x; then @c x == 0;
    */
    nonnegative() : data() { }

    /*! Constructors from both built-in unsigned integral types,
        as well as from the internal @c value_type of the data member.
    */
    template<typename IntegralType>
    nonnegative(const IntegralType& n,
                const typename std::enable_if<  (   std::is_integral<IntegralType>::value
                                                 && std::is_unsigned<IntegralType>::value)
                                              || std::is_same<IntegralType, value_type>::value>::type* = nullptr)
      : data()
    {
      make_from_unsigned_integral_type(n);
    }

    /*! Constructors from built-in signed integral types.
    */
    template<typename IntegralType>
    nonnegative(const IntegralType& n,
                const typename std::enable_if<   std::is_integral<IntegralType>::value
                                              && std::is_signed<IntegralType>::value>::type* = nullptr)
      : data()
    {
      make_from_signed_integral_type(n);
    }

    /*! Constructors from built-in floating-point types: @c float, @c double or @c long @c double.\n
        Example: nonnegative<15,-16> x(2.3L);\n
        Overflow and underflow is of course possible.
    */

    template<typename FloatingPointType>
    nonnegative(const FloatingPointType& f,
                const typename std::enable_if<   std::is_floating_point<FloatingPointType>::value
                                              || std::is_same<FloatingPointType, float_type>::value>::type* = nullptr)
      : data()
    {
      //make_from_floating_point_type(f);
    }




  private:
    value_type data;

    typedef typename detail::integer_type_helper<(all_bits + 1) * 1>::exact_signed_type   signed_small_type;
    typedef typename detail::integer_type_helper<(all_bits + 1) * 1>::exact_unsigned_type unsigned_small_type;
    typedef typename detail::integer_type_helper<(all_bits + 1) * 2>::exact_unsigned_type unsigned_large_type;

    static const unsigned_small_type& unsigned_small_mask() BOOST_NOEXCEPT
    {
//      initialization_helper.force_premain_init_of_static_constants();

      static const unsigned_small_type the_value =
        detail::bit_mask_helper<unsigned_small_type,
                                0U,
                                unsigned(IntegralRange - FractionalResolution)>::value();

      return the_value;
    }

    struct nothing { };

    /*! \tparam IntegralType Integer type on which the fixed-point type is based, typically the native unsigned integer type @c unsigned @c int,
    but can be a smaller fundamental type like @c short @c int, or a much longer type like @c boost::multiprecision::cpp_int.
    \sa http://www.boost.org/doc/libs/release/libs/multiprecision/doc/html/boost_multiprecision/tut/ints/cpp_int.html
    */
    template<typename IntegralType>
    nonnegative(const nothing&,
                const IntegralType& u) : data(static_cast<value_type>(u)) { }

    template<typename UnsignedIntegralType>
    void make_from_unsigned_integral_type(const UnsignedIntegralType& u)
    {
      data = (value_type(unsigned_small_type(unsigned_small_type(u) << radix_split) & unsigned_small_mask()));
    }

    template<typename SignedIntegralType>
    void make_from_signed_integral_type(const SignedIntegralType& n)
    {
      data = ((!(n < 0)) ? value_type( signed_small_type((unsigned_small_type(+n) << radix_split) & unsigned_small_mask()))
                         : value_type(-signed_small_type((unsigned_small_type(-n) << radix_split) & unsigned_small_mask())));
    }
  };

  } } // namespace boost::fixed_point

#endif // FIXED_POINT_NONNEGATIVE_2015_08_01_HPP_
