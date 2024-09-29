// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TF_PY_CLASS_METHOD_H
#define PXR_TF_PY_CLASS_METHOD_H

#include <boost/python/class.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/object.hpp>
#include <boost/python/def_visitor.hpp>

namespace pxr {

namespace Tf_PyClassMethod {

using namespace boost::python;

// Visitor for wrapping functions as Python class methods.
// See typedef below for docs.
// This is very similar to the staticmethod() method on boost::python::class,
// except it uses PyClassMethod_New() instead of PyStaticMethod_New().
struct _TfPyClassMethod : def_visitor<_TfPyClassMethod>
{
    friend class def_visitor_access;

    _TfPyClassMethod(const std::string &methodName) :
        _methodName(methodName) {}
    explicit _TfPyClassMethod(const char *methodName) :
        _methodName(methodName) {}

    template <typename CLS>
    void visit(CLS &c) const
    {
        PyTypeObject* self = downcast<PyTypeObject>( c.ptr() );
        dict d((handle<>(borrowed(self->tp_dict))));

        object method(d[_methodName]);

        c.attr(_methodName.c_str()) = object(
            handle<>( PyClassMethod_New((_CallableCheck)(method.ptr()) )));
    }

private:

    PyObject* _CallableCheck(PyObject* callable) const
    {
        if (PyCallable_Check(expect_non_null(callable)))
            return callable;

        PyErr_Format( PyExc_TypeError,
           "classmethod expects callable object; got an object of type %s, "
           "which is not callable",
           callable->ob_type->tp_name);

        throw_error_already_set();
        return 0;
    }

    const std::string _methodName;
};

}

/// A boost.python class visitor which replaces the named method with a
/// classmethod()-wrapped one.
///
/// \code
///    void Foo( boost::python::object & pyClassObject ) { /* ... */ }
///    
///    class_<...>(...)
///         .def("Foo", &Foo)
///         .def(TfPyClassMethod("Foo"))
///         ;
/// \endcode
///
typedef Tf_PyClassMethod::_TfPyClassMethod TfPyClassMethod;

}  // namespace pxr

#endif // PXR_TF_PY_CLASS_METHOD_H
