#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = MVerb

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	DistrhoPluginMVerb.cpp

FILES_UI  = \
	DistrhoArtworkMVerb.cpp \
	DistrhoUIMVerb.cpp

# --------------------------------------------------------------
# Do some magic

FILE_BROWSER_DISABLED = true
include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS = jack ladspa lv2_sep vst2 vst3 clap

ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi
endif
endif

all: $(TARGETS)

# --------------------------------------------------------------
