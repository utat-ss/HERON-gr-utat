/*
 * Copyright 2023 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(tagged_stream_tail_tagger.h) */
/* BINDTOOL_HEADER_FILE_HASH(a544000c6ef5c511ea9a95c65ec60c94)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/UTAT_HERON/tagged_stream_tail_tagger.h>
// pydoc.h is automatically generated in the build directory
#include <tagged_stream_tail_tagger_pydoc.h>

void bind_tagged_stream_tail_tagger(py::module& m)
{

    using tagged_stream_tail_tagger = gr::UTAT_HERON::tagged_stream_tail_tagger;


    py::class_<tagged_stream_tail_tagger,
               gr::block,
               gr::basic_block,
               std::shared_ptr<tagged_stream_tail_tagger>>(
        m, "tagged_stream_tail_tagger", D(tagged_stream_tail_tagger))

        .def(py::init(&tagged_stream_tail_tagger::make),
             py::arg("length_tag_key"),
             py::arg("tail_tag_key"),
             D(tagged_stream_tail_tagger, make))


        ;
}
