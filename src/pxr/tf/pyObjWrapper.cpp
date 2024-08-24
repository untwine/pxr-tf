// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include "./pyObjWrapper.h"

#ifdef PXR_PYTHON_SUPPORT_ENABLED
#include "./pyLock.h"
#include "./pyUtils.h"
#include "./type.h"

#include <boost/python/object.hpp>

namespace pxr {

TF_REGISTRY_FUNCTION(pxr::TfType)
{
    TfType::Define<TfPyObjWrapper>();
}

}  // namespace pxr

namespace {

// A custom deleter for shared_ptr<boost::python::object> that takes the
// python lock before deleting the python object.  This is necessary since it's
// invalid to decrement the python refcount without holding the lock.
struct _DeleteObjectWithLock {
    void operator()(boost::python::object *obj) const {
        pxr::TfPyLock lock;
        delete obj;
    }
};
}

namespace pxr {

TfPyObjWrapper::TfPyObjWrapper()
{
    TfPyLock lock;
    TfPyObjWrapper none((boost::python::object())); // <- extra parens for "most
                                                    // vexing parse".
    *this = none;
}

TfPyObjWrapper::TfPyObjWrapper(boost::python::object obj)
    : _objectPtr(new object(obj), _DeleteObjectWithLock())
{
}

PyObject *
TfPyObjWrapper::ptr() const
{
    return _objectPtr->ptr();
}

bool
TfPyObjWrapper::operator==(TfPyObjWrapper const &other) const
{
    // If they point to the exact same object instance, we know they're equal.
    if (_objectPtr == other._objectPtr)
        return true;

    // Otherwise lock and let python determine equality.
    TfPyLock lock;
    return Get() == other.Get();
}

bool
TfPyObjWrapper::operator!=(TfPyObjWrapper const &other) const
{
    return !(*this == other);
}

}  // namespace pxr

#endif // PXR_PYTHON_SUPPORT_ENABLED
