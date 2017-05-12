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

OBJS_DSP = \
	DistrhoPluginKars.cpp.o

OBJS_UI  = \
	DistrhoArtworkKars.cpp.o \
	DistrhoUIKars.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.Kars.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(BUILD_JACK),true)
ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
endif

ifeq ($(BUILD_DSSI),true)
TARGETS += dssi_dsp
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi_ui
endif
endif
endif

ifeq ($(BUILD_LV2),true)
ifeq ($(HAVE_DGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif
endif

ifeq ($(BUILD_VST),true)
TARGETS += vst
endif

all: $(TARGETS)

# --------------------------------------------------------------
