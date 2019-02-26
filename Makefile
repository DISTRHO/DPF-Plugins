#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

include dpf/Makefile.base.mk

all: dgl plugins gen

# --------------------------------------------------------------

PREFIX  ?= /usr/local
DESTDIR ?=

# --------------------------------------------------------------

HAVE_PROJM = $(shell pkg-config --exists libprojectM && echo true)

# --------------------------------------------------------------

dgl:
ifeq ($(HAVE_DGL),true)
	$(MAKE) -C dpf/dgl
endif

plugins: dgl
	# glBars (needs OpenGL)
ifeq ($(HAVE_DGL),true)
	$(MAKE) all -C plugins/glBars
endif

	# Kars
	$(MAKE) all -C plugins/Kars

	# Mini-Series
	$(MAKE) all -C plugins/3BandEQ
	$(MAKE) all -C plugins/3BandSplitter
	$(MAKE) all -C plugins/PingPongPan

	# ndc-Plugs
	$(MAKE) all -C plugins/AmplitudeImposer
	$(MAKE) all -C plugins/CycleShifter
	$(MAKE) all -C plugins/SoulForce

	# MVerb
	$(MAKE) all -C plugins/MVerb

	# Nekobi
	$(MAKE) all -C plugins/Nekobi

	# ProM (needs OpenGL + ProjectM)
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_PROJM),true)
	$(MAKE) all -C plugins/ProM
endif
endif

	# Max-Gen stuff
	$(MAKE) all -C plugins/bitcrush
	$(MAKE) all -C plugins/freeverb
	$(MAKE) all -C plugins/gigaverb
	$(MAKE) all -C plugins/pitchshift

ifneq ($(CROSS_COMPILING),true)
gen: plugins dpf/utils/lv2_ttl_generator
	@$(CURDIR)/dpf/utils/generate-ttl.sh
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator
else
gen:
endif

# --------------------------------------------------------------

clean:
	$(MAKE) clean -C dpf/dgl
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator

	# glBars
	$(MAKE) clean -C plugins/glBars

	# Kars
	$(MAKE) clean -C plugins/Kars

	# Mini-Series
	$(MAKE) clean -C plugins/3BandEQ
	$(MAKE) clean -C plugins/3BandSplitter
	$(MAKE) clean -C plugins/PingPongPan

	# ndc-Plugs
	$(MAKE) clean -C plugins/AmplitudeImposer
	$(MAKE) clean -C plugins/CycleShifter
	$(MAKE) clean -C plugins/SoulForce

	# MVerb
	$(MAKE) clean -C plugins/MVerb

	# Nekobi
	$(MAKE) clean -C plugins/Nekobi

	# ProM
	$(MAKE) clean -C plugins/ProM

	# Max-Gen stuff
	$(MAKE) clean -C plugins/bitcrush
	$(MAKE) clean -C plugins/freeverb
	$(MAKE) clean -C plugins/gigaverb
	$(MAKE) clean -C plugins/pitchshift

# --------------------------------------------------------------

install:
	install -d $(DESTDIR)$(PREFIX)/lib/ladspa/
	install -d $(DESTDIR)$(PREFIX)/lib/dssi/
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/
	install -d $(DESTDIR)$(PREFIX)/lib/vst/
	install -d $(DESTDIR)$(PREFIX)/bin/

	cp bin/*-ladspa.* $(DESTDIR)$(PREFIX)/lib/ladspa/
	cp bin/*-dssi.*   $(DESTDIR)$(PREFIX)/lib/dssi/
	cp bin/*-vst.*    $(DESTDIR)$(PREFIX)/lib/vst/

ifeq ($(HAVE_DGL),true)
	cp -r bin/*-dssi  $(DESTDIR)$(PREFIX)/lib/dssi/
endif
	cp -r bin/*.lv2   $(DESTDIR)$(PREFIX)/lib/lv2/

ifeq ($(HAVE_JACK),true)
	cp -r bin/3BandEQ          $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/3BandSplitter    $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/AmplitudeImposer $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/CycleShifter     $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/Kars             $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/MVerb            $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/MaBitcrush       $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/MaFreeverb       $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/MaGigaverb       $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/MaPitchshift     $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/Nekobi           $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/PingPongPan      $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/ProM             $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/SoulForce        $(DESTDIR)$(PREFIX)/bin/
	cp -r bin/glBars           $(DESTDIR)$(PREFIX)/bin/
endif

# --------------------------------------------------------------

.PHONY: plugins
