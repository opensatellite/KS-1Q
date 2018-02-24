/* -*- c++ -*- */
/* 
 * Copyright 2016 KeChuang Space Academy.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifndef INCLUDED_KCSA_CCSDS_UDP_TC_IMPL_H
#define INCLUDED_KCSA_CCSDS_UDP_TC_IMPL_H

#include <kcsa/ccsds_udp_tc.h>
#include <netinet/in.h>


namespace gr {
  namespace kcsa {

    class ccsds_udp_tc_impl : public ccsds_udp_tc
    {
     private:
      //----------------config parameters-----------------
      //udp remote address
      struct sockaddr_in udp_addr;
      int decim;
      //ccsds tc use randomizer or not
      bool randomizer;
      //ccsds tc plop-2 cmm-1 sine wave (all zero) sequence length (samples)
      int sine_samples;
      //ccsds tc plop-2 cmm-1 acquisition (0b01010101) sequence length (samples)
      int acq_samples;
      
      //----------------variables-------------------------
      //udp file descriptor
      int sockfd;
      //cltu output buffer
      uint8_t buf[512];
      int bufpos;
      int buflen;
      //sine wave sequence counter
      int sine_count;
      //acquisition sequence counter
      int acq_count;
      //uplink state
      enum uplink_state_t {
		plop_sine, 
		plop_acq, 
		plop_idle,
		plop_off
	  } stat;
	  
	  void on();
	  void off();
	  
	  void buf_reset();
	  void buf_push(uint8_t x);
	  void make_cltus();
	  
     public:
      ccsds_udp_tc_impl(char* addr, int port, int decim, bool randomizer, int sine_samples, int acq_samples);
      ~ccsds_udp_tc_impl();

	  bool start();
      // Where all the action really happens
      int work (int noutput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_CCSDS_TC_IMPL_H */

