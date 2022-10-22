#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = Kars

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	DistrhoPluginKars.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS = jack lv2_dsp vst2 vst3 clap

ifeq ($(BUILD_DSSI),true)
TARGETS += dssi_dsp
endif

all: $(TARGETS)

# --------------------------------------------------------------
