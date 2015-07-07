#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

include Makefile.mk

all: libs plugins gen

# --------------------------------------------------------------

PREFIX  ?= /usr/local
DESTDIR ?=

# --------------------------------------------------------------

libs:
ifeq ($(HAVE_DGL),true)
	$(MAKE) -C dpf/dgl
endif

plugins: libs
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

	# ProM (needs DGL + ProjectM)
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_PROJM),true)
	$(MAKE) all -C plugins/ProM
endif
endif

gen: plugins dpf/utils/lv2_ttl_generator
	@$(CURDIR)/dpf/utils/generate-ttl.sh
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator

# --------------------------------------------------------------

clean:
ifeq ($(HAVE_DGL),true)
	$(MAKE) clean -C dpf/dgl
endif
	$(MAKE) clean -C dpf/utils/lv2-ttl-generator

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

# --------------------------------------------------------------

install:
	install -d $(DESTDIR)$(PREFIX)/lib/ladspa/
	install -d $(DESTDIR)$(PREFIX)/lib/dssi/
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/
	install -d $(DESTDIR)$(PREFIX)/lib/vst/
	cp    bin/*-ladspa.* $(DESTDIR)$(PREFIX)/lib/ladspa/
	cp -r bin/*-dssi.* bin/*-dssi $(DESTDIR)$(PREFIX)/lib/dssi/
	cp -r bin/*.lv2 modguis/*.lv2 $(DESTDIR)$(PREFIX)/lib/lv2/
	cp    bin/*-vst.* $(DESTDIR)$(PREFIX)/lib/vst/

# --------------------------------------------------------------

.PHONY: plugins
