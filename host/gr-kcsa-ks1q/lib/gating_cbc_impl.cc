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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "gating_cbc_impl.h"

namespace gr {
  namespace kcsa {

    gating_cbc::sptr
    gating_cbc::make()
    {
      return gnuradio::get_initial_sptr
        (new gating_cbc_impl());
    }

    /*
     * The private constructor
     */
    gating_cbc_impl::gating_cbc_impl()
      : gr::block("gating_cbc",
    	    gr::io_signature::make2(2, 2, sizeof(gr_complex), sizeof(uint8_t)),
            gr::io_signature::make(1, 1, sizeof(gr_complex))),
            d_state(false)
    {}

    /*
     * Our virtual destructor.
     */
    gating_cbc_impl::~gating_cbc_impl()
    {
    }

    void
    gating_cbc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
      ninput_items_required[0] = noutput_items;
      ninput_items_required[1] = noutput_items;
    }

    int
    gating_cbc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      const uint8_t *gate = (const uint8_t *) input_items[1];
      gr_complex *out = (gr_complex *) output_items[0];

      // Do <+signal processing+>
      for(int i = 0; i < noutput_items; i++) {
      	*out++ = (*gate++) ? (*in) : gr_complex(0.0, 0.0);
      	in++;
      }
      
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each(noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace kcsa */
} /* namespace gr */

