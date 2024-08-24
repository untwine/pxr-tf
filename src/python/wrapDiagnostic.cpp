// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/tf/diagnostic.h>
#include <pxr/tf/pyEnum.h>

#include <boost/python/def.hpp>

using namespace boost::python;

using namespace pxr;

void wrapDiagnostic()
{
    TfPyWrapEnum<TfDiagnosticType>();

    def("InstallTerminateAndCrashHandlers",
        TfInstallTerminateAndCrashHandlers);

}
