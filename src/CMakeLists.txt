set(midi_lib_src_files
    utils.cpp
    keyboard.cpp
    track.cpp
    midi.cpp
)

add_library(midi_lib ${midi_lib_src_files})

target_include_directories(midi_lib 
    PRIVATE ${PROJECT_SOURCE_DIR}/include
)