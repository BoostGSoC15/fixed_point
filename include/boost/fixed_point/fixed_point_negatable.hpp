///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2013 - 2015.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is a partial reference implementation for the proposed
// "C++ binary fixed-point arithmetic" as specified in N3352.

/*! \file
    \brief Fixed_point negatable class used for @b signed fractional arithmetic.
    \details This is a partial reference implementation for the proposed by
       Lawrence Crowl, "C++ binary fixed-point arithmetic" as specified in N3352.\n

   In this particular file, we implement a prototype for the proposed
   @b negatable template class. (See fixed_point_nonnegative.hpp for an unsigned version).\n
   \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html
*/

// ----------------------------------------------------------------
// This file contains only the negatable class.
// ----------------------------------------------------------------

#ifndef FIXED_POINT_NEGATABLE_2015_03_06_HPP_
  #define FIXED_POINT_NEGATABLE_2015_03_06_HPP_

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

  // Forward declaration of the negatable class.
  template<const int IntegralRange,
            const int FractionalResolution,
            typename RoundMode,
            typename OverflowMode>
  class negatable;

  // What follows are forward declarations of elementary transcendental functions mainly from <cmath>.

  // Forward declaration of abs.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline negatable<IntegralRange,
                   FractionalResolution,
                   RoundMode,
                   OverflowMode> abs(negatable<IntegralRange,
                                               FractionalResolution,
                                               RoundMode,
                                               OverflowMode> x);

  // Forward declaration of fabs.
  template<const int IntegralRange,
            const int FractionalResolution,
            typename RoundMode,
            typename OverflowMode>
  inline negatable<IntegralRange,
                   FractionalResolution,
                   RoundMode,
                   OverflowMode> fabs(negatable<IntegralRange,
                                                FractionalResolution,
                                                RoundMode,
                                                OverflowMode> x);

  // Forward declaration of frexp.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline negatable<IntegralRange,
                   FractionalResolution,
                   RoundMode,
                   OverflowMode> frexp(negatable<IntegralRange,
                                                 FractionalResolution,
                                                 RoundMode,
                                                 OverflowMode> x, int* exp2);

  // Forward declaration of ldexp.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline negatable<IntegralRange,
                   FractionalResolution,
                   RoundMode,
                   OverflowMode> ldexp(negatable<IntegralRange,
                                                 FractionalResolution,
                                                 RoundMode,
                                                 OverflowMode> x, int exp2);


  // Forward declaration of fixed_next.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline negatable<IntegralRange,
                   FractionalResolution,
                   RoundMode,
                   OverflowMode> fixed_next(negatable<IntegralRange,
                                                      FractionalResolution,
                                                      RoundMode,
                                                      OverflowMode> x);

  // Forward declaration of fixed_prior.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline negatable<IntegralRange,
                   FractionalResolution,
                   RoundMode,
                   OverflowMode> fixed_prior(negatable<IntegralRange,
                                                       FractionalResolution,
                                                       RoundMode,
                                                       OverflowMode> x);

  // Forward declaration of fixed_distance.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline negatable<IntegralRange,
                   FractionalResolution,
                   RoundMode,
                   OverflowMode> fixed_distance(negatable<IntegralRange,
                                                          FractionalResolution,
                                                          RoundMode,
                                                          OverflowMode> x);

  // Forward declaration of fixed_advance.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline negatable<IntegralRange,
                   FractionalResolution,
                   RoundMode,
                   OverflowMode> fixed_advance(negatable<IntegralRange,
                                                         FractionalResolution,
                                                         RoundMode,
                                                         OverflowMode> x,
                                                         int distance);

  // Forward declaration of fixed_nextafter.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline negatable<IntegralRange,
                   FractionalResolution,
                   RoundMode,
                   OverflowMode> fixed_nextafter(negatable<IntegralRange,
                                                           FractionalResolution,
                                                           RoundMode,
                                                           OverflowMode> x);

  // Forward declaration of sqrt.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  inline negatable<IntegralRange,
                   FractionalResolution,
                   RoundMode,
                   OverflowMode> sqrt(negatable<IntegralRange,
                                                FractionalResolution,
                                                RoundMode,
                                                OverflowMode> x);

  } } // namespace boost::fixed_point: 
  // End of forward declaration of transcendental and cmath functions.

  namespace std
  {
    // Forward declaration of the specialization of std::numeric_limits<negatable>.
    template<const int IntegralRange,
             const int FractionalResolution,
             typename RoundMode,
             typename OverflowMode>
    class numeric_limits<boost::fixed_point::negatable<IntegralRange,
                                                       FractionalResolution,
                                                       RoundMode,
                                                       OverflowMode>>;
  }

  namespace boost { namespace fixed_point {

  // We will now begin the implementation of the negatable class.

  /*!
    \brief Fixed_point negatable class used for signed fractional arithmetic.
    \details This is a partial reference implementation for the proposed by
      Lawrence Crowl, "C++ binary fixed-point arithmetic" as specified in N3352.\n
      In this particular file, we implement a prototype for the @c negatable template class.\n
      Examples:\n
      @c boost::fixed_point::negatable<2,-5> @c x; // 8-bit\n
      @c negatable<15, -16> @c y;  // 32-bit \n
      @c negatable<10, -53> @c y;  // 64-bit \n
      @c negatable<10, -245> @c y;  // 256-bit (requires use of @c cpp_bin_float)\n

    \tparam IntegralRange integer >= 0, defines a range of signed number n that is 2^-IntegralRange < n < 2^IntegralRange.
    \tparam FractionalResolution integer <= -1, defines resolution. 
      The resolution of a fractional number is 2^FractionalResolution.
    \tparam RoundMode struct defining the rounding behaviour, default @c round::fastest.\n
    \tparam OverflowMode struct defining the behaviour from rounding, default @c overflow::undefined.
    \note  Not all rounding or all overflow modes proposed in N3352 are yet implemented.
    \sa http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html
  */

  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode = round::fastest,
           typename OverflowMode = overflow::undefined>
  class negatable
  {
  public:
    // The negatable class must have at least one fractional digit.
    // Pure integer instantiations of negatable allowed
    static_assert(FractionalResolution < 0,
                  "Error: The fractional resolution of negatable must be negative and include at least 1 fractional bit.");

    // The negatable class can not have a negative integral range.
    static_assert(IntegralRange >= 0,
                  "Error: The integral range of negatable must be 0 or more.");

    static_assert(   std::is_same<RoundMode, round::fastest>::value
                  || std::is_same<RoundMode, round::nearest_even>::value,
                  "Error: Only fastest and nearest_even round modes are supported at the moment.");

    static_assert(   std::is_same<OverflowMode, overflow::undefined>::value,
                  "Error: Only undefined overflow mode is supported at the moment.");

    // Make the range, resolution and total number of bits available to the user.
    // These just echo the values of the template parameters.

    /*! Value of template parameter IntegralRange for the negatable type.\n
    Example: boost::fixed_point::negatable<2, -5> x; x.range == 2;
    */
    static BOOST_CONSTEXPR_OR_CONST int range = IntegralRange;

    /*! Value of template parameter FractionalResolution for the negatable type.\n
    Example: boost::fixed_point::negatable<2, -5> x; x.resolution == -5;
    \note The value of resolution is always negative.
    */
    static BOOST_CONSTEXPR_OR_CONST int resolution = FractionalResolution;

    /*! Total number of bits in the negatable type, including sign.\n
        For example:
        \code
        boost::fixed_point::negatable<2, -5> x; int n=x.all_bits; n==8 \n
        x.range + (-x.resolution) + 1 == 2 + (-(-5)) + 1 == 8.
        \endcode
    */
    static BOOST_CONSTEXPR_OR_CONST int all_bits = (range + 1) + (-resolution); // +1 for a sign bit.

    #if defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)
      static_assert(all_bits <= 32, "Error: the width of fixed_point negatable can not exceed 32 bits when multiprecision is disabled.");
    #endif

    //! See also public static data items range and resolution.
    static BOOST_CONSTEXPR_OR_CONST int radix_split = -FractionalResolution;

    // Friend forward declaration of another negatable class
    // with different template parameters.

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution,
             typename OtherRoundMode,
             typename OtherOverflowMode>
    friend class negatable;

    // Here we declare two convenient class-local type definitions.
    //
    /*!
      The signed integer representation of the fixed-point negatable number.\n
      For low digit counts, this will be
      a built-in type such as @c int8_t, @c int16_t, @c int32_t, @c int64_t, etc.\n
      For larger digit counts, this will be a multiprecision signed integer type.
    */
    typedef typename detail::integer_type_helper<all_bits>::exact_signed_type value_type;

    /*!
      The floating-point type that is guaranteed to be wide
      enough to represent the fixed-point negatable number in its entirety.\n
      For low digit counts, this will be a built-in type such as @c float, @c double or @c long @c double.\n
      Example: @c negatable<15, -16> using 32-bits\n
      \code
       Fixed_point Type class boost::fixed_point::negatable<15,-16,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 15, resolution -16
       value_type is int
       float_type is double
      \endcode

      For larger digit counts, this will be a multiprecision floating-point type such as @c cpp_bin_float.\n
      Example: for a fixed_point type @c negatable<10, -53> using 64-bits
      \code
       Fixed_point Type class boost::fixed_point::negatable<10,-53,struct boost::fixed_point::round::fastest,struct boost::fixed_point::overflow::undefined> with range 10, resolution -53
       value_type is __int64
       float_type is class boost::multiprecision::number<class boost::multiprecision::backends::cpp_bin_float<63,2,void,int,0,0>,0>

      \endcode

    */
   typedef typename detail::float_type_helper<all_bits - 1>::exact_float_type float_type;

    // The public class constructors follow below.

    // The class default constructor is implemented below.

    /*! Default constructor.\n By design choice, this clears the data member.\n 
        So after defining @c negatable<15,-16> @c x; then @c x==0; 
    */
    negatable() : data() { }

    /*! Constructors from built-in unsigned integral types.
    */
    template<typename IntegralType>
    negatable(const IntegralType& u,
              const typename std::enable_if<(   std::is_integral<IntegralType>::value
                                             && std::is_unsigned<IntegralType>::value)>::type* = nullptr)
      : data()
    {
      make_from_unsigned_integral_type(u);
    }

    /*! Constructors from both built-in signed integral types,
        as well as from the internal @c value_type of the data member.
    */
    template<typename IntegralType>
    negatable(const IntegralType& n,
              const typename std::enable_if<  (   std::is_integral<IntegralType>::value
                                               && std::is_signed  <IntegralType>::value)
                                            || std::is_same<IntegralType, value_type>::value>::type* = nullptr)
      : data()
    {
      make_from_signed_integral_type(n);
    }

    /*! Constructors from built-in floating-point types: @c float, @c double or @c long @c double.\n
        Example: negatable<15,-16> x(2.3L);\n
        (Overflow and underflow is, of course, possible).
    */
    template<typename FloatingPointType>
    negatable(const FloatingPointType& f,
              const typename std::enable_if<   std::is_floating_point<FloatingPointType>::value
                                            || std::is_same<FloatingPointType, float_type>::value>::type* = nullptr)
      : data()
    {
      make_from_floating_point_type(f);
    }

    /*! Copy constructors, including mixed-math class constructors that
        create a negatable type from another negatable type
        having @b different range and/or resolution.\n
        Copying a value of same type is simple.\n
        We can identify four mixed-math cases:\n
        1) smaller : smaller --> ( OtherIntegralRange <= IntegralRange) && (|OtherFractionalResolution| <= |FractionalResolution|)\n
        2) larger  : smaller --> ( OtherIntegralRange >  IntegralRange) && (|OtherFractionalResolution| <= |FractionalResolution|)\n
        3) smaller : larger  --> ( OtherIntegralRange <= IntegralRange) && (|OtherFractionalResolution| >  |FractionalResolution|)\n
        4) larger  : larger  --> ( OtherIntegralRange >  IntegralRange) && (|OtherFractionalResolution| >  |FractionalResolution|)\n

        Design choices were made that the result of @c (a+b) has the type of @c a.
    */

    negatable(const negatable& v) : data(v.data) { }

    // What follows are mixed-math class constructors that
    // create a negatable type from another negatable type
    // having different range and/or resolution.

    // We can identify four cases:
    // 1) smaller : smaller -->    ( OtherIntegralRange         <=  IntegralRange)
    //                          && (|OtherFractionalResolution| <= |FractionalResolution|)
    // 2) larger  : smaller -->    ( OtherIntegralRange         >   IntegralRange)
    //                          && (|OtherFractionalResolution| <= |FractionalResolution|)
    // 3) smaller : larger  -->    ( OtherIntegralRange         <=  IntegralRange)
    //                          && (|OtherFractionalResolution| >  |FractionalResolution|)
    // 4) larger  : larger  -->    ( OtherIntegralRange         >   IntegralRange)
    //                          && (|OtherFractionalResolution| >  |FractionalResolution|)

    // Here is the mixed-math class constructor for case 1).
    // There is less range and less resolution in the other type.
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution,
             typename std::enable_if<   (   ( OtherIntegralRange        <   IntegralRange)
                                         && (-OtherFractionalResolution <  -FractionalResolution))
                                     || (   ( OtherIntegralRange        <=  IntegralRange)
                                         && (-OtherFractionalResolution <  -FractionalResolution))
                                     || (   ( OtherIntegralRange        <   IntegralRange)
                                         && (-OtherFractionalResolution <= -FractionalResolution))>::type* = nullptr>
    negatable(const negatable<OtherIntegralRange,
                              OtherFractionalResolution,
                              RoundMode,
                              OverflowMode>& other) : data()
    {
      typedef negatable<OtherIntegralRange,
                        OtherFractionalResolution,
                        RoundMode,
                        OverflowMode> other_negatable_type;

      typedef unsigned_small_type superior_unsigned_small_type;

      const bool is_neg = (other.data < 0);

      superior_unsigned_small_type u_superior((!is_neg) ? superior_unsigned_small_type(+other.data)
                                                        : superior_unsigned_small_type(-other.data));

      BOOST_CONSTEXPR_OR_CONST int total_left_shift = radix_split - other_negatable_type::radix_split;

      u_superior = (u_superior << total_left_shift);

      unsigned_small_type u_round = static_cast<unsigned_small_type>(u_superior);

      data = ((!is_neg) ? value_type(u_round) : -value_type(u_round));
    }

    // Here is the mixed-math class constructor for case 2).
    // There is more range and less resolution in the other type.
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution,
             typename std::enable_if<   ( OtherIntegralRange        >  IntegralRange)
                                     && (-OtherFractionalResolution < -FractionalResolution)>::type* = nullptr>
    negatable(const negatable<OtherIntegralRange,
                              OtherFractionalResolution,
                              RoundMode,
                              OverflowMode>& other) : data()
    {
      typedef negatable<OtherIntegralRange,
                        OtherFractionalResolution,
                        RoundMode,
                        OverflowMode> other_negatable_type;

      typedef unsigned_small_type superior_unsigned_small_type;

      const bool is_neg = (other.data < 0);

      superior_unsigned_small_type u_superior((!is_neg) ? superior_unsigned_small_type(+other.data)
                                                        : superior_unsigned_small_type(-other.data));

      BOOST_CONSTEXPR_OR_CONST int total_left_shift = radix_split - other_negatable_type::radix_split;

      u_superior = (u_superior << total_left_shift);

      unsigned_small_type u_round = static_cast<unsigned_small_type>(u_superior);

      u_round = (u_round & unsigned_small_mask());

      data = ((!is_neg) ? value_type(u_round) : -value_type(u_round));
    }

    // Here is the mixed-math class constructor for case 3).
    // There is less range and more resolution in the other type.
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution,
             typename std::enable_if<   ( OtherIntegralRange        <   IntegralRange)
                                     && (-OtherFractionalResolution >  -FractionalResolution)>::type* = nullptr>
    negatable(const negatable<OtherIntegralRange,
                              OtherFractionalResolution,
                              RoundMode,
                              OverflowMode>& other) : data()
    {
      typedef negatable<OtherIntegralRange,
                        OtherFractionalResolution,
                        RoundMode,
                        OverflowMode> other_negatable_type;

      typedef typename other_negatable_type::unsigned_small_type superior_unsigned_small_type;

      const bool is_neg = (other.data < 0);

      superior_unsigned_small_type u_superior((!is_neg) ? superior_unsigned_small_type(+other.data)
                                                        : superior_unsigned_small_type(-other.data));

      BOOST_CONSTEXPR_OR_CONST int total_right_shift = (other_negatable_type::radix_split - radix_split) - 1;

      u_superior = detail::right_shift_helper(u_superior, total_right_shift);

      unsigned_small_type u_round = static_cast<unsigned_small_type>(u_superior);

      // Round the result of the construction.
      const boost::int_fast8_t rounding_result = binary_round(u_round);

      // Add or subtract the result of the rounding (-1, 0, or +1).
      if     (rounding_result == INT8_C(+1)) { ++u_round; }
      else if(rounding_result == INT8_C(-1)) { --u_round; }

      data = ((!is_neg) ? value_type(u_round) : -value_type(u_round));
    }

    // Here is the mixed-math class constructor for case 4).
    // There is more range and more resolution in the other type.
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution,
             typename std::enable_if<   (   ( OtherIntegralRange        >   IntegralRange)
                                         && (-OtherFractionalResolution >  -FractionalResolution))
                                     || (   ( OtherIntegralRange        >=  IntegralRange)
                                         && (-OtherFractionalResolution >  -FractionalResolution))
                                     || (   ( OtherIntegralRange        >   IntegralRange)
                                         && (-OtherFractionalResolution >= -FractionalResolution))>::type* = nullptr>
    negatable(const negatable<OtherIntegralRange,
                              OtherFractionalResolution,
                              RoundMode,
                              OverflowMode>& other) : data()
    {
      typedef negatable<OtherIntegralRange,
                        OtherFractionalResolution,
                        RoundMode,
                        OverflowMode> other_negatable_type;

      typedef typename other_negatable_type::unsigned_small_type superior_unsigned_small_type;

      const bool is_neg = (other.data < 0);

      superior_unsigned_small_type u_superior((!is_neg) ? superior_unsigned_small_type(+other.data)
                                                        : superior_unsigned_small_type(-other.data));

      BOOST_CONSTEXPR_OR_CONST int total_right_shift = (other_negatable_type::radix_split - radix_split) - 1;

      u_superior = detail::right_shift_helper(u_superior, total_right_shift);

      unsigned_small_type u_round = static_cast<unsigned_small_type>(u_superior);

      // Round the result of the construction.
      const boost::int_fast8_t rounding_result = binary_round(u_round);

      u_round = (u_round & unsigned_small_mask());

      // Add or subtract the result of the rounding (-1, 0, or +1).
      if     (rounding_result == INT8_C(+1)) { ++u_round; }
      else if(rounding_result == INT8_C(-1)) { --u_round; }

      data = ((!is_neg) ? value_type(u_round) : -value_type(u_round));
    }

    /*! Destructor.\n
        It has trivial complexity because the negatable class does not do any allocation
        or complex operations (if any) that are not already handled by the underlying @c value_type.
    */
    ~negatable() { }

    /*! Equality operators.\n
    */
    // This is the standard equality operator.
    negatable& operator=(const negatable& other)
    {
      if(this != (&other))
      {
        data = other.data;
      }

      return *this;
    }

    /*! Equality operator of @c *this with another negatable type
    having @b different range and/or resolution parameters than @c *this.
    */
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    negatable& operator=(const negatable<OtherIntegralRange,
                                         OtherFractionalResolution,
                                         RoundMode,
                                         OverflowMode>& other)
    {
      // Use a relatively lazy method that creates an intermediate temporary object.
      // The temporary object is subsequently used to initialize the data field of *this.

      const negatable tmp(other);

      data = tmp.data;

      return *this;
    }

    //! Equality operators for built-in integral types.
    negatable& operator=(const char& n)               { make_from_signed_integral_type(n); return *this; }
    negatable& operator=(const short& n)              { make_from_signed_integral_type(n); return *this; }
    negatable& operator=(const int& n)                { make_from_signed_integral_type(n); return *this; }
    negatable& operator=(const long& n)               { make_from_signed_integral_type(n); return *this; }
    negatable& operator=(const long long& n)          { make_from_signed_integral_type(n); return *this; }

    negatable& operator=(const unsigned char& u)      { make_from_unsigned_integral_type(u); return *this; }
    negatable& operator=(const unsigned short& u)     { make_from_unsigned_integral_type(u); return *this; }
    negatable& operator=(const unsigned int& u)       { make_from_unsigned_integral_type(u); return *this; }
    negatable& operator=(const unsigned long& u)      { make_from_unsigned_integral_type(u); return *this; }
    negatable& operator=(const unsigned long long& u) { make_from_unsigned_integral_type(u); return *this; }

    //! Equality operators for built-in floating-point types.
    template<typename FloatingPointType,
             typename = typename std::enable_if<   std::is_floating_point<FloatingPointType>::value
                                                || std::is_same<FloatingPointType, float_type>::value>::type>
    negatable& operator=(const FloatingPointType& f)
    {
      make_from_floating_point_type(f);

      return *this;
    }

    //! Unary pre-increment and post-increment operators.
    negatable& operator++()   { data += value_type(unsigned_small_type(1) << radix_split); return *this; }
    negatable& operator--()   { data -= value_type(unsigned_small_type(1) << radix_split); return *this; }

    negatable operator++(int) { const negatable tmp(*this); data += value_type(unsigned_small_type(1) << radix_split); return tmp; }
    negatable operator--(int) { const negatable tmp(*this); data -= value_type(unsigned_small_type(1) << radix_split); return tmp; }

    //! Unary operator add of negatable with negatable.
    negatable& operator+=(const negatable& v)
    {
      data += v.data;

      const bool is_neg = (data < 0);

      unsigned_small_type u = (!(is_neg) ? unsigned_small_type(data) : unsigned_small_type (-data));

      u = (u & unsigned_small_mask());

      data = value_type(!(is_neg) ? value_type(u) : -value_type(u));

      return *this;
    }

    //! Unary operator subtract of negatable with negatable.
    negatable& operator-=(const negatable& v)
    {
      data -= v.data;

      const bool is_neg = (data < 0);

      unsigned_small_type u = (!(is_neg) ? unsigned_small_type(data) : unsigned_small_type(-data));

      u = (u & unsigned_small_mask());

      data = value_type(!(is_neg) ? value_type(u) : -value_type(u));

      return *this;
    }

    //! Unary operator multiply of negatable * negatable.
    negatable& operator*=(const negatable& v)
    {
      const bool u_is_neg      = (  data < 0);
      const bool v_is_neg      = (v.data < 0);
      const bool result_is_neg = (u_is_neg != v_is_neg);

      // Multiplication will be carried out using unsigned integers.

      // Multiplication uses a relatively lazy method.
      // The result is first placed in a variable of
      // type @c unsigned_large_type (which is twice as wide
      // as @c unsigned_small_type).

      // The result is multiplied as (u * v) in the
      // unsigned_large_type and subsequently scaled down
      // (potentially with rounding) to the size of the
      // fixed-point data field.

      // Hereby, we scale the result of the multiplication to a larger
      // internal size so that the multiplication operation is
      // straightforward and simple. But this is potentially costly
      // for higher digit counts.

      unsigned_large_type result((!u_is_neg) ? unsigned_large_type(data) : unsigned_large_type(-data));

      result *= unsigned_large_type((!v_is_neg) ? unsigned_large_type(v.data) : unsigned_large_type(-v.data));

      // Scale the result of the multiplication to fit once again
      // in the fixed-point data field.

      // Here, we use 1 extra binary digit for rounding.
      // The extra rounding digit fits in @c unsigned_small_type
      // because the value_type (even though just as wide as
      // @c unsigned_small_type) reserves one bit for the sign.

      const int total_right_shift = radix_split - 1;

      unsigned_small_type u_round(static_cast<unsigned_small_type>(detail::right_shift_helper(result, total_right_shift)));

      // Round the result of the multiplication.
      const boost::int_fast8_t rounding_result = binary_round(u_round);

      // Add or subtract the result of the rounding (-1, 0, or +1).
      if     (rounding_result == INT8_C(+1)) { ++u_round; }
      else if(rounding_result == INT8_C(-1)) { --u_round; }

      u_round = (u_round & unsigned_small_mask());

      // Load the fixed-point result (and account for potentially signed values).
      data = value_type((!result_is_neg) ? value_type(u_round) : -value_type(u_round));

      return *this;
    }

    //! Unary operator divide of negatable with negatable.
    negatable& operator/=(const negatable& v)
    {
      if(v.data == 0)
      {
        data = value_type((std::numeric_limits<value_type>::max)());
      }
      else
      {
        const bool u_is_neg      = (  data < 0);
        const bool v_is_neg      = (v.data < 0);
        const bool result_is_neg = (u_is_neg != v_is_neg);

        // Division will be carried out using unsigned integers.

        // Division uses a relatively lazy method.
        // The result is first placed in a variable of
        // type unsigned_large_type (which is twice as wide
        // as unsigned_small_type).

        // The result is divided as (u / v) in the
        // unsigned_large_type and subsequently scaled down
        // (potentially with rounding) to the size of the
        // fixed-point data field.

        // Hereby, we scale the result of the division to a larger
        // internal size so that the division operation is
        // straightforward and simple. But this is potentially costly
        // for higher digit counts.

        unsigned_large_type result((!u_is_neg) ? unsigned_large_type(data) : unsigned_large_type(-data));

        // Here, we use 1 extra binary digit for rounding.
        // The extra rounding digit fits in unsigned_small_type
        // because the value_type (even though just as wide as
        // unsigned_small_type) reserves one bit for the sign.

        result = (result << (radix_split + 1));

        result /= unsigned_large_type((!v_is_neg) ? unsigned_large_type(v.data) : unsigned_large_type(-v.data));

        unsigned_small_type u_round = static_cast<unsigned_small_type>(result);

        // Round the result of the division.
        const boost::int_fast8_t rounding_result = binary_round(u_round);

        // Add or subtract the result of the rounding (-1, 0, or +1).
        if     (rounding_result == INT8_C(+1)) { ++u_round; }
        else if(rounding_result == INT8_C(-1)) { --u_round; }

        u_round = (u_round & unsigned_small_mask());

        // Load the fixed-point result (and account for potentially signed values).
        data = value_type((!result_is_neg) ? value_type(u_round) : -value_type(u_round));
      }

      return *this;
    }

    //! Unary operators add, sub, and mul of negatable with an arithmetic built-in type.
    template<typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type> negatable& operator+=(const ArithmeticType& v) { return (*this) += negatable(v); }
    template<typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type> negatable& operator-=(const ArithmeticType& v) { return (*this) -= negatable(v); }
    template<typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type> negatable& operator*=(const ArithmeticType& v) { return (*this) *= negatable(v); }

    //! Unary operator div of negatable with a floating-point built-in type.
    negatable& operator/=(const float       v) { return (*this) /= negatable(v); }
    negatable& operator/=(const double      v) { return (*this) /= negatable(v); }
    negatable& operator/=(const long double v) { return (*this) /= negatable(v); }

    //! Unary operator div of negatable with long and long long.
    negatable& operator/=(const long      v) { return (*this) /= negatable(v); }
    negatable& operator/=(const long long v) { return (*this) /= negatable(v); }

    //! Unary operator div of negatable with unsigned long and unsigned long long.
    negatable& operator/=(const unsigned long      v) { return (*this) /= negatable(v); }
    negatable& operator/=(const unsigned long long v) { return (*this) /= negatable(v); }

    //! Optimized unary operator div of negatable with @c int.
    negatable& operator/=(const int v)
    {
      if(v == 0)
      {
        data = value_type((std::numeric_limits<value_type>::max)());
      }
      else
      {
        const bool u_is_neg      = (data < 0);
        const bool v_is_neg      = (v    < 0);
        const bool result_is_neg = (u_is_neg != v_is_neg);

        // Division will be carried out using unsigned integers.

        unsigned_small_type result((!u_is_neg) ? unsigned_small_type(data) : unsigned_small_type(-data));

        // Here, we use 1 extra binary digit for rounding.
        // The extra rounding digit fits in unsigned_small_type
        // because the value_type (even though just as wide as
        // unsigned_small_type) reserves one bit for the sign.

        result = (result << 1);

        result /= unsigned((!v_is_neg) ? unsigned(v) : unsigned(-v));

        unsigned_small_type u_round = static_cast<unsigned_small_type>(result);

        // Round the result of the division.
        const boost::int_fast8_t rounding_result = binary_round(u_round);

        // Add or subtract the result of the rounding (-1, 0, or +1).
        if     (rounding_result == INT8_C(+1)) { ++u_round; }
        else if(rounding_result == INT8_C(-1)) { --u_round; }

        u_round = (u_round & unsigned_small_mask());

        // Load the fixed-point result (and account for potentially signed values).
        data = value_type((!result_is_neg) ? value_type(u_round) : -value_type(u_round));
      }

      return (*this);
    }

    //! Optimized unary operator div of negatable with @c unsigned @c int.
    negatable& operator/=(const unsigned int v)
    {
      if(v == 0U)
      {
        data = value_type((std::numeric_limits<value_type>::max)());
      }
      else
      {
        const bool is_neg = (data < 0);

        // Division will be carried out using unsigned integers.

        unsigned_small_type result((!is_neg) ? unsigned_small_type(data) : unsigned_small_type(-data));

        // Here, we use 1 extra binary digit for rounding.
        // The extra rounding digit fits in unsigned_small_type
        // because the value_type (even though just as wide as
        // unsigned_small_type) reserves one bit for the sign.

        result = (result << 1);

        result /= v;

        unsigned_small_type u_round = static_cast<unsigned_small_type>(result);

        // Round the result of the division.
        const boost::int_fast8_t rounding_result = binary_round(u_round);

        // Add or subtract the result of the rounding (-1, 0, or +1).
        if     (rounding_result == INT8_C(+1)) { ++u_round; }
        else if(rounding_result == INT8_C(-1)) { --u_round; }

        u_round = (u_round & unsigned_small_mask());

        // Load the fixed-point result (and account for potentially signed values).
        data = value_type((!is_neg) ? value_type(u_round) : -value_type(u_round));
      }

      return (*this);
    }

    //! Optimized unary operator div of negatable with @c signed and @c unsigned @c char and @c short.
    negatable& operator/=(const char           v) { return operator/=(static_cast<int>(v)); }
    negatable& operator/=(const short          v) { return operator/=(static_cast<int>(v)); }
    negatable& operator/=(const unsigned char  v) { return operator/=(static_cast<unsigned int>(v)); }
    negatable& operator/=(const unsigned short v) { return operator/=(static_cast<unsigned int>(v)); }

    /*! Cast operators for built-in signed and unsigned integral types.\n

     Note: Cast from @c negatable to a built-in integral type truncates
     the fractional part regardless of the rounding mode.\n
     This is consistent with the conversion from built-in floating-point types
     to built-in integral types. See also ISO/IEC 14882:2011 paragraph 4.9.1.
    */

    operator char     () const { return static_cast<char>     ((!(data < 0)) ? static_cast<char>     (unsigned_small_type(data) >> radix_split) : -static_cast<char>     (unsigned_small_type(-data) >> radix_split)); }
    operator short    () const { return static_cast<short>    ((!(data < 0)) ? static_cast<short>    (unsigned_small_type(data) >> radix_split) : -static_cast<short>    (unsigned_small_type(-data) >> radix_split)); }
    operator int      () const { return static_cast<int>      ((!(data < 0)) ? static_cast<int>      (unsigned_small_type(data) >> radix_split) : -static_cast<int>      (unsigned_small_type(-data) >> radix_split)); }
    operator long     () const { return static_cast<long>     ((!(data < 0)) ? static_cast<long>     (unsigned_small_type(data) >> radix_split) : -static_cast<long>     (unsigned_small_type(-data) >> radix_split)); }
    operator long long() const { return static_cast<long long>((!(data < 0)) ? static_cast<long long>(unsigned_small_type(data) >> radix_split) : -static_cast<long long>(unsigned_small_type(-data) >> radix_split)); }

    operator unsigned char     () const { return static_cast<unsigned char>     (unsigned_small_type(data) >> radix_split); }
    operator unsigned short    () const { return static_cast<unsigned short>    (unsigned_small_type(data) >> radix_split); }
    operator unsigned int      () const { return static_cast<unsigned int>      (unsigned_small_type(data) >> radix_split); }
    operator unsigned long     () const { return static_cast<unsigned long>     (unsigned_small_type(data) >> radix_split); }
    operator unsigned long long() const { return static_cast<unsigned long long>(unsigned_small_type(data) >> radix_split); }

    // Cast operators for built-in floating-point types.
    operator float      () const { return convert_to_floating_point_type<float>      (); }
    operator double     () const { return convert_to_floating_point_type<double>     (); }
    operator long double() const { return convert_to_floating_point_type<long double>(); }

    #if !defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM)

      // Supply a decimal string representation get-function.
      std::string to_string() const
      {
        const float_type x = convert_to_floating_point_type<float_type>();

        std::stringstream ss;

        ss << std::setprecision(std::numeric_limits<float_type>::digits10)
           << std::fixed
           << x;

        return ss.str();
      }

      // Supply a bit-field string representation get-function.
      // This function is used primarily for debugging and testing purposes.
      std::string bit_pattern() const
      {
        // Acquire the fixed-point data field and convert it to an unsigned type.
        unsigned_small_type number(static_cast<unsigned_small_type>(this->data));
        unsigned_small_type mask  (1U);

        // Allocate a string of the proper length with all of the
        // characters initialized to '0'. Use the total number
        // of binary digits in the negatable_type, which is
        // all_bits = (range - resolution) + 1.
        std::string answer(all_bits, char('0'));

        // Extract all of the bits from *this and place them in the string.
        // Use the reverse order to simplify the bit-extraction algorithm.
        // Reverse the bit pattern after the bit extraction.

        // Use forward iteration as suggested by Nikhar and reverse
        // after the bit extraction.

        // TBD: Why do reverse iteration and bit-test not work
        // for certain GCCs, but do work for MSVC?

        std::for_each(answer.begin(),
                      answer.end(),
                      [&mask, &number](char& c)
                      {
                        const bool bit_is_set = (unsigned_small_type(mask & number) != 0U);

                        if(bit_is_set)
                        {
                          c = char('1');
                        }

                        mask = (mask << 1);
                      });

        std::reverse(answer.begin(), answer.end());

        return answer;
      }

    #endif // !BOOST_FIXED_POINT_DISABLE_IOSTREAM

  private:
    value_type data;

    typedef typename detail::integer_type_helper<negatable::all_bits * 1>::exact_unsigned_type unsigned_small_type;
    typedef typename detail::integer_type_helper<negatable::all_bits * 2>::exact_unsigned_type unsigned_large_type;

    static const unsigned_small_type& unsigned_small_mask() BOOST_NOEXCEPT
    {
      initialization_helper.force_premain_init_of_static_constants();

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
    negatable(const nothing&,
              const IntegralType& n) : data(static_cast<value_type>(n)) { }

    template<typename UnsignedIntegralType>
    void make_from_unsigned_integral_type(const UnsignedIntegralType& u)
    {
      data = (value_type(unsigned_small_type(unsigned_small_type(u) << radix_split) & unsigned_small_mask()));
    }

    template<typename SignedIntegralType>
    void make_from_signed_integral_type(const SignedIntegralType& n)
    {
      data = ((!(n < 0)) ? +value_type(unsigned_small_type(unsigned_small_type(+n) << radix_split) & unsigned_small_mask())
                         : -value_type(unsigned_small_type(unsigned_small_type(-n) << radix_split) & unsigned_small_mask()));
    }

    template<typename FloatingPointType>
    void make_from_floating_point_type(const FloatingPointType& f)
    {
      // Define a local_unsigned_small_type.
      // This is an unsigned integral type that is guaranteed
      // to hold the larger of:
      // * the number of digits in FloatingPointType plus 1 extra digit
      // * the number of digits in unsigned_small_type.

      BOOST_CONSTEXPR_OR_CONST int fp_digits_plus_one = std::numeric_limits<FloatingPointType>::digits + 1;

      typedef
      typename detail::integer_type_helper<
        unsigned((fp_digits_plus_one > std::numeric_limits<unsigned_small_type>::digits)
                    ? fp_digits_plus_one
                    : std::numeric_limits<unsigned_small_type>::digits)>::exact_unsigned_type
      local_unsigned_small_type;

      const bool is_neg = (f < FloatingPointType(0));

      using std::frexp;
      using std::ldexp;

      // Extract the mantissa and exponent.
      int exp2;
      const FloatingPointType fp(frexp((!is_neg) ? f : -f, &exp2));

      // Here we scale the mantissa to an unsigned integer value
      // that is large enough to contain all the binary digits
      // of the FloatingPointType representation.
      local_unsigned_small_type u;

      const FloatingPointType fp_shifted = ldexp(fp, fp_digits_plus_one);

      detail::conversion_helper<local_unsigned_small_type,
                                FloatingPointType>::convert_floating_point_to_unsigned_integer(fp_shifted, u);

      // Select the scale factor for the conversion to the fixed-point type.

      // Here, we use 1 extra binary digit for rounding.
      // The extra rounding digit fits in unsigned_small_type
      // because the value_type (even though just as wide as
      // unsigned_small_type) reserves one bit for the sign.

      const int total_left_shift =   (radix_split + exp2)
                                   - (std::numeric_limits<FloatingPointType>::digits);

      const local_unsigned_small_type u_round_local(detail::left_shift_helper(u, total_left_shift));

      unsigned_small_type u_round = static_cast<unsigned_small_type>(u_round_local);

      // Round the result of the construction from FloatingPointType.
      const boost::int_fast8_t rounding_result = binary_round(u_round);

      // Add or subtract the result of the rounding (-1, 0, or +1).
      if     (rounding_result == INT8_C(+1)) { ++u_round; }
      else if(rounding_result == INT8_C(-1)) { --u_round; }

      u_round = (u_round & unsigned_small_mask());

      // Load the fixed-point result (and account for potentially signed values).
      data = value_type((!is_neg) ? value_type(u_round) : -value_type(u_round));
    }

    /*! Convert the fixed_point value to FloatingPointType result.
        The fixed_point value is converted with brute force,
        using one bit at a time.

        TBD: The conversion uses brute force with an inefficient loop.
        Can (should) this mechanism be optimized?

        \tparam FloatingPointType Type for the result of conversion.
        Usually a built-in type, @c float, @c double, but may be a multiprecision type.

       TBD Is this correct?
    */
    template<typename FloatingPointType>
    FloatingPointType convert_to_floating_point_type() const
    {
      const bool is_neg = (data < static_cast<value_type>(0));

      FloatingPointType f(0);
 
      {
        using std::ldexp;

        unsigned_small_type u((!is_neg) ? data : -data);

        BOOST_CONSTEXPR_OR_CONST int digits_unsigned_small_type = std::numeric_limits<unsigned_small_type>::digits;
        BOOST_CONSTEXPR_OR_CONST int digits_floating_point_type = std::numeric_limits<FloatingPointType>::digits;

        BOOST_CONSTEXPR_OR_CONST int max_digits = ((digits_unsigned_small_type >= digits_floating_point_type)
                                                     ? digits_unsigned_small_type
                                                     : digits_floating_point_type);

        boost::int_fast16_t digit_index;

        for(digit_index = INT16_C(0); ((digit_index < max_digits) && (u != 0)); ++digit_index)
        {
          const boost::uint_fast8_t bit_test_value = static_cast<boost::uint_fast8_t>(u);

          u = (u >> 1);

          if(boost::uint_fast8_t(bit_test_value & UINT8_C(1)) != UINT8_C(0))
          {
            f += ldexp(FloatingPointType(1), digit_index);
          }
        }

        // Rounding needs to be done if the fixed_point type
        // has more digits than the target FloatingPointType
        // of the result.

        if(digit_index < boost::int_fast16_t(std::numeric_limits<unsigned_small_type>::digits))
        {
          // Check if the result of the conversion needs to be rounded.
          const boost::int_fast8_t rounding_result = binary_round(u);

          // Add or subtract the result of the rounding (-1, 0, or +1).
          f += FloatingPointType(rounding_result);
        }

        f = ldexp(f, -int(radix_split));
      }

      return ((!is_neg) ? f : -f);
    }

    template<typename LocalRoundMode = RoundMode>
    static boost::int_fast8_t
      binary_round(unsigned_small_type& u_round,
                   typename std::enable_if<std::is_same<LocalRoundMode, round::fastest>::value>::type* = nullptr)
    {
      /*! Here, @c u_round contains the value to be rounded whereby
       this value is left-shifted one binary digit larger than
       the final result will be.

       Perform the rounding algorithm for @c round::fastest.
       For @c round::fastest, there is simply no rounding at all;
       the value is truncated.
     */

      u_round = (u_round >> 1);

      return INT8_C(0);
    }

   /*! Perform the rounding algorithm for @c round::nearest_even.
       For @c round::nearest_even, the value is rounded to larger
       absolute value when both 1/2-ULP as well as 1-ULP are 1,
       representing round odd 1-ULP to higher value.
    \tparam LocalRoundMode Rounding mode for this operation.
     \param u_round contains the value to be rounded whereby
       this value is left-shifted one binary digit larger than
       the final result will be.
    */
    template<typename LocalRoundMode = RoundMode>
    static boost::int_fast8_t
      binary_round(unsigned_small_type& u_round,
                   typename std::enable_if<std::is_same<LocalRoundMode, round::nearest_even>::value>::type* = nullptr)
    {
   
      const bool round_up =   ((boost::uint_fast8_t(u_round & UINT8_C(1)) == UINT8_C(1))
                            && (boost::uint_fast8_t(u_round & UINT8_C(2)) == UINT8_C(2)));

      u_round = (u_round >> 1);

      return (round_up ? INT8_C(1) : INT8_C(0));
    }

    static const unsigned_small_type& radix_split_value() BOOST_NOEXCEPT
    {
      initialization_helper.force_premain_init_of_static_constants();

      static const unsigned_small_type the_radix_split_value(unsigned_small_type(1U) << radix_split);

      return the_radix_split_value;
    }

    /*! Compute machine epsilon (at compile time) for 
        @c std::numeric_limits<>::epsilon() function.
        Epsilon is defined as the smallest number that,
        when added to one, yields a result different from one.
        By this definition, epsilon equals the value of the unit
        in the last place relative to 1 (i.e. b^{ -(p - 1) }),
        where p is the total number of significand bits including
        any implicit bit. For negatable<7, -8>, for example,
        we have p = 8 and b = 2, so the value of epsilon is:
        2^{-(8 - 1)} = 2^{-7} = 0.0078125.
        \sa http://en.cppreference.com/w/cpp/types/numeric_limits/epsilon
    */
    static const negatable& epsilon_maker() BOOST_NOEXCEPT
    {
      initialization_helper.force_premain_init_of_static_constants();

      BOOST_CONSTEXPR_OR_CONST int total_right_shift = -(resolution + 1);

      static const negatable the_epsilon(nothing(), radix_split_value() >> total_right_shift);

      return the_epsilon;
    }

    /*! Compute (at compile time) the minimum value that the type can represent.\n
        Used to define function @c std::numeric_limits<>::min().
    */
    static const negatable& value_min() BOOST_NOEXCEPT
    {
      initialization_helper.force_premain_init_of_static_constants();

      static const negatable the_value_min(nothing(), 1U);

      return the_value_min;
    }

    /*! Compute (at compile time) the maximum value that the type can represent.\n
        Used to define function @c std::numeric_limits<>::max()
        and, when negated, @c std::numeric_limits<>::lowest().
    */
    static const negatable& value_max() BOOST_NOEXCEPT
    {
      initialization_helper.force_premain_init_of_static_constants();

      static const negatable the_value_max(nothing(), unsigned_small_mask());

      return the_value_max;
    }

    friend class std::numeric_limits<negatable>;

    friend struct initializer;

    struct initializer
    {
      initializer()
      {
        static_cast<void>(negatable::radix_split_value());
        static_cast<void>(negatable::unsigned_small_mask());
        static_cast<void>(negatable::value_min());
        static_cast<void>(negatable::value_max());
        static_cast<void>(negatable::epsilon_maker());
      }

      void force_premain_init_of_static_constants() { }
    };

    static initializer initialization_helper;

    /*! @c std::ostream output @c operator\<\< \n
        Send a fixed-point number to the output stream by first
        expressing the fixed-point number as a floating-point number.\n

        \note Macro BOOST_FIXED_POINT_DISABLE_IOSTREAM can be defined to 
        disable all I/O streaming and the inclusion of associated standard
        library headers. This is intended to eliminate I/O stream
        overhead in particular for bare-metal microcontroller projects.
    */

    #if !defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM)

      template<typename char_type,
               typename traits_type>
      friend inline std::basic_ostream<char_type,
                                       traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& out,
                                                                const negatable& x)
      {
        // Send a fixed-point number to the output stream.
        // Express the fixed-point number as a floating-point number.

        std::basic_ostringstream<char_type, traits_type> ostr;

        ostr.flags    (out.flags());
        ostr.imbue    (out.getloc());
        ostr.precision(out.precision());

        static_cast<void>(ostr << x.convert_to_floating_point_type<float_type>());

        return (out << ostr.str());
      } // operator<<

      /*! @c std::istream input @c operator>>
          Receive a floating-point number from the input stream.
          Subsequently make a fixed-point object from it.
      */

      template<typename char_type,
               typename traits_type>
      friend inline std::basic_istream<char_type,
                                       traits_type>& operator>>(std::basic_istream<char_type, traits_type>& in,
                                                                negatable& x)
      {
        float_type v;

        // Receive a floating-point number from the input stream.
        static_cast<void>(in >> v);

        // Subsequently make a fixed-point object from it.
        x.make_from_floating_point_type(v);

        return in;
      }

    #endif // !BOOST_FIXED_POINT_DISABLE_IOSTREAM

    //! \cond DETAIL
    // Do not document the implementation details unless macro DETAIL is defined.

    //! Implementations of global unary plus and minus.
    friend inline negatable operator+(const negatable& self) { return negatable(self); }
    friend inline negatable operator-(const negatable& self) { negatable tmp(self); tmp.data = -tmp.data; return tmp; }

    //! Implementations of global binary add, sub, mul, div of [lhs(negatable)] operator [rhs(negatable)].
    friend inline negatable operator+(const negatable& u, const negatable& v) { return negatable(u) += v; }
    friend inline negatable operator-(const negatable& u, const negatable& v) { return negatable(u) -= v; }
    friend inline negatable operator*(const negatable& u, const negatable& v) { return negatable(u) *= v; }
    friend inline negatable operator/(const negatable& u, const negatable& v) { return negatable(u) /= v; }

    //! Implementations of global binary add, sub, mul, div of [lhs(negatable)] operator [rhs(arithmetic_type)].
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator+(const negatable& u, const ArithmeticType v) { return negatable(u) += v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator-(const negatable& u, const ArithmeticType v) { return negatable(u) -= v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator*(const negatable& u, const ArithmeticType v) { return negatable(u) *= v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator/(const negatable& u, const ArithmeticType v) { return negatable(u) /= v; }

    //! Implementations of global binary add, sub, mul, div of [lhs(arithmetic_type)] operator [rhs(negatable)].
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator+(const ArithmeticType u, const negatable& v) { return negatable(u) += v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator-(const ArithmeticType u, const negatable& v) { return negatable(u) -= v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator*(const ArithmeticType u, const negatable& v) { return negatable(u) *= v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator/(const ArithmeticType u, const negatable& v) { return negatable(u) /= v; }

    //! Implementations of global binary add, sub, mul, div of [lhs(negatable)] operator [rhs(other_negatable)].
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline negatable<((-FractionalResolution > -OtherFractionalResolution) ? IntegralRange        : OtherIntegralRange),
                            ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                            RoundMode,
                            OverflowMode>
    operator+(const negatable& u, const negatable<OtherIntegralRange,
                                                  OtherFractionalResolution,
                                                  RoundMode,
                                                  OverflowMode>& v)
    {
      typedef negatable<((-FractionalResolution > -OtherFractionalResolution) ? IntegralRange        : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                        RoundMode,
                        OverflowMode> higher_resolution_fixed_point_type;

      return higher_resolution_fixed_point_type(u) += higher_resolution_fixed_point_type(v);
    }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline negatable<((-FractionalResolution > -OtherFractionalResolution) ? IntegralRange        : OtherIntegralRange),
                            ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                            RoundMode,
                            OverflowMode>
    operator-(const negatable& u, const negatable<OtherIntegralRange,
                                                  OtherFractionalResolution,
                                                  RoundMode,
                                                  OverflowMode>& v)
    {
      typedef negatable<((-FractionalResolution > -OtherFractionalResolution) ? IntegralRange        : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                        RoundMode,
                        OverflowMode> higher_resolution_fixed_point_type;

      return higher_resolution_fixed_point_type(u) -= higher_resolution_fixed_point_type(v);
    }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline negatable<((-FractionalResolution > -OtherFractionalResolution) ? IntegralRange        : OtherIntegralRange),
                            ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                            RoundMode,
                            OverflowMode>
    operator*(const negatable& u, const negatable<OtherIntegralRange,
                                                  OtherFractionalResolution,
                                                  RoundMode,
                                                  OverflowMode>& v)
    {
      typedef negatable<((-FractionalResolution > -OtherFractionalResolution) ? IntegralRange        : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                        RoundMode,
                        OverflowMode> higher_resolution_fixed_point_type;

      return higher_resolution_fixed_point_type(u) *= higher_resolution_fixed_point_type(v);
    }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline negatable<((-FractionalResolution > -OtherFractionalResolution) ? IntegralRange        : OtherIntegralRange),
                            ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                            RoundMode,
                            OverflowMode>
    operator/(const negatable& u, const negatable<OtherIntegralRange,
                                                  OtherFractionalResolution,
                                                  RoundMode,
                                                  OverflowMode>& v)
    {
      typedef negatable<((-FractionalResolution > -OtherFractionalResolution) ? IntegralRange        : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                        RoundMode,
                        OverflowMode> higher_resolution_fixed_point_type;

      return higher_resolution_fixed_point_type(u) /= higher_resolution_fixed_point_type(v);
    }

    //! Implementations of global equality.
    friend inline bool operator==(const negatable& u, const negatable& v) { return (u.data == v.data); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator==(const negatable& u, const ArithmeticType& v) { return (u.data == negatable(v).data); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator==(const ArithmeticType& u, const negatable& v) { return (negatable(u).data == v.data); }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline bool operator==(const negatable& u, const negatable<OtherIntegralRange,
                                                                      OtherFractionalResolution,
                                                                      RoundMode,
                                                                      OverflowMode>& v)
    {
      typedef negatable<(( IntegralRange        >  OtherIntegralRange)        ? IntegralRange        : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                        RoundMode,
                        OverflowMode> supra_fixed_point_type;

      return (supra_fixed_point_type(u) == supra_fixed_point_type(v));
    }

    //! Implementations of global inequality.
    friend inline bool operator!=(const negatable& u, const negatable& v) { return (u.data != v.data); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator!=(const negatable& u, const ArithmeticType& v) { return (u.data != negatable(v).data); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator!=(const ArithmeticType& u, const negatable& v) { return (negatable(u).data != v.data); }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline bool operator!=(const negatable& u, const negatable<OtherIntegralRange,
                                                                      OtherFractionalResolution,
                                                                      RoundMode,
                                                                      OverflowMode>& v)
    {
      typedef negatable<(( IntegralRange        >  OtherIntegralRange)        ? IntegralRange        : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                        RoundMode,
                        OverflowMode> supra_fixed_point_type;

      return (supra_fixed_point_type(u) != supra_fixed_point_type(v));
    }

    //! Implementations of global operators >, <, >=, <=.
    friend inline bool operator> (const negatable& u, const negatable& v) { return (u.data >  v.data); }
    friend inline bool operator< (const negatable& u, const negatable& v) { return (u.data <  v.data); }
    friend inline bool operator>=(const negatable& u, const negatable& v) { return (u.data >= v.data); }
    friend inline bool operator<=(const negatable& u, const negatable& v) { return (u.data <= v.data); }

    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator> (const negatable& u, const ArithmeticType& v) { return (u.data >  negatable(v).data); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator< (const negatable& u, const ArithmeticType& v) { return (u.data <  negatable(v).data); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator>=(const negatable& u, const ArithmeticType& v) { return (u.data >= negatable(v).data); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator<=(const negatable& u, const ArithmeticType& v) { return (u.data <= negatable(v).data); }

    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator> (const ArithmeticType& u, const negatable& v) { return (negatable(u).data >  v.data); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator< (const ArithmeticType& u, const negatable& v) { return (negatable(u).data <  v.data); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator>=(const ArithmeticType& u, const negatable& v) { return (negatable(u).data >= v.data); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline bool operator<=(const ArithmeticType& u, const negatable& v) { return (negatable(u).data <= v.data); }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline bool operator>(const negatable& u, const negatable<OtherIntegralRange,
                                                                     OtherFractionalResolution,
                                                                     RoundMode,
                                                                     OverflowMode>& v)
    {
      typedef negatable<(( IntegralRange        >  OtherIntegralRange)        ? IntegralRange        : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                        RoundMode,
                        OverflowMode> supra_fixed_point_type;

      return (supra_fixed_point_type(u) > supra_fixed_point_type(v));
    }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline bool operator<(const negatable& u, const negatable<OtherIntegralRange,
                                                                     OtherFractionalResolution,
                                                                     RoundMode,
                                                                     OverflowMode>& v)
    {
      typedef negatable<(( IntegralRange        >  OtherIntegralRange)        ? IntegralRange        : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                        RoundMode,
                        OverflowMode> supra_fixed_point_type;

      return (supra_fixed_point_type(u) < supra_fixed_point_type(v));
    }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline bool operator>=(const negatable& u, const negatable<OtherIntegralRange,
                                                                      OtherFractionalResolution,
                                                                      RoundMode,
                                                                      OverflowMode>& v)
    {
      typedef negatable<(( IntegralRange        >  OtherIntegralRange)        ? IntegralRange        : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                        RoundMode,
                        OverflowMode> supra_fixed_point_type;

      return (supra_fixed_point_type(u) >= supra_fixed_point_type(v));
    }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline bool operator<=(const negatable& u, const negatable<OtherIntegralRange,
                                                                      OtherFractionalResolution,
                                                                      RoundMode,
                                                                      OverflowMode>& v)
    {
      typedef negatable<(( IntegralRange        >  OtherIntegralRange)        ? IntegralRange        : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution),
                        RoundMode,
                        OverflowMode> supra_fixed_point_type;

      return (supra_fixed_point_type(u) <= supra_fixed_point_type(v));
    }
    //! \endcond // DETAIL

    //! @c std::abs function.
    friend inline negatable  abs(negatable x) { return ((x.data < 0) ? -x : x); }

    //! @c std::fabs function (identical to abs and provided for completeness).
    friend inline negatable fabs(negatable x) { return ((x.data < 0) ? -x : x); }

    // TBD implement C++11 copysign and other floating-point manipulation functions.

    /*! @c std::frexp function \<cmath\> implementation for negatable types.\n

    TBD examples.\n
        \param x Fixed-point value to decompose into fraction and integral power of 2.
        \param exp2 Pointer to integer value to store the exponent to. 
        \returns fraction part as a @c negatable type.
    */
    friend inline negatable frexp(negatable x, int* exp2)
    {
      if(x.data == 0)
      {
        *exp2 = 0;

        return negatable(0);
      }
      else
      {
        const bool is_neg = (x.data < 0);

        // Extract the unsigned representation of the data field.
        unsigned_small_type result((!is_neg) ? unsigned_small_type(x.data) : unsigned_small_type(-x.data));

        // Use a binary-halving mechanism to obtain the most significant bit.
        const int msb = static_cast<int>(detail::msb_meta_helper_nonconstant<unsigned_small_type>::hi_bit(result));

        // Evaluate the necessary amount of right-shift.
        const int total_right_shift = (msb - radix_split) + 1;

        // Compute the (still unsigned) data field and the exponent.
        result = detail::right_shift_helper(result, total_right_shift);

        *exp2 = total_right_shift;

        // Create and return the signed fixed-point result.
        return negatable(nothing(), value_type((!is_neg) ?  value_type(result)
                                                         : -value_type(result)));
      }
    } // negatable frexp(negatable x, int* exp2)

    /*! @c std::ldexp function \<cmath\> implementation for negatable type.\n
        Multiplies a floating point value x by the number 2 raised to the exp2 power.\n
        TBD examples.\n

        \param x Fixed-point value to multiply by @c exp2 integral power of 2.
        \param exp2 power of 2 to use to multiply.
        \return x * 2^exp2.
    */

    friend inline negatable ldexp(negatable x, int exp2)
    {
      if(exp2 > 0)
      {
        return negatable(nothing(), value_type(x.data << exp2));
      }
      else if(exp2 < 0)
      {
        const bool is_neg = (x.data < 0);

        unsigned_small_type result((!is_neg) ? unsigned_small_type(x.data) : unsigned_small_type(-x.data));

        result = (result >> -exp2);

        return negatable(nothing(), value_type((!is_neg) ? value_type(result) : -value_type(result)));
      }
      else
      {
        return x;
      }
    } // negatable ldexp(negatable x, int exp2)

    /*! Provide next, prior and distance for fixed_point types.
    */

    friend inline negatable fixed_next(negatable x)
    {
      // TBD overflow?
      return x + (std::numeric_limits<negatable>::min)();
    }

    friend inline negatable fixed_prior(negatable x)
    { // TBD underflow?
      return x - (std::numeric_limits<negatable>::min)();
    }

    friend inline negatable fixed_distance(negatable x, negatable y)
    {
      // TBD over/underflow?
      // TBD no account of sign of x or y - see complex logic in boost.math next.hpp.
      // Not sure if return should not be an integer?
      // float_distance in next.hpp returns type T
      if(x == y)
      {
        return 0;
      }
      else
      {
        return (y - x) / (std::numeric_limits<negatable>::min)();
      }
    }

    friend inline negatable fixed_advance(negatable x, int distance)
    {
      // TBD over/underflow?
      return x + (distance * (std::numeric_limits<negatable>::min)());
    }

    friend inline negatable fixed_nextafter(negatable val, int direction)
    {
      // TBD over and underflow?
      return ((val < direction)
        ? val + (std::numeric_limits<negatable>::min)()
        : ((val == direction) ? val
                              : val - (std::numeric_limits<negatable>::min)()));  // val > direction 
    }

    friend inline negatable sqrt(negatable x)
    {
      // TBD: This implementation of square root seems to fail
      // for low digit counts.

      // TBD: This implementation of square root may be too inefficient
      // for low digit counts such as 5...16 digits. Consider optimization
      // (such as table-lookup of initial root estimate) for low digit counts.

      if(x.data <= 0)
      {
        return negatable(0);
      }
      else if(unsigned_small_type(x.data) < radix_split_value())
      {
        return 1U / (sqrt(1U / x));
      }
      else
      {
        // Get the initial estimate of the square root.

        // TBD: Is there a more efficient way to do this?
        // TBD: Is this initial guess accurate enough in all situations?
        // TBD: Compute some initial guesses and tabulate them as constant values.

        int exp_value;
        negatable mantissa = frexp(x, &exp_value);

        if((exp_value & 1) != 0)
        {
          mantissa.data <<= 1;
          --exp_value;
        }

        negatable result(ldexp(mantissa, exp_value / 2));

        // Estimate the zeroth term of the iteration = 1 / (2 * result).
        negatable vi = 1U / (result * 2U);

        // Compute the square root of x using coupled Newton iteration.
        // Here we use a conservative calculation of the digits of precision.
        // We begin with an estimate of 1/2 digit of precision and double
        // the number of digits with each iteration.

        // TBD: Here we are using too many Newton-Raphson steps.
        // TBD: Improve the accuracy of the initial estimate of the
        // square root and subsequently use a tighter tolerance
        // on the number of iterations in the Newton-Raphson loop.

        for(boost::uint_fast16_t i = UINT16_C(1); boost::uint_fast16_t(i / 2U) <= boost::uint_fast16_t(negatable::all_bits); i *= UINT16_C(2))
        {
          // Perform the next iteration of vi.
          vi += vi * (-((result * vi) * 2U) + 1U);

          // Perform the next iteration of the result.
          result += (vi * (-((result) * (result)) + x));
        }

        return result;
      }// negatable sqrt(negatable x)
    }
  };

  // Once-only instances of static constant variables of the negative class.
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> BOOST_CONSTEXPR_OR_CONST int negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::range;
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> BOOST_CONSTEXPR_OR_CONST int negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::resolution;
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> BOOST_CONSTEXPR_OR_CONST int negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::all_bits;
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> BOOST_CONSTEXPR_OR_CONST int negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::radix_split;

  //! Once-only instance of static constant variables of the negative class.
  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode,
           typename OverflowMode>
  typename negatable<IntegralRange,
                     FractionalResolution,
                     RoundMode,
                     OverflowMode>::initializer
  negatable<IntegralRange,
            FractionalResolution,
            RoundMode,
            OverflowMode>::initialization_helper;
  } } // namespace boost::fixed_point

  //! Effectively inject all defined cmath functions into the global namespace.
  //! So users can write sqrt(negatable) without requiring any namespace decoration.

  using boost::fixed_point::abs;
  using boost::fixed_point::fabs;
  using boost::fixed_point::frexp;
  using boost::fixed_point::ldexp;
  using boost::fixed_point::sqrt;

  // TBD: Ensure that ALL std:: added functions are also listed in this section.

  namespace std
  {
    //! Provide specializations of std::numeric_limits<negatable>.

    /*! \note Individual template specializations need to be provided
     for each different rounding mode and overflow mode.
     This might be 7 rounding * 5 overflow, a total of 35 specializations!
    */

    /*!template specialization of std::numeric_limits<negatable>
       for @c round::fastest and @c overflow::undefined.
    */
    template<const int IntegralRange,
             const int FractionalResolution>
    class numeric_limits<boost::fixed_point::negatable<IntegralRange,
                                                       FractionalResolution,
                                                       boost::fixed_point::round::fastest,
                                                       boost::fixed_point::overflow::undefined>>
    {
    private:
      typedef boost::fixed_point::negatable<IntegralRange,
                                            FractionalResolution,
                                            boost::fixed_point::round::fastest,
                                            boost::fixed_point::overflow::undefined> negatable_type;

    public:
      static BOOST_CONSTEXPR_OR_CONST bool                    is_specialized    = true;
      static BOOST_CONSTEXPR_OR_CONST int                     digits            = negatable_type::all_bits - 1;
      static BOOST_CONSTEXPR_OR_CONST int                     digits10          = static_cast<int>((static_cast<boost::uintmax_t>(digits - 1) * UINTMAX_C(3010)) / UINTMAX_C(10000));
      static BOOST_CONSTEXPR_OR_CONST int                     max_digits10      = static_cast<int>((static_cast<boost::uintmax_t>(digits - 0) * UINTMAX_C(3010)) / UINTMAX_C(10000)) + 2;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_signed         = true;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_integer        = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_exact          = false;
      static BOOST_CONSTEXPR_OR_CONST int                     radix             = 2;
      static BOOST_CONSTEXPR_OR_CONST int                     min_exponent      = -negatable_type::radix_split;
      static BOOST_CONSTEXPR_OR_CONST int                     min_exponent10    = -static_cast<int>((static_cast<boost::uintmax_t>(-min_exponent) * UINTMAX_C(3010)) / UINTMAX_C(10000));
      static BOOST_CONSTEXPR_OR_CONST int                     max_exponent      = digits - negatable_type::radix_split;
      static BOOST_CONSTEXPR_OR_CONST int                     max_exponent10    = +static_cast<int>((static_cast<boost::uintmax_t>(+max_exponent) * UINTMAX_C(3010)) / UINTMAX_C(10000));
      static BOOST_CONSTEXPR_OR_CONST bool                    has_infinity      = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    has_quiet_NaN     = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    has_signaling_NaN = false;
      static BOOST_CONSTEXPR_OR_CONST std::float_denorm_style has_denorm        = std::denorm_absent;
      static BOOST_CONSTEXPR_OR_CONST bool                    has_denorm_loss   = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_iec559         = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_bounded        = true;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_modulo         = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    traps             = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    tinyness_before   = false;
      static BOOST_CONSTEXPR_OR_CONST std::float_round_style  round_style       = std::round_indeterminate;

      static BOOST_CONSTEXPR_OR_CONST negatable_type (min)        () BOOST_NOEXCEPT { return negatable_type::value_min(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type (max)        () BOOST_NOEXCEPT { return negatable_type::value_max(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type lowest       () BOOST_NOEXCEPT { return -(max)(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type epsilon      () BOOST_NOEXCEPT { return negatable_type::epsilon_maker(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type round_error  () BOOST_NOEXCEPT { return negatable_type(1); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type infinity     () BOOST_NOEXCEPT { return negatable_type(0); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type quiet_NaN    () BOOST_NOEXCEPT { return negatable_type(0); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type signaling_NaN() BOOST_NOEXCEPT { return negatable_type(0); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type denorm_min   () BOOST_NOEXCEPT { return (min)(); }
    };

    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_specialized;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::digits;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::digits10;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::max_digits10;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_signed;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_integer;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_exact;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::radix;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::min_exponent;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::min_exponent10;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::max_exponent;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::max_exponent10;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_infinity;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_quiet_NaN;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_signaling_NaN;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST std::float_denorm_style numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_denorm;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_denorm_loss;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_iec559;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_bounded;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_modulo;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::traps;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::tinyness_before;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST std::float_round_style  numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::round_style;

    /*!template specialization of std::numeric_limits<negatable>
       for @c round::nearest_even and @c overflow::undefined.
    */
    template<const int IntegralRange,
             const int FractionalResolution>
    class numeric_limits<boost::fixed_point::negatable<IntegralRange,
                                                       FractionalResolution,
                                                       boost::fixed_point::round::nearest_even,
                                                       boost::fixed_point::overflow::undefined>>
    {
    private:
      typedef boost::fixed_point::negatable<IntegralRange,
                                            FractionalResolution,
                                            boost::fixed_point::round::nearest_even,
                                            boost::fixed_point::overflow::undefined> negatable_type;

    public:
      static BOOST_CONSTEXPR_OR_CONST bool                    is_specialized    = true;
      static BOOST_CONSTEXPR_OR_CONST int                     digits            = negatable_type::all_bits - 1;
      static BOOST_CONSTEXPR_OR_CONST int                     digits10          = static_cast<int>((static_cast<boost::uintmax_t>(digits - 1) * UINTMAX_C(3010)) / UINTMAX_C(10000));
      static BOOST_CONSTEXPR_OR_CONST int                     max_digits10      = static_cast<int>((static_cast<boost::uintmax_t>(digits - 0) * UINTMAX_C(3010)) / UINTMAX_C(10000)) + 2;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_signed         = true;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_integer        = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_exact          = false;
      static BOOST_CONSTEXPR_OR_CONST int                     radix             = 2;
      static BOOST_CONSTEXPR_OR_CONST int                     min_exponent      = -negatable_type::radix_split;
      static BOOST_CONSTEXPR_OR_CONST int                     min_exponent10    = -static_cast<int>((static_cast<boost::uintmax_t>(-min_exponent) * UINTMAX_C(3010)) / UINTMAX_C(10000));
      static BOOST_CONSTEXPR_OR_CONST int                     max_exponent      = digits - negatable_type::radix_split;
      static BOOST_CONSTEXPR_OR_CONST int                     max_exponent10    = +static_cast<int>((static_cast<boost::uintmax_t>(+max_exponent) * UINTMAX_C(3010)) / UINTMAX_C(10000));
      static BOOST_CONSTEXPR_OR_CONST bool                    has_infinity      = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    has_quiet_NaN     = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    has_signaling_NaN = false;
      static BOOST_CONSTEXPR_OR_CONST std::float_denorm_style has_denorm        = std::denorm_absent;
      static BOOST_CONSTEXPR_OR_CONST bool                    has_denorm_loss   = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_iec559         = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_bounded        = true;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_modulo         = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    traps             = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    tinyness_before   = false;
      static BOOST_CONSTEXPR_OR_CONST std::float_round_style  round_style       = std::round_to_nearest;

      static BOOST_CONSTEXPR_OR_CONST negatable_type (min)        () BOOST_NOEXCEPT { return negatable_type::value_min(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type (max)        () BOOST_NOEXCEPT { return negatable_type::value_max(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type lowest       () BOOST_NOEXCEPT { return -(max)(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type epsilon      () BOOST_NOEXCEPT { return negatable_type::epsilon_maker(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type round_error  () BOOST_NOEXCEPT { return negatable_type(1) / 2; }
      static BOOST_CONSTEXPR_OR_CONST negatable_type infinity     () BOOST_NOEXCEPT { return negatable_type(0); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type quiet_NaN    () BOOST_NOEXCEPT { return negatable_type(0); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type signaling_NaN() BOOST_NOEXCEPT { return negatable_type(0); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type denorm_min   () BOOST_NOEXCEPT { return (min)(); }
    };

    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_specialized;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::digits;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::digits10;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::max_digits10;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_signed;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_integer;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_exact;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::radix;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::min_exponent;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::min_exponent10;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::max_exponent;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::max_exponent10;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_infinity;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_quiet_NaN;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_signaling_NaN;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST std::float_denorm_style numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_denorm;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_denorm_loss;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_iec559;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_bounded;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_modulo;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::traps;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::tinyness_before;
    template<const int IntegralRange, const int FractionalResolution> BOOST_CONSTEXPR_OR_CONST std::float_round_style  numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::round_style;
  } // namespace std

#endif // FIXED_POINT_NEGATABLE_2015_03_06_HPP_