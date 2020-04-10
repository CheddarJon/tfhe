#!/usr/bin/python3

################################################################
# Run from Jupyter
#
# run /home/xilinx/jupyter_notebooks/transfer/adder_run.py
################################################################


#from pynq import Overlay
import tfhe_py

def add(first, second):

    overlay = tfhe_py.base.Overlay('/home/xilinx/adder/adder.bit')

    add_ip = overlay.scalar_add
    add_ip.write(0x10, first)
    add_ip.write(0x18, second)
    result = add_ip.read(0x20)

    print("{0}+{1}={2}".format(first, second, result)) 
