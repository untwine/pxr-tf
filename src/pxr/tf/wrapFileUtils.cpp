//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include <boost/python/def.hpp>

#include "./fileUtils.h"

#include <string>

using std::string;

using namespace boost::python;

using namespace pxr;

void wrapFileUtils() {

    def("TouchFile", &TfTouchFile, (arg("fileName"), arg("create")=bool(false)));
}
