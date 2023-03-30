#include <MIDI/utils.hpp>

namespace midi
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS Reader //---------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////

// Methods //--------------------------------------------------------------------------------------

// Public:

error_status_t Reader::openFileStream(std::string fileName)
{
    std::ifstream inputStream(fileName, std::ios::binary);

    if (!inputStream.is_open())
    {
        perror(("Cannot open file " + fileName).c_str());
        return -1;
    }

    data = std::vector<uint8_t>(
        (std::istreambuf_iterator<char>(inputStream)),
        (std::istreambuf_iterator<char>())
    );

    bytesRead = 0;

    if(inputStream.bad())
    {
        perror(("Error while reading file " + fileName).c_str());
        return -1;
    }

    return 0;
}

bool Reader::isGood()
{
    return !data.empty();
}

std::vector<char> Reader::getNext(uint64_t numBytes)
{
    auto position = data.begin() + bytesRead;
    std::vector<char> next_block = std::vector<char>(position, position + numBytes);
    bytesRead += numBytes;
    return next_block;
};

uint32_t Reader::readVariableLength()
{
    uint64_t currPosition = bytesRead;
    uint64_t length = 0;
    uint32_t value = 0;

    /*
    * A variable length quantity is represented (lower) 7 bits per byte, most significant bits first.
    * Up to a max of 4 bytes (32 bits). Bit 7 is a flag, that is set if the value has not ended.
    * All 1 byte chunks have bit 7 set except for the last chunk, indicating the end.
    */
    for (uint64_t i = currPosition; i < currPosition + MAX_BYTES; i++)
    {
        uint8_t currByte = data.at(i);
        value = (value << BITS_LENGTH) + (currByte & MASK_VARIABLE_LENGTH_VALUE);
        ++length;
        // Check flag for end of variable value, set = not ended.
        int flag = (currByte & MASK_VARIABLE_LENGTH_FLAG) >> FLAG_OFFSET;

        if (flag == 0)
        {
            break;
        }
    }
    bytesRead += length;
    return value;
};

uint64_t Reader::readFixedLength(uint64_t len)
{

    uint64_t currPosititon = bytesRead;
    uint64_t value = 0;
    for (uint64_t i = currPosititon; i < currPosititon + len; i++)
    {
        uint8_t currByte = data.at(i);
        value = (value << 8) + currByte;
    }
    bytesRead += len;
    return value;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS Writer //---------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////

// TODO: Implement.

} //namespace midi