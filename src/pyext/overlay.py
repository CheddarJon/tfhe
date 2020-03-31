import tfhe_py  # Imports the c++ function bound in include/embpy.h

def dump(a, b, c):
    with open('/tmp/tfhe.dump', 'a') as f:
        f.write('{}, {}, {}\n'.format(a, b, c, d, e))

def execute(kpl, decaFFT, deca, tmpa, tlwe_params, gsw, accum):
    for p in range(kpl):
        tfhe_py.IntPolynomial_ifft(decaFFT[p], deca[p])

    tfhe_py.tLweFFTClear(tmpa, tlwe_params)

    for p in range(kpl):
        tfhe_py.tLweFFTAddMulRTo(tmpa, decaFFT[p], gsw.all_samples[p], tlwe_params)
    tfhe_py.tLweFromFFTConvert(accum, tmpa, tlwe_params)
    return 0
