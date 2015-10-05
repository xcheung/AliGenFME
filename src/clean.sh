#!/bin/bash

for fn in cmake_install.cmake CMakeCache.txt Makefile CMakeFiles AliGenFME_Dict.* \
	libAliGenFME.dylib install_manifest.txt
do
	rm -rfv $fn
done
