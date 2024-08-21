//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
////////////////////////////////////////////////////////////////////////

#include "./registryManager.h"
#include "./scriptModuleLoader.h"
#include "./token.h"

#include <vector>

namespace pxr {

TF_REGISTRY_FUNCTION(pxr::TfScriptModuleLoader) {
    // List of direct dependencies for this library.
    const std::vector<TfToken> reqs = {
        TfToken("arch")
    };
    TfScriptModuleLoader::GetInstance().
        RegisterLibrary(TfToken("tf"), TfToken("pxr.Tf"), reqs);
}

}  // namespace pxr


