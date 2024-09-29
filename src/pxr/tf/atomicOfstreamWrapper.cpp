// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

///
/// \file Tf/AtomicOfstreamWrapper.cpp

#include "./atomicOfstreamWrapper.h"

#include <pxr/arch/defines.h>
#include <pxr/arch/errno.h>
#include <pxr/arch/fileSystem.h>

#include "./atomicRenameUtil.h"
#include "./diagnostic.h"
#include "./fileUtils.h"
#include "./pathUtils.h"
#include "./stringUtils.h"

#include <iostream>
#include <cerrno>
#include <cstdio>

#if defined(ARCH_OS_WINDOWS)
#include <Windows.h>
#include <io.h>
#endif

namespace pxr {

using std::string;

TfAtomicOfstreamWrapper::TfAtomicOfstreamWrapper(
    const string& filePath)
    : _filePath(filePath)
{
    // Do Nothing.
}

TfAtomicOfstreamWrapper::~TfAtomicOfstreamWrapper()
{
    Cancel();
}

bool
TfAtomicOfstreamWrapper::Open(
    string* reason)
{
    if (_stream.is_open()) {
        if (reason) {
            *reason = "Stream is already open";
        }
        return false;
    }

    std::string localError, *err = reason ? reason : &localError;
    int tmpFd = Tf_CreateSiblingTempFile(
        _filePath, &_filePath, &_tmpFilePath, err);
    if (tmpFd == -1) {
        return false;
    }
    
    // Close the temp file descriptor returned by Arch, and open this buffer
    // with the same file name.
    ArchCloseFile(tmpFd);

#if defined(ARCH_OS_WINDOWS)
    _stream.open(ArchWindowsUtf8ToUtf16(_tmpFilePath).c_str(),
                 std::fstream::out|std::fstream::binary|std::fstream::trunc);
#else
    _stream.open(_tmpFilePath.c_str(),
        std::fstream::out | std::fstream::binary | std::fstream::trunc);
#endif
    if (!_stream) {
        if (reason) {
            *reason = TfStringPrintf(
                "Unable to open '%s' for writing: %s",
                _tmpFilePath.c_str(), ArchStrerror().c_str());
        }
        return false;
    }

    return true;
}

bool
TfAtomicOfstreamWrapper::Commit(
    string* reason)
{
    if (!_stream.is_open()) {
        if (reason) {
            *reason = "Stream is not open";
        }
        return false;
    }

    // Flush any pending writes to disk and close the temporary file stream
    // before calling rename.
    _stream.close();

    std::string localError, *err = reason ? reason : &localError;
    return Tf_AtomicRenameFileOver(_tmpFilePath, _filePath, err);
}

bool
TfAtomicOfstreamWrapper::Cancel(
    string* reason)
{
    if (!_stream.is_open()) {
        if (reason) {
            *reason = "Buffer is not open";
        }
        return false;
    }

    // Flush any pending writes to disk and close the temporary file stream
    // before unlinking the temporary file.
    _stream.close();

    bool success = true;

    if (ArchUnlinkFile(_tmpFilePath.c_str()) != 0) {
        if (errno != ENOENT) {
            if (reason) {
                *reason = TfStringPrintf(
                    "Unable to remove temporary file '%s': %s",
                    _tmpFilePath.c_str(),
                    ArchStrerror(errno).c_str());
            }
            success = false;
        }
    }

    return success;
}

}  // namespace pxr
