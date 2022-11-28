#ifndef UTIL
#define UTIL

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <vector>

static constexpr uint8_t BITS_PER_BYTE = 8;

static constexpr uint8_t MASK_0 {0b0000'0001};
static constexpr uint8_t MASK_1 {0b0000'0010};
static constexpr uint8_t MASK_2 {0b0000'0100};
static constexpr uint8_t MASK_3 {0b0000'1000};
static constexpr uint8_t MASK_4 {0b0001'0000};
static constexpr uint8_t MASK_5 {0b0010'0000};
static constexpr uint8_t MASK_6 {0b0100'0000};
static constexpr uint8_t MASK_7 {0b1000'0000};

static constexpr uint8_t MASK_VARIABLE_LENGTH_VALUE {0b0111'1111};
static constexpr uint8_t MASK_VARIABLE_LENGTH_FLAG {0b1000'0000};

typedef unsigned int uint;

/**
 * Opens a file stream for reading.
*/
class Reader {
    private:
        static const size_t MAX_BYTES = 4;
        static const size_t FLAG_OFFSET = 7;
        static const size_t BITS_LENGTH = 7;

    public:
        uint64_t bytes_read;
        bool file_good;
        std::vector<char> data;

        Reader();
        ~Reader();

        /**
         * Open a file named 'file_name' in the current directory for reading.
        */
        int read_file(std::string file_name);

        /**
         * Put the next 'num_bytes' bytes into the buffer 's'.
        */
        std::vector<char> get_next(uint64_t num_bytes);

        /**
        * Reads fron the file, a value of variable length
        */
        uint32_t read_variable_length();

        /**
         * Reads from the file, a value with a fixed length 
        */
        uint64_t read_fixed_length(size_t len);
};

class Writer
{
};

#endif