#ifndef UTIL
#define UTIL

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdint>
#include <vector>

namespace midi
{
    /**
     * Opens a file stream for reading.
    */
    class Reader {
        private:
            static const uint64_t MAX_BYTES = 4;
            static const uint64_t FLAG_OFFSET = 7;
            static const uint64_t BITS_LENGTH = 7;

            static constexpr uint8_t MASK_VARIABLE_LENGTH_VALUE {0b0111'1111};
            static constexpr uint8_t MASK_VARIABLE_LENGTH_FLAG {0b1000'0000};

        public:
            uint64_t bytes_read;
            bool file_good;
            std::vector<uint8_t> data;
            char** raw_data;

            Reader();
            ~Reader();

            /**
             * Open a file named {file_name} in the current directory for reading.
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
            uint64_t read_fixed_length(uint64_t len);
            
    };

    class Writer
    {
    };
}
#endif