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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "fullduplex_pdu_to_stream_impl.h"

namespace gr {
  namespace kcsa {

    fullduplex_pdu_to_stream::sptr
    fullduplex_pdu_to_stream::make(uint8_t acqsamples, bool flenmode, uint32_t syncword, int syncbits)
    {
      return gnuradio::get_initial_sptr
        (new fullduplex_pdu_to_stream_impl(acqsamples, flenmode, syncword, syncbits));
    }

    /*
     * The private constructor
     */
    fullduplex_pdu_to_stream_impl::fullduplex_pdu_to_stream_impl(uint8_t acqsamples,
                                                                 bool flenmode, uint32_t syncword, int syncbits)
        : gr::sync_decimator("halfduplex_pdu_to_stream",
                             gr::io_signature::make(0, 0, 0),
                             gr::io_signature::make(1, 1, sizeof(uint8_t)), 1),
          d_acqsamples(acqsamples),
          d_flenmode(flenmode),
          d_syncword(syncword),
          d_syncbits(syncbits)
    {
        message_port_register_in(pmt::mp("pdu"));
        set_msg_handler(pmt::mp("pdu"), boost::bind(&fullduplex_pdu_to_stream_impl::pdu_callback, this ,_1));
    }

    /*
     * Our virtual destructor.
     */
    fullduplex_pdu_to_stream_impl::~fullduplex_pdu_to_stream_impl()
    {
    }

    void fullduplex_pdu_to_stream_impl::pdu_callback(pmt::pmt_t msg)
    {
        size_t msg_len;
        size_t i = 0;
        uint32_t mask = (0x1 << d_syncbits) - 1;
        uint32_t syncword = 0x55555555;

        syncword &= ~mask;
        syncword |= (d_syncword & mask);

        //pmt::pmt_t meta = pmt::car(msg);
        pmt::pmt_t data = pmt::cdr(msg);

        //get a message, push into queue
        const uint8_t* bytes_in = pmt::u8vector_elements(data, msg_len);

        syncword = BigLittleSwap32(syncword);

        if(!d_flenmode) //False:Variable Frame Length;   True:Fixed Frame Length,don't fill
        {
            std::vector<uint8_t> vmsg(sizeof(uint32_t) + sizeof(uint16_t) + msg_len);
            //insert syncword
            memcpy(vmsg.data(), &syncword, sizeof(uint32_t));

            //insert frame length
            uint16_t flen = msg_len;
            //flen = BigLittleSwap16(flen);
            memcpy(vmsg.data() + sizeof(uint32_t), &flen, sizeof(uint16_t));

            //insert frame  data
            memcpy(vmsg.data() + sizeof(uint32_t) + sizeof(uint16_t), bytes_in, msg_len);

            gr::thread::scoped_lock guard(d_mutex);
            //insert frame  data queue
            q.push(vmsg);
        }
        else
        {
            std::vector<uint8_t> vmsg(sizeof(uint32_t) + msg_len);
            //insert syncword
            memcpy(vmsg.data(), &syncword, sizeof(uint32_t));

            //insert frame  data
            memcpy(vmsg.data() + sizeof(uint32_t), bytes_in, msg_len);

            gr::thread::scoped_lock guard(d_mutex);
            //insert frame  data queue
            q.push(vmsg);
        }
    }

    int
    fullduplex_pdu_to_stream_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      int i = 0;
      uint8_t* out = (uint8_t*)output_items[0];

      // Do <+signal processing+>
      while(i < noutput_items)
      {
          if (q.empty())
          {
              out[i++] = d_acqsamples;
              //std::cout << "out[ " << i << "]=" << 0x55 << std::endl;
          }
          else
          {
              gr::thread::scoped_lock guard(d_mutex);
              std::vector<uint8_t> vdata = q.front();

              if(i + vdata.size () < noutput_items)
              {
                  q.pop();
                  //insert data
                  for(int j = 0; j < vdata.size (); j++)
                  {
                      out[i++] = vdata[j];
                  }
                  //std::cout << "out[ " << i << "]=" << std::hex << uint16_t(out[i]) << std::endl;
              }
              else
              {
                  //not enough free buffer,the left buffer fills with d_acqsamples
                  while (i < noutput_items)
                  {
                      out[i++] = d_acqsamples;
                  }
              }
          }
      }
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace kcsa */
} /* namespace gr */

