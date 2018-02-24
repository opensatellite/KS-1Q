/* -*- c++ -*- */
/* 
 * Copyright 2013 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_AFSK_AFSK1200_RAW_RX_IMPL_H
#define INCLUDED_AFSK_AFSK1200_RAW_RX_IMPL_H

#include <kcsa/afsk1200_raw_rx.h>
#include <boost/crc.hpp>

namespace gr {
  namespace kcsa {

    class afsk1200_raw_rx_impl : public afsk1200_raw_rx
    {
     private:
      // decoder states
      struct l1_state_afsk12 {
                unsigned int dcd_shreg;
                unsigned int sphase;
                unsigned int lasts;
                unsigned int subsamp;
        } afsk12;

      // actual samplerate  of the decoder
      int d_sample_rate;
      int d_corrlen;
      int d_sphaseinc;

      float *corr_mark_i;
      float *corr_mark_q;
      float *corr_space_i;
      float *corr_space_q;

      inline float mac(const float *a, const float *b, unsigned int size)
      {
              float sum = 0;
              unsigned int i;

              for (i = 0; i < size; i++)
                      sum += (*a++) * (*b++);
              return sum;
      }

      inline float fsqr(float f)
      {
              return f*f;
      }

     public:
      afsk1200_raw_rx_impl(int sample_rate);
      ~afsk1200_raw_rx_impl();

      // Where all the action really happens
      int general_work (int noutput_items,
                        gr_vector_int &ninput_items,
                        gr_vector_const_void_star &input_items,
                        gr_vector_void_star &output_items);
   };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_AFSK1200_RAW_RX_IMPL_H */

