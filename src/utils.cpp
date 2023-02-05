#include <MIDI/utils.h>

// Constants //---------------------------------------------------------------------------------------


// Constructors //------------------------------------------------------------------------------------
midi::Reader::Reader()
{
    bytes_read = 0;
    file_good = false;
};

midi::Reader::~Reader()
{
    data.clear();
};

// Static //------------------------------------------------------------------------------------------

// Member //------------------------------------------------------------------------------------------
int midi::Reader::read_file(std::string file_name)
{
    std::ifstream input_stream(file_name, std::ios::binary);

    if (!input_stream.is_open())
    {
        perror(("Cannot open file " + file_name).c_str());
        return -1;
    }

    data = std::vector<uint8_t>(
        (std::istreambuf_iterator<char>(input_stream)),
        (std::istreambuf_iterator<char>())
    );

    if(input_stream.bad())
    {
        perror(("Error while reading file " + file_name).c_str());
        return -1;
    }

    file_good = true;
    return 0;
}


std::vector<char> midi::Reader::get_next(uint64_t num_bytes)
{
    auto position = data.begin() + bytes_read;
    std::vector<char> next_block = std::vector<char>(position, position + num_bytes);
    bytes_read += num_bytes;
    return next_block;
};



/*
 * A variable length quantity is represented (lower) 7 bits per byte, most significant bits first.
 * Up to a max of 4 bytes (32 bits). Bit 7 is a flag, that is set if the value has not ended.
 * All 1 byte chunks have bit 7 set except for the last chunk, indicating the end.
*/
uint32_t midi::Reader::read_variable_length()
{
    uint64_t curr_position = bytes_read;
    uint64_t length = 0;
    uint32_t value = 0;

    for (uint64_t i = curr_position; i < curr_position + MAX_BYTES; i++)
    {
        uint8_t curr_byte = data.at(i);
        value = (value << BITS_LENGTH) + (curr_byte & MASK_VARIABLE_LENGTH_VALUE);
        ++length;
        // Check flag for end of variable value, set = not ended.
        int flag = (curr_byte & MASK_VARIABLE_LENGTH_FLAG) >> FLAG_OFFSET;

        if (flag == 0)
        {
            break;
        }
    }
    bytes_read += length;
    return value;
};

/**
 *  Assumes that most significant bits are placed first. Reads an unsigned value.
*/
uint64_t midi::Reader::read_fixed_length(uint64_t len)
{
    uint64_t curr_posititon = bytes_read;
    uint64_t value = 0;
    for (uint64_t i = curr_posititon; i < curr_posititon + len; i++)
    {
        uint8_t curr_byte = data.at(i);
        value = (value << 8) + curr_byte;
    }
    bytes_read += len;
    return value;
};