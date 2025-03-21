/*
 * Copyright 2021-2025 Hewlett Packard Enterprise Development LP
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

  /** Information about the program's compilation state that's available to
      the programs themselves via global variables. */
  struct CompilerGlobals {
    #define COMPILER_GLOBAL(TYPE__, IDENT__, NAME__) TYPE__ NAME__;
    #include "chpl/uast/compiler-globals-list.h"
    #undef COMPILER_GLOBAL

    static bool update(CompilerGlobals& keep, CompilerGlobals& addin);

    void mark(Context* context) const {}
    void swap(CompilerGlobals& other);
    bool operator==(const CompilerGlobals& other) const;
    bool operator!=(const CompilerGlobals& other) const;
  };

  /** Get the values of compiler-provided global variables. Variables here
      are intended to be those influenced by command-line flags.
   */
  const CompilerGlobals& compilerGlobals(Context* context);

  /**
    Set the values of the various compiler global variables.
   */
  void setCompilerGlobals(Context* Context, CompilerGlobals newValue);

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
