import tfhe_py

def led():
    base = tfhe_py.base.Overlay("base.bit")
    for led in base.leds:
        led.on()
    for led in base.leds:
        led.off()

def add(a, b):
    return a + b
