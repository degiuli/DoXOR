# /*--
# The MIT License (MIT)
# 
# Copyright (c) 2012-2024 Fabio Lourencao De Giuli (http://degiuli.github.io)
# Copyright (c) 2012-2024 De Giuli Informatica Ltda. (http://www.degiuli.com.br)
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy of
# this software and associated documentation files (the "Software"), to deal in
# the Software without restriction, including without limitation the rights to
# use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
# the Software, and to permit persons to whom the Software is furnished to do so,
# subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
# FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
# COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
# IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
# CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# --*/

#
#   Result sample: bbe1b31d5b712923795249161f11032a07006166624e7e442e2d6d354b6e4f126f45591f6359117d3945626b19353e6119281600754f30571723267b5729717a
#

import os
import sys
import time
import datetime

BLOCKSIZE = 64

def xor(block1, block2):
    return bytearray((x ^ y) for x,y in zip(bytearray(x for x in block1), bytearray(x for x in block2)))

if __name__ == "__main__":
    print("DoXOR")
    print("-----")

    if len(sys.argv) == 2:
        print("File to XOR = " + sys.argv[1])
        
        start = time.time()
        try:
            block = b"################################################################"
            with open(sys.argv[1], 'rb') as file:
                while True:
                    contents = file.read(BLOCKSIZE)
                    if not contents:
                        break
                    
                    if len(contents) < BLOCKSIZE:
                        contents = contents + str(x for x in b"################################################################")
                        
                    block = xor(block, contents)
            
            print("Result:")
            print(''.join('{:02x}'.format(x) for x in block))
            
        except:
            print("Unexpected error: ")
            print(sys.exc_info())
            
        finally:
            pass
            
        done = time.time()        
        elapsed = done - start        
        print("Processing completed: " + str(elapsed) + "s")
    else:
        print("USAGE: python DoXOR.py <path to file>")
