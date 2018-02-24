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

#include "ber_sink_impl.h"
#include <gnuradio/io_signature.h>

namespace gr {
namespace kcsa {

ber_sink::sptr ber_sink::make(int minerrs, int drop) {
    return gnuradio::get_initial_sptr(new ber_sink_impl(minerrs, drop));
}

/*
 * The private constructor
 */
ber_sink_impl::ber_sink_impl(int minerrs, int drop)
    : gr::sync_block("ber_sink",
                     gr::io_signature::make(1, 1, sizeof(unsigned char)),
                     gr::io_signature::make(0, 0, 0)),
      d_update_time(1000.0), d_factor(3),
      d_last_update(boost::posix_time::min_date_time), d_minerrs(minerrs),
      d_drop(drop), d_dropped(false), d_errs(0), d_total(0)
{
    message_port_register_out(pmt::mp("ber"));
}

/*
 * Our virtual destructor.
 */
ber_sink_impl::~ber_sink_impl() {}

void ber_sink_impl::setup_rpc() {
#ifdef GR_CTRLPORT
    /*
    add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_get<probe_rate_impl, double >(
            alias(), "rate_items",
            &probe_rate_impl::rate,
            pmt::mp(0), pmt::mp(1e6), pmt::mp(1),
            "items/sec", "Item rate",
            RPC_PRIVLVL_MIN, DISPTIME | DISPOPTSTRIP)));
    add_rpc_variable(
        rpcbasic_sptr(new rpcbasic_register_get<probe_rate_impl, double >(
            alias(), "timesincelast",
            &probe_rate_impl::timesincelast,
            pmt::mp(0), pmt::mp(d_min_update_time*2), pmt::mp(0),
            "ms", "Time since last update",
            RPC_PRIVLVL_MIN, DISPTIME | DISPOPTSTRIP)));
            */
#endif
}

int ber_sink_impl::work(int noutput_items,
                        gr_vector_const_void_star &input_items,
                        gr_vector_void_star &output_items) {
    const unsigned char *in = (const unsigned char *)input_items[0];

    // count error bits
    if (!d_dropped)
    {
        d_total += noutput_items;
        if (d_total >= d_drop)
        {
            d_dropped = true;
            d_total = 0;
        }
    }
    else
    {
        for (int i = 0; i < noutput_items; i++)
        {
            if (*in++ == 0)
                d_errs++;
        }
        d_total += noutput_items;
    }

    if (d_errs >= d_minerrs)
    {
        boost::posix_time::ptime now(
                    boost::posix_time::microsec_clock::local_time());
        boost::posix_time::time_duration diff = now - d_last_update;
        if (diff.total_milliseconds() > d_update_time)
        {
            d_last_update = now;
            double ber = (double)d_errs / d_total / d_factor;
            pmt::pmt_t d = pmt::make_dict();
            d = pmt::dict_add(d, pmt::mp("error_bits"), pmt::mp(d_errs));
            d = pmt::dict_add(d, pmt::mp("total_bits"), pmt::mp(d_total));
            d = pmt::dict_add(d, pmt::mp("factor"), pmt::mp(d_factor));
            d = pmt::dict_add(d, pmt::mp("ber"), pmt::mp(ber));
            message_port_pub(pmt::mp("ber"), pmt::cons(d, pmt::PMT_NIL));
        }
    }

    // Tell runtime system how many output items we produced.
    return noutput_items;
}

void ber_sink_impl::reset()
{
    d_errs = 0;
    d_total = 0;
}

bool ber_sink_impl::start()
{
    d_errs = 0;
    d_total = 0;
    d_dropped = false;
    // d_last_update = boost::posix_time::microsec_clock::local_time();
    return true;
}

bool ber_sink_impl::stop()
{
    return true;
}

} /* namespace kcsa */
} /* namespace gr */
