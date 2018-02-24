/* -*- c++ -*- */
/*
Copyright 2016 KeChuang Space Academy <sa@kc.ac.cn>

This is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

This software is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this software; see the file COPYING.  If not, write to
the Free Software Foundation, Inc., 51 Franklin Street,
Boston, MA 02110-1301, USA.
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "sync_det_b_impl.h"
#include <stdexcept>
#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <sstream>

#define FRAME_LENGTH       16

/*
static inline int nsetbits(uint16_t i)
{
      i = i - ((i >> 1) & 0x5555);
      i = (i & 0x3333) + ((i >> 2) & 0x3333);
      return (int)(unchecked(((i + (i >> 4)) & 0x0F0F) * 0x0101) >> 8);s
}

static inline int nsetbits(uint32_t i)
{
      i = i - ((i >> 1) & 0x55555555);
      i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
      return (int)(unchecked(((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24);
}

static int nsetbits(uint64_t i)
{
i = i - ((i >> 1) & 0x5555555555555555);
i = (i & 0x3333333333333333) + ((i >> 2) & 0x3333333333333333);
return (int)(unchecked(((i + (i >> 4)) & 0xF0F0F0F0F0F0F0F) * 0x101010101010101) >> 56);
}
*/

//link with -mpopcnt
static inline int nsetbits(uint64_t x)
{
    return __builtin_popcountll(x);
}

namespace gr {
namespace kcsa {

sync_det_b::sptr
sync_det_b::make(uint64_t sync_word, uint64_t sync_mask, bool len_mode, int len, uint8_t nrz, int maxerrs)
{
    return gnuradio::get_initial_sptr
            (new sync_det_b_impl(sync_word, sync_mask, len_mode, len, nrz, maxerrs));
}

/*
* The private constructor
*/
sync_det_b_impl::sync_det_b_impl(uint64_t sync_word, uint64_t sync_mask, bool len_mode, int len, uint8_t nrz, int maxerrs)
    : gr::sync_block("sync_det_b",
                     gr::io_signature::make(1, 1, sizeof(unsigned char)),
                     gr::io_signature::make(0, 0, 0)),
      d_state(0),
      d_sync_word(sync_word),
      d_sync_mask(sync_mask),
      d_len_mode(len_mode),        //True:Fixed;  False:Variable
      d_len(len),
      d_len_bytes(2),
      d_nrz(nrz),
      d_maxerrs(maxerrs),
      shiftreg(0),
      d_frame_mtu(len)
{
    //keep at least two bytes for d_len_bytes
    d_buf = new uint8_t[d_frame_mtu * sizeof(uint8_t) + d_len_bytes];

    d_out_port = pmt::mp("out");
    message_port_register_out(d_out_port);
}

/*
* Our virtual destructor.
*/
sync_det_b_impl::~sync_det_b_impl()
{
    delete[] d_buf;
}

int
sync_det_b_impl::work(int noutput_items,
                      gr_vector_const_void_star &input_items,
                      gr_vector_void_star &output_items)
{
    const uint8_t *in = (const uint8_t *) input_items[0];

    // Do <+signal processing+>
    for(int i = 0; i < noutput_items; i++)
    {
        switch(d_state)
        {
        case 0: //find sync word
            shiftreg = (shiftreg << 1) | (*in++ ? 1 : 0);
            if(nsetbits((shiftreg ^ d_sync_word) & d_sync_mask) <= d_maxerrs)
            {
                cnt = 0;
                bcnt = 0;
                if(!d_len_mode)//False:Variable Frame Length
                    d_state = 1;
                else  { //True:Fixed Frame Length,start of pdu directly
                    if(d_len > 0) {
                        d_state = 2;
                    } else {
                        //end of pdu
                        message_port_pub(d_out_port, pmt::cons(
                                         pmt::PMT_NIL,
                                         pmt::init_u8vector(size_t(d_len), d_buf))
                                         );
                        d_state = 0;
                    }
                }
            }
            break;
        case 1: //receive length
            bytereg = (bytereg << 1) | (*in++ ? 1 : 0);
            bcnt++;
            if(bcnt >= 8)
            {
                //push byte
                d_buf[cnt++] = bytereg;
                bcnt = 0;
                if(cnt > d_len_bytes) std::cerr << "invaild cnt in state 2" << std::endl;
                if(cnt >= d_len_bytes) {
                    //get length
                    d_len = d_buf[0] | (d_buf[1] << 8);
                    //memcpy(&d_len, d_buf, d_len_bytes);
                    //check d_len less than mtu
                    if(d_len > d_frame_mtu) {
                        d_state = 0;
                    }
                    else {
                        cnt = 0;
                        d_state = 2;
                    }
                }
            }
            break;
        case 2://receive data
            bytereg = (bytereg << 1) | (*in++ ? 1 : 0);
            bcnt++;
            if(bcnt >= 8)
            {
                //push byte
                d_buf[cnt++] = bytereg;
                bcnt = 0;
                if(cnt > d_len) std::cerr << "invaild cnt in state 2" << std::endl;
                if(cnt >= d_len)
                {
                    //end of pdu
                    message_port_pub(d_out_port, pmt::cons(
                                         pmt::PMT_NIL,
                                         pmt::init_u8vector(size_t(d_len), d_buf))
                                     );
                    d_state = 0;
                }
            }
            break;
        default:
            d_state = 0;
            in++;
            std::cerr << "invaild cnt in state " << d_state << std::endl;

        }
    }

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

} /* namespace kcsa */
} /* namespace gr */
