# Copyright 2011 Free Software Foundation, Inc.
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
install(FILES
    kcsa_ccsds_tc.xml
    kcsa_ccsds_udp_tc.xml
    kcsa_fec_decode_b.xml
    kcsa_fec_encode_b.xml
    kcsa_vitfilt27_bb.xml
    kcsa_encode27_bb.xml
    kcsa_kiss_server.xml
    kcsa_vitfilt27_fb.xml
    kcsa_clock_recovery_gardner_xx.xml
    kcsa_gmsk_demod.xml
    kcsa_halfduplex_tc.xml
    kcsa_pdu_to_stream.xml
    kcsa_pdu_to_stream2.xml
    kcsa_byte_interpolator.xml
    kcsa_gating_cbc.xml
    kcsa_afsk1200_raw_rx.xml
    kcsa_ax25_debug.xml
    kcsa_ber_sink.xml
    kcsa_ber_source.xml
    kcsa_sync_det_b.xml
    kcsa_halfduplex_pdu_to_stream.xml
    kcsa_fullduplex_pdu_to_stream.xml
    kcsa_power_sensor.xml
    kcsa_pdu_to_stream2.xml
    DESTINATION share/gnuradio/grc/blocks
)
