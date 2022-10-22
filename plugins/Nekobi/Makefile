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

all: jack dssi lv2_sep vst2 vst3 clap

# --------------------------------------------------------------
