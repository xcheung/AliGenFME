# AliGenFME
The Fast Modeling Event (FME) generator

# Description
- Generate the Fast Modeling Events (FME) according to Pb–Pb at 2.76 TeV with ALICE (the LHC11h data)
- Events will be generated in the given centrality range defined by user
- By default, the track/cluster pT is sampled from Boltzmann with the mean pT defined by user
- the track/cluster multiplicity and eta-phi then sampled from the distribution obtained in data. Tracks are genereted in TPC acceptance, clusters are generated in EMCal acceptance
- optional: the track/cluster multiplicity can force to the user defined values
- see the example macros: AnalysisCINT.C and AnalysisACLiC.C for the details

# src
- class: AliGenFastModelingEvent.h(.cxx)
- dependency: CERN ROOT
- compile: root -b -q -l setup.C

# examples
## CINT mode
- macros: AnalysisCINT.C
- dependency: if only to generate the tracks/clusters the macros dependend only on the CERN ROOT; if one needs also the jet finder on-the-flying, the corresponding AliRoot libs has to be loaded
- exe: root -b -q -l AnalysisCINT.C # optional: with the arguments

## ACliC mode
- macros: AnalysisACLiC.C
- dependency: if only to generate the tracks/clusters the macros dependend only on the CERN ROOT; it also allows user to run the fastjet for jet finding and PYHTIA 8 to import hard particles on-the-flying
- make file: Makefile
- exe: sh runACliC.sh

# Structure
- inputs # contains the input data files
  - inputs/AnaGrid # all GRID macros used to generate the input files
  - inputs/macros # local macros used to create the input files based on the GRID outputs
- bak # the old class used by the LBNL 2015 summer students

# Environments
- the package is developed and tested on NERSC/PDSF
- for safety reason, one had better to load the following modules when compile and exe the package on PDSF

module load gcc/4.8.1
module load cmake/2.8.12

module use-apped /project/projectdirs/alice/software/modulefiles
module use-apped /project/projectdirs/alice/xmzhang/modulefiles

module load xmzhang/HepMC/2.06.09
module load xmzhang/pythia/8.2.09
module load xmzhang/jewel/2.0.2
module load xmzhang/alisoft/vAN
