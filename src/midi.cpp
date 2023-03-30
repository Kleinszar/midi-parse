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

// Methods //--------------------------------------------------------------------------------------

// Public:

error_status_t midi::Midi::parseMidi(std::string fileName)
{
    
    error_status_t error;

    error = fileStreamReader_.openFileStream(fileName);
    if (error)
    {
        std::cout << "Error opening file." << std::endl;
        return -1;
    }

    error = parseHeader();
    if (error)
    {
        std::cout << "Problem parsing header." << std::endl;
        return -1;
    };

    if (numTracks_ <= 0) {
        std::cout << "There are no tracks in the file" << std::endl;
        return -1;
    }

    // Clear previous data
    allTracks_.clear();

    // Parse all tracks
    std::cout << "num_tracks: " << numTracks_ << std::endl;
    for (size_t i = 0; i < numTracks_; i++) {
        std::cout << "Reading track " << i << std::endl;
        error = readTrack();
        if (error) {
            std::cout << "Error reading track " << i << std::endl;
            return error;
        }
    }
    return 0;
}

std::vector<Track> Midi::allTracks()
{
    return allTracks_;
}

// Private:

error_status_t Midi::parseHeader()
{
    if (!fileStreamReader_.isGood())
    {
        std::cout << "File not found" << std::endl;
        return -1;
    }

    // Read Header contents
    std::vector<char> header_indentifier = fileStreamReader_.getNext(HEADER_IDENTIFIER_LENGTH);
    std::string header = std::string(header_indentifier.begin(), header_indentifier.end());
    if (header != "MThd") {
        std::cout << "File is not a midi file, Found Header as: " << std::hex << header << std::endl;
        return -1;
    }
    uint64_t header_length = fileStreamReader_.readFixedLength(HEADER_LENGTH_SIZE);
    std::cout << "Header Length: " << header_length << std::endl;

    if (header_length != STANDARD_HEADER_LENGTH)
    {
        std::cout << "Header length differs from current header standard." << std::endl;
        return -1;
    }

    // Parse time format
    uint16_t format = fileStreamReader_.readFixedLength(2);
    uint16_t number_of_tracks = fileStreamReader_.readFixedLength(2);
    uint16_t division = fileStreamReader_.readFixedLength(2);


    formatType_ = format;
    std::cout << "Format - " << FILE_FORMAT.at(formatType_) << std::endl;

    numTracks_ = number_of_tracks;
    std::cout << "Number of Tracks: " << numTracks_ << std::endl;

    divisionType_ = (division & DIVISION_TIME_MASK) >> 15;
    std::cout << "Division Type - " << DIVISION_TYPE.at(divisionType_) << std::endl;

    if (divisionType_ == 0) // PPQN
    {
        divisionTime_ = (division & DIVISION_TIME_MASK);
        std::cout << "Beats per Quarter Note: " << divisionTime_ << std::endl;
    }
    else // SMPTE
    {
        int8_t smtpe_format = (division & SMTPE_FORMAT_MASK) >> 8;
        fps_ = ~division + 1; // 2's complement inversion
        frameResolution_ = (division & TICKS_PER_FRAME_MASK); // Number of subdivision per frame
        std::cout << "fps: " << fps_ << "\t" << "resolution: " << frameResolution_ << std::endl;
    }
    return 0;
};

error_status_t Midi::readTrack()
{
    error_status_t error = 0;
    Track curr_track;
    error = curr_track.readTrackPPQN(fileStreamReader_);
    
    if (error)
    {
        std::cout << "Problem reading the track" << std::endl;
        return error;
    };

    std::vector<Event> events = curr_track.allEvents();
    std::cout << "Num Events: " << events.size() << std::endl;
    allTracks_.push_back(curr_track);
    return error;
};

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

    switch (eventType) {
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