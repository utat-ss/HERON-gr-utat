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
/* BINDTOOL_HEADER_FILE(esttc_deframer.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(634f9d91a8cfa3c164174c1ed95eaff4)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/UTAT_HERON/esttc_deframer.h>
// pydoc.h is automatically generated in the build directory
#include <esttc_deframer_pydoc.h>

void bind_esttc_deframer(py::module& m)
{

    using esttc_deframer = gr::UTAT_HERON::esttc_deframer;


    py::class_<esttc_deframer,
               gr::hier_block2,
               std::shared_ptr<esttc_deframer>>(m, "esttc_deframer", D(esttc_deframer))

        .def(py::init(&esttc_deframer::make),
             py::arg("samp_rate"),
             D(esttc_deframer, make))


        ;
}
