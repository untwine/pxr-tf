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
#ifndef PXR_BASE_TF_PREPROCESSOR_UTILS_LITE_H
#define PXR_BASE_TF_PREPROCESSOR_UTILS_LITE_H

// Helper for TF_PP_CAT. This extra indirection is required so that macros get
// expanded before the pasting occurs.
#define TF_PP_CAT_IMPL(x, y) x ## y

/// Paste concatenate preprocessor expressions x and y after expansion.  This
/// is similar to BOOST_PP_CAT but doesn't require including the boost config
/// header which is somewhat heavy.
#define TF_PP_CAT(x, y) TF_PP_CAT_IMPL(x, y)

#endif // PXR_BASE_TF_PREPROCESSOR_UTILS_LITE_H
