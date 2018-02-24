#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Top Block
# Generated: Mon Jun 12 20:59:31 2017
##################################################

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print "Warning: failed to XInitThreads()"

from PyQt4 import Qt
from gnuradio import blocks
from gnuradio import digital
from gnuradio import eng_notation
from gnuradio import filter
from gnuradio import gr
from gnuradio import qtgui
from gnuradio import uhd
from gnuradio.eng_option import eng_option
from gnuradio.filter import firdes
from gnuradio.qtgui import Range, RangeWidget
from optparse import OptionParser
import gpredict
import kcsa
import math
import numpy as np
import sip
import sys
import time
from gnuradio import qtgui


class top_block(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "Top Block")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Top Block")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.restoreGeometry(self.settings.value("geometry").toByteArray())

        ##################################################
        # Variables
        ##################################################
        self.down_samprate = down_samprate = 120e3
        self.down_bw = down_bw = 20e3
        self.up_symbolrate = up_symbolrate = 10e3

        self.lpf_taps = lpf_taps = firdes.low_pass(1.0, down_samprate, down_bw/2, down_bw/8, firdes.WIN_HAMMING, 6.76)

        self.vfo = vfo = 30e3
        self.up_samprate = up_samprate = down_samprate
        self.up_mod_index = up_mod_index = 2
        self.up_gain = up_gain = 60
        self.up_freq = up_freq = 436.5e6
        self.up_bw = up_bw = up_symbolrate*4
        self.sc_id = sc_id = 1
        self.lpf_ntaps = lpf_ntaps = len(lpf_taps)
        self.fcorr = fcorr = 0
        self.down_symbolrate = down_symbolrate = 20e3
        self.down_gain = down_gain = 23
        self.down_freq = down_freq = 436.5e6
        self.down_decim = down_decim = 3

        ##################################################
        # Blocks
        ##################################################
        self._fcorr_range = Range(-15e3, 15e3, 100, 0, 200)
        self._fcorr_win = RangeWidget(self._fcorr_range, self.set_fcorr, "fcorr", "counter_slider", float)
        self.top_layout.addWidget(self._fcorr_win)
        self.uhd_usrp_source_0 = uhd.usrp_source(
        	",".join(("", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_source_0.set_samp_rate(down_samprate)
        self.uhd_usrp_source_0.set_center_freq(down_freq-vfo, 0)
        self.uhd_usrp_source_0.set_gain(down_gain, 0)
        self.uhd_usrp_source_0.set_antenna('RX2', 0)
        self.uhd_usrp_source_0.set_bandwidth(down_samprate/2, 0)
        self.uhd_usrp_sink_0 = uhd.usrp_sink(
        	",".join(("", "")),
        	uhd.stream_args(
        		cpu_format="fc32",
        		channels=range(1),
        	),
        )
        self.uhd_usrp_sink_0.set_samp_rate(up_samprate)
        self.uhd_usrp_sink_0.set_center_freq(up_freq+fcorr, 0)
        self.uhd_usrp_sink_0.set_gain(up_gain, 0)
        self.uhd_usrp_sink_0.set_antenna('TX/RX', 0)
        self.uhd_usrp_sink_0.set_bandwidth(up_samprate/2, 0)
        self.qtgui_waterfall_sink_x_0 = qtgui.waterfall_sink_c(
        	1024, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	down_freq-vfo, #fc
        	down_samprate, #bw
        	"", #name
                1 #number of inputs
        )
        self.qtgui_waterfall_sink_x_0.set_update_time(0.1)
        self.qtgui_waterfall_sink_x_0.enable_grid(False)
        self.qtgui_waterfall_sink_x_0.enable_axis_labels(True)

        if not True:
          self.qtgui_waterfall_sink_x_0.disable_legend()

        if "complex" == "float" or "complex" == "msg_float":
          self.qtgui_waterfall_sink_x_0.set_plot_pos_half(not True)

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        colors = [0, 0, 0, 0, 0,
                  0, 0, 0, 0, 0]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_waterfall_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_waterfall_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_waterfall_sink_x_0.set_color_map(i, colors[i])
            self.qtgui_waterfall_sink_x_0.set_line_alpha(i, alphas[i])

        self.qtgui_waterfall_sink_x_0.set_intensity_range(-140, -60)

        self._qtgui_waterfall_sink_x_0_win = sip.wrapinstance(self.qtgui_waterfall_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_waterfall_sink_x_0_win)
        self.qtgui_time_sink_x_0 = qtgui.time_sink_f(
        	1024, #size
        	down_samprate/1024, #samp_rate
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_time_sink_x_0.set_update_time(0.10)
        self.qtgui_time_sink_x_0.set_y_axis(-1, 1)

        self.qtgui_time_sink_x_0.set_y_label('Amplitude', "")

        self.qtgui_time_sink_x_0.enable_tags(-1, True)
        self.qtgui_time_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, qtgui.TRIG_SLOPE_POS, 0.0, 0, 0, "")
        self.qtgui_time_sink_x_0.enable_autoscale(False)
        self.qtgui_time_sink_x_0.enable_grid(False)
        self.qtgui_time_sink_x_0.enable_axis_labels(True)
        self.qtgui_time_sink_x_0.enable_control_panel(False)

        if not True:
          self.qtgui_time_sink_x_0.disable_legend()

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "blue"]
        styles = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        markers = [-1, -1, -1, -1, -1,
                   -1, -1, -1, -1, -1]
        alphas = [1.0, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]

        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_time_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_time_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_time_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_time_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_time_sink_x_0.set_line_style(i, styles[i])
            self.qtgui_time_sink_x_0.set_line_marker(i, markers[i])
            self.qtgui_time_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_time_sink_x_0_win = sip.wrapinstance(self.qtgui_time_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_time_sink_x_0_win)
        self.qtgui_freq_sink_x_0 = qtgui.freq_sink_c(
        	4096, #size
        	firdes.WIN_BLACKMAN_hARRIS, #wintype
        	0, #fc
        	down_samprate/down_decim, #bw
        	"", #name
        	1 #number of inputs
        )
        self.qtgui_freq_sink_x_0.set_update_time(0.10)
        self.qtgui_freq_sink_x_0.set_y_axis(-140, -40)
        self.qtgui_freq_sink_x_0.set_y_label('Relative Gain', 'dB')
        self.qtgui_freq_sink_x_0.set_trigger_mode(qtgui.TRIG_MODE_FREE, 0.0, 0, "")
        self.qtgui_freq_sink_x_0.enable_autoscale(False)
        self.qtgui_freq_sink_x_0.enable_grid(True)
        self.qtgui_freq_sink_x_0.set_fft_average(1.0)
        self.qtgui_freq_sink_x_0.enable_axis_labels(True)
        self.qtgui_freq_sink_x_0.enable_control_panel(False)

        if not False:
          self.qtgui_freq_sink_x_0.disable_legend()

        if "complex" == "float" or "complex" == "msg_float":
          self.qtgui_freq_sink_x_0.set_plot_pos_half(not True)

        labels = ['', '', '', '', '',
                  '', '', '', '', '']
        widths = [1, 1, 1, 1, 1,
                  1, 1, 1, 1, 1]
        colors = ["blue", "red", "green", "black", "cyan",
                  "magenta", "yellow", "dark red", "dark green", "dark blue"]
        alphas = [0.3, 1.0, 1.0, 1.0, 1.0,
                  1.0, 1.0, 1.0, 1.0, 1.0]
        for i in xrange(1):
            if len(labels[i]) == 0:
                self.qtgui_freq_sink_x_0.set_line_label(i, "Data {0}".format(i))
            else:
                self.qtgui_freq_sink_x_0.set_line_label(i, labels[i])
            self.qtgui_freq_sink_x_0.set_line_width(i, widths[i])
            self.qtgui_freq_sink_x_0.set_line_color(i, colors[i])
            self.qtgui_freq_sink_x_0.set_line_alpha(i, alphas[i])

        self._qtgui_freq_sink_x_0_win = sip.wrapinstance(self.qtgui_freq_sink_x_0.pyqwidget(), Qt.QWidget)
        self.top_layout.addWidget(self._qtgui_freq_sink_x_0_win)
        self.kcsa_vitfilt27_bb_0_0 = kcsa.vitfilt27_bb()
        self.kcsa_vitfilt27_bb_0 = kcsa.vitfilt27_bb()
        self.kcsa_kiss_server_0 = kcsa.kiss_server('127.0.0.1', 2610, 1024, 25, 10)
        self.kcsa_halfduplex_tc_0 = kcsa.halfduplex_tc(1, 0, True, 128, 4)
        self.kcsa_fec_decode_b_0_0_0 = kcsa.fec_decode_b(223, False, False, True, False)
        self.kcsa_fec_decode_b_0_0 = kcsa.fec_decode_b(223, False, False, True, False)
        self.gpredict_doppler_0 = gpredict.doppler(self.set_fcorr, "localhost", 4532, True)
        self.freq_xlating_fir_filter_xxx_0 = filter.freq_xlating_fir_filter_ccc(down_decim, (lpf_taps), vfo+fcorr, down_samprate)
        self.fir_filter_xxx_0 = filter.fir_filter_fff(1024, (np.ones(1024)/1024))
        self.fir_filter_xxx_0.declare_sample_delay(0)
        self.digital_gmsk_demod_0 = digital.gmsk_demod(
        	samples_per_symbol=int(down_samprate/down_decim/down_symbolrate),
        	gain_mu=0.175,
        	mu=0.5,
        	omega_relative_limit=0.02,
        	freq_error=0.01,
        	verbose=False,
        	log=False,
        )
        self.digital_gfsk_mod_0 = digital.gfsk_mod(
        	samples_per_symbol=int(up_samprate/up_symbolrate),
        	sensitivity=math.pi*up_mod_index/int(up_samprate/up_symbolrate),
        	bt=0.5,
        	verbose=False,
        	log=False,
        )
        self.blocks_tag_debug_0 = blocks.tag_debug(gr.sizeof_gr_complex*1, '', ""); self.blocks_tag_debug_0.set_display(True)
        self.blocks_rms_xx_0 = blocks.rms_cf(0.0001)
        self.blocks_nlog10_ff_0 = blocks.nlog10_ff(10, 1, 0)
        self.blocks_delay_0 = blocks.delay(gr.sizeof_char*1, 1)

        ##################################################
        # Connections
        ##################################################
        self.msg_connect((self.kcsa_fec_decode_b_0_0, 'out'), (self.kcsa_kiss_server_0, 'in'))
        self.msg_connect((self.kcsa_fec_decode_b_0_0_0, 'out'), (self.kcsa_kiss_server_0, 'in'))
        self.msg_connect((self.kcsa_kiss_server_0, 'out'), (self.kcsa_halfduplex_tc_0, 'pdu'))
        self.connect((self.blocks_delay_0, 0), (self.kcsa_vitfilt27_bb_0, 0))
        self.connect((self.blocks_nlog10_ff_0, 0), (self.fir_filter_xxx_0, 0))
        self.connect((self.blocks_rms_xx_0, 0), (self.blocks_nlog10_ff_0, 0))
        self.connect((self.digital_gfsk_mod_0, 0), (self.blocks_tag_debug_0, 0))
        self.connect((self.digital_gfsk_mod_0, 0), (self.uhd_usrp_sink_0, 0))
        self.connect((self.digital_gmsk_demod_0, 0), (self.blocks_delay_0, 0))
        self.connect((self.digital_gmsk_demod_0, 0), (self.kcsa_vitfilt27_bb_0_0, 0))
        self.connect((self.fir_filter_xxx_0, 0), (self.qtgui_time_sink_x_0, 0))
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.blocks_rms_xx_0, 0))
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.digital_gmsk_demod_0, 0))
        self.connect((self.freq_xlating_fir_filter_xxx_0, 0), (self.qtgui_freq_sink_x_0, 0))
        self.connect((self.kcsa_halfduplex_tc_0, 0), (self.digital_gfsk_mod_0, 0))
        self.connect((self.kcsa_vitfilt27_bb_0, 0), (self.kcsa_fec_decode_b_0_0, 0))
        self.connect((self.kcsa_vitfilt27_bb_0_0, 0), (self.kcsa_fec_decode_b_0_0_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.freq_xlating_fir_filter_xxx_0, 0))
        self.connect((self.uhd_usrp_source_0, 0), (self.qtgui_waterfall_sink_x_0, 0))

    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "top_block")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_down_samprate(self):
        return self.down_samprate

    def set_down_samprate(self, down_samprate):
        self.down_samprate = down_samprate
        self.set_up_samprate(self.down_samprate)
        self.uhd_usrp_source_0.set_samp_rate(self.down_samprate)
        self.uhd_usrp_source_0.set_bandwidth(self.down_samprate/2, 0)
        self.qtgui_waterfall_sink_x_0.set_frequency_range(self.down_freq-self.vfo, self.down_samprate)
        self.qtgui_time_sink_x_0.set_samp_rate(self.down_samprate/1024)
        self.qtgui_freq_sink_x_0.set_frequency_range(0, self.down_samprate/self.down_decim)

    def get_down_bw(self):
        return self.down_bw

    def set_down_bw(self, down_bw):
        self.down_bw = down_bw

    def get_up_symbolrate(self):
        return self.up_symbolrate

    def set_up_symbolrate(self, up_symbolrate):
        self.up_symbolrate = up_symbolrate
        self.set_up_bw(self.up_symbolrate*4)

    def get_lpf_taps(self):
        return self.lpf_taps

    def set_lpf_taps(self, lpf_taps):
        self.lpf_taps = lpf_taps
        self.set_lpf_ntaps(len(self.lpf_taps))
        self.freq_xlating_fir_filter_xxx_0.set_taps((self.lpf_taps))

    def get_vfo(self):
        return self.vfo

    def set_vfo(self, vfo):
        self.vfo = vfo
        self.uhd_usrp_source_0.set_center_freq(self.down_freq-self.vfo, 0)
        self.qtgui_waterfall_sink_x_0.set_frequency_range(self.down_freq-self.vfo, self.down_samprate)
        self.freq_xlating_fir_filter_xxx_0.set_center_freq(self.vfo+self.fcorr)

    def get_up_samprate(self):
        return self.up_samprate

    def set_up_samprate(self, up_samprate):
        self.up_samprate = up_samprate
        self.uhd_usrp_sink_0.set_samp_rate(self.up_samprate)
        self.uhd_usrp_sink_0.set_bandwidth(self.up_samprate/2, 0)

    def get_up_mod_index(self):
        return self.up_mod_index

    def set_up_mod_index(self, up_mod_index):
        self.up_mod_index = up_mod_index

    def get_up_gain(self):
        return self.up_gain

    def set_up_gain(self, up_gain):
        self.up_gain = up_gain
        self.uhd_usrp_sink_0.set_gain(self.up_gain, 0)


    def get_up_freq(self):
        return self.up_freq

    def set_up_freq(self, up_freq):
        self.up_freq = up_freq
        self.uhd_usrp_sink_0.set_center_freq(self.up_freq+self.fcorr, 0)

    def get_up_bw(self):
        return self.up_bw

    def set_up_bw(self, up_bw):
        self.up_bw = up_bw

    def get_sc_id(self):
        return self.sc_id

    def set_sc_id(self, sc_id):
        self.sc_id = sc_id

    def get_lpf_ntaps(self):
        return self.lpf_ntaps

    def set_lpf_ntaps(self, lpf_ntaps):
        self.lpf_ntaps = lpf_ntaps

    def get_fcorr(self):
        return self.fcorr

    def set_fcorr(self, fcorr):
        self.fcorr = fcorr
        self.uhd_usrp_sink_0.set_center_freq(self.up_freq+self.fcorr, 0)
        self.freq_xlating_fir_filter_xxx_0.set_center_freq(self.vfo+self.fcorr)

    def get_down_symbolrate(self):
        return self.down_symbolrate

    def set_down_symbolrate(self, down_symbolrate):
        self.down_symbolrate = down_symbolrate

    def get_down_gain(self):
        return self.down_gain

    def set_down_gain(self, down_gain):
        self.down_gain = down_gain
        self.uhd_usrp_source_0.set_gain(self.down_gain, 0)


    def get_down_freq(self):
        return self.down_freq

    def set_down_freq(self, down_freq):
        self.down_freq = down_freq
        self.uhd_usrp_source_0.set_center_freq(self.down_freq-self.vfo, 0)
        self.qtgui_waterfall_sink_x_0.set_frequency_range(self.down_freq-self.vfo, self.down_samprate)

    def get_down_decim(self):
        return self.down_decim

    def set_down_decim(self, down_decim):
        self.down_decim = down_decim
        self.qtgui_freq_sink_x_0.set_frequency_range(0, self.down_samprate/self.down_decim)


def main(top_block_cls=top_block, options=None):

    from distutils.version import StrictVersion
    if StrictVersion(Qt.qVersion()) >= StrictVersion("4.5.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()
    tb.start()
    tb.show()

    def quitting():
        tb.stop()
        tb.wait()
    qapp.connect(qapp, Qt.SIGNAL("aboutToQuit()"), quitting)
    qapp.exec_()


if __name__ == '__main__':
    main()
