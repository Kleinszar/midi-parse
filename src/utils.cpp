#include <MIDI/utils.h>

// Constants //---------------------------------------------------------------------------------------


// Constructors //------------------------------------------------------------------------------------
Reader::Reader()
{
    this->bytes_read = 0;
    this->file_good = false;
};

Reader::~Reader()
{
    this->data.clear();
};

// Static //------------------------------------------------------------------------------------------

// Member //------------------------------------------------------------------------------------------


int Reader::read_file(std::string file_name)
{
    std::ifstream input_stream(file_name, std::ios::binary | std::ios::ate); // Seek to end

    if (!input_stream.is_open())
    {
        perror(("Cannot open file " + file_name).c_str());
        return -1;
    }
    
    std::ifstream::pos_type position = input_stream.tellg(); // Get current position (Size of file)

    if(input_stream.bad())
    {
        perror(("Error while reading file " + file_name).c_str());
        return -1;
    }

    if (position == 0)
    {
        return -1; // Empty file
        input_stream.close();
    }
    std::vector<char> raw_data(position);
    input_stream.seekg(0, std::ios::beg);
    input_stream.read(&raw_data[0], position);
    input_stream.close();
    this->data = raw_data;
    this->file_good = true;
    /*
    for (std::vector<char>::iterator it = this->data.begin(); it != this->data.end(); it++)
    {
        std::cout << *it << std::hex << " ";
    }
    */

    return 0;
}

std::vector<char> Reader::get_next(uint64_t num_bytes)
{
    auto position = this->data.begin() + this->bytes_read;
    std::vector<char> next_block = std::vector<char>(position, position + num_bytes);
    this->bytes_read += num_bytes;
    return next_block;
};



/*
 * A variable length quantity is represented (lower) 7 bits per byte, most significant bits first.
 * Up to a max of 4 bytes (32 bits). Bit 7 is a flag, that is set if the value has not ended.
 * All 1 byte chunks have bit 7 set except for the last chunk, indicating the end.
*/
uint32_t Reader::read_variable_length()
{
    uint64_t curr_position = this->bytes_read;
    uint64_t length = 0;
    uint32_t value = 0;

    for (size_t i = curr_position; i < curr_position + this->MAX_BYTES; i++)
    {
        uint8_t curr_byte = this->data.at(i);
        value = (value << BITS_LENGTH) + (curr_byte & MASK_VARIABLE_LENGTH_VALUE);
        ++length;
        // Check flag for end of variable value, set = not ended.
        int flag = (curr_byte & MASK_VARIABLE_LENGTH_FLAG) >> FLAG_OFFSET;

        if (flag == 0)
        {
            break;
        }
    }
    this->bytes_read += length;
    return value;
};

/**
 *  Assumes that most significant bits are placed first. Reads an unsigned value.
*/
uint64_t Reader::read_fixed_length(size_t len)
{
    uint64_t curr_posititon = this->bytes_read;
    uint64_t value = 0;
    for (size_t i = curr_posititon; i < curr_posititon + len; i++)
    {
        uint8_t curr_byte = this->data.at(i);
        value = (value << 8) + curr_byte;
    }
    this->bytes_read += len;
    return value;
};