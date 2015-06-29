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

  #include <algorithm>
  #include <cmath>
  #include <iomanip>
  #include <istream>
  #include <limits>
  #include <ostream>
  #include <boost/fixed_point/fixed_point_detail.hpp>
  #include <boost/math/constants/constants.hpp>

  // TBD: Consider supporting certain options for fixed_point
  // with preprocessor definitions. Some ideas include:

  //#define BOOST_FIXED_POINT_DISABLE_MULTIPRECISION
  //#define BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH
  //#define BOOST_FIXED_POINT_DISABLE_IOSTREAM
  //#define BOOST_FIXED_POINT_DISABLE_CPP11

  // TBD: With BOOST_FIXED_POINT_DISABLE_MULTIPRECISION,
  // we would disable the use of Boost.Multiprecision
  // for back-ends of the fixed-point classes.

  // TBD: With BOOST_FIXED_POINT_DISABLE_WIDE_INTEGER_MATH,
  // we avoid using the unsigned_large_type. This option
  // is intended for systems with limited integer widths
  // such as bare-metal microcontrollers.
  // When used in combination with BOOST_FIXED_POINT_DISABLE_MULTIPRECISION,
  // the this option is intended to provide fixed-point representations
  // with up to 64-bits (if 64-bit integral types are available)
  // without requiring any of Boost.Multiprecision.

  // TBD: With BOOST_FIXED_POINT_DISABLE_IOSTREAM, we would disable
  // all I/O streaming and the inclusion of associated standard
  // library headers. This is intended to eliminate I/O stream
  // overhead in particular for bare-metal microcontroller projects.

  // TBD: With BOOST_FIXED_POINT_DISABLE_CPP11, we would decide
  // to support an optional back-port to C++03 and eliminate
  // the use of all C++11 language elements. This might send the
  // wrong message about language technology, but could potentially
  // increase the range of potential target compilers (especially
  // for embedded systems).

  namespace boost { namespace fixed_point {

  namespace round
  {
    struct fastest      { }; //!< Speed is more important than the choice in value.
    struct negative     { }; //!< Round towards negative infinity. This mode is useful in interval arithmetic.
    struct truncated    { }; //!< Round towards zero. This mode is useful in implementing integral arithmetic.
    struct positive     { }; //!< Round towards positive infinity. This mode is useful in interval arithmetic.
    struct classic      { }; // Round towards the nearest value, but exactly-half values are rounded towards maximum magnitude. This mode is the standard school algorithm.
    struct nearest_even { }; // Round towards the nearest value, but exactly-half values are rounded towards even values. This mode has more balance than the classic mode.
    struct nearest_odd  { }; // Round towards the nearest value, but exactly-half values are rounded towards odd values. This mode has as much balance as the near_even mode, but preserves more information.
  }

  namespace overflow
  {
    struct impossible   { }; // Programmer analysis of the program has determined that overflow cannot occur. Uses of this mode should be accompanied by an argument supporting the conclusion.
    struct undefined    { }; // Programmers are willing to accept undefined behavior in the event of an overflow.
    struct modulus      { }; // The assigned value is the dynamic value mod the range of the variable. This mode makes sense only with unsigned numbers. It is useful for angular measures.
    struct saturate     { }; // If the dynamic value exceeds the range of the variable, assign the nearest representable value.
    struct exception    { }; // If the dynamic value exceeds the range of the variable, throw an exeception of type std::overflow_error.
  }
  } } // namespace boost::fixed_point

  namespace boost { namespace fixed_point {

    // Forward declaration of the negatable class.
    template<const int integral_range,
             const int fractional_resolution,
             typename round_mode,
             typename overflow_mode>
    class negatable;

    // What follows are forward declarations of elementary transcendental functions.

    // Forward declaration of abs.
    template<const int integral_range,
             const int fractional_resolution,
             typename round_mode,
             typename overflow_mode>
    inline negatable<integral_range,
                     fractional_resolution,
                     round_mode,
                     overflow_mode> abs(negatable<integral_range,
                                                  fractional_resolution,
                                                  round_mode,
                                                  overflow_mode> x);

    // Forward declaration of fabs.
    template<const int integral_range,
             const int fractional_resolution,
             typename round_mode,
             typename overflow_mode>
    inline negatable<integral_range,
                     fractional_resolution,
                     round_mode,
                     overflow_mode> fabs(negatable<integral_range,
                                                   fractional_resolution,
                                                   round_mode,
                                                   overflow_mode> x);

    // Forward declaration of frexp.
    template<const int integral_range,
             const int fractional_resolution,
             typename round_mode,
             typename overflow_mode>
    inline negatable<integral_range,
                     fractional_resolution,
                     round_mode,
                     overflow_mode> frexp(negatable<integral_range,
                                                    fractional_resolution,
                                                    round_mode,
                                                    overflow_mode> x, int* expptr);

    // Forward declaration of ldexp.
    template<const int integral_range,
             const int fractional_resolution,
             typename round_mode,
             typename overflow_mode>
    inline negatable<integral_range,
                     fractional_resolution,
                     round_mode,
                     overflow_mode> ldexp(negatable<integral_range,
                                                    fractional_resolution,
                                                    round_mode,
                                                    overflow_mode> x, int exp);

    // Forward declaration of sqrt.
    template<const int integral_range,
             const int fractional_resolution,
             typename round_mode,
             typename overflow_mode>
    inline negatable<integral_range,
                     fractional_resolution,
                     round_mode,
                     overflow_mode> sqrt(negatable<integral_range,
                                                   fractional_resolution,
                                                   round_mode,
                                                   overflow_mode> x);

  } } // namespace boost::fixed_point

  namespace std
  {
    // Forward declaration of the specialization of std::numeric_limits<negatable>.
    template<const int integral_range,
             const int fractional_resolution,
             typename round_mode,
             typename overflow_mode>
    class numeric_limits<boost::fixed_point::negatable<integral_range,
                                                       fractional_resolution,
                                                       round_mode,
                                                       overflow_mode>>;
  }

  namespace boost { namespace fixed_point {

  // We will now begin the implementation of the negatable class.
  template<const int integral_range,
           const int fractional_resolution,
           typename round_mode = round::fastest,
           typename overflow_mode = overflow::undefined>
  class negatable
  {
  private:
    static BOOST_CONSTEXPR_OR_CONST int radix_split  = -fractional_resolution;
    static BOOST_CONSTEXPR_OR_CONST int digits_total = (integral_range + 1) - fractional_resolution;

    // TBD: Is this limitation correct?
    // TBD: Or are pure integer instantiations of negatable allowed?
    static_assert(fractional_resolution < 0,
                  "Error: The fractional resolution of negatable must be negative and include at least 1 fractional bit.");

    // TBD: Is this limitation correct?
    // TBD: Or can negatable have a negative integral range?
    static_assert(integral_range >= 0,
                  "Error: The integral range of negatable must be 0 or more.");

    static_assert(   std::is_same<round_mode, round::fastest>::value
                  || std::is_same<round_mode, round::nearest_even>::value,
                  "Error: Only fastest and nearest_even round modes are supported at the moment.");

    static_assert(   std::is_same<overflow_mode, overflow::undefined>::value,
                  "Error: Only undefined overflow mode is supported at the moment.");

  public:

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

    /*getter function for data to be used by mixed constructors*/
    // tbd: Is there a better way to expose 'data' to mixed constructors and ONLY to mixed constructors?
    const value_type& get_data () const {return data;}

    // The public class constructors follow below.

    // The class default constructor is implemented below.
    // By design choice, the default constructor clears
    // the data member.
    negatable() : data() { }

    // Here are the class constructors from built-in unsigned integral types.
    template<typename integral_type>
    negatable(const integral_type& u,
              const typename std::enable_if<(   std::is_integral<integral_type>::value
                                             && std::is_unsigned<integral_type>::value)>::type* = nullptr)
      : data(value_type(unsigned_small_type(unsigned_small_type(u) << radix_split) & unsigned_small_mask())) { }

    // Here are the class constructors from both built-in signed integral
    // types as well as from the internal value_type of the data member.
    template<typename integral_type>
    negatable(const integral_type& n,
              const typename std::enable_if<  (   std::is_integral<integral_type>::value
                                               && std::is_signed  <integral_type>::value)
                                            || std::is_same<integral_type, value_type>::value>::type* = nullptr)
      : data((!(n < 0)) ? +value_type(unsigned_small_type(unsigned_small_type(+n) << radix_split) & unsigned_small_mask())
                        : -value_type(unsigned_small_type(unsigned_small_type(-n) << radix_split) & unsigned_small_mask())) { }

    // Here are the class constructors from built-in floating-point types.
    template<typename floating_point_type>
    negatable(const floating_point_type& f,
              const typename std::enable_if<   std::is_floating_point<floating_point_type>::value
                                            || std::is_same<floating_point_type, float_type>::value>::type* = nullptr)
      : data()
    {
      make_from_floating_point_type(f);
    }

    // Here is the class copy constructor.
    negatable(const negatable& v) : data(v.data) { }


    /** Mixed constructors **/
    /**
     *  Constructor from a negatable with larger range AND resolution
     */

    template<int m_range, int m_resolution, typename m_round, typename m_overflow,
             typename std::enable_if< std::is_same<m_round, round_mode>::value
                                              && std::is_same<m_overflow, overflow_mode>::value
                                              && m_range <= integral_range
                                              //Since resolution in negatable will always be negative
                                              && m_resolution >= fractional_resolution
                                              >::type* = nullptr>
    negatable(const negatable<m_range, m_resolution, m_round, m_overflow>& rhs): data (rhs.get_data())
    {
      data = data << (m_resolution - fractional_resolution);
    }

    template<int m_range, int m_resolution, typename m_round, typename m_overflow,
             typename std::enable_if< !(std::is_same<m_round, round_mode>::value
                                              && std::is_same<m_overflow, overflow_mode>::value
                                              && m_range <= integral_range
                                              //Since resolution in negatable will always be negative
                                              && m_resolution >= fractional_resolution)
                                              >::type* = nullptr>                                          
    negatable(const negatable<m_range, m_resolution, m_round, m_overflow>& rhs): data (rhs.get_data())
    {
      //static_assert(false, "The range and resolution of target should be greater than source");
      std::cout<<"more"<<std::endl;
    }

    /*template<int m_range, int m_resolution, typename m_round, typename m_overflow>                                          
    negatable(const negatable<m_range, m_resolution, m_round, m_overflow>& rhs)
    {
      //static_assert(false, "The range and resolution BOTH should be less");
    }*/

    /*template<int ran1, int res1, typename rou1, typename ove1, int ran2, int res2, typename rou2, typename ove2>
    bool is_constructible(const negatable<ran1,res1,rou1,ove1> &from, const negatable<ran2, res2, rou2, ove2> &to) const
    {
      return std::is_same<ove1, ove2>::value
          && std::is_same<rou1, rou2>::value
         && ran1 <= ran2
         && res1 >= res2;
    }*/

    ~negatable() { }

    negatable& operator=(const negatable& v)
    {
      if(this != (&v))
      {
        data = v.data;
      }

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
      // because the value_type (even though just s wide as
      // unsigned_small_type) reserves one bit for the sign.

      const int total_right_shift = radix_split - 1;

      unsigned_small_type u_round((total_right_shift >= 0) ? static_cast<unsigned_small_type>(result >> +total_right_shift)
                                                           : static_cast<unsigned_small_type>(result << -total_right_shift));

      // Round the result of the multiplication.
      const boost::int_fast8_t rounding_result = binary_round(u_round);

      if(rounding_result != 0)
      {
        // Add or subtract the result of the rounding (-1, 0, or +1).
        u_round += rounding_result;
      }

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

        if(rounding_result != INT8_C(0))
        {
          // Add or subtract the result of the rounding (-1, 0, or +1).
          u_round += rounding_result;
        }

        u_round = (u_round & unsigned_small_mask());

        // Load the fixed-point result and account for potentially signed values.
        data = value_type((!result_is_neg) ? value_type(u_round) : -value_type(u_round));
      }

      return *this;
    }

    // Unary operators add, sub, mul, div of negatable with an arithmetic built-in type.

    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type> negatable& operator+=(T& n) { return (*this) += negatable(n); }
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type> negatable& operator-=(T& n) { return (*this) -= negatable(n); }
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type> negatable& operator*=(T& n) { return (*this) *= negatable(n); }
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type> negatable& operator/=(T& n) { return (*this) /= negatable(n); }

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

    // this function is used primarily for debugging and testing purposes
    // tbd: should not be exposed to users
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

  private:
    value_type data;

    typedef typename detail::integer_type_helper<negatable::digits_total * 1>::exact_unsigned_type unsigned_small_type;
    typedef typename detail::integer_type_helper<negatable::digits_total * 2>::exact_unsigned_type unsigned_large_type;

    static const unsigned_small_type& unsigned_small_mask() BOOST_NOEXCEPT
    {
      static const unsigned_small_type the_value =
        detail::bit_mask_helper<unsigned_small_type,
                                0U,
                                unsigned(integral_range - fractional_resolution)>::value();

      return the_value;
    }

    struct nothing { };

    /*! \tparam integral_type Integer type on which the fixed-point type is based, typically the native unsigned integer type unsigned int, 
    but can be a smaller fundamental type like short int, or a much longer type like boost::multiprecision::cpp_int.
    \sa http://www.boost.org/doc/libs/release/libs/multiprecision/doc/html/boost_multiprecision/tut/ints/cpp_int.html
    */
    template<typename integral_type>
    negatable(const nothing&,  
              const integral_type& n,
              const typename std::enable_if<   std::is_integral<integral_type>::value
                                            || std::is_same<typename negatable::value_type, integral_type>::value
                                            || std::is_same<typename negatable::unsigned_small_type, integral_type>::value>::type* = nullptr) : data(n) { }

    template<typename local_round_mode = round_mode>
    static boost::int_fast8_t
      binary_round(unsigned_small_type& u_round,
                   typename std::enable_if<std::is_same<local_round_mode, round::fastest>::value>::type* = nullptr)
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

    template<typename local_round_mode = round_mode>
    static boost::int_fast8_t
      binary_round(unsigned_small_type& u_round,
                   typename std::enable_if<std::is_same<local_round_mode, round::nearest_even>::value>::type* = nullptr)
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

    template<typename floating_point_type>
    floating_point_type convert_to_floating_point_type() const
    {
      const bool is_neg = (data < static_cast<value_type>(0));

      floating_point_type f(0);

      // Convert the fixed_point value to floating_point_type result.
      // The fixed_point value is converted with brute force,
      // using one bit at a time.

      // TBD: The conversion uses brute force with an inefficient loop.
      // Can (should) this mechanism be optimized?

      {
        using std::ldexp;

        unsigned_small_type u((!is_neg) ? data : -data);

        BOOST_CONSTEXPR_OR_CONST int digits_unsigned_small_type = std::numeric_limits<unsigned_small_type>::digits;
        BOOST_CONSTEXPR_OR_CONST int digits_floating_point_type = std::numeric_limits<floating_point_type>::digits;

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
            f += ldexp(floating_point_type(1), digit_index);
          }
        }

        // Rounding needs to be done if the fixed_point type
        // has more digits than the target floating_point_type
        // of the result.

        if(digit_index < boost::int_fast16_t(std::numeric_limits<unsigned_small_type>::digits))
        {
          // Check if the result of the conversion needs to be rounded.
          const boost::int_fast8_t rounding_result = binary_round(u);

          // Add or subtract the result of the rounding (-1, 0, or +1).
          f += floating_point_type(rounding_result);
        }

        f = ldexp(f, -int(radix_split));
      }

      return ((!is_neg) ? f : -f);
    }

    template<typename floating_point_type>
    void make_from_floating_point_type(const floating_point_type& f)
    {
      // Define a local_unsigned_small_type.

      // This is an unsigned integral type that is guaranteed
      // to hold the larger of:
      // * the number of digits in floating_point_type plus 1 extra digit
      // * the number of digits in unsigned_small_type.

      BOOST_CONSTEXPR_OR_CONST int fp_digits_plus_one = std::numeric_limits<floating_point_type>::digits + 1;

      typedef
      typename detail::integer_type_helper<
        unsigned((fp_digits_plus_one > std::numeric_limits<unsigned_small_type>::digits)
                    ? fp_digits_plus_one
                    : std::numeric_limits<unsigned_small_type>::digits)>::exact_unsigned_type
      local_unsigned_small_type;

      const bool is_neg = (f < floating_point_type(0));

      using std::frexp;
      using std::ldexp;

      // Extract the mantissa and exponent.
      int exp;
      const floating_point_type fp(frexp((!is_neg) ? f : -f, &exp));

      // Here we scale the mantissa to an unsigned integer value
      // that is large enough to contain all the binary digits
      // of the floating_point_type representation.
      local_unsigned_small_type u;

      const floating_point_type fp_shifted = ldexp(fp, fp_digits_plus_one);

      detail::conversion_helper<local_unsigned_small_type,
                                floating_point_type>::convert_floating_point_to_unsigned_integer(fp_shifted, u);

      // Select the scale factor for the conversion to the fixed-point type.

      // Here, we use 1 extra binary digit for rounding.
      // The extra rounding digit fits in unsigned_small_type
      // because the value_type (even though just s wide as
      // unsigned_small_type) reserves one bit for the sign.

      const boost::int_fast16_t total_left_shift =   boost::int_fast16_t(radix_split + exp)
                                                   - boost::int_fast16_t(std::numeric_limits<floating_point_type>::digits);

      const local_unsigned_small_type u_round_local((total_left_shift >= INT16_C(0)) ? (u << +total_left_shift)
                                                                                     : (u >> -total_left_shift));

      unsigned_small_type u_round = static_cast<unsigned_small_type>(u_round_local);

      // Round the result of the construction from floating_point_type.
      const boost::int_fast8_t rounding_result = binary_round(u_round);

      // Add or subtract the result of the rounding (-1, 0, or +1).
      u_round += rounding_result;

      u_round = (u_round & unsigned_small_mask());

      // Load the fixed-point result and account for potentially signed values.
      data = value_type((!is_neg) ? value_type(u_round) : -value_type(u_round));
    }

    template<typename arithmetic_type>
    static const arithmetic_type& radix_split_value()
    {
      static const arithmetic_type the_radix_split_value(detail::radix_split_maker<arithmetic_type, radix_split>::value());

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

    // Implementations of global unary plus and minus.
    friend inline negatable operator+(const negatable& self) { return negatable(self); }
    friend inline negatable operator-(const negatable& self) { negatable tmp(self); tmp.data = -tmp.data; return tmp; }

    // Implementations of global binary add, sub, mul, div of [lhs(negatable)] operator [rhs(negatable)].
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline T operator+(const T& u, const T& v) { return T(u) += v; }
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline T operator-(const T& u, const T& v) { return T(u) -= v; }
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline T operator*(const T& u, const T& v) { return T(u) *= v; }
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline T operator/(const T& u, const T& v) { return T(u) /= v; }

    // Implementations of global binary add, sub, mul, div of [lhs(negatable)] operator [rhs(arithmetic_type)].
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator+(const negatable& u, const T& v) { return negatable(u) += v; }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator-(const negatable& u, const T& v) { return negatable(u) -= v; }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator*(const negatable& u, const T& v) { return negatable(u) *= v; }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator/(const negatable& u, const T& v) { return negatable(u) /= v; }

    // Implementations of global binary add, sub, mul, div of [lhs(arithmetic_type)] operator [rhs(negatable)].
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator+(const T& u, const negatable& v) { return negatable(u) += v; }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator-(const T& u, const negatable& v) { return negatable(u) -= v; }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator*(const T& u, const negatable& v) { return negatable(u) *= v; }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator/(const T& u, const negatable& v) { return negatable(u) /= v; }

    // Implementations of global equality.
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline bool operator==(const T& u,         const T& v) { return (u.data == v.data); }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator==(const negatable& u, const T& v) { return (u.data == negatable(v).data); }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator==(const T& u, const negatable& v) { return (negatable(u).data == v.data); }

    // Implementations of global inequality.
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline bool operator!=(const T& u,         const T& v) { return (u.data != v.data); }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator!=(const negatable& u, const T& v) { return (u.data != negatable(v).data); }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator!=(const T& u, const negatable& v) { return (negatable(u).data != v.data); }

    // Implementations of global operators >, <, >=, <=.
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline bool operator> (const T& u,         const T& v) { return (u.data >  v.data); }
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline bool operator< (const T& u,         const T& v) { return (u.data <  v.data); }
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline bool operator>=(const T& u,         const T& v) { return (u.data >= v.data); }
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline bool operator<=(const T& u,         const T& v) { return (u.data <= v.data); }

    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator> (const negatable& u, const T& v) { return (u.data >  negatable(v).data); }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator< (const negatable& u, const T& v) { return (u.data <  negatable(v).data); }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator>=(const negatable& u, const T& v) { return (u.data >= negatable(v).data); }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator<=(const negatable& u, const T& v) { return (u.data <= negatable(v).data); }

    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator> (const T& u, const negatable& v) { return (negatable(u).data >  v.data); }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator< (const T& u, const negatable& v) { return (negatable(u).data <  v.data); }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator>=(const T& u, const negatable& v) { return (negatable(u).data >= v.data); }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr>      friend inline bool operator<=(const T& u, const negatable& v) { return (negatable(u).data <= v.data); }

    // Helper utilities for mathematical constants.
    // We need mathematical constants for transcendental functions.
    template<const int bit_count,
             typename enable_type = void>
    struct constant_maker
    {
      static const negatable& pi()
      {
        static_assert(integral_range >= 2,
                      "The constant pi can not be created with fewer than 2 binary integral range digits");

        static const negatable value_pi(boost::math::constants::pi<negatable>());
        return value_pi;
      }

      static const negatable& ln_two()
      {
        static const negatable value_ln_two(boost::math::constants::ln_two<negatable>());
        return value_ln_two;
      }
    };

    template<const int bit_count>
    struct constant_maker<bit_count,
                          typename std::enable_if<(bit_count <= 8)>::type>
    {
      static const negatable& pi()
      {
        static_assert(integral_range >= 2,
                      "The constant pi can not be created with fewer than 2 binary integral range digits");

        static const negatable value_pi(nothing(), value_type((UINT8_C(0x64) + ((UINT8_C(1) << (5 + fractional_resolution)) / 2U)) >> (5 + fractional_resolution)));
        return value_pi;
      }

      static const negatable& ln_two()
      {
        static const negatable value_ln_two(nothing(), value_type((UINT8_C(0x58) + ((UINT8_C(1) << (7 + fractional_resolution)) / 2U)) >> (7 + fractional_resolution)));
        return value_ln_two;
      }
    };

    template<const int bit_count>
    struct constant_maker<bit_count,
                          typename std::enable_if<(bit_count > 8) && (bit_count <= 16)>::type>
    {
      static const negatable& pi()
      {
        static_assert(integral_range >= 2,
                      "The constant pi can not be created with fewer than 2 binary integral range digits");

        static const negatable value_pi(nothing(), value_type((UINT16_C(0x6487) + ((UINT16_C(1) << (13 + fractional_resolution)) / 2U)) >> (13 + fractional_resolution)));
        return value_pi;
      }

      static const negatable& ln_two()
      {
        static const negatable value_ln_two(nothing(), value_type((UINT16_C(0x58B9) + ((UINT16_C(1) << (15 + fractional_resolution)) / 2U)) >> (15 + fractional_resolution)));
        return value_ln_two;
      }
    };

    template<const int bit_count>
    struct constant_maker<bit_count,
                          typename std::enable_if<(bit_count > 16) && (bit_count <= 32)>::type>
    {
      static const negatable& pi()
      {
        static_assert(integral_range >= 2,
                      "The constant pi can not be created with fewer than 2 binary integral range digits");

        static const negatable value_pi(nothing(), value_type((UINT32_C(0x6487ED51) + ((UINT32_C(1) << (29 + fractional_resolution)) / 2U)) >> (29 + fractional_resolution)));
        return value_pi;
      }

      static const negatable& ln_two()
      {
        static const negatable value_ln_two(nothing(), value_type((UINT32_C(0x58B90BfB) + ((UINT32_C(1) << (31 + fractional_resolution)) / 2U)) >> (31 + fractional_resolution)));
        return value_ln_two;
      }
    };

    template<const int bit_count>
    struct constant_maker<bit_count,
                          typename std::enable_if<(bit_count > 32) && (bit_count <= 64)>::type>
    {
      static const negatable& pi()
      {
        static_assert(integral_range >= 2,
                      "The constant pi can not be created with fewer than 2 binary integral range digits");

        static const negatable value_pi(nothing(), value_type((UINT64_C(0x6487ED5110B4611A) + ((UINT64_C(1) << (61 + fractional_resolution)) / 2U)) >> (61 + fractional_resolution)));
        return value_pi;
      }

      static const negatable& ln_two()
      {
        static const negatable value_ln_two(nothing(), value_type((UINT64_C(0x58B90BFBE8E7BCD6) + ((UINT64_C(1) << (63 + fractional_resolution)) / 2U)) >> (63 + fractional_resolution)));
        return value_ln_two;
      }
    };

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

  template<const int integral_range, const int fractional_resolution, typename round_mode, typename overflow_mode> BOOST_CONSTEXPR_OR_CONST int negatable<integral_range, fractional_resolution, round_mode, overflow_mode>::digits_total;
  template<const int integral_range, const int fractional_resolution, typename round_mode, typename overflow_mode> BOOST_CONSTEXPR_OR_CONST int negatable<integral_range, fractional_resolution, round_mode, overflow_mode>::radix_split;

  } } // namespace boost::fixed_point

  using boost::fixed_point::abs;
  using boost::fixed_point::fabs;
  using boost::fixed_point::frexp;
  using boost::fixed_point::ldexp;
  using boost::fixed_point::sqrt;

  namespace std
  {
    // Provide specializations of std::numeric_limits<negatable>.

    //! \note Individual template specializations need to be provided
    // for each different rounding mode and overflow mode.

    // Here is the template specialization of std::numeric_limits<negatable>
    // for round::fastest and overflow::undefined.
    template<const int integral_range,
             const int fractional_resolution>
    class numeric_limits<boost::fixed_point::negatable<integral_range,
                                                       fractional_resolution,
                                                       boost::fixed_point::round::fastest,
                                                       boost::fixed_point::overflow::undefined>>
    {
    private:
      typedef boost::fixed_point::negatable<integral_range,
                                            fractional_resolution,
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

      static BOOST_CONSTEXPR_OR_CONST negatable_type (min)      () BOOST_NOEXCEPT { return negatable_type::value_min(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type (max)      () BOOST_NOEXCEPT { return negatable_type::value_max(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type lowest     () BOOST_NOEXCEPT { return -(max)(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type epsilon    () BOOST_NOEXCEPT { return negatable_type::epsilon_maker(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type round_error() BOOST_NOEXCEPT { return negatable_type(1); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type infinity   () BOOST_NOEXCEPT { return negatable_type(0); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type quiet_NaN  () BOOST_NOEXCEPT { return negatable_type(0); }
    };

    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_specialized;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::digits;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::digits10;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::max_digits10;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_signed;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_integer;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_exact;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::radix;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::min_exponent;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::min_exponent10;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::max_exponent;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::max_exponent10;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_infinity;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_quiet_NaN;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_signaling_NaN;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST std::float_denorm_style numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_denorm;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::has_denorm_loss;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_iec559;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_bounded;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::is_modulo;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::traps;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::tinyness_before;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST std::float_round_style  numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::fastest, boost::fixed_point::overflow::undefined>>::round_style;

    // Here is the template specialization of std::numeric_limits<negatable>
    // for round::nearest_even and overflow::undefined.
    template<const int integral_range,
             const int fractional_resolution>
    class numeric_limits<boost::fixed_point::negatable<integral_range,
                                                       fractional_resolution,
                                                       boost::fixed_point::round::nearest_even,
                                                       boost::fixed_point::overflow::undefined>>
    {
    private:
      typedef boost::fixed_point::negatable<integral_range,
                                            fractional_resolution,
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

      static BOOST_CONSTEXPR_OR_CONST negatable_type (min)      () BOOST_NOEXCEPT { return negatable_type::value_min(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type (max)      () BOOST_NOEXCEPT { return negatable_type::value_max(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type lowest     () BOOST_NOEXCEPT { return -(max)(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type epsilon    () BOOST_NOEXCEPT { return negatable_type::epsilon_maker(); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type round_error() BOOST_NOEXCEPT { return negatable_type(1) / 2; }
      static BOOST_CONSTEXPR_OR_CONST negatable_type infinity   () BOOST_NOEXCEPT { return negatable_type(0); }
      static BOOST_CONSTEXPR_OR_CONST negatable_type quiet_NaN  () BOOST_NOEXCEPT { return negatable_type(0); }
    };

    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_specialized;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::digits;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::digits10;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::max_digits10;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_signed;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_integer;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_exact;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::radix;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::min_exponent;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::min_exponent10;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::max_exponent;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST int                     numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::max_exponent10;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_infinity;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_quiet_NaN;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_signaling_NaN;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST std::float_denorm_style numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_denorm;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::has_denorm_loss;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_iec559;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_bounded;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::is_modulo;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::traps;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST bool                    numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::tinyness_before;
    template<const int integral_range, const int fractional_resolution> BOOST_CONSTEXPR_OR_CONST std::float_round_style  numeric_limits<boost::fixed_point::negatable<integral_range, fractional_resolution, boost::fixed_point::round::nearest_even, boost::fixed_point::overflow::undefined>>::round_style;
  } // namespace std

#endif // FIXED_POINT_2015_03_06_HPP_
