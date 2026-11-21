cmake_minimum_required(VERSION 3.22.1)
project(slate_synth C CXX)

# FluidSynth (LGPL-2.1), built as its own shared library (libfluidsynth.so) so users can replace it, with no audio drivers:
# the app pulls the samples itself and plays them through AudioTrack.
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
foreach(opt alsa aufile dbus jack ladspa libsndfile midishare network oss sdl3 pulseaudio pipewire readline openmp portaudio
            opensles oboe systemd native-dls signalsmith ipv6 dsound wasapi waveout winmidi)
    set(enable-${opt} OFF CACHE BOOL "" FORCE)
endforeach()
# FluidSynth reads the compressed soundfont (.sf3) with the built-in stb_vorbis decoder instead of libsndfile
add_compile_definitions(JAMSLATE_STB_VORBIS=1)
add_subdirectory(fluidsynth EXCLUDE_FROM_ALL)

add_library(slate_synth SHARED slate_synth.cpp)
target_include_directories(slate_synth PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/fluidsynth/include
    ${CMAKE_CURRENT_BINARY_DIR}/fluidsynth/include)
# Android only packages plain 'lib*.so' names: no version suffix on libfluidsynth
set_target_properties(libfluidsynth PROPERTIES VERSION "" SOVERSION "" OUTPUT_NAME fluidsynth)
target_link_libraries(slate_synth libfluidsynth log)
