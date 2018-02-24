/* -*- c++ -*- */
/* 
 * Copyright 2017 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_KCSA_HALFDUPLEX_PDU_TO_STREAM_IMPL_H
#define INCLUDED_KCSA_HALFDUPLEX_PDU_TO_STREAM_IMPL_H


#include <kcsa/halfduplex_pdu_to_stream.h>
#include <queue>
#include <gnuradio/thread/thread.h>
#include <pmt/pmt.h>

#define BigLittleSwap16(A) ((((uint16_t)(A) & 0xff00) >> 8) |  \
                            (((uint16_t)(A) & 0x00ff) << 8))

#define BigLittleSwap32(A) ((((uint32_t)(A) & 0xff000000) >> 24) | \
                            (((uint32_t)(A) & 0x00ff0000) >> 8) |  \
                            (((uint32_t)(A) & 0x0000ff00) << 8) | \
                            (((uint32_t)(A) & 0x000000ff) << 24))

#define BigLittleSwap64(A) ((((uint64_t)(A) & 0xff00000000000000) >> 56) | \
                            (((uint64_t)(A) & 0x00ff000000000000) >> 40) | \
                            (((uint64_t)(A) & 0x0000ff0000000000) >> 24) | \
                            (((uint64_t)(A) & 0x000000ff00000000) >> 8)  | \
                            (((uint64_t)(A) & 0x00000000ff000000) << 8)  | \
                            (((uint64_t)(A) & 0x0000000000ff0000) << 24) | \
                            (((uint64_t)(A) & 0x000000000000ff00) << 40) | \
                            (((uint64_t)(A) & 0x00000000000000ff) << 56))
namespace gr {
  namespace kcsa {

    class halfduplex_pdu_to_stream_impl : public halfduplex_pdu_to_stream
    {
     private:
        //message input buffer
        uint8_t d_acqsamples;
        int d_acqcount;
        int d_syncbits;
        int d_state;
        uint32_t d_syncword;
        bool d_flenmode;
        bool isfullflag;

        std::stringstream sStream;

        //std::queue<uint8_t> q;
        std::queue<std::vector<uint8_t> > q;
        gr::thread::mutex d_mutex;

        void pdu_callback(pmt::pmt_t msg);
     public:
      halfduplex_pdu_to_stream_impl(uint8_t acqsamples, int acqcount, bool flenmode, uint32_t syncword, int syncbits);
      ~halfduplex_pdu_to_stream_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_HALFDUPLEX_PDU_TO_STREAM_IMPL_H */

