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

# function hmac_md5 (key, message)
#   if (length(key) > blocksize) then
#       key = hash(key) // keys longer than blocksize are shortened
#   end if
#   if (length(key) < blocksize) then
#       key = key || [0x00 * (blocksize - length(key))] // keys shorter than blocksize are zero-padded (where || is concatenation)
#   end if
#
#   o_key_pad = [0x5c * blocksize] ^ key // Where blocksize is that of the underlying hash function
#   i_key_pad = [0x36 * blocksize] ^ key // Where ^ is exclusive or (XOR)
#
#   return hash(o_key_pad || hash(i_key_pad || message)) // Where || is concatenation
# end function
#

import os
import sys
import time
import datetime
from hashlib import md5

trans_5C = bytearray((x ^ 0x5c) for x in range(256))
trans_36 = bytearray((x ^ 0x36) for x in range(256))
blocksize = md5().block_size # 64

def hmac_md5(key, msg):
    if len(key) > blocksize:
        key = md5(key).digest()
    key = key + bytearray(blocksize - len(key))
    o_key_pad = key.translate(trans_5C)
    i_key_pad = key.translate(trans_36)
    return md5(o_key_pad + md5(i_key_pad + msg).digest())

if __name__ == "__main__":
    if len(sys.argv) == 2:
        print("File to hash = " + sys.argv[1])
        
        start = time.time()
        try:
            with open(sys.argv[1], 'rb') as file:
                contents = file.read()
                if contents:
                    h = hmac_md5(b"ActivCash_6.0.x", contents)
                    print(h.hexdigest())
                else:
                    print("File has not content to hash")
                    
        except:
            print("Unexpected error: ")
            print(sys.exc_info())
            
        finally:
            pass
            
        done = time.time()
        elapsed = done - start
        print("Processing completed: " + str(elapsed) + "s")
    else:
        print("USAGE: python DoHMAC_MD5.py <path to file>")
