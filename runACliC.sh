#!/bin/bash
#
root -b -q -l setup.C
export LD_LIBRARY_PATH=${PWD}:${LD_LIBRARY_PATH}
#
make
./AnalysisACLiC
#
exit 0
