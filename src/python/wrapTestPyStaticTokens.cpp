// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

/// \file wrapTestPyStaticTokens.cpp

#include <pxr/tf/pyStaticTokens.h>

namespace pxr {

#define TF_TEST_TOKENS                  \
    (orange)                            \
    ((pear, "d'Anjou"))                 

TF_DECLARE_PUBLIC_TOKENS(tfTestStaticTokens, TF_API, TF_TEST_TOKENS);
TF_DEFINE_PUBLIC_TOKENS(tfTestStaticTokens, TF_TEST_TOKENS);

}  // namespace pxr

using namespace pxr;

namespace {
struct _DummyScope {
};
}

void
wrapTf_TestPyStaticTokens()
{
    TF_PY_WRAP_PUBLIC_TOKENS("_testStaticTokens",
                             tfTestStaticTokens, TF_TEST_TOKENS);

    boost::python::class_<_DummyScope, boost::noncopyable>
        cls("_TestStaticTokens", boost::python::no_init);
    boost::python::scope testScope = cls;

    TF_PY_WRAP_PUBLIC_TOKENS_IN_CURRENT_SCOPE(
        tfTestStaticTokens, TF_TEST_TOKENS);
}
