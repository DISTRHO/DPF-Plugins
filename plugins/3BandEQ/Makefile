#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = 3BandEQ

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	DistrhoPlugin3BandEQ.cpp

FILES_UI  = \
	DistrhoArtwork3BandEQ.cpp \
	DistrhoUI3BandEQ.cpp

# --------------------------------------------------------------
# Do some magic

UI_TYPE = generic
SKIP_NATIVE_AUDIO_FALLBACK = true
USE_FILE_BROWSER = false
include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS = au clap jack ladspa lv2_sep vst2 vst3

ifeq ($(HAVE_CAIRO_OR_OPENGL)$(HAVE_LIBLO),truetrue)
TARGETS += dssi
endif

all: $(TARGETS)

# --------------------------------------------------------------
