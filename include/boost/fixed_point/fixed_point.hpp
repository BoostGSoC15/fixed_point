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
  #include <limits>
  #include <ostream>
  #include <sstream>
  #include <boost/fixed_point/fixed_point_detail.hpp>

  namespace boost { namespace fixed_point {

  namespace round
  {
    typedef enum round_type
    {
      fastest,       // Speed is more important than the choice in value.
      negative,      // Round towards negative infinity. This mode is useful in interval arithmetic.
      truncated,     // Round towards zero. This mode is useful in implementing integral arithmetic.
      positive,      // Round towards positive infinity. This mode is useful in interval arithmetic.
      classic,       // Round towards the nearest value, but exactly-half values are rounded towards maximum magnitude. This mode is the standard school algorithm.
      nearest_even,  // Round towards the nearest value, but exactly-half values are rounded towards even values. This mode has more balance than the classic mode.
      nearest_odd,   // Round towards the nearest value, but exactly-half values are rounded towards odd values. This mode has as much balance as the near_even mode, but preserves more information.
    }
    round_type;
  }

  namespace overflow
  {
    typedef enum overflow_type
    {
      impossible, // Programmer analysis of the program has determined that overflow cannot occur. Uses of this mode should be accompanied by an argument supporting the conclusion.
      undefined,  // Programmers are willing to accept undefined behavior in the event of an overflow.
      modulus,    // The assigned value is the dynamic value mod the range of the variable. This mode makes sense only with unsigned numbers. It is useful for angular measures.
      saturate,   // If the dynamic value exceeds the range of the variable, assign the nearest representable value.
      exception   // If the dynamic value exceeds the range of the variable, throw an exeception of type std::overflow_error.
    }
    overflow_type;
  }
  } } // namespace boost::fixed_point

  // Forward declaration of the negatable class.
  namespace boost { namespace fixed_point {
    template<const int integral_range,
             const int decimal_resolution,
             const fixed_point::round::round_type round_mode,
             const fixed_point::overflow::overflow_type overflow_mode>
    class negatable;
  } }
  // namespace boost::fixed_point

  namespace std
  {
    // Forward declaration of the specialization of std::numeric_limits<negatable>.
    template<const int integral_range,
             const int decimal_resolution,
             const boost::fixed_point::round::round_type round_mode,
             const boost::fixed_point::overflow::overflow_type overflow_mode>
    class numeric_limits<boost::fixed_point::negatable<integral_range,
                                                       decimal_resolution,
                                                       round_mode,
                                                       overflow_mode>>;
  }

  namespace boost { namespace fixed_point {

  // We will now begin the implementation of the negatable class.
  template<const int integral_range,
           const int decimal_resolution,
           const round::round_type round_mode = round::fastest,
           const overflow::overflow_type overflow_mode = overflow::undefined>
  class negatable
  {
  private:
    static const int range      = integral_range - decimal_resolution;
    static const int resolution = decimal_resolution;

    static_assert( resolution < 0,
                  "Error: the negatable class resolution must be fractional (negative).");
    static_assert(-resolution < range - 1,
                  "Error: the negatable class resolution exceeds the available range.");
    static_assert( round_mode == round::fastest || round_mode == round::negative,
                  "Error: only negative and fastest round modes supported at the moment");

    /*Primarily for debugging/testing purposes*/
    template<typename T>
    void print_bits (T num)
    {
      std::string ans = "";
      size_t bits = range;
      T mask = T(1);
      for (int i = 0; i < bits; i++)
      {
        if (num & mask) ans+="1";
        else ans+="0";
        mask = mask << 1;
      }
      std::reverse (ans.begin (), ans.end ());
      std::cout<<ans<<"\n";
    }

    /*round the value at the time of construction
      for example: negatable<2,-2, round::negative> a (-1.32);
      the value of a should be -1.5
    */
    template<typename initialize_type, typename underlying_type>
    void round_construct (const initialize_type &value, underlying_type &data)
    {
      // tbd: this works under the assumption that without any work, the default behavior happens to be similar to round::truncated
      // I'm not sure if this is machine specific though
      if (round_mode == round::negative)
      {
        /* check if number is already perfectly representable; no need to round then*/
        if (resolution > 0)
        {
          // TBD: I don't really like dividing here, not sure if there's a better way though
          initialize_type scale = value / fixed_point::detail::radix_split_maker<initialize_type, resolution>::value ();

          /* no rounding needed if perfectly divisible */
          if (floor (scale) == scale)
            return;
        }

        if (resolution < 0)
        {
          initialize_type scale = value * fixed_point::detail::radix_split_maker<initialize_type, -resolution>::value ();

          /* no rounding needed if perfectly divisible */
          if (floor (scale) == scale)
            return;
        }

        /*Since the default rounding behaviour is towards_zero/truncated, for values > 0, behaviour is same as towards_zero*/
        if (value < 0) 
          data -= 1;

      }
    }

  public:
    typedef typename detail::integer_type_helper<range>::exact_signed_type value_type;
    typedef typename detail::float_type_helper  <range>::exact_float_type  float_type;

    negatable() : data() { }

    template<typename integral_type>
    negatable(const integral_type& n,
              const typename std::enable_if<std::is_integral<integral_type>::value>::type* = nullptr) : data(value_type(n) * radix_split_value<value_type>())
    {
      /*std::cout<<typeid(signed_integral_type).name() <<"\n";
      std::cout<<sizeof(signed_integral_type)<<"\n";
      std::cout<<typeid(value_type).name() <<"\n";
      std::cout<<sizeof(value_type)<<std::endl;
      std::cout<<data<<"\n";*/
      round_construct (n, data);
      print_bits(data);
    }

    template<typename floating_point_type>
    negatable(const floating_point_type& f,
              const typename std::enable_if<std::is_floating_point<floating_point_type>::value>::type* = nullptr) : data(value_type(f * radix_split_value<floating_point_type>()))
    {
      round_construct (f, data);
      print_bits(data);	
    }

    negatable(const negatable& v) : data(v.data) { }

    ~negatable() { }

    negatable& operator=(const negatable& v)
    {
      if(this != (&v))
      {
        data = v.data;
      }

      return *this;
    }

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

    negatable& operator=(const float& f)              { data = value_type(f  * radix_split_value<float>      ()); return *this; }
    negatable& operator=(const double& d)             { data = value_type(d  * radix_split_value<double>     ()); return *this; }
    negatable& operator=(const long double& ld)       { data = value_type(ld * radix_split_value<long double>()); return *this; }

    negatable& operator++()   { data += value_type(unsigned_small_type(1) << radix_split); return *this; }
    negatable& operator--()   { data -= value_type(unsigned_small_type(1) << radix_split); return *this; }

    negatable operator++(int) { const negatable tmp(*this); data += value_type(unsigned_small_type(1) << radix_split); return tmp; }
    negatable operator--(int) { const negatable tmp(*this); data -= value_type(unsigned_small_type(1) << radix_split); return tmp; }

    negatable& operator+=(const negatable& v)
    {
      data += v.data;
      return *this;
    }

    negatable& operator-=(const negatable& v)
    {
      data -= v.data;
      return *this;
    }

    negatable& operator*=(const negatable& v)
    {
      const bool u_is_neg      = (  data < 0);
      const bool v_is_neg      = (v.data < 0);
      const bool result_is_neg = (u_is_neg != v_is_neg);

      unsigned_large_type result((!u_is_neg) ? data : -data);

      result *= ((!v_is_neg) ? unsigned_large_type(v.data) : unsigned_large_type(-v.data));

      result >>= radix_split;

      data = detail::convert_to<unsigned_large_type, value_type>(result);

      if(result_is_neg)
      {
        data = -data;
      }

      return *this;
    }

    negatable& operator/=(const negatable& v)
    {
      const bool u_is_neg      = (  data < 0);
      const bool v_is_neg      = (v.data < 0);
      const bool result_is_neg = (u_is_neg != v_is_neg);

      if(v.data == 0)
      {
        data = 0;
        return *this;
      }

      unsigned_large_type result((!u_is_neg) ? data : -data);

      result <<= radix_split;

      result /= ((!v_is_neg) ? unsigned_large_type(v.data) : unsigned_large_type(-v.data));

      data = detail::convert_to<unsigned_large_type, value_type>(result);

      if(result_is_neg)
      {
        data = -data;
      }

      return *this;
    }

    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type> negatable& operator+=(T& n) { return (*this) += negatable(n); }
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type> negatable& operator-=(T& n) { return (*this) -= negatable(n); }
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type> negatable& operator*=(T& n) { return (*this) *= negatable(n); }
    template<typename T, typename = typename std::enable_if<std::is_arithmetic<T>::value>::type> negatable& operator/=(T& n) { return (*this) /= negatable(n); }

    operator char     () { return static_cast<char>     (data / radix_split_value<value_type>()); }
    operator short    () { return static_cast<short>    (data / radix_split_value<value_type>()); }
    operator int      () { return static_cast<int>      (data / radix_split_value<value_type>()); }
    operator long     () { return static_cast<long>     (data / radix_split_value<value_type>()); }
    operator long long() { return static_cast<long long>(data / radix_split_value<value_type>()); }

    operator unsigned char     () { return static_cast<unsigned char>     (unsigned_small_type(data) >> radix_split); }
    operator unsigned short    () { return static_cast<unsigned short>    (unsigned_small_type(data) >> radix_split); }
    operator unsigned int      () { return static_cast<unsigned int>      (unsigned_small_type(data) >> radix_split); }
    operator unsigned long     () { return static_cast<unsigned long>     (unsigned_small_type(data) >> radix_split); }
    operator unsigned long long() { return static_cast<unsigned long long>(unsigned_small_type(data) >> radix_split); }

    // TBD: Floating-point cast operators for float, double and long double
    // can make use of a private template member function in order to
    // localize the conversion code to one spot.
    operator float()
    {
      const bool is_neg = (data < static_cast<value_type>(0));

      const unsigned_small_type x = static_cast<unsigned_small_type>((!is_neg) ? data : -data);

      const unsigned_small_type integer_part = (x >> radix_split);
      const unsigned_small_type decimal_part = x - (integer_part << radix_split);

      const float f = static_cast<float>(integer_part) + (static_cast<float>(decimal_part) / radix_split_value<float>());

      return ((!is_neg) ? f : -f);
    }

    operator double()
    {
      const bool is_neg = (data < static_cast<value_type>(0));

      const unsigned_small_type x = static_cast<unsigned_small_type>((!is_neg) ? data : -data);

      const unsigned_small_type integer_part = (x >> radix_split);
      const unsigned_small_type decimal_part = x - (integer_part << radix_split);

      const double f = static_cast<double>(integer_part) + (static_cast<double>(decimal_part) / radix_split_value<double>());

      return ((!is_neg) ? f : -f);
    }

    operator long double()
    {
      const bool is_neg = (data < static_cast<value_type>(0));

      const unsigned_small_type x = static_cast<unsigned_small_type>((!is_neg) ? data : -data);

      const unsigned_small_type integer_part = (x >> radix_split);
      const unsigned_small_type decimal_part = x - (integer_part << radix_split);

      const long double f = static_cast<long double>(integer_part) + (static_cast<long double>(decimal_part) / radix_split_value<long double>());

      return ((!is_neg) ? f : -f);
    }

  private:
    value_type data;

    static const int radix_split = -resolution;

    typedef typename detail::integer_type_helper<range * 1>::exact_unsigned_type unsigned_small_type;
    typedef typename detail::integer_type_helper<range * 2>::exact_unsigned_type unsigned_large_type;

    template<typename arithmetic_type>
    static arithmetic_type radix_split_value()
    {
      return detail::radix_split_maker<arithmetic_type, radix_split>::value();
    }

    struct nothing { };

    template<typename integral_type>
    negatable(const nothing&,
              const integral_type& n,
              const typename std::enable_if<   std::is_integral<integral_type>::value
                                            || std::is_same<value_type, integral_type>::value>::type* = nullptr) : data(n) { }

    static negatable& value_epsilon()
    {
      static bool      is_init     = bool();
      static negatable the_epsilon = negatable();

      if(is_init == false)
      {
        is_init = true;

        value_type r10  = radix_split_value<value_type>();

        if(r10 <= 10)
        {
          the_epsilon = negatable(nothing(), 1);
        }
        else
        {
          while(r10 > 10)
          {
            r10 = (r10 + 9) / 10;
          }

          the_epsilon = negatable(nothing(), r10);
        }
      }

      return the_epsilon;
    }

    static negatable& value_min() { static negatable the_value_min(nothing(), 1); return the_value_min; }
    static negatable& value_max() { static negatable the_value_max(nothing(), (std::numeric_limits<value_type>::max)()); return the_value_max; }

    friend class std::numeric_limits<negatable>;

    template<typename char_type,
             typename traits_type>
    friend
    inline std::basic_ostream<char_type,
                              traits_type>& operator<<(std::basic_ostream<char_type, traits_type>& os,
                                                       const negatable& x)
    {
      std::basic_ostringstream<char_type, traits_type> ostr;

      ostr.flags    (os.flags());
      ostr.imbue    (os.getloc());
      ostr.precision(os.precision());

      // Use a stringstream to convert the integer representation
      // of the data to a string and then subsequently extract
      // a floating-point representation from it.

      // TBD: Is there a more efficient way to do this?
      // TBD: Or is there a more sensible way to do this?

      std::stringstream ss;
      ss << unsigned_large_type(x.data);

      float_type v;
      ss >> v;

      ostr << (v / detail::radix_split_maker<float_type, radix_split>::value());

      return (os << ostr.str());
    }

    // Implementations of global unary plus and minus.
    friend inline negatable operator+(const negatable& left) { return negatable(left); }
    friend inline negatable operator-(const negatable& left) { negatable tmp(left); tmp.data = -tmp.data; return tmp; }

    // Implementations of global add, sub, mul, div of [lhs(negatable)] operator [rhs(negatable)].
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline T operator+(const T& u, const T& v) { return T(u) += v; }
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline T operator-(const T& u, const T& v) { return T(u) -= v; }
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline T operator*(const T& u, const T& v) { return T(u) *= v; }
    template<typename T, typename std::enable_if<std::is_same<T, negatable>::value>::type* = nullptr> friend inline T operator/(const T& u, const T& v) { return T(u) /= v; }

    // Implementations of global add, sub, mul, div of [lhs(negatable)] operator [rhs(arithmetic_type)].
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator+(const negatable& u, const T& v) { return negatable(u) += v; }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator-(const negatable& u, const T& v) { return negatable(u) -= v; }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator*(const negatable& u, const T& v) { return negatable(u) *= v; }
    template<typename T, typename std::enable_if<std::is_arithmetic<T>::value>::type* = nullptr> friend inline negatable operator/(const negatable& u, const T& v) { return negatable(u) /= v; }

    // Implementations of global add, sub, mul, div of [lhs(arithmetic_type)] operator [rhs(negatable)].
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
  };
  } } // namespace boost::fixed_point

  namespace std
  {
    // Provide a specialization of std::numeric_limits<negatable>.
    template<const int integral_range,
             const int decimal_resolution,
             const boost::fixed_point::round::round_type round_mode,
             const boost::fixed_point::overflow::overflow_type overflow_mode>
    class numeric_limits<boost::fixed_point::negatable<integral_range,
                                                       decimal_resolution,
                                                       round_mode,
                                                       overflow_mode>>
    {
    private:
      typedef boost::fixed_point::negatable<integral_range,
                                            decimal_resolution,
                                            round_mode,
                                            overflow_mode> negatable_type;

    public:
      // TBD: Use the proper C++11 interface to numeric_limits.
      static const bool                    is_specialized    = true;
      static const int                     digits            = std::numeric_limits<typename negatable_type::value_type>::digits;
      static const int                     digits10          = std::numeric_limits<typename negatable_type::value_type>::digits10;
      static const int                     max_digits10      = std::numeric_limits<typename negatable_type::value_type>::max_digits10;
      static const bool                    is_signed         = true;
      static const bool                    is_integer        = false;
      static const bool                    is_exact          = true;
      static const int                     radix             = 2;
      static const int                     min_exponent      = -negatable_type::radix_split;
      static const int                     min_exponent10    = static_cast<int>((static_cast<long>(min_exponent) * 301L) / 1000L);
      static const int                     max_exponent      = std::numeric_limits<typename negatable_type::value_type>::digits - negatable_type::radix_split;
      static const int                     max_exponent10    = static_cast<int>((static_cast<long>(max_exponent) * 301L) / 1000L);
      static const bool                    has_infinity      = false;
      static const bool                    has_quiet_NaN     = false;
      static const bool                    has_signaling_NaN = false;
      static const std::float_denorm_style has_denorm        = std::denorm_absent;
      static const bool                    has_denorm_loss   = false;
      static const bool                    is_iec559         = false;
      static const bool                    is_bounded        = true;
      static const bool                    is_modulo         = false;
      static const bool                    traps             = false;
      static const bool                    tinyness_before   = false;
      static const std::float_round_style  round_style       = std::round_toward_zero;

      static negatable_type (min)      () throw()            { return negatable_type::value_min(); }
      static negatable_type (max)      () throw()            { return negatable_type::value_max(); }
      static negatable_type lowest     () throw()            { return -(max)(); }
      static negatable_type epsilon    () throw()            { return negatable_type::value_epsilon(); }
      static negatable_type round_error() throw()            { return negatable_type(1) / 2; }
      static negatable_type infinity   () throw()            { return negatable_type(0); }
      static negatable_type quiet_NaN  () throw()            { return negatable_type(0); }
    };
  }
  // namespace std

#endif // FIXED_POINT_2015_03_06_HPP_

/*
#include <iomanip>
#include <iostream>
#include <limits>
#include <boost/fixed_point/fixed_point.hpp>

int main()
{
  typedef boost::fixed_point::negatable<8, -120> fixed_point_type;
  typedef fixed_point_type::float_type           float_point_type;

  const fixed_point_type xa = fixed_point_type(10) / 3;
  const fixed_point_type xb = fixed_point_type(10) / 7;

  const float_point_type fa = float_point_type(10) / 3;
  const float_point_type fb = float_point_type(10) / 7;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << xa + xb
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float_point_type>::digits10)
            << fa + fb
            << std::endl;

  std::cout << std::setprecision
  (std::numeric_limits<fixed_point_type>::digits10)
            << xa - xb
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float_point_type>::digits10)
            << fa - fb
            << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << xa * xb
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float_point_type>::digits10)
            << fa * fb
            << std::endl;

  std::cout << std::setprecision(std::numeric_limits<fixed_point_type>::digits10)
            << xa / xb
            << std::endl;
  std::cout << std::setprecision(std::numeric_limits<float_point_type>::digits10)
            << fa / fb
            << std::endl;

  std::cout << std::boolalpha
            << (xa == xa)
            << std::endl;

  std::cout << std::boolalpha
            << (xa == xb)
            << std::endl;

  std::cout << std::boolalpha
            << (xa != xb)
            << std::endl;

  std::cout << std::boolalpha
            << (xa != 1)
            << std::endl;
}
*/
