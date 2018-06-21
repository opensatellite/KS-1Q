# Copyright 2011,2012,2016 Free Software Foundation, Inc.
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
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set (CMAKE_CXX_STANDARD 11)

set(GR_REQUIRED_COMPONENTS RUNTIME FILTER)
find_package(Gnuradio "3.7.9" REQUIRED)
list(APPEND kcsa_sources
	util/fifobuf.c
    ccsds/rs.c
    ccsds/ccsds.c
    ccsds/tab.c
    ccsds/ccsds_tal.c
    ccsds/metrics.c
    ccsds/randomizer.c
    ccsds/viterbi27.c
    fec_decode_b_impl.cc
    fec_encode_b_impl.cc
    encode27_bb_impl.cc
    vitfilt27_bb_impl.cc
    vitfilt27_fb_impl.cc
    ccsds_udp_tc_impl.cc
    ccsds_tc_impl.cc
    kiss_server_impl.cc
    clock_recovery_gardner_ff_impl.cc
    clock_recovery_gardner_cc_impl.cc
    halfduplex_tc_impl.cc
    pdu_to_stream_impl.cc
    pdu_to_stream2_impl.cc
    byte_interpolator_impl.cc
    gating_cbc_impl.cc
    afsk1200_raw_rx_impl.cc
    ax25_debug_impl.cc
    ber_sink_impl.cc
    ber_source_impl.cc
    sync_det_b_impl.cc
    halfduplex_pdu_to_stream_impl.cc
    fullduplex_pdu_to_stream_impl.cc
    power_sensor_impl.cc
    )

set(kcsa_sources "${kcsa_sources}" PARENT_SCOPE)
if(NOT kcsa_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT kcsa_sources)

add_library(gnuradio-kcsa SHARED ${kcsa_sources})
target_link_libraries(gnuradio-kcsa ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-kcsa PROPERTIES DEFINE_SYMBOL "gnuradio_kcsa_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-kcsa PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library file
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-kcsa RUNTIME_COMPONENT "kcsa_runtime" DEVEL_COMPONENT "kcsa_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})
#include_directories(/home/sa/gnss-sdr/build/volk_gnsssdr_module/install/include)
list(APPEND test_kcsa_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_kcsa.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_kcsa.cc
    )

add_executable(test-kcsa ${test_kcsa_sources})

target_link_libraries(
  test-kcsa
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-kcsa
)

GR_ADD_TEST(test_kcsa test-kcsa)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")
