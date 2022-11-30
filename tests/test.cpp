#include <MIDI/midi.h>

int main()
{
    Midi test_midi("PPQN.mid");
    int ret = test_midi.parse_midi();
    std::vector<Track> tracks = test_midi.get_tracks();

    std::cout << "Listing track information" << std::endl;

    for(std::vector<Track>::iterator track = tracks.begin(); track < tracks.end(); track++)
    {
        std::cout << "Printing Track" << std::endl;
        std::vector<event_t> events = track->get_events();
        for (std::vector<event_t>::iterator event = events.begin(); event < events.end(); event++)
        {
            std::cout << "Category: " << event->category << std::endl;
            std::cout << "ID: " << event->id << std::endl;
            std::cout << "DeltaT: " << event->delta_time << std::endl;
            std::cout << "Arguments: " << std::hex <<int(event->args[0]) << "\t" << int(event->args[1]) << std::endl;
            std::cout << "absolute time: " << event->absolute_time << std::endl;
        }
    }
    return ret;
};