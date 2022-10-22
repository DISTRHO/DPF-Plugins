#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = CycleShifter

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	DistrhoPluginCycleShifter.cpp

FILES_UI  = \
	DistrhoArtworkCycleShifter.cpp \
	DistrhoUICycleShifter.cpp

# --------------------------------------------------------------
# Do some magic

UI_TYPE = generic
FILE_BROWSER_DISABLED = true
include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS += clap
TARGETS += jack
TARGETS += ladspa
TARGETS += lv2_sep
TARGETS += vst2
TARGETS += vst3

ifeq ($(BUILD_DSSI),true)
TARGETS += dssi
endif

all: $(TARGETS)

# --------------------------------------------------------------
