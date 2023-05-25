/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_LIB_RESOLUTION_INIT_RESOLVER_H
#define CHPL_LIB_RESOLUTION_INIT_RESOLVER_H

#include "chpl/parsing/parsing-queries.h"
#include "chpl/resolution/resolution-queries.h"
#include "chpl/resolution/resolution-types.h"
#include "chpl/types/all-types.h"

#include <cmath>
#include <cinttypes>

namespace chpl {
namespace resolution {

struct Resolver;

class InitResolver {
  using NameVec = std::vector<BorrowedIdsWithName>;

  // Used over 'ResolutionResultsByPostorderID' due to additional fields.
  struct FieldInitState {
    int ordinalPos = -1;
    ID initPointId = ID();
    types::QualifiedType qt;
    UniqueString name;
    bool isInitialized;
    FieldInitState() = default;
  };

  enum Phase {
    PHASE_NEED_SUPER_INIT,  // Need super.init() calls on all paths.
    PHASE_NEED_COMPLETE,    // Need this.complete() calls on all paths.
    PHASE_COMPLETE          // Fully resolved type past this point.
  };

  Context* ctx_;
  Resolver& initResolver_;
  const uast::Function* fn_;
  const types::Type* initialRecvType_;

  std::map<ID, FieldInitState> fieldToInitState_;
  std::vector<ID> fieldIdsByOrdinal_;
  Phase phase_ = PHASE_NEED_SUPER_INIT;
  int currentFieldIndex_ = 0;
  std::vector<ID> thisCompleteIds_;
  bool isDescendingIntoAssignment_ = false;
  const types::Type* currentRecvType_ = initialRecvType_;

  InitResolver(Context* ctx, Resolver& visitor,
               const uast::Function* fn,
               const types::Type* recvType)
      : ctx_(ctx),
        initResolver_(visitor),
        fn_(fn),
        initialRecvType_(recvType) {
    return;
  }

  bool isCallToSuperInitRequired(void);
  void doSetupInitialState(void);

  bool isFinalReceiverStateValid(void);
  const types::Type* computeReceiverTypeConsideringState(void);

  types::QualifiedType::Kind determineReceiverIntent(void);

  const TypedFnSignature*
  computeTypedSignature(const types::Type* newRecvType);


  void updateResolverVisibleReceiverType(void);
  bool implicitlyResolveFieldType(ID id);

  const uast::AstNode* parentOf(const uast::AstNode* node);
  FieldInitState* fieldStateFromId(ID id);
  FieldInitState* fieldStateFromIndex(int idx);
  bool isMentionOfNodeInLhsOfAssign(const uast::AstNode* node);
  ID fieldIdFromName(UniqueString name);
  ID fieldIdFromPossibleMentionOfField(const uast::AstNode* node);
  bool isFieldInitialized(ID fieldId);

  bool handleCallToThisComplete(const uast::FnCall* node);
  bool handleCallToSuperInit(const uast::FnCall* node);
  bool handleCallToInit(const uast::FnCall* node);
  bool handleAssignmentToField(const uast::OpCall* node);
  ID solveNameConflictByIgnoringField(const NameVec& vec);

  static Phase getMaxPhase(Phase A, Phase B);
  void copyState(InitResolver& other);

public:

  static owned<InitResolver>
  create(Context* context, Resolver& visitor, const uast::Function* fn);

  owned<InitResolver> fork();

  void merge(owned<InitResolver>& A, owned<InitResolver>& B);

  // Called on entry for calls.
  void doDetectPossibleAssignmentToField(const uast::OpCall* node);

  // Called on exit for calls.
  bool handleResolvingCall(const uast::Call* node);

  // Called in exit for dot expressions and identifiers.
  bool handleUseOfField(const uast::AstNode* node);

  // Called on entry for dot expressions and identifiers.
  bool handleResolvingFieldAccess(const uast::Identifier* node);
  bool handleResolvingFieldAccess(const uast::Dot* node);

  // Called to produce the final function signature.
  const TypedFnSignature* finalize(void);

  void checkEarlyReturn(const uast::Return* ret);
};

} // end namespace resolution
} // end namespace chpl

#endif
