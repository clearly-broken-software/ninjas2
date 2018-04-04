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
# Files to build

OBJS_DSP = \
	Ninjas2Plugin.cpp.o

OBJS_UI  = \
	Common/Utils/src/Mathf.cpp.o \
	Common/Structures/src/Graph.cpp.o \
	Common/Structures/src/Layout.cpp.o \
	Common/Structures/src/Animation.cpp.o \
	Common/Widgets/src/NanoSwitch.cpp.o \
	Common/Widgets/src/RemoveDCSwitch.cpp.o \
	Common/Widgets/src/NanoButton.cpp.o \
	Common/Widgets/src/ResetGraphButton.cpp.o \
	Common/Widgets/src/NanoWheel.cpp.o \
	Common/Widgets/src/NanoKnob.cpp.o \
	Common/Widgets/src/VolumeKnob.cpp.o \
	Common/Widgets/src/ResizeHandle.cpp.o \
	Common/Widgets/src/LabelBox.cpp.o \
	Common/Widgets/src/BipolarModeSwitch.cpp.o \
	Common/Widgets/src/GlowingLabelsBox.cpp.o \
	Common/Widgets/src/NanoLabel.cpp.o \
	Ninjas2UI.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
endif

ifeq ($(HAVE_DGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif

TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
