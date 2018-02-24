#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2017 <+YOU OR YOUR COMPANY+>.
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

from gnuradio import gr, gr_unittest
from gnuradio import blocks
import kcsa_swig as kcsa

class qa_framesync (gr_unittest.TestCase):

    def setUp (self):
        self.dut = dut = kcsa.framesync(1, False, 0, "0xfedcba98765432100123456789abcdeffedcba98765432100123456789abcdef", 256, 0, 1024)
        self.dut2 = dut2 = kcsa.framesync(1, False, 0, "0xfedcba98765432100123456789abcdef", 128, 0, 1024)
        self.dut3 = dut3 = kcsa.framesync(1, False, 0, "0x0123456789abcdef", 64, 0, 1024)
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        # check data
        self.tb.run ()

if __name__ == '__main__':
    gr_unittest.run(qa_framesync, "qa_framesync.xml")
 
