/*
 * Copyright 2004-2018 Cray Inc.
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

#include "optimizations.h"

#include "astutil.h"
#include "bb.h"
#include "driver.h"
#include "expr.h"
#include "passes.h"
#include "stlUtil.h"
#include "stmt.h"

//
// Tracking machinery
//

// The AutoTrack class is used to track autoCopy and autoDestroy calls within a
// basic block, and also to record sets of variables that share ownership of a
// specific object.  The meaning of shared ownership is that even though each
// such variable has its own storage and a separate copy of the data, the record
// object has only been "created" once (through a call to chpl__autoCopy) and
// therefore only needs to be "deleted" once (through a call to
// chpl__autoDestroy).
//
// Because they are all mutual aliases, the variables which share ownership of a
// record object are members of an equivalence class.  Each equivalence class is
// represented by a root (containing identifying information) and a linked list
// of members.
class AutoTrack {
 public:
  AutoTrack();
  ~AutoTrack();
 public:
  void move(CallExpr* call);        // Track a move primitive.
  void autoCopy(CallExpr* call);    // Track an autoCopy call.
  void autoDestroy(CallExpr* call); // Track an autoDestroy call.
  bool update();    // Update the current block using tracking info.
 private:
  struct Link {
    CallExpr* call;
    Link* next;
  };
  struct Root {
    Symbol* var;
    int refCnt;
    int destCnt;
    Link* copyHead;
    Link* destHead;
    Root* next;
  };
 private:
  void updateAutoCopy(CallExpr* call);
  void updateAutoDestroy(CallExpr* call);
  Root* getRoot(Symbol* var);
  void insertLink(Link** head, CallExpr* call);
 private:
  bool confPart;
  Root* rootHead;
  Map<Symbol*, Root*> rootMap;
};

AutoTrack::AutoTrack() {
  confPart = true;
  rootHead = NULL;
}

AutoTrack::~AutoTrack() {
  Root* r = rootHead;
  // For each root.
  while (r != NULL) {
    Root* rnext = r->next;
    // Clean out the list of autoCopies.
    Link* p = r->copyHead;
    while (p != NULL) {
      Link* pnext = p->next;
      delete p;
      p = pnext;
    }
    // Clean out the list of autoDestroys.
    p = r->destHead;
    while (p != NULL) {
      Link* pnext = p->next;
      delete p;
      p = pnext;
    }
    delete r;   // Delete the root.
    r = rnext;
  }
}

void AutoTrack::move(CallExpr* call) {
  SymExpr* lhs = toSymExpr(call->get(1));
  SymExpr* rhs = toSymExpr(call->get(2));
  INT_ASSERT(lhs);
  INT_ASSERT(rhs);
  Root* r = getRoot(rhs->symbol());
  rootMap.put(lhs->symbol(), r);
}

void AutoTrack::autoCopy(CallExpr* call) {
  SymExpr* lhs = toSymExpr(call->get(1));
  CallExpr* rhs = toCallExpr(call->get(2));
  INT_ASSERT(lhs);
  INT_ASSERT(rhs);
  SymExpr* se = toSymExpr(rhs->get(1));
  INT_ASSERT(se);
  Root* r = getRoot(se->symbol());
  rootMap.put(lhs->symbol(), r);
  insertLink(&r->copyHead, call);
  if (r->refCnt >= 0)
    r->refCnt++;
}

void AutoTrack::autoDestroy(CallExpr* call) {
  SymExpr* se = toSymExpr(call->get(1));
  INT_ASSERT(se);
  Root* r = getRoot(se->symbol());
  insertLink(&r->destHead, call);
  if (r->refCnt >= 0)
    r->refCnt--;
  r->destCnt++;
}

// Remove only as many autoCopy calls as we have matching autoDestroys.
// Then remove those autodestroys.
bool AutoTrack::update() {
  bool changed = false;
  for (Root* r = rootHead; r != NULL; r = r->next) {
    if (r->refCnt < 0 || (!confPart && r->refCnt > 0))
      continue;
    int destCnt = r->destCnt;
    if (destCnt <= 0)
      continue;
    for (Link* p = r->copyHead; p != NULL; p = p->next) {
      if (destCnt <= 0)
        break;
      updateAutoCopy(p->call);
      destCnt--;
    }
    for (Link* p = r->destHead; p != NULL; p = p->next)
      updateAutoDestroy(p->call);
    changed = true;
  }
  return changed;
}

void AutoTrack::updateAutoCopy(CallExpr* call) {
//fprintf(stderr, "@@@ updateAutoCopy %d\n", call->id);
  CallExpr* rhs = toCallExpr(call->get(2));
  Type* lhsType = call->get(1)->typeInfo();
  Type* rhsType = rhs->get(1)->typeInfo();
  SET_LINENO(call);
  if (lhsType->getValType() == rhsType) {
    rhs->replace(rhs->get(1)->remove());
  } else if (lhsType->getRefType() == rhsType) {
    rhs->replace(new CallExpr(PRIM_DEREF, rhs->get(1)->remove()));
  } else {
    INT_ASSERT("Type mismatch in updateAutoCopy");
  }
}

void AutoTrack::updateAutoDestroy(CallExpr* call) {
//fprintf(stderr, "@@@ updateAutoDestroy %d\n", call->id);
  call->remove();
}

// Gets an existing root for the given symbol, or creates a new one.
AutoTrack::Root* AutoTrack::getRoot(Symbol* var) {
  Root* r = rootMap.get(var);
  if (r != NULL)
    return r;
  r = new Root();
  r->var = var;
  r->refCnt = 0;
  r->destCnt = 0;
  r->copyHead = NULL;
  r->destHead = NULL;
  r->next = rootHead;
  rootHead = r;
  rootMap.put(var, r);
  return r;
}

void AutoTrack::insertLink(Link** head, CallExpr* call) {
  Link* p = new Link();
  p->call = call;
  p->next = *head;
  *head = p;
}

//
// Remove autoCopy (and matching autoDestroy calls) that are
// unnecessary within a function
//
static void removeUnnecessaryAutoCopyCalls(FnSymbol* fn) {
  BasicBlock::buildBasicBlocks(fn);

  bool changed = false;

  // Scan each basic block in turn.
  for_vector(BasicBlock, bb, *fn->basicBlocks) {
    // We only match up and remove autoCopy/autoDestroy pairs
    // within one basic block.
    AutoTrack track;

    // Scan the expressions in this basic block.
    for_vector(Expr, expr, bb->exprs) {
      if (CallExpr* call = toCallExpr(expr)) {
        // If it is a call expression, then we want to see if it is either
        // an  autocopy call or a straight copy of one variable to another.
        // Both of these have a 'move' primitive as the outer call.
        if (isMoveOrAssign(call)) {

          // If the RHS is a SymExpr, then this is a straight move.
          // Record the alias.
          if (toSymExpr(call->get(2))) {
            track.move(call);
          }

          // If the RHS is a CallExpr,
          // then check to see if it is an autoCopy call.
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (FnSymbol* ac = rhs->resolvedFunction()) {
              if (ac->hasFlag(FLAG_REMOVABLE_AUTO_COPY)) {
                // Yup.  So track the autoCopy
                INT_ASSERT(rhs->argList.head);

                SymExpr* se = toSymExpr(call->get(1));

                // Do not remove necessary autoCopies!
                if (!se->symbol()->hasFlag(FLAG_NECESSARY_AUTO_COPY)) {
                  track.autoCopy(call);
                }
              }
            }
          }
        }

        // Check if the call is a removable autoDestroy call.
        if (FnSymbol* ad = call->resolvedFunction()) {
          if (ad->hasFlag(FLAG_REMOVABLE_AUTO_DESTROY)) {
            INT_ASSERT(call->argList.head);

            SymExpr* se = toSymExpr(call->get(1));

            // Global/module-level symbols have lifetimes larger than function
            // scope, so we ignore them here.
            if (se->symbol()->defPoint->parentSymbol == fn) {
              track.autoDestroy(call);
            }
          }
        }
      }
    }

    // Now, look for and delete autoCopy/autoDestroy pairs,
    // and set "changed" if any were removed.
    if (track.update()) {
      changed = true;
    }
  }

#if DEBUG_CP < 2    // That is, disabled if DEBUG_CP >= 2
  // Re-run some other optimizations if progress was made.
  if (changed) {
    if (!fNoCopyPropagation) {
      localCopyPropagation(fn);

      if (!fNoDeadCodeElimination) {
        deadVariableElimination(fn);
      }

      globalCopyPropagation(fn);
      singleAssignmentRefPropagation(fn);
    }
  }
#endif
}

// We can remove the calls to chpl__initCopy (should actually be chpl__autoCopy)
// and corresponding calls to chpl__autoDestroy for Plain-Old-Data (POD) types.
// See isPOD() and propagateNotPOD().
static void removePODinitDestroy()
{
  compute_call_sites();

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_INIT_COPY_FN) || fn->hasFlag(FLAG_AUTO_COPY_FN))
    {
      if (fn->retType == dtVoid)
        // initCopy(void) and autoCopy(void) will have had their void
        // argument removed
        continue;

      // We expect both initCopy and autoCopy functions to have one argument
      // whose type is the same as the return type.
      INT_ASSERT(fn->numFormals() >= 1);

      if (fn->getFormal(1)->type != fn->retType)
        // In some cases, the autoCopy function has a different return type than
        // its argument type.
        // In that case, the replace() below won't work because it will cause a
        // type mismatch at the call sites, so we just punt.
        continue;

      if (isPOD(fn->retType)) {
        forv_Vec(CallExpr, call, *fn->calledBy) {
          Expr* actual = call->get(1);
          ArgSymbol* arg = actual_to_formal(actual);
          if (arg)
          {
            arg->removeFlag(FLAG_INSERT_AUTO_DESTROY);
            arg->removeFlag(FLAG_INSERT_AUTO_DESTROY_FOR_EXPLICIT_NEW);
          }
          else
            INT_FATAL(arg, "Expected autoCopy argument to be a SymExpr.");

          // Bridge out the autoCopy call.

          Type* lhsType = NULL;
          Type* rhsType = actual->typeInfo();

          CallExpr* move = toCallExpr(call->parentExpr);
          INT_ASSERT(isMoveOrAssign(move));
          lhsType = move->get(1)->typeInfo();

          SET_LINENO(call);

          if (lhsType->getValType() != rhsType->getValType()) {
            INT_FATAL(actual, "Type mismatch in updateAutoCopy");
          } else {
            call->replace(actual->remove());
          }
        }
      }
    }
  }
}


void removeUnnecessaryAutoCopyCalls() {
  if (fNoRemoveCopyCalls)
    return;

  //
  // remove pointless initCopy calls, e.g., initCopy calls on records of
  // primitive types
  //
  removePODinitDestroy();

  return; // TODO -- delete more in removeUnnecessaryAutoCopyCalls.

  //
  // remove matched pairs of autoCopy and autoDestroy calls marked with the
  // removable auto destroy pragma
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    removeUnnecessaryAutoCopyCalls(fn);
  }
}
