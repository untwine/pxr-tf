//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
//
// Shared library with a registry function.
//

#include <pxr/tf/registryManager.h>
#include <cstdio>

using namespace pxr;

class Tf_TestRegistryFunctionPlugin;

TF_REGISTRY_FUNCTION(Tf_TestRegistryFunctionPlugin)
{
    printf("* Running Tf_TestRegistryFunctionPlugin registry function from "
           __FILE__ "\n");
}
