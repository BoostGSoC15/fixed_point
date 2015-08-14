
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

all_files : test_negatable_arithmetic.exe                 \
            test_negatable_bare_metal_config.exe          \
            test_negatable_bare_metal_config_32bit.exe    \
            test_negatable_basic.exe                      \
            test_negatable_bernoullib2n.exe               \
            test_negatable_bit_pattern.exe                \
            test_negatable_cbrt_fastest.exe               \
            test_negatable_cbrt_nearest_even.exe          \
            test_negatable_construct.exe                  \
            test_negatable_floor_ceil.exe                 \
            test_negatable_frexp_ldexp.exe                \
            test_negatable_local_constants.exe            \
            test_negatable_mixed_constructors.exe         \
            test_negatable_multiply.exe                   \
            test_negatable_round_trip_digits02_005m2.exe  \
            test_negatable_round_trip_digits10_003.exe    \
            test_negatable_round_trip_digits10_005.exe    \
            test_negatable_round_trip_digits10_006.exe    \
            test_negatable_round_trip_digits10_007.exe    \
            test_negatable_round_trip_digits10_014.exe    \
            test_negatable_round_trip_digits10_018.exe    \
            test_negatable_round_trip_digits10_050.exe    \
            test_negatable_round_trip_digits10_100.exe    \
            test_negatable_round_trip_digits10_999.exe    \
            test_negatable_sqrt_big.exe                   \
            test_negatable_sqrt_fastest.exe               \
            test_negatable_sqrt_nearest_even.exe

cleanup :
	@-echo +++cleaning all executable files
	@-rm *.exe
