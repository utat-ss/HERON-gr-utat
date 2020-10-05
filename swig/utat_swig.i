/* -*- c++ -*- */

#define UTAT_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "utat_swig_doc.i"

%{
#include "utat/endurosat_frame_sync_bb.h"
#include "utat/heron_rx.h"
%}


%include "utat/endurosat_frame_sync_bb.h"
GR_SWIG_BLOCK_MAGIC2(utat, endurosat_frame_sync_bb);
%include "utat/heron_rx.h"
GR_SWIG_BLOCK_MAGIC2(utat, heron_rx);
