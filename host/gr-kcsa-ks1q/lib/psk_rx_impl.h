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

#ifndef INCLUDED_KCSA_PSK_RX_IMPL_H
#define INCLUDED_KCSA_PSK_RX_IMPL_H

#include <kcsa/psk_rx.h>
#include <psk/psk_rx_cc.h>

namespace gr {
  namespace kcsa {

    class psk_rx_impl : public psk_rx
    {
     private:
      psk_rx_cc d_rx;
      int d_sps;
      float d_sym_rate;

     public:
      psk_rx_impl(int _mod, int sps, float sym_rate, float sym_rate_dev, float sym_bw, float carr_rate, float carr_rate_dev, float carr_bw);
      ~psk_rx_impl();

      // Where all the action really happens
      void forecast (int noutput_items, gr_vector_int &ninput_items_required);

      int general_work(int noutput_items,
           gr_vector_int &ninput_items,
           gr_vector_const_void_star &input_items,
           gr_vector_void_star &output_items);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_PSK_RX_IMPL_H */
