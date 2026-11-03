# Builds m16cflasher on Linux with GCC.
# Flags and output paths match the Code::Blocks project m16cflasher.cbp.
#
# make                 build the release configuration
# make debug           build the debug configuration
# make CONFIG=debug    same as "make debug"
# make install         install into $(DESTDIR)$(PREFIX)/bin
# make help            list all targets and variables

CONFIG ?= release

ifeq ($(CONFIG),release)
  CONFIG_DIR := Release
  CXXFLAGS ?= -O2
  LDFLAGS ?= -s
else ifeq ($(CONFIG),debug)
  CONFIG_DIR := Debug
  CXXFLAGS ?= -g
  LDFLAGS ?=
else
  $(error CONFIG must be "release" or "debug", got "$(CONFIG)")
endif

CXX ?= g++
INSTALL ?= install
PREFIX ?= /usr/local
DESTDIR ?=

NAME := m16cflasher
BUILD_ROOT := ../build/linux
OUT_DIR := $(BUILD_ROOT)/$(CONFIG_DIR)
OBJ_DIR := $(BUILD_ROOT)/obj/$(CONFIG_DIR)
TARGET := $(OUT_DIR)/$(NAME)

SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(SRCS))
DEPS := $(OBJS:.o=.d)

# -MMD -MP writes a depfile per object so that editing a header rebuilds
# every object that includes it.
BASE_CXXFLAGS := -std=gnu++20 -Wall -fexceptions -MMD -MP

.PHONY: all release debug clean distclean install uninstall help

all: $(TARGET)

release:
	$(MAKE) CONFIG=release all

debug:
	$(MAKE) CONFIG=debug all

$(TARGET): $(OBJS) | $(OUT_DIR)
	$(CXX) $(OBJS) $(LDFLAGS) -o $@

$(OBJ_DIR)/%.o: %.cpp | $(OBJ_DIR)
	$(CXX) $(BASE_CXXFLAGS) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

$(OUT_DIR) $(OBJ_DIR):
	mkdir -p $@

clean:
	rm -f $(OBJS) $(DEPS) $(TARGET)

distclean:
	rm -rf $(BUILD_ROOT)

install: $(TARGET)
	$(INSTALL) -d $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) -m 755 $(TARGET) $(DESTDIR)$(PREFIX)/bin/$(NAME)

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(NAME)

help:
	@echo 'Targets:'
	@echo '  all        build the $(CONFIG) configuration (default)'
	@echo '  release    build with -O2 into $(BUILD_ROOT)/Release'
	@echo '  debug      build with -g into $(BUILD_ROOT)/Debug'
	@echo '  clean      remove the objects and binary of the $(CONFIG) configuration'
	@echo '  distclean  remove $(BUILD_ROOT)'
	@echo '  install    copy the binary to $$(DESTDIR)$$(PREFIX)/bin'
	@echo '  uninstall  remove the installed binary'
	@echo
	@echo 'Variables:'
	@echo '  CONFIG     release (default) or debug'
	@echo '  CXX        compiler, currently $(CXX)'
	@echo '  CXXFLAGS   added to the compile step, currently $(CXXFLAGS)'
	@echo '  CPPFLAGS   preprocessor flags, currently $(CPPFLAGS)'
	@echo '  LDFLAGS    added to the link step, currently $(LDFLAGS)'
	@echo '  PREFIX     install prefix, currently $(PREFIX)'
	@echo '  DESTDIR    staging directory for install, currently $(DESTDIR)'
	@echo
	@echo 'Current output: $(TARGET)'

-include $(DEPS)
