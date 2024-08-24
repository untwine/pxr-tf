// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.
//
// testTfPyLock.mm
//

#include <pxr/tf/pyLock.h>
#include <pxr/tf/pyInterpreter.h>

using namespace pxr;

static unsigned int
testLock(bool verbose)
{
    unsigned int numErrors = 0;
    
    TfPyLock lock;
    
    TfPyInitialize();

    lock.Acquire();
    
    printf("===== Expected error output =====\n");
    lock.Acquire();
    printf("===== End expected output =====\n");

    {
        TF_PY_ALLOW_THREADS_IN_SCOPE();
    }
    
    lock.Release();

    printf("===== Expected error output =====\n");
    lock.Release();
    printf("===== End expected output =====\n");
    
    return numErrors;
}



int
main(int argc, char **argv)
{
    bool verbose = ((argc == 2) && (strcmp(argv[1], "-v") == 0));
    unsigned int numErrors = 0;
    
    numErrors += testLock(verbose);

    // Print status
    if (numErrors > 0) {
        printf("\nTest FAILED\n");
    } else if (verbose) {
        printf("\nTest SUCCEEDED\n");
    }
    
    return numErrors;
}
