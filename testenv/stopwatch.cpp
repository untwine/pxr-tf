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

#include "pxr/pxr.h"
#include "pxr/base/tf/regTest.h"
#include "pxr/base/tf/stopwatch.h"
#include "pxr/base/tf/stringUtils.h"

#include <algorithm>
#include <chrono>
#include <iostream>
#include <thread>

using namespace std;
PXR_NAMESPACE_USING_DIRECTIVE

static bool
IsClose(double a, double b, double epsilon=1e-3)
{
    auto diff = fabs(a-b);
    return diff <= epsilon * abs(a)
           && diff <= epsilon * abs(b);
}

static bool
Test_TfStopwatch()
{
    bool ok = true;

    // Test constructor
    TfStopwatch watch1;

    // Test copy constructor.
    TfStopwatch watchCopy(watch1);
    if (watchCopy.GetSeconds() != watch1.GetSeconds()) {
        cout << "expected watchCopy to contain (0.0) but got (\""
             <<  watchCopy.GetSeconds()
             << ")" << endl;
        ok = false;
    }

    // Test the timer

    // Delay .5 seconds (500 million nanoseconds)
    //
    watch1.Start();
    auto start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    watch1.Stop();
    auto end = std::chrono::steady_clock::now();

    std::chrono::duration<double> elapsed = end - start;

    // The value of watch1 should match the elapsed duration
    // measured via std::chrono. This should be roughly .5 seconds,
    // but may be higher since sleep_for may overshoot the requested
    // time.
    if (!IsClose(watch1.GetSeconds(), elapsed.count())) {
        cout << "Sleep for .5 seconds but got: " << endl
             << "TfStopwatch: " << watch1.GetSeconds() << endl
             << "std::chrono: " << elapsed.count() << endl;
        ok = false;
    }

    // Delay another .5 seconds and see if watch is near 1
    //
    watch1.Start();
    start = std::chrono::steady_clock::now();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    watch1.Stop();
    end = std::chrono::steady_clock::now();

    elapsed += (end - start);

    // The value of watch1 should match the updated elapsed duration.
    if (!IsClose(watch1.GetSeconds(), elapsed.count())) {
        cout << "Sleep for 1.0 seconds but got: " << endl
             << "TfStopwatch: " << watch1.GetSeconds() << endl
             << "std::chrono: " << elapsed.count() << endl;
        ok = false;
    }

    // The value of watchCopy should be zero
    //
    if (watchCopy.GetSeconds() != 0.0) {
        cout << "watchCopy has non-zero initial time of "
             << watchCopy.GetSeconds()
             << " seconds."
             << endl;
        ok = false;
    }

    // Test AddFrom
    //
    watchCopy.AddFrom(watch1);
    if (!IsClose(watchCopy.GetSeconds(), watch1.GetSeconds())) {
        cout << "AddFrom: watchCopy has time of "
             << watchCopy.GetSeconds()
             << " instead of "
             << watch1.GetSeconds()
             << " seconds."
             << endl;
        ok = false;
    }

    // Test AddFrom
    //
    watchCopy.AddFrom(watch1);
    if (!IsClose(watchCopy.GetSeconds()/watch1.GetSeconds(), 2.0)) {
        cout << "AddFrom: watchCopy has time of "
             << watchCopy.GetSeconds()
             << " instead of "
             << 2 * watch1.GetSeconds()
             << " seconds."
             << endl;
        ok = false;
    }

    // Test Reset
    watchCopy.Reset();
    if (watchCopy.GetSeconds() != 0.0) {
        cout << "Reset: watchCopy has time of "
             << watchCopy.GetSeconds()
             << " instead of "
             << 0.0
             << " seconds."
             << endl;
        ok = false;
    }

    return ok;
}

TF_ADD_REGTEST(TfStopwatch);
