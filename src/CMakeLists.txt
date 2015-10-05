# Fast simulation tool for the DETECTOR response based on ALICE data
# Detector system
# Copyright (C) 2015 Xiaoming Zhang ALICE Collaboration
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
project(AliGenFME CXX)
cmake_minimum_required(VERSION 2.8.11)

# indep. if root built with cmake assume the root-cint is in the path
execute_process ( COMMAND root-config --prefix WORKING_DIRECTORY /tmp OUTPUT_VARIABLE ROOT_DIR OUTPUT_STRIP_TRAILING_WHITESPACE )
message ("[info] ROOT_DIR: ${ROOT_DIR}")
set( CMAKE_MODULE_PATH "${ROOT_DIR}/etc/root/cmake" )
message ("[info] CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")
find_package(ROOT REQUIRED)

set(CMAKE_MACOSX_RPATH 1)

set(SRCS
    AliGenFastModelingEvent.cxx
)

# Headers from sources
string(REPLACE ".cxx" ".h" HDRS "${SRCS}")

# generate dictionaries
set(DICT ${PROJECT_NAME}_Dict )
list(APPEND DICTINC ${HDRS})
set(LINKDEFS AliGenFastModelingEventLinkDefs.h )
# if root not build with cmake this does not work
#ROOT_GENERATE_DICTIONARY(${DICT} ${DICTINC} LINKDEF ${LINKDEFS})
# workaround
ADD_CUSTOM_COMMAND(
    OUTPUT ${DICT}.cxx
    COMMAND ${ROOTCINT_EXECUTABLE} -f ${DICT}.cxx -c ${HDRS} ${LINKDEFS}
    DEPENDS ${HDRS} )
message ("[info] ${DICT} ${ROOTCINT_EXECUTABLE}")

include_directories(${ROOT_INCLUDE_DIR})
link_libraries(${ROOT_LIBRARIES})
add_library(AliGenFME SHARED ${SRCS} ${HDRS} ${DICT}.cxx)

install(TARGETS AliGenFME
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib)
install(FILES ${HDRS} DESTINATION include)