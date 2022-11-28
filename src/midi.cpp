#include <MIDI/midi.h>

using namespace std;

// Constants //---------------------------------------------------------------------------------------
const std::map<int, std::string> Midi::FILE_FORMAT = {
    {0, "Single Track"},
    {1, "Multi Track Simultaneous"},
    {2, "Multi Track Independent"},
};

const std::map<int, std::string> Midi::DIVISION_TYPE = {
    {0, "PPQN"},
    {1, "SMTPE"},
};


// Constructors //------------------------------------------------------------------------------------
Midi::Midi(){};
Midi::~Midi(){};

Midi::Midi(string file_name)
{
    this->current_file.read_file(file_name);
};

// Private //-----------------------------------------------------------------------------------------

int Midi::parse_header()
{
    if (!this->current_file.file_good)
    {
        std::cout << "File not found" << std::endl;
        return -1;
    }

    // Read Header contents
    std::vector<char> header_indentifier = this->current_file.get_next(HEADER_IDENTIFIER_LENGTH);
    string header = string(header_indentifier.begin(), header_indentifier.end());
    if (header != "MThd") {
        std::cout << "File is not a midi file, Found Header as: " << hex << header << std::endl;
        return -1;
    }
    uint64_t header_length = this->current_file.read_fixed_length(HEADER_LENGTH_SIZE);
    std::cout << "Header Length: " << header_length << std::endl;

    if (header_length != STANDARD_HEADER_LENGTH)
    {
        std::cout << "Header length differs from current header standard." << std::endl;
        return -1;
    }



    // Parse time format
    uint16_t format = this->current_file.read_fixed_length(2);
    uint16_t num_tracks = this->current_file.read_fixed_length(2);
    uint16_t division = this->current_file.read_fixed_length(2);


    this->format_type = format;
    std::cout << "Format - " << FILE_FORMAT.at(this->format_type) << std::endl;

    this->num_tracks = num_tracks;
    std::cout << "Number of Tracks: " << this->num_tracks << std::endl;

    this->division_type = (division & DIVISION_TIME_MASK) >> 15;
    std::cout << "Division Type - " << DIVISION_TYPE.at(this->division_type) << std::endl;

    if (this->division_type == 0) // PPQN
    {
        this->division_time = (division & DIVISION_TIME_MASK);
        std::cout << "Beats per Quarter Note: " << this->division_time << std::endl;
    }
    else // SMPTE
    {
        int8_t smtpe_format = (division & SMTPE_FORMAT_MASK) >> 8;
        this->fps = ~division + 1; // 2's complement inversion
        this->frame_resolution = (division & TICKS_PER_FRAME_MASK); // Number of subdivision per frame
        std::cout << "fps: " << this->fps << "\t" << "resolution: " << this->frame_resolution << std::endl;
    }
    return 0;
};

int Midi::read_track()
{
    int ret;
    Track curr_track(&this->current_file);
    ret = curr_track.read_track_PPQN();
    
    if (ret != 0)
    {
        std::cout << "Problem reading the track" << std::endl;
        return ret;
    };

    std::vector<event_t> events = curr_track.get_events();
    std::cout << "Num Events: " << events.size() << std::endl;

    for (std::vector<event_t>::iterator it = events.begin(); it != events.end(); it++)
    {
        std::cout << "Category: " << it->category << std::endl;
        std::cout << "ID: " << it->id << std::endl;
        std::cout << "DeltaT: " << it->delta_time << std::endl;
        std::cout << "Arguments: " << hex <<int(it->args[0]) << "\t" << int(it->args[1]) << std::endl;
    }
    return 0;
};

int Midi::parse_midi()
{
    int ret;
    ret = this->parse_header();
    if (ret != 0)
    {
        std::cout << "Problem parsing Header" << endl;
        return -1;
    };

    if (this->num_tracks <= 0) {
        std::cout << "There are no tracks in the file" << endl;
        return -1;
    }
    std::cout << "num_tracks: " << this->num_tracks << endl;
    for (size_t i = 0; i < this->num_tracks; i++) {
        std::cout << "Reading track " << i << endl;
        ret = this->read_track();
        if (ret != 0) {
            std::cout << "Error" << endl;
            break;
        }
    }
    return ret;
}

// Public //------------------------------------------------------------------------------------------


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