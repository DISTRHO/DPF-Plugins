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

FILES_DSP = \
	DistrhoPluginGLBars.cpp

FILES_UI  = \
	DistrhoUIGLBars.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Extra flags

LINK_FLAGS += -lpthread

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS += jack
TARGETS += lv2
TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
