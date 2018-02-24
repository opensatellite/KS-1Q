#!/usr/bin/env python
#
# Copyright 2011-2013 Free Software Foundation, Inc.
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
#

import random
import cmath

from gnuradio import gr, gr_unittest, blocks
import kcsa

class test_clock_recovery_gardner(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test02(self):
        # Test float/float version
        omega = 2
        gain_omega = 0.01
        mu = 0.5
        gain_mu = 0.01
        omega_rel_lim = 0.001

        self.test = kcsa.clock_recovery_gardner_ff(omega, gain_omega,
                                                 mu, gain_mu,
                                                 omega_rel_lim)

        data = 100*[1,]
        self.src = blocks.vector_source_f(data, False)
        self.snk = blocks.vector_sink_f()

        self.tb.connect(self.src, self.test, self.snk)
        self.tb.run()

        expected_result = 100*[0.9997, ] # doesn't quite get to 1.0
        dst_data = self.snk.data()

        # Only compare last Ncmp samples
        Ncmp = 30
        len_e = len(expected_result)
        len_d = len(dst_data)
        expected_result = expected_result[len_e - Ncmp:]
        dst_data = dst_data[len_d - Ncmp:]

        #print expected_result
        #print dst_data

        self.assertFloatTuplesAlmostEqual(expected_result, dst_data, 4)

    def test04(self):
        # Test float/float version
        omega = 2
        gain_omega = 0.01
        mu = 0.25
        gain_mu = 0.1
        omega_rel_lim = 0.001

        self.test = kcsa.clock_recovery_gardner_ff(omega, gain_omega,
                                                 mu, gain_mu,
                                                 omega_rel_lim)

        data = 1000*[1, 1, -1, -1]
        self.src = blocks.vector_source_f(data, False)
        self.snk = blocks.vector_sink_f()

        self.tb.connect(self.src, self.test, self.snk)
        self.tb.run()

        expected_result = 1000*[-1.2, 1.2]
        dst_data = self.snk.data()

        # Only compare last Ncmp samples
        Ncmp = 100
        len_e = len(expected_result)
        len_d = len(dst_data)
        expected_result = expected_result[len_e - Ncmp:]
        dst_data = dst_data[len_d - Ncmp:]

        #print expected_result
        #print dst_data

        self.assertFloatTuplesAlmostEqual(expected_result, dst_data, 1)


if __name__ == '__main__':
    gr_unittest.run(test_clock_recovery_gardner, "test_clock_recovery_gardner.xml")
