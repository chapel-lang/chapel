/*
 * Copyright 2004-2019 Cray Inc.
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

#include "chpl-comp-detect-macros.h"

/* This is a portability shim that allows us to use cstdlib atomics if the
 * compiler supports them, and falls back to intrinsics if it doesn't. This
 * allows us to build our module with a version that doesn't support cstdlib
 * atomics, but still allows the generated code to take advantage of them if
 * the user has a new enough compiler. */

// GCC >= 5.0
#if RT_COMP_CC == RT_COMP_GCC
  #if RT_COMP_GCC_VERSION_MAJOR >= 5 && !defined(__STDC_NO_ATOMICS__)
      #define USE_CSTDLIB 1
  #endif
// Intel >= 18.0 with GCC >= 5.0
#elif RT_COMP_CC == RT_COMP_INTEL
  #if RT_COMP_INTEL_VERSION >= 1800 && __GNUC__ >= 5 && !defined(__STDC_NO_ATOMICS__)
      #define USE_CSTDLIB 1
  #endif
// Cray >= 8.7.7
#elif RT_COMP_CC == RT_COMP_CRAY
  #if ((RT_COMP_CRAY_VERSION_MAJOR == 8 && RT_COMP_CRAY_VERSION_MINOR == 7 && RT_COMP_CRAY_VERSION_PATCH >= 7) || \
  #if (RT_COMP_CRAY_VERSION_MAJOR == 8 && RT_COMP_CRAY_VERSION_MINOR >= 8) || \
       RT_COMP_CRAY_VERSION_MAJOR > 8 ) && !defined(__STDC_NO_ATOMICS__)
      #define USE_CSTDLIB 1
  #endif
// Clang or others that support C11 and haven't opted out of atomic support
#elif __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_ATOMICS__)
  #define USE_CSTDLIB 1
#endif

#ifdef USE_CSTDLIB
#undef USE_CSTDLIB
#include "../cstdlib/chpl-atomics.h"
#else
#include "../intrinsics/chpl-atomics.h"
#endif
