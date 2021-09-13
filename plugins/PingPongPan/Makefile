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
include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS += jack
TARGETS += ladspa
TARGETS += vst2

ifeq ($(HAVE_CAIRO_OR_OPENGL),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi
endif
endif

ifeq ($(HAVE_CAIRO_OR_OPENGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif

all: $(TARGETS)

# --------------------------------------------------------------
