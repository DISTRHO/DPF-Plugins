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
ifeq ($(HAVE_CAIRO_OR_OPENGL),true)
	$(MAKE) -C dpf/dgl
endif

plugins: dgl
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

	# Max-Gen stuff
	$(MAKE) all -C plugins/bitcrush
	$(MAKE) all -C plugins/freeverb
	$(MAKE) all -C plugins/gigaverb
	$(MAKE) all -C plugins/pitchshift

ifeq ($(HAVE_CAIRO_OR_OPENGL),true)
	# glBars (needs OpenGL)
	$(MAKE) all -C plugins/glBars

ifeq ($(HAVE_PROJM),true)
	# ProM (needs OpenGL + ProjectM)
	$(MAKE) all -C plugins/ProM
endif # HAVE_PROJM
endif # HAVE_CAIRO_OR_OPENGL

gen: plugins dpf/utils/lv2_ttl_generator
ifneq ($(CROSS_COMPILING),true)
	@$(CURDIR)/dpf/utils/generate-ttl.sh
endif
ifeq ($(MACOS),true)
	@$(CURDIR)/dpf/utils/generate-vst-bundles.sh
endif

dpf/utils/lv2_ttl_generator:
	$(MAKE) -C dpf/utils/lv2-ttl-generator

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

	install -m 644 bin/*-ladspa.* $(DESTDIR)$(PREFIX)/lib/ladspa/
	install -m 644 bin/*-dssi.*   $(DESTDIR)$(PREFIX)/lib/dssi/
ifeq ($(MACOS),true)
	cp -r bin/*.vst               $(DESTDIR)$(PREFIX)/lib/vst/
else
	install -m 644 bin/*-vst.*    $(DESTDIR)$(PREFIX)/lib/vst/
endif

ifeq ($(HAVE_CAIRO_OR_OPENGL),true)
ifneq ($(MACOS),true)
	cp -r bin/*-dssi  $(DESTDIR)$(PREFIX)/lib/dssi/
endif # MACOS
endif # HAVE_CAIRO_OR_OPENGL
	cp -r bin/*.lv2   $(DESTDIR)$(PREFIX)/lib/lv2/

ifeq ($(HAVE_JACK),true)
	install -m 755 bin/Kars$(APP_EXT)             $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/3BandEQ$(APP_EXT)          $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/3BandSplitter$(APP_EXT)    $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/PingPongPan$(APP_EXT)      $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/AmplitudeImposer$(APP_EXT) $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/CycleShifter$(APP_EXT)     $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/SoulForce$(APP_EXT)        $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/MVerb$(APP_EXT)            $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/Nekobi$(APP_EXT)           $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/MaBitcrush$(APP_EXT)       $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/MaFreeverb$(APP_EXT)       $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/MaGigaverb$(APP_EXT)       $(DESTDIR)$(PREFIX)/bin/
	install -m 755 bin/MaPitchshift$(APP_EXT)     $(DESTDIR)$(PREFIX)/bin/
ifeq ($(HAVE_CAIRO_OR_OPENGL),true)
	install -m 755 bin/glBars$(APP_EXT)           $(DESTDIR)$(PREFIX)/bin/
ifeq ($(HAVE_PROJM),true)
	install -m 755 bin/ProM$(APP_EXT)             $(DESTDIR)$(PREFIX)/bin/
endif # HAVE_PROJM
endif # HAVE_CAIRO_OR_OPENGL
endif # HAVE_JACK

# --------------------------------------------------------------

.PHONY: plugins
