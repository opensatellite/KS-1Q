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
#include "psk_rx_impl.h"

namespace gr {
  namespace kcsa {

    psk_rx::sptr
    psk_rx::make(int _mod, int sps, float sym_rate, float sym_rate_dev, float sym_bw, float carr_rate, float carr_rate_dev, float carr_bw)
    {
      return gnuradio::get_initial_sptr
        (new psk_rx_impl(_mod, sps, sym_rate, sym_rate_dev, sym_bw, carr_rate, carr_rate_dev, carr_bw));
    }

    /*
     * The private constructor
     */
    psk_rx_impl::psk_rx_impl(int _mod, int sps, float sym_rate, float sym_rate_dev, float sym_bw, float carr_rate, float carr_rate_dev, float carr_bw)
      : gr::block("psk_rx",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(1, 1, sizeof(gr_complex))),
        d_rx((enum psk_rx_cc::mod_type)_mod, sps, sym_rate, sym_rate_dev, sym_bw, carr_rate, carr_rate_dev, carr_bw),
        d_sps(sps),
        d_sym_rate(sym_rate)
    {

    }

    /*
     * Our virtual destructor.
     */
    psk_rx_impl::~psk_rx_impl()
    {
    }

    void
    psk_rx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
        ninput_items_required[0] = floor(noutput_items / d_sym_rate * d_sps) + 1;
    }

    int
    psk_rx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const gr_complex *in = (const gr_complex *) input_items[0];
      gr_complex *out = (gr_complex *) output_items[0];

      int consumed, produced;
      d_rx.work(in, ninput_items[0], &consumed, out, noutput_items, &produced);

      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (consumed);

      // Tell runtime system how many output items we produced.
      return produced;
    }

  } /* namespace kcsa */
} /* namespace gr */
