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

#include "CallInitDeinit.h"

#include "Resolver.h"

#include "chpl/resolution/scope-queries.h"

/*#include "chpl/parsing/parsing-queries.h"
#include "chpl/framework/global-strings.h"
#include "chpl/framework/query-impl.h"
#include "chpl/resolution/can-pass.h"
#include "chpl/resolution/disambiguation.h"
#include "chpl/resolution/intents.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/scope-queries.h"
#include "chpl/types/all-types.h"
#include "chpl/uast/all-uast.h"

#include <cstdio>
#include <set>
#include <string>
#include <tuple>
#include <unordered_map>
#include <utility>
#include <vector>
*/

namespace chpl {
namespace resolution {


using namespace uast;
using namespace types;

void CallInitDeinit::process(Resolver& resolver) {
  CallInitDeinit uv(resolver);
  MutatingResolvedVisitor<CallInitDeinit> rv(resolver.context,
                                             resolver.symbol,
                                             uv,
                                             resolver.byPostorder);

  resolver.symbol->traverse(rv);
}

CallInitDeinit::CallInitDeinit(Resolver& resolver)
  : context(resolver.context), resolver(resolver)
{
}

void CallInitDeinit::addDeinitCalls(InitDeinitFrame& frame) {
  for (auto varId : frame.vars) {
    printf("deinit %s\n", varId.str().c_str());
  }
}

void CallInitDeinit::enterScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    printf("ENTER SCOPE %s\n", ast->id().str().c_str());
    const Scope* scope = scopeForId(context, ast->id());
    scopeStack.push_back(InitDeinitFrame(scope));
  }
  /*if (auto d = ast->toDecl()) {
    declStack.push_back(d);
  }*/
}
void CallInitDeinit::exitScope(const AstNode* ast) {
  if (createsScope(ast->tag())) {
    addDeinitCalls(scopeStack.back());
    printf("EXIT SCOPE %s\n", ast->id().str().c_str());
    assert(!scopeStack.empty());
    scopeStack.pop_back();
  }
  /*if (ast->isDecl()) {
    assert(!declStack.empty());
    declStack.pop_back();
  }*/
}

bool CallInitDeinit::enter(const VarLikeDecl* ast, RV& rv) {

  enterScope(ast);

  return true;
}
void CallInitDeinit::exit(const VarLikeDecl* ast, RV& rv) {
  assert(!scopeStack.empty());
  if (!scopeStack.empty()) {
    printf("ADDING VARIABLE %s\n", ast->id().str().c_str());
    InitDeinitFrame& frame = scopeStack.back();
    frame.vars.push_back(ast->id());
  }

  exitScope(ast);
}

// TODO: visit nested calls & record their IDs
// in InitDeinitState to record required deinit actions

bool CallInitDeinit::enter(const OpCall* ast, RV& rv) {
  if (ast->op() == USTR("=")) {
    // TODO: determine which of these cases it really is:
    //  * assignment
    //  * copy initialization
    //  * move initialization
  }
  return true;
}
void CallInitDeinit::exit(const OpCall* ast, RV& rv) {
}


bool CallInitDeinit::enter(const AstNode* ast, RV& rv) {
  enterScope(ast);

  printf("IN CALLINITDEINIT %s\n", ast->id().str().c_str());
  if (rv.hasAst(ast)) {
    rv.byAst(ast).dump();
    printf("\n");
  }
  return true;
}
void CallInitDeinit::exit(const AstNode* ast, RV& rv) {
  exitScope(ast);
}


} // end namespace resolution
} // end namespace chpl
