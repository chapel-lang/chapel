#include "astutil.h"
#include "bb.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
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
  buildBasicBlocks(fn);

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
              if (ac->hasFlag(FLAG_REMOVABLE_AUTO_COPY) && rhs->argList.head) {
                // Yup.  So track the autoCopy
                track.autoCopy(call);
                continue;
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
} 

static bool
isPrimitiveInitCopy(Vec<Type*>& primitiveInitCopyTypeSet, Type* type) {
  if (primitiveInitCopyTypeSet.set_in(type))
    return true;
  if (isRecordWrappedType(type) ||
      isSyncType(type) ||
      type->symbol->hasFlag(FLAG_REF) ||
      type->symbol->hasFlag(FLAG_ITERATOR_RECORD))
    return false;
  if (is_bool_type(type) || is_int_type(type) || is_uint_type(type) ||
      is_real_type(type) || is_imag_type(type) || is_complex_type(type) ||
      is_enum_type(type) ||
      (isClass(type)))
    return true;
  if (isRecord(type)) {
    ClassType* ct = toClassType(type);
    INT_ASSERT(ct);
    for_fields(field, ct) {
      if (!isPrimitiveInitCopy(primitiveInitCopyTypeSet, field->type))
        return false;
    }
    return true;
  }
  return false;
}

void removeUnnecessaryAutoCopyCalls() {
  if (fNoRemoveCopyCalls)
    return;

  //
  // remove pointless initCopy calls, e.g., initCopy calls on records of
  // primitive types
  //
  compute_call_sites();

  Vec<Type*> primitiveInitCopyTypeSet;
  
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_INIT_COPY_FN) &&
        fn->numFormals() == 1 &&
        fn->getFormal(1)->type == fn->retType) {
      if (isPrimitiveInitCopy(primitiveInitCopyTypeSet, fn->retType)) {
        forv_Vec(CallExpr, call, *fn->calledBy) {
          call->replace(call->get(1)->remove());
        }
      }
    }
  }

  //
  // remove matched pairs of autoCopy and autoDestroy calls marked with the
  // removable auto destroy pragma
  //
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    removeUnnecessaryAutoCopyCalls(fn);
  }
}
