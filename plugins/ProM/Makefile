#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = ProM

# --------------------------------------------------------------
# Files to build

OBJS_DSP = \
	DistrhoPluginProM.cpp.o

OBJS_UI  = \
	DistrhoUIProM.cpp.o

# --------------------------------------------------------------
# Do some magic

include ../Makefile.ProM.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif
TARGETS += lv2
TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
