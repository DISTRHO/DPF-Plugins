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

OBJS_DSP = \
	DistrhoPluginPingPongPan.cpp.o

OBJS_UI  = \
	DistrhoArtworkPingPongPan.cpp.o \
	DistrhoUIPingPongPan.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.Mini-Series.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
endif

ifeq ($(LINUX),true)
TARGETS += ladspa
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi
endif
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
