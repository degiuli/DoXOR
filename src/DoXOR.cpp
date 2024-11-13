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

#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <iomanip>
#include <cstdint>
#include <chrono>

#define BLOCKSIZE                   (64)
#define XOR_SIZE                    (128)

#define XOR_SUCCESS                 (0)
#define XOR_INVALID_PARAM           (1)
#define XOR_FILE_PROC_ERROR         (2)
#define XOR_INCORRECT_FILE          (3)

template<class Lambda>
class OnExitScope
{
private:
    Lambda& m_lambda;
    
public:
    OnExitScope(Lambda& lambda) : m_lambda(lambda) {}
    ~OnExitScope() { m_lambda(); }
};

///////////////////////////////////////////////////////////////////////////////////////////////
///
/// <summary>Print the Usage, a error message and return invalid data error.</summary>
///
/// <param message>Error message to be printed along with the Usage message.</param>
///
/// <returns>Always XOR_INVALID_PARAM</returns>
///
///////////////////////////////////////////////////////////////////////////////////////////////
int Usage(std::string const message)
{
    std::cout << message << '\n';
    std::cout << "USAGE: DoXOR \"file_name\" [\"check_data\"].\n";
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
std::string to_hex_string(std::vector<unsigned char>& input)
{
    static const char* const lut = "0123456789ABCDEF";
    std::size_t len = input.size();

    std::string output;
    output.reserve(2 * len);
    for (std::size_t i = 0; i < len; ++i)
    {
        const unsigned char c = input[i];
        output.push_back(lut[c >> 4]);
        output.push_back(lut[c & 15]);
    }
    return output;
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
std::vector<unsigned char> DoXOR(std::vector<unsigned char> block1, std::vector<unsigned char> block2)
{
    std::vector<unsigned char> result;
    
    //find min size
    auto block1_size = block1.size();
    auto block2_size = block2.size();
    auto size = (block1_size < block2_size ? block1_size : block2_size);

    //do XOR for all bytes in the blocks
    for (decltype(size) cnt = 0; cnt < size; ++cnt)
    {
        result.push_back(block1[cnt] ^ block2[cnt]);
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
unsigned short LoadFileBlocks(std::string fileName, std::vector<std::vector<unsigned char>>& blocks)
{
    unsigned short res = XOR_FILE_PROC_ERROR;
    
    //open the files to read
    std::ifstream is(fileName.c_str(), std::ifstream::binary);
    try
    {
        if (is)
        {
            auto func = [&is]() {
                is.close();
            };
            OnExitScope<decltype(func)> on_exit_scope(func);
            
            // get length of file:
            is.seekg(0, is.end);
            auto length = is.tellg();
            
            is.seekg(0, is.beg);
            
            if (length > BLOCKSIZE)
            {
                while (length >= 0)
                {
                    std::vector<unsigned char> block(BLOCKSIZE, '#');
                    unsigned char* begin = &*block.begin();
                    decltype(length) toRead = (length > BLOCKSIZE ? static_cast<decltype(length)>(BLOCKSIZE) : length);

                    is.read((char*)begin, toRead);

                    blocks.push_back(block);
                    length -= BLOCKSIZE;
                }
                res = XOR_SUCCESS;
            }
            else
            {
                std::cout << "Unable to read " << fileName << " (length " << length << ").\n";
                res = XOR_FILE_PROC_ERROR;
            }
        }
        else
        {
            std::cout << "Unable to open and read " << fileName << ".\n";
        }
    }
    catch(std::ios_base::failure const& iof)
    {
        std::cout << "Caught an IO failure: " << iof.what() << ", code: " << iof.code() << ".\n";
        res = XOR_FILE_PROC_ERROR;
    }
    catch(std::system_error const& se)
    {
        std::cout << "Caught an system error: " << se.what() << ", code: " << se.code() << ".\n";
        res = XOR_FILE_PROC_ERROR;
    }
    catch(std::runtime_error const& re)
    {
        std::cout << "Caught an system error: " << re.what() << ".\n";
        res = XOR_FILE_PROC_ERROR;
    }
    catch(std::exception const& ex)
    {
        std::cout << "Caught an exception: " << ex.what() << ".\n";
        res = XOR_FILE_PROC_ERROR;
    }
    catch(...)
    {
        std::cout << "Caught an unhandled exception.\n";
        res = XOR_FILE_PROC_ERROR;
    }
    
    return res;
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
std::vector<unsigned char> ProcessFileBlocks(std::vector<std::vector<unsigned char>>& blocks)
{
    std::vector<unsigned char> resultBlock(BLOCKSIZE, '#');
    
    for(auto& block : blocks)
    {
        resultBlock = DoXOR(resultBlock, block);
    }
    
    return resultBlock;
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
std::pair<std::string, unsigned short> GenerateXORCheckingData(std::string fileName)
{
    std::vector<std::vector<unsigned char>> blocks;
    auto res = LoadFileBlocks(fileName, blocks);
    if (res != XOR_SUCCESS)
    {
        std::cout << "Failed to load file blocks.\n";
        return std::make_pair<std::string, unsigned short>({}, std::move(res));
    }

    auto result = ProcessFileBlocks(blocks);
    if (result.size() != BLOCKSIZE)
    {
        std::cout << "Incorrect result block size "
                  << result.size()
                  << " has been created for file "
                  << fileName << ".\n";
        return std::make_pair<std::string, unsigned short>({}, XOR_FILE_PROC_ERROR);
    }

    return std::make_pair<std::string, unsigned short>(to_hex_string(result), XOR_SUCCESS);
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
unsigned short CheckFileContent(std::string fileName, std::string checkingData)
{
    auto result = GenerateXORCheckingData(fileName);
    auto res = result.second;
    if (res == XOR_SUCCESS)
    {
        std::size_t cnt = 0;
        for (auto it = result.first.cbegin(); it != result.first.cend() && checkingData[cnt]; ++it, ++cnt)
        {
            if(*it != checkingData[cnt])
            {
                std::cout << "Data differs from result block in at least one byte.\n";
                res = XOR_INCORRECT_FILE;
                break;
            }
        }

        if (res == XOR_SUCCESS && cnt != result.first.size())
        {
            std::cout << "Data size " << cnt
                      << " different from result size "
                      << result.first.size() << ".\n";
            res = XOR_INCORRECT_FILE;
        }
    }
    
    if (res == XOR_SUCCESS)
        std::cout << "Data file is equal to the checking data.\n";
        
    std::cout << "Result: " << res << '\n';
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
int main(int argc, char* argv[])
{
    std::cout << "DoXOR\n-----\n";

    if (argc != 2 && argc != 3)
    {
        std::stringstream ss;
        ss << "Invalid number of arguments - " << argc;
        return Usage(ss.str());
    }

    std::string fileName{ argv[1] };
    std::cout << "File to XOR = " << fileName << '\n';
    if (fileName.size() == 0)
    {
        return Usage("Invalid file name.");
    }
    
    if (argc == 3)
    {
        std::string checkingData = argv[2];
        if (checkingData.size() != XOR_SIZE)
        {
            std::stringstream ss;
            ss << "Invalid XOR checking data size "
               << checkingData.size()
               << ". It must be 128 bytes.";
            return Usage(ss.str());
        }
        
        auto start = std::chrono::system_clock::now();
        int ret_val = static_cast<int>(CheckFileContent(fileName, checkingData));

        auto end = std::chrono::system_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
        std::cout << "Processing completed: " << (float)(elapsed/1000) << "s\n\n";
        return ret_val;
    }
    
    auto start = std::chrono::system_clock::now();
    auto result = GenerateXORCheckingData(fileName);
    auto end = std::chrono::system_clock::now();

    if (result.second == XOR_SUCCESS)
    {
        std::cout << "Result: " << result.first << '\n';
    }
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end-start).count();
    std::cout << "Processing completed: " << (float)(elapsed/1000) << "s\n\n";
    return static_cast<int>(result.second);
}
