
cmake_minimum_required(VERSION 3.10.0)
project(ninjas2 VERSION 0.3.0)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(dpf)

# file(READ "thirdparty/aubio/VERSION" ver)
# string(REGEX MATCH "AUBIO_MAJOR_VERSION=([0-9]*)" _ ${ver})
# set(ver_major ${CMAKE_MATCH_1})
# string(REGEX MATCH "AUBIO_MINOR_VERSION=([0-9]*)" _ ${ver})
# set(ver_minor ${CMAKE_MATCH_1})
# string(REGEX MATCH "AUBIO_PATCH_VERSION=([0-9]*)" _ ${ver})
# set(ver_patch ${CMAKE_MATCH_1})

option(HAVE_CONFIG_H "Have Config" ON)
option(HAVE_STDLIB_H "Have stdlib" ON)
option(HAVE_STDIO_H "Have stdio" ON)
option(HAVE_MATH_H "Have math.h" ON)
option(HAVE_STRING_H "Have string.h" ON)
option(HAVE_LIMITS_H "Have limits.h" ON)
option(HAVE_STDARG_H "Have stdarg.h" ON)
option(HAVE_GETOPT_H "Have getopt.h" ON)
option(HAVE_UNISTD_H "Have unistd.h" ON)
option(HAVE_C99_VARARGS_MACROS "Have c99 varargs macros" ON)
option(HAVE_WAVREAD "Have wavread" ON)
option(HAVE_WAVWRITE "Have wavwrite" ON)
option(HAVE_MEMCPY_HACKS "Have memcpy hacks" ON)
option(HAVE_ERRNO_H "Have errno.h" ON)

add_definitions(-DHAVE_CONFIG_H -fPIC)

configure_file(src/AubioConfig.h.in src/config.h)

add_library(
  aubio STATIC
  thirdparty/aubio/src/cvec.c
  thirdparty/aubio/src/fmat.c
  thirdparty/aubio/src/fvec.c
  thirdparty/aubio/src/lvec.c
  thirdparty/aubio/src/mathutils.c
  thirdparty/aubio/src/musicutils.c
  thirdparty/aubio/src/vecutils.c
  thirdparty/aubio/src/io/audio_unit.c
  thirdparty/aubio/src/io/ioutils.c
  thirdparty/aubio/src/io/sink.c
  thirdparty/aubio/src/io/sink_apple_audio.c
  thirdparty/aubio/src/io/sink_sndfile.c
  thirdparty/aubio/src/io/sink_wavwrite.c
  thirdparty/aubio/src/io/source.c
  thirdparty/aubio/src/io/source_apple_audio.c
  thirdparty/aubio/src/io/source_avcodec.c
  thirdparty/aubio/src/io/source_sndfile.c
  thirdparty/aubio/src/io/source_wavread.c
  thirdparty/aubio/src/io/utils_apple_audio.c
  thirdparty/aubio/src/notes/notes.c
  thirdparty/aubio/src/onset/onset.c
  thirdparty/aubio/src/onset/peakpicker.c
  thirdparty/aubio/src/pitch/pitch.c
  thirdparty/aubio/src/pitch/pitchfcomb.c
  thirdparty/aubio/src/pitch/pitchmcomb.c
  thirdparty/aubio/src/pitch/pitchschmitt.c
  thirdparty/aubio/src/pitch/pitchspecacf.c
  thirdparty/aubio/src/pitch/pitchyin.c
  thirdparty/aubio/src/pitch/pitchyinfast.c
  thirdparty/aubio/src/pitch/pitchyinfft.c
  thirdparty/aubio/src/spectral/awhitening.c
  thirdparty/aubio/src/spectral/dct_accelerate.c
  thirdparty/aubio/src/spectral/dct_fftw.c
  thirdparty/aubio/src/spectral/dct_ipp.c
  thirdparty/aubio/src/spectral/dct_ooura.c
  thirdparty/aubio/src/spectral/dct_plain.c
  thirdparty/aubio/src/spectral/fft.c
  thirdparty/aubio/src/spectral/filterbank.c
  thirdparty/aubio/src/spectral/filterbank_mel.c
  thirdparty/aubio/src/spectral/mfcc.c
  thirdparty/aubio/src/spectral/ooura_fft8g.c
  thirdparty/aubio/src/spectral/phasevoc.c
  thirdparty/aubio/src/spectral/specdesc.c
  thirdparty/aubio/src/spectral/statistics.c
  thirdparty/aubio/src/spectral/tss.c
  thirdparty/aubio/src/synth/sampler.c
  thirdparty/aubio/src/synth/wavetable.c
  thirdparty/aubio/src/tempo/beattracking.c
  thirdparty/aubio/src/tempo/tempo.c
  thirdparty/aubio/src/temporal/a_weighting.c
  thirdparty/aubio/src/temporal/biquad.c
  thirdparty/aubio/src/temporal/c_weighting.c
  thirdparty/aubio/src/temporal/filter.c
  thirdparty/aubio/src/temporal/resampler.c
  thirdparty/aubio/src/utils/hist.c
  thirdparty/aubio/src/utils/log.c
  thirdparty/aubio/src/utils/parameter.c
  thirdparty/aubio/src/utils/scale.c
  thirdparty/aubio/src/utils/windll.c)

target_include_directories(
  aubio PUBLIC "${PROJECT_BINARY_DIR}/src"
               "${PROJECT_SOURCE_DIR}/thirdparty/aubio/src")

dpf_add_plugin(
  ninjas2 TARGETS jack lv2 vst2
  
  FILES_DSP
  src/Ninjas2Plugin.cpp
  
  FILES_UI
  src/Ninjas2UI.cpp
  src/EnvelopeGenerator.cpp
  src/fonts.cpp
  src/Ninjas2Resources.cpp
  src/widgets/src/Knob.cpp
  src/widgets/src/NanoLabel.cpp
  src/widgets/src/PianoKeyboard.cpp
  src/widgets/src/PlayModeSwitch.cpp
  src/widgets/src/SliceModeSwitch.cpp
  src/widgets/src/Spinner.cpp
  src/widgets/src/TextButton.cpp
  thirdparty/widgets/src/ExtraEventHandlers.cpp
  thirdparty/widgets/src/NanoButton.cpp
  thirdparty/widgets/src/NanoKnob.cpp
  thirdparty/widgets/src/NanoSlider.cpp
  thirdparty/widgets/src/NanoSpinner.cpp
  thirdparty/widgets/src/NanoSwitch.cpp
)

target_include_directories(
  ninjas2 PUBLIC 
  "src"
  "src/widgets"
  "thirdparty/widgets"
  "thirdparty/aubio/src"
)

target_link_libraries(ninjas2 PUBLIC aubio -lsndfile -lsamplerate)


