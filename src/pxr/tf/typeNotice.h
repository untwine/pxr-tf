//
// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
#ifndef PXR_TF_TYPE_NOTICE_H
#define PXR_TF_TYPE_NOTICE_H

#include "./notice.h"
#include "./type.h"

namespace pxr {

/// \class TfTypeWasDeclaredNotice
///
/// TfNotice sent after a TfType is declared.
class TfTypeWasDeclaredNotice : public TfNotice
{
public:
    TfTypeWasDeclaredNotice( TfType t );
    virtual ~TfTypeWasDeclaredNotice();

    /// Get the newly declared TfType.
    TfType GetType() const { return _type; }

private:
    TfType _type;
};

}  // namespace pxr

#endif // PXR_TF_TYPE_NOTICE_H
