#include <MIDI/keyboard.hpp>

std::string midi::KeyBoard::midi_to_note(int midi_value, bool sharp)
{
    if (sharp) {
        return NOTE_DICT_SHARP[midi_value] + std::to_string(midi_value / 12 - 2);
    }
    else
    {
        return NOTE_DICT_FLAT[midi_value] + std::to_string(midi_value / 12 - 2);
    }
}