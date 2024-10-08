// Copyright 2020 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TF_PY_SAFE_PYTHON_H
#define PXR_TF_PY_SAFE_PYTHON_H

#include <pxr/arch/pragmas.h>

/// \file tf/pySafePython.h
/// Intended to replace a direct include of Python.h, which causes several
/// build problems with certain configurations and platforms (e.g., debug 
/// builds on Windows, Qt slots keyword, etc.)

// This include is a hack to avoid build errors due to incompatible
// macro definitions in pyport.h on MacOS for older versions of Python.
// See: https://bugs.python.org/issue10910
#include <locale>

// Python 3 has a conflict with the slots macro defined by the Qt library,
// so we're undef'ing here temporarily before including Python.
ARCH_PRAGMA_PUSH_MACRO(slots)
#undef slots

#include <boost/python/detail/wrap_python.hpp>

ARCH_PRAGMA_POP_MACRO(slots)

#endif // PXR_TF_PY_SAFE_PYTHON_H
