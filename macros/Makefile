# ======================================================================
# NOTE: make all
# -----
#
# ======================================================================
.PHONY: prep clean vars

ROOTCINT      = $(ROOTSYS)/bin/rootcling
ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags)
ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)
ROOTLDFLAGS   = $(shell $(ROOTSYS)/bin/root-config --ldflags)

ifdef CXXCOMPILER
  CXX         := $(CXXCOMPILER)
else
  CXX         := c++
endif

CXXFLAGS      = -g -O0 -Wall -fPIC -pipe -std=c++17
LD            = $(CXX)
LDFLAGS       = -g
SOFLAGS       = -shared

CXXFLAGS     += $(ROOTCFLAGS)
GLIBS         = $(filter-out -lz, $(ROOTGLIBS)) -lTMVA -lTMVAGui -lRooFitCore -lRooFit
EXTHEADERS    = -I..


# -- Source code
STUFF = convertRootToBLTrack2.o

CLASSES = 

DICT = ${CLASSES:.o=Dict.o}
DICTHEADERS = ${CLASSES:.o=Dict.h}
DICTSOURCES = ${CLASSES:.o=Dict.cc}


# -- Default rules
$(addprefix obj/,%.o) : %.cc %.hh
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(addprefix obj/,%.o) : %.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

%Dict.cc : %.hh %LinkDef.h
	$(ROOTCINT) $@ $^
	mv $(subst Dict.cc,Dict_rdict.pcm,$@) lib

%Dict.cc : %.hh
	$(ROOTCINT) $@  $^
	mv $(subst Dict.cc,Dict_rdict.pcm,$@) lib

$(addprefix lib/,%.pcm) :
	cd lib && ln -s $< && cd -

# -- Targets
all: prep lib convertRootToBLTrack2 checkDuplicateTracks


lib: $(addprefix obj/,$(STUFF)  $(DICT))
	$(CXX) $(SOFLAGS) $(addprefix obj/,$(STUFF) $(DICT)) -o lib/libStuff.so $(GLIBS) -lMinuit


convertRootToBLTrack2:  $(addprefix obj/,$(STUFF)) obj/runConvertRootToBLTrack2.o
	$(LD) $(LDFLAGS) -o bin/convertRootToBLTrack2 obj/runConvertRootToBLTrack2.o $(GLIBS)  lib/libStuff.so

checkDuplicateTracks:  $(addprefix obj/,$(STUFF)) obj/runCheckDuplicateTracks.o
	$(LD) $(LDFLAGS) -o bin/checkDuplicateTracks obj/runCheckDuplicateTracks.o $(GLIBS)


# -- create directories if not yet existing
prep:
	mkdir -p obj bin lib

clean:
	rm -f obj/*
	rm -f bin/*
	rm -f $(DICTHEADERS)
	rm -f lib/*.pcm
	rm -f lib/libStuff.so
