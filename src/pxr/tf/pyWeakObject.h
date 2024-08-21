//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_TF_PY_WEAK_OBJECT_H
#define PXR_TF_PY_WEAK_OBJECT_H

#include "./api.h"
#include "./pyIdentity.h"

#include "./hash.h"
#include "./singleton.h"
#include "./weakBase.h"
#include "./weakPtr.h"

#include <boost/python/handle.hpp>
#include <boost/python/object.hpp>

#include "./hashmap.h"

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
