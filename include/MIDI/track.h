#ifndef TRACK
#define TRACK

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "utils.h"


enum event_category {meta, regular};

typedef struct event_meta_data {
    event_category category;
    int64_t id;
    uint64_t delta_time;
    uint64_t absolute_time;
    uint8_t args[2];
} event_t;

class Track {
    private:
        static constexpr uint8_t EVENT_TYPE_MASK = {0b1111'0000};
        static constexpr uint8_t MIDI_CHANNEL_MASK = {0b0000'1111};
        static constexpr size_t TRACK_IDENTIFIER_LENGTH = 4;


        static const std::map<int, std::string> MIDI_META_EVENTS;
        static const std::map<int, std::string> MIDI_REG_EVENTS;
        static const std::map<int, std::string> MIDI_CONTROLLER_TYPES;
        static const std::map<int, uint8_t> REG_EVENT_PARAM_LENGTHS;

        Reader* file_stream;
        uint64_t track_length;
        uint32_t time_signiture_numerator;
        uint32_t time_signiture_denominator;
        std::vector<event_t> event_list;
        
        int handle_next_meta_event(event_t& event);
        int handle_next_regular_event(event_t& event, uint8_t type_and_channel);
        
    public:
        static constexpr uint8_t META_EVENT_IDENTIFIER = 0xFF;
        static constexpr uint8_t END_OF_TRACK_IDENTIFIER = 0x2F;

        std::string track_name;

        Track(Reader* file_reader);
        ~Track();
        
        int read_track_PPQN();
        int read_track_SMTPE();
        std::vector<event_t> get_events();
};

#endif