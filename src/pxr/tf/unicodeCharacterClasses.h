// Copyright 2023 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TF_UNICODE_CHARACTER_CLASSES_H
#define PXR_TF_UNICODE_CHARACTER_CLASSES_H

#include "./staticData.h"

#include <bitset>

namespace pxr {

// Unicode defines a maximum of 17 * 2^16 code points
// not all of these code points are valid code points
// but we need the flags to be contiguous
constexpr uint32_t TF_MAX_CODE_POINT = 1114112;

/// \brief 
/// Provides static initialization of the whether a Unicode code
/// point is contained with the XID_Start set of Unicode character
/// classes.
///
class TfUnicodeXidStartFlagData
{
public:

    TfUnicodeXidStartFlagData();

    /// \brief Determines whether the given code point is contained within
    /// the XID_Start character class.
    /// \param codePoint The Unicode code point to determine inclusion for.
    /// \return true if the given codePoint is in the XID_Start character
    /// class, false otherwise.
    inline bool IsXidStartCodePoint(uint32_t codePoint) const {
        return (codePoint < TF_MAX_CODE_POINT) ? _flags[codePoint] : false;
    }

private:

    std::bitset<TF_MAX_CODE_POINT> _flags;
};

/// \brief 
/// Provides static initialization of the whether a Unicode code
/// point is contained with the XID_Continue set of Unicode character
/// classes.
///
class TfUnicodeXidContinueFlagData
{
public:

    TfUnicodeXidContinueFlagData();

    /// \brief Determines whether the given code point is contained within
    /// the XID_Continue character class.
    /// \param codePoint The Unicode code point to determine inclusion for.
    /// \return true if the given codePoint is in the XID_Continue 
    /// character class false otherwise.
    inline bool IsXidContinueCodePoint(uint32_t codePoint) const {
        return (codePoint < TF_MAX_CODE_POINT) ? _flags[codePoint] : false;
    }

private:
    
    std::bitset<TF_MAX_CODE_POINT> _flags;
};

/// \brief Retrieves character class data for XID_Start.
/// \return An object which can be used to interrogate whether a code point
/// is contained within the XID_Start character class.
const TfUnicodeXidStartFlagData& TfUnicodeGetXidStartFlagData();

/// \brief Retrieves character class data for XID_Continue.
/// \return An object which can be used to interrogate whether a code point
/// is contained within the XID_Continue character class.
const TfUnicodeXidContinueFlagData& TfUnicodeGetXidContinueFlagData();

}  // namespace pxr

#endif // PXR_TF_UNICODE_CHARACTER_CLASSES_H_