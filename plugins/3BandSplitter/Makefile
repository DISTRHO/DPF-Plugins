#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = 3BandSplitter

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	DistrhoPlugin3BandSplitter.cpp

FILES_UI  = \
	DistrhoArtwork3BandSplitter.cpp \
	DistrhoUI3BandSplitter.cpp

# --------------------------------------------------------------
# Do some magic

UI_TYPE = generic
FILE_BROWSER_DISABLED = true
include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS = jack ladspa lv2_sep vst2 vst3 clap

ifeq ($(HAVE_CAIRO_OR_OPENGL),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi
endif
endif

all: $(TARGETS)

# --------------------------------------------------------------
