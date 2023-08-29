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
    from gnuradio.UTAT_HERON import tagged_stream_tail_tagger
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio.UTAT_HERON import tagged_stream_tail_tagger

class qa_tagged_stream_tail_tagger(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_instance(self):
        instance = tagged_stream_tail_tagger('packet_len', 'trailer')

    def test_001_basic(self):

        input = (complex(1,1), complex(2,2), complex(3,3))
        expected_output = [complex(1,1), complex(2,2), complex(3,3), complex()]
        
        source = blocks.vector_source_c(input, True, 1, [])
        tag_packet = blocks.stream_to_tagged_stream(gr.sizeof_gr_complex, 1, 3, 'packet_len')
        instance = tagged_stream_tail_tagger('packet_len', 'trailer')
        head = blocks.head(gr.sizeof_gr_complex, 4)
        sink = blocks.vector_sink_c(1, 4)

        self.tb.connect(source, tag_packet)
        self.tb.connect(tag_packet, instance)
        self.tb.connect(instance, head)
        self.tb.connect(head, sink)
        self.tb.run()
        output = sink.data()
        self.assertEqual(output, expected_output)


if __name__ == '__main__':
    gr_unittest.run(qa_tagged_stream_tail_tagger)
