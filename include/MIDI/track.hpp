#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "utils.hpp"

namespace midi
{

// Events split into two categories.
enum event_category {meta, regular};

// Event meta data container.
struct event_t {
    uint64_t delta_time;
    uint64_t absolute_time;
    int16_t event_type;
    uint8_t args[2];
    event_category category;
};


///////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS Track //----------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////
class Track {


// Constants //------------------------------------------------------------------------------------
private:
    static constexpr uint8_t EVENT_TYPE_MASK = {0b1111'0000};
    static constexpr uint8_t MIDI_CHANNEL_MASK = {0b0000'1111};
    static constexpr uint8_t TRACK_IDENTIFIER_LENGTH = 4;


    static const std::map<int, std::string> MIDI_META_EVENTS;
    static const std::map<int, std::string> MIDI_REG_EVENTS;
    static const std::map<int, std::string> MIDI_CONTROLLER_TYPES;
    static const std::map<int, uint8_t> REG_EVENT_PARAM_LENGTHS;

    static constexpr uint8_t META_EVENT_IDENTIFIER = 0xFF;
    static constexpr uint8_t END_OF_TRACK_IDENTIFIER = 0x2F;

// Variables //------------------------------------------------------------------------------------
private:
    std::string track_name;
    uint64_t track_length;
    uint32_t time_signiture_numerator;
    uint32_t time_signiture_denominator;
    std::vector<event_t> event_list;

// Constructors //---------------------------------------------------------------------------------
public:

    Track();

    ~Track();

// Methods //--------------------------------------------------------------------------------------
public:
    // Track reading

    /**
     * Reads and consumes a track from the Reader.
     * @note For tracks using PPQN time format.
     * @param reader The reader to get data from.
     * @return Error status, 0 for success.
    */
    error_status_t read_track_PPQN(Reader& reader);
    
    /**
     * Reads and consumes a track from the Reader.
     * @note For tracks using SMTPE time format.
     * @param reader The reader to get data from.
     * @return Error status, 0 for success.
     * TODO: UNIMPLEMENTED
    */
    error_status_t read_track_SMTPE(Reader& reader);

    /**
     * Gets the list of events.
     * @return A vector of all events in the track.
    */
    std::vector<event_t> get_events();

    /**
     * Gets the name of the track.
     * @return The name.
    */
    std::string get_name();


private:
    // Helpers
    /**
     * Parse the next event as a meta event.
     * @param reader The reader to get data from.
     * @param event The event object to store the parsed information.
     * @return Error status, 0 for sucess.
    */
    error_status_t handle_next_meta_event(Reader& reader, event_t& event);

    /**
     * Parse the next event as a regular event.
     * @param reader The reader to get data from.
     * @param event The event object to store the parsed information.
     * @param type_and_channel Container for event type and channel number.
     * @return Error status, 0 for sucess.
    */
    error_status_t handle_next_regular_event(Reader& reader, event_t& event, uint8_t type_and_channel);

};

} // namespace midi
