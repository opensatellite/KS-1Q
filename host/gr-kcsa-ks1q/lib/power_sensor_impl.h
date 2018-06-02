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

#ifndef INCLUDED_KCSA_POWER_SENSOR_IMPL_H
#define INCLUDED_KCSA_POWER_SENSOR_IMPL_H

#include <kcsa/power_sensor.h>

namespace gr {
  namespace kcsa {

    class power_sensor_impl : public power_sensor
    {
     private:
      float *magsqr, *sumout;
      float result, d_cal;

     public:
      power_sensor_impl(int decim, float cal);
      ~power_sensor_impl();

      float power() { return result + d_cal; }
      float cal() { return d_cal; }
      void set_cal(float _cal) { d_cal = _cal; }

      void setup_rpc();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace kcsa
} // namespace gr

#endif /* INCLUDED_KCSA_POWER_SENSOR_IMPL_H */
