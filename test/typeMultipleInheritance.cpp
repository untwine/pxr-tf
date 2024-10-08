// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/tf/type.h>
#include <pxr/tf/errorMark.h>
#include <pxr/tf/regTest.h>

using namespace std;
using namespace pxr;

class A {
public:
    virtual ~A() {}
};

class B {
public:
    virtual ~B() {}
};

class X : public A, public B {
public:
    virtual ~X() {}
};

// Note that A,B are inherited in the opposite order from X.
class Y : public B, public A {
public:
    virtual ~Y() {}
};

class Z : public X, public Y {
public:
    virtual ~Z() {}
};


TF_REGISTRY_FUNCTION(pxr::TfType)
{
    TfType::Define<A>();
    TfType::Define<B>();
    TfType::Define<X, TfType::Bases< A, B > >();
    TfType::Define<Y, TfType::Bases< B, A > >();
    TfType::Define<Z, TfType::Bases< X, Y > >();
}

static bool
Test_TfType_MultipleInheritance()
{
    // Test TfType::GetAncestorTypes()'s error condition of
    // inconsistent multiple inheritance.  (We'd ideally test this from
    // Python, but Python prohibits you from even declaring hierarchies
    // like this to begin with.)

    TfErrorMark m;
    m.SetMark();

    vector<TfType> types;

    TF_AXIOM(m.IsClean());

    TfType::Find<Z>().GetAllAncestorTypes(&types);

    TF_AXIOM(!m.IsClean());
    m.Clear();

    return true;
}

TF_ADD_REGTEST(TfType_MultipleInheritance);

