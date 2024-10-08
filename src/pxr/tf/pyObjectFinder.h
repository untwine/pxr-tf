// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TF_PY_OBJECT_FINDER_H
#define PXR_TF_PY_OBJECT_FINDER_H

#include "./api.h"
#include "./pyIdentity.h"

#include <boost/python/handle.hpp>
#include <boost/python/object.hpp>

#include <typeinfo>

namespace pxr {

struct Tf_PyObjectFinderBase {
    TF_API virtual ~Tf_PyObjectFinderBase();
    virtual boost::python::object Find(void const *objPtr) const = 0;
};

template <class T, class PtrType>
struct Tf_PyObjectFinder : public Tf_PyObjectFinderBase {
    virtual ~Tf_PyObjectFinder() {}
    virtual boost::python::object Find(void const *objPtr) const {
        using namespace boost::python;
        TfPyLock lock;
        void *p = const_cast<void *>(objPtr);
        PyObject *obj = Tf_PyGetPythonIdentity(PtrType(static_cast<T *>(p)));
        return obj ? object(handle<>(obj)) : object();
    }
};

TF_API
void Tf_RegisterPythonObjectFinderInternal(std::type_info const &type,
                                           Tf_PyObjectFinderBase const *finder);

template <class T, class PtrType>
void Tf_RegisterPythonObjectFinder() {
    Tf_RegisterPythonObjectFinderInternal(typeid(T),
                                          new Tf_PyObjectFinder<T, PtrType>());
}

TF_API boost::python::object
Tf_FindPythonObject(void const *objPtr, std::type_info const &type);

}  // namespace pxr

#endif // PXR_TF_PY_OBJECT_FINDER_H
