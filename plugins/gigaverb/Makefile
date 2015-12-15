#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = MaGigaverb

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	DistrhoPluginMaxGen.cpp.o \
	gen_exported.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.DPF-Max-Gen.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif

ifeq ($(LINUX),true)
TARGETS += ladspa
endif

TARGETS += lv2_dsp
TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
