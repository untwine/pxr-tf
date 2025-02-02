//
// Copyright 2026 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//

#include <pxr/tf/pxr.h>
#include <pxr/tf/diagnosticContainer.h>
#include <pxr/tf/diagnosticMgr.h>
#include <pxr/tf/stl.h>

TF_NAMESPACE_OPEN_SCOPE

void
Tf_DiagnosticContainer::Post()
{
    TfDiagnosticMgr &mgr = TfDiagnosticMgr::GetInstance();
    Iterator it = GetIterator();
    while (it.Next(TfOverloads {
                [&mgr](TfError const &e)   { mgr.PostError(e);    },
                [&mgr](TfWarning const &w) { mgr.PostWarning(w);  },
                [&mgr](TfStatus const &s)  { mgr.PostStatus(s);   }
            }));
    Clear();
}

TF_NAMESPACE_CLOSE_SCOPE
