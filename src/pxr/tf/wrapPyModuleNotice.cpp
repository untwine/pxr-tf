//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include "./pyModuleNotice.h"
#include "./pyNoticeWrapper.h"

#include <boost/python/return_by_value.hpp>
#include <boost/python/return_value_policy.hpp>

using namespace boost::python;

using namespace pxr;

namespace {

TF_INSTANTIATE_NOTICE_WRAPPER(pxr::TfPyModuleWasLoaded, pxr::TfNotice);

} // anonymous namespace 

void wrapPyModuleNotice() {

    TfPyNoticeWrapper<TfPyModuleWasLoaded, TfNotice>::Wrap("PyModuleWasLoaded")
        .def("name", make_function(&TfPyModuleWasLoaded::GetName,
                                   return_value_policy<return_by_value>()))
        ;
}
