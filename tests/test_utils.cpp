#include <fstream>
#include <string>
#include <iostream>
#include <ios>

#include <array>

// Testing utils
#include <MIDI/utils.h>

using namespace std;

// Writes a series of hex values represented in by string to a file in bytes
// Overwrites the previous file
// Credit: ElectRocnic
// https://stackoverflow.com/questions/21819782/writing-hex-to-a-file
int write_test_file(string file_name, string hex_data)
{
    string hex_string = hex_data;

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
    ofstream output_file(file_name, ofstream::binary | ofstream::out | ofstream::trunc);

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
int test_vl()
{   
    array<uint32_t, 12> expected_values = {
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
    
    string test_file = "test_utils.txt";

    cout << "\n__Writing test file__\n" << endl;
    write_test_file(test_file,
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
    Reader vl_read;
    vl_read.read_file(test_file);
    if (!vl_read.file_good)
    {
        cout << "File does not exits" << endl;
        return 1;
    }

    cout << endl << "Checking values" << endl;
    bool flag = false;
    for (size_t i = 0; i < expected_values.size(); i++)
    {
        cout << "*" << i+1 << "* Testing " << hex << expected_values[i] << endl;
        int value = vl_read.read_variable_length();
        cout << "Got " << hex << value;
        if (expected_values[i] == value)
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
    int ret_val = test_vl();
    return ret_val;
};