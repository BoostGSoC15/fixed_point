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

   In this file, we implement a prototype for the proposed
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
    //   * We must eliminate all I/O streaming and lexical conversions.
    //   * We must eliminate use of Boost.Multiprecision.

    #if !defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)
      #error Error: BOOST_FIXED_POINT_DISABLE_IOSTREAM can not be set without also setting BOOST_FIXED_POINT_DISABLE_MULTIPRECISION.
    #endif

    #include <algorithm>
    #include <cmath>
    #include <limits>
    #include <type_traits>

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
    #include <boost/lexical_cast.hpp>

  #else

    // When multiprecision and I/O streaming are enabled:
    //   * We eliminate nothing and include all overhead from both
    //     Boost.Multiprecision as well as I/O streaming and
    //     lexical conversions.

    #include <algorithm>
    #include <cmath>
    #include <iomanip>
    #include <istream>
    #include <limits>
    #include <ostream>
    #include <sstream>
    #include <string>
    #include <type_traits>
    #include <boost/lexical_cast.hpp>
    #include <boost/math/constants/constants.hpp>
    #include <boost/multiprecision/cpp_bin_float.hpp>
    #include <boost/multiprecision/cpp_int.hpp>

  #endif

  #include <boost/fixed_point/detail/fixed_point_detail.hpp>
  #include <boost/fixed_point/detail/fixed_point_detail_constants.hpp>
  #include <boost/fixed_point/fixed_point_overflow.hpp>
  #include <boost/fixed_point/fixed_point_round.hpp>

  // At the end of this file, we include headers for <cmath>
  // functions for the negatable type and also for specializations
  // of std::numeric_limits<negatable>.

  static_assert(std::numeric_limits<boost::uint8_t >::digits ==  8, "Configuration error: The size of boost::uint8_t  must be 8  bits!");
  static_assert(std::numeric_limits<boost::uint16_t>::digits == 16, "Configuration error: The size of boost::uint16_t must be 16 bits!");
  static_assert(std::numeric_limits<boost::uint32_t>::digits == 32, "Configuration error: The size of boost::uint32_t must be 32 bits!");
  static_assert(std::numeric_limits<boost::uint64_t>::digits == 64, "Configuration error: The size of boost::uint64_t must be 64 bits!");

  namespace boost { namespace fixed_point {

  // Forward declaration of the negatable class.
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  class negatable;

  // Forward declarations of the negatable_constants structures.
  template<typename NonSpecializedParameter>
  struct negatable_constants { };

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  struct negatable_constants<negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>;

  // Forward declarations of non-member binary add, sub, mul, div of (negatable op arithmetic_type).
  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator+(const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& u, const ArithmeticType& v);

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator-(const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& u, const ArithmeticType& v);

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator*(const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& u, const ArithmeticType& v);

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator/(const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& u, const ArithmeticType& v);

  // Forward declarations of non-member binary add, sub, mul, div of (arithmetic_type op negatable).
  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator+(const ArithmeticType& u, const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& v);

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator-(const ArithmeticType& u, const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& v);

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator*(const ArithmeticType& u, const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& v);

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator/(const ArithmeticType& u, const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& v);

  #if !defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM)

    // Forward declarations of I/O streaming functions.
    template<typename char_type, typename traits_type,
             const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
    std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& out, const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& x);

    template<typename char_type, typename traits_type,
             const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
    std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>& in, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& x);

  #endif

  // Forward declarations of <cmath> transcendental functions.
  // TBD: Implement all <cmath> transcendental functions.

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> abs  (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> fabs (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> floor(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> ceil (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> trunc(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> frexp(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, int* exp2);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> ldexp(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, int  exp2);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sqrt (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> exp  (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log  (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log2 (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> log10(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> pow  (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> a);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sin  (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cos  (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x, typename std::enable_if<int(24) >= (-FractionalResolution)>::type const* = nullptr);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> tan  (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> asin (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> acos (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atan (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atan2(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> y, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> sinh (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> cosh (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> tanh (negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> asinh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> acosh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> atanh(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x);
  } } // namespace boost::fixed_point:

  namespace std
  {
    // Forward declaration of the specialization of std::numeric_limits<negatable>.
    template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
    class numeric_limits<boost::fixed_point::negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>;
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
      @c negatable<15,  -16> @c y; // 32-bit \n
      @c negatable<10,  -53> @c y; // 64-bit \n
      @c negatable<10, -245> @c y; // 256-bit (requires use of @c cpp_bin_float)\n

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
    static_assert(FractionalResolution < 0, "Error: The fractional resolution of negatable must be negative and include at least 1 fractional bit.");

    // The negatable class can not have a negative integral range.
    static_assert(IntegralRange >= 0, "Error: The integral range of negatable must be 0 or more.");

    static_assert(   std::is_same<RoundMode, round::fastest>::value
                  || std::is_same<RoundMode, round::nearest_even>::value,
                  "Error: Only fastest and nearest_even round modes are supported at the moment.");

    static_assert(   std::is_same<OverflowMode, overflow::undefined>::value,
                  "Error: Only undefined overflow mode is supported at the moment.");

    // Make the integral range, the fractional resolution, and the total number
    // of bits available to the user. These just echo the values of the
    // template parameters.

    /*! Value of template parameter IntegralRange for the negatable type.\n
    Example: boost::fixed_point::negatable<2, -5> x; x.range == 2;
    */
    BOOST_STATIC_CONSTEXPR int range = IntegralRange;

    /*! Value of template parameter FractionalResolution for the negatable type.\n
    Example: boost::fixed_point::negatable<2, -5> x; x.resolution == -5;
    \note The value of resolution is always negative.
    */
    BOOST_STATIC_CONSTEXPR int resolution = FractionalResolution;

    /*! Total number of bits in the negatable type, including sign.\n
        For example:
        \code
        boost::fixed_point::negatable<2, -5> x; int n=x.all_bits; n==8 \n
        x.range + (-x.resolution) + 1 == 2 + (-(-5)) + 1 == 8.
        \endcode
    */
    BOOST_STATIC_CONSTEXPR int all_bits = (range + 1) + (-resolution); // +1 for the sign bit.

    static_assert(all_bits < 32768, "Error: At the moment, the width of fixed_point negatable can not exceed 32767 bits.");

    #if defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)
      static_assert(all_bits <= 32, "Error: The width of fixed_point negatable can not exceed 32 bits when multiprecision is disabled.");
    #endif

    //! See also public static data items range and resolution.
    BOOST_STATIC_CONSTEXPR int radix_split = -FractionalResolution;

    // Friend forward declaration of another negatable class
    // with different template parameters.

    template<const int OtherIntegralRange, const int OtherFractionalResolution, typename OtherRoundMode, typename OtherOverflowMode>
    friend class negatable;

    // Here we declare two convenient class-local type definitions.
    //
    /*!
      The signed integer representation of the fixed-point negatable number.\n
      For low digit counts, this will be
      a built-in type such as @c int8_t, @c int16_t, @c int32_t, @c int64_t, etc.\n
      For larger digit counts, this will be a multiprecision signed integer type.
    */
    typedef typename detail::integer_type_helper<boost::uint32_t(all_bits)>::exact_signed_type value_type;

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
    typedef typename detail::float_type_helper<boost::uint32_t(all_bits - 1)>::exact_float_type float_type;

  private:
    typedef typename detail::integer_type_helper<boost::uint32_t(negatable::all_bits * 1)>::exact_unsigned_type unsigned_small_type;
    typedef typename detail::integer_type_helper<boost::uint32_t(negatable::all_bits * 2)>::exact_unsigned_type unsigned_large_type;

  public:
    // The public class constructors follow below.

    // The class default constructor is implemented below.

    /*! Default constructor.\n By design choice, this clears the data member.\n 
        So after defining @c negatable<15,-16> @c x; then @c x==0; 
    */
    negatable() : data() { }

    /*! Constructors from built-in signed integral types. Lossy construction is made explicit.
    */

    // This is non-explicit because the conversion from SignedIntegralType to fixed-point is non-lossy.
    template<typename SignedIntegralType>
    BOOST_CONSTEXPR negatable(const SignedIntegralType& n,
                              typename std::enable_if<   (std::is_integral<SignedIntegralType>::value == true)
                                                      && (std::is_signed  <SignedIntegralType>::value == true)
                                                      && (std::numeric_limits<SignedIntegralType>::digits <= IntegralRange)>::type const* = nullptr)
      : data(make_from_signed_integral_type(n)) { }

    // This is explicit because the conversion from SignedIntegralType to fixed-point is lossy.
    template<typename SignedIntegralType>
    explicit BOOST_CONSTEXPR negatable(const SignedIntegralType& n,
                                       typename std::enable_if<   (std::is_integral<SignedIntegralType>::value == true)
                                                               && (std::is_signed  <SignedIntegralType>::value == true)
                                                               && (std::numeric_limits<SignedIntegralType>::digits > IntegralRange)>::type const* = nullptr)
      : data(make_from_signed_integral_type(n)) { }

    /*! Constructors from built-in unsigned integral types. Lossy construction is made explicit.
    */

    // This is non-explicit because the conversion from UnsignedIntegralType to fixed-point is non-lossy.
    template<typename UnsignedIntegralType>
    BOOST_CONSTEXPR negatable(const UnsignedIntegralType& u,
                              typename std::enable_if<   (std::is_integral<UnsignedIntegralType>::value == true)
                                                      && (std::is_signed  <UnsignedIntegralType>::value == false)
                                                      && (std::numeric_limits<UnsignedIntegralType>::digits <= IntegralRange)>::type const* = nullptr)
      : data(make_from_unsigned_integral_type(u)) { }

    // This is explicit because the conversion from UnsignedIntegralType to fixed-point is lossy.
    template<typename UnsignedIntegralType>
    explicit BOOST_CONSTEXPR negatable(const UnsignedIntegralType& u,
                                       typename std::enable_if<   (std::is_integral<UnsignedIntegralType>::value == true)
                                                               && (std::is_signed  <UnsignedIntegralType>::value == false)
                                                               && (std::numeric_limits<UnsignedIntegralType>::digits > IntegralRange)>::type const* = nullptr)
      : data(make_from_unsigned_integral_type(u)) { }

    /*! Constructors enabled when value_type and unsigned_small_type are non-built-in types.\n
        These constructors are explicit.
    */

    template<typename ValueType>
    explicit negatable(const ValueType& n,
                       typename std::enable_if<   (std::is_same<ValueType, value_type>::value == true)
                                               && (std::is_arithmetic<ValueType>::value == false)>::type const* = nullptr)
    : data(n) { }

    template<typename UnsignedIntegralType>
    explicit negatable(const UnsignedIntegralType& u,
                       typename std::enable_if<   (std::is_same<UnsignedIntegralType, unsigned_small_type>::value == true)
                                               && (std::is_arithmetic<UnsignedIntegralType>::value == false)>::type const* = nullptr)
      : data(value_type(u)) { }

    /*! Constructors from built-in floating-point types: @c float, @c double or @c long @c double.\n
        Example: negatable<15,-16> x(2.3L);\n
        (Overflow and underflow are, of course, possible).
    */
    template<typename FloatingPointType>
    negatable(const FloatingPointType& f,
              typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr)
      : data(make_from_floating_point_type(f)) { }

    template<typename FloatingPointType>
    negatable(const FloatingPointType& f,
              typename std::enable_if<   (std::is_same<FloatingPointType, float_type>::value == true)
                                      && (std::is_arithmetic<FloatingPointType>::value == false)>::type const* = nullptr)
      : data(make_from_floating_point_type(f)) { }

    /*! Copy constructor.
    */
    negatable(const negatable& v) : data(v.data) { }

    /*! Move constructor.
    */
    negatable(negatable&& v) : data(static_cast<value_type&&>(v.data)) { }

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

    // What follows are mixed-math class constructors that
    // create a negatable type from another negatable type
    // having different range and/or resolution.

    // We can identify four cases:
    // 1) smaller : smaller -->    ( OtherIntegralRange  <=  IntegralRange)
    //                          && (|OtherFractionalResolution| <= |FractionalResolution|)
    // 2) larger  : smaller -->    ( OtherIntegralRange  >   IntegralRange)
    //                          && (|OtherFractionalResolution| <= |FractionalResolution|)
    // 3) smaller : larger  -->    ( OtherIntegralRange  <=  IntegralRange)
    //                          && (|OtherFractionalResolution| >  |FractionalResolution|)
    // 4) larger  : larger  -->    ( OtherIntegralRange  >   IntegralRange)
    //                          && (|OtherFractionalResolution| >  |FractionalResolution|)

    // Here is the mixed-math class constructor for case 1).
    // There is less range and less resolution in the other type.
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    negatable(const negatable<OtherIntegralRange, OtherFractionalResolution, RoundMode, OverflowMode>& other,
              typename std::enable_if<   (   ( OtherIntegralRange        <   IntegralRange)
                                          && (-OtherFractionalResolution <  -FractionalResolution))
                                      || (   ( OtherIntegralRange        <=  IntegralRange)
                                          && (-OtherFractionalResolution <  -FractionalResolution))
                                      || (   ( OtherIntegralRange        <   IntegralRange)
                                          && (-OtherFractionalResolution <= -FractionalResolution))>::type* = nullptr)
       : data()
    {
      typedef negatable<OtherIntegralRange, OtherFractionalResolution, RoundMode, OverflowMode> other_negatable_type;

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
             const int OtherFractionalResolution>
    negatable(const negatable<OtherIntegralRange, OtherFractionalResolution, RoundMode, OverflowMode>& other,
              typename std::enable_if<   ( OtherIntegralRange        >  IntegralRange)
                                      && (-OtherFractionalResolution < -FractionalResolution)>::type* = nullptr)
      : data()
    {
      typedef negatable<OtherIntegralRange, OtherFractionalResolution, RoundMode, OverflowMode> other_negatable_type;

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
             const int OtherFractionalResolution>
    negatable(const negatable<OtherIntegralRange, OtherFractionalResolution, RoundMode, OverflowMode>& other,
              typename std::enable_if<   ( OtherIntegralRange        <   IntegralRange)
                                      && (-OtherFractionalResolution >  -FractionalResolution)>::type* = nullptr)
      : data()
    {
      typedef negatable<OtherIntegralRange, OtherFractionalResolution, RoundMode, OverflowMode> other_negatable_type;

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
      // With round modes fastest and nearest even, there is no need
      // for special code for handling underflow. But be aware of
      // underflow issues if other rounding modes are supported.
      if     (rounding_result == INT8_C(+1)) { ++u_round; }
      else if(rounding_result == INT8_C(-1)) { --u_round; }

      data = ((!is_neg) ? value_type(u_round) : -value_type(u_round));
    }

    // Here is the mixed-math class constructor for case 4).
    // There is more range and more resolution in the other type.
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    negatable(const negatable<OtherIntegralRange, OtherFractionalResolution, RoundMode, OverflowMode>& other,
              typename std::enable_if<   (   ( OtherIntegralRange        >   IntegralRange)
                                          && (-OtherFractionalResolution >  -FractionalResolution))
                                      || (   ( OtherIntegralRange        >=  IntegralRange)
                                          && (-OtherFractionalResolution >  -FractionalResolution))
                                      || (   ( OtherIntegralRange        >   IntegralRange)
                                          && (-OtherFractionalResolution >= -FractionalResolution))>::type* = nullptr)
      : data()
    {
      typedef negatable<OtherIntegralRange, OtherFractionalResolution, RoundMode, OverflowMode> other_negatable_type;

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
      // With round modes fastest and nearest even, there is no need
      // for special code for handling underflow. But be aware of
      // underflow issues if other rounding modes are supported.
      if     (rounding_result == INT8_C(+1)) { ++u_round; }
      else if(rounding_result == INT8_C(-1)) { --u_round; }

      data = ((!is_neg) ? value_type(u_round) : -value_type(u_round));
    }

    /*! Destructor.\n
        It has trivial complexity because the negatable class does not do any allocation
        or complex operations (if any) that are not already handled by the underlying @c value_type.
    */
    ~negatable() { }

    /*! Assigment operators.\n
    */
    // This is the standard assigment operator.
    negatable& operator=(const negatable& other)
    {
      if(this != (&other))
      {
        data = other.data;
      }

      return *this;
    }

    // This is the move assignment operator.
    negatable& operator=(negatable&& other)
    {
      data = static_cast<value_type&&>(other.data);

      return *this;
    }

    /*! Assignment operator of @c *this with another negatable type
    having @b different range and/or resolution parameters than @c *this.
    */
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    negatable& operator=(const negatable<OtherIntegralRange, OtherFractionalResolution, RoundMode, OverflowMode>& other)
    {
      // Use a relatively lazy method that creates an intermediate temporary object.
      // The temporary object is subsequently used to initialize the data field of *this.

      const negatable tmp(other);

      data = tmp.data;

      return *this;
    }

    //! Assignment operators for built-in integral types.
    template<typename SignedIntegralType,
             typename std::enable_if<   (std::is_integral<SignedIntegralType>::value == true)
                                     && (std::is_signed  <SignedIntegralType>::value == true)
                                     && (std::numeric_limits<SignedIntegralType>::digits <= IntegralRange)>::type const* = nullptr>
    negatable& operator=(const SignedIntegralType& n)
    {
      data = make_from_signed_integral_type(n);

      return *this;
    }

    template<typename UnsignedIntegralType,
             typename std::enable_if<   (std::is_integral<UnsignedIntegralType>::value == true)
                                     && (std::is_signed  <UnsignedIntegralType>::value == false)
                                     && (std::numeric_limits<UnsignedIntegralType>::digits <= IntegralRange)>::type const* = nullptr>
    negatable& operator=(const UnsignedIntegralType& u)
    {
      data = make_from_unsigned_integral_type(u);

      return *this;
    }

    template<typename FloatingPointType,
             typename std::enable_if<std::is_floating_point<FloatingPointType>::value>::type const* = nullptr>
    negatable& operator=(const FloatingPointType& f)
    {
      data = make_from_floating_point_type(f);

      return *this;
    }

    //! Unary pre-increment and pre-decrement operators.
    negatable& operator++()   { data += value_type(unsigned_small_type(1) << radix_split); return *this; }
    negatable& operator--()   { data -= value_type(unsigned_small_type(1) << radix_split); return *this; }

    //! Unary post-increment and post-decrement operators.
    negatable operator++(int) { const negatable tmp(*this); data += value_type(unsigned_small_type(1) << radix_split); return tmp; }
    negatable operator--(int) { const negatable tmp(*this); data -= value_type(unsigned_small_type(1) << radix_split); return tmp; }

    //! Unary operator add of (*this += negatable).
    negatable& operator+=(const negatable& v)
    {
      data += v.data;

      const bool is_neg = (data < 0);

      unsigned_small_type u = (!(is_neg) ? unsigned_small_type(data) : unsigned_small_type (-data));

      u = (u & unsigned_small_mask());

      data = value_type(!(is_neg) ? value_type(u) : -value_type(u));

      return *this;
    }

    //! Unary operator subtract of (*this -= negatable).
    negatable& operator-=(const negatable& v)
    {
      data -= v.data;

      const bool is_neg = (data < 0);

      unsigned_small_type u = (!(is_neg) ? unsigned_small_type(data) : unsigned_small_type(-data));

      u = (u & unsigned_small_mask());

      data = value_type(!(is_neg) ? value_type(u) : -value_type(u));

      return *this;
    }

    //! Unary operator multiply of (*this *= negatable).
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
      // With round modes fastest and nearest even, there is no need
      // for special code for handling underflow. But be aware of
      // underflow issues if other rounding modes are supported.
      if     (rounding_result == INT8_C(+1)) { ++u_round; }
      else if(rounding_result == INT8_C(-1)) { --u_round; }

      u_round = (u_round & unsigned_small_mask());

      // Load the fixed-point result (and account for potentially signed values).
      data = value_type((!result_is_neg) ? value_type(u_round) : -value_type(u_round));

      return *this;
    }

    //! Unary operator divide of (*this /= negatable).
    negatable& operator/=(const negatable& v)
    {
      if(v.data == 0)
      {
        data = 0;
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
        // With round modes fastest and nearest even, there is no need
        // for special code for handling underflow. But be aware of
        // underflow issues if other rounding modes are supported.
        if     (rounding_result == INT8_C(+1)) { ++u_round; }
        else if(rounding_result == INT8_C(-1)) { --u_round; }

        u_round = (u_round & unsigned_small_mask());

        // Load the fixed-point result (and account for potentially signed values).
        data = value_type((!result_is_neg) ? value_type(u_round) : -value_type(u_round));
      }

      return *this;
    }

    //! Unary operators add, sub, mul, div of (*this op= negatable).
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> negatable& operator+=(const ArithmeticType& a) { return (*this) += negatable(a); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> negatable& operator-=(const ArithmeticType& a) { return (*this) -= negatable(a); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> negatable& operator*=(const ArithmeticType& a) { return (*this) *= negatable(a); }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type const* = nullptr> negatable& operator/=(const ArithmeticType& a) { return (*this) /= negatable(a); }

    /*! Cast operators for built-in signed and unsigned integral types.\n

     Note: Cast from @c negatable to a built-in integral type truncates
     the fractional part regardless of the rounding mode.\n
     This is consistent with the conversion from built-in floating-point types
     to built-in integral types. See also ISO/IEC 14882:2011 paragraph 4.9.1.
    */

    operator signed char     () const { return static_cast<signed char>     ((!(data < 0)) ? static_cast<char>     (unsigned_small_type(data) >> radix_split) : -static_cast<char>     (unsigned_small_type(-data) >> radix_split)); }
    operator signed short    () const { return static_cast<signed short>    ((!(data < 0)) ? static_cast<short>    (unsigned_small_type(data) >> radix_split) : -static_cast<short>    (unsigned_small_type(-data) >> radix_split)); }
    operator signed int      () const { return static_cast<signed int>      ((!(data < 0)) ? static_cast<int>      (unsigned_small_type(data) >> radix_split) : -static_cast<int>      (unsigned_small_type(-data) >> radix_split)); }
    operator signed long     () const { return static_cast<signed long>     ((!(data < 0)) ? static_cast<long>     (unsigned_small_type(data) >> radix_split) : -static_cast<long>     (unsigned_small_type(-data) >> radix_split)); }
    operator signed long long() const { return static_cast<signed long long>((!(data < 0)) ? static_cast<long long>(unsigned_small_type(data) >> radix_split) : -static_cast<long long>(unsigned_small_type(-data) >> radix_split)); }

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

    static const unsigned_small_type& unsigned_small_mask() BOOST_NOEXCEPT
    {
      initialization_helper.force_premain_init_of_static_constants();

      static const unsigned_small_type the_value =
        detail::bit_mask_helper<unsigned_small_type,
                                0U,
                                boost::uint32_t(IntegralRange - FractionalResolution)>::value();

      return the_value;
    }

    static const unsigned_small_type& unsigned_integer_part_mask() BOOST_NOEXCEPT
    {
      initialization_helper.force_premain_init_of_static_constants();

      static const unsigned_small_type the_value =
        detail::bit_mask_helper<unsigned_small_type,
                                boost::uint32_t(-FractionalResolution),
                                boost::uint32_t(IntegralRange + 1)>::value();

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
    static value_type make_from_unsigned_integral_type(const UnsignedIntegralType& u)
    {
      // Here, we make a negatable value_type from an unsigned integral source value.
      return value_type(unsigned_small_type(unsigned_small_type(u) << radix_split) & unsigned_small_mask());
    }

    template<typename SignedIntegralType>
    static value_type make_from_signed_integral_type(const SignedIntegralType& n)
    {
      // Here, we make a negatable value_type from a signed integral source value.
      return ((!(n < 0)) ? +value_type((unsigned_small_type(+n) << radix_split) & unsigned_small_mask())
                         : -value_type((unsigned_small_type(-n) << radix_split) & unsigned_small_mask()));
    }

    template<typename FloatingPointType>
    static value_type make_from_floating_point_type(const FloatingPointType& f)
    {
      // Here, we make a negatable value_type from a floating-point source value.

      // If the number of digits in the floating-point source type and
      // the number of digits in the target data value type differ,
      // then rounding potentially makes sense. In this case,
      // we reserve 1 extra binary digit for rounding.

      // Otherwise the floating-point source type and the target
      // data value type have exactly the same amount of digits.
      // In this case rounding need not be carried out because
      // no additional information is available for rounding,
      // and no extra binary digit is reserved for rounding.

      BOOST_CONSTEXPR_OR_CONST boost::uint32_t floating_point_digits          = boost::uint32_t(std::numeric_limits<FloatingPointType>::digits);
      BOOST_CONSTEXPR_OR_CONST boost::uint32_t floating_point_digits_plus_one = boost::uint32_t(floating_point_digits + 1);
      BOOST_CONSTEXPR_OR_CONST boost::uint32_t unsigned_small_digits          = boost::uint32_t(std::numeric_limits<unsigned_small_type>::digits);

      BOOST_CONSTEXPR_OR_CONST bool rounding_is_to_be_carried_out = (floating_point_digits_plus_one != unsigned_small_digits);

      // Determine the number of digits needed for the conversion.
      // * If rounding is to be done, this is the larger of [digits(FloatingPointType) + 1]
      //   and digits(unsigned_small_type).
      // * If rounding is *not* to be done, this is the larger of digits(FloatingPointType)
      //   and digits(unsigned_small_type).

      BOOST_CONSTEXPR_OR_CONST boost::uint32_t floating_point_conversion_digits =
        (rounding_is_to_be_carried_out ? floating_point_digits_plus_one : floating_point_digits);

      BOOST_CONSTEXPR_OR_CONST boost::uint32_t unsigned_conversion_digits =
        ((floating_point_conversion_digits > unsigned_small_digits) ? floating_point_conversion_digits
                                                                    : unsigned_small_digits);

      // Define a local_unsigned_conversion_type.
      typedef
      typename detail::integer_type_helper<unsigned_conversion_digits>::exact_unsigned_type
      local_unsigned_conversion_type;

      const bool is_neg = (f < FloatingPointType(0));

      using std::frexp;
      using std::ldexp;

      // Extract the mantissa and exponent.
      int exp2;
      const FloatingPointType fp(frexp((!is_neg) ? f : -f, &exp2));

      // Here we scale the mantissa to an unsigned integer value
      // that is large enough to contain all the binary digits
      // of the FloatingPointType representation (plus potentially
      // one extra rounding digit).

      const FloatingPointType fp_shifted = ldexp(fp, int(floating_point_conversion_digits));

      local_unsigned_conversion_type u;

      detail::conversion_helper<local_unsigned_conversion_type,
                                FloatingPointType>::convert_floating_point_to_unsigned_integer(fp_shifted, u);

      const int total_left_shift = (radix_split + exp2) - floating_point_digits;

      const local_unsigned_conversion_type u_round_local(detail::left_shift_helper(u, total_left_shift));

      unsigned_small_type u_round = static_cast<unsigned_small_type>(u_round_local);

      // Round the result of the construction from FloatingPointType if needed.
      const boost::int_fast8_t rounding_result =
        (rounding_is_to_be_carried_out ? binary_round(u_round) : INT8_C(0));

      // Add or subtract the result of the rounding (-1, 0, or +1).
      // With round modes fastest and nearest even, there is no need
      // for special code for handling underflow. But be aware of
      // underflow issues if other rounding modes are supported.
      if     (rounding_result == INT8_C(+1)) { ++u_round; }
      else if(rounding_result == INT8_C(-1)) { --u_round; }

      u_round = (u_round & unsigned_small_mask());

      // Load the fixed-point result (and account for potentially signed values).
      return value_type((!is_neg) ? value_type(u_round) : -value_type(u_round));
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
          // With round modes fastest and nearest even, there is no need
          // for special code for handling underflow. But be aware of
          // underflow issues if other rounding modes are supported.
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

    /*! Compute (during pre-main static initialization) the maximum value that the type can represent.\n
        Used to define function @c std::numeric_limits<>::max()
        and, when negated, @c std::numeric_limits<>::lowest().
    */
    static const negatable& value_max() BOOST_NOEXCEPT
    {
      initialization_helper.force_premain_init_of_static_constants();

      static const negatable the_value_max(nothing(), static_cast<value_type>(unsigned_small_mask()));

      return the_value_max;
    }

    /*! Compute (during pre-main static initialization) the minimum value that the type can represent.\n
        Used to define function @c std::numeric_limits<>::min().
    */
    static const negatable& value_min() BOOST_NOEXCEPT
    {
      initialization_helper.force_premain_init_of_static_constants();

      static const negatable the_value_min(nothing(), static_cast<value_type>(1));

      return the_value_min;
    }

    /*! Compute machine epsilon (during pre-main static initialization) 
        for @c std::numeric_limits<>::epsilon() function.
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

      BOOST_CONSTEXPR int total_right_shift = -(resolution + 1);

      static const negatable the_epsilon(nothing(), static_cast<value_type>(radix_split_value() >> total_right_shift));

      return the_epsilon;
    }

    /*! Compute (during pre-main static initialization) the representation of the mathematical constant pi.\n
    */
    static const negatable& value_pi()
    {
      initialization_helper.force_premain_init_of_static_constants();

      static const negatable the_value_pi = pi_helper<boost::uint32_t(-resolution)>::calculate_pi();

      return the_value_pi;
    }

    /*! Compute (during pre-main static initialization) the representation of the mathematical constant log(2).\n
    */
    static const negatable& value_ln_two()
    {
      initialization_helper.force_premain_init_of_static_constants();

      static const negatable the_value_ln_two = ln_two_helper<boost::uint32_t(-resolution)>::calculate_ln_two();

      return the_value_ln_two;
    }

    template<const boost::uint32_t BitCount,
             typename EnableType = void>
    struct pi_helper
    {
      static negatable calculate_pi()
      {
        const float_type val_pi_floating_point_rep = boost::fixed_point::detail::calculate_pi<float_type>();

        return negatable(val_pi_floating_point_rep);
      }
    };

    template<const boost::uint32_t BitCount>
    struct pi_helper<BitCount,
                     typename std::enable_if<(BitCount < 62U)>::type>
    {
      static negatable calculate_pi()
      {
        BOOST_CONSTEXPR_OR_CONST value_type pi_data = value_type(UINT64_C(0x6487ED5110B4611A) >> (61 - int(BitCount)));

        return negatable(nothing(), pi_data);
      }
    };

    template<const boost::uint32_t BitCount,
             typename EnableType = void>
    struct ln_two_helper
    {
      static negatable calculate_ln_two()
      {
        const float_type val_ln_two_floating_point_rep = boost::fixed_point::detail::calculate_ln_two<float_type>();

        return negatable(val_ln_two_floating_point_rep);
      }
    };

    template<const boost::uint32_t BitCount>
    struct ln_two_helper<BitCount,
                         typename std::enable_if<(BitCount < UINT32_C(64))>::type>
    {
      static negatable calculate_ln_two()
      {
        BOOST_CONSTEXPR_OR_CONST value_type ln_two_data = value_type(UINT64_C(0x58B90BFBE8E7BCD6) >> (UINT32_C(63) - (BitCount)));

        return negatable(nothing(), ln_two_data);
      }
    };

    friend class std::numeric_limits<negatable>;

    friend struct negatable_constants<negatable>;

    friend struct initializer;

    struct initializer
    {
      initializer()
      {
        static_cast<void>(negatable::radix_split_value());
        static_cast<void>(negatable::unsigned_small_mask());
        static_cast<void>(negatable::unsigned_integer_part_mask());
        static_cast<void>(negatable::value_max());
        static_cast<void>(negatable::value_min());
        static_cast<void>(negatable::epsilon_maker());
        static_cast<void>(negatable::value_pi());
        static_cast<void>(negatable::value_ln_two());
      }

      void force_premain_init_of_static_constants() { }
    };

    static initializer initialization_helper;

    //! \cond DETAIL
    // Do not document the implementation details unless macro DETAIL is defined.

    //! Implementations of non-member unary plus and minus.
    friend inline negatable operator+(const negatable& self) { return negatable(self); }
    friend inline negatable operator-(const negatable& self) { negatable tmp(self); tmp.data = -tmp.data; return tmp; }

    //! Implementations of non-member comparison operators of (negatable cmp negatable).
    friend inline bool operator==(const negatable& u, const negatable& v) { return (u.data == v.data); }
    friend inline bool operator!=(const negatable& u, const negatable& v) { return (u.data != v.data); }
    friend inline bool operator> (const negatable& u, const negatable& v) { return (u.data >  v.data); }
    friend inline bool operator< (const negatable& u, const negatable& v) { return (u.data <  v.data); }
    friend inline bool operator>=(const negatable& u, const negatable& v) { return (u.data >= v.data); }
    friend inline bool operator<=(const negatable& u, const negatable& v) { return (u.data <= v.data); }

    //! Implementations of non-member comparison operators of (negatable cmp ArithmeticType).
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator==(const negatable& u, const ArithmeticType& v) { return (u.data == negatable(v).data); }
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator!=(const negatable& u, const ArithmeticType& v) { return (u.data != negatable(v).data); }
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator> (const negatable& u, const ArithmeticType& v) { return (u.data >  negatable(v).data); }
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator< (const negatable& u, const ArithmeticType& v) { return (u.data <  negatable(v).data); }
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator>=(const negatable& u, const ArithmeticType& v) { return (u.data >= negatable(v).data); }
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator<=(const negatable& u, const ArithmeticType& v) { return (u.data <= negatable(v).data); }

    //! Implementations of non-member comparison operators of (ArithmeticType cmp negatable).
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator==(const ArithmeticType& u, const negatable& v) { return (negatable(u).data == v.data); }
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator!=(const ArithmeticType& u, const negatable& v) { return (negatable(u).data != v.data); }
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator> (const ArithmeticType& u, const negatable& v) { return (negatable(u).data >  v.data); }
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator< (const ArithmeticType& u, const negatable& v) { return (negatable(u).data <  v.data); }
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator>=(const ArithmeticType& u, const negatable& v) { return (negatable(u).data >= v.data); }
    template<typename ArithmeticType> friend inline typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, bool>::type operator<=(const ArithmeticType& u, const negatable& v) { return (negatable(u).data <= v.data); }

    //! Implementations of non-member comparison operators of (negatable cmp other_negatable).
    template<const int OtherIntegralRange, const int OtherFractionalResolution>
    friend inline bool operator==(const negatable& u,
                                  const negatable<OtherIntegralRange, OtherFractionalResolution>& v)
    {
      typedef negatable<(( IntegralRange >  OtherIntegralRange) ? IntegralRange : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution)> supra_negatable_type;

      return (supra_negatable_type(u).data == supra_negatable_type(v).data);
    }

    template<const int OtherIntegralRange, const int OtherFractionalResolution>
    friend inline bool operator!=(const negatable& u,
                                  const negatable<OtherIntegralRange, OtherFractionalResolution>& v)
    {
      typedef negatable<(( IntegralRange        >  OtherIntegralRange) ? IntegralRange : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution)> supra_negatable_type;

      return (supra_negatable_type(u).data != supra_negatable_type(v).data);
    }

    template<const int OtherIntegralRange, const int OtherFractionalResolution>
    friend inline bool operator> (const negatable& u,
                                  const negatable<OtherIntegralRange, OtherFractionalResolution>& v)
    {
      typedef negatable<(( IntegralRange        >  OtherIntegralRange) ? IntegralRange : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution)> supra_negatable_type;

      return (supra_negatable_type(u).data > supra_negatable_type(v).data);
    }

    template<const int OtherIntegralRange, const int OtherFractionalResolution>
    friend inline bool operator< (const negatable& u,
                                  const negatable<OtherIntegralRange, OtherFractionalResolution>& v)
    {
      typedef negatable<(( IntegralRange        >  OtherIntegralRange) ? IntegralRange : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution)> supra_negatable_type;

      return (supra_negatable_type(u).data < supra_negatable_type(v).data);
    }

    template<const int OtherIntegralRange, const int OtherFractionalResolution>
    friend inline bool operator>=(const negatable& u,
                                  const negatable<OtherIntegralRange, OtherFractionalResolution>& v)
    {
      typedef negatable<(( IntegralRange        >  OtherIntegralRange) ? IntegralRange : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution)> supra_negatable_type;

      return (supra_negatable_type(u).data >= supra_negatable_type(v).data);
    }

    template<const int OtherIntegralRange, const int OtherFractionalResolution>
    friend inline bool operator<=(const negatable& u,
                                  const negatable<OtherIntegralRange, OtherFractionalResolution>& v)
    {
      typedef negatable<(( IntegralRange        >  OtherIntegralRange) ? IntegralRange : OtherIntegralRange),
                        ((-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution)> supra_negatable_type;

      return (supra_negatable_type(u).data <= supra_negatable_type(v).data);
    }

    //! \endcond // DETAIL

    #if !defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM)

      template<typename char_type, typename traits_type, const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2>
      friend std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& out,
                                                                    const negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2>& x);

      template<typename char_type, typename traits_type, const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2>
      friend std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>& in,
                                                                    negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2>& x);

    #endif // !BOOST_FIXED_POINT_DISABLE_IOSTREAM

    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> abs  (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> fabs (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> floor(negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> ceil (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> trunc(negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> frexp(negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x, int* exp2);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> ldexp(negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x, int  exp2);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> sqrt (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> exp  (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> log  (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> log2 (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> log10(negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> pow  (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x, negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> a);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> sin  (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> cos  (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x, typename std::enable_if<int(24) >= (-FractionalResolution2)>::type const*);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> tan  (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> asin (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> acos (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> atan (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> atan2(negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> y, negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> sinh (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> cosh (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> tanh (negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> asinh(negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> acosh(negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
    template<const int IntegralRange2, const int FractionalResolution2, typename RoundMode2, typename OverflowMode2> friend negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> atanh(negatable<IntegralRange2, FractionalResolution2, RoundMode2, OverflowMode2> x);
  };

  //! Once-only instances of static constant variables of the negative class.
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> BOOST_CONSTEXPR_OR_CONST int negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::range;
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> BOOST_CONSTEXPR_OR_CONST int negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::resolution;
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> BOOST_CONSTEXPR_OR_CONST int negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::all_bits;
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> BOOST_CONSTEXPR_OR_CONST int negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::radix_split;

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::initializer negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::initialization_helper;

  template<const int IntegralRange, const int FractionalResolution>
  struct negatable_constants<negatable<IntegralRange, FractionalResolution, round::fastest, overflow::undefined>>
  {
  private:
    typedef negatable<IntegralRange, FractionalResolution, round::fastest, overflow::undefined> local_negatable_type;

  public:
    BOOST_STATIC_CONSTEXPR local_negatable_type pi    () { return local_negatable_type::value_pi(); }
    BOOST_STATIC_CONSTEXPR local_negatable_type ln_two() { return local_negatable_type::value_ln_two(); }
  };

  template<const int IntegralRange, const int FractionalResolution>
  struct negatable_constants<negatable<IntegralRange, FractionalResolution, round::nearest_even, overflow::undefined>>
  {
  private:
    typedef negatable<IntegralRange, FractionalResolution, round::fastest, overflow::undefined> local_negatable_type;

  public:
    BOOST_STATIC_CONSTEXPR local_negatable_type pi    () { return local_negatable_type::value_pi(); }
    BOOST_STATIC_CONSTEXPR local_negatable_type ln_two() { return local_negatable_type::value_ln_two(); }
  };

  // Implementations of non-member binary add, sub, mul, div of (negatable op arithmetic_type).
  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator+(const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& u, const ArithmeticType& v)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> negatable_type;

    return negatable_type(u) += v;
  }

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator-(const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& u, const ArithmeticType& v)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> negatable_type;

    return negatable_type(u) -= v;
  }

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator*(const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& u, const ArithmeticType& v)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> negatable_type;

    return negatable_type(u) *= v;
  }

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator/(const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& u, const ArithmeticType& v)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> negatable_type;

    return negatable_type(u) /= v;
  }

  // Implementations of non-member binary add, sub, mul, div of (arithmetic_type op negatable).
  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator+(const ArithmeticType& u, const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& v)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> negatable_type;

    return negatable_type(u) += v;
  }

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator-(const ArithmeticType& u, const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& v)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> negatable_type;

    return negatable_type(u) -= v;
  }

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator*(const ArithmeticType& u, const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& v)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> negatable_type;

    return negatable_type(u) *= v;
  }

  template<typename ArithmeticType,
           const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename std::enable_if<std::is_arithmetic<ArithmeticType>::value, negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>>::type operator/(const ArithmeticType& u, const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& v)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> negatable_type;

    return negatable_type(u) /= v;
  }

  //! Implementations of non-member mixed-math binary add, sub, mul, div of (any_negatable op any_other_negatable).
  //! This includes non-member binary add, sub, mul, div of (negatable op negatable).
  template <const int IntegralRange1, const int FractionalResolution1,
            const int IntegralRange2, const int FractionalResolution2,
            typename RoundMode,
            typename OverflowMode>
  negatable<((-FractionalResolution1 > -FractionalResolution2) ? IntegralRange1 : IntegralRange2),
            ((-FractionalResolution1 > -FractionalResolution2) ? FractionalResolution1 : FractionalResolution2),
            RoundMode,
            OverflowMode>
  operator+(const negatable<IntegralRange1, FractionalResolution1, RoundMode, OverflowMode>& a,
            const negatable<IntegralRange2, FractionalResolution2, RoundMode, OverflowMode>& b)
  {
    typedef negatable<((-FractionalResolution1 > -FractionalResolution2) ? IntegralRange1        : IntegralRange2),
                      ((-FractionalResolution1 > -FractionalResolution2) ? FractionalResolution1 : FractionalResolution2),
                      RoundMode,
                      OverflowMode>
    negatable_type;

    return negatable_type(a) += b;
  }

  template <const int IntegralRange1, const int FractionalResolution1,
            const int IntegralRange2, const int FractionalResolution2,
            typename RoundMode,
            typename OverflowMode>
  negatable<((-FractionalResolution1 > -FractionalResolution2) ? IntegralRange1        : IntegralRange2),
            ((-FractionalResolution1 > -FractionalResolution2) ? FractionalResolution1 : FractionalResolution2),
            RoundMode,
            OverflowMode>
  operator-(const negatable<IntegralRange1, FractionalResolution1, RoundMode, OverflowMode>& a,
            const negatable<IntegralRange2, FractionalResolution2, RoundMode, OverflowMode>& b)
  {
    typedef negatable<((-FractionalResolution1 > -FractionalResolution2) ? IntegralRange1        : IntegralRange2),
                      ((-FractionalResolution1 > -FractionalResolution2) ? FractionalResolution1 : FractionalResolution2),
                      RoundMode,
                      OverflowMode>
    negatable_type;

    return negatable_type(a) -= b;
  }

  template <const int IntegralRange1, const int FractionalResolution1,
            const int IntegralRange2, const int FractionalResolution2,
            typename RoundMode,
            typename OverflowMode>
  negatable<((-FractionalResolution1 > -FractionalResolution2) ? IntegralRange1        : IntegralRange2),
            ((-FractionalResolution1 > -FractionalResolution2) ? FractionalResolution1 : FractionalResolution2),
            RoundMode,
            OverflowMode>
  operator*(const negatable<IntegralRange1, FractionalResolution1, RoundMode, OverflowMode>& a,
            const negatable<IntegralRange2, FractionalResolution2, RoundMode, OverflowMode>& b)
  {
    typedef negatable<((-FractionalResolution1 > -FractionalResolution2) ? IntegralRange1        : IntegralRange2),
                      ((-FractionalResolution1 > -FractionalResolution2) ? FractionalResolution1 : FractionalResolution2),
                      RoundMode,
                      OverflowMode>
    negatable_type;

    return negatable_type(a) *= b;
  }

  template <const int IntegralRange1, const int FractionalResolution1,
            const int IntegralRange2, const int FractionalResolution2,
            typename RoundMode,
            typename OverflowMode>
  negatable<((-FractionalResolution1 > -FractionalResolution2) ? IntegralRange1        : IntegralRange2),
            ((-FractionalResolution1 > -FractionalResolution2) ? FractionalResolution1 : FractionalResolution2),
            RoundMode,
            OverflowMode>
  operator/(const negatable<IntegralRange1, FractionalResolution1, RoundMode, OverflowMode>& a,
            const negatable<IntegralRange2, FractionalResolution2, RoundMode, OverflowMode>& b)
  {
    typedef negatable<((-FractionalResolution1 > -FractionalResolution2) ? IntegralRange1        : IntegralRange2),
                      ((-FractionalResolution1 > -FractionalResolution2) ? FractionalResolution1 : FractionalResolution2),
                      RoundMode,
                      OverflowMode>
    negatable_type;

    return negatable_type(a) /= b;
  }

  #if !defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM)

    /*! @c std::ostream output @c operator\<\< \n
        Send a fixed-point number to the output stream by first
        expressing the fixed-point number as a floating-point number.\n

        \note Macro BOOST_FIXED_POINT_DISABLE_IOSTREAM can be defined to 
        disable all I/O streaming and the inclusion of associated standard
        library headers. This is intended to eliminate I/O stream
        overhead in particular for bare-metal microcontroller projects.
    */
    template<typename char_type, typename traits_type, const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
    std::basic_ostream<char_type, traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& out,
                                                           const negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& x)
    {
      typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> negatable_type;

      typedef typename negatable_type::float_type local_float_type;

      // Send a fixed-point number to the output stream.
      // Express the fixed-point number as a floating-point representation.
      std::basic_ostringstream<char_type, traits_type> ostr;

      ostr.flags    (out.flags());
      ostr.imbue    (out.getloc());
      ostr.precision(out.precision());

      static_cast<void>(ostr << x.template convert_to_floating_point_type<local_float_type>());

      return (out << ostr.str());
    }

    /*! @c std::istream input @c operator>>
        Receive a floating-point number from the input stream.
        Subsequently make a fixed-point object from it.
    */
    template<typename char_type, typename traits_type, const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
    std::basic_istream<char_type, traits_type>& operator>>(std::basic_istream<char_type, traits_type>& in,
                                                           negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>& x)
    {
      typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> negatable_type;

      typedef typename negatable_type::float_type local_float_type;

      local_float_type v;

      // Receive a floating-point number from the input stream.
      static_cast<void>(in >> v);

      // Subsequently make a fixed-point object from it.
      x.data = negatable_type::make_from_floating_point_type(v);

      return in;
    }

  #endif // !BOOST_FIXED_POINT_DISABLE_IOSTREAM

  } } // namespace boost::fixed_point

  // Here we include headers for negatable <cmath> functions.
  #include <boost/fixed_point/detail/fixed_point_detail_hypergeometric.hpp>
  #include <boost/fixed_point/fixed_point_negatable_cmath.hpp>

  // Here we include specializations of std::numeric_limits<negatable>.
  #include <boost/fixed_point/fixed_point_negatable_limits.hpp>

#endif // FIXED_POINT_NEGATABLE_2015_03_06_HPP_
