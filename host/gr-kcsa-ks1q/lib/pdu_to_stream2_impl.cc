/* -*- c++ -*- */
/*
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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
#include "pdu_to_stream2_impl.h"

namespace gr {
  namespace kcsa {

    pdu_to_stream2::sptr
    pdu_to_stream2::make(double decim, unsigned char filler, int firstfill)
    {
      return gnuradio::get_initial_sptr
        (new pdu_to_stream2_impl(decim, filler, firstfill));
    }

    /*
     * The private constructor
     */
    pdu_to_stream2_impl::pdu_to_stream2_impl(double _decim, unsigned char filler, int firstfill)
      : gr::block("pdu_to_stream2",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 2, sizeof(unsigned char))),
              d_decim(_decim),
              rem_samples(0)
    {
        d_pdu_port = pmt::mp("pdu");
        message_port_register_in(d_pdu_port);
        set_msg_handler(d_pdu_port, boost::bind(&pdu_to_stream2_impl::pdu_callback, this ,_1) );
        set_relative_rate(1.0 / d_decim);
    }

    /*
     * Our virtual destructor.
     */
    pdu_to_stream2_impl::~pdu_to_stream2_impl()
    {
    }

    void pdu_to_stream2_impl::pdu_callback(pmt::pmt_t msg)
    {
        //pmt::pmt_t meta(pmt::car(msg));
        pmt::pmt_t data(pmt::cdr(msg));

        //get a message, push into queue
        size_t msg_len;
        const uint8_t* bytes_in = pmt::u8vector_elements(data, msg_len);

        gr::thread::scoped_lock guard(d_mutex);
        for(size_t i = 0; i < msg_len; i++)
        {
            q.push(bytes_in[i]);
        }
    }

    void
    pdu_to_stream2_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    	double req_real = ((double)noutput_items) * d_decim;
    	if(req_real > 1048576.0*64.0) req_real = 1048576.0*64.0;
        ninput_items_required[0] = floor(req_real) + 1;
    }

    int
    pdu_to_stream2_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
        uint8_t *out = (uint8_t *) output_items[0];
        uint8_t *amp = nullptr;
        if(output_items.size())
            amp = (uint8_t *) output_items[1];

		double possible_nout = ninput_items[0] / d_decim;
		if(possible_nout > noutput_items)
			noutput_items = floor(possible_nout);	
		
        //static int counter = 0;
        // Do <+signal processing+>
        for(int i = 0; i < noutput_items; i++)
        {
              gr::thread::scoped_lock guard(d_mutex);
              if(q.empty()) {
                  //if(counter) {
                      //std::cout << "packet " << counter << " bytes" << std::endl;
                    //  counter = 0;
                  //}
                  *out++ = d_filler;
                  if(amp) *amp++ = 0;
              } else {
                  //counter++;
                  *out++ = q.front();
                  //std::cout << (int)q.front() << ",";
                  q.pop();
                  if(amp) *amp++ = 1;
              }
        }

        rem_samples += noutput_items * d_decim;
        int to_consume = floor(rem_samples);
        rem_samples -= to_consume;
        consume_each(to_consume);

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace kcsa */
} /* namespace gr */
