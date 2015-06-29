//  Copyright Nikhar Agrawal 2015.
//  Copyright Paul Bristow 2015.
//  Copyright Christopher Kormanyos 2014 - 2015.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)

// Test compile fail (static_assert) for resolution must be < 0

#include "boost/fixed_point/fixed_point.hpp"

int main()
{
  using namespace boost::fixed_point;

  boost::fixed_point::negatable<7, 0, boost::fixed_point::round::nearest_even> x0;
  negatable<7, 1, round::nearest_even> x1;
} // int main()

/*
test_fail_resolution_nonneg.cpp

I:\modular-boost\boost/fixed_point/fixed_point.hpp(187): error C2338: Error: The resolution of negatable must be fractional (negative).
I:\modular-boost\libs\fixed_point\test\compile_fail\test_fail_resolution_nonneg.cpp(15)
: see reference to class template instantiation 'boost::fixed_point::negatable<7,0,boost::fixed_point::round::nearest_even,boost::fixed_point::overflow::undefined>' being compiled

*/