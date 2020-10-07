/* -*- c++ -*- */

#define UTAT_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "utat_swig_doc.i"

%{
#include "utat/heron_rx_bb.h"
#include "utat/heron_rx_bb.h"
%}

%include "utat/heron_rx_bb.h"
GR_SWIG_BLOCK_MAGIC2(utat, heron_rx_bb);
%include "utat/heron_rx_bb.h"
GR_SWIG_BLOCK_MAGIC2(utat, heron_rx_bb);
