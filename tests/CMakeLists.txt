# Utils
add_executable(Test_MIDI_Util test_utils.cpp)
target_link_libraries(Test_MIDI_Util midi_lib)
target_include_directories(Test_MIDI_Util PRIVATE ${PROJECT_SOURCE_DIR}/include)

# Main Program
add_executable(Test_MIDI_Main test.cpp)
target_link_libraries(Test_MIDI_Main midi_lib)
target_include_directories(Test_MIDI_Main PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_compile_options(Test_MIDI_Main PRIVATE -Wall)

#add_test(NAME Test_Utils COMMAND Test_Utils)
add_test(NAME "MIDI_Utilities" COMMAND Test_MIDI_Util)
add_test(NAME "MIDI_Main" COMMAND Test_MIDI_Main)

#Files
configure_file("in/PPQN.mid" "PPQN.mid" COPYONLY)