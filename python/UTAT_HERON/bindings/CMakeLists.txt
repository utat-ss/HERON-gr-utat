# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT UTAT_HERON_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT UTAT_HERON_sources)

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
list(APPEND UTAT_HERON_python_files
    heron_rx_bb_python.cc
    header_format_esttc_python.cc
    esttc_framer_python.cc
    esttc_deframer_python.cc
    tagged_stream_tail_tagger_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(UTAT_HERON
   ../../..
   gr::UTAT_HERON
   "${UTAT_HERON_python_files}")

# copy bindings extension for use in QA test module
add_custom_command(TARGET UTAT_HERON_python POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:UTAT_HERON_python>
    ${PROJECT_BINARY_DIR}/test_modules/gnuradio/UTAT_HERON/
)

install(TARGETS UTAT_HERON_python DESTINATION ${GR_PYTHON_DIR}/gnuradio/UTAT_HERON COMPONENT pythonapi)
