/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

#ifndef E_CONSTANT_RT
  #ifdef LAUNCHER
    // By default, expand to nothing under the launcher.
    #define E_CONSTANT_RT(name__, type__)
  #else
    #define E_CONSTANT_RT(name__, type__) E_CONSTANT(name__, type__)
  #endif
#endif

#ifndef E_CALLBACK_RT
  #ifdef LAUNCHER
    // By default, expand to nothing under the launcher.
    #define E_CALLBACK_RT(name__, ret_type__, ...)
  #else
    #define E_CALLBACK_RT(name__, ret_type__, ...) \
      E_CALLBACK(name__, ret_type__, __VA_ARGS__)
  #endif
#endif

#include "chpl-prginfo-data-macro.h"

#undef E_CONSTANT_RT
#undef E_CALLBACK_RT
#undef E_CONSTANT
#undef E_CALLBACK
