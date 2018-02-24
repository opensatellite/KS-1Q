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

#ifndef INCLUDED_KCSA_HALFDUPLEX_TC_IMPL_H
#define INCLUDED_KCSA_HALFDUPLEX_TC_IMPL_H

#include <kcsa/halfduplex_tc.h>
#include <queue>
#include <vector>

    namespace gr {
  namespace kcsa {

  class halfduplex_tc_impl : public halfduplex_tc {
  private:
    // message input port
    pmt::pmt_t d_pdu_port;
    // ccsds tc use randomizer or not
    bool randomizer;
    // ccsds tc plop-2 cmm-1 acquisition (0b01010101) sequence length (samples)
    int acq_samples;
    // number of cltus
    int cltus;

    //----------------variables-------------------------
    uint16_t spacecraft_id;
    uint8_t version;
    // message input buffer
    std::queue<std::vector<uint8_t> > q;
    // cltu output buffer
    uint8_t *buf; // cltus * 8 + 2 + 24
    int bufpos;
    int buflen;
    int bufallocated;
    // acquisition sequence counter
    int acq_count;
    // static const int payload_bytes = 220;
    // static const int up_ip_bytes = 224;

    int state; // 0 = off, 1 = acq, 2 = data

    void buf_reset();
    void buf_push(uint8_t x);
    void make_cltus();
    void pdu_callback(pmt::pmt_t msg);

  public:
    halfduplex_tc_impl(uint16_t spacecraft_id, uint8_t version, bool randomizer,
                       int acq_samples, int cltus);
    ~halfduplex_tc_impl();

    // Where all the action really happens
    int work(int noutput_items, gr_vector_const_void_star &input_items,
             gr_vector_void_star &output_items);
  };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_HALFDUPLEX_TC_IMPL_H */
