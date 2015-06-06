#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = SoulForce

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	DistrhoPluginSoulForce.cpp.o

OBJS_UI  = \
	DistrhoArtworkSoulForce.cpp.o \
	DistrhoUISoulForce.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.ndc-Plugs.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_JACK),true)
TARGETS += jack
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
