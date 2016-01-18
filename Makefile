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

	# ProM (needs DGL + ProjectM)
ifeq ($(HAVE_DGL),true)
ifeq ($(HAVE_PROJM),true)
	$(MAKE) all -C plugins/ProM
endif
endif

	# Max-Gen stuff
	$(MAKE) all -C plugins/bitcrush
	$(MAKE) all -C plugins/freeverb
	$(MAKE) all -C plugins/gigaverb
	$(MAKE) all -C plugins/larynx
	$(MAKE) all -C plugins/modulay
	$(MAKE) all -C plugins/pitchotto
	$(MAKE) all -C plugins/pitchshift
	$(MAKE) all -C plugins/shiroverb

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
	$(MAKE) clean -C plugins/larynx
	$(MAKE) clean -C plugins/modulay
	$(MAKE) clean -C plugins/pitchotto
	$(MAKE) clean -C plugins/pitchshift
	$(MAKE) clean -C plugins/shiroverb

# --------------------------------------------------------------

install:
	install -d $(DESTDIR)$(PREFIX)/lib/ladspa/
	install -d $(DESTDIR)$(PREFIX)/lib/dssi/
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/
	install -d $(DESTDIR)$(PREFIX)/lib/vst/

	cp bin/*-ladspa.* $(DESTDIR)$(PREFIX)/lib/ladspa/
	cp bin/*-dssi.*   $(DESTDIR)$(PREFIX)/lib/dssi/
	cp bin/*-vst.*    $(DESTDIR)$(PREFIX)/lib/vst/

ifeq ($(HAVE_DGL),true)
	cp -r bin/*-dssi  $(DESTDIR)$(PREFIX)/lib/dssi/
endif
	cp -r bin/*.lv2   $(DESTDIR)$(PREFIX)/lib/lv2/

	# Kars
	cp -r modguis/Kars.modgui/modgui       $(DESTDIR)$(PREFIX)/lib/lv2/Kars.lv2/
	cp    modguis/Kars.modgui/manifest.ttl $(DESTDIR)$(PREFIX)/lib/lv2/Kars.lv2/modgui.ttl

	# Mini-Series
	cp -r modguis/PingPongPan.modgui/modgui       $(DESTDIR)$(PREFIX)/lib/lv2/PingPongPan.lv2/
	cp    modguis/PingPongPan.modgui/manifest.ttl $(DESTDIR)$(PREFIX)/lib/lv2/PingPongPan.lv2/modgui.ttl

	# MVerb
	cp -r modguis/MVerb.modgui/modgui       $(DESTDIR)$(PREFIX)/lib/lv2/MVerb.lv2/
	cp    modguis/MVerb.modgui/manifest.ttl $(DESTDIR)$(PREFIX)/lib/lv2/MVerb.lv2/modgui.ttl

	# Max-Gen stuff
# 	cp -r modguis/Larynx.modgui/modgui       $(DESTDIR)$(PREFIX)/lib/lv2/Larynx.lv2/
# 	cp    modguis/Larynx.modgui/manifest.ttl $(DESTDIR)$(PREFIX)/lib/lv2/Larynx.lv2/modgui.ttl

	cp -r modguis/Modulay.modgui/modgui       $(DESTDIR)$(PREFIX)/lib/lv2/Modulay.lv2/
	cp    modguis/Modulay.modgui/manifest.ttl $(DESTDIR)$(PREFIX)/lib/lv2/Modulay.lv2/modgui.ttl

	cp -r modguis/Pitchotto.modgui/modgui       $(DESTDIR)$(PREFIX)/lib/lv2/Pitchotto.lv2/
	cp    modguis/Pitchotto.modgui/manifest.ttl $(DESTDIR)$(PREFIX)/lib/lv2/Pitchotto.lv2/modgui.ttl

	cp -r modguis/Shiroverb.modgui/modgui       $(DESTDIR)$(PREFIX)/lib/lv2/Shiroverb.lv2/
	cp    modguis/Shiroverb.modgui/manifest.ttl $(DESTDIR)$(PREFIX)/lib/lv2/Shiroverb.lv2/modgui.ttl

# --------------------------------------------------------------

.PHONY: plugins
