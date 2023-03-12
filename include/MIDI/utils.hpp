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

    uint64_t total_length;
    uint64_t bytes_read;
    std::vector<uint8_t> data;
    

// Methods //--------------------------------------------------------------------------------------
public:
    Reader();
    ~Reader();

    /**
     * Open a file named {file_name} in the current directory for reading.
     * @param file_name String name of the file.
    */
    error_status_t read_file(std::string file_name);

    /**
     * @return True if reader has data.
    */
    bool is_good();

    /**
     * Put the next 'num_bytes' bytes into the buffer 's'.
     * @param num_bytes The number of bytes to read.
     * @return A vector of characters read.
    */
    std::vector<char> get_next(uint64_t num_bytes);

    /**
    * Reads fron the file, a value of variable length.
    * @return the value read.
    */
    uint32_t read_variable_length();

    /**
     * Reads from the file, a value with a fixed length.
     * @note Assumes that most significant bits are placed first.
     * @param len The length of the value.
     * @return The value read.
    */
    uint64_t read_fixed_length(uint64_t len);
        
};

class Writer
{
};

}


