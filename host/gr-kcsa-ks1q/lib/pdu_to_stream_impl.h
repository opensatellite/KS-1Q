/* -*- c++ -*- */
/* 
 * Copyright 2016 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_KCSA_PDU_TO_STREAM_IMPL_H
#define INCLUDED_KCSA_PDU_TO_STREAM_IMPL_H

#include <kcsa/pdu_to_stream.h>
#include <queue>
#include <gnuradio/thread/thread.h>

namespace gr {
  namespace kcsa {

    class pdu_to_stream_impl : public pdu_to_stream
    {
     private:
      //message input port
      pmt::pmt_t d_pdu_port;
      //empty output filler
      uint8_t d_filler;
      //message input buffer
      std::queue<uint8_t> q;

      gr::thread::mutex d_mutex;
      
      void pdu_callback(pmt::pmt_t msg);

     public:
      pdu_to_stream_impl(int decim, uint8_t filler);
      ~pdu_to_stream_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_PDU_TO_STREAM_IMPL_H */

