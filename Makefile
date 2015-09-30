# Linking pythia to fastjet
# Adapted form Vincia1024 makefile and Dr. Soyez's tutorial

# test OS - Ubuntu 11.04 64bit;
# Mushfiq, University of Dhaka , date : 06.11.2011 .
# XMZHANG, add ROOT application, date : 25.01.2013.

SHELL = /bin/sh
TOPDIR=$(shell \pwd)

PYTHIA8INC := `pythia8-config --cxxflags`
PYTHIA8LIB := -L$(shell pythia8-config --libdir) -lpythia8

LHAPDFARGS := `lhapdf-config  --cxxflags --ldflags`

FASTJETINC :=`fastjet-config --cxxflags --plugins`
FASTJETLIB :=`fastjet-config --libs --plugins`

ROOTINC := $(shell root-config --cflags)
ROOTLIB := $(shell root-config --ldflags --glibs) -lEG

HEPMCINC := -I$(HepMCdir)/include
HEPMCLIB := -L$(HepMCdir)/lib -lHepMC -lHepMCfio

ALIGENINC := -I$(TOPDIR)
ALIGENLIB := -L$(TOPDIR)

all:
	$(CXX) -o AnalysisACLiC AnalysisACLiC.C \
	$(PYTHIA8INC) $(PYTHIA8LIB) \
	$(LHAPDFARGS) \
	$(FASTJETINC)  $(FASTJETLIB) \
	$(ROOTINC)     $(ROOTLIB) \
	$(ALIROOTPATH) $(ALIROOTLIBS) \
	$(ALIGENINC) AliGenFastModelingEvent_cxx.so
