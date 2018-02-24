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
#include "ber_source_impl.h"

namespace gr {
  namespace kcsa {

    ber_source::sptr
    ber_source::make()
    {
      return gnuradio::get_initial_sptr
        (new ber_source_impl());
    }

    /*
     * The private constructor
     */
    ber_source_impl::ber_source_impl()
      : gr::sync_block("ber_source",
              gr::io_signature::make(0, 0, 0),
              gr::io_signature::make(1, 1, sizeof(unsigned char)))
    {}

    /*
     * Our virtual destructor.
     */
    ber_source_impl::~ber_source_impl()
    {
    }
    
    /*
    ASM32 1ACFFC1D
    ASM64 034776C7272895B0 
    ASM128 25D5C0CE8990F6C9461BF79C
    ASM256 034776C7272895B0 FCB88938D8D76A4F
    ASM512 25D5C0CE8990F6C9461BF79C DA2A3F31766F0936B9E40863
    */

    int
    ber_source_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      unsigned char *out = (unsigned char *) output_items[0];

      // Do <+signal processing+>
      for(int i = 0;i < noutput_items; i++)
      {
      	*out++ = 1;
      }

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace kcsa */
} /* namespace gr */

