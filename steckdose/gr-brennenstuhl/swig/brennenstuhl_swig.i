/* -*- c++ -*- */

#define BRENNENSTUHL_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "brennenstuhl_swig_doc.i"

%{
#include "brennenstuhl/demodulator.h"
#include "brennenstuhl/modulator.h"
%}

%include "brennenstuhl/demodulator.h"
GR_SWIG_BLOCK_MAGIC2(brennenstuhl, demodulator);
%include "brennenstuhl/modulator.h"
GR_SWIG_BLOCK_MAGIC2(brennenstuhl, modulator);
