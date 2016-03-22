// Copyright Christopher Kormanyos 2016.
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <boost/fixed_point/fixed_point.hpp>

namespace local
{
  template<typename T>
  T hypergeometric_1f1_rational_approx(const T& AP, const T& CP, const T& ZM)
  {
    const T Z(-ZM);

    // Implement a rational approximation of hypergeometric_1f1.
    // This C++11 code uses the computational scheme shown in
    // Y.L. Luke, "Algorithms for the Comuptation of Mathematical
    // Functions", Academic Press, New York, 1977.

    // C
    // C INITIALIZATION :
    // C

    const T Z2(Z / 2U);

    std::array<T, 4U> A;
    std::array<T, 4U> B;

    {
      const T CT1x((AP * Z) / CP);
      const T CT2x(Z2 / (1U + CP));

      A[0U] = T(1U);
      B[0U] = T(1U);
      B[1U] = 1U + (((1U + AP) * Z2) / CP);
      A[1U] = B[1U] - CT1x;
      B[2U] = 1U + (((((2U + B[1U]) * (2U + AP))) * CT2x) / 3U);
      A[2U] = B[2U] - ((1U + CT2x) * CT1x);
    }

    std::uint_fast32_t CT1 = UINT32_C(3);
    T CT2;

    std::array<std::uint_fast32_t, 4U> N =
    {{
      UINT32_C(3),
      UINT32_C(2),
      UINT32_C(1),
      UINT32_C(0)
    }};

    std::array<T, 3U> G;

    // C
    // C FOR I=3,...,N , THE VALUES A(I) AND B(I) ARE CALCULATED
    // C USING THE RECURRENCE RELATIONS BELOW.
    // C

    for(std::uint_fast32_t I = UINT32_C(0); I < UINT32_C(10000); ++I)
    {
      // C
      // C CALCULATION OF THE MULTIPLIERS FOR THE RECURSION
      // C

      // Use simplification with computer algebra to reduce
      // the total work of each recursion cycle from:
      //  8 multiplications and 7 divisions ( 8 + (7 * 3)) = 29 to
      // 10 multiplications and 4 divisions (10 + (4 * 3)) = 22,
      // where division is assumed to require 3 times more work
      // than multiplication does.

      CT2   = Z2 / (CT1 * (CP + N[1U]));
      G[0U] = 1U + (CT2 * (N[2U] - AP));
      CT2   = (CT2 * (AP + N[1U])) / (CP + N[2U]);
      G[1U] = CT2 * ((CP - N[1U]) + (((AP + N[0U]) / (CT1 + 2U)) * Z2));
      G[2U] = ((CT2 * (AP - N[2U])) * ((AP + N[2U]) * (Z2 * Z2))) / (((-2 + CT1) * CT1) * (CP + N[3U]));

      // C -----------------------------------------------------------------
      // C THE RECURRENCE RELATIONS FOR A(I) and B(I) ARE AS FOLLOWS
      // C -----------------------------------------------------------------

      A[3U] = std::inner_product(G.crbegin(), G.crend(), A.cbegin(), T(0U));
      B[3U] = std::inner_product(G.crbegin(), G.crend(), B.cbegin(), T(0U));

      // Check if the iteration difference (delta) is within
      // tolerance and break from the recursion if it is.
      // Here we analyze the difference between this iteration
      // and the previous iteration using:
      // [(A3/B3) - (A2/B2)] / (A3/B3) = 1 - [(A2*B3)/(A3*B2)].

      using std::fabs;

      const T ratio = (A[2U] * B[3U]) / (A[3U] * B[2U]);
      const T delta = 1U - fabs(ratio);

      if((I > UINT32_C(4)) && (fabs(delta) < std::numeric_limits<T>::epsilon()))
      {
        break;
      }

      // Shift the arrays for the next recursion.
      std::copy(A.begin() + 1U, A.end(), A.begin());
      std::copy(B.begin() + 1U, B.end(), B.begin());

      std::copy(N.rbegin() + 1U, N.rend(), N.rbegin());

      ++N[0U];

      CT1 += UINT32_C(2);
    }

    // Return the rational approximation. This is given
    // by the ratio of the final recursions of A and B.
    return A.back() / B.back();
  }
}

template<typename T>
T hypergeometric_1f1(const T& a, const T& b, const T& z)
{
  if(z < 0U)
  {
    // Use rational approximation for negative argument.
    return local::hypergeometric_1f1_rational_approx(a, b, z);
  }
  else if(z > 0U)
  {
    // Use reflection and rational approximation for positive argument.
    using std::exp;

    return exp(z) * hypergeometric_1f1(T(b - a), b, T(-z));
  }
  else
  {
    // The argument is exactly 0. Return 1.
    return T(1U);
  }
}

int main()
{
  typedef boost::fixed_point::negatable<25, -230> fixed_point_type;

  const fixed_point_type a(fixed_point_type(2U) / 3U);
  const fixed_point_type b(fixed_point_type(4U) / 3U);
  const fixed_point_type z(fixed_point_type(3U) / 4U);

  std::cout << std::setprecision((-fixed_point_type::resolution * 301L) / 1000L)
            << hypergeometric_1f1(a, b, z)
            << std::endl;
}

// hypergeometric_1f1(2/3, 4/3, 3/4), 69 digits =
// 1.
// 49919305157788885430792586775321807572342038234724813773626117774756
