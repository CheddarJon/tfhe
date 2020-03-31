import tfhe_py  # Imports the c++ function bound in include/embpy.h

def dump(a):
    with open('/tmp/tfhe.dump', 'a') as f:
        f.write('{}\n'.format(a))

def execute(temp2, bki, bk_params):
    tfhe_py.tGswFFTExternMulToTLwe(temp2, bki, bk_params)
