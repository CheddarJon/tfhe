import tfhe_py  # Imports the c++ function bound in include/embpy.h

def dump(a, b, c):
    with open('/tmp/tfhe.dump', 'a') as f:
        f.write('{}, {}, {}\n'.format(a, b, c, d, e))

def execute(a, b, c):
    tfhe_py.tGswFFTExternMulToTLwe(a, b, c)
    return 0
