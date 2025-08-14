/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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

#ifndef _CONVERT_UAST_H_
#define _CONVERT_UAST_H_

#include "alist.h"
#include "baseAST.h"
#include "ModuleSymbol.h"

#include "chpl/framework/Context.h"
#include "chpl/framework/ID.h"
#include "chpl/resolution/call-graph.h"
#include "chpl/uast/BuilderResult.h"
#include "chpl/uast/Module.h"
#include "chpl/util/memory.h"

#include <vector>
#include <unordered_set>

// base class for Converter and TConverter (typed Converter)
class UastConverter {
 public:
  virtual ~UastConverter();

  // Provide a vector of modules / submodules to be converted.
  // When converting, only convert modules listed here.
  // It will help performance if the order of the modules in the vector
  // matches the order in which they are converted.
  virtual void setModulesToConvert(const std::vector<chpl::ID>& vec) = 0;

  // Provide the set of functions that should be converted with full
  // type information.
  // This doesn't do anything for the untyped Converter.
  virtual void
  setFunctionsToConvertWithTypes(const chpl::resolution::CalledFnsSet& calledFns) = 0;

  virtual void setSymbolsToIgnore(std::unordered_set<chpl::ID> ignore) = 0;
  virtual void eraseSymbolToIgnore(chpl::ID ignore) = 0;

  // Indicate which module is the main module
  virtual void setMainModule(chpl::ID mainModule) = 0;

  // This function helps the TConverter to work with an untyped Converter.
  // It informs the untyped Converter about the ModuleSymbol* for the
  // given module ID. This allows it to update a map to support mentions
  // of that module in code that is converted in an untyped way.
  virtual void useModuleWhenConverting(const chpl::ID& modId,
                                       ModuleSymbol* modSym) = 0;

  // convert a toplevel module
  virtual ModuleSymbol*
  convertToplevelModule(const chpl::uast::Module* mod, ModTag modTag) = 0;

  // convert AST, in an untyped manner
  virtual Expr* convertAST(const chpl::uast::AstNode* node) = 0;
  virtual Expr* convertAST(const chpl::uast::AstNode* node, ModTag modTag) = 0;

  // apply fixups to fix SymExprs to refer to Symbols that
  // might have been created in a different order.
  virtual void postConvertApplyFixups() = 0;

  // Generate main functions (for use with TConverter)
  virtual void createMainFunctions() = 0;

  virtual void noteConvertedSym(const chpl::uast::AstNode* ast, Symbol* sym) = 0;
  virtual void noteConvertedFn(const chpl::resolution::TypedFnSignature* sig, FnSymbol* fn) = 0;
};

chpl::owned<UastConverter> createUntypedConverter(chpl::Context* context);

chpl::owned<UastConverter> createTypedConverter(chpl::Context* context);

#endif
