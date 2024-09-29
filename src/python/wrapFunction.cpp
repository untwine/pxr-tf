// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/tf/pyFunction.h>
#include <pxr/tf/pyObjWrapper.h>

#include <boost/python/object.hpp>

#include <string>

using namespace boost;

using namespace pxr;

void wrapFunction() {
    TfPyFunctionFromPython<void ()>();
    TfPyFunctionFromPython<bool ()>();
    TfPyFunctionFromPython<int ()>();
    TfPyFunctionFromPython<long ()>();
    TfPyFunctionFromPython<double ()>();
    TfPyFunctionFromPython<std::string ()>();
    TfPyFunctionFromPython<python::object ()>();
    TfPyFunctionFromPython<TfPyObjWrapper ()>();
}
