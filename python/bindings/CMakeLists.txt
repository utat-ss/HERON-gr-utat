# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT utat3_9_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT utat3_9_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND utat3_9_python_files
    heron_rx_bb_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(utat3_9 
   ../..
   gr::utat3_9
   "${utat3_9_python_files}")

install(TARGETS utat3_9_python DESTINATION ${GR_PYTHON_DIR}/utat3_9 COMPONENT pythonapi)
