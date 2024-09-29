// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_TF_DIAGNOSTIC_HELPER_H
#define PXR_TF_DIAGNOSTIC_HELPER_H

#include "./api.h"
#include <pxr/arch/attributes.h>
#include <pxr/arch/defines.h>

// XXX: This include is a hack to avoid build errors due to
// incompatible macro definitions in pyport.h on macOS.
#include <locale>

#include <any>
#include <string>

// Follow up changes should more tightly scope these to just where it's needed
// in pxr.
#if defined(ARCH_OS_LINUX) || defined(ARCH_OS_DARWIN)
// Include <unistd.h> to provide _exit for tf/debugger.cpp and dependencies
// that were previously transitively getting this from boost
#include <unistd.h>
// Include <cstring> to provide memset, memcmp, and memcpy for dependencies
// that were previously transitively getting them from boost
#include <cstring>
#endif

namespace pxr {

typedef std::any TfDiagnosticInfo;
class TfCallContext;
enum TfDiagnosticType : int;
class TfEnum;
class TfError;

TF_API void
Tf_PostErrorHelper(
    const TfCallContext &context,
    const TfEnum &code,
    const std::string &msg);

TF_API void
Tf_PostErrorHelper(
    const TfCallContext &context,
    TfDiagnosticType code,
    const std::string &msg);

TF_API void
Tf_PostErrorHelper(
    const TfCallContext &context,
    const TfEnum &code,
    const char *fmt, ...) ARCH_PRINTF_FUNCTION(3, 4);

TF_API void
Tf_PostErrorHelper(
    const TfCallContext &context,
    TfDiagnosticType code,
    const char *fmt, ...) ARCH_PRINTF_FUNCTION(3, 4);

TF_API void
Tf_PostErrorHelper(
    const TfCallContext &context,
    const TfDiagnosticInfo &info,
    const TfEnum &code,
    const std::string &msg);

TF_API void
Tf_PostErrorHelper(
    const TfCallContext &context,
    const TfDiagnosticInfo &info,
    const TfEnum &code,
    const char *fmt, ...) ARCH_PRINTF_FUNCTION(4, 5);

TF_API void
Tf_PostQuietlyErrorHelper(
    const TfCallContext &context,
    const TfEnum &code,
    const TfDiagnosticInfo &info,
    const std::string &msg);

TF_API void
Tf_PostQuietlyErrorHelper(
    const TfCallContext &context,
    const TfEnum &code,
    const TfDiagnosticInfo &info,
    const char *fmt, ...) ARCH_PRINTF_FUNCTION(4, 5);

TF_API void
Tf_PostQuietlyErrorHelper(
    const TfCallContext &context,
    const TfEnum &code,
    const std::string &msg);

TF_API void
Tf_PostQuietlyErrorHelper(
    const TfCallContext &context,
    const TfEnum &code,
    const char *fmt, ...) ARCH_PRINTF_FUNCTION(3, 4);


// Helper functions for posting a warning with TF_WARN.
TF_API void
Tf_PostWarningHelper(const TfCallContext &context,
                     const std::string &msg);

TF_API void
Tf_PostWarningHelper(const TfCallContext &context,
                     const char *fmt, ...)  ARCH_PRINTF_FUNCTION(2, 3);

TF_API void
Tf_PostWarningHelper(
    const TfCallContext &context,
    const TfEnum &code,
    const std::string &msg);

TF_API void
Tf_PostWarningHelper(
    const TfCallContext &context,
    TfDiagnosticType code,
    const std::string &msg);

TF_API void
Tf_PostWarningHelper(
    const TfCallContext &context,
    const TfEnum &code,
    const char *fmt, ...) ARCH_PRINTF_FUNCTION(3, 4);

TF_API void
Tf_PostWarningHelper(
    const TfCallContext &context,
    TfDiagnosticType code,
    const char *fmt, ...) ARCH_PRINTF_FUNCTION(3, 4);

TF_API void
Tf_PostWarningHelper(
    const TfCallContext &context,
    const TfDiagnosticInfo &info,
    const TfEnum &code,
    const std::string &msg);

TF_API void
Tf_PostWarningHelper(
    const TfCallContext &context,
    const TfDiagnosticInfo &info,
    const TfEnum &code,
    const char *fmt, ...) ARCH_PRINTF_FUNCTION(4, 5);

TF_API void
Tf_PostStatusHelper(
    const TfCallContext &context, 
    const char *fmt, ...) ARCH_PRINTF_FUNCTION(2, 3);

TF_API void
Tf_PostStatusHelper(
    const TfCallContext &context, 
    const std::string &msg);


TF_API void
Tf_PostStatusHelper(
    const TfCallContext &context,
    const TfEnum &code,
    const std::string &msg);

TF_API void
Tf_PostStatusHelper(
    const TfCallContext &context, 
    const TfEnum &code,
    const char *fmt, ...) ARCH_PRINTF_FUNCTION(3, 4);

TF_API void
Tf_PostStatusHelper(
    const TfCallContext &context,
    const TfDiagnosticInfo &info,
    const TfEnum &code,
    const std::string &msg);

TF_API void
Tf_PostStatusHelper(
    const TfCallContext &context,
    const TfDiagnosticInfo &info,
    const TfEnum &code,
    const char *fmt, ...) ARCH_PRINTF_FUNCTION(4, 5);

}  // namespace pxr

#endif // PXR_TF_DIAGNOSTIC_HELPER_H
