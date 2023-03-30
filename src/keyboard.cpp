#include <MIDI/keyboard.hpp>

namespace midi
{

///////////////////////////////////////////////////////////////////////////////////////////////////
// CLASS KeyBoard //-------------------------------------------------------------------------------
///////////////////////////////////////////////////////////////////////////////////////////////////

// Constants //------------------------------------------------------------------------------------

const std::map<int, std::string> KeyBoard::NOTE_DICT_SHARP = {
    {0, "C"},
    {1, "C#"},
    {2, "D"},
    {3, "D#"},
    {4, "E"},
    {5, "F"},
    {6, "F#"},
    {7, "G"},
    {8, "G#"},
    {9, "A"},
    {10, "A#"},
    {11, "B"},
};

const std::map<int, std::string> KeyBoard::NOTE_DICT_FLAT = {
    {0, "C"},
    {1, "C#"},
    {2, "D"},
    {3, "D#"},
    {4, "E"},
    {5, "F"},
    {6, "F#"},
    {7, "G"},
    {8, "G#"},
    {9, "A"},
    {10, "A#"},
    {11, "B"},
};


// Constructors //---------------------------------------------------------------------------------


// Methods //--------------------------------------------------------------------------------------

// Public:

std::string KeyBoard::midiToNote(int midiValue, bool sharp)
{
    if (sharp) {
        return NOTE_DICT_SHARP.at(midiValue) + std::to_string(midiValue / 12 - 2);
    }
    else
    {
        return NOTE_DICT_FLAT.at(midiValue) + std::to_string(midiValue / 12 - 2);
    }
}

} // namespace midi