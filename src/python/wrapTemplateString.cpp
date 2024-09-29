// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/tf/templateString.h>

#include <pxr/tf/pyResultConversions.h>
#include <pxr/tf/pyUtils.h>

#include <boost/python/class.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/return_value_policy.hpp>
#include <boost/python/return_by_value.hpp>

#include <string>

using std::string;

using namespace boost::python;

using namespace pxr;

namespace {

static string __repr__(TfTemplateString const &self)
{
    return TF_PY_REPR_PREFIX + "TemplateString(" +
        (self.GetTemplate().empty() ? string() : TfPyRepr(self.GetTemplate())) +
        ")";
}

static string _Substitute(TfTemplateString const &self, dict const &d)
{
    TfTemplateString::Mapping m;
    list items = d.items();
    for (int i = 0; i < len(items); ++i)
        m[extract<string>(items[i][0])] = extract<string>(items[i][1]);
    return self.Substitute(m);
}

static string _SafeSubstitute(TfTemplateString const &self, dict const &d)
{
    TfTemplateString::Mapping m;
    list items = d.items();
    for (int i = 0; i < len(items); ++i)
        m[extract<string>(items[i][0])] = extract<string>(items[i][1]);
    return self.SafeSubstitute(m);
}

} // anonymous namespace 

void wrapTemplateString()
{
    typedef TfTemplateString This;

    class_<This>("TemplateString")
        .def(init<string>())
        .def("__repr__", __repr__)
        .add_property("template",
                      make_function(&This::GetTemplate,
                                    return_value_policy<return_by_value>()))
        .def("Substitute", _Substitute)
        .def("SafeSubstitute", _SafeSubstitute)
        .def("GetEmptyMapping", &This::GetEmptyMapping,
             return_value_policy<TfPyMapToDictionary>())
        .def("GetParseErrors", &This::GetParseErrors,
             return_value_policy<TfPySequenceToList>())
        .add_property("valid", &This::IsValid)
        ;
}
