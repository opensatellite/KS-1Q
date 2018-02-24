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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "clock_recovery_gardner_ff_impl.h"
#include <gnuradio/io_signature.h>
#include <gnuradio/math.h>
#include <stdexcept>

//#define LINEAR

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>   
#include <cstdio>  

namespace gr {
  namespace kcsa {

    clock_recovery_gardner_ff::sptr
    clock_recovery_gardner_ff::make(float omega, float gain_omega,
			       float mu, float gain_mu,
			       float omega_relative_limit)
    {
      return gnuradio::get_initial_sptr
	(new clock_recovery_gardner_ff_impl(omega, gain_omega,
				       mu, gain_mu,
				       omega_relative_limit));
    }

    clock_recovery_gardner_ff_impl::clock_recovery_gardner_ff_impl(float omega, float gain_omega,
							 float mu, float gain_mu,
							 float omega_relative_limit)
      : block("clock_recovery_gardner_ff",
              io_signature::make(1, 1, sizeof(float)),
              io_signature::make(1, 1, sizeof(float))),
	d_mu(mu), d_gain_mu(gain_mu), d_gain_omega(gain_omega),
	d_omega_relative_limit(omega_relative_limit),
	d_last_mid(false), d_mid_sample(0), d_last_sample(0), 
	d_interp(new filter::mmse_fir_interpolator_ff())
    {
      if(omega <  1)
	throw std::out_of_range("clock rate must be > 0");
      if(gain_mu <  0  || gain_omega < 0)
	throw std::out_of_range("Gains must be non-negative");

      set_omega(omega);			// also sets min and max omega
      set_relative_rate (1.0 / omega);
      
      d_verbose = false;
      d_log = false;
      
      if(d_log)
      	fd = open("gardner.csv", O_WRONLY | O_CREAT, S_IRWXU | S_IRGRP | S_IROTH);
    }

    clock_recovery_gardner_ff_impl::~clock_recovery_gardner_ff_impl()
    {
      delete d_interp;
      if(d_log)
      	close(fd);
    }

    void
    clock_recovery_gardner_ff_impl::forecast(int noutput_items,
					gr_vector_int &ninput_items_required)
    {
      unsigned ninputs = ninput_items_required.size();
      for(unsigned i=0; i < ninputs; i++) {
		#ifdef LINEAR
		  ninput_items_required[i] = (int) ceil((noutput_items * 2 * d_omega) + 2);
		#else
		  ninput_items_required[i] = (int) ceil((noutput_items * 2 * d_omega) + d_interp->ntaps());
	  	#endif
	  }
    }

    void
    clock_recovery_gardner_ff_impl::set_omega (float omega)
    {
      d_omega = omega;
      d_omega_mid = omega;
      d_omega_lim = d_omega_mid * d_omega_relative_limit;
    }
  
  
	float clock_recovery_gardner_ff_impl::interp (const float input[], float mu)
	{
		//return input[0] * (1.0 - mu) + input[1] * mu;
		return input[1] * mu + input[0] * ( 1.0 - mu );
	}

    int
    clock_recovery_gardner_ff_impl::general_work(int noutput_items,
					    gr_vector_int &ninput_items,
					    gr_vector_const_void_star &input_items,
					    gr_vector_void_star &output_items)
    {
      const float *in = (const float *)input_items[0];
      float *out = (float *)output_items[0];

      int ii = 0; // input index
      int oo = 0; // output index
      #ifdef LINEAR
      int ni = ninput_items[0] - 2;
      #else
      int ni = ninput_items[0] - d_interp->ntaps(); // don't use more input than this
      #endif
      float g_val;
      //static float phase = 0;

      while(oo < noutput_items && ii < ni ) {
      
      if(d_last_mid) {
		// produce output sample
		#ifdef LINEAR
			out[oo] = interp(&in[ii], d_mu);
		#else
			out[oo] = d_interp->interpolate(&in[ii], d_mu);
		#endif
		
		g_val = -(out[oo] - d_last_sample) * d_mid_sample;
		//g_val = (out[oo] - d_last_sample) * d_mid_sample;
		
		if(d_verbose) {
			//std::cout << "phase: " << phase << "\terr: " << g_val << "\tx0: " << out[oo] << "\tx1: " << d_mid_sample << "\tx2: " << d_last_sample << std::endl;
			printf("%f,%f,%f,\n", g_val, out[oo], d_mu);
		}
		
		//std::cout << g_val << std::endl;
		if(d_log) dprintf(fd, "%f,%f,%f,\n", g_val, out[oo], d_mu);
		
		d_last_sample = out[oo];
		
		d_omega = d_omega + d_gain_omega * g_val;
		d_omega = d_omega_mid + gr::branchless_clip(d_omega-d_omega_mid, d_omega_lim);
		d_mu = d_mu + d_omega / 2.0 + d_gain_mu * g_val;
		//phase += d_omega / 2.0 + d_gain_mu * g_val;
		
	    oo++;
	  } else {
		// save mid sample
		#ifdef LINEAR
			d_mid_sample = interp(&in[ii], d_mu);
		#else
			d_mid_sample = d_interp->interpolate(&in[ii], d_mu);
		#endif
		/*
		if(d_verbose) {
			std::cout << "mid at: " << phase << "\tin[ii]: " << in[ii] << std::endl;
		}
		*/
		d_mu = d_mu + d_omega / 2.0;
		//phase += d_omega / 2.0;
	  }
	
	  d_last_mid = !d_last_mid;
	  ii += (int)floor(d_mu);
	  d_mu = d_mu - floor(d_mu);
    }

      consume_each(ii);
      return oo;
    }

  } /* namespace kcsa */
} /* namespace gr */
