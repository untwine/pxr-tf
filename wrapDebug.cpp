//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
#include "pxr/base/tf/debug.h"
#include "pxr/base/tf/pyUtils.h"

#include <boost/python/class.hpp>
#include <boost/python/object.hpp>

using namespace boost::python;

PXR_NAMESPACE_USING_DIRECTIVE

namespace {

static void
_SetOutputFile(object const &file)
{
    FILE *fp = PyFile_AsFile(file.ptr());
    if (!fp)
        TfPyThrowTypeError("expected file object");

    // On Windows the FILE* for sys.__stdout__ and __stderr__ will not
    // match stdout and stderr if there's redirection but output will
    // go to the same handle.  To satisfy TfDebug::SetOutputFile() we
    // translate the FILE pointers here.
    if (fp != stdout && fp != stderr) {
        object sys(handle<>(PyImport_ImportModule("sys")));
        if (PyFile_AsFile(object(sys.attr("__stdout__")).ptr()) == fp) {
            fp = stdout;
        }
        else if (PyFile_AsFile(object(sys.attr("__stderr__")).ptr()) == fp) {
            fp = stderr;
        }
    }

    TfDebug::SetOutputFile(fp);
}

} // anonymous namespace 

void wrapDebug()
{
    typedef TfDebug This;

    class_<This>("Debug", no_init)
        .def("SetDebugSymbolsByName", &This::SetDebugSymbolsByName,
             ( arg("pattern"), arg("value") ))
        .staticmethod("SetDebugSymbolsByName")

        .def("IsDebugSymbolNameEnabled", &This::IsDebugSymbolNameEnabled)
        .staticmethod("IsDebugSymbolNameEnabled")

        .def("GetDebugSymbolDescriptions", &This::GetDebugSymbolDescriptions)
        .staticmethod("GetDebugSymbolDescriptions")

        .def("GetDebugSymbolNames", &This::GetDebugSymbolNames)
        .staticmethod("GetDebugSymbolNames")

        .def("GetDebugSymbolDescription", &This::GetDebugSymbolDescription)
        .staticmethod("GetDebugSymbolDescription")

        .def("SetOutputFile", _SetOutputFile)
        .staticmethod("SetOutputFile")

        ;
}
