#include <MIDI/track.hpp>

namespace midi
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS Track //----------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////

// Constants //------------------------------------------------------------------------------------

const std::map<int, std::string> Track::MIDI_META_EVENTS = {
    {0x00, "0 - Sequence Number (len, Number_part1 MSB, Number_part2 LSB)"},
    {0x01, "1 - Text Event"},
    {0x02, "2 - CopyRight Notice"},
    {0x03, "3 - Sequence/Track Name"},
    {0x04, "4 - Instrument Name"},
    {0x05, "5 - Lyrics"},
    {0x06, "6 - Marker"},
    {0x07, "7 - Cue Point"},
    {0x20, "32 - MIDI Channel Prefix"},
    {0x2F, "47 - End of Track"},
    {0x51, "81 - Set Tempo"},
    {0x54, "84 - SMTPE Offset"},
    {0x58, "88 - Time Signiture"},
    {0x59, "89 - Key Signiture"},
    {0x7F, "127 - Sequencer Specific"},
};

const std::map<int, std::string> Track::MIDI_REG_EVENTS = {
    {0x8, "8 - Note OFF (note, vel)"},
    {0x9, "9 - Note ON (note, vel)"},
    {0xA, "10 - Note Aftertouch (note, val)"},
    {0xB, "11 - Controller (controller, val)"},
    {0xC, "12 - Program Change (program, void)"},
    {0xD, "13 - Channel Aftertouch (val, void)"},
    {0xE, "14 - Pitch Bend (val lsb, val msb)"},
};

const std::map<int, std::string> Track::MIDI_CONTROLLER_TYPES = {
    {0x00, "0 - Band Slelect"},
    {0x01, "1 - Modulation"},
    {0x02, "2 - Breath Controller"},
    {0x04, "4 - Foot Controller"},
    {0x05, "5 - Portamento Time"},
    {0x06, "6 - Data Entry (MSB)"},
    {0x07, "7 - Main Volume"},
    {0x08, "8 - Balance"},
    {0x0A, "10 - Pan"},
    {0x0B, "11 - Expression Controller"},
    {0x0C, "12 - Effect Control 1"},
    {0x0D, "13 - Effect Control 2"},

    {0x10, "16 - General Purpose Controller 1"},
    {0x11, "17 - General Purpose Controller 2"},
    {0x12, "18 - General Purpose Controller 3"},
    {0x13, "19 - General Purpose Controller 4"},

    {0x20, "32 - LSB for Controller 0"},
    {0x21, "33 - LSB for Controller 1"},
    {0x22, "34 - LSB for Controller 2"},
    {0x23, "35 - LSB for Controller 3"},
    {0x24, "36 - LSB for Controller 4"},
    {0x25, "37 - LSB for Controller 5"},
    {0x26, "38 - LSB for Controller 6"},
    {0x27, "39 - LSB for Controller 7"},
    {0x28, "40 - LSB for Controller 8"},
    {0x29, "41 - LSB for Controller 9"},
    {0x2A, "42 - LSB for Controller 10"},
    {0x2B, "43 - LSB for Controller 11"},
    {0x2C, "44 - LSB for Controller 12"},
    {0x2D, "45 - LSB for Controller 13"},
    {0x2E, "46 - LSB for Controller 14"},
    {0x2F, "47 - LSB for Controller 15"},
    {0x30, "48 - LSB for Controller 16"},

    {0x31, "49 - LSB for Controller 17"},
    {0x32, "50 - LSB for Controller 18"},
    {0x33, "51 - LSB for Controller 19"},
    {0x34, "52 - LSB for Controller 20"},
    {0x35, "53 - LSB for Controller 21"},
    {0x36, "54 - LSB for Controller 22"},
    {0x37, "55 - LSB for Controller 23"},
    {0x38, "56 - LSB for Controller 24"},
    {0x39, "57 - LSB for Controller 25"},
    {0x3A, "58 - LSB for Controller 26"},
    {0x3B, "59 - LSB for Controller 27"},
    {0x3C, "60 - LSB for Controller 28"},
    {0x3D, "61 - LSB for Controller 29"},
    {0x3E, "62 - LSB for Controller 30"},
    {0x3F, "63 - LSB for Controller 31"},

    {0x40, "64 - Damper Pedal (Sustain)"},
    {0x41, "65 - Portamento"},
    {0x42, "66 - Sostenuto"},
    {0x43, "67 - Soft Pedal"},
    {0x44, "68 - Legato Footswitch"},
    {0x45, "69 - Hold 2"},

    {0x45, "70 - Sound Controller 1 (Default: Timber Variation)"},
    {0x47, "71 - Sound Controller 2 (Default: Timber/Harmonic Content)"},
    {0x48, "72 - Sound Controller 3 (Default: Release Time)"},
    {0x49, "73 - Sound Controller 4 (Default: Attach Time)"},
    {0x4A, "74 - Sound Controller 5"},
    {0x4B, "75 - Sound Controller 6"},
    {0x4C, "76 - Sound Controller 7"},
    {0x4D, "77 - Sound Controller 8"},
    {0x4E, "78 - Sound Controller 9"},
    {0x4F, "79 - Sound Controller 10"},

    {0x50, "80 - General Purpose Controller 5"},
    {0x51, "81 - General Purpose Controller 6"},
    {0x52, "82 - General Purpose Controller 7"},
    {0x53, "83 - General Purpose Controller 8"},

    {0x54, "84 - Portamento Control"},
    {0x5B, "91 - Effects 1 Depth (formerly External Effects Depth)"},
    {0x5C, "92 - Effects 2 Depth (formerly Tremolo Depth)"},
    {0x5D, "93 - Effects 3 Depth (formerly Chorus Depth)"},
    {0x5E, "94 - Effects 4 Depth (formerly Celeste Depth)"},
    {0x5F, "95 - Effects 5 Depth (formerly Phaser Depth)"},
    {0x60, "96 - Data Increment"},
    {0x61, "97 - Data Decrement"},
    {0x62, "98 - Non-Registered Parameter Number (LSB)"},
    {0x63, "99 - Non-Registered Parameter Number (MSB)"},
    {0x64, "100 - Registered Parameter Number (LSB)"},
    {0x65, "101 - Registered Parameter Number (MSB)"},

    {0x79, "121 - Mode Message"},
    {0x7A, "122 - Mode Message"},
    {0x7B, "123 - Mode Message"},
    {0x7C, "124 - Mode Message"},
    {0x7D, "125 - Mode Message"},
    {0x7E, "126 - Mode Message"},
    {0x7F, "127 - Mode Message"},
};

const std::map<int, uint8_t> Track::REG_EVENT_PARAM_LENGTHS = {
    {0x8, 2}, // 8 - Note OFF (note, vel)
    {0x9, 2}, // 9 - Note ON (note, vel)
    {0xA, 2}, // 10 - Note Aftertouch (note, val)
    {0xB, 2}, // 11 - Controller (controller, val)
    {0xC, 1}, // 12 - Program Change (program, void)
    {0xD, 1}, // 13 - Channel Aftertouch (val, void)
    {0xE, 2}, // 14 - Pitch Bend (val lsb, val msb)
};

// Constructors //---------------------------------------------------------------------------------

Track::~Track(){};

Track::Track(){};

// Methods //--------------------------------------------------------------------------------------

// Public:

error_status_t Track::read_track_PPQN(Reader& reader)
{
    int flag = 0;
    int num_events = 0;
    uint64_t total_time = 0;

    std::vector<char> track_identifier = reader.get_next(TRACK_IDENTIFIER_LENGTH);
    std::string head = std::string(track_identifier.begin(), track_identifier.end());
    if (head != "MTrk") {
        std::cout << "There is no track identifier present. Found: " << std::hex << head << std::endl;
        exit(EXIT_FAILURE);
        return -1;
    }

    int32_t track_length = reader.read_fixed_length(4);

    std::cout << "Track Length: " << track_length << "\n";

    while (flag == 0)
    {
        event_t next_event;

        uint32_t delta_time = reader.read_variable_length();
        next_event.delta_time = delta_time;

        total_time += delta_time;
        next_event.absolute_time = total_time;

        uint8_t event_type = reader.read_fixed_length(1);
        if (event_type == META_EVENT_IDENTIFIER)
        {
            flag = handle_next_meta_event(reader, next_event);
        }
        else
        {
            flag = handle_next_regular_event(reader, next_event, event_type);
        }
        event_list.push_back(next_event);
    }

    return 0;
};

std::vector<event_t> Track::get_events()
{
    return event_list;
}

std::string Track::get_name()
{
    return track_name;
}

// Private:

error_status_t Track::handle_next_meta_event(Reader& reader, event_t& event)
{
    event.category = meta;
    
    int event_subtype = reader.read_fixed_length(1);
    uint32_t event_length = reader.read_variable_length();
    std::vector<char> event_data = reader.get_next(event_length);

    // Process data
    event.event_type = event_subtype;

    // End
    
    // /*DEBUG*/ std::cout << "Meta Event: " << MIDI_META_EVENTS.at(event_subtype) << std::endl;

    if (event_subtype == END_OF_TRACK_IDENTIFIER)
    {
        return -1;
    }
    return 0;

};

error_status_t Track::handle_next_regular_event(Reader& reader, event_t& event, uint8_t type_and_channel)
{
    event.category = regular;

    uint8_t event_type = (type_and_channel & EVENT_TYPE_MASK) >> 4;
    uint8_t midi_channel = (type_and_channel & MIDI_CHANNEL_MASK);

    // /*DEBUG*/ std::cout << "Reg Event: " << MIDI_REG_EVENTS.at(event_type) << "\n";

    event.event_type = event_type;

    uint8_t event_args[2];

    for (size_t i = 0; i < REG_EVENT_PARAM_LENGTHS.at(event_type); i++)
    {
        event.args[i] = reader.read_fixed_length(1);
        // /*DEBUG*/ std::cout << "Event argument: " << event.args[i] << "\n";
    }
    return 0;
};

} // namspace midi