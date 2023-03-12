#include <MIDI/midi.hpp>

namespace midi
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS Midi //-----------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////

// Constants //------------------------------------------------------------------------------------

const std::map<int, std::string> Midi::FILE_FORMAT = {
    {0, "Single Track"},
    {1, "Multi Track Simultaneous"},
    {2, "Multi Track Independent"},
};

const std::map<int, std::string> Midi::DIVISION_TYPE = {
    {0, "PPQN"},
    {1, "SMTPE"},
};

// Constructors //---------------------------------------------------------------------------------

Midi::Midi(){};

Midi::~Midi(){};

Midi::Midi(std::string file_name)
{
    file_stream.read_file(file_name);
};

// Methods //--------------------------------------------------------------------------------------

// Public:

error_status_t midi::Midi::parse_midi()
{
    error_status_t error;
    error = parse_header();
    if (error)
    {
        std::cout << "Problem parsing Header" << std::endl;
        return -1;
    };

    if (num_tracks <= 0) {
        std::cout << "There are no tracks in the file" << std::endl;
        return -1;
    }
    std::cout << "num_tracks: " << num_tracks << std::endl;
    for (size_t i = 0; i < num_tracks; i++) {
        std::cout << "Reading track " << i << std::endl;
        error = read_track();
        if (error) {
            std::cout << "Error reading track " << i << std::endl;
            return error;
        }
    }
    return 0;
}

std::vector<Track> Midi::get_tracks()
{
    return all_tracks;
}

// Private:

error_status_t Midi::parse_header()
{
    if (!file_stream.is_good())
    {
        std::cout << "File not found" << std::endl;
        return -1;
    }

    // Read Header contents
    std::vector<char> header_indentifier = file_stream.get_next(HEADER_IDENTIFIER_LENGTH);
    std::string header = std::string(header_indentifier.begin(), header_indentifier.end());
    if (header != "MThd") {
        std::cout << "File is not a midi file, Found Header as: " << std::hex << header << std::endl;
        return -1;
    }
    uint64_t header_length = file_stream.read_fixed_length(HEADER_LENGTH_SIZE);
    std::cout << "Header Length: " << header_length << std::endl;

    if (header_length != STANDARD_HEADER_LENGTH)
    {
        std::cout << "Header length differs from current header standard." << std::endl;
        return -1;
    }

    // Parse time format
    uint16_t format = file_stream.read_fixed_length(2);
    uint16_t number_of_tracks = file_stream.read_fixed_length(2);
    uint16_t division = file_stream.read_fixed_length(2);


    format_type = format;
    std::cout << "Format - " << FILE_FORMAT.at(format_type) << std::endl;

    num_tracks = number_of_tracks;
    std::cout << "Number of Tracks: " << num_tracks << std::endl;

    division_type = (division & DIVISION_TIME_MASK) >> 15;
    std::cout << "Division Type - " << DIVISION_TYPE.at(division_type) << std::endl;

    if (division_type == 0) // PPQN
    {
        division_time = (division & DIVISION_TIME_MASK);
        std::cout << "Beats per Quarter Note: " << division_time << std::endl;
    }
    else // SMPTE
    {
        int8_t smtpe_format = (division & SMTPE_FORMAT_MASK) >> 8;
        fps = ~division + 1; // 2's complement inversion
        frame_resolution = (division & TICKS_PER_FRAME_MASK); // Number of subdivision per frame
        std::cout << "fps: " << fps << "\t" << "resolution: " << frame_resolution << std::endl;
    }
    return 0;
};

error_status_t Midi::read_track()
{
    error_status_t error = 0;
    Track curr_track;
    error = curr_track.read_track_PPQN(file_stream);
    
    if (error)
    {
        std::cout << "Problem reading the track" << std::endl;
        return error;
    };

    std::vector<event_t> events = curr_track.get_events();
    std::cout << "Num Events: " << events.size() << std::endl;
    all_tracks.push_back(curr_track);
    return error;
};

error_status_t Midi::open_to_read(std::string file_name)
{
    Midi::file_stream = Reader();
    return 0;
}

/*
    switch(event_subtype)
    {
        case 0x00: 0 ; break; // 0 - Sequence Number (len, Number_part1 MSB, Number_part2 LSB)
        case 0x01: 0 ; break; // 1 - Text Event
        case 0x02: 0 ; break; // 2 - CopyRight Notice
        case 0x03: 1 ; break; // 3 - Sequence/Track Name
        case 0x04: 0 ; break; // 4 - Instrument Name
        case 0x05: 0 ; break; // 5 - Lyrics
        case 0x06: 0 ; break; // 6 - Marker
        case 0x07: 0 ; break; // 7 - Cue Point
        case 0x20: 0 ; break; // 32 - MIDI Channel Prefix
        case 0x2F: -1 ; break; // 47 - End of Track
        case 0x51: 0 ; break; // 81 - Set Tempo
        case 0x54: 0 ; break; // 84 - SMTPE Offset
        case 0x58: 1 ; break; // 88 - Time Signiture
        case 0x59: 1 ; break; // 89 - Key Signiture
        case 0x7F: 0 ; break; // 127 - Sequencer Specific
        default: break; // Not implemented
    }

    switch (event_type) {
        case 0x8: break;// Note OF
        case 0x9: break;// Note ON
        case 0xA: break;// Note Aftertouch
        case 0xB: break;// Controller
        case 0xC: break;// Program Change
        case 0xD: break;// Channel Aftertouch
        case 0xE: break;// Pitch Bend
    }
*/

} // namespace midi