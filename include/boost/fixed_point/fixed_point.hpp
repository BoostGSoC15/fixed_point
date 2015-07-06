///////////////////////////////////////////////////////////////////////////////
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Copyright Christopher Kormanyos 2014 - 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

// This file is a partial reference implementation for the proposed
// "C++ binary fixed-point arithmetic" as specified in N3352.
// See: http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2012/n3352.html

#ifndef FIXED_POINT_2015_03_06_HPP_
  #define FIXED_POINT_2015_03_06_HPP_

  // There is optional support for certain variations of fixed_point
  // using preprocessor definitions. Not all of these are supported
  // at the moment. The options include:

  //#define BOOST_FIXED_POINT_DISABLE_IOSTREAM
  //#define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION
  //#define BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH
  //#define BOOST_FIXED_POINT_DISABLE_CPP11

  // With BOOST_FIXED_POINT_DISABLE_IOSTREAM, we disable
  // all I/O streaming and the inclusion of associated standard
  // library headers. This is intended to eliminate I/O stream
  // overhead in particular for bare-metal microcontroller projects.
  // Disabling I/O streaming requires simultaneous disabling
  // of multiprecision.

  // With BOOST_FIXED_POINT_DISABLE_MULTIPRECISION,
  // we disable the use of Boost.Multiprecision
  // for back-ends of the fixed-point classes.

  // With BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH,
  // we avoid using the unsigned_large_type. This option
  // is intended for systems with limited integer widths
  // such as bare-metal microcontrollers.
  // When used in combination with BOOST_FIXED_POINT_DISABLE_MULTIPRECISION,
  // the this option is intended to provide fixed-point representations
  // with up to 64-bits (if 64-bit integral types are available)
  // without requiring any of Boost.Multiprecision.

  // With BOOST_FIXED_POINT_DISABLE_CPP11, we support
  // an optional back-port to C++03 and eliminate
  // the use of all C++11 language elements. This might send the
  // wrong message about language technology, but could potentially
  // increase the range of potential target compilers (especially
  // for embedded systems).

  #if defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM)

    // When I/O streaming is disabled:
    //   * We must eliminate Boost.Multiprecision.
    //   * We must eliminate Boost.Math.Constants (because these require istreaming).
    //   * We must eliminate the parts of Boost.Cstdfloat that require I/O streams.

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

  #include <boost/fixed_point/fixed_point_detail.hpp>

  static_assert(std::numeric_limits<boost::uint8_t>::digits  ==  8, "Configuration error: the size of boost::uint8_t  must be 8  bits");
  static_assert(std::numeric_limits<boost::uint16_t>::digits == 16, "Configuration error: the size of boost::uint16_t must be 16 bits");
  static_assert(std::numeric_limits<boost::uint32_t>::digits == 32, "Configuration error: the size of boost::uint32_t must be 32 bits");
  static_assert(std::numeric_limits<boost::uint64_t>::digits == 64, "Configuration error: the size of boost::uint64_t must be 64 bits");

  namespace boost { namespace fixed_point {

  namespace round
  { // Care - Doxygen is really picky about layout here - don't try to tidy it up!
    struct fastest      { }; //!< Template parameter for fixed_point negatable.\n Speed is more important than the choice in value.
    struct negative     { }; //!< Template parameter for fixed_point negatable.\n Round towards negative infinity. This mode is useful in interval arithmetic.
    struct truncated    { }; //!< Template parameter for fixed_point negatable.\n Template parameter for fixed_point negatableRound towards zero. This mode is useful in implementing integral arithmetic.
    struct positive     { }; //!< Template parameter for fixed_point negatable.\n Round towards positive infinity. This mode is useful in interval arithmetic.
    struct classic      { }; //!< Template parameter for fixed_point negatable.\n Template parameter for fixed_point negatableRound towards the nearest value, but exactly-half values are rounded towards maximum magnitude. This mode is the standard school algorithm.
    struct nearest_even { }; //!< Template parameter for fixed_point negatable.\n Round towards the nearest value, but exactly-half values are rounded towards even values. This mode has more balance than the classic mode.
    struct nearest_odd  { }; //!< Template parameter for fixed_point negatable.\n Round towards the nearest value, but exactly-half values are rounded towards odd values. This mode has as much balance as the near_even mode, but preserves more information.
  }

  // See also ISO/IEC 14882:2011 Paragraph 18.3.2.5.

  // 18.3.2.5 Type float_round_style
  //
  // namespace std
  // {
  //   enum float_round_style
  //   {
  //     round_indeterminate       = -1, : Potential interpretation: fastest
  //     round_toward_zero         =  0, : Potential interpretation: truncated (Is this the same as fastest?)
  //     round_to_nearest          =  1, : Potential interpretation: nearest_even
  //     round_toward_infinity     =  2, : Potential interpretation: positive (toward positive infinity)
  //     round_toward_neg_infinity =  3  : Potential interpretation: negative (toward negative infinity)
  //   };
  // }

  namespace overflow
  {
    struct impossible   { }; //!< Template parameter for fixed_point negatable.\n Programmer analysis of the program has determined that overflow cannot occur. Uses of this mode should be accompanied by an argument supporting the conclusion.
    struct undefined    { }; //!< Template parameter for fixed_point negatable.\n Programmers are willing to accept undefined behavior in the event of an overflow.
    struct modulus      { }; //!< Template parameter for fixed_point negatable.\n The assigned value is the dynamic value mod the range of the variable. This mode makes sense only with unsigned numbers. It is useful for angular measures.
    struct saturate     { }; //!< Template parameter for fixed_point negatable.\n If the dynamic value exceeds the range of the variable, assign the nearest representable value.
    struct exception    { }; //!< Template parameter for fixed_point negatable.\n If the dynamic value exceeds the range of the variable, throw an exeception of type `std::overflow_error`.
  }
  } } // namespace boost::fixed_point

  namespace boost { namespace fixed_point {

    // Forward declaration of the negatable class.
    template<const int IntegralRange,
             const int FractionalResolution,
             typename RoundMode,
             typename OverflowMode>
    class negatable;

    // What follows are forward declarations of elementary transcendental functions.

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
                                                   OverflowMode> x, int* expptr);

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
                                                   OverflowMode> x, int exp);

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

  } } // namespace boost::fixed_point

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
    \brief Fixed_point class used for signed fractional arithmetic.
    \details TODO  some examples here?
    \tparam IntegralRange  Integer g  >= 0 defines a range of sign number n that is 2^-g < n < 2^g.
    \tparam FractionalResolution Integer s <= -1 defines resolution. The resolution of a fractional number is 2^s.
    \tparam RoundMode struct defining the rounding behaviour, default round::fastest.
    \tparam OverflowMode struct defining the behaviour from rounding, default overflow::undefined.
  */

  template<const int IntegralRange,
           const int FractionalResolution,
           typename RoundMode = round::fastest,
           typename OverflowMode = overflow::undefined>
  class negatable
  {
  private:
    static BOOST_CONSTEXPR_OR_CONST int radix_split  = -FractionalResolution;
    static BOOST_CONSTEXPR_OR_CONST int digits_total = (IntegralRange + 1) - FractionalResolution;

    // TBD: Is this limitation correct?
    // TBD: Or are pure integer instantiations of negatable allowed?
    static_assert(FractionalResolution < 0,
                  "Error: The fractional resolution of negatable must be negative and include at least 1 fractional bit.");

    // TBD: Is this limitation correct?
    // TBD: Or can negatable have a negative integral range?
    static_assert(IntegralRange >= 0,
                  "Error: The integral range of negatable must be 0 or more.");

    static_assert(   std::is_same<RoundMode, round::fastest>::value
                  || std::is_same<RoundMode, round::nearest_even>::value,
                  "Error: Only fastest and nearest_even round modes are supported at the moment.");

    static_assert(   std::is_same<OverflowMode, overflow::undefined>::value,
                  "Error: Only undefined overflow mode is supported at the moment.");

  #if defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)
    static_assert(digits_total <= 32, "Error: the width of fixed_point can not exceed 32 bits when multiprecision is disabled.");
  #endif

  #if !defined(BOOST_FLOAT64_C)
    static_assert(digits_total <= 24, "Error: the width of fixed_point can not exceed 24 bits when float64_t is unavailable.");
  #endif

  public:

    // Friend forward declaration of another negatable class
    // with different template parameters.

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution,
             typename OtherRoundMode,
             typename OtherOverflowMode>
    friend class negatable;

    // Here we declare two convenient class-local type definitions.
    //
    //   * value_type : is the signed integer representation of the fixed-point
    //                  negatable number. For low digit counts, this will be
    //                  a built-in type such as int8_t, int16_t, int32_t, etc.
    //                  For larger digit counts, this will be a multiprecision
    //                  signed integer type.
    //   * float_type : is the floating-point type that is guaranteed to be wide
    //                  enough to represent the fixed-point negatable number
    //                  in its entirety. For low digit counts, this will be
    //                  a built-in type such as float, double or long double.
    //                  For larger digit counts, this will be a multiprecision
    //                  floating point type.
    //

    typedef typename detail::integer_type_helper<negatable::digits_total - 0>::exact_signed_type value_type;
    typedef typename detail::float_type_helper  <negatable::digits_total - 1>::exact_float_type  float_type;

    // The public class constructors follow below.

    // The class default constructor is implemented below.
    // By design choice, the default constructor clears
    // the data member.
    negatable() : data() { }

    // Here are the class constructors from built-in unsigned integral types.
    template<typename IntegralType>
    negatable(const IntegralType& u,
              const typename std::enable_if<(   std::is_integral<IntegralType>::value
                                             && std::is_unsigned<IntegralType>::value)>::type* = nullptr)
      : data(value_type(unsigned_small_type(unsigned_small_type(u) << radix_split) & unsigned_small_mask())) { }

    // Here are the class constructors from both built-in signed integral
    // types as well as from the internal value_type of the data member.
    template<typename IntegralType>
    negatable(const IntegralType& n,
              const typename std::enable_if<  (   std::is_integral<IntegralType>::value
                                               && std::is_signed  <IntegralType>::value)
                                            || std::is_same<IntegralType, value_type>::value>::type* = nullptr)
      : data((!(n < 0)) ? +value_type(unsigned_small_type(unsigned_small_type(+n) << radix_split) & unsigned_small_mask())
                        : -value_type(unsigned_small_type(unsigned_small_type(-n) << radix_split) & unsigned_small_mask())) { }

    // Here are the class constructors from built-in floating-point types.
    template<typename FloatingPointType>
    negatable(const FloatingPointType& f,
              const typename std::enable_if<   std::is_floating_point<FloatingPointType>::value
                                            || std::is_same<FloatingPointType, float_type>::value>::type* = nullptr)
      : data()
    {
      make_from_floating_point_type(f);
    }

    // Here is the class copy constructor.
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

    // Here is the mixed class constructor for case 1).
    // There is less range and less resolution in the other type.
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution,
             typename std::enable_if<   ( OtherIntegralRange        <=  IntegralRange)
                                     && (-OtherFractionalResolution <= -FractionalResolution)>::type* = nullptr>
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

      const unsigned_small_type u(u_superior);

      data = ((!is_neg) ? value_type(u) : -value_type(u));
    }

    // Here is the mixed class constructor for case 2).
    // There is more range and less resolution in the other type.
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution,
             typename std::enable_if<   ( OtherIntegralRange        >   IntegralRange)
                                     && (-OtherFractionalResolution <= -FractionalResolution)>::type* = nullptr>
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

      u_superior = (u_superior & superior_unsigned_small_type(unsigned_small_mask()));

      unsigned_small_type u_round = static_cast<unsigned_small_type>(u_superior);

      data = ((!is_neg) ? value_type(u_round) : -value_type(u_round));
    }

    // Here is the mixed class constructor for case 3).
    // There is less range and more resolution in the other type.
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution,
             typename std::enable_if<   ( OtherIntegralRange        <=  IntegralRange)
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

    // Here is the mixed class constructor for case 4).
    // There is more range and more resolution in the other type.
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution,
             typename std::enable_if<   ( OtherIntegralRange        >  IntegralRange)
                                     && (-OtherFractionalResolution > -FractionalResolution)>::type* = nullptr>
    negatable(const negatable<OtherIntegralRange,
                              OtherFractionalResolution,
                              RoundMode,
                              OverflowMode>& other) : data()
    {
      // TBD: The contents of this constructor need to be written.
    }

    ~negatable() { }

    negatable& operator=(const negatable& other)
    {
      if(this != (&other))
      {
        data = other.data;
      }

      return *this;
    }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    negatable& operator=(const negatable<OtherIntegralRange,
                                         OtherFractionalResolution,
                                         RoundMode,
                                         OverflowMode>& other)
    {
      // Here, we are equating to another negatable type with different
      // range and/or resolution paramters than *this.

      // Use a relatively lazy method that creates an intermediate
      // temporary object. This temporary object is subsequently used
      // to initialize the data field of *this.

      const negatable tmp(other);

      data = tmp.data;

      return *this;
    }

    // Equality operators for built-in integral types.
    negatable& operator=(const char& n)               { data = value_type(n) << radix_split; return *this; }
    negatable& operator=(const short& n)              { data = value_type(n) << radix_split; return *this; }
    negatable& operator=(const int& n)                { data = value_type(n) << radix_split; return *this; }
    negatable& operator=(const long& n)               { data = value_type(n) << radix_split; return *this; }
    negatable& operator=(const long long& n)          { data = value_type(n) << radix_split; return *this; }

    negatable& operator=(const unsigned char& u)      { data = value_type(unsigned_small_type(u) << radix_split); return *this; }
    negatable& operator=(const unsigned short& u)     { data = value_type(unsigned_small_type(u) << radix_split); return *this; }
    negatable& operator=(const unsigned int& u)       { data = value_type(unsigned_small_type(u) << radix_split); return *this; }
    negatable& operator=(const unsigned long& u)      { data = value_type(unsigned_small_type(u) << radix_split); return *this; }
    negatable& operator=(const unsigned long long& u) { data = value_type(unsigned_small_type(u) << radix_split); return *this; }

    // Equality operators for built-in floating-point types.
    negatable& operator=(const float& f)              { make_from_floating_point_type(f);  return *this; }
    negatable& operator=(const double& d)             { make_from_floating_point_type(d);  return *this; }
    negatable& operator=(const long double& ld)       { make_from_floating_point_type(ld); return *this; }

    // Unary pre-increment and post-increment operators.
    negatable& operator++()   { data += value_type(unsigned_small_type(1) << radix_split); return *this; }
    negatable& operator--()   { data -= value_type(unsigned_small_type(1) << radix_split); return *this; }

    negatable operator++(int) { const negatable tmp(*this); data += value_type(unsigned_small_type(1) << radix_split); return tmp; }
    negatable operator--(int) { const negatable tmp(*this); data -= value_type(unsigned_small_type(1) << radix_split); return tmp; }

    // Unary operators add and subtract of negatable with negatable.
    negatable& operator+=(const negatable& v) { data += v.data; return *this; }
    negatable& operator-=(const negatable& v) { data -= v.data; return *this; }

    // Unary operator multiply of negatable with negatable.
    negatable& operator*=(const negatable& v)
    {
      const bool u_is_neg      = (  data < 0);
      const bool v_is_neg      = (v.data < 0);
      const bool result_is_neg = (u_is_neg != v_is_neg);

      // Multiplication will be carried out using unsigned integers.

      // Multiplication uses with a relatively lazy method.
      // The result is first placed in unsigned_large_type,
      // which is twice as wide as unsigned_small_type.

      unsigned_large_type result((!u_is_neg) ? data : -data);

      result *= unsigned_large_type((!v_is_neg) ? unsigned_large_type(v.data) : unsigned_large_type(-v.data));

      // Scale the result of the multiplication to fit once again
      // in the fixed-point data field.

      // Here, we use 1 extra binary digit for rounding.
      // The extra rounding digit fits in unsigned_small_type
      // because the value_type (even though just as wide as
      // unsigned_small_type) reserves one bit for the sign.

      const int total_right_shift = radix_split - 1;

      unsigned_small_type u_round(static_cast<unsigned_small_type>(detail::right_shift_helper(result, total_right_shift)));

      // Round the result of the multiplication.
      const boost::int_fast8_t rounding_result = binary_round(u_round);

      // Add or subtract the result of the rounding (-1, 0, or +1).
      if     (rounding_result == INT8_C(+1)) { ++u_round; }
      else if(rounding_result == INT8_C(-1)) { --u_round; }

      u_round = (u_round & unsigned_small_mask());

      // Load the fixed-point result and account for potentially signed values.
      data = value_type((!result_is_neg) ? value_type(u_round) : -value_type(u_round));

      return *this;
    }

    // Unary operator divide of negatable with negatable.
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

        // Division uses with a relatively lazy method.
        // The result is first placed in unsigned_large_type,
        // which is twice as wide as unsigned_small_type.

        // Hereby, we scale the result of the division to a larger value
        // so that integer division is straightforward and simple,
        // although potentially costly for higher digit counts.

        unsigned_large_type result((!u_is_neg) ? unsigned_large_type(data) : unsigned_large_type(-data));

        // Here, we use 1 extra binary digit for rounding.
        // The extra rounding digit fits in unsigned_small_type
        // because the value_type (even though just s wide as
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

        // Load the fixed-point result and account for potentially signed values.
        data = value_type((!result_is_neg) ? value_type(u_round) : -value_type(u_round));
      }

      return *this;
    }

    // Unary operators add, sub, mul, div of negatable with an arithmetic built-in type.

    template<typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type> negatable& operator+=(ArithmeticType& n) { return (*this) += negatable(n); }
    template<typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type> negatable& operator-=(ArithmeticType& n) { return (*this) -= negatable(n); }
    template<typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type> negatable& operator*=(ArithmeticType& n) { return (*this) *= negatable(n); }
    template<typename ArithmeticType, typename = typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type> negatable& operator/=(ArithmeticType& n) { return (*this) /= negatable(n); }

    // Here are the cast operators for built-in signed and unsigned integral types.

    // Note: Cast from negatable to a built-in integral type truncates
    // the fractional part without rounding. This is consistent with
    // conversion from built-in floasting-point types to built-in
    // integral types as described in ISO/IEC 14882:2011 paragraph 4.9.1.

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
      // This function is used primarily for debugging and testing purposes
      // TBD: Perhaps this function should not be exposed to users?
      std::string bit_pattern()
      {
        value_type num = this->data;
        std::string ans;
        value_type mask(1);
        for(int i = 0; i < digits_total; i++)
        {
          if(num & mask)
          {
            ans += "1";
          }
          else ans += "0";
          mask <<= 1;
        }
        std::reverse(ans.begin(), ans.end());
        return ans;
      }
    #endif // !BOOST_FIXED_POINT_DISABLE_IOSTREAM

    // Return range and resolution to the user.
    static int get_range     () { return IntegralRange; }
    static int get_resolution() { return FractionalResolution; }

  private:
    value_type data;

    typedef typename detail::integer_type_helper<negatable::digits_total * 1>::exact_unsigned_type unsigned_small_type;
    typedef typename detail::integer_type_helper<negatable::digits_total * 2>::exact_unsigned_type unsigned_large_type;

    static const unsigned_small_type& unsigned_small_mask() BOOST_NOEXCEPT
    {
      static const unsigned_small_type the_value =
        detail::bit_mask_helper<unsigned_small_type,
                                0U,
                                unsigned(IntegralRange - FractionalResolution)>::value();

      return the_value;
    }

    struct nothing { };

    /*! \tparam IntegralType Integer type on which the fixed-point type is based, typically the native unsigned integer type unsigned int,
    but can be a smaller fundamental type like short int, or a much longer type like boost::multiprecision::cpp_int.
    \sa http://www.boost.org/doc/libs/release/libs/multiprecision/doc/html/boost_multiprecision/tut/ints/cpp_int.html
    */
    template<typename IntegralType>
    negatable(const nothing&,
              const IntegralType& n,
              const typename std::enable_if<   std::is_integral<IntegralType>::value
                                            || std::is_same<typename negatable::value_type, IntegralType>::value
                                            || std::is_same<typename negatable::unsigned_small_type, IntegralType>::value>::type* = nullptr) : data(static_cast<value_type>(n)) { }

    template<typename LocalRoundMode = RoundMode>
    static boost::int_fast8_t
      binary_round(unsigned_small_type& u_round,
                   typename std::enable_if<std::is_same<LocalRoundMode, round::fastest>::value>::type* = nullptr)
    {
      /*! Here, u_round contains the value to be rounded whereby
       this value is left-shifted one binary digit larger than
       the final result will be.

       Perform the rounding algorithm for round::fastest.
       For round::fastest, there is simply no rounding at all.
       The value is truncated.
     */

      u_round = (u_round >> 1);

      return INT8_C(0);
    }

    template<typename LocalRoundMode = RoundMode>
    static boost::int_fast8_t
      binary_round(unsigned_small_type& u_round,
                   typename std::enable_if<std::is_same<LocalRoundMode, round::nearest_even>::value>::type* = nullptr)
    {
      /*! Here, u_round contains the value to be rounded whereby
       this value is left-shifted one binary digit larger than
       the final result will be.

       Perform the rounding algorithm for round::nearest_even.
       For round::nearest_even, the value is rounded to larger
       absolute value when both 1/2-ULP as well as 1-ULP are 1,
       representing round odd 1-ULP to higher value.
     */

      const bool round_up =   ((boost::uint_fast8_t(u_round & UINT8_C(1)) == UINT8_C(1))
                            && (boost::uint_fast8_t(u_round & UINT8_C(2)) == UINT8_C(2)));

      u_round = (u_round >> 1);

      return (round_up ? INT8_C(1) : INT8_C(0));
    }

    template<typename FloatingPointType>
    FloatingPointType convert_to_floating_point_type() const
    {
      const bool is_neg = (data < static_cast<value_type>(0));

      FloatingPointType f(0);

      // Convert the fixed_point value to FloatingPointType result.
      // The fixed_point value is converted with brute force,
      // using one bit at a time.

      // TBD: The conversion uses brute force with an inefficient loop.
      // Can (should) this mechanism be optimized?

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
      int exp;
      const FloatingPointType fp(frexp((!is_neg) ? f : -f, &exp));

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
      // because the value_type (even though just s wide as
      // unsigned_small_type) reserves one bit for the sign.

      const int total_left_shift =   (radix_split + exp)
                                   - (std::numeric_limits<FloatingPointType>::digits);

      const local_unsigned_small_type u_round_local(detail::left_shift_helper(u, total_left_shift));

      unsigned_small_type u_round = static_cast<unsigned_small_type>(u_round_local);

      // Round the result of the construction from FloatingPointType.
      const boost::int_fast8_t rounding_result = binary_round(u_round);

      // Add or subtract the result of the rounding (-1, 0, or +1).
      if     (rounding_result == INT8_C(+1)) { ++u_round; }
      else if(rounding_result == INT8_C(-1)) { --u_round; }

      u_round = (u_round & unsigned_small_mask());

      // Load the fixed-point result and account for potentially signed values.
      data = value_type((!is_neg) ? value_type(u_round) : -value_type(u_round));
    }

    template<typename ArithmeticType>
    static const ArithmeticType& radix_split_value()
    {
      static const ArithmeticType the_radix_split_value(detail::radix_split_maker<ArithmeticType, radix_split>::value());

      return the_radix_split_value;
    }

    static const negatable& epsilon_maker() BOOST_NOEXCEPT
    {
      static bool is_init = bool();

      negatable local_epsilon = negatable();

      if(is_init == false)
      {
        is_init = true;

        value_type r10  = radix_split_value<value_type>();

        if(r10 <= 10)
        {
          local_epsilon = negatable(nothing(), UINT8_C(1));
        }
        else
        {
          while(r10 > 10)
          {
            r10 = (r10 + 9) / 10;
          }

          local_epsilon = negatable(nothing(), r10);
        }
      }

      static const negatable the_epsilon(local_epsilon);

      return the_epsilon;
    }

    static const negatable& value_min() BOOST_NOEXCEPT { static const negatable the_value_min(nothing(), 1U); return the_value_min; }
    static const negatable& value_max() BOOST_NOEXCEPT { static const negatable the_value_max(nothing(), unsigned_small_mask()); return the_value_max; }

    friend class std::numeric_limits<negatable>;

    // Disable all I/O streaming and the inclusion of associated standard
    // library headers. This is intended to eliminate I/O stream
    // overhead in particular for bare-metal microcontroller projects.
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
      }

      template<typename char_type,
               typename traits_type>
      friend inline std::basic_istream<char_type,
                                       traits_type>& operator>>(std::basic_istream<char_type, traits_type>& in,
                                                                negatable& x)
      {
        // Receive a floating-point number from the input stream.
        // Subsequently make a fixed-point object from it.

        float_type v;

        static_cast<void>(in >> v);

        x.make_from_floating_point_type(v);

        return in;
      }

    #endif // !BOOST_FIXED_POINT_DISABLE_IOSTREAM

    // Implementations of global unary plus and minus.
    friend inline negatable operator+(const negatable& self) { return negatable(self); }
    friend inline negatable operator-(const negatable& self) { negatable tmp(self); tmp.data = -tmp.data; return tmp; }

    // Implementations of global binary add, sub, mul, div of [lhs(negatable)] operator [rhs(negatable)].
    friend inline negatable operator+(const negatable& u, const negatable& v) { return negatable(u) += v; }
    friend inline negatable operator-(const negatable& u, const negatable& v) { return negatable(u) -= v; }
    friend inline negatable operator*(const negatable& u, const negatable& v) { return negatable(u) *= v; }
    friend inline negatable operator/(const negatable& u, const negatable& v) { return negatable(u) /= v; }

    // Implementations of global binary add, sub, mul, div of [lhs(negatable)] operator [rhs(arithmetic_type)].
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator+(const negatable& u, const ArithmeticType& v) { return negatable(u) += v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator-(const negatable& u, const ArithmeticType& v) { return negatable(u) -= v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator*(const negatable& u, const ArithmeticType& v) { return negatable(u) *= v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator/(const negatable& u, const ArithmeticType& v) { return negatable(u) /= v; }

    // Implementations of global binary add, sub, mul, div of [lhs(arithmetic_type)] operator [rhs(negatable)].
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator+(const ArithmeticType& u, const negatable& v) { return negatable(u) += v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator-(const ArithmeticType& u, const negatable& v) { return negatable(u) -= v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator*(const ArithmeticType& u, const negatable& v) { return negatable(u) *= v; }
    template<typename ArithmeticType, typename std::enable_if<std::is_arithmetic<ArithmeticType>::value>::type* = nullptr> friend inline negatable operator/(const ArithmeticType& u, const negatable& v) { return negatable(u) /= v; }

    // Implementations of global binary add, sub, mul, div of [lhs(negatable)] operator [rhs(other_negatable)].
    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline negatable operator+(const negatable& u, const negatable<OtherIntegralRange,
                                                                          OtherFractionalResolution,
                                                                          RoundMode,
                                                                          OverflowMode>& v)
    {
      return negatable(u) += v;
    }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline negatable operator-(const negatable& u, const negatable<OtherIntegralRange,
                                                                          OtherFractionalResolution,
                                                                          RoundMode,
                                                                          OverflowMode>& v)
    {
      return negatable(u) -= v;
    }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline negatable operator*(const negatable& u, const negatable<OtherIntegralRange,
                                                                          OtherFractionalResolution,
                                                                          RoundMode,
                                                                          OverflowMode>& v)
    {
      return negatable(u) *= v;
    }

    template<const int OtherIntegralRange,
             const int OtherFractionalResolution>
    friend inline negatable operator/(const negatable& u, const negatable<OtherIntegralRange,
                                                                          OtherFractionalResolution,
                                                                          RoundMode,
                                                                          OverflowMode>& v)
    {
      return negatable(u) /= v;
    }

    // Implementations of global equality.
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
      typedef negatable<( IntegralRange        >  OtherIntegralRange)        ? IntegralRange : OtherIntegralRange,
                        (-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution,
                        RoundMode,
                        OverflowMode> supra_fixed_point_type;

      return (supra_fixed_point_type(u) == supra_fixed_point_type(v));
    }

    // Implementations of global inequality.
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
      typedef negatable<( IntegralRange        >  OtherIntegralRange)        ? IntegralRange : OtherIntegralRange,
                        (-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution,
                        RoundMode,
                        OverflowMode> supra_fixed_point_type;

      return (supra_fixed_point_type(u) != supra_fixed_point_type(v));
    }

    // Implementations of global operators >, <, >=, <=.
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
      typedef negatable<( IntegralRange        >  OtherIntegralRange)        ? IntegralRange : OtherIntegralRange,
                        (-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution,
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
      typedef negatable<( IntegralRange        >  OtherIntegralRange)        ? IntegralRange : OtherIntegralRange,
                        (-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution,
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
      typedef negatable<( IntegralRange        >  OtherIntegralRange)        ? IntegralRange : OtherIntegralRange,
                        (-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution,
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
      typedef negatable<( IntegralRange        >  OtherIntegralRange)        ? IntegralRange : OtherIntegralRange,
                        (-FractionalResolution > -OtherFractionalResolution) ? FractionalResolution : OtherFractionalResolution,
                        RoundMode,
                        OverflowMode> supra_fixed_point_type;

      return (supra_fixed_point_type(u) <= supra_fixed_point_type(v));
    }

    // Helper utilities for mathematical constants.
    // We need mathematical constants for transcendental functions.

    /*!
      Construct a constant of type negatable using values of constants from 
      <a href="http://www.boost.org/doc/libs/release/libs/math/doc/html/math_toolkit/constants.html">boost::math::constants</a>\n
      It is a private member of class negatable 
      because it is not normally needed by users who just want to @b call a function.
      It @b is required to add new constants.
      \tparam bit_count Precision in bits to create.
      \tparam enable_type Used internally to enable the most suitable version for the value of bit_count.
    */

    // Forward declaration (so that the above comments appear in Doxygen listing).
    template<const int BitCount,
             typename EnableType>
    struct constant_maker;

    //! \cond DETAIL
    template<const int BitCount,
             typename EnableType = void>
    struct constant_maker
    {
      /*!
        pi function using values of constants from boost::math::constants.
        \returns 3.14159265358979323846264338327950288419716939937510 to the precision specified by BitCount.
      */
      static const negatable& pi()
      {
        static_assert(IntegralRange >= 2,
                      "The constant pi can not be created with fewer than 2 binary integral range digits");

        #if !defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM) && !defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)
          static const negatable value_pi(boost::math::constants::pi<negatable>());
        #else
          static const negatable value_pi(3.14159265358979323846264338327950288419716939937510L);
        #endif

        return value_pi;
      }
      /*!
      loge(2) function  using values of constants from boost::math::constants.
      \returns ln(2) 0.693147180559945309417232121458.
      */

      static const negatable& ln_two()
      {
        #if !defined(BOOST_FIXED_POINT_DISABLE_IOSTREAM) && !defined(BOOST_FIXED_POINT_DISABLE_MULTIPRECISION)
          static const negatable value_ln_two(boost::math::constants::ln_two<negatable>());
        #else
          static const negatable value_ln_two(0.693147180559945309417232121458L);
        #endif

        return value_ln_two;
      }
    };

    template<const int BitCount>
    struct constant_maker<BitCount,
                          typename std::enable_if<(BitCount <= 8)>::type>
    {
      static const negatable& pi()
      {
        static_assert(IntegralRange >= 2,
                      "The constant pi can not be created with fewer than 2 binary integral range digits");

        static const negatable value_pi(nothing(), value_type((UINT8_C(0x64) + ((UINT8_C(1) << (5 + FractionalResolution)) / 2U)) >> (5 + FractionalResolution)));
        return value_pi;
      }

      static const negatable& ln_two()
      {
        static const negatable value_ln_two(nothing(), value_type((UINT8_C(0x58) + ((UINT8_C(1) << (7 + FractionalResolution)) / 2U)) >> (7 + FractionalResolution)));
        return value_ln_two;
      }
    };

    template<const int BitCount>
    struct constant_maker<BitCount,
                          typename std::enable_if<(BitCount > 8) && (BitCount <= 16)>::type>
    {
      static const negatable& pi()
      {
        static_assert(IntegralRange >= 2,
                      "The constant pi can not be created with fewer than 2 binary integral range digits");

        static const negatable value_pi(nothing(), value_type((UINT16_C(0x6487) + ((UINT16_C(1) << (13 + FractionalResolution)) / 2U)) >> (13 + FractionalResolution)));
        return value_pi;
      }

      static const negatable& ln_two()
      {
        static const negatable value_ln_two(nothing(), value_type((UINT16_C(0x58B9) + ((UINT16_C(1) << (15 + FractionalResolution)) / 2U)) >> (15 + FractionalResolution)));
        return value_ln_two;
      }
    };

    template<const int BitCount>
    struct constant_maker<BitCount,
                          typename std::enable_if<(BitCount > 16) && (BitCount <= 32)>::type>
    {
      static const negatable& pi()
      {
        static_assert(IntegralRange >= 2,
                      "The constant pi can not be created with fewer than 2 binary integral range digits");

        static const negatable value_pi(nothing(), value_type((UINT32_C(0x6487ED51) + ((UINT32_C(1) << (29 + FractionalResolution)) / 2U)) >> (29 + FractionalResolution)));
        return value_pi;
      }

      static const negatable& ln_two()
      {
        static const negatable value_ln_two(nothing(), value_type((UINT32_C(0x58B90BfB) + ((UINT32_C(1) << (31 + FractionalResolution)) / 2U)) >> (31 + FractionalResolution)));
        return value_ln_two;
      }
    };

    template<const int BitCount>
    struct constant_maker<BitCount,
                          typename std::enable_if<(BitCount > 32) && (BitCount <= 64)>::type>
    {
      static const negatable& pi()
      {
        static_assert(IntegralRange >= 2,
                      "The constant pi can not be created with fewer than 2 binary integral range digits");

        static const negatable value_pi(nothing(), value_type((UINT64_C(0x6487ED5110B4611A) + ((UINT64_C(1) << (61 + FractionalResolution)) / 2U)) >> (61 + FractionalResolution)));
        return value_pi;
      }

      static const negatable& ln_two()
      {
        static const negatable value_ln_two(nothing(), value_type((UINT64_C(0x58B90BFBE8E7BCD6) + ((UINT64_C(1) << (63 + FractionalResolution)) / 2U)) >> (63 + FractionalResolution)));
        return value_ln_two;
      }
    };

    //! \endcond  // DETAIL

    friend inline negatable  abs(negatable x) { return ((x.data < 0) ? -x : x); }
    friend inline negatable fabs(negatable x) { return ((x.data < 0) ? -x : x); }

    friend inline negatable frexp(negatable x, int* expptr)
    {
      *expptr = 0;

      if(x.data == 0)
      {
        return negatable(0);
      }
      else
      {
        const bool is_neg = (x.data < 0);

        unsigned_small_type result((!is_neg) ? unsigned_small_type(x.data) : unsigned_small_type(-x.data));

        while(result < detail::radix_split_maker<unsigned_small_type, radix_split - 1>::value())
        {
          result = (result << 1);

          --(*expptr);
        }

        while(result >= radix_split_value<unsigned_small_type>())
        {
          result = (result >> 1);

          ++(*expptr);
        }

        return negatable(nothing(), value_type((!is_neg) ?  value_type(result)
                                                         : -value_type(result)));
      }
    }

    friend inline negatable ldexp(negatable x, int exp)
    {
      if(exp > 0)
      {
        return negatable(nothing(), value_type(x.data << exp));
      }
      else if(exp < 0)
      {
        const bool is_neg = (x.data < 0);

        unsigned_small_type result((!is_neg) ? unsigned_small_type(x.data) : unsigned_small_type(-x.data));

        result = (result >> -exp);

        return negatable(nothing(), value_type((!is_neg) ? value_type(result) : -value_type(result)));
      }
      else
      {
        return x;
      }
    }

    friend inline negatable sqrt(negatable x)
    {
      // TBD: This implementation of square root seems to fail
      // for low digit counts.

      // TBD: This implementation of square root may be too inefficient
      // for low digit counts such as 5...16 digits. Consider optimization
      // for low digit counts.

      if(x.data <= 0)
      {
        return negatable(0);
      }
      else if(x.data < radix_split_value<value_type>())
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

        // TBD: Improve the accuracy of the initial estimate of the
        // square root and subsequently use a tighter tolerance
        // on the number of iterations in the Newton-Raphson loop.

        for(boost::uint_fast16_t i = UINT16_C(1); boost::uint_fast16_t(i / 2U) <= boost::uint_fast16_t(negatable::digits_total); i *= UINT16_C(2))
        {
          // Perform the next iteration of vi.
          vi += vi * (-((result * vi) * 2U) + 1U);

          // Perform the next iteration of the result.
          result += (vi * (-((result) * (result)) + x));
        }

        return result;
      }
    }
  };

  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> BOOST_CONSTEXPR_OR_CONST int negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::digits_total;
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode> BOOST_CONSTEXPR_OR_CONST int negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::radix_split;

  } } // namespace boost::fixed_point

  // Effectively inject all defined cmath functions into the global namespace.
  // So users can write sqrt(negatable) without requiring any namespace decoration.

  using boost::fixed_point::abs;
  using boost::fixed_point::fabs;
  using boost::fixed_point::frexp;
  using boost::fixed_point::ldexp;
  using boost::fixed_point::sqrt;

  // TODO Ensure that added functions are also listed in this section.

  namespace std
  {
    // Provide specializations of std::numeric_limits<negatable>.

    /*! \note Individual template specializations need to be provided
     for each different rounding mode and overflow mode.
     This might be 7 rounding * 5 overflow, a total of 35 specializations!
    */

    // Here is the template specialization of std::numeric_limits<negatable>
    // for round::fastest and overflow::undefined.
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
      static BOOST_CONSTEXPR_OR_CONST int                     digits            = negatable_type::digits_total - 1;
      static BOOST_CONSTEXPR_OR_CONST int                     digits10          = static_cast<int>((static_cast<boost::uintmax_t>(digits - 1) * UINTMAX_C(3010)) / UINTMAX_C(10000));
      static BOOST_CONSTEXPR_OR_CONST int                     max_digits10      = static_cast<int>((static_cast<boost::uintmax_t>(digits - 0) * UINTMAX_C(3010)) / UINTMAX_C(10000)) + 2;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_signed         = true;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_integer        = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_exact          = true;
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

    // Here is the template specialization of std::numeric_limits<negatable>
    // for round::nearest_even and overflow::undefined.
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
      static BOOST_CONSTEXPR_OR_CONST int                     digits            = negatable_type::digits_total - 1;
      static BOOST_CONSTEXPR_OR_CONST int                     digits10          = static_cast<int>((static_cast<boost::uintmax_t>(digits - 1) * UINTMAX_C(3010)) / UINTMAX_C(10000));
      static BOOST_CONSTEXPR_OR_CONST int                     max_digits10      = static_cast<int>((static_cast<boost::uintmax_t>(digits - 0) * UINTMAX_C(3010)) / UINTMAX_C(10000)) + 2;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_signed         = true;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_integer        = false;
      static BOOST_CONSTEXPR_OR_CONST bool                    is_exact          = true;
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

#endif // FIXED_POINT_2015_03_06_HPP_
