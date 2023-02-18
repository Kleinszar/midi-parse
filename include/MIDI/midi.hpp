#ifndef MIDI
#define MIDI

#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <cassert>

#include "track.hpp"
#include "utils.hpp"

namespace midi
{
    class Midi {
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
            
            uint64_t format_type;
            uint64_t division_type;
            Reader current_file;

            uint16_t division_time;
            uint16_t num_tracks;
            uint8_t fps;
            uint8_t frame_resolution;

            std::vector<Track> all_tracks;

        public:
            Midi();
            Midi(std::string file_name);
            ~Midi();

            uint32_t read_var();
            uint64_t read_fixed();

            int open_to_read(std::string file_name);
            int parse_header();
            int read_track();
            int parse_midi();

            std::vector<Track> get_tracks();
    };
}

#endif