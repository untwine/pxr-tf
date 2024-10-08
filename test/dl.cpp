// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/tf/regTest.h>
#include <pxr/tf/debugCodes.h>
#include <pxr/tf/debug.h>
#include <pxr/tf/diagnosticLite.h>
#include <pxr/tf/stringUtils.h>
#include <pxr/tf/dl.h>
#include <pxr/arch/fileSystem.h>
#include <pxr/arch/library.h>
#include <pxr/arch/symbols.h>

using std::string;
using namespace pxr;

static bool
Test_TfDl()
{
    // We should not be in the process of opening/closing a DL right now
    TF_AXIOM(!Tf_DlOpenIsActive());
    TF_AXIOM(!Tf_DlCloseIsActive());

    // Turn on TfDlopen debugging so we get coverage on the debug output too
    TfDebug::Enable(TF_DLOPEN);
    TfDebug::Enable(TF_DLCLOSE);

    // Check that opening a non-existing shared library fails
    TF_AXIOM(!TfDlopen("nonexisting" ARCH_LIBRARY_SUFFIX, ARCH_LIBRARY_NOW));

    // Check that TfDlopen fills in our error string with something
    string dlerror;
    TfDlopen("nonexisting" ARCH_LIBRARY_SUFFIX, ARCH_LIBRARY_NOW, &dlerror);
    TF_AXIOM(!dlerror.empty());

    // Compute path to test library.
    string dlname;
    TF_AXIOM(ArchGetAddressInfo((void*)Test_TfDl, &dlname, NULL, NULL, NULL));
    dlname = TfGetPathName(dlname) +
#if !defined(ARCH_OS_WINDOWS)
        "lib"
#endif
        "TestTfDl" ARCH_LIBRARY_SUFFIX;

    // Make sure that this .so does indeed exist first
    printf("Checking test shared lib: %s\n", dlname.c_str());
    TF_AXIOM(!ArchFileAccess(dlname.c_str(), R_OK));

    // Check that we can open the existing library.
    void *handle =
        TfDlopen(dlname, ARCH_LIBRARY_LAZY|ARCH_LIBRARY_LOCAL, &dlerror);
    TF_AXIOM(handle);
    TF_AXIOM(dlerror.empty());
    TF_AXIOM(!TfDlclose(handle));

    // we should not be in the process of opening/closing a DL now either
    TF_AXIOM(!Tf_DlOpenIsActive());
    TF_AXIOM(!Tf_DlCloseIsActive());

    return true;
}

TF_ADD_REGTEST(TfDl);
