#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#
# Modified by Clearly Broken Software
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ninjas2

# --------------------------------------------------------------
# Plugin types to build

BUILD_LV2 ?= true
BUILD_VST2 ?= true
BUILD_JACK ?= true
BUILD_DSSI ?= true
BUILD_LADSPA = false

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	Ninjas2Plugin.cpp

FILES_UI  = \
	Common/Utils/src/Mathf.cpp \
	Common/Structures/src/Graph.cpp \
	Common/Structures/src/Layout.cpp \
	Common/Structures/src/Animation.cpp \
	Common/Widgets/src/NanoSwitch.cpp \
	Common/Widgets/src/RemoveDCSwitch.cpp \
	Common/Widgets/src/NanoButton.cpp \
	Common/Widgets/src/ResetGraphButton.cpp \
	Common/Widgets/src/NanoWheel.cpp \
	Common/Widgets/src/NanoKnob.cpp \
	Common/Widgets/src/VolumeKnob.cpp \
	Common/Widgets/src/ResizeHandle.cpp \
	Common/Widgets/src/LabelBox.cpp \
	Common/Widgets/src/BipolarModeSwitch.cpp \
	Common/Widgets/src/GlowingLabelsBox.cpp \
	Common/Widgets/src/NanoLabel.cpp \
	Ninjas2UI.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Extra flags

BASE_FLAGS += -I. -I../../aubio/src -I./Widgets -I./Utils
BASE_FLAGS += $(shell pkg-config --cflags sndfile samplerate)
LINK_FLAGS += $(shell pkg-config --libs sndfile samplerate)

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(BUILD_JACK),true)
ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
endif

ifeq ($(BUILD_LADSPA),true)
TARGETS += ladspa
endif

ifeq ($(BUILD_DSSI),true)
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi
endif
endif
endif

ifeq ($(BUILD_LV2),true)
ifeq ($(HAVE_DGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif
endif


ifeq ($(BUILD_VST2),true)
TARGETS += vst
endif

all: $(TARGETS)

# --------------------------------------------------------------
