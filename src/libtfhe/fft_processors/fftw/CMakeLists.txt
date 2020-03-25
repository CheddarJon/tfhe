cmake_minimum_required(VERSION 3.0)

# This is the nayuki fft processor for the tfhe library
#enable_language(ASM)

set(PYTHON_HEADERS /usr/include/python3.8)
set(SRCS
    fft_processor_fftw.cpp
    lagrangehalfc_impl.cpp
    )

set(HEADERS
    lagrangehalfc_impl.h
    ${PYTHON_HEADERS}/Python.h
    )

add_library(tfhe-fft-fftw OBJECT ${SRCS} ${HEADERS})
target_include_directories(tfhe-fft-fftw PRIVATE ${PYTHON_HEADERS})
set_property(TARGET tfhe-fft-fftw PROPERTY POSITION_INDEPENDENT_CODE ON)
