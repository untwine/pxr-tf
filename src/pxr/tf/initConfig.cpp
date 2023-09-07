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

#include "./debug.h"
#include "./getenv.h"
#include "./mallocTag.h"
#include <pxr/arch/attributes.h>
#include <pxr/arch/systemInfo.h>

#include <string>

namespace pxr {

extern void Tf_DebugInitFromEnvironment();

namespace {

ARCH_CONSTRUCTOR(Tf_InitConfig, 2, void)
{
    std::string capture = TfGetenv("TF_MALLOC_TAG_CAPTURE");
    std::string debug   = TfGetenv("TF_MALLOC_TAG_DEBUG");
    if (!capture.empty() || !debug.empty() ||
        TfGetenvBool("TF_MALLOC_TAG", false)) {
        std::string errMsg;

        /*
         * Only the most basic error output can be done this early...
         */
        
        if (!TfMallocTag::Initialize(&errMsg)) {
            fprintf(stderr, "%s: TF_MALLOC_TAG environment variable set, but\n"
                    "            malloc tag initialization failed: %s\n",
                    ArchGetExecutablePath().c_str(), errMsg.c_str());
        }
        else {
            TfMallocTag::SetCapturedMallocStacksMatchList(capture);
            TfMallocTag::SetDebugMatchList(debug);
        }
    }
}

// Run this after registry functions execute.  This is only necessary because
// of the TF_DEBUG="list" feature which prints the registered names and their
// descriptions and exits.  If we call this before registry functions were
// executed we would not see any added inside TF_REGISTRY_FUNCTION, which is
// most of them.
ARCH_CONSTRUCTOR(Tf_InitConfigPost, 202, void)
{
    Tf_DebugInitFromEnvironment();
}

}

}  // namespace pxr
