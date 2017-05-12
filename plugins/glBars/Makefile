#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = glBars

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	DistrhoPluginGLBars.cpp.o

OBJS_UI  = \
	DistrhoUIGLBars.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.glBars.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(BUILD_JACK),true)
ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
endif

ifeq ($(BUILD_LV2),true)
TARGETS += lv2
endif

ifeq ($(BUILD_VST),true)
TARGETS += vst
endif

all: $(TARGETS)

# --------------------------------------------------------------
