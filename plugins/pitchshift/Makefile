#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = MaPitchshift

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	DistrhoPluginMaxGen.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.DPF-Max-Gen.mk

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

ifeq ($(BUILD_LV2),true)
TARGETS += lv2_dsp
endif

ifeq ($(BUILD_VST),true)
TARGETS += vst
endif

all: $(TARGETS)

# --------------------------------------------------------------
