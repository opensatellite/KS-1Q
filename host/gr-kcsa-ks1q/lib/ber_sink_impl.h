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

#ifndef INCLUDED_KCSA_BER_SINK_IMPL_H
#define INCLUDED_KCSA_BER_SINK_IMPL_H

#include <kcsa/ber_sink.h>

namespace gr {
  namespace kcsa {

    class ber_sink_impl : public ber_sink
    {
     private:
      double d_update_time;
      boost::posix_time::ptime d_last_update;
      int d_factor;
      int d_minerrs;
      int d_drop;
      bool d_dropped;
      uint64_t d_errs;
      uint64_t d_total;
      void setup_rpc();

     public:
      ber_sink_impl(int minerrs, int drop);
      ~ber_sink_impl();
      
      double ber();
      void reset();

      bool start();
      bool stop();

      // Where all the action really happens
      int work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_BER_SINK_IMPL_H */

