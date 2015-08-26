
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Christopher Kormanyos 2015.
// Copyright Paul A. Bristow 2015.

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

// This file also includes Doxygen-style documentation about the function of the code.
// See http://www.doxygen.org for details.

//! \file

//! \brief Example program showing fixed-point text-based Mandelbrot calculation with low resolution.

// Below are snippets of code that are included into Quickbook file fixed_point.qbk.

#include <fstream>
#include <boost/cstdint.hpp>
#include <boost/fixed_point/fixed_point.hpp>

typedef boost::fixed_point::negatable<16, -14>      fixed_point_type;
#define BOOST_CSTDFLOAT_EXTENDED_COMPLEX_FLOAT_TYPE fixed_point_type

#include <boost/math/cstdfloat/cstdfloat_complex_std.hpp>


template<typename NumericType>
void generate_mandelbrot_image(std::ostream& out)
{
  const NumericType x_lo = NumericType(-2);
  const NumericType x_hi = NumericType(+1) / 2;

  const NumericType y_lo = NumericType(-1);
  const NumericType y_hi = NumericType(+1);

  const NumericType delta_x = ldexp(NumericType(1), - 5);
  const NumericType delta_y = ldexp(NumericType(1), - 4);

  for(NumericType y = y_hi; y > y_lo; y -= delta_y)
  {
    for(NumericType x = x_lo; x < x_hi; x += delta_x)
    {
      std::complex<NumericType> z(NumericType(0));

      for(unsigned i = 0U; i < 20U; ++i)
      {
        z = (z * z) + std::complex<NumericType>(x, y);
      }

      const boost::uint_fast16_t color = static_cast<boost::uint_fast16_t>(std::abs(z));

      out.put((color < UINT16_C(2)) ? char('#') : char('.'));
    }

    out << std::endl;
  }
}

int main()
{
  std::ofstream out("mandelbrot.txt");

  if(out.is_open())
  {
    generate_mandelbrot_image<fixed_point_type>(out);

    out.close();
  }
}
