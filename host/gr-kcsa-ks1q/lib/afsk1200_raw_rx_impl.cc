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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "afsk1200_raw_rx_impl.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>

#define FREQ_MARK  1200
#define FREQ_SPACE 2200
#define BAUD       1200
#define SUBSAMP    2

namespace gr {
namespace kcsa {
afsk1200_raw_rx::sptr
afsk1200_raw_rx::make(int sample_rate)
{
    return gnuradio::get_initial_sptr (new afsk1200_raw_rx_impl(sample_rate));
}

/*
     * The private constructor
     */
afsk1200_raw_rx_impl::afsk1200_raw_rx_impl(int sample_rate)
    : gr::block("afsk1200_raw_rx",
                    gr::io_signature::make(1, 1, sizeof (float)),
                    gr::io_signature::make(1, 1, sizeof(char)))
{
    float f;
    int i;
    d_sample_rate = sample_rate;
    d_corrlen     = d_sample_rate/BAUD;
    set_history(d_corrlen+1);
    d_sphaseinc   =  (0x10000u*BAUD*SUBSAMP/d_sample_rate);
    // Init the demodulator
    corr_mark_i = new float[d_corrlen];
    corr_mark_q = new float[d_corrlen];
    corr_space_i= new float[d_corrlen];
    corr_space_q= new float[d_corrlen];

    afsk12.dcd_shreg=0;
    afsk12.lasts    =0;
    afsk12.sphase   =0;
    afsk12.subsamp  =0;

    f=0.;
    for (i = 0; i < d_corrlen; i++) {
        f = 2.0*M_PI*FREQ_MARK*i/d_sample_rate;
        corr_mark_i[i] = cosf(f);
        corr_mark_q[i] = sinf(f);
    }
    f=0.;
    for (i = 0; i < d_corrlen; i++) {
        f = 2.0*M_PI*FREQ_SPACE*i/d_sample_rate;
        corr_space_i[i] = cosf(f);
        corr_space_q[i] = sinf(f);
    }

}

/*
     * Our virtual destructor.
     */
afsk1200_raw_rx_impl::~afsk1200_raw_rx_impl()
{
    delete corr_mark_i;
    delete corr_mark_q;
    delete corr_space_i;
    delete corr_space_q;
}

int afsk1200_raw_rx_impl::general_work (int noutput_items,
                             gr_vector_int &ninput_items,
                             gr_vector_const_void_star &input_items,
                             gr_vector_void_star &output_items)
{
    float f;
    unsigned char curbit;
    int length;

    const float *in = (const float *) input_items[0];
    char *out = (char *) output_items[0];
    // Do <+signal processing+>
    length=noutput_items;
    int d_numchars=0;
    if (afsk12.subsamp) {
        int numfill = SUBSAMP - afsk12.subsamp;
        if (length < numfill) {
            afsk12.subsamp += length;
            return length;
        }
        in += numfill;
        length -= numfill;
        afsk12.subsamp = 0;
    }
    for (; length >= SUBSAMP; length -= SUBSAMP) {
        in += SUBSAMP;
        f = fsqr(mac(in, corr_mark_i, d_corrlen)) +
                fsqr(mac(in, corr_mark_q, d_corrlen)) -
                fsqr(mac(in, corr_space_i,d_corrlen)) -
                fsqr(mac(in, corr_space_q,d_corrlen));
        afsk12.dcd_shreg <<= 1;
        afsk12.dcd_shreg |= (f > 0);
        /*
             * check if transition
             */
        if ((afsk12.dcd_shreg ^ (afsk12.dcd_shreg >> 1)) & 1) {
            if (afsk12.sphase < (0x8000u-(d_sphaseinc/2)))
                afsk12.sphase += d_sphaseinc/8;
            else
                afsk12.sphase -= d_sphaseinc/8;
        }
        afsk12.sphase += d_sphaseinc;
        if (afsk12.sphase >= 0x10000u) {
            afsk12.sphase &= 0xffffu;
            afsk12.lasts <<= 1;
            afsk12.lasts |= afsk12.dcd_shreg & 1;
            *out++ = (afsk12.lasts ^ (afsk12.lasts >> 1) ^ 1) & 1;
            d_numchars++;
        }
    }
    afsk12.subsamp = length;
    // Tell runtime system how many output items we produced.
    consume_each (noutput_items);
    return d_numchars;

}

  } /* namespace afsk */
} /* namespace gr */

