///////////////////////////////////////////////////////////////////////////////
//  Copyright Christopher Kormanyos 2013 - 2016.
//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

/*!
  \file
  \brief Definitions of fixed_point functions for representation distance & finding adjacent fixed-point values.
  \sa http://www.boost.org/doc/libs/release/libs/math/doc/html/math_toolkit/next_float.html
  Function @c fixed_distance finds the number of gaps/bits/ULP between any two @c fixed_point values.

  If the @c fixed_point values are viewed as the underlying integral representation type,
  then their difference is the number of ULP/gaps/bits different.
*/

#ifndef FIXED_POINT_NEGATABLE_NEXT_HPP_
  #define FIXED_POINT_NEGATABLE_NEXT_HPP_

  namespace boost { namespace fixed_point {

  // Forward declaration of fixed_next are now in fixed_point_negatable.hpp with their friend declarations.

  // Definition of fixed_next.
  /*! Find the next representation that is grater than x.\n
    \param x fixed_point value to find the next fixed_point.
    \returns next representable value which is greater than x.

    \code
      fixed_next(max_value) // returns the max_value unchanged.
    \endcode
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>
    fixed_next(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    static_assert(is_fixed_point<local_negatable_type>::value,
                  "Error: fixed_next can only be instantiated with a fixed-point negatable type.");

    local_negatable_type result(x);

    if(x < (std::numeric_limits<local_negatable_type>::max)())
    {
      const local_negatable_type dx = local_negatable_type(local_nothing(), local_value_type(1));

      result += dx;
    }

    return result;
  } // fixed_next

  // Definition of fixed_prior.
  /*! Find the prior representation value that is less than x.
    \param x fixed_point value to find the prior fixed_point.
    \returns the next representable value which is less than x.\n
    \note If implemented using
    \code
      fixed_prior(std::numeric_limits<fixed_point_type>::lowest()) == std::numeric_limits<fixed_point_type>::lowest()
    \endcode

    \sa http://www.boost.org/doc/libs/release/libs/math/doc/html/math_toolkit/next_float/float_prior.html \n
    // Defined using cmath function nextafter (but might be coded more efficiently).
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>
    fixed_prior(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;
    typedef typename local_negatable_type::nothing                                  local_nothing;

    static_assert(is_fixed_point<local_negatable_type>::value,
                  "Error: fixed_next can only be instantiated with a fixed-point negatable type.");

    local_negatable_type result(x);

    if(x > std::numeric_limits<local_negatable_type>::lowest())
    {
      const local_negatable_type dx = local_negatable_type(local_nothing(), local_value_type(1));

      result -= dx;
    }

    return result;
  } // fixed_prior

  // Definition of fixed_advance.
  /*! Function fixed_advance advances a @c fixed_point number by a specified @b signed number of ULP
  (https://en.wikipedia.org/wiki/Unit_in_the_last_place),
  incrementing (or decrementing) the integral type value by @c distance bits.
  \warning It is not possible to advance more than the maximum value of the integral representation type
  in a single call of fixed_advance.
    
  \sa C math function @c nextafter and @c nexttoward,\n
  <a href="http://en.cppreference.com/w/c/numeric/math/nextafter">nextafter</a>,
  <a href="http://en.cppreference.com/w/c/numeric/math/nexttoward">nexttoward</a>,\n
  <a href="http://www.boost.org/doc/libs/release/libs/math/doc/html/math_toolkit/next_float/float_advance.html">boost::math::float_advance</a>,\n
  <a href="http://www.boost.org/doc/libs/release/libs/math/doc/html/math_toolkit/next_float/float_next.html">boost::fixed_point::float_next</a>,\n
  <a href="http://www.boost.org/doc/libs/release/libs/math/doc/html/math_toolkit/next_float/float_prior.html">boost::fixed_point::float_prior</a>,\n
  <a href="http://www.boost.org/doc/libs/release/libs/math/doc/html/math_toolkit/next_float/float_distance.html">boost::fixed_point::float_distance</a>
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>
    fixed_advance(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                  typename negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::value_type distance)
    // Use the signed underlying integral type for distance to advance.
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;

    local_negatable_type result(x);

    // distance may be negative.
    result.representation() += distance;

    // TBD: What about overflow and underflow?
    return result;
  } // fixed_advance
    
  // Definition of function fixed_distance
  /*! Function @c fixed_distance finds the number of gaps/bits/ULP between any two @c fixed_point values.
    If the significands of @c fixed_point values are viewed as the underlying integral type,
    then their difference is the number of ULP/gaps/bits different.\n
    \returns the @b signed number of gaps/bits/ULP between any two @c fixed_point values as the
    integral representation type used for the fixed_point type\n
    For example, if x is any @c boost::fixed_point fixed_point type, then
    \code
      fixed_distance(fixed_prior(x), fixed_next(x)); // == 2
      fixed_distance(fixed_next(x), fixed_prior(x)); // == -2
    \endcode
    \note (An built-in integral type like @c long might overflow 
    if @c fixed_point used a multiprecision integral type).
    \warning If the distance is greater than the size of the underlying integral type 
    used for representation of the fixed_point type,
    then overflow will occur and the result misleading. For example:
    \code
      fixed_distance(lowest, max); == -2
    \endcode
  */
  /*
  TBD: There are some problems for edge cases,
  for example fixed_distance(lowest, max); == -2
  */
  template<const int IntegralRange, const int FractionalResolution, typename RoundMode, typename OverflowMode>
  typename negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode>::value_type
    fixed_distance(negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> x,
                   negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> y)
  {
    typedef negatable<IntegralRange, FractionalResolution, RoundMode, OverflowMode> local_negatable_type;
    typedef typename local_negatable_type::value_type                               local_value_type;

    // TBD: What about cases for which distance exceeds
    // the total range of the fixed-point type??

    // Note: distance may be negative.
    const local_value_type distance = y.crepresentation() - x.crepresentation();

    return distance;
  } // fixed_distance

  } } // namespace boost::fixed_point.

#endif // FIXED_POINT_NEGATABLE_NEXT_HPP_
