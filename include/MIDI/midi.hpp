#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cassert>

#include "track.hpp"
#include "utils.hpp"

namespace midi
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS Midi //-----------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////

class Midi {

// Constants //------------------------------------------------------------------------------------
private:
    static constexpr uint16_t TYPE_MASK = {0b1000'0000'0000'0000};
    static constexpr uint16_t DIVISION_TIME_MASK = {0b0111'1111'1111'1111};
    static constexpr uint16_t SMTPE_FORMAT_MASK = {0b0111'1111'0000'0000};
    static constexpr uint16_t TICKS_PER_FRAME_MASK = {0b0000'0000'1111'1111};

    static constexpr uint8_t HEADER_IDENTIFIER_LENGTH = 4;
    static constexpr uint8_t HEADER_LENGTH_SIZE = 4;
    static constexpr uint8_t STANDARD_HEADER_LENGTH = 6;

    static const std::map<int, std::string> FILE_FORMAT; 
    static const std::map<int, std::string> DIVISION_TYPE;

// Variables //------------------------------------------------------------------------------------
private:
    uint64_t format_type;
    uint64_t division_type;
    Reader file_stream;

    uint16_t division_time;
    uint16_t num_tracks;
    uint16_t fps;
    uint16_t frame_resolution;

    std::vector<Track> all_tracks;

// Constructors //---------------------------------------------------------------------------------
public:

    // Constructors

    Midi();

    /**
     * Create a midi object and opens a Reader given a name.
     * @param file_name The name of the file to open.
    */
    Midi(std::string file_name);

    ~Midi();

// Methods //--------------------------------------------------------------------------------------
public:
    // Midi parsing
    /**
     * Opens a Reader with a file name.
     * @param file_name The name of the file to open.
    */
    error_status_t open_to_read(std::string file_name);

    /**
     * Parses the data that the Reader has read.
     * Requires Reader to have data first.
     * Loads all the tracks contained in the Reader data.
     * @return Error status, 0 for success.
    */
    error_status_t parse_midi();

    /**
     * Gets all the parsed tracks.
     * @return A vector of the all the tracks.
    */
    std::vector<Track> get_tracks();

private:
    // Helpers
    /**
     * Parses the header from the Reader file.
     * @return Error status, 0 for success.
    */
    error_status_t parse_header();

    /**
     * Reads a single track from the Reader file.
     * @return Error status, 0 for success.
    */
    error_status_t read_track();

};

} // namespace midi