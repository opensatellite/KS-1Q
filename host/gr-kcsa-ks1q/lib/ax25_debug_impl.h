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

#ifndef INCLUDED_KCSA_AX25_DEBUG_IMPL_H
#define INCLUDED_KCSA_AX25_DEBUG_IMPL_H

#include <kcsa/ax25_debug.h>

namespace gr {
  namespace kcsa {

    class ax25_debug_impl : public ax25_debug
    {
     private:
      pmt::pmt_t d_pdu_port;
      
	  void pdu_callback(pmt::pmt_t msg);

     public:
      ax25_debug_impl();
      ~ax25_debug_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_AX25_DEBUG_IMPL_H */
