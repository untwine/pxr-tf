// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/tf/errorMark.h>
#include <pxr/arch/stackTrace.h>

#include <unistd.h>

#include <thread>
#include <atomic>
#include <iostream>

using namespace pxr;

// This could probably be a regular int since we're not really worried
// about full thread safety, just whether it is 1 or 0.
static std::atomic_int synchronizer;

/**
 * This executable performs multiple "simultaneous" invalid memory
 * references (SIGSEGV) for testing of the Tf crash handler from
 * multiple threads.
 */

static void
_ThreadTask()
{
    TfErrorMark m;
    TF_RUNTIME_ERROR("Pending secondary thread error for crash report!");

    // Wait for synchronizer to become 0
    while (synchronizer) {
        // spin!
    }

    // Dereference a null pointer!
    int* bunk(0);
    std::cout << *bunk << '\n';
}

int
main(int argc, char **argv)
{
    ArchSetFatalStackLogging( true );

    TfErrorMark m;

    TF_RUNTIME_ERROR("Pending error to report in crash output!");

    // Make sure the threads don't run off and generate segmentation faults
    // before we're ready.
    //
    synchronizer = 1;

    // Spawn 2 threads, each of which will wait for synchronizer to
    // become 0 and then generate a SIGSEGV. The desire is to produce
    // two SIGSEGV signals in two different threads at very nearly the
    // same time.
    //
    std::thread t1(_ThreadTask);
    std::thread t2(_ThreadTask);

    // Wait to ensure the threads are spinning on synchronizer
    sleep(1);

    // Release them.
    synchronizer = 0;

    // Wait for them to die
    t1.join();
    t2.join();

    return 0;
}


