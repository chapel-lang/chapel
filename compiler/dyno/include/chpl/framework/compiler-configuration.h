/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_FRAMEWORK_COMPILER_CONFIGURATION_H
#define CHPL_FRAMEWORK_COMPILER_CONFIGURATION_H

#include "chpl/framework/CompilerFlags.h"

namespace chpl {


  /**
    Get the flags for the current session. The class returned by this
    query is not intended to store every possible flag - rather, it
    acts as a convenient place to store binary flags in a single place.
  */
  const CompilerFlags& compilerFlags(Context* context);

  /**
    Set the compiler flags to be used during the current revision.
  */
  void setCompilerFlags(Context* context, CompilerFlags list);

  /**
    Returns true if a given flag is set in the current revision.
  */
  bool isCompilerFlagSet(Context* context, CompilerFlags::Name flag);

} // end namespace chpl

#endif
