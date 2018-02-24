/* -*- c++ -*- */
/* 
 * Copyright 2016 KeChuang Space Academy <sa@kc.ac.cn>
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

#ifndef INCLUDED_KCSA_SYNC_DET_B_IMPL_H
#define INCLUDED_KCSA_SYNC_DET_B_IMPL_H

#include <kcsa/sync_det_b.h>
#include <vector>

namespace gr {
  namespace kcsa {
    class sync_det_b_impl : public sync_det_b
    {
      private:
        int           d_state;
        uint64_t      d_sync_word;
        uint64_t      d_sync_mask;
        uint64_t      shiftreg;
        uint16_t      d_len;
        int           d_len_bytes;
        uint16_t      d_frame_mtu;    //frame MTU
        int           d_maxerrs;
        uint16_t      cnt;
        uint8_t       d_nrz;
        uint8_t*      d_buf;//
        uint8_t       bytereg;
        int           bcnt;
        bool          d_len_mode;
        pmt::pmt_t    d_out_port;
        //std::vector<uint8_t>	d_buf;
        std::stringstream sStream;
      public:
        sync_det_b_impl(uint64_t sync_word, uint64_t sync_mask, bool len_mode, int len, uint8_t nrz, int maxerrs);
        ~sync_det_b_impl();

        // Where all the action really happens
        int work(int noutput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_SYNC_DET_B_IMPL_H */

