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

FILES_DSP = \
	DistrhoPluginProM.cpp

FILES_UI  = \
	DistrhoUIProM.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Extra flags

BASE_FLAGS += $(shell pkg-config --cflags libprojectM)
LINK_FLAGS += $(shell pkg-config --libs libprojectM) -lpthread

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS += jack
TARGETS += lv2
TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
