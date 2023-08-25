#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2023 University of Toronto Aerospace Team.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

import time
import pmt
from gnuradio import gr, gr_unittest
from gnuradio import blocks
try:
    from gnuradio.UTAT_HERON import esttc_deframer
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio.UTAT_HERON import esttc_deframer

class qa_esttc_deframer(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_instance(self):
        instance = esttc_deframer(32e3)

    def test_001_basic(self):

        samp_rate = 32e3
        input = [0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x7e, 0x02, 0xff, 0xff, 0xbf, 0xf3]
        expected_output = [0xff, 0xff]

        src = blocks.vector_source_b(input, True, 1, [])
        unpack = blocks.unpack_k_bits_bb(8)
        deframer = esttc_deframer(samp_rate)
        msg_debug = blocks.message_debug()


        self.tb.connect(src, unpack)
        self.tb.connect(unpack, deframer)
        self.tb.msg_connect((deframer, 'crc_ok'), (msg_debug, 'store'))
        self.tb.msg_connect((deframer, 'crc_ok'), (msg_debug, 'log'))
        self.tb.msg_connect((deframer, 'crc_fail'), (msg_debug, 'store'))
        self.tb.msg_connect((deframer, 'crc_fail'), (msg_debug, 'log'))
        self.tb.start()
        start = time.time()
        while msg_debug.num_messages() < 1 and time.time() - start < 5:
            pass
        self.tb.stop()
        self.assertGreaterEqual(msg_debug.num_messages(), 1)
        pdu_out = msg_debug.get_message(0)
        output = pmt.u8vector_elements(pmt.cdr(pdu_out))
        
        self.assertEqual(expected_output, output)


if __name__ == '__main__':
    gr_unittest.run(qa_esttc_deframer)
