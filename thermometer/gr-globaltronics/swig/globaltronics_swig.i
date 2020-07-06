/* -*- c++ -*- */

#define GLOBALTRONICS_API

%include "gnuradio.i"           // the common stuff

//load generated python docstrings
%include "globaltronics_swig_doc.i"

%{
#include "globaltronics/demodulator.h"
#include "globaltronics/modulator.h"
%}

%include "globaltronics/demodulator.h"
GR_SWIG_BLOCK_MAGIC2(globaltronics, demodulator);
%include "globaltronics/modulator.h"
GR_SWIG_BLOCK_MAGIC2(globaltronics, modulator);
