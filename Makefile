SRCDIR = src
STRUCTURE = $(shell cd $(SRCDIR) && find . -type d)

CXX ?= g++
CXXFLAGS ?= -g $(INCLUDES)

BINARYDIR = bin
OBJECTDIR = $(BINARYDIR)/obj

TARGET = $(BINARYDIR)/light_strip

LIBS += -lhomecontroller
LIBS += -lpthread

# src root
_OBJECTS += console.o
_HEADERS += console.h

_OBJECTS += lightstrip.o
_HEADERS += lightstrip.h

_OBJECTS += main.o

_OBJECTS += program_manager.o
_HEADERS += program_manager.h

_OBJECTS += pwm.o
_HEADERS += pwm.h

# programs
_OBJECTS += programs/program.o
_HEADERS += programs/program.h

_OBJECTS += programs/rainbow_fade_program.o
_HEADERS += programs/rainbow_fade_program.h

_OBJECTS += programs/rave_program.o
_HEADERS += programs/rave_program.h

OBJECTS = $(patsubst %,$(OBJECTDIR)/%,$(_OBJECTS))
HEADERS = $(patsubst %,$(SRCDIR)/%,$(_HEADERS))

$(OBJECTDIR)/%.o: $(SRCDIR)/%.cpp $(HEADERS) | $(OBJECTDIR)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(TARGET): $(OBJECTS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

$(OBJECTDIR):
	mkdir -p $(OBJECTDIR)
	mkdir -p $(addprefix $(OBJECTDIR)/,$(STRUCTURE))

clean:
	rm -rf bin

.PHONY: clean