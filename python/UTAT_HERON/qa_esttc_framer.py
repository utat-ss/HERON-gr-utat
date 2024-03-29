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
    from gnuradio.UTAT_HERON import esttc_framer
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio.UTAT_HERON import esttc_framer

class qa_esttc_framer(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_instance(self):
        instance = esttc_framer()

    def test_001_basic(self):

        input = [0xff, 0xff]
        expected_output = [0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0x7e, 0x02, 0xff, 0xff, 0xbf, 0xf3]

        src = blocks.vector_source_b(input, False, 1, [])
        tag = blocks.stream_to_tagged_stream(gr.sizeof_char, 1, len(input), 'packet_len')
        to_pdu = pdu.tagged_stream_to_pdu(gr.types.byte_t, 'packet_len')
        framer = esttc_framer()
        msg_debug = blocks.message_debug()

        self.tb.connect(src, tag)
        self.tb.connect(tag, to_pdu)
        self.tb.msg_connect((to_pdu, 'pdus'), (framer, 'pdu_in'))
        self.tb.msg_connect((framer, 'pdu_out'), (msg_debug, 'store'))
        self.tb.run()
        pdu_out = msg_debug.get_message(0)
        output = pmt.u8vector_elements(pmt.cdr(pdu_out))
        
        self.assertEqual(expected_output, output)





if __name__ == '__main__':
    gr_unittest.run(qa_esttc_framer)
