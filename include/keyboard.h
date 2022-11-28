#ifndef KEYBOARD
#define KEYBOARD

#include <iostream>
#include <string>
#include <map>

static std::map<int, std::string> NOTE_DICT_SHARP = {
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
static std::map<int, std::string> NOTE_DICT_FLAT = {
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

class KeyBoard {
    private:
    public:
        std::string midi_to_note(int midi_value, bool sharp);
        void note_on();
};

#endif
