/* -*- c++ -*- */

#define KCSA_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "kcsa_swig_doc.i"

%{
#include "kcsa/kiss_server.h"
#include "kcsa/ccsds_tc.h"
#include "kcsa/ccsds_udp_tc.h"
#include "kcsa/fec_decode_b.h"
#include "kcsa/fec_encode_b.h"
#include "kcsa/vitfilt27_bb.h"
#include "kcsa/vitfilt27_fb.h"
#include "kcsa/encode27_bb.h"
#include "kcsa/clock_recovery_gardner_ff.h"
#include "kcsa/clock_recovery_gardner_cc.h"
#include "kcsa/halfduplex_tc.h"
#include "kcsa/pdu_to_stream.h"
#include "kcsa/pdu_to_stream2.h"
#include "kcsa/byte_interpolator.h"
#include "kcsa/gating_cbc.h"
#include "kcsa/afsk1200_raw_rx.h"
#include "kcsa/ax25_debug.h"
#include "kcsa/ber_sink.h"
#include "kcsa/ber_source.h"
#include "kcsa/sync_det_b.h"
#include "kcsa/halfduplex_pdu_to_stream.h"
#include "kcsa/fullduplex_pdu_to_stream.h"
#include "kcsa/power_sensor.h"
#include "kcsa/psk_rx.h"
%}

%include "kcsa/kiss_server.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, kiss_server);
%include "kcsa/ccsds_tc.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, ccsds_tc);
%include "kcsa/ccsds_udp_tc.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, ccsds_udp_tc);
%include "kcsa/fec_decode_b.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, fec_decode_b);
%include "kcsa/fec_encode_b.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, fec_encode_b);
%include "kcsa/vitfilt27_bb.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, vitfilt27_bb);
%include "kcsa/vitfilt27_fb.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, vitfilt27_fb);
%include "kcsa/encode27_bb.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, encode27_bb);
%include "kcsa/clock_recovery_gardner_ff.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, clock_recovery_gardner_ff);
%include "kcsa/clock_recovery_gardner_cc.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, clock_recovery_gardner_cc);
%include "kcsa/halfduplex_tc.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, halfduplex_tc);
%include "kcsa/power_sensor.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, power_sensor);



%include "kcsa/pdu_to_stream.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, pdu_to_stream);
%include "kcsa/byte_interpolator.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, byte_interpolator);
%include "kcsa/gating_cbc.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, gating_cbc);
%include "kcsa/afsk1200_raw_rx.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, afsk1200_raw_rx);

%include "kcsa/ax25_debug.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, ax25_debug);
%include "kcsa/ber_sink.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, ber_sink);
%include "kcsa/ber_source.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, ber_source);
%include "kcsa/sync_det_b.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, sync_det_b);

%include "kcsa/halfduplex_pdu_to_stream.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, halfduplex_pdu_to_stream);
%include "kcsa/fullduplex_pdu_to_stream.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, fullduplex_pdu_to_stream);
%include "kcsa/pdu_to_stream2.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, pdu_to_stream2);

%include "kcsa/psk_rx.h"
GR_SWIG_BLOCK_MAGIC2(kcsa, psk_rx);
