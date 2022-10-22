#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = PingPongPan

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	DistrhoPluginPingPongPan.cpp

FILES_UI  = \
	DistrhoArtworkPingPongPan.cpp \
	DistrhoUIPingPongPan.cpp

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
