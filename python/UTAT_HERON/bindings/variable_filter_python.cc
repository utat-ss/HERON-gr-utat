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
/* BINDTOOL_HEADER_FILE(variable_filter.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(766845e61a49b7b901968168b8cc740f)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <gnuradio/UTAT_HERON/variable_filter.h>
// pydoc.h is automatically generated in the build directory
#include <variable_filter_pydoc.h>

void bind_variable_filter(py::module& m)
{

    using variable_filter = gr::UTAT_HERON::variable_filter;


    py::class_<variable_filter,
               gr::block,
               gr::basic_block,
               std::shared_ptr<variable_filter>>(m, "variable_filter", D(variable_filter))

        .def(py::init(&variable_filter::make),
             py::arg("variable_name"),
             D(variable_filter, make))


        ;
}
