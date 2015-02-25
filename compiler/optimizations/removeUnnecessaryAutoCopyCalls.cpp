/*
 * Copyright 2004-2015 Cray Inc.
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
  Root* r = getRoot(rhs->var);
  rootMap.put(lhs->var, r);
}

void AutoTrack::autoCopy(CallExpr* call) {
  SymExpr* lhs = toSymExpr(call->get(1));
  CallExpr* rhs = toCallExpr(call->get(2));
  INT_ASSERT(lhs);
  INT_ASSERT(rhs);
  SymExpr* se = toSymExpr(rhs->get(1));
  INT_ASSERT(se);
  Root* r = getRoot(se->var);
  rootMap.put(lhs->var, r);
  insertLink(&r->copyHead, call);
  if (r->refCnt >= 0)
    r->refCnt++;
}

void AutoTrack::autoDestroy(CallExpr* call) {
  SymExpr* se = toSymExpr(call->get(1));
  INT_ASSERT(se);
  Root* r = getRoot(se->var);
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
  rhs->replace(rhs->get(1)->remove());
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
    // We only match up and remove autoCopy/autoDestroy pairs within one basic block.
    AutoTrack track;
    // Scan the expressions in this basic block.
    for_vector(Expr, expr, bb->exprs) {
      if (CallExpr* call = toCallExpr(expr)) {
        // If it is a call expression, then we want to see if it is either an
        // autocopy call or a straight copy of one variable to another.  Both of
        // these have a 'move' primitive as the outer call.
        if (call->isPrimitive(PRIM_MOVE)) {

          // If the RHS is a SymExpr, then this is a straight move. Record the alias.
          if (toSymExpr(call->get(2)))
            track.move(call);

          // If the RHS is a CallExpr, then check to see if it is an autoCopy call.
          if (CallExpr* rhs = toCallExpr(call->get(2))) {
            if (FnSymbol* ac = rhs->isResolved()) {
              if (ac->hasFlag(FLAG_REMOVABLE_AUTO_COPY)) {
                // Yup.  So track the autoCopy
                INT_ASSERT(rhs->argList.head);
                SymExpr* se = toSymExpr(call->get(1));
                // Do not remove necessary autoCopies!
                if (!se->var->hasFlag(FLAG_NECESSARY_AUTO_COPY))
                  track.autoCopy(call);
              }
            }
          }
        }

        // Check if the call is a removable autoDestroy call.
        if (FnSymbol* ad = call->isResolved()) {
          if (ad->hasFlag(FLAG_REMOVABLE_AUTO_DESTROY)) {
            INT_ASSERT(call->argList.head);
            SymExpr* se = toSymExpr(call->get(1));
            // Global/module-level symbols have lifetimes larger than function
            // scope, so we ignore them here.
            if (se->var->defPoint->parentSymbol == fn)
              track.autoDestroy(call);
          }
        }
      }
    }

    // Now, look for and delete autoCopy/autoDestroy pairs, and set "changed" if
    // any were removed.
    if (track.update())
      changed = true;
  }

#if DEBUG_CP < 2    // That is, disabled if DEBUG_CP >= 2
  // Re-run some other optimizations if progress was made.  
  if (changed) {
    if (!fNoCopyPropagation) {
      localCopyPropagation(fn);
      if (!fNoDeadCodeElimination)
        deadVariableElimination(fn);
      globalCopyPropagation(fn);
      singleAssignmentRefPropagation(fn);
    }  
    if (!fNoDeadCodeElimination)
      deadCodeElimination(fn);
  }
#endif
} 

// A Plain-Old Data type is one whose default-constructor, copy-constructor,
// assignment operator and destructor are all trivial.  Trivial means that they
// have no observable side-effects beyond the default behavior provided by
// the compiler.
// While it is possible for the user to write ones of these functions that
// override the default and exhibit the same behavior as the compiler-supplied
// ones and are thus behaviorally trivial, for practical purposes we choose to
// treat as nontrivial any user-supplied versions of these functions.

// This routine traverses all defined functions, and for those of interest
// (being one of the four types above) determines if it is user-defined and
// finds the class to which it belongs.  It then marks that class as being a
// non-POD class (since the rules for it being a POD class have been violated).
//
// The assignment operator is not currently a member function, so we infer the
// target type from the type LHS argument.
// TODO: Add clauses for the copy-constructor and default-constructor.
//
// To yield a result that is strictly correct w.r.t. whether a class is POD or
// non-POD, this routine should be run after resolution but before the
// post-resolution pruning pass.  However, even though it is run after pruning,
// the results are correct for our immediate use.  That is because if a function
// is pruned, that means it is not called.  And if it is not called, whether it
// is trivial or not is moot question.
// TODO: Move this routine to a pass that precedes the post-resolution pruning pass.
//
static void
markPODtypes()
{
  forv_Vec(FnSymbol, fn, gFnSymbols)
  {
    // Ignore invisible (compiler-defined) functions.
    if (fn->hasFlag(FLAG_COMPILER_GENERATED))
      continue;

    // Decide whether to mark a class based on this function.
    enum Flag flag = FLAG_UNKNOWN;
    AggregateType* ct = NULL;

    if (fn->hasFlag(FLAG_DESTRUCTOR))
    {
      // ~chpl_destroy(_mt, obj);
      ArgSymbol* this_arg = fn->getFormal(1);
      INT_ASSERT(!strcmp(this_arg->name, "this"));
      ct = toAggregateType(this_arg->type);
      if (ct)
        flag = FLAG_HAS_USER_DESTRUCTOR;
    }

    if (fn->hasFlag(FLAG_INIT_COPY_FN))
    {
      // chpl__initCopy(obj);
      if (fn->numFormals() > 0)
      {
        ArgSymbol* arg = fn->getFormal(1);
        ct = toAggregateType(arg->type);
        if (ct)
          flag = FLAG_HAS_USER_INIT_COPY_FN;
      }
    }

    // We should probably use a flag to distinguish assignment operators.  Then
    // we can test easily that fn->hasFlag(FLAG_HAS_USER_DESTRUCTOR) and
    // fn->hasFlag(FLAG_HAS_USER_ASSIGNMENT) are mutually exclusive.
    if (!strcmp(fn->name,"="))
    {
      ArgSymbol* lhs = fn->getFormal(1);
      ct = toAggregateType(lhs->type);
      if (ct)
        flag = FLAG_HAS_USER_ASSIGNMENT;
    }

    if (flag != FLAG_UNKNOWN)
      ct->symbol->addFlag(flag);
  }
}


static bool
isPrimitiveCopy(Vec<Type*>& primitiveCopyTypeSet, Type* type) {
  if (primitiveCopyTypeSet.set_in(type))
    return true;
  if (is_bool_type(type) || is_int_type(type) || is_uint_type(type) ||
      is_real_type(type) || is_imag_type(type) || is_complex_type(type) ||
      is_enum_type(type) ||
      (isClass(type)))
    return true;
// These cases should be captured by the test for nontrivial assignment and/or
//  destruction below.  Those cases still appearing in the conditional are
//  implemented internally, rather than in module code, so until they are
//  implemented in module code we have to carry along these special cases.
//  if (isRecordWrappedType(type) ||
//      type->symbol->hasFlag(FLAG_REF) ||
//      type->symbol->hasFlag(FLAG_ITERATOR_RECORD))
  if (isSyncType(type))
    return false;
  if (isRecord(type)) {
    AggregateType* ct = toAggregateType(type);
    INT_ASSERT(ct);

    // If this class defines an assignment or initCopy or destructor, it is not
    // POD.
    if (ct->symbol->hasFlag(FLAG_HAS_USER_DESTRUCTOR) ||
        ct->symbol->hasFlag(FLAG_HAS_USER_INIT_COPY_FN) ||
        ct->symbol->hasFlag(FLAG_HAS_USER_ASSIGNMENT))
      return false;

    for_fields(field, ct) {
      if (!isPrimitiveCopy(primitiveCopyTypeSet, field->type))
        return false;
    }
    return true;
  }
  return false;
}

// We can remove the calls to chpl__initCopy (should actually be chpl__autoCopy)
// and corresponding calls to chpl__autoDestroy for Plain-Old-Data (POD) types.
// POD types are fundamental types, class types, and records that contain only
// POD-typed fields (see isPODtype test above).
static void removePODinitDestroy()
{
  markPODtypes();
  compute_call_sites();

  Vec<Type*> primitiveCopyTypeSet;
  
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_INIT_COPY_FN) || fn->hasFlag(FLAG_AUTO_COPY_FN))
    {
      // We expect both initCopy and autoCopy functions to have one argument
      // whose type is the same as the return type.
      if (fn->numFormals() == 0 && fn->retType == dtNil)
        // This is an oddity.  It should probably be pruned earlier, and this
        // test turned into a per-pass verification or removed entirely.
        continue; 

      INT_ASSERT(fn->numFormals() == 1);

      if (fn->getFormal(1)->type != fn->retType)
        // In some cases, the autoCopy function has a different return type than
        // its argument type.
        // In that case, the replace() below won't work because it will cause a
        // type mismatch at the call sites, so we just punt.
        continue;

      if (isPrimitiveCopy(primitiveCopyTypeSet, fn->retType)) {
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
          call->replace(actual->remove());
        }
      }
    }
  }
}


void removeUnnecessaryAutoCopyCalls() {
  if (fNoRemoveCopyCalls || 1) // Disable this pass for a test.
    return;

  //
  // remove pointless initCopy calls, e.g., initCopy calls on records of
  // primitive types
  //
  removePODinitDestroy();

  //
  // remove matched pairs of autoCopy and autoDestroy calls marked with the
  // removable auto destroy pragma
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    removeUnnecessaryAutoCopyCalls(fn);
  }
}
