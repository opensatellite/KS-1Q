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

#include <gnuradio/config.h>

#include <gnuradio/io_signature.h>
#include "power_sensor_impl.h"
#include <volk/volk.h>
#include <gnuradio/rpcregisterhelpers.h>

#ifndef GR_CTRLPORT
#error "gr-ctrlport must be enabled"
#endif

namespace gr {
  namespace kcsa {

    power_sensor::sptr
    power_sensor::make(int decim, float cal)
    {
      return gnuradio::get_initial_sptr
        (new power_sensor_impl(decim, cal));
    }

    /*
     * The private constructor
     */
    power_sensor_impl::power_sensor_impl(int decim, float _cal)
      : gr::sync_decimator("power_sensor",
              gr::io_signature::make(1, 1, sizeof(gr_complex)),
              gr::io_signature::make(0, 1, sizeof(float)), decim),
              result(0),
              d_cal(_cal)
    {
        magsqr = (float*)volk_malloc(sizeof(float) * decim, volk_get_alignment());
        sumout = (float*)volk_malloc(sizeof(float), volk_get_alignment());
    }

    /*
     * Our virtual destructor.
     */
    power_sensor_impl::~power_sensor_impl()
    {
        volk_free(magsqr);
        volk_free(sumout);
    }

    void power_sensor_impl::setup_rpc()
    {
        //#ifdef GR_CTRLPORT
        add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_get<power_sensor, float>(
        alias(), "power",
        &power_sensor::power,
        pmt::mp(-300.0), pmt::mp(100.0), pmt::mp(-300.0),
        "dBW", "Get power readout", RPC_PRIVLVL_MIN,
        DISPTIME)));

        add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_get<power_sensor, float>(
        alias(), "cal",
        &power_sensor::cal,
        pmt::mp(-300.0), pmt::mp(300.0), pmt::mp(0.0),
        "dB", "Get cal offset", RPC_PRIVLVL_MIN,
        DISPNULL)));

        add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_set<power_sensor, float>(
        alias(), "cal",
        &power_sensor::set_cal,
        pmt::mp(-300.0), pmt::mp(300.0), pmt::mp(0.0),
        "dB", "Set cal offset", RPC_PRIVLVL_MIN,
        DISPNULL)));
        //#endif /* GR_CTRLPORT */
    }

    int
    power_sensor_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
        const gr_complex *in = (const gr_complex *) input_items[0];
        float *out = NULL;
        if(output_items.size()) out = (float *) output_items[0];

        // Do <+signal processing+>
        for(int i = 0; i < noutput_items; i++) {
            volk_32fc_magnitude_squared_32f(magsqr, in, decimation());
            volk_32f_accumulator_s32f(sumout, magsqr, decimation());
            in += decimation();
            result = 10.0 * log10( *sumout / decimation());
            if(out) *out++ = result;
        }

        // Tell runtime system how many output items we produced.
        return noutput_items;
    }

  } /* namespace kcsa */
} /* namespace gr */
