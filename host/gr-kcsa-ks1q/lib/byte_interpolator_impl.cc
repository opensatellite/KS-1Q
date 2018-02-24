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
#include "byte_interpolator_impl.h"

namespace gr {
  namespace kcsa {

    byte_interpolator::sptr
    byte_interpolator::make(int interp)
    {
      return gnuradio::get_initial_sptr
        (new byte_interpolator_impl(interp));
    }

    /*
     * The private constructor
     */
    byte_interpolator_impl::byte_interpolator_impl(int interp)
      : gr::sync_interpolator("byte_interpolator",
              gr::io_signature::make(1, 1, sizeof(uint8_t)),
              gr::io_signature::make(1, 1, sizeof(uint8_t)), interp),
              d_interp(interp)
    {}

    /*
     * Our virtual destructor.
     */
    byte_interpolator_impl::~byte_interpolator_impl()
    {
    }

    int
    byte_interpolator_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const uint8_t *in = (const uint8_t *) input_items[0];
      uint8_t *out = (uint8_t *) output_items[0];

      // Do <+signal processing+>
      if((noutput_items % d_interp) != 0) {
      	std::cerr << "items not in multiple" << std::endl;
      }
      for(int i = 0; i < noutput_items / d_interp; i++) {
      	for(int j = 0; j < d_interp; j++) {
      		*out++ = *in;
      	}
      	in++;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace kcsa */
} /* namespace gr */

