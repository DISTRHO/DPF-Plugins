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

ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
TARGETS += lv2
TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
