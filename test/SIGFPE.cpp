// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/arch/stackTrace.h>
#include <pxr/tf/diagnostic.h>

#include <csignal>
#include <cstdio>

using namespace pxr;

// This test raises SIGFPE to test the Tf crash handler

int
main(int argc, char **argv)
{
    ArchSetFatalStackLogging( true );

    // Make sure handlers have been installed
    // This isn't guaranteed in external environments
    // as we leave them off by default.
    TfInstallTerminateAndCrashHandlers();

    // Raise SIGFPE.
    raise(SIGFPE);

    // We shouldn't get here.  Exit with zero because we expect a non-zero
    // exit code from this test.
    printf("failed\n");
    exit(0);
}
