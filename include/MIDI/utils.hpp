#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <vector>

namespace midi
{

using error_status_t = int32_t;

/**
 * Opens a file stream for reading.
*/
class Reader {

// Variables //------------------------------------------------------------------------------------
private:
    static const uint64_t MAX_BYTES = 4;
    static const uint64_t FLAG_OFFSET = 7;
    static const uint64_t BITS_LENGTH = 7;

    static constexpr uint8_t MASK_VARIABLE_LENGTH_VALUE {0b0111'1111};
    static constexpr uint8_t MASK_VARIABLE_LENGTH_FLAG {0b1000'0000};

    uint64_t totalLength;
    uint64_t bytesRead;
    std::vector<uint8_t> data;
    

// Methods //--------------------------------------------------------------------------------------
public:

    /**
     * Open a file named {file_name} in the current directory for reading.
     * @param file_name String name of the file.
     * @return Error status, 0 for success.
    */
    error_status_t openFileStream(std::string fileName);

    /**
     * @return True if reader has data.
    */
    bool isGood();

    /**
     * Put the next 'numBytes' bytes into the buffer 's'.
     * @param numBytes The number of bytes to read.
     * @return A vector of characters read.
    */
    std::vector<char> getNext(uint64_t numBytes);

    /**
    * Reads fron the file, a value of variable length.
    * @return the value read.
    */
    uint32_t readVariableLength();

    /**
     * Reads from the file, a value with a fixed length.
     * @note Assumes that most significant bits are placed first.
     * @param len The length of the value.
     * @return The value read.
    */
    uint64_t readFixedLength(uint64_t len);
        
};

class Writer
{
};

}


