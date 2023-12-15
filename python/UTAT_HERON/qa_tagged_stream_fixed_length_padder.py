#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Copyright 2023 University of Toronto Aerospace Team.
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

from gnuradio import gr, gr_unittest
# from gnuradio import blocks
try:
    from gnuradio.UTAT_HERON import tagged_stream_fixed_length_padder
except ImportError:
    import os
    import sys
    dirname, filename = os.path.split(os.path.abspath(__file__))
    sys.path.append(os.path.join(dirname, "bindings"))
    from gnuradio.UTAT_HERON import tagged_stream_fixed_length_padder

class qa_tagged_stream_fixed_length_padder(gr_unittest.TestCase):

    def setUp(self):
        self.tb = gr.top_block()

    def tearDown(self):
        self.tb = None

    def test_instance(self):
        instance = tagged_stream_fixed_length_padder('packet_len', 1, 10, 0x00, 5)

    def test_001_descriptive_test_name(self):
        # set up fg
        self.tb.run()
        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_tagged_stream_fixed_length_padder)
