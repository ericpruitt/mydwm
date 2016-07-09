# This file is copied into the dwm repository as "config.mk" when compiling. It
# was renamed so "config.h" could be autocompleted after typing a single
# character.
VERSION = git-$(shell git log -1 --format=%h)
DEPENDENCIES = fontconfig x11 xft

# Xinerama is only needed to support multiple monitors, so it's an optional
# dependency since I generally only use one monitor.
XINERAMALIBS = $(shell pkg-config --silence-errors --libs xinerama)
XINERAMAFLAGS = $(if $(XINERAMALIBS),-DXINERAMA,)

# Includes and libraries
INCS = $(shell pkg-config --cflags $(DEPENDENCIES))
LIBS = $(shell pkg-config --libs $(DEPENDENCIES)) $(XINERAMALIBS) -lrt

# General compiler and linker flags
CPPFLAGS = -D_DEFAULT_SOURCE -D_BSD_SOURCE -D_POSIX_C_SOURCE=199309L -DVERSION=\"$(VERSION)\" $(XINERAMAFLAGS)
CFLAGS = -std=c99 -pedantic -Wall -Wno-deprecated-declarations -Os $(INCS) $(CPPFLAGS)
LDFLAGS = $(LIBS)

CC = cc
