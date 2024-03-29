#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2023 University of Toronto Aerospace Team.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
from gnuradio import blocks
try:
  from gnuradio.UTAT_HERON import heron_rx_bb
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio.UTAT_HERON import heron_rx_bb

# ====================================================
# run tests individually to get proper debug file output.
# disable test by adding underscore in front of name of
# the function that starts with the word "test".
# ====================================================

class qa_heron_rx_bb(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_instance(self):
        instance = heron_rx_bb()

    def test_001_single_byte(self):
        samp_rate = 1e6
        input = [
            0xAA,0xAA,0xAA,0xAA,0xAA,0x7E,1,0x69,0xD3,0xB1
        ]
        expected = [
            0x69, 0x0D
        ]
        throttle = blocks.throttle(gr.sizeof_char*1, samp_rate, True)
        src = blocks.vector_source_b(input, False, 1, [])
        unpack = blocks.unpack_k_bits_bb(8)
        rx = heron_rx_bb()
        rx.set_max_noutput_items(len(expected))
        dst = blocks.vector_sink_b()
        self.tb.connect(src, unpack)
        self.tb.connect(unpack, rx)
        self.tb.connect(rx, dst)
        self.tb.run()
        output = dst.data()
        self.assertEqual(expected, output)
    
    def test_002_multiple_bytes(self):
        samp_rate = 1e6
        input = [
            0xAB,0xAB,0xBA,0xFA,0x24,
            0xAB,0xAB,0xBA,0xFA,0x24,
            0xAB,0xAB,0xBA,0xFA,0x24,
            0xAA,0xAA,0xAA,0xAA,0xAA,0x7E,1,0x01,0x3E,0x1F,
            0xAB,0xAB,0xBA,0xFA,0x24,
            0xAA,0xAA,0xAA,0xAA,0xAA,0x7E,3,0x61,0x62,0x63,0x82,0xCA,
            0xAB,0xAB,0xBA,0xFA,0x24,
            0xAA,0xAA,0xAA,0xAA,0xAA,0x7E,6,0x69,0x69,0x69,0x69,0x69,0x69,0x15,0xD3,
            0xAA,0xAA,0xAA,0xAA,0xAA,0x7E,6,0x69,0x69,0x69,0x69,0x69,0x69,0x15,0xD3,
        ]
        expected = [
            0x01,0x0D,
            0x61,0x62,0x63,0x0D,
            0x69,0x69,0x69,0x69,0x69,0x69,0x0D,
            0x69,0x69,0x69,0x69,0x69,0x69,0x0D
        ]
        throttle = blocks.throttle(gr.sizeof_char*1, samp_rate, True)
        src = blocks.vector_source_b(input, False, 1, [])
        unpack = blocks.unpack_k_bits_bb(8)
        rx = heron_rx_bb()
        rx.set_max_noutput_items(len(expected))
        dst = blocks.vector_sink_b()
        self.tb.connect(src, unpack)
        self.tb.connect(unpack, rx)
        self.tb.connect(rx, dst)
        self.tb.run()
        output = dst.data()
        self.assertEqual(expected, output)

    def test_003_timeout(self):
        samp_rate = 1e6
        input = [
            0xAA,0xAA,0xAA,0xAA,0xAA,0x7E,6,0x69,0x69,0x69,0x69,0x69,0x69,0x15,0xD3,
            0xAA,0xAA,0xAA,0xAA,0xAA,
            0xAA,0xAA,0xAA,0xAA,0xAA,0x7E,6,0x69,0x69,0x69,0x69,0x69,0x69,0x15,0xD3, # should timeout
            0xAA,0xAA,0xAA,0xAA,0xAA,
            0xAB,0xAB,0xBA,0xFA,0x24, # should timeout
        ]
        expected = [
            0x69,0x69,0x69,0x69,0x69,0x69,0x0D
        ]
        throttle = blocks.throttle(gr.sizeof_char*1, samp_rate, True)
        src = blocks.vector_source_b(input, False, 1, [])
        unpack = blocks.unpack_k_bits_bb(8)
        rx = heron_rx_bb()
        rx.set_max_noutput_items(len(expected))
        dst = blocks.vector_sink_b()
        self.tb.connect(src, unpack)
        self.tb.connect(unpack, rx)
        self.tb.connect(rx, dst)
        self.tb.run()
        output = dst.data()
        self.assertEqual(expected, output)
    
    def test_004_zero_data(self):
        samp_rate = 1e6
        input = [
            0xAB,0xAB,0xBA,0xFA,0x24,
            0xAA,0xAA,0xAA,0xAA,0xAA,0x7E,1,0x01,0x3E,0x1F,
            0xAB,0xAB,0xBA,0xFA,0x24,
            0xAB,0xAB,0xBA,0xFA,0x24,
            0xAA,0xAA,0xAA,0xAA,0xAA,0x7E,0,0xE1,0xF0

        ]
        expected = [
            0x01,0x0D,
            0x0D
        ]
        throttle = blocks.throttle(gr.sizeof_char*1, samp_rate, True)
        src = blocks.vector_source_b(input, False, 1, [])
        unpack = blocks.unpack_k_bits_bb(8)
        rx = heron_rx_bb()
        rx.set_max_noutput_items(len(expected))
        dst = blocks.vector_sink_b()
        self.tb.connect(src, unpack)
        self.tb.connect(unpack, rx)
        self.tb.connect(rx, dst)
        self.tb.run()
        output = dst.data()
        self.assertEqual(expected, output)

if __name__ == '__main__':
    gr_unittest.run(qa_heron_rx_bb)
