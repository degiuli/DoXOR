/*--
The MIT License (MIT)

Copyright (c) 2012-2024 Fabio Lourencao De Giuli (http://degiuli.github.io)
Copyright (c) 2012-2024 De Giuli Informatica Ltda. (http://www.degiuli.com.br)

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
--*/

import java.io.*;

public class DoXOR
{
    private static final int BLOCKSIZE = (64);
    private static final int XOR_SIZE = (128);
    
    private static final int XOR_SUCCESS = (0);
    private static final int XOR_INVALID_PARAM = (1);
    private static final int XOR_FILE_PROC_ERROR = (2);
    private static final int XOR_INCORRECT_FILE = (3);

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// <summary>Print the Usage, a error message and return invalid data error.</summary>
    ///
    /// <param message>Error message to be printed along with the Usage message.</param>
    ///
    /// <returns>Always XOR_INVALID_PARAM</returns>
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////
    private static int Usage(String message)
    {
        System.out.printf("%s\n", message);
        System.out.println("USAGE: DoXOR \"file_name\" [\"check_data\"].");
        return XOR_INVALID_PARAM;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// <summary>Convert a vector of unsigned char into hex string.</summary>
    ///
    /// <param input>The vector to be converted.</param>
    ///
    /// <returns>The hex string.</returns>
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////
    private static String to_hex_string(byte[] bytes)
    {
        final char[] hexArray = "0123456789ABCDEF".toCharArray();
        char[] hexChars = new char[bytes.length * 2];
        for (int j = 0; j < bytes.length; j++)
        {
            int v = bytes[j] & 0xFF;
            hexChars[j * 2] = hexArray[v >>> 4];
            hexChars[j * 2 + 1] = hexArray[v & 0x0F];
        }
        return new String(hexChars);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// <summary>Gets two blocks of data and do a XOR.</summary>
    ///
    /// <param block1>First block to be processed.</param>
    /// <param block2>Second block to be processed.</param>
    ///
    /// <returns>Result block with the xor on both blocks.</returns>
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////
    private static byte[] do_xor(byte[] block1, byte[] block2)
    {
        //find min size
        int block1_size = block1.length;
        int block2_size = block2.length;
        int size = (block1_size < block2_size ? block1_size : block2_size);
        byte[] result = new byte[size];

        //do XOR for all bytes in the blocks
        for (int cnt = 0; cnt < size; ++cnt)
        {
            result[cnt] = (byte)(block1[cnt] ^ block2[cnt]);
        }

        return result;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// <summary>Open the file and loads a list of blocks.</summary>
    ///
    /// <param fileName>File to be read.</param>
    /// <param blocks>Output vector that will contain all blocks read from file.</param>
    ///
    /// <returns>
    ///     XOR_SUCCESS - Checking data has been successfully created and printed.
    ///     XOR_FILE_PROC_ERROR - Unable to process the currency report file.
    /// </returns>
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////
    int LoadFileBlocks(String fileName, byte[][] blocks)
    {
        int res = XOR_FILE_PROC_ERROR;
        return res;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////
    ///
    /// <summary>Entry Point</summary>
    ///
    /// <param argc>Number of application arguments - includes app name.</param>
    /// <param argv>List of arguments.</param>
    ///
    /// <returns>
    ///     XOR_SUCCESS - Checking data has been successfully created and printed.
    ///     XOR_INVALID_PARAM - Application parameters are invalid.
    ///     XOR_FILE_PROC_ERROR - Unable to process the currency report file.
    ///     XOR_INCORRECT_FILE - Currency report file is not corect based on the data.
    /// </returns>
    ///
    ///////////////////////////////////////////////////////////////////////////////////////////////
    public static void main(String[] args)
    {
        System.out.println("DoXOR");
        System.out.println("-----");

        if (args.length != 1 && args.length != 2)
        {
            String message = String.format("Invalid number of arguments - %d", args.length);
            System.exit(Usage(message));
        }

        String fileName = new String(args[0]);
        System.out.printf("File to XOR = %s\n", fileName);
        if (fileName.length() == 0)
        {
            System.exit(Usage("Invalid file name."));
        }
        
        if (args.length == 2)
        {
            String checking_data = new String(args[1]);
            if (checking_data.length() != XOR_SIZE)
            {
                String message = String.format("Invalid XOR checking data size %d. It must be %d bytes.", checking_data.length(), XOR_SIZE);
                System.exit(Usage(message));
            }
            
            long start = System.currentTimeMillis();
            int ret_val = Usage("Not yet implemented");//int ret_val = check_file_content(filename, checking_data.to_string());
            long end = System.currentTimeMillis();
            System.out.printf("Processing completed: %fs\n", (float)(end - start)/1000);
            System.exit(ret_val);
        }
        
        long start = System.currentTimeMillis();
        int ret_val = Usage("Not yet implemented");//int result = GenerateXORCheckingData(fileName);
        long end = System.currentTimeMillis();
    
        // if (result.second == XOR_SUCCESS)
        // {
        //     std::cout << "Result: " << result.first << '\n';
        // }
        final byte[] block1 = {(byte)0x01, (byte)0x02, (byte)0x03, (byte)0x04, (byte)0x05};
        final byte[] block2 = {(byte)0xF0, (byte)0xE0, (byte)0xD0, (byte)0xC0, (byte)0xB0, (byte)0xA0};
        byte[] xor_result = do_xor(block1, block2);
        System.out.printf("xor result = %s\n", to_hex_string(xor_result));
        System.out.printf("Processing completed: %fs\n", (float)(end - start)/1000);
        System.exit(ret_val);
    }
}
