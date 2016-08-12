// Copyright Christopher Kormanyos 2016.
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include <array>
#include <chrono>
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
    // Implement a rational approximation of hypergeometric_1f1.
    // This C++11 code uses a computational scheme similar to
    // one shown in Y.L. Luke, "Algorithms for the Computation
    // of Mathematical Functions", Academic Press, New York, 1977.

    const T Z(-ZM);

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

    std::uint_fast16_t CT1 = UINT16_C(3);
    T CT2;

    std::array<std::uint_fast16_t, 4U> N =
    {{
      UINT16_C(3),
      UINT16_C(2),
      UINT16_C(1),
      UINT16_C(0)
    }};

    std::array<T, 3U> G;

    // C
    // C FOR I=3,...,N , THE VALUES A(I) AND B(I) ARE CALCULATED
    // C USING THE RECURRENCE RELATIONS BELOW.
    // C

    const T z2z2 = (Z2 * Z2);

    for( ; N[0U] < UINT16_C(10000); ++N[0U])
    {
      // C
      // C CALCULATION OF THE MULTIPLIERS FOR THE RECURSION
      // C

      // Use simplification with computer algebra and integer values
      // where possible to reduce the total work of the following
      // calculations from:
      // 8 multiplications and 7 divisions (8 + (7 * 3)) = 29 to
      // 7 multiplications and 4 divisions (7 + (4 * 3)) = 19,
      // where division is assumed to require 3 times more work
      // than multiplication does.

      CT2   = Z2 / (CT1 * (CP + N[1U]));
      G[0U] = 1U + (CT2 * (N[2U] - AP));
      CT2   = (CT2 * (AP + N[1U])) / (CP + N[2U]);
      G[1U] = CT2 * ((CP - N[1U]) + (((AP + N[0U]) / (CT1 + 2U)) * Z2));
      G[2U] = ((CT2 * (AP - N[2U])) * ((AP + N[2U]) * (z2z2))) / (std::uint32_t(std::uint32_t(CT1 - 2U) * CT1) * (CP + N[3U]));

      // C -----------------------------------------------------------------
      // C THE RECURRENCE RELATIONS FOR A(I) and B(I) ARE AS FOLLOWS
      // C -----------------------------------------------------------------

      A[3U] = std::inner_product(G.crbegin(), G.crend(), A.cbegin(), T(0U));
      B[3U] = std::inner_product(G.crbegin(), G.crend(), B.cbegin(), T(0U));

      std::copy(N.rbegin() + 1U, N.rend(), N.rbegin());

      if(N[3U] > UINT16_C(2))
      {
        // Check if the iteration difference (delta) is within
        // tolerance and break from the recursion if it is.
        // Here we analyze the difference between this iteration
        // result and the previous iteration result using:
        //
        //         |     |(A2/B2)]| |
        // delta = | 1 - |--------| |
        //         |     |(A3/B3) | |
        //
        //         |     |(A2*B3)| |
        //       = | 1 - |-------| |,
        //         |     |(A3*B2)| |
        //
        // where the absolute value of the ratio in the second term
        // is used and the absolute value of delta is used.

        using std::fabs;

        const T ratio = fabs((A[2U] * B[3U]) / (A[3U] * B[2U]));
        const T delta = fabs(1U - ratio);

        if(delta < std::numeric_limits<T>::epsilon())
        {
          break;
        }
      }

      // Shift the arrays for the next recursion.
      std::copy(A.begin() + 1U, A.end(), A.begin());
      std::copy(B.begin() + 1U, B.end(), B.begin());

      CT1 += UINT16_C(2);
    }

    // Return the rational approximation. This is given
    // by the ratio of the final recursions of A and B.
    return ((N[0U] < UINT16_C(10000)) ? (A.back() / B.back()) : T(0U));
  }

  template<typename clock_type>
  struct stopwatch final
  {
  public:
    typedef typename clock_type::duration duration_type;

    stopwatch() : my_start(clock_type::now()) { }

    stopwatch(const stopwatch& other) : my_start(other.my_start) { }

    stopwatch& operator=(const stopwatch& other)
    {
      if(this != &other)
      {
        my_start = other.my_start;
      }

      return *this;
    }

    ~stopwatch() { }

    template<typename count_type>
    count_type elapsed() const
    {
      const duration_type my_elapsed = (clock_type::now() - my_start);

      return std::chrono::duration_cast<std::chrono::duration<count_type>>(my_elapsed).count();
    }

    void reset()
    {
      my_start = clock_type::now();
    }

  private:
    typename clock_type::time_point my_start;
  };
}

template<typename T>
T hypergeometric_1f1(const T& a, const T& b, const T& z)
{
  if(z < 0U)
  {
    // Use the rational approximation for negative argument.
    return local::hypergeometric_1f1_rational_approx(a, b, z);
  }
  else if(z > 0U)
  {
    // Use reflection and the rational approximation for positive argument.
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
  typedef boost::fixed_point::negatable<122, -3200> fixed_point_type;

  const std::uintmax_t fixed_point_resolution_digits10((std::uintmax_t(-fixed_point_type::resolution) * UINTMAX_C(301)) / UINTMAX_C(1000));

  const fixed_point_type a( fixed_point_type(2U) / 3U);
  const fixed_point_type b( fixed_point_type(4U) / 3U);
  const fixed_point_type z(-fixed_point_type(3U) / 4U);

  const local::stopwatch<std::chrono::high_resolution_clock> timer;

  const fixed_point_type h = hypergeometric_1f1(a, b, z);

  const float elapsed = timer.elapsed<float>();

  std::cout << std::setprecision(std::streamsize(fixed_point_resolution_digits10))
            << h
            << std::endl
            << "Run time:     "
            << std::setprecision(3)
            << elapsed << " sec, "
            << " showing " << std::streamsize(fixed_point_resolution_digits10) << " decimal digits."
            << std::endl;
}

// hypergeometric_1f1(2/3, 4/3, -3/4) = approx.
// 0.
// 7081686536671296176070489154441892975263864254727871399151279335156755130218980066854878718659644876
// 1305121691418288067229656427737120644431158087451536612475482465245551262861209703714089698028175454
// 0070644713429820469109420541161232888933036256421986450730383363242819718385845578123146346562416912
// 8459987657984935712740682227830255529693892427453550857117109315771584082876374760460718792291350348
// 5190554952923571564136649540077735757762129258093268390205343996981829581788815350317949553374856332
// 4600791123444332864835615846878224843114597006279577350905460171544671226390900903288844703332800883
// 7461634557128794965583053125950999187778364988189708317696297046872660631251555306433290041457209364
// 7846449827307910707408292805642488619055129649252986312848264653807329387537374580180299791208841957
// 4522132120358559031634677139462281867313156223643446521436399244700681185183813207181756167736113034
// 857827184751921039509821261325652834885514775332851906824822638...
