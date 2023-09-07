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
#ifndef PXR_TF_PY_TRACING_H
#define PXR_TF_PY_TRACING_H

#ifdef PXR_PYTHON_SUPPORT_ENABLED
#include "./pySafePython.h"
#endif // PXR_PYTHON_SUPPORT_ENABLED

#include "./api.h"

#include <functional>
#include <memory>

namespace pxr {

#ifdef PXR_PYTHON_SUPPORT_ENABLED
/// \struct TfPyTraceInfo
/// Structure passed to python trace functions.  See the Python C API
/// documentation reference for the meaning of \a what and \a arg.
struct TfPyTraceInfo {
    PyObject *arg;
    char const *funcName;
    char const *fileName;
    int funcLine;
    int what;
};

typedef std::function<void (TfPyTraceInfo const &)> TfPyTraceFn;
typedef std::shared_ptr<TfPyTraceFn> TfPyTraceFnId;

/// Register \a f as a python trace function.
/// It will be invoked for python tracing events. If python is not yet
/// initialized, the function will not be invoked until python is initialized.
TF_API TfPyTraceFnId TfPyRegisterTraceFn(TfPyTraceFn const &f);

// For internal use only.  Do not use.
void Tf_PyFabricateTraceEvent(TfPyTraceInfo const &info);

// For internal use only.  Do not use.
void Tf_PyTracingPythonInitialized();
#else
/// \struct TfPyTraceInfo
/// For storage alignment when PXR_PYTHON_SUPPORT_ENABLED is not enabled.
struct TfPyTraceInfo;
typedef std::function<void (TfPyTraceInfo const &)> TfPyTraceFn;
typedef std::shared_ptr<TfPyTraceFn> TfPyTraceFnId;
#endif // PXR_PYTHON_SUPPORT_ENABLED

}  // namespace pxr

#endif // PXR_TF_PY_TRACING_H
