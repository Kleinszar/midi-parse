#pragma once

#include <iostream>
#include <string>
#include <map>

namespace midi
{

class KeyBoard
{
    
// Constants //------------------------------------------------------------------------------------
public:
    static const std::map<int, std::string> NOTE_DICT_SHARP;
    static const std::map<int, std::string> NOTE_DICT_FLAT;

// Variables //------------------------------------------------------------------------------------

//

// Constructors //---------------------------------------------------------------------------------
    
//

// Methods //--------------------------------------------------------------------------------------
public:
    std::string midiToNote(int midi_value, bool sharp);
    void noteOn();
};


} // namespace midi
