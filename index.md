# TFHE: Fast Fully Homomorphic Encryption over the Torus

**version 1.0-rc1** -- *first release candidate date: 2017.04.05*

TFHE is an open-source library for fully homomorphic encryption, 
distributed under the terms of the Apache 2.0 license. 

The underlying scheme is described in best paper of the IACR
conference Asiacrypt 2016: "Faster fully homomorphic encryption: 
Bootstrapping in less than 0.1 seconds", presented by 
Ilaria Chillotti, Nicolas Gama, Mariya Georgieva and Malika Izabachène.


### Description 

TFHE is a C/C++ library which implements a very fast gate-by-gate 
bootstrapping, based on [CGGI16]. The library allows to evaluate an  
arbitrary boolean circuit composed of binary gates, over encrypted 
data, without revealing any information on the data.

The library supports the homomorphic evaluation of the 10 binary gates (And, Or, Xor, 
Nand, Nor, etc...), as well as the negation and the Mux gate. Each binary gate takes 
about 20 milliseconds single-core time to evaluate, which improves [DM15] by a factor 30, 
and the mux gate takes about 40 CPU-ms.

Unlike other libraries, the gate-bootstrapping mode of TFHE has no restriction on the 
number of gates or on their composition. This allows to perform any
computation over encrypted data, even if the actual function that will be applied is 
not yet known when the data is encrypted. The library is easy to use with either
manually crafted circuits, or with the output of automated circuit
generation tools. 

From the user point of view, the library can:
* generate a secret-keyset and a cloud-keyset. The secret keyset is
  private, and provides encryption/decryption abilities. The cloud-keyset 
  can be exported to the cloud, and allows to operate over encrypted
  data. 
* With the secret keyset, the library allows to encrypt and decrypt data.
  The encrypted data can safely be outsourced to the cloud, in order to perform
  secure homomorphic computations. 
* With the cloud-keyset, the library can evaluate a net-list of
  binary gates homomorphically at a rate of about 50 gates per second per
  core, without decrypting its input. It suffices to provide the sequence
  of gates, as well as ciphertexts of the input bits. And the
  library computes ciphertexts of the output bits.

### Under the hood

The library implements a Ring-variant of the GSW [GSW13]
cryptosystem and makes many optimizations described in [DM15] and
[CGGI16]. 

It also implements a dedicated Fast Fourier
Transformation for the anticyclic ring R[X]/(X^N+1), and uses AVX
assembly vectorization instructions. 
The default parameter set achieves a 120-bit cryptographic security,
based on ideal lattice assumptions.


Since the running time per gate seems to be the bottleneck of fully
homomorphic encryption, an optimal circuits for TFHE is most likely a circuit 
with the smallest possible number of gates, and to a lesser extent, the 
possibility to evaluate them in parallel. 


### Dependencies 


The library interface can be used in a regular C code. However, to
compile the core of the library you will need a standard C++11 compiler.
Currently, the project has only been tested with the g++ compiler under
Linux. In the future, we plan to extend the compatibility to other
compilers, platforms and operating systems.

At least one FFT processor is needed to run the project:

* The default processor comes from Project Nayuki, who proposes two
  implementations of the fast Fourier transform - one in portable C, and
  the other using the AVX assembly instructions.
  This component is licensed under the MIT license, and we added the code
  of the reverse FFT (both in C and in assembly). Original source:
  https://www.nayuki.io/page/fast-fourier-transform-in-x86-assembly
* we provide another processor, named the spqlios processor, which is
  written in AVX and FMA assembly in the style of the nayuki processor,
  and which is dedicated to the ring R[X]/(X^N+1) for N a power of 2.
<!--
* We also provide a connector for the FFTW3 library: http://www.fftw.org.
  With this library, the performance of the FFT is between 2 and 3 times
  faster than the default Nayuki implementation. However, you should keep
  in mind that the library FFTW is published under the GPL License. If you
  choose to use this library in a final product, this product may have to
  be released under GPL License as well (other commercial licenses are
  available on their web site)
-->
* We plan to add other connectors in the future (for instance the Intel’s
  IPP Fourier Transform, which should be 1.5× faster than FFTW for 1D
  real data)


### Installation

To build the library with the default options, run ```make``` from the
top level directory of the TFHE project.
This assumes that the standard tool cmake is already installed on the
system, and an
up-to-date c++ compiler (i.e. g++ >=5.2) as well.
It will compile the library in optimized mode in the build/libtfhe
folder.
Currently, only static libraries are generated. There is no make install
yet, as the developpement of the library is still 
in an early state. 

If you want to choose additional compile options (i.e. debug mode, tests,
fftw), you need to run cmake manually and pass the desired options:
```
mkdir build
cd build
cmake ../src -DENABLE_TESTS=on -DENABLE_FFTW=on -DCMAKE_BUILD_TYPE=debug
make
```
The available options are the following:

| Variable Name          | values           | 
|------------------------|-------|
| CMAKE_BUILD_TYPE       | <ul><li>*optim* enables compiler's optimization flags, including native architecture specific optimizations</li><li>*debug* disables any optimization and include all debugging info (-g3 -O0)</li> | 
| ENABLE_TESTS           | *on/off* compiles the library's unit tests and sample applications in the test/ folder. This assumes that googletest>1.8 is installed on the system. (use ```ctest``` to run all tests) | 
| ENABLE_FFTW            | *on/off* compiles libtfhe-fftw.a, using FFTW3 (GPL licence) for fast FFT computations |
| ENABLE_NAYUKI_PORTABLE | *on/off* compiles libtfhe-nayuki-portable.a, using the fast C version of nayuki for FFT computations |
| ENABLE_NAYUKI_AVX      | *on/off* compiles libtfhe-nayuki-avx.a, using the avx assembly version of nayuki for FFT computations |
| ENABLE_SPQLIOS_AVX     | *on/off* compiles libtfhe-spqlios-avx.a, using tfhe's dedicated avx assembly version for FFT computations |
| ENABLE_SPQLIOS_FMA     | *on/off* compiles libtfhe-spqlios-fma.a, using tfhe's dedicated fma assembly version for FFT computations |

### References

[DM15](): L. Ducas and D. Micciancio.  FHEW: Bootstrapping homomorphic
encryption in less than a second.  In Eurocrypt 2015, pages 617-640.

[GSW13]():  C. Gentry, A. Sahai, and B. Waters. Homomorphic encryption from
learning with errors:  Conceptually-simpler,  asymptotically-faster,
attribute-based. In Crypto 2013, pages 75-92

[CGGI16](): I. Chillotti, N. Gama, M. Georgieva, and M. Izabachène. Faster fully homomorphic encryption: Bootstrapping in less than 0.1 seconds. Asiacrypt, To appear, 2016. Cryptology ePrint Archive, report 2016/870.

[HElib]: An Implementation of homomorphic encryption
https://github.com/shaih/HElib