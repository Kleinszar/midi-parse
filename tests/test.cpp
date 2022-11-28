#include <midi.h>

int main()
{
    Midi test_midi("PPQN.mid");
    int ret = test_midi.parse_midi();
    return ret;
};