/*
 * Copyright 2021-2024 Hewlett Packard Enterprise Development LP
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

#include "test-resolution.h"

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/Identifier.h"
#include "chpl/uast/Module.h"
#include "chpl/uast/Record.h"
#include "chpl/uast/Variable.h"

const std::string allGlobalsProgram = ""
  #define COMPILER_GLOBAL(TYPE__, NAME__, FIELD__)\
    "param my" NAME__ " = " NAME__ ";\n"
  #include "chpl/uast/compiler-globals-list.h"
  #undef COMPILER_GLOBAL
  ;

static std::unordered_map<std::string, QualifiedType> compilationGlobalTypes(Context* context) {
  static std::vector<std::string> variables = {
    #define COMPILER_GLOBAL(TYPE__, NAME__, FIELD__) "my" NAME__,
    #include "chpl/uast/compiler-globals-list.h"
    #undef COMPILER_GLOBAL
  };

  return resolveTypesOfVariables(context, allGlobalsProgram, variables);
}

static CompilerGlobals defaultGlobals() {
  CompilerGlobals globals;
  // For the time being, all globals are bools so just set them to false.
  #define COMPILER_GLOBAL(TYPE__, NAME__, FIELD__) globals.FIELD__ = false;
  #include "chpl/uast/compiler-globals-list.h"
  #undef COMPILER_GLOBAL
  return globals;
}

template <typename T>
void verifyGlobal(const QualifiedType& actual, T expected);

template <>
void verifyGlobal<bool>(const QualifiedType& type, bool expected) {
  CHPL_ASSERT(type.isParam() && type.type()->isBoolType());
  CHPL_ASSERT(type.isParamFalse() ^ expected);
}

template <typename F>
void verifyCompilerGlobals(F&& function) {
  // Create the context
  Context ctx;
  Context* context = &ctx;

  // Configure the globals (including running user code to set them)
  CompilerGlobals myGlobals = defaultGlobals();
  function(myGlobals);
  setCompilerGlobals(context, myGlobals);

  std::cout << "--- verifying globals ---" << std::endl;

  // Verify all the globals are as we set them.
  auto types = compilationGlobalTypes(context);
  #define COMPILER_GLOBAL(TYPE__, NAME__, FIELD__)\
    { \
      auto& type = types.at("my" NAME__); \
      std::cout << "Expecting " << myGlobals.FIELD__ << ", got: "; \
      type.dump(); \
      verifyGlobal<TYPE__>(type, myGlobals.FIELD__); \
    }
  #include "chpl/uast/compiler-globals-list.h"
  #undef COMPILER_GLOBAL
}

static void test1() {
  // Default configuration, everything is false.

  verifyCompilerGlobals([](CompilerGlobals& globals) {
    // Do not change the globals; all should be false.
  });
}

static void test2() {
  verifyCompilerGlobals([](CompilerGlobals& globals) {
    globals.boundsChecking = true;
  });
}

static void test3() {
  // Default configuration, everything is false.

  verifyCompilerGlobals([](CompilerGlobals& globals) {
    globals.boundsChecking = true;
    globals.cacheRemote = true;
  });
}


int main() {
  test1();
  test2();
  test3();
}
