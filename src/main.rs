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

use std::env;
use std::fs::File;
use std::io::Read;
use std::cmp::Ordering;
use std::time::SystemTime;

const BLOCKSIZE: usize = 64;
const XOR_SIZE: usize = 128;

const XOR_SUCCESS: i32 = 0;
const XOR_INVALID_PARAM: i32 = 1;
const XOR_FILE_PROC_ERROR: i32 = 2;
const XOR_INCORRECT_FILE: i32 = 3;

///////////////////////////////////////////////////////////////////////////////////////////////
///
/// <summary>Print the Usage, a error message and return invalid data error.</summary>
///
/// <param message>Error message to be printed along with the Usage message.</param>
///
/// <returns>Always XOR_INVALID_PARAM</returns>
///
///////////////////////////////////////////////////////////////////////////////////////////////
fn usage(message: String) -> i32
{
    println!("{}", message);
    println!("USAGE: DoXOR \"file_name\" [\"check_data\"].");
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
fn to_hex_string(bytes: &Vec<u8>) -> String
{
    let strs: Vec<String> = bytes.iter()
                                 .map(|b| format!("{:02X}", b))
                                 .collect();
    let mut string = String::new();
    for s in strs.iter()
    {
        string.push_str(s);
    }
    return string;
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
fn do_xor(block1: &Vec<u8>, block2: &Vec<u8>) -> Vec<u8>
{
    let mut result: Vec<u8> = Vec::new();
    
    //find min size
    let block1_size = block1.len();
    let block2_size = block2.len();
    let size = if block1_size < block2_size {
        block1_size
    } else {
        block2_size
    };

    //do XOR for all bytes in the blocks
    let mut cnt = 0;
    while cnt < size
    {
        result.push(block1[cnt] ^ block2[cnt]);
        cnt += 1;
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
fn load_file_blocks(filename: &String) -> (i32, Vec<Vec<u8>>)
{
    //open the files to read
    let file = match File::open(&filename)
    {
        Err(why) => {
            println!("Unable to open {}, {}", filename, why.to_string());
            return (XOR_FILE_PROC_ERROR, Vec::new());
        },
        Ok(file) => file,
    };

    let mut blocks: Vec<Vec<u8>> = Vec::new();
    let mut temp_vec: Vec<u8> = Vec::new();

    //read each byte and push into a temporary vector
    //once its size is BLOCKSIZE, push it into the 
    //blocks vector of vectors and clear to next iteration
    let file_bytes = file.bytes();
    for byte in file_bytes
    {
        temp_vec.push(byte.unwrap());

        if temp_vec.len() >= BLOCKSIZE
        {
            let v_cloned: Vec<u8> = temp_vec.iter().cloned().collect();
            blocks.push(v_cloned);
            temp_vec.clear();
        }
    }

    if temp_vec.len() < BLOCKSIZE
    {
        let mut cnt = BLOCKSIZE - temp_vec.len();
        while cnt > 0
        {
            temp_vec.push('#' as u8);
            cnt -= 1;
        }
        blocks.push(temp_vec);
    }

    return (XOR_SUCCESS, blocks);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///
/// <summary>Process all file blocks and generate the result block.</summary>
///
/// <param blocks>List of blocks that will be used to generate the result block.</param>
///
/// <returns>Result block after all blocks from file were processed.</returns>
///
///////////////////////////////////////////////////////////////////////////////////////////////
fn process_file_blocks(blocks: Vec<Vec<u8>>) -> Vec<u8>
{
    let mut result_block: Vec<u8> = Vec::with_capacity(BLOCKSIZE);
    for _ in 0..BLOCKSIZE
    {
        result_block.push('#' as u8);
    }
    
    for block in blocks
    {
        result_block = do_xor(&result_block, &block);
    }
    
    return result_block;
}

///////////////////////////////////////////////////////////////////////////////////////////////
///
/// <summary>Generate the file content XOR chacking data.</summary>
///
/// <param fileName>File to be checked.</param>
///
/// <returns>
///     A pair which contains:
///     * first: string with the XOR checking data if processing ok.
///     * second: the processing result which could be:
///       - XOR_SUCCESS - Currency report file has been successfully checked against data.
///       - XOR_FILE_PROC_ERROR - Unable to process the currency report file.
/// </returns>
///
///////////////////////////////////////////////////////////////////////////////////////////////
fn gen_xor_checking_data(filename: &String) -> (String, i32)
{
    let (res, blocks) = load_file_blocks(&filename);
    if res != XOR_SUCCESS
    {
        return ("Failed to load file blocks".to_string(), res);
    }

    let result = process_file_blocks(blocks);
    if result.len() != BLOCKSIZE
    {
        return (format!("Incorrect result block size {} has been crated for file {}", result.len(), filename), XOR_FILE_PROC_ERROR);
    }

    return (to_hex_string(&result), XOR_SUCCESS);
}

///////////////////////////////////////////////////////////////////////////////////////////////
///
/// <summary>Check file.</summary>
///
/// <param fileName>File to be checked.</param>
/// <param checkingData>XOR checking data generated previously.</param>
///
/// <returns>
///     XOR_SUCCESS - Currency report file has been successfully checked against data.
///     XOR_INVALID_PARAM - Application parameters are invalid.
///     XOR_FILE_PROC_ERROR - Unable to process the currency report file.
///     XOR_INCORRECT_FILE - Currency report file is not corect based on the data.
/// </returns>
///
///////////////////////////////////////////////////////////////////////////////////////////////
fn check_file_content(filename: String, checking_data: String) -> i32
{
    let (data_msg, res) = gen_xor_checking_data(&filename);
    let mut result = res;
    if result == XOR_SUCCESS
    {
        if data_msg.len() == checking_data.len()
        {
            //check each result data
            match data_msg.cmp(&checking_data) {
                    Ordering::Less    => {
                        println!("Data file differs from checking data.");
                        result = XOR_INCORRECT_FILE;
                    },
                    Ordering::Greater => {
                        println!("Data file differs from checking data.");
                        result = XOR_INCORRECT_FILE;
                    },
                    Ordering::Equal   => {
                        println!("Data file is equal to the checking data.");
                        result = XOR_SUCCESS;
                }
            }
        }
        else
        {
            println!("Data size {} differs from result size {}", data_msg.len(), checking_data.len());
            result = XOR_INCORRECT_FILE;
        }
    }
    else
    {
        println!("Result {}, {}", result, data_msg);
    }
    return result;
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
fn main()
{
    println!("DoXOR");
    println!("-----");

    let args: Vec<String> = env::args().collect();
    if args.len() != 2 && args.len() != 3
    {
        let message: String = format!("Invalid number of arguments - {}", args.len());
        ::std::process::exit(usage(message));
    }

    let filename: String = (&args[1]).to_string();
    println!("File to XOR = {}", filename);
    if filename.len() == 0
    {
        ::std::process::exit(usage("Invalid file name".to_string()));
    }
    
    if args.len() == 3
    {
        let checking_data = &args[2];
        if checking_data.len() != XOR_SIZE
        {
            let message: String = format!("Invalid XOR checking data size {}. It must be {} bytes.", checking_data.len(), XOR_SIZE);
            ::std::process::exit(usage(message));
        }
        
        let now = SystemTime::now();
        let ret_val = check_file_content(filename, checking_data.to_string());
        match now.elapsed()
        {
            Ok(elapsed) => {
                // it prints '2'
                println!("Processing completed: {}s", elapsed.as_secs());
            }
            Err(e) => {
                // an error occurred!
                println!("Error: {:?}", e);
            }
        }        
        ::std::process::exit(ret_val);
    }
    
    let now = SystemTime::now();
    let (data_msg, res) = gen_xor_checking_data(&filename);
    if res == XOR_SUCCESS
    {
         println!("Result: {}", data_msg);
    }
    else {
        println!("Result {}, {}", res, data_msg);
    }
    match now.elapsed()
    {
        Ok(elapsed) => {
            // it prints '2'
            println!("Processing completed: {}s", elapsed.as_secs());
        }
        Err(e) => {
            // an error occurred!
            println!("Error: {:?}", e);
        }
    }

    // let block1: Vec<u8> = vec![0x01u8, 0x02u8, 0x03u8, 0x04u8, 0x05u8];
    // let block2: Vec<u8> = vec![0xF0u8, 0xE0u8, 0xD0u8, 0xC0u8, 0xB0u8, 0xA0u8];
    // let xor_result = do_xor(&block1, &block2);
    // println!("xor result = {}", to_hex_string(&xor_result));
    ::std::process::exit(res);
}
