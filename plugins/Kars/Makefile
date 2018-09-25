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

FILES_DSP = \
	DistrhoPluginKars.cpp

# --------------------------------------------------------------
# Do some magic

include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Enable all possible plugin types

ifeq ($(HAVE_JACK),true)
TARGETS += jack
endif

TARGETS += dssi_dsp
TARGETS += lv2_sep
TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
