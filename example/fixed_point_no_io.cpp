
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// Copyright Paul A. Bristow 2015.
// Copyright Christopher Kormanyos 2015.
// Copyright Nikhar Agrawal 2015.


// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

// This file also includes Doxygen-style documentation about the function of the code.
// See http://www.doxygen.org for details.

//! \file

// Below are snippets of code that can be included into a Quickbook file.

/*
  \brief Demonstrate use of macro BOOST_FIXED_POINT_DISABLE_IOSTREAM.

  \details BOOST_FIXED_POINT_DISABLE_IOSTREAM defined to disable
  all I / O streaming and the inclusion of associated standard
  library headers.This is intended to eliminate I / O stream
  overhead, in particular for bare - metal microcontroller projects.
*/

#define BOOST_FIXED_POINT_DISABLE_IOSTREAM

#include <boost/fixed_point/fixed_point.hpp>

typedef boost::fixed_point::negatable<15, -16> fixed_point_type;

int main()
{
  fixed_point_type x(4.2);
  // Do something useful - but not output using iostream.

  //std::cout << x << std::endl; // Fails.
  return 0;
} // int main()



/*

NO output!

*/