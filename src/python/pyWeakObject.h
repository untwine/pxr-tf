// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TF_PY_WEAK_OBJECT_H
#define PXR_TF_PY_WEAK_OBJECT_H

#include <pxr/tf/api.h>
#include <pxr/tf/pyIdentity.h>

#include <pxr/tf/hash.h>
#include <pxr/tf/singleton.h>
#include <pxr/tf/weakBase.h>
#include <pxr/tf/weakPtr.h>

#include <boost/python/handle.hpp>
#include <boost/python/object.hpp>

#include <pxr/tf/hashmap.h>

namespace pxr {

typedef TfWeakPtr<struct Tf_PyWeakObject> Tf_PyWeakObjectPtr;

// A weak pointable weak reference to a python object.
struct Tf_PyWeakObject : public TfWeakBase
{
public:
    typedef Tf_PyWeakObject This;

    static Tf_PyWeakObjectPtr GetOrCreate(boost::python::object const &obj);
    boost::python::object GetObject() const;
    void Delete();
    
private:
    explicit Tf_PyWeakObject(boost::python::object const &obj);
    
    boost::python::handle<> _weakRef;
};

}  // namespace pxr

#endif // PXR_TF_PY_WEAK_OBJECT_H
