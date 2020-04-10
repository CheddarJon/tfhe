def led(base):
    for led in base.leds:
        led.on()
    for led in base.leds:
        led.off()

def add(a, b):
    return a + b
