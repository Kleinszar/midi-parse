#include <fstream>
#include <string>
#include <iostream>
#include <ios>

#include <array>

// Testing utils
#include <MIDI/utils.hpp>

using namespace std;
using namespace midi;

// Writes a series of hex values represented in by string to a file in bytes
// Overwrites the previous file
// Credit: ElectRocnic
// https://stackoverflow.com/questions/21819782/writing-hex-to-a-file
int writeTestFile(string fileName, string hexData)
{
    string hex_string = hexData;

    basic_string<uint8_t> bytes;

    cout << "LENGTH: " << hex_string.length() << endl;

    cout << "BYTE DATA: ";
    for (size_t i = 0; i < hex_string.length(); i += 2)
    {
        uint16_t byte;

        string next_byte = hex_string.substr(i, 2);

        (i % 16 == 0) ? (cout << endl << next_byte << " ") : (cout << next_byte << " ");

        // Put the pair into an istringstream and stream it through std::hex for
        // conversion into an integer value.
        // This will calculate the byte value of your string-represented hex value.
        istringstream(next_byte) >> hex >> byte;

        // As the stream above does not work with uint8 directly,
        // we have to cast it now.
        // As every pair can have a maximum value of "ff",
        // which is "11111111" (8 bits), we will not lose any information during this cast.
        // This line adds the current byte value to our final byte "array".
        bytes.push_back(static_cast<uint8_t>(byte));
    }
    cout << endl << "END DATA" << endl;

    // we are now generating a string obj from our bytes-"array"
    // this string object contains the non-human-readable binary byte values
    string result(begin(bytes), end(bytes));

    // Write tests to file
    ofstream output_file(fileName, ofstream::binary | ofstream::out | ofstream::trunc);

    if (output_file.is_open())
    {
        output_file << result;
        output_file.close();
        return 0;
    }
    else
    {
        cout << "Error: Could not create file." << endl;
        return 1;
    }
};


// Test variable length read from a file
int testVl()
{   
    array<uint32_t, 12> expectedValues = {
        0x00000000,
        0x00000040,
        0x0000007F,
        0x00000080,
        0x00002000,
        0x00003FFF,
        0x00004000,
        0x00100000,
        0x001FFFFF,
        0x00200000,
        0x08000000,
        0x0FFFFFFF
    };
    
    string testFile = "test_utils.txt";

    cout << "\n__Writing test file__\n" << endl;
    writeTestFile(testFile,
        "00"
        "40"
        "7F"
        "8100"
        "C000"
        "FF7F"
        "818000"
        "C08000"
        "FFFF7F"
        "81808000"
        "C0808000"
        "FFFFFF7F");


    cout << endl << "Testing Read File" << endl << endl;
    Reader vlRead;
    int error = vlRead.openFileStream(testFile);
    if (error)
    {
        cout << "File does not exits" << endl;
        return 1;
    }

    cout << endl << "Checking values" << endl;
    bool flag = false;
    for (size_t i = 0; i < expectedValues.size(); i++)
    {
        cout << "*" << i+1 << "* Testing " << hex << expectedValues[i] << endl;
        int value = vlRead.readVariableLength();
        cout << "Got " << hex << value;
        if (expectedValues[i] == value)
        {
            cout << " Pass" << endl;
        } else
        {
             cout << " Fail" << endl;
             flag = true;
        }
    }
    return flag ? 1 : 0;

};

int main()
{
    // Test variable length reads
    int error = testVl();
    return error;
};