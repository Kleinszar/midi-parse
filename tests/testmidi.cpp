#include <iostream>
#include <MIDI/midi.hpp>

using namespace std;
using namespace midi;

int main()
{
    Midi testMidi;
    int ret = testMidi.parseMidi("PPQN.mid");
    vector<Track> tracks = testMidi.allTracks();

    cout << "Listing track information" << endl;

    for(vector<Track>::iterator track = tracks.begin(); track < tracks.end(); track++)
    {
        cout << "Printing Track" << endl;
        vector<Event> events = track->allEvents();
        for (vector<Event>::iterator event = events.begin(); event < events.end(); event++)
        {
            if (event->category == EventCategory::META)
            {
                cout << "Meta Event" << endl;
            } 
            else if (event->category == EventCategory::REGULAR)
            {
                cout << "Regular Event" << endl;
            }
            cout << "ID: " << event->eventType << endl;
            cout << "DeltaT: " << event->deltaTime << endl;
            cout << "Arguments: " << hex <<int(event->args[0]) << "\t" << int(event->args[1]) << endl;
            cout << "absolute time: " << event->absoluteTime << endl;
        }
    }
    return ret;
};