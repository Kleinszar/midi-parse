#include <MIDI/midi.h>

using namespace std;
using namespace midi;

int main()
{
    Midi test_midi("PPQN.mid");
    int ret = test_midi.parse_midi();
    vector<Track> tracks = test_midi.get_tracks();

    cout << "Listing track information" << endl;

    for(vector<Track>::iterator track = tracks.begin(); track < tracks.end(); track++)
    {
        cout << "Printing Track" << endl;
        vector<event_t> events = track->get_events();
        for (vector<event_t>::iterator event = events.begin(); event < events.end(); event++)
        {
            cout << "Category: " << event->category << endl;
            cout << "ID: " << event->id << endl;
            cout << "DeltaT: " << event->delta_time << endl;
            cout << "Arguments: " << hex <<int(event->args[0]) << "\t" << int(event->args[1]) << endl;
            cout << "absolute time: " << event->absolute_time << endl;
        }
    }
    return ret;
};