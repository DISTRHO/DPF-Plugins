#!/usr/bin/make -f
# Makefile for DISTRHO Plugins #
# ---------------------------- #
# Created by falkTX
#

# NAME, OBJS_DSP and OBJS_UI have been defined before

include ../../Makefile.mk

# --------------------------------------------------------------
# Basic setup

TARGET_DIR = ../../bin

BUILD_C_FLAGS   += -I.
BUILD_CXX_FLAGS += -I. -I../../dpf/distrho -I../../dpf/dgl

BASE_FLAGS += -DHAVE_DGL

ifeq ($(HAVE_JACK),true)
BASE_FLAGS += -DHAVE_JACK
endif

# --------------------------------------------------------------
# Set plugin binary file targets

jack = $(TARGET_DIR)/$(NAME)$(APP_EXT)
lv2  = $(TARGET_DIR)/$(NAME).lv2/$(NAME)$(LIB_EXT)
vst  = $(TARGET_DIR)/$(NAME)-vst$(LIB_EXT)

# --------------------------------------------------------------
# Set distrho code files

DISTRHO_PLUGIN_FILES = ../../dpf/distrho/DistrhoPluginMain.cpp
DISTRHO_UI_FILES     = ../../dpf/distrho/DistrhoUIMain.cpp ../../dpf/libdgl.a

# --------------------------------------------------------------
# all needs to be first

all:

# --------------------------------------------------------------
# Common

%.c.o: %.c
	$(CC) $< $(BUILD_C_FLAGS) -MD -MP -c -o $@

%.cpp.o: %.cpp
	$(CXX) $< $(BUILD_CXX_FLAGS) -MD -MP -c -o $@

clean:
	rm -f *.d *.o
	rm -rf $(TARGET_DIR)/$(NAME) $(TARGET_DIR)/$(NAME)-* $(TARGET_DIR)/$(NAME).lv2/

# --------------------------------------------------------------
# JACK

jack: $(jack)

$(jack): $(OBJS_DSP) $(OBJS_UI) $(DISTRHO_PLUGIN_FILES) $(DISTRHO_UI_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(DGL_LIBS) $(shell pkg-config --cflags --libs jack libprojectM) -lpthread -DDISTRHO_PLUGIN_TARGET_JACK -o $@

# --------------------------------------------------------------
# LV2

lv2: $(lv2)

$(lv2): $(OBJS_DSP) $(OBJS_UI) $(DISTRHO_PLUGIN_FILES) $(DISTRHO_UI_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(DGL_LIBS) $(shell pkg-config --cflags --libs libprojectM) -lpthread $(SHARED) -DDISTRHO_PLUGIN_TARGET_LV2 -o $@

# --------------------------------------------------------------
# VST

vst: $(vst)

$(vst): $(OBJS_DSP) $(OBJS_UI) $(DISTRHO_PLUGIN_FILES) $(DISTRHO_UI_FILES)
	mkdir -p $(shell dirname $@)
	$(CXX) $^ $(BUILD_CXX_FLAGS) $(LINK_FLAGS) $(DGL_LIBS) $(shell pkg-config --cflags --libs libprojectM) -lpthread $(SHARED) -DDISTRHO_PLUGIN_TARGET_VST -o $@

# --------------------------------------------------------------

-include $(OBJS_DSP:%.o=%.d)
-include $(OBJS_UI:%.o=%.d)

# --------------------------------------------------------------
