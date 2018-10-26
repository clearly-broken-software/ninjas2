#!/usr/bin/make -f
# Makefile for static aubio #
# ------------------------- #
# Created by falkTX
#

include ../dpf/Makefile.base.mk

# --------------------------------------------------------------

BUILD_C_FLAGS += -Isrc -DHAVE_CONFIG_H

OBJS = \
	src/cvec.c.o \
	src/fmat.c.o \
	src/fvec.c.o \
	src/lvec.c.o \
	src/mathutils.c.o \
	src/vecutils.c.o \
	src/io/audio_unit.c.o \
	src/io/ioutils.c.o \
	src/io/sink.c.o \
	src/io/sink_apple_audio.c.o \
	src/io/sink_sndfile.c.o \
	src/io/sink_wavwrite.c.o \
	src/io/source.c.o \
	src/io/source_apple_audio.c.o \
	src/io/source_avcodec.c.o \
	src/io/source_sndfile.c.o \
	src/io/source_wavread.c.o \
	src/io/utils_apple_audio.c.o \
	src/notes/notes.c.o \
	src/onset/onset.c.o \
	src/onset/peakpicker.c.o \
	src/pitch/pitch.c.o \
	src/pitch/pitchfcomb.c.o \
	src/pitch/pitchmcomb.c.o \
	src/pitch/pitchschmitt.c.o \
	src/pitch/pitchspecacf.c.o \
	src/pitch/pitchyin.c.o \
	src/pitch/pitchyinfast.c.o \
	src/pitch/pitchyinfft.c.o \
	src/spectral/awhitening.c.o \
	src/spectral/fft.c.o \
	src/spectral/filterbank.c.o \
	src/spectral/filterbank_mel.c.o \
	src/spectral/mfcc.c.o \
	src/spectral/ooura_fft8g.c.o \
	src/spectral/phasevoc.c.o \
	src/spectral/specdesc.c.o \
	src/spectral/statistics.c.o \
	src/spectral/tss.c.o \
	src/synth/sampler.c.o \
	src/synth/wavetable.c.o \
	src/tempo/beattracking.c.o \
	src/tempo/tempo.c.o \
	src/temporal/a_weighting.c.o \
	src/temporal/biquad.c.o \
	src/temporal/c_weighting.c.o \
	src/temporal/filter.c.o \
	src/temporal/resampler.c.o \
	src/utils/hist.c.o \
	src/utils/log.c.o \
	src/utils/parameter.c.o \
	src/utils/scale.c.o \
	src/utils/windll.c.o

# --------------------------------------------------------------

%.c.o: %.c
	$(CC) $< $(BUILD_C_FLAGS) -MD -MP -c -o $@

# --------------------------------------------------------------

all: libaubio.a

clean:
	rm -f *.a src/*.d src/*.o src/*/*.d src/*/*.o

libaubio.a: $(OBJS)
	rm -f $@
	$(AR) crs $@ $^

# --------------------------------------------------------------

-include $(OBJS:%.o=%.d)

# --------------------------------------------------------------
