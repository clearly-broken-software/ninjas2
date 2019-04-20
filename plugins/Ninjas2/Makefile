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
BUILD_DSSI ?= false
BUILD_LADSPA = false

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	Ninjas2Plugin.cpp

FILES_UI  = \
	Utils/src/Mathf.cpp \
	Structures/src/Animation.cpp \
	Structures/src/Layout.cpp \
	Widgets/src/WolfWidget.cpp \
	Widgets/src/NanoSwitch.cpp \
	Widgets/src/RemoveDCSwitch.cpp \
	Widgets/src/NanoButton.cpp \
	Widgets/src/NanoWheel.cpp \
	Widgets/src/NanoKnob.cpp \
	Widgets/src/VolumeKnob.cpp \
	Widgets/src/ResizeHandle.cpp \
	Widgets/src/LabelBox.cpp \
	Widgets/src/LabelContainer.cpp \
	Widgets/src/BipolarModeSwitch.cpp \
	Widgets/src/GlowingLabelsBox.cpp \
	Widgets/src/NanoLabel.cpp \
	Widgets/src/SpinBox.cpp \
	Widgets/src/NanoSpinBox.cpp \
	Widgets/src/TextButton.cpp \
	Widgets/src/PlayModeSwitch.cpp \
	Resources/fonts.cpp \
	Widgets/src/StateSwitch.cpp \
	Ninjas2UI.cpp \
	Ninjas2Resources.cpp
	

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Extra flags

BASE_FLAGS += -I. -I../../aubio/src -I./Widgets -I./Utils -I./Structures
BASE_FLAGS += $(shell pkg-config --cflags sndfile samplerate)
LINK_FLAGS += $(shell pkg-config --libs sndfile samplerate)
LINK_FLAGS += -L../../aubio/ -l:libaubio.a

# Set PARANOID_MODE=true to make the build error out on warnings
ifeq ($(PARANOID_MODE),true)
BASE_FLAGS += -Wall -Wextra -Werror
endif

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
TARGETS += lv2
else
TARGETS += lv2_dsp
endif
endif


ifeq ($(BUILD_VST2),true)
TARGETS += vst
endif

all: $(TARGETS)

# --------------------------------------------------------------
