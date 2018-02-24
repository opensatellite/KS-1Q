# Copyright 2011,2012 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.

########################################################################
# Install public header files
########################################################################
install(FILES
    api.h
    kiss_server.h
    ccsds_tc.h
    ccsds_udp_tc.h
    encode27_bb.h
    fec_decode_b.h
    fec_encode_b.h
    vitfilt27_bb.h
    vitfilt27_fb.h
    clock_recovery_gardner_ff.h
    clock_recovery_gardner_cc.h
    halfduplex_tc.h
    pdu_to_stream.h
    byte_interpolator.h
    gating_cbc.h
    afsk1200_raw_rx.h
    ax25_debug.h
    ber_sink.h
    ber_source.h
    sync_det_b.h
    halfduplex_pdu_to_stream.h
    fullduplex_pdu_to_stream.h
    pdu_to_stream2.h DESTINATION include/kcsa
)
