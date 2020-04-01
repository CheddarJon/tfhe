import tfhe_py  # Imports the c++ function bound in include/embpy.h

def dump(a):
    with open('/tmp/tfhe.dump', 'a') as f:
        f.write('{}\n'.format(a))

def execute(temp2, bki, bk_params):
    tfhe_py.tGswFFTExternMulToTLwe(temp2, bki, bk_params)

def loopIntPolyIFFT(deca, decaFFT, kpl):
    for p in range(kpl):
        tfhe_py.IntPolynomial_ifft(decaFFT[p], deca[p])

def loopAddMulRTo(tmpa, decaFFT, gsw, tlwe_params, accum, kpl):
    for p in range(kpl):
        tfhe_py.tLweFFTAddMulRTo(tmpa, decaFFT[p], gsw.all_samples[p], tlwe_params)
    tfhe_py.tLweFromFFTConvert(accum, tmpa, tlwe_params)
