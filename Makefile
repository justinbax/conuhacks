CXX = g++
CXXFLAGS = -Wall

SRCDIR = src
BINDIR = bin
INCLUDEDIR = include
# BASELIBDIR is used to avoid self-references by appending $(LIBDIR)/{platform} to itself
BASELIBDIR = lib
LIBDIR = $(BASELIBDIR)

# All source and header files found within src/ with corresponding object files for .c files
SRCFILES = $(wildcard $(SRCDIR)/*.cpp)
OBJFILES = $(SRCFILES:$(SRCDIR)/%.c=$(BINDIR)/%.o)
EXECUTABLE = $(BINDIR)/main
HEADFILES = $(wildcard $(SRCDIR)/*.h)
HEADFILES += $(wildcard $(SRCDIR)/*.hpp)
HEADFILES += $(wildcard $(INCLUDEDIR)/*.h)
HEADFILES += $(wildcard $(INCLUDEDIR)/*.hpp)

LIBRARIES = SDL2

ifeq ($(OS),Windows_NT)
	CXXFLAGS += -D_WIN32 -DGLEW_STATIC
	LIBDIR += $(BASELIBDIR)/win32
	LIBRARIES += gdi32 winmm ole32 setupapi
else
	UNAME = $(shell uname -s)
	ifeq ($(UNAME),Linux)
		CXXFLAGS += -D_LINUX
		LIBDIR += /usr/lib
		INCLUDEDIR += /usr/include
	endif
endif

all: $(EXECUTABLE)

# Appends appropriate debug information and optimization flags for debug and release modes
debug: CXXFLAGS += -O0 -g
debug: $(EXECUTABLE)

release: CXXFLAGS += -O2
release: $(EXECUTABLE)

$(EXECUTABLE): $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o $(EXECUTABLE) $(OBJFILES) $(addprefix -L,$(LIBDIR)) $(addprefix -l,$(LIBRARIES))

$(BINDIR)/%.o: $(SRCDIR)/%.c $(HEADFILES)
	$(CXX) $(CXXFLAGS) -I$(INCLUDEDIR) -c -o $@ $<

.phony: all debug release