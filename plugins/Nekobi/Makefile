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

FILES_UI = \
	DistrhoArtworkNekobi.cpp \
	DistrhoUINekobi.cpp

# --------------------------------------------------------------
# Do some magic

UI_TYPE = generic
FILE_BROWSER_DISABLED = true
include ../../dpf/Makefile.plugins.mk

# --------------------------------------------------------------
# Extra flags

BASE_FLAGS += -pthread
LINK_FLAGS += -pthread

# --------------------------------------------------------------
# Enable all possible plugin types

TARGETS = jack lv2_sep vst2 vst3 clap

ifeq ($(BUILD_DSSI),true)
TARGETS += dssi
endif

all: $(TARGETS)

# --------------------------------------------------------------
