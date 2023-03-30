#include <iostream>
#include <MIDI/midi.hpp>

using namespace std;
using namespace midi;

int main()
{
    Midi testMidi;
    int ret = testMidi.parseMidi("PPQN.mid");
    vector<Track> tracks = testMidi.getTracks();

    cout << "Listing track information" << endl;

    for(vector<Track>::iterator track = tracks.begin(); track < tracks.end(); track++)
    {
        cout << "Printing Track" << endl;
        vector<Event> events = track->getEvents();
        for (vector<Event>::iterator event = events.begin(); event < events.end(); event++)
        {
            if (event->category == EventCategory::meta)
            {
                cout << "Meta Event" << endl;
            }
            cout << "ID: " << event->eventType << endl;
            cout << "DeltaT: " << event->deltaTime << endl;
            cout << "Arguments: " << hex <<int(event->args[0]) << "\t" << int(event->args[1]) << endl;
            cout << "absolute time: " << event->absoluteTime << endl;
        }
    }
    return ret;
};