// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

///
/// \file tf/wrapPathUtils.cpp

#include <pxr/tf/pathUtils.h>
#include <pxr/tf/diagnostic.h>
#include <boost/python/def.hpp>
#include <string>

using std::string;

using namespace boost::python;

using namespace pxr;

namespace {

static string
_RealPath(string const &path, bool allowInaccessibleSuffix, bool raiseOnError)
{
    string error;
    string realPath = TfRealPath(path, allowInaccessibleSuffix, &error);
    if (raiseOnError && !error.empty()) {
        TF_RUNTIME_ERROR(error);
    }
    return realPath;
}

static string::size_type
_FindLongestAccessiblePrefix(string const &path)
{
    // For python, we convert npos into path's length, which makes the return
    // value correct to use in slices.
    string error;
    string::size_type result = TfFindLongestAccessiblePrefix(path, &error);

    if (!error.empty()) {
        PyErr_SetString(PyExc_OSError, error.c_str());
        throw_error_already_set();
    }

    return result;
}

} // anonymous namespace 

void wrapPathUtils()
{
    def("RealPath", _RealPath,
        ( arg("path"),
          arg("allowInaccessibleSuffix") = false,
          arg("raiseOnError") = false ));

    def("FindLongestAccessiblePrefix", _FindLongestAccessiblePrefix);
}
