#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "utils.hpp"

namespace midi
{

// Events split into two categories.
enum class EventCategory
{
    meta,
    regular
};

// Event meta data container.
struct Event
{
    uint64_t deltaTime;
    uint64_t absoluteTime;
    int16_t eventType;
    uint8_t args[2];
    EventCategory category;
};

class Track
{
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
    std::string trackName;
    uint64_t trackLength;
    uint32_t timeSignitureNumerator;
    uint32_t timeSignitureDenominator;
    std::vector<Event> eventList;

// Methods //--------------------------------------------------------------------------------------
public:
    // Track reading

    /**
     * Reads and consumes a track from the Reader.
     * @note For tracks using PPQN time format.
     * @param reader The reader to get data from.
     * @return Error status, 0 for success.
    */
    error_status_t readTrackPPQN(Reader& reader);
    
    /**
     * Reads and consumes a track from the Reader.
     * @note For tracks using SMTPE time format.
     * @param reader The reader to get data from.
     * @return Error status, 0 for success.
     * TODO: UNIMPLEMENTED
    */
    error_status_t readTrackSMTPE(Reader& reader);

    /**
     * Gets the list of events.
     * @return A vector of all events in the track.
    */
    std::vector<Event> getEvents();

    /**
     * Gets the name of the track.
     * @return The name.
    */
    std::string getName();


private:
    // Helpers
    /**
     * Parse the next event as a meta event.
     * @param reader The reader to get data from.
     * @param event The event object to store the parsed information.
     * @return Error status, 0 for sucess.
    */
    error_status_t handleNextMetaEvent(Reader& reader, Event& event);

    /**
     * Parse the next event as a regular event.
     * @param reader The reader to get data from.
     * @param event The event object to store the parsed information.
     * @param type_and_channel Container for event type and channel number.
     * @return Error status, 0 for sucess.
    */
    error_status_t handleNextRegularEvent(Reader& reader, Event& event, uint8_t typeAndChannel);

};

} // namespace midi
