
all : all_files cleanup

CPPFLAGS    = -O2                                              \
              -Wall                                            \
              -Wextra                                          \
              -Wno-unused-parameter                            \
              -Wno-deprecated-declarations                     \
              -Wno-maybe-uninitialized                         \
              -std=c++11                                       \
              -I/d/boost_fixed_point/include                   \
              -I/d/boost/boost_1_59_0

%.exe : %.cpp
	@-echo +++compile $<
	@-g++ $< $(CPPFLAGS) -o $@
	@-echo +++running $@
	@-./$@

all_files : test_negatable_basic_arithmetic_fastest.exe            \
            test_negatable_basic_arithmetic_nearest_even.exe       \
            test_negatable_basic_bare_metal_config_16bit.exe       \
            test_negatable_basic_bare_metal_config_32bit.exe       \
            test_negatable_basic_basic_ops.exe                     \
            test_negatable_basic_bit_pattern.exe                   \
            test_negatable_basic_construct.exe                     \
            test_negatable_basic_limits.exe                        \
            test_negatable_basic_mixed_constructors_part1.exe      \
            test_negatable_basic_mixed_constructors_part2.exe      \
            test_negatable_basic_mixed_math.exe                    \
            test_negatable_basic_multiply.exe                      \
            test_negatable_constants_local_ln_two.exe              \
            test_negatable_constants_local_pi.exe                  \
            test_negatable_func_arccosine_big.exe                  \
            test_negatable_func_arccosine_small.exe                \
            test_negatable_func_arcsine_big.exe                    \
            test_negatable_func_arcsine_small.exe                  \
            test_negatable_func_arctangent_big.exe                 \
            test_negatable_func_arctangent_small.exe               \
            test_negatable_func_cosine_big.exe                     \
            test_negatable_func_cosine_small.exe                   \
            test_negatable_func_cosine_tiny.exe                    \
            test_negatable_func_exp_fastest.exe                    \
            test_negatable_func_exp_nearest_even.exe               \
            test_negatable_func_exp_small.exe                      \
            test_negatable_func_exp_tiny.exe                       \
            test_negatable_func_floor_ceil.exe                     \
            test_negatable_func_frexp_ldexp.exe                    \
            test_negatable_func_hyperbolic_arccosine_big.exe       \
            test_negatable_func_hyperbolic_arccosine_small.exe     \
            test_negatable_func_hyperbolic_arcsine_big.exe         \
            test_negatable_func_hyperbolic_arcsine_small.exe       \
            test_negatable_func_hyperbolic_arctangent_big.exe      \
            test_negatable_func_hyperbolic_arctangent_small.exe    \
            test_negatable_func_hyperbolic_cosine_big.exe          \
            test_negatable_func_hyperbolic_cosine_small.exe        \
            test_negatable_func_hyperbolic_sine_big.exe            \
            test_negatable_func_hyperbolic_sine_small.exe          \
            test_negatable_func_hyperbolic_tangent_big.exe         \
            test_negatable_func_hyperbolic_tangent_small.exe       \
            test_negatable_func_log_fastest.exe                    \
            test_negatable_func_log_nearest_even.exe               \
            test_negatable_func_log_tiny.exe                       \
            test_negatable_func_sine_big.exe                       \
            test_negatable_func_sine_small.exe                     \
            test_negatable_func_sine_tiny.exe                      \
            test_negatable_func_sqrt_big.exe                       \
            test_negatable_func_sqrt_small_fastest.exe             \
            test_negatable_func_sqrt_small_nearest_even.exe        \
            test_negatable_func_sqrt_tiny.exe                      \
            test_negatable_func_tangent_big.exe                    \
            test_negatable_func_tangent_small.exe                  \
            test_negatable_func_tangent_tiny.exe                   \
            test_negatable_math_bernoulli_b2n.exe                  \
            test_negatable_math_cbrt_fastest.exe                   \
            test_negatable_math_cbrt_nearest_even.exe              \
            test_negatable_math_cyl_bessel_j_big.exe               \
            test_negatable_math_cyl_bessel_j_small.exe             \
            test_negatable_math_root_find_toms748.exe              \
            test_negatable_math_trapezoid_integral.exe             \
            test_negatable_round_trip_digits02_005m2.exe           \
            test_negatable_round_trip_digits10_001.exe             \
            test_negatable_round_trip_digits10_002.exe             \
            test_negatable_round_trip_digits10_003.exe             \
            test_negatable_round_trip_digits10_005.exe             \
            test_negatable_round_trip_digits10_006.exe             \
            test_negatable_round_trip_digits10_007.exe             \
            test_negatable_round_trip_digits10_014.exe             \
            test_negatable_round_trip_digits10_018.exe             \
            test_negatable_round_trip_digits10_050.exe             \
            test_negatable_round_trip_digits10_100.exe             \
            test_negatable_round_trip_digits10_999.exe

cleanup :
	@-echo +++cleaning all executable files
	@-rm *.exe
