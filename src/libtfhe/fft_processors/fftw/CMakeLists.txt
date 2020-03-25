cmake_minimum_required(VERSION 3.0)

# This is the nayuki fft processor for the tfhe library
#enable_language(ASM)

find_package(Python3 COMPONENTS Development)
set(SRCS
    fft_processor_fftw.cpp
    lagrangehalfc_impl.cpp
    )

set(HEADERS
    lagrangehalfc_impl.h
    Python.h
    )

add_library(tfhe-fft-fftw OBJECT ${SRCS} ${HEADERS})
target_include_directories(tfhe-fft-fftw PRIVATE Python3_INCLUDE_DIRS)
set_property(TARGET tfhe-fft-fftw PROPERTY POSITION_INDEPENDENT_CODE ON)
