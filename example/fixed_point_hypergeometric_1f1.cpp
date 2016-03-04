
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Christopher Kormanyos 2016.

#include <algorithm>
#include <array>
#include <cmath>
#include <ctime>
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
      const T delta = (A[3U] / B[3U]) - (A[2U] / B[2U]);

      using std::fabs;
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
  typedef boost::fixed_point::negatable<25, -230> numeric_type;

  const numeric_type a(numeric_type(2U) / 3U);
  const numeric_type b(numeric_type(4U) / 3U);
  const numeric_type z(numeric_type(3U) / 4U);

  // TBD: Use <chrono> functions here.
  const std::time_t start = std::clock();
  const numeric_type h = hypergeometric_1f1(a, b, z);
  const std::time_t stop  = std::clock();

  const float elapsed = (float(stop) - float(start)) / float(CLOCKS_PER_SEC);

  std::cout << std::setprecision(std::numeric_limits<numeric_type>::digits10)
            << h
            << std::endl
            << "time: "
            << std::setprecision(6)
            << elapsed
            << std::endl;
}

// hypergeometric_1f1(2/3, 4/3, 3/4) = approx. (2001 digits)
// 1.
// 4991930515778888543079258677532180757234203823472481377362611777475646096590011211954363123411075368
// 0846286096787492391924270574347303971306083345949356025777566612176061307448157944520069800535171068
// 0447232308437081652427770052478300690072184196947863121063511100532382160320226268394272097086282361
// 1377829501676277319877298867104411759181435082414982543727003368985195692688950068969394455255793911
// 8934569076420064384919940569588193966339104354461419590642334218123558457321344488998886937584158755
// 4085685292996395345233436769437757713736343663246151550740436474023141918088535987643678813730996284
// 9680339693568315632746448481366164059219049898405039003675837598549855929518623260080194308350126466
// 3923847217544222014815918363738790502564022669915484400980354917697424454328531643381153076531047050
// 0739102767965228761310386478561877949450964134248372171371035283556290037525384421987280947099391201
// 9076569429803364881626084880723588660423221208116035638687636668386249623612002067914021469872598540
// 0796249874670038256704263018503485851546341126458441212650840915950837191323506934917740334005596002
// 4344548034565713560161625179302782203524786990821372029200364422249355557042106667017012676557329128
// 0310745006017963507494655628033913857446809329174117587950584430719041998377239325690181423546817886
// 9425342935300363590432823063900211161130543649724441934083779031784122780833832489313977070682126487
// 3730742324362739779617308607623087601766103092307455652175603685686974023158773350278978264708827202
// 9264725944733754962535399915677500058435827292001659648380949273050065609696806728900635503033024886
// 9062118086878812312407076989329010227289632890252227983669111452043137502839286187451305085301483296
// 5824702783183869777035573909299300784897167684106753643995352794652221591168906024679799721242760437
// 1209092444321983790459274461650315012561832693303643443685031037756261111746121239895505446727605914
// 7825322487913837194680053907830748467022742101846700691219038769368014656358646594690788298884443913
