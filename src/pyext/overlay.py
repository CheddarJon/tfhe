# Imports the c++ function bound in include/embpy.h
import tfhe_py  
import random
from pynq.overlays.base import BaseOverlay

def dump(a):
    with open('/tmp/tfhe.dump', 'a') as f:
        f.write('{}\n'.format(a))

def execute(base, temp2, bki, bk_params):
    tfhe_py.tGswFFTExternMulToTLwe(temp2, bki, bk_params)

def loopIntPolyIFFT(deca, decaFFT, kpl):
    o = BaseOverlay("base.bit")
    r = random.randint(0,len(o.leds) - 1)
    o.leds[r].toggle()

    for p in range(kpl):
        tfhe_py.IntPolynomial_ifft(decaFFT[p], deca[p])

def loopAddMulRTo(tmpa, decaFFT, gsw, tlwe_params, accum, kpl):
    a = random.randint(0, 10)
    b = random.randint(0, 10)
    overlay = tfhe_py.base.Overlay('/home/xilinx/adder/adder.bit')

    add_ip = overlay.scalar_add
    add_ip.write(0x10, a)
    add_ip.write(0x18, b)
    result = add_ip.read(0x20)

    print("{0}+{1}={2}".format(a, b, result)) 


    for p in range(kpl):
        tfhe_py.tLweFFTAddMulRTo(tmpa, decaFFT[p],
                gsw.all_samples[p], tlwe_params)
    tfhe_py.tLweFromFFTConvert(accum, tmpa, tlwe_params)
