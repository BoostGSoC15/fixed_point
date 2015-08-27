
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

//! \brief Example program showing fixed-point text-based Mandelbrot calculation with high resolution.

// Below are snippets of code that are included into Quickbook file fixed_point.qbk.

#include <algorithm>
#include <ctime>
#include <iostream>
#include <iterator>
#include <vector>
#include <boost/cstdint.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <drv/vgx_drv_windows.h>

typedef boost::fixed_point::negatable<16, -(53 - 16)> fixed_point_type;
#define BOOST_CSTDFLOAT_EXTENDED_COMPLEX_FLOAT_TYPE   fixed_point_type

#include <boost/math/cstdfloat/cstdfloat_complex_std.hpp>

vgx::head::windows<2048 * 2,
                   1586 * 2,
                   2048 * 2,
                   1586 * 2>
head(0, 0, 100, 100, 1, 2);

template<const boost::uint_fast16_t MaxIterations = UINT16_C(1000)>
boost::uint32_t get_color(const boost::uint_least16_t i)
{
  BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t max_iterations = MaxIterations;

  // Blend a pretty color scheme.
  const double t = double(i) / float(max_iterations);

  using std::pow;
  const double factor = pow(t, 0.2);

  const boost::uint8_t pigment_mix_green  = static_cast<boost::uint8_t>(double(0xFF) * (1.0 - factor));
  const boost::uint8_t pigment_mix_purple = static_cast<boost::uint8_t>(double(0xFF) * factor);

  const boost::uint32_t color_mix_green   =   (UINT32_C(0x00) <<  0)
                                            | (boost::uint32_t(pigment_mix_green)  <<  8)
                                            | (UINT32_C(0x00) <<  16);

  const boost::uint32_t color_mix_purple  =   (boost::uint32_t(pigment_mix_purple) <<  0)
                                            | (UINT32_C(0x00) <<  8)
                                            | (boost::uint32_t(pigment_mix_purple) << 16);

  return color_mix_green + color_mix_purple;
}

template<typename NumericType,
         const int ResolutionX,
         const int ResolutionY,
         const boost::uint_fast16_t MaxIterations = UINT16_C(1000)>
void generate_mandelbrot_image()
{
  const NumericType x_lo = NumericType(-2);
  const NumericType x_hi = NumericType(+1) / 2;

  const NumericType y_lo = NumericType(-1);
  const NumericType y_hi = NumericType(+1);

  const NumericType x_step = ldexp(NumericType(1), ResolutionX);
  const NumericType y_step = ldexp(NumericType(1), ResolutionY);

  const boost::uint_fast16_t width  = static_cast<boost::uint_fast16_t>((x_hi - x_lo) / x_step);
  const boost::uint_fast16_t height = static_cast<boost::uint_fast16_t>((y_hi - y_lo) / y_step);

  BOOST_CONSTEXPR_OR_CONST boost::uint_fast16_t max_iterations = MaxIterations;

  NumericType y = y_hi;

  for(boost::uint_fast16_t row = UINT16_C(0); row < height; ++row, y -= y_step)
  {
    std::vector<boost::uint32_t> color_values(width);

    NumericType x = x_lo;

    for(boost::uint_fast16_t col = UINT16_C(0); col < width; ++col, x += x_step)
    {
      std::complex<NumericType> z(NumericType(0));

      const std::complex<NumericType> c(x, y);

      boost::uint_fast16_t i = UINT16_C(0);

      NumericType zr_sqr(0);
      NumericType zi_sqr(0);

      for( ; (((zr_sqr + zi_sqr) < 4) && (i < max_iterations)); ++i)
      {
        z.imag(z.real() * z.imag());
        z.imag(z.imag() + z.imag());
        z.imag(z.imag() + c.imag());

        z.real((zr_sqr - zi_sqr) + c.real());

        zr_sqr = z.real() * z.real();
        zi_sqr = z.imag() * z.imag();
      }

      color_values[col] = get_color<max_iterations>(i);
    }

    // Write the entire row of colors to the output.
    {
      boost::int16_t col_index = INT16_C(0);

      std::for_each(color_values.cbegin(),
                    color_values.cend(),
                    [&col_index, &row](const boost::uint32_t the_color)
                    {
                      head.drv_pixel_set_color(col_index, row, the_color);

                      ++col_index;
                    });
    }

    head.drv_primitive_done();
  }
}

int main()
{
  head.init();

  std::cout << "Calculating Mandelbrot image. Please have patience..." << std::endl;

  const std::clock_t start = std::clock();

  generate_mandelbrot_image<fixed_point_type, -10, -9, 1000>();

  const std::clock_t stop = std::clock();

  std::cout << "Time for calculation: "
            << (double(stop) - double(start)) / double(CLOCKS_PER_SEC)
            << "s"
            << std::endl;

  std::cout << "Enter any character to quit: ";
  char c;
  std::cin >> c;
}
