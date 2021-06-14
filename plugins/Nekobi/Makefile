#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# --------------------------------------------------------------
# Project name, used for binaries

NAME = Nekobi

# --------------------------------------------------------------
# Files to build

FILES_DSP = \
	DistrhoPluginNekobi.cpp

FILES_UI  = \
	DistrhoArtworkNekobi.cpp \
	DistrhoUINekobi.cpp

# --------------------------------------------------------------
# Do some magic

UI_TYPE = generic
include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Extra flags

LINK_FLAGS += -lpthread

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS += jack
TARGETS += dssi_dsp

ifeq ($(HAVE_CAIRO_OR_OPENGL),true)
ifeq ($(HAVE_LIBLO),true)
TARGETS += dssi_ui
endif
endif

ifeq ($(HAVE_CAIRO_OR_OPENGL),true)
TARGETS += lv2_sep
else
TARGETS += lv2_dsp
endif

TARGETS += vst

all: $(TARGETS)

# --------------------------------------------------------------
