import tfhe_py  # Imports the c++ function bound in include/embpy.h

from pynq import PL
from pynq.overlays.base import BaseOverlay

def dump(a):
    with open('/tmp/tfhe.dump', 'a') as f:
        f.write('{}\n'.format(a))

def execute(base, temp2, bki, bk_params):
    #base.leds[0].on()
    tfhe_py.tGswFFTExternMulToTLwe(temp2, bki, bk_params)
    #base.leds[0].off()

def loopIntPolyIFFT(base, deca, decaFFT, kpl):
    #print("loopIntPolyIFFT")
    #base.leds[1].on()
    for p in range(kpl):
        tfhe_py.IntPolynomial_ifft(decaFFT[p], deca[p])
    #base.leds[1].off()

def loopAddMulRTo(base, tmpa, decaFFT, gsw, tlwe_params, accum, kpl):
    #base.leds[2].on()
    #print("loopAddMulRTo")
    for p in range(kpl):
        tfhe_py.tLweFFTAddMulRTo(tmpa, decaFFT[p], gsw.all_samples[p], tlwe_params)
    tfhe_py.tLweFromFFTConvert(accum, tmpa, tlwe_params)
    #base.leds[2].off()

def led(a):
    base = BaseOverlay("base.bit")
    for led in base.leds:
        led.on()
    for led in base.leds:
        led.off()
