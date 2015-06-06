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

OBJS_DSP = \
	DistrhoPlugin3BandSplitter.cpp.o

OBJS_UI  = \
	DistrhoArtwork3BandSplitter.cpp.o \
	DistrhoUI3BandSplitter.cpp.o

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
