
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.gray
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
#include <numeric>
#include <vector>

#include <boost/cstdint.hpp>
#include <boost/fixed_point/fixed_point.hpp>
#include <boost/gil/extension/io/jpeg_io.hpp>
#include <boost/gil/image.hpp>
#include <boost/gil/typedefs.hpp>

class mandelbrot_configuration_base
{
public:
  virtual ~mandelbrot_configuration_base() { }

  long double x_lo() const { return my_x_lo; }
  long double x_hi() const { return my_x_hi; }
  long double y_lo() const { return my_y_lo; }
  long double y_hi() const { return my_y_hi; }

  virtual boost::uint_fast16_t max_iterations() const = 0;

  virtual int mandelbrot_fractional_resolution() const = 0;

  virtual long double step() const = 0;

  virtual boost::uint_fast32_t width () const = 0;
  virtual boost::uint_fast32_t height() const = 0;

  virtual boost::uint_fast32_t red_hue  () const = 0;
  virtual boost::uint_fast32_t green_hue() const = 0;
  virtual boost::uint_fast32_t blue_hue () const = 0;

protected:
  const long double my_x_lo;
  const long double my_x_hi;
  const long double my_y_lo;
  const long double my_y_hi;

  mandelbrot_configuration_base(const long double xl, const long double xh,
                                const long double yl, const long double yh) : my_x_lo(xl), my_x_hi(xh),
                                                                              my_y_lo(yl), my_y_hi(yh) { }
private:
  mandelbrot_configuration_base() : my_x_lo(0.0L), my_x_hi(0.0L),
                                    my_y_lo(0.0L), my_y_hi(0.0L) { }
};

template<const int TotalDigits,
         const boost::uint_fast16_t MaxIterations,
         const int MandelbrotFractionalResolution,
         const boost::uint_fast32_t RedHue,
         const boost::uint_fast32_t GreenHue,
         const boost::uint_fast32_t BlueHue>
class mandelbrot_configuration : public mandelbrot_configuration_base
{
public:
  static_assert(    (RedHue   < UINT32_C(256))
                 && (GreenHue < UINT32_C(256))
                 && (BlueHue  < UINT32_C(256)), "The color hue parameters must be 255 or smaller.");

  typedef boost::fixed_point::negatable<16, 16 + 1 - TotalDigits> fixed_point_type;

  mandelbrot_configuration(const long double xl, const long double xh,
                           const long double yl, const long double yh)
    : mandelbrot_configuration_base(xl, xh, yl, yh) { }

  virtual ~mandelbrot_configuration() { }

private:
  virtual boost::uint_fast16_t max_iterations() const { return MaxIterations; }

  virtual int mandelbrot_fractional_resolution() const { return MandelbrotFractionalResolution; }

  virtual long double step() const { return 1.0L / (UINT64_C(1) << -mandelbrot_fractional_resolution()); }

  virtual boost::uint_fast32_t width () const { return static_cast<boost::uint_fast32_t>((x_hi() - x_lo()) / step()); }
  virtual boost::uint_fast32_t height() const { return static_cast<boost::uint_fast32_t>((y_hi() - y_lo()) / step()); }

  virtual boost::uint_fast32_t red_hue  () const { return RedHue; }
  virtual boost::uint_fast32_t green_hue() const { return GreenHue; }
  virtual boost::uint_fast32_t blue_hue () const { return BlueHue; }
};

template<typename NumericType>
class mandelbrot_generator
{
public:
  mandelbrot_generator(const mandelbrot_configuration_base& config)
    : mandelbrot_configuration_object(config),
      mandelbrot_image               (config.width(), config.height()),
      mandelbrot_view                (boost::gil::rgb8_view_t()),
      mandelbrot_iteration_matrix    (mandelbrot_configuration_object.width(),
                                      std::vector<boost::uint_fast16_t>(mandelbrot_configuration_object.height())),
      mandelbrot_color_histogram     (static_cast<std::size_t>(config.max_iterations() + 1U), UINT32_C(0))
  {
    mandelbrot_view = boost::gil::view(mandelbrot_image);
  }

  ~mandelbrot_generator() { }

  void generate_mandelbrot_image()
  {
    // Setup the x-axis coordinates.
    std::vector<NumericType> x_values(mandelbrot_configuration_object.width());

    // Initialize the x-axis coordinates (one time only).
    {
      NumericType x_step(mandelbrot_configuration_object.x_lo());

      for(NumericType& x : x_values)
      {
        x = x_step;

        x_step += mandelbrot_configuration_object.step();
      }
    }

    // Initialize the y-axis coordinate.
    NumericType y(mandelbrot_configuration_object.y_hi());

    // TBD: The iteration through rows can be distributed in multithreading.

    // Loop through all the rows of pixels on the vertical
    // y-axis in the direction of decreasing y-value.
    for(boost::uint_fast32_t row = UINT32_C(0); row < mandelbrot_configuration_object.height(); ++row, y -= mandelbrot_configuration_object.step())
    {
      // Loop through this column of pixels on the horizontal
      // x-axis in the direction of increasing x-value.
      boost::uint_fast32_t col = UINT32_C(0);

      std::for_each(x_values.cbegin(),
                    x_values.cend(),
      [&y, &col, &row, this](const NumericType& x)
      {
        const NumericType cr(x);
        const NumericType ci(y);

        NumericType zr(0);
        NumericType zi(0);

        NumericType zr_sqr(0);
        NumericType zi_sqr(0);

        // Use an optimized complex-numbered multiplication scheme.
        // Thereby reduce the main work of the Mandelbrot iteration to
        // three real-valued multiplications and several real-valued
        // addition/subtraction operations.

        boost::uint_fast16_t i = UINT16_C(0);

        // Perform the iteration sequence for generating the Mandelbrot set.
        // Herein lies the work of the program.
        // TBD: This can easily be distributed to parallel processes.

        while(   (i < mandelbrot_configuration_object.max_iterations())
              && ((zr_sqr + zi_sqr) < 4))
        {
          zi *= zr;
          zi  = (zi + zi) + ci;

          zr = (zr_sqr - zi_sqr) + cr;

          zr_sqr = zr * zr;
          zi_sqr = zi * zi;

          ++i;
        }

        mandelbrot_iteration_matrix[col][row] = i;

        ++mandelbrot_color_histogram[i];

        ++col;
      });

      std::cout << "Calculating Mandelbrot image at row "
                << std::setw(6)
                << (row + 1U)
                << " of "
                << std::setw(6)
                << mandelbrot_configuration_object.height()
                << " total. Have patience."
                << "\r";
    }

    const boost::uint_fast32_t total = boost::uint_fast32_t(mandelbrot_configuration_object.width()) * mandelbrot_configuration_object.height();

    // Perform color-stretching using the histogram approach.
    // Convert the histogram entries such that a given entry contains
    // the sum of its own entries plus all previous entries. This provides
    // a set of scale factors for the color. The histogram approach
    // automatically scales to the distribution of pixels in the image.

    std::accumulate(mandelbrot_color_histogram.begin(),
                    mandelbrot_color_histogram.end(),
                    boost::uint_fast32_t(0U),
    [&total](boost::uint_fast32_t& sum, boost::uint_fast32_t& histogram_entry) -> boost::uint_fast32_t
    {
      sum += histogram_entry;

      histogram_entry = UINT32_C(0xFF) - static_cast<boost::uint_fast32_t>((boost::uint64_t(sum) * 0xFFU) / total);

      return sum;
    });

    for(boost::uint_fast32_t row = UINT32_C(0); row < mandelbrot_configuration_object.height(); ++row)
    {
      for(boost::uint_fast32_t col = UINT32_C(0); col < mandelbrot_configuration_object.width(); ++col)
      {
        const boost::uint_fast32_t color = mandelbrot_color_histogram[mandelbrot_iteration_matrix[col][row]];

        // Mix the color supplied in the template hue parameters.
        const boost::uint8_t rh = static_cast<boost::uint8_t>((mandelbrot_configuration_object.red_hue  () * color) / UINT32_C(255));
        const boost::uint8_t gh = static_cast<boost::uint8_t>((mandelbrot_configuration_object.green_hue() * color) / UINT32_C(255));
        const boost::uint8_t bh = static_cast<boost::uint8_t>((mandelbrot_configuration_object.blue_hue () * color) / UINT32_C(255));

        const boost::gil::rgb8_pixel_t the_color  = boost::gil::rgb8_pixel_t(rh, gh, bh);

        mandelbrot_view(col, row) = boost::gil::rgb8_pixel_t(the_color);
      }
    }

    boost::gil::jpeg_write_view("mandelbrot.jpg", mandelbrot_view);

    std::cout << std::endl
              << "The ouptput file mandelbrot.jpg has been written"
              << std::endl;
  }

private:
  const mandelbrot_configuration_base&           mandelbrot_configuration_object;
  boost::gil::rgb8_image_t                       mandelbrot_image;
  boost::gil::rgb8_view_t                        mandelbrot_view;
  std::vector<std::vector<boost::uint_fast16_t>> mandelbrot_iteration_matrix;
  std::vector<boost::uint_fast32_t>              mandelbrot_color_histogram;
};

int main()
{
  // This is the classic full immage rendered in baby-blue tones (and black).
  //typedef mandelbrot_configuration<128, UINT16_C(10000), -10,
  //                                 UINT32_C(80),
  //                                 UINT32_C(255),
  //                                 UINT32_C(255)> mandelbrot_configuration_type;

  //const mandelbrot_configuration_type mandelbrot_configuration_object(-2.000L, +0.500L,
  //                                                                    -1.000L, +1.000L);

  // This is a fanning image rendered in fuschia tones (and black).
  typedef mandelbrot_configuration<128, UINT16_C(10000), -21,
                                   UINT32_C(255),
                                   UINT32_C(0),
                                   UINT32_C(210)> mandelbrot_configuration_type;

  const mandelbrot_configuration_type mandelbrot_configuration_object(-0.749730L - 0.0002315L, -0.749730L + 0.0002315L,
                                                                      -0.046608L - 0.0002315L, -0.046608L + 0.0002315L);

  // This is a swirly rectangular image rendered in gray tones (and black).
  //typedef mandelbrot_configuration<128, UINT16_C(10000), -45,
  //                                 UINT32_C(255),
  //                                 UINT32_C(255),
  //                                 UINT32_C(255)> mandelbrot_configuration_type;

  //const mandelbrot_configuration_type mandelbrot_configuration_object(-0.745398360667L - 1.25E-11L, -0.745398360667L + 1.25E-11L,
  //                                                                    +0.112504634996L - 1.25E-11L, +0.112504634996L + 1.25E-11L);

  typedef mandelbrot_configuration_type::fixed_point_type mandelbrot_numeric_type;

  typedef mandelbrot_generator<mandelbrot_numeric_type> mandelbrot_generator_type;

  const std::clock_t start = std::clock();

  mandelbrot_generator_type* the_mandelbrot_generator = new mandelbrot_generator_type(mandelbrot_configuration_object);

  the_mandelbrot_generator->generate_mandelbrot_image();

  const float elapsed = (float(std::clock()) - float(start)) / CLOCKS_PER_SEC;

  std::cout << "Time for calculation: "
            << elapsed
            << "s"
            << std::endl;

  delete the_mandelbrot_generator;
}
