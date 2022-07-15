SRCDIR=src

IDIRS = -I./libhomecontroller/include

LDIR = lib

CXX=g++
CXXFLAGS=-g $(IDIRS) -L$(LDIR) -Wl,-rpath,./$(LDIR)

TARGET=bin/light_strip
ODIR=bin/obj

LIBS=-lhomecontroller -pthread -lssl -lllhttp

_DEPS =	lightstrip.h console.h pwm.h program_manager.h programs/program.h programs/rainbow_fade_program.h programs/rave_program.h
DEPS = $(patsubst %,$(SRCDIR)/%,$(_DEPS))

_OBJ = main.o lightstrip.o console.o pwm.o program_manager.o programs/program.o programs/rainbow_fade_program.o programs/rave_program.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SRCDIR)/%.cpp $(DEPS) | bin
	$(CXX) -c -o $@ $< $(CXXFLAGS)

$(TARGET): libhomecontroller/bin/libhomecontroller.so $(OBJ)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

bin:
	mkdir $@
	mkdir $@/obj
	mkdir $@/obj/programs

libhomecontroller/bin/libhomecontroller.so:
	cd libhomecontroller && make
	mkdir lib
	cd lib && ln -s ../libhomecontroller/bin/libhomecontroller.so libhomecontroller.so

.PHONY: clean

clean:
	find $(ODIR) -type f -name '*.o' -delete
	rm -f $(TARGET)

cleanall:
	rm -r bin
	rm -r lib
	cd libhomecontroller && make cleanall