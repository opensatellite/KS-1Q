#
# FSK modulation and demodulation.  
#
#
# Copyright 2016 KeChuang Space Academy.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr
from gnuradio import analog
from gnuradio import blocks
import modulation_utils
import digital_swig as digital
from math import pi
import numpy
from pprint import pprint
import inspect

# default values (used in __init__ and add_options)
_def_samples_per_symbol = 2
_def_sensitivity = 1
_def_verbose = False
_def_log = False


# FIXME: Figure out how to make GFSK work with pfb_arb_resampler_fff for both
# transmit and receive so we don't require integer samples per symbol.


# /////////////////////////////////////////////////////////////////////////////
#                              GFSK modulator
# /////////////////////////////////////////////////////////////////////////////

class gfsk_mod(gr.hier_block2):

    def __init__(self,
                 samples_per_symbol=_def_samples_per_symbol,
                 sensitivity=_def_sensitivity,
                 verbose=_def_verbose,
                 log=_def_log):
        """
	Hierarchical block for Gaussian Frequency Shift Key (GFSK)
	modulation.

	The input is a byte stream (unsigned char) and the
	output is the complex modulated signal at baseband.

        Args:
            samples_per_symbol: samples per baud >= 2 (integer)
            verbose: Print information about modulator? (bool)
            debug: Print modualtion data to files? (bool)
	"""

	gr.hier_block2.__init__(self, "gfsk_mod",
				gr.io_signature(1, 1, gr.sizeof_char),       # Input signature
				gr.io_signature(1, 1, gr.sizeof_gr_complex)) # Output signature

        samples_per_symbol = int(samples_per_symbol)
        self._samples_per_symbol = samples_per_symbol
        self._differential = False

        if not isinstance(samples_per_symbol, int) or samples_per_symbol < 2:
            raise TypeError, ("samples_per_symbol must be an integer >= 2, is %r" % (samples_per_symbol,))

	#sensitivity = (pi / 2) / samples_per_symbol	# phase change per bit = pi / 2

	# Turn it into NRZ data.
	#self.nrz = digital.bytes_to_syms()
        self.unpack = blocks.packed_to_unpacked_bb(1, gr.GR_MSB_FIRST)
        self.nrz = digital.chunks_to_symbols_bf([-1, 1])
        
	# FM modulation
	self.fmmod = analog.frequency_modulator_fc(sensitivity)

	# small amount of output attenuation to prevent clipping USRP sink
	self.amp = blocks.multiply_const_cc(0.999)
		
        if verbose:
            self._print_verbage()
         
        if log:
            self._setup_logging()

	# Connect & Initialize base class
	self.connect(self, self.unpack, self.nrz, self.fmmod, self.amp, self)

    def samples_per_symbol(self):
        return self._samples_per_symbol

    def bits_per_symbol(self=None):     # staticmethod that's also callable on an instance
        return 1
    bits_per_symbol = staticmethod(bits_per_symbol)      # make it a static method.


    def _print_verbage(self):
        print "bits per symbol = %d" % self.bits_per_symbol()
        print "Gaussian filter bt = %.2f" % self._bt


    def _setup_logging(self):
        print "Modulation logging turned on."
        self.connect(self.nrz,
                     blocks.file_sink(gr.sizeof_float, "nrz.dat"))
        self.connect(self.gaussian_filter,
                     blocks.file_sink(gr.sizeof_float, "gaussian_filter.dat"))
        self.connect(self.fmmod,
                     blocks.file_sink(gr.sizeof_gr_complex, "fmmod.dat"))


    def add_options(parser):
        """
        Adds GFSK modulation-specific options to the standard parser
        """
        parser.add_option("", "--bt", type="float", default=_def_bt,
                          help="set bandwidth-time product [default=%default] (GFSK)")
    add_options=staticmethod(add_options)


    def extract_kwargs_from_options(options):
        """
        Given command line options, create dictionary suitable for passing to __init__
        """
        return modulation_utils.extract_kwargs_from_options(gfsk_mod.__init__,
                                                            ('self',), options)
    extract_kwargs_from_options=staticmethod(extract_kwargs_from_options)


