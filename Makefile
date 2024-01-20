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

LIBRARIES = SDL2 SDL2_image SDL2_ttf

ifeq ($(OS),Windows_NT)
	CXXFLAGS += -D_WIN32 -DGLEW_STATIC
# TODO fix this
	DEPENDENCIES =C:\Users\Nathan\Downloads\SDL2\SDL2-2.28.5\x86_64-w64-mingw32 C:\Users\Nathan\Downloads\SDL2_image-devel-2.8.2-mingw\SDL2_image-2.8.2\x86_64-w64-mingw32 C:\Users\Nathan\Downloads\SDL2_ttf-devel-2.21.2-mingw\SDL2_ttf-2.21.2\x86_64-w64-mingw32
	LIBDIR = $(addsuffix /lib,$(DEPENDENCIES))
	INCLUDEDIR = $(addsuffix /include,$(DEPENDENCIES))
	LIBRARIES = mingw32 SDL2main SDL2 SDL2_image SDL2_ttf

	INCLUDEDIR 
else
	ifeq ($(UNAME),Linux)
		CXXFLAGS += -D_LINUX
		LIBDIR += /usr/lib
		INCLUDEDIR += /usr/include
	else
		CXXFLAGS += -D_OSX
		LIBDIR += /usr/local/lib
		INCLUDEDIR += /usr/local/include
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