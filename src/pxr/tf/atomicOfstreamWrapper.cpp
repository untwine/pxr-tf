//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
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
    arch::CloseFile(tmpFd);

#if defined(ARCH_OS_WINDOWS)
    _stream.open(arch::WindowsUtf8ToUtf16(_tmpFilePath).c_str(),
                 std::fstream::out|std::fstream::binary|std::fstream::trunc);
#else
    _stream.open(_tmpFilePath.c_str(),
        std::fstream::out | std::fstream::binary | std::fstream::trunc);
#endif
    if (!_stream) {
        if (reason) {
            *reason = TfStringPrintf(
                "Unable to open '%s' for writing: %s",
                _tmpFilePath.c_str(), arch::Strerror().c_str());
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

    if (arch::UnlinkFile(_tmpFilePath.c_str()) != 0) {
        if (errno != ENOENT) {
            if (reason) {
                *reason = TfStringPrintf(
                    "Unable to remove temporary file '%s': %s",
                    _tmpFilePath.c_str(),
                    arch::Strerror(errno).c_str());
            }
            success = false;
        }
    }

    return success;
}

}  // namespace pxr
