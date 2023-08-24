#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2023 University of Toronto Aerospace Team.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

import pmt
from gnuradio import gr, gr_unittest
from gnuradio import blocks, pdu
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
        instance = esttc_deframer(1e6)

    def test_001_basic(self):
        input = [0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x7e, 0x02, 0xff, 0xff, 0xbf, 0xf3]
        expected_output = [0xff, 0xff]

        src = blocks.vector_source_b(input, False, 1, [])
        unpack = blocks.unpack_k_bits_bb(8)
        deframer = esttc_deframer(1e6)
        msg_debug = blocks.message_debug(True, gr.log_levels.info)

        self.tb.connect(src, unpack)
        self.tb.connect(unpack, deframer)
        self.tb.msg_connect((deframer, 'crc_ok'), (msg_debug, 'store'))
        self.tb.msg_connect((deframer, 'crc_fail'), (msg_debug, 'print'))
        self.tb.run()
        pdu_out = msg_debug.get_message(0)
        output = pmt.u8vector_elements(pmt.cdr(pdu_out))
        
        self.assertEqual(expected_output, output)


if __name__ == '__main__':
    gr_unittest.run(qa_esttc_deframer)
