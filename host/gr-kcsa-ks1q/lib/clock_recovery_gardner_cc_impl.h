/* -*- c++ -*- */
/*
 * Copyright 2016 KeChuang Space Academy <sa@kc.ac.cn>
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

#ifndef INCLUDED_KCSA_CLOCK_RECOVERY_GARDNER_CC_IMPL_H
#define	INCLUDED_KCSA_CLOCK_RECOVERY_GARDNER_CC_IMPL_H

#include <kcsa/clock_recovery_gardner_cc.h>
#include <gnuradio/filter/mmse_fir_interpolator_cc.h>

namespace gr {
  namespace kcsa {

    class clock_recovery_gardner_cc_impl : public clock_recovery_gardner_cc
    {
    public:
      clock_recovery_gardner_cc_impl(float omega, float gain_omega,
				float mu, float gain_mu,
				float omega_relative_limi);
      ~clock_recovery_gardner_cc_impl();

      void forecast(int noutput_items, gr_vector_int &ninput_items_required);
      int general_work(int noutput_items,
		       gr_vector_int &ninput_items,
		       gr_vector_const_void_star &input_items,
		       gr_vector_void_star &output_items);

      float mu() const { return d_mu;}
      float omega() const { return d_omega;}
      float gain_mu() const { return d_gain_mu;}
      float gain_omega() const { return d_gain_omega;}

      void set_verbose (bool verbose) { d_verbose = verbose; }
      void set_gain_mu (float gain_mu) { d_gain_mu = gain_mu; }
      void set_gain_omega (float gain_omega) { d_gain_omega = gain_omega; }
      void set_mu (float mu) { d_mu = mu; }
      void set_omega (float omega);

    private:
      float d_mu;                   // fractional sample position [0.0, 1.0]
      float d_gain_mu;              // gain for adjusting mu
      float d_omega;                // nominal frequency
      float d_gain_omega;           // gain for adjusting omega
      float d_omega_relative_limit; // used to compute min and max omega
      float d_omega_mid;            // average omega
      float d_omega_lim;            // actual omega clipping limit

	  bool d_last_mid;
	  gr_complex d_mid_sample;
      gr_complex d_last_sample;
      filter::mmse_fir_interpolator_cc *d_interp;

	  gr_complex interp(const gr_complex input[], float mu);

      bool d_verbose;
      bool d_log;

      int fd;
    };

  } /* namespace kcsa */
} /* namespace gr */

#endif /* INCLUDED_KCSA_CLOCK_RECOVERY_GARDNER_CC_IMPL_H */
