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

#ifndef CALL_INIT_DEINIT_H
#define CALL_INIT_DEINIT_H

#include "chpl/resolution/ResolvedVisitor.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/uast/all-uast.h"

namespace chpl {
namespace resolution {

class Resolver;

  // TODO -- this is just for split init
  // TOOD -- use ResolvedVisitor.h for copy and deinit
  // TODO -- figure out where to store copy (associatedFns?)
  //         and where to store deinit (associatedFns not so good)
struct InitDeinitState {
  bool inited;
  bool lastIsCopy;
  bool inScope;
};

struct CallInitDeinit {
  using RV = MutatingResolvedVisitor<CallInitDeinit>;

  // inputs to the process
  Context* context = nullptr;
  Resolver& resolver;

  // internal variables
  std::map<ID, InitDeinitState> varState;

  // main entry point to this code
  // updates the ResolutionResultsByPostorderID
  static void process(Resolver& resolver);

  CallInitDeinit(Resolver& resolver);

  bool enter(const uast::AstNode* node, RV& rv);
  void exit(const uast::AstNode* node, RV& rv);

};


} // end namespace resolution
} // end namespace chpl

#endif
