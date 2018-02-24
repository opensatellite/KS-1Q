#
# GMSK modulation and demodulation.  
#
#
# Copyright 2005,2006,2007 Free Software Foundation, Inc.
# 
# This file is part of GNU Radio
# 
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.


from gnuradio import gr
from gnuradio.blocks import *
from gnuradio.analog import *
from gnuradio.digital import *
from gnuradio.filter import *
#from gnuradio import utils
from math import pi

# default values (used in __init__ and add_options)
_def_samples_per_symbol = 2
_def_verbose = False
_def_log = False

_def_gain_mu = None
_def_mu = 0.5
_def_freq_error = 0.0
_def_omega_relative_limit = 0.005

# /////////////////////////////////////////////////////////////////////////////
#                            GMSK demodulator
# /////////////////////////////////////////////////////////////////////////////

class gmsk_demod(gr.hier_block2):
    """
    Hierarchical block for Gaussian Minimum Shift Key (GMSK)
    demodulation.
    
    The input is the complex modulated signal at baseband.
    The output is a stream of bits packed 1 bit per byte (the LSB)
    
    Args:
        samples_per_symbol: samples per baud (integer)
        gain_mu: controls rate of mu adjustment (float)
        mu: fractional delay [0.0, 1.0] (float)
        omega_relative_limit: sets max variation in omega (float)
        freq_error: bit rate error as a fraction (float)
        verbose: Print information about modulator? (boolean)
        log: Print modualtion data to files? (boolean)
    """
    
    def __init__(self,
                 samples_per_symbol=_def_samples_per_symbol,
                 gain_mu=_def_gain_mu,
                 mu=_def_mu,
                 omega_relative_limit=_def_omega_relative_limit,
                 freq_error=_def_freq_error,
                 verbose=_def_verbose,
                 log=_def_log,
                 log_fn_prefix='',
                 sym_per_sec=1e6,
                 pre_cr_filt_bt=1.0,
                 pre_cr_filt_tr=0.1,
                 decision_threshold=0.1,
                 nrz_output_enabled=False):
       
        
        gr.hier_block2.__init__(self, "gmsk_demod",
                gr.io_signature(1, 1, gr.sizeof_gr_complex), # Input signature
                gr.io_signature(1, 1, gr.sizeof_char))       # Output signature


        self._samples_per_symbol = samples_per_symbol
        self._gain_mu = gain_mu
        self._mu = mu
        self._omega_relative_limit = omega_relative_limit
        self._freq_error = freq_error
        self._log_fn_prefix = log_fn_prefix
        self._decision_threshold = decision_threshold
        
        if samples_per_symbol < 2:
            raise TypeError, "samples_per_symbol >= 2, is %f" % samples_per_symbol

        self._omega = samples_per_symbol*(1+self._freq_error)

        if not self._gain_mu:
            self._gain_mu = 0.175
            
        self._gain_omega = .25 * self._gain_mu * self._gain_mu        # critically damped

        

        self.kc = copy(gr.sizeof_gr_complex)
        self.delay = delay(gr.sizeof_gr_complex, 2*self._samples_per_symbol) #2T delay
        self.conj = conjugate_cc()
        self.mult = multiply_cc() #This is producing our differential phasor
        self.c2mag = complex_to_mag()
        self.safety_add = add_const_ff(0.0000001)
        self.c2f = complex_to_float()
        self.rescaler = divide_ff() #This is used to normalize the signal
        self.sub = add_const_ff(-self._decision_threshold)
        samp_per_sec = samples_per_symbol * sym_per_sec
        pre_cr_filt_bw = sym_per_sec*pre_cr_filt_bt
        pre_cr_filt_taps = firdes.low_pass(1.0, samp_per_sec, pre_cr_filt_bw, pre_cr_filt_tr*samp_per_sec, firdes.WIN_HAMMING)


        self.pre_cr_filt = fir_filter_fff(1, pre_cr_filt_taps)

        # the clock recovery block tracks the symbol clock and resamples as needed.
        # the output of the block is a stream of soft symbols (float)
        self.clock_recovery = clock_recovery_mm_ff(self._omega, self._gain_omega,
                                                      self._mu, self._gain_mu,
                                                      self._omega_relative_limit)

        # slice the floats at 0, outputting 1 bit (the LSB of the output byte) per sample
        self.slicer = binary_slicer_fb()

        if verbose:
            self._print_verbage()
         
        if log:
            self._setup_logging()

        # Connect & Initialize base class
        self.connect(self, self.kc, self.delay, self.conj, (self.mult, 0))
        self.connect(self.kc, (self.mult, 1))
        self.connect(self.mult, self.c2f, (self.rescaler, 0))
        self.connect(self.mult, self.c2mag, self.safety_add, (self.rescaler, 1))
        self.connect(self.rescaler, self.pre_cr_filt, self.sub,
                self.clock_recovery, self.slicer, self)

    def samples_per_symbol(self):
        return self._samples_per_symbol

    def bits_per_symbol(self=None):   # staticmethod that's also callable on an instance
        return 1
    bits_per_symbol = staticmethod(bits_per_symbol)      # make it a static method.


    def _print_verbage(self):
        print "bits per symbol = %d" % self.bits_per_symbol()
        print "M&M clock recovery omega = %f" % self._omega
        print "M&M clock recovery gain mu = %f" % self._gain_mu
        print "M&M clock recovery mu = %f" % self._mu
        print "M&M clock recovery omega rel. limit = %f" % self._omega_relative_limit
        print "frequency error = %f" % self._freq_error


    def _setup_logging(self):
        print "Demodulation logging turned on."
        self.connect(self.mult,
                    gr.file_sink(gr.sizeof_gr_complex, self._log_fn_prefix+"mult_output.dat"))
        self.connect(self.rescaler,
                    gr.file_sink(gr.sizeof_float, self._log_fn_prefix+"rescaler_output.dat"))
        self.connect((self.c2f, 0),
                    gr.file_sink(gr.sizeof_float, self._log_fn_prefix+"rescaler_input.dat"))
        self.connect(self.pre_cr_filt,
                    gr.file_sink(gr.sizeof_float, self._log_fn_prefix+"pre_cr_filt_output.dat"))
        self.connect(self.sub,
                    gr.file_sink(gr.sizeof_float, self._log_fn_prefix+"cr_input.dat"))
        self.connect(self.clock_recovery,
                    gr.file_sink(gr.sizeof_float, self._log_fn_prefix+"cr_output.dat"))
        self.connect(self.slicer,
                    gr.file_sink(gr.sizeof_char, self._log_fn_prefix+"slicer.dat"))

    def add_options(parser):
        """
        Adds GMSK demodulation-specific options to the standard parser
        """
        parser.add_option("", "--gain-mu", type="float", default=_def_gain_mu,
                          help="M&M clock recovery gain mu [default=%default] (GMSK/PSK)")
        parser.add_option("", "--mu", type="float", default=_def_mu,
                          help="M&M clock recovery mu [default=%default] (GMSK/PSK)")
        parser.add_option("", "--omega-relative-limit", type="float", default=_def_omega_relative_limit,
                          help="M&M clock recovery omega relative limit [default=%default] (GMSK/PSK)")
        parser.add_option("", "--freq-error", type="float", default=_def_freq_error,
                          help="M&M clock recovery frequency error [default=%default] (GMSK)")
    add_options=staticmethod(add_options)


