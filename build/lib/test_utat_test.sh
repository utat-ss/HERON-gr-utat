#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/ryan/Dev/comms/gr-utat/lib
export PATH=/home/ryan/Dev/comms/gr-utat/build/lib:$PATH
export LD_LIBRARY_PATH=/home/ryan/Dev/comms/gr-utat/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-utat 
