/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
 * Other additional copyright holders may be indicated within.
 * 
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * 
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _chpl_rt_comp_detect_h_
#define _chpl_rt_comp_detect_h_

#define RT_COMP_UNKNOWN 0
#define RT_COMP_CRAY 1
#define RT_COMP_INTEL 2
#define RT_COMP_CLANG 4
#define RT_COMP_GCC 8
#define RT_COMP_PGI 16

// Detect backend compiler

#if defined(_CRAYC)
#define RT_COMP_CC RT_COMP_CRAY
#elif defined(__INTEL_COMPILER)
#define RT_COMP_CC RT_COMP_INTEL
#elif defined(__clang__)
#define RT_COMP_CC RT_COMP_CLANG
#elif defined(__PGI)
#define RT_COMP_CC RT_COMP_PGI
// Note: This check for gcc must come last since other compilers
// also define __GNUC__ to declare library/extension compatibility
#elif defined(__GNUC__)
#define RT_COMP_CC RT_COMP_GCC
#else
#define RT_COMP_CC RT_COMP_UNKNOWN
#endif


// Detect backend compiler version. This does not provide a single macro
// for all compilers but instead provides a unique macro for each
// compiler since there's no uniform versioning scheme. This primarily
// serves as a means to avoid looking up each compiler's versioning
// scheme (they can be surprisingly hard to find.) These could be
// defined above, but it makes the compiler detection hard to read. See
// the commit message for links to documentation on these macros.

//  Version number format:
//  - Cray  : _RELEASE_MAJOR, _RELEASE_MINOR
//  - Intel : __INTEL_COMPILER
//  - Clang : < Use Feature Checking Macros Instead >
//  - GCC   : __GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__
//  - PGI   : __PGIC__, __PGIC_MINOR__, __PGIC_PATCHLEVEL__

#if RT_COMP_CC == RT_COMP_CRAY
#define RT_COMP_CRAY_VERSION_MAJOR _RELEASE_MAJOR
#define RT_COMP_CRAY_VERSION_MINOR _RELEASE_MINOR

#elif RT_COMP_CC == RT_COMP_INTEL
// Intel only has a single macro for the version and the form isn't well
// documented. I think it's supposed to be VersionMinorPatch, but
// patch seems to always be zero. I *think* minor is always a single
// digit and this seems consistent with Boost's usage.
// For example: 5.0 => 500, 13.1.3 => 1310, 15.0.2 => 1500
#define RT_COMP_INTEL_VERSION __INTEL_COMPILER

#elif RT_COMP_CC == RT_COMP_CLANG
// NOTE: Clang version macros are NOT provided as their version
// macros are the marketing version numbers. From clang's website:
// "Note that marketing version numbers should not be used to check
// for language features, as different vendors use different
// numbering schemes. Instead, use the Feature Checking Macros."

#elif RT_COMP_CC == RT_COMP_PGI
#define RT_COMP_PGI_VERSION_MAJOR __PGIC__
#define RT_COMP_PGI_VERSION_MINOR __PGIC_MINOR__
#define RT_COMP_PGI_VERSION_PATCH __PGIC_PATCHLEVEL__

#elif RT_COMP_CC == RT_COMP_GCC
#define RT_COMP_GCC_VERSION_MAJOR __GNUC__
#define RT_COMP_GCC_VERSION_MINOR __GNUC_MINOR__
#define RT_COMP_GCC_VERSION_PATCH __GNUC_PATCHLEVEL__
// Easier to read macro, mentioned on the GCC predefined macro page,
// so it's safe to assume this will always be valid
#define RT_COMP_GCC_VERSION (__GNUC__ * 10000 \
                                      + __GNUC_MINOR__ * 100 \
                                      + __GNUC_PATCHLEVEL__)
#endif

#endif // _chpl_rt_comp_detect_h_
