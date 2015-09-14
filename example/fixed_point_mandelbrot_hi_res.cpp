
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
#include <iomanip>
#include <iostream>
#include <iterator>
#include <vector>
#include <boost/cstdint.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <drv/vgx_drv_windows.h>

template<const int TotalDigits,
         const boost::uint_fast16_t MaxIterations,
         const int MandelbrotFractionalResolution>
struct mandelbrot_configuration
{
  typedef boost::fixed_point::negatable<16, 16 + 1 - TotalDigits> fixed_point_type;

  BOOST_STATIC_CONSTEXPR boost::uint_fast16_t max_iterations = MaxIterations;

  BOOST_STATIC_CONSTEXPR int mandelbrot_fractional_resolution = MandelbrotFractionalResolution;

  BOOST_STATIC_CONSTEXPR boost::uint16_t width  = static_cast<boost::uint16_t>(2.5L * static_cast<long double>(1UL << (-mandelbrot_fractional_resolution)));
  BOOST_STATIC_CONSTEXPR boost::uint16_t height = static_cast<boost::uint16_t>(2.0L * static_cast<long double>(1UL << (-mandelbrot_fractional_resolution)));

  static vgx::head::windows<width, height,
                            width, height>& head()
  {
    static vgx::head::windows<width, height,
                              width, height>
    the_head(0, 0, 128, 128, 1, 1);

    return the_head;
  }
};

// This line configures the Mandelbrot generator.
typedef mandelbrot_configuration<128, UINT16_C(1000), -10> mandelbrot_configuration_type;

#define BOOST_CSTDFLOAT_EXTENDED_COMPLEX_FLOAT_TYPE mandelbrot_configuration_type::fixed_point_type
#include <boost/math/cstdfloat/cstdfloat_complex_std.hpp>
#undef BOOST_CSTDFLOAT_EXTENDED_COMPLEX_FLOAT_TYPE

template<typename NumericType,
         typename AllocatorType = std::allocator<NumericType>>
class mandelbrot_generator
{
public:
  static_assert(std::is_same<typename AllocatorType::value_type, NumericType>::value,
                "Error: Something is wrong with the allocator type provided to the mandelbrot_generator template class.");

  static void generate_mandelbrot_image()
  {
    const NumericType x_lo = NumericType(-2);
    const NumericType x_hi = NumericType(+1) / 2;

    const NumericType y_lo = NumericType(-1);
    const NumericType y_hi = NumericType(+1);

    const NumericType step = ldexp(NumericType(1), mandelbrot_configuration_type::mandelbrot_fractional_resolution);

    // Setup the x-axis coordinates.
    std::vector<NumericType> x_values(mandelbrot_configuration_type::width);

    // Initialize the x-axis coordinates (one time only).
    {
      NumericType x = x_lo;
      for(boost::uint_fast16_t col = UINT16_C(0); col < mandelbrot_configuration_type::width; ++col, x += step)
      {
        x_values[col] = x;
      }
    }

    // Create storage for the row pixel results.
    std::vector<boost::uint32_t> row_pixel_color_values(mandelbrot_configuration_type::width);

    // Initialize the y-axis coordinate.
    NumericType y = y_hi;

    // TBD: The iteration through rows should be distributed in multithreading.

    // Loop through all the rows of pixels on the vertical y-axis
    // in the direction of decrementing the y-value.
    for(boost::uint_fast16_t row = UINT16_C(0); row < mandelbrot_configuration_type::height; ++row, y -= step)
    {
      auto row_pixel_color_iterator = row_pixel_color_values.begin();

      // Loop through this column of pixels on the horizontal x-axis
      // in the direction of incrementing the x-value.
      std::for_each(x_values.cbegin(),
                    x_values.cend(),
                    [&y, &row_pixel_color_iterator](const NumericType& x)
                    {
                      std::complex<NumericType> z(NumericType(0));

                      const std::complex<NumericType> c(x, y);

                      boost::uint_fast16_t i = UINT16_C(0);

                      NumericType zr_sqr(0);
                      NumericType zi_sqr(0);

                      // Use a highly optimized complex-numbered multiplication scheme here.
                      // Thereby reduce the main work of complex multiplication to
                      // only 3 real-valued multiplication operations and 4 real-valued
                      // addition/subtraction operations.

                      for( ; (((zr_sqr + zi_sqr) < 4) && (i < mandelbrot_configuration_type::max_iterations)); ++i)
                      {
                        z.imag(z.real() * z.imag());
                        z.imag(z.imag() + z.imag() + c.imag());

                        z.real((zr_sqr - zi_sqr) + c.real());

                        zr_sqr = z.real() * z.real();
                        zi_sqr = z.imag() * z.imag();
                      }

                      *row_pixel_color_iterator = get_color(i);

                      ++row_pixel_color_iterator;
                    });

      std::cout << "Calculating Mandelbrot image at row "
                << std::setw(6)
                << (row + 1U)
                << " of "
                << std::setw(6)
                << mandelbrot_configuration_type::height
                << " total. Have patience."
                << "\r";

      // Write this entire row of colors to the output.
      {
        boost::int16_t col_index = INT16_C(0);

        // Set the pixels in memory before driving them to the output.
        std::for_each(row_pixel_color_values.cbegin(),
                      row_pixel_color_values.cend(),
                      [&col_index, &row](const boost::uint32_t the_color)
                      {
                        mandelbrot_configuration_type::head().drv_pixel_set_color(col_index, row, the_color);

                        ++col_index;
                      });

        // Drive this row of pixels to the output.
        mandelbrot_configuration_type::head().drv_primitive_done();
      }
    }

    std::cout << std::endl;
  }

private:
  static boost::uint32_t get_color(const boost::uint_least16_t i)
  {
    // Blend a classic black-and-white color scheme.

    static std::vector<NumericType, AllocatorType> scale_factors;

    // Initialize the color scale factors.
    // TBD: Do this once only before pre-main static initialization.
    if(scale_factors.empty())
    {
      scale_factors.resize(static_cast<std::size_t>(mandelbrot_configuration_type::max_iterations + 1U), NumericType(0));

      static const NumericType fractional_power = NumericType(1) / 5;

      for(std::size_t fi = UINT16_C(1); fi < scale_factors.size(); ++fi)
      {
        using std::pow;

        scale_factors[fi] = pow(NumericType(fi) / mandelbrot_configuration_type::max_iterations, fractional_power);
      }
    }

    const boost::uint8_t scaled_color = static_cast<boost::uint8_t>(float(0xFF) * (1.0F - scale_factors[i]));

    const boost::uint32_t gray_tone =   (boost::uint32_t(scaled_color) <<  0)
                                      | (boost::uint32_t(scaled_color) <<  8)
                                      | (boost::uint32_t(scaled_color) << 16);

    return boost::uint32_t(gray_tone);
  }
};

int main()
{
  typedef mandelbrot_configuration_type::fixed_point_type fixed_point_type;

  mandelbrot_configuration_type::head().init();

  const std::clock_t start = std::clock();

  mandelbrot_generator<fixed_point_type,
                       std::allocator<fixed_point_type>>::generate_mandelbrot_image();

  const std::clock_t stop = std::clock();

  const float elapsed = (float(stop) - float(start)) / float(CLOCKS_PER_SEC);

  std::cout << "Time for calculation: "
            << elapsed
            << "s"
            << std::endl;

  std::cout << "Enter any character to quit: ";
  char c;
  std::cin >> c;
}

/*
#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL
#include <boost/gil/gil_all.hpp>
#include <boost/gil/extension/io/png_dynamic_io.hpp>
using namespace boost::gil;
int main()
{
    rgb8_image_t img(512, 512);
    rgb8_pixel_t red(255, 0, 0);
    fill_pixels(view(img), red);
    png_write_view("redsquare.png", const_view(img));
}
*/
