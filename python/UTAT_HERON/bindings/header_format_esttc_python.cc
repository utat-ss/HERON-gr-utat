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
/* BINDTOOL_HEADER_FILE(header_format_esttc.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(ebcce81ddfd71dea3232d374f3d6f3cf)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/UTAT_HERON/header_format_esttc.h>
// pydoc.h is automatically generated in the build directory
#include <header_format_esttc_pydoc.h>

void bind_header_format_esttc(py::module& m)
{

    using header_format_esttc    = gr::UTAT_HERON::header_format_esttc;


    py::class_<header_format_esttc, gr::digital::header_format_default,
        std::shared_ptr<header_format_esttc>>(m, "header_format_esttc", D(header_format_esttc))

        .def(
            py::init(&header_format_esttc::make),
            py::arg("access_code"),
            py::arg("threshold"),
            py::arg("bps") = 1,
            py::arg("trailer_nbits") = 0,
            D(header_format_esttc,make)
        )
        .def_static(
            "make",
            &header_format_esttc::make,
            py::arg("access_code"),
            py::arg("threshold"),
            py::arg("bps") = 1,
            py::arg("trailer_nbits") = 0,
            D(header_format_esttc,make)
        )
        .def(
            "format",
            &header_format_esttc::format,
            py::arg("nbytes_in"),
            py::arg("input"),
            py::arg("output"),
            py::arg("info"),
            D(header_format_esttc, format))
        .def(
            "parse",
            &header_format_esttc::parse,
            py::arg("nbits_in"),
            py::arg("input"),
            py::arg("info"),
            py::arg("nbits_processed"),
            D(header_format_esttc, parse))
        .def(
            "header_nbits",
            &header_format_esttc::header_nbits,
            D(header_format_esttc, header_nbits))



        ;




}








