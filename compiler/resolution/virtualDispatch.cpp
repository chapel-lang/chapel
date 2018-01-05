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

/*

-----------
addToVirtualMaps, addAllToVirtualMaps, buildVirtualMaps

buildVirtualMaps invokes addAllToVirtualMaps on each method 'fn' declared in
in each class.  Only non-generic classes and instantiations of generic classes
are considered.

addAllToVirtualMaps invokes addToVirtualMaps on each (transitive) dispatch
child for that class, passing that same method 'fn'.

addToVirtualMaps adds to the virtual maps all methods
in that dispatch child that could override the above 'fn'.

These functions go down the inheritance hierarchy because in the current
language, a call to a class with static type could dispatch to any child call.

If calling an arbitrary super method is supported, a call to an object of
static child type could end up calling something in the parent.

-----------
*/



#include "virtualDispatch.h"

#include "baseAST.h"
#include "driver.h"
#include "expr.h"
#include "iterator.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "stmt.h"
#include "symbol.h"

#include <set>
#include <vector>

bool                            inDynamicDispatchResolution = false;

Map<FnSymbol*, Vec<FnSymbol*>*> virtualRootsMap;

Map<FnSymbol*, Vec<FnSymbol*>*> virtualChildrenMap;

Map<Type*,     Vec<FnSymbol*>*> virtualMethodTable;

Map<FnSymbol*, int>             virtualMethodMap;

static bool buildVirtualMaps();

static void clearRootsAndChildren();

static void buildVirtualMethodTable();

static void buildVirtualMethodMap();

static void filterVirtualChildren();

static void printDispatchInfo();

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void resolveDynamicDispatches() {
  inDynamicDispatchResolution = true;

  // Repeat until maps are stable
  while (buildVirtualMaps() == false) {
    clearRootsAndChildren();
  }

  buildVirtualMethodTable();

  buildVirtualMethodMap();

  filterVirtualChildren();

  if (fPrintDispatch == true) {
    printDispatchInfo();
  }

  inDynamicDispatchResolution = false;
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void addAllToVirtualMaps(FnSymbol*      pfn,
                                AggregateType* pct);

static void addToVirtualMaps(FnSymbol*      pfn,
                             AggregateType* ct,
                             FnSymbol*      cfn);

static void collectMethods(FnSymbol*               pfn,
                           AggregateType*          ct,
                           std::vector<FnSymbol*>& methods);

static bool possibleSignatureMatch(FnSymbol* fn, FnSymbol* gn);

static void resolveOverride(FnSymbol* pfn, FnSymbol* cfn);

static void overrideIterator(FnSymbol* pfn, FnSymbol* cfn);

static void virtualDispatchUpdate(FnSymbol* pfn, FnSymbol* cfn);

static void virtualDispatchUpdateChildren(FnSymbol* pfn, FnSymbol* cfn);

static void virtualDispatchUpdateRoots(FnSymbol* pfn, FnSymbol* cfn);

static bool isVirtualChild(FnSymbol* child, FnSymbol* parent);

static bool isSubType(Type* sub, Type* super);

// Returns true if the maps are "stable" i.e. set of types is unchanged
static bool buildVirtualMaps() {
  int numTypes = gTypeSymbols.n;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (AggregateType* at = fn->getReceiver()) {
      if (at->isClass() == true) {
        if (at->isGeneric() == false) {
          if (fn->isResolved()            == true      &&

              fn->hasFlag(FLAG_WRAPPER)   == false     &&
              fn->hasFlag(FLAG_NO_PARENS) == false     &&

              fn->retTag                  != RET_PARAM &&
              fn->retTag                  != RET_TYPE) {
            addAllToVirtualMaps(fn, at);
          }
        }
      }
    }
  }

  return (numTypes == gTypeSymbols.n) ? true : false;
}

// Add overrides of pfn to virtual maps down the inheritance hierarchy
static void addAllToVirtualMaps(FnSymbol* pfn, AggregateType* pct) {
  forv_Vec(AggregateType, ct, pct->dispatchChildren) {
    if (ct->isGeneric() == false) {
      if (ct->mayHaveInstances() == true) {
        std::vector<FnSymbol*> methods;

        collectMethods(pfn, ct, methods);

        for_vector(FnSymbol, cfn, methods) {
          addToVirtualMaps(pfn, ct, cfn);
        }
      }

      // Recurse over this child's children
      addAllToVirtualMaps(pfn, ct);
    }
  }
}

static void addToVirtualMaps(FnSymbol*      pfn,
                             AggregateType* ct,
                             FnSymbol*      cfn) {
  ArgSymbol*     _this   = cfn->getFormal(2);
  AggregateType* _thisAt = toAggregateType(_this->type);
  SymbolMap      subs;

  if (_thisAt->isGeneric() == true) {
    subs.put(_this, ct->symbol);
  }

  for (int i = 3; i <= cfn->numFormals(); i++) {
    ArgSymbol* carg = cfn->getFormal(i);
    ArgSymbol* parg = pfn->getFormal(i);

    if (carg->intent == INTENT_PARAM) {
      subs.put(carg, paramMap.get(parg));

    } else if (carg->type->symbol->hasFlag(FLAG_GENERIC) == true) {
      subs.put(carg, parg->type->symbol);
    }
  }

  if (subs.n == 0) {
    resolveOverride(pfn, cfn);

  } else {
    FnSymbol* fn = instantiate(cfn, subs);

    if (ct->hasInitializers() == false) {
      FnSymbol*  typeConstr         = ct->defaultTypeConstructor;
      BlockStmt* instantiationPoint = typeConstr->instantiationPoint;

      if (instantiationPoint == NULL) {
        instantiationPoint = toBlockStmt(typeConstr->defPoint->parentExpr);
      }

      fn->instantiationPoint = instantiationPoint;
    }

    resolveOverride(pfn, fn);
  }
}

static void collectMethods(FnSymbol*               pfn,
                           AggregateType*          pct,
                           std::vector<FnSymbol*>& methods) {
  AggregateType* fromType = pct;

  while (fromType != NULL) {
    forv_Vec(FnSymbol, cfn, fromType->methods) {
      if (cfn->instantiatedFrom == NULL) {
        if (possibleSignatureMatch(pfn, cfn) == true) {
          methods.push_back(cfn);
        }
      }
    }

    fromType = fromType->instantiatedFrom;
  }
}

static bool possibleSignatureMatch(FnSymbol* fn, FnSymbol* gn) {
  bool retval = true;

  if (fn->name != gn->name) {
    retval = false;

  } else if (fn->numFormals() != gn->numFormals()) {
    retval = false;

  } else {
    for (int i = 3; i <= fn->numFormals() && retval == true; i++) {
      ArgSymbol* fa = fn->getFormal(i);
      ArgSymbol* ga = gn->getFormal(i);

      if (strcmp(fa->name, ga->name) != 0) {
        retval = false;
      }
    }
  }

  return retval;
}

static void resolveOverride(FnSymbol* pfn, FnSymbol* cfn) {
  resolveSignature(cfn);

  if (signatureMatch(pfn, cfn) == true && evaluateWhereClause(cfn) == true) {
    resolveFunction(cfn);

    if (cfn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) == true &&
        pfn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) == true) {
      overrideIterator(pfn, cfn);

    } else if (isSubType(cfn->retType, pfn->retType) == false) {
      USR_FATAL_CONT(pfn,
                     "conflicting return type specified for '%s: %s'",
                     toString(pfn),
                     pfn->retType->symbol->name);

      USR_FATAL_CONT(cfn,
                     "  overridden by '%s: %s'",
                     toString(cfn),
                     cfn->retType->symbol->name);

      USR_STOP();

    } else if (cfn->throwsError() != pfn->throwsError()) {
      const char* pfnThrowing = NULL;
      const char* cfnThrowing = NULL;

      if (pfn->throwsError()) {
        pfnThrowing = "throwing";
        cfnThrowing = "non-throwing";

      } else {
        pfnThrowing = "non-throwing";
        cfnThrowing = "throwing";
      }

      USR_FATAL_CONT(cfn, "conflicting throws for '%s'", toString(cfn));

      USR_FATAL_CONT(pfn, "%s function '%s'", pfnThrowing, toString(pfn));

      USR_FATAL_CONT(cfn,
                     "overridden by %s function '%s'",
                     cfnThrowing,
                     toString(cfn));

      USR_STOP();

    } else {
      virtualDispatchUpdate(pfn, cfn);
    }
  }
}

static void overrideIterator(FnSymbol* pfn, FnSymbol* cfn) {
  AggregateType* pfnRetType = toAggregateType(pfn->retType);
  IteratorInfo*  pfnInfo    = pfnRetType->iteratorInfo;
  FnSymbol*      pfnValue   = pfnInfo->getValue;

  AggregateType* cfnRetType = toAggregateType(cfn->retType);
  IteratorInfo*  cfnInfo    = cfnRetType->iteratorInfo;
  FnSymbol*      cfnValue   = cfnInfo->getValue;

  if (isSubType(cfnValue->retType, pfnValue->retType) == true) {
    AggregateType* pic          = pfnInfo->iclass;
    AggregateType* cic          = cfnInfo->iclass;

    Type*          pthisType    = pfnInfo->iterator->_this->typeInfo();
    Type*          cthisType    = cfnInfo->iterator->_this->typeInfo();

    AggregateType* atPfnRetType = toAggregateType(pfn->retType);
    AggregateType* atCfnRetType = toAggregateType(cfn->retType);

    AggregateType* atCthisType  = toAggregateType(cthisType);

    INT_ASSERT(atPfnRetType != NULL);
    INT_ASSERT(atCfnRetType != NULL);
    INT_ASSERT(atCthisType  != NULL);

    atPfnRetType->dispatchChildren.add_exclusive(atCfnRetType);
    atCfnRetType->dispatchParents.add_exclusive(atPfnRetType);

    INT_ASSERT(cic->symbol->hasFlag(FLAG_ITERATOR_CLASS) == true);
    INT_ASSERT(atCthisType->dispatchParents.n              == 1);

    if (atCthisType->dispatchParents.only() == pthisType) {
      AggregateType* parent = cic->dispatchParents.only();

      INT_ASSERT(cic->dispatchParents.n == 1);

      if (parent == dtObject) {
        AggregateType* atCic = toAggregateType(cic);

        INT_ASSERT(atCic != NULL);

        int            item  = parent->dispatchChildren.index(atCic);

        parent->dispatchChildren.remove(item);

        cic->dispatchParents.remove(0);
      }

      AggregateType* atCic = toAggregateType(cic);
      AggregateType* atPic = toAggregateType(pic);

      INT_ASSERT(atCic != NULL);
      INT_ASSERT(atPic != NULL);

      pic->dispatchChildren.add_exclusive(atCic);
      cic->dispatchParents.add_exclusive(atPic);
    }

  } else {
    USR_FATAL_CONT(pfn,
                   "conflicting return type specified for '%s: %s'",
                   toString(pfn),
                   pfnInfo->getValue->retType->symbol->name);

    USR_FATAL_CONT(cfn,
                   "  overridden by '%s: %s'",
                   toString(cfn),
                   cfnInfo->getValue->retType->symbol->name);

    USR_STOP();
  }
}

static bool isSubType(Type* sub, Type* super) {
  bool retval = false;

  if (sub == super) {
    retval = true;

  } else if (AggregateType* atSub = toAggregateType(sub)) {
    forv_Vec(AggregateType, parent, atSub->dispatchParents) {
      if (isSubType(parent, super) == true) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

static void virtualDispatchUpdate(FnSymbol* pfn, FnSymbol* cfn) {
  cfn->addFlag(FLAG_VIRTUAL);
  pfn->addFlag(FLAG_VIRTUAL);

  // There is the potential for a data dependency between these
  virtualDispatchUpdateChildren(pfn, cfn);
  virtualDispatchUpdateRoots(pfn, cfn);
}

static void virtualDispatchUpdateChildren(FnSymbol* pfn, FnSymbol* cfn) {
  Vec<FnSymbol*>* fns = virtualChildrenMap.get(pfn);

  if (fns == NULL) {
    fns = new Vec<FnSymbol*>();
  }

  fns->add(cfn);

  virtualChildrenMap.put(pfn, fns);
}

static void virtualDispatchUpdateRoots(FnSymbol* pfn, FnSymbol* cfn) {
  Vec<FnSymbol*>* fns = virtualRootsMap.get(cfn);

  if (fns == NULL) {
    fns = new Vec<FnSymbol*>();

    fns->add(pfn);

  } else {
    bool added = false;

    // check if parent or child already exists in vector
    for (int i = 0; i < fns->n && added == false; i++) {
      if (isVirtualChild(pfn, fns->v[i]) == true) {
        added = true;

      } else if (isVirtualChild(fns->v[i], pfn) == true) {
        fns->v[i] = pfn;
        added     = true;
      }
    }

    if (added == false) {
      fns->add(pfn);
    }
  }

  virtualRootsMap.put(cfn, fns);
}

// return true if child overrides parent in dispatch table
static bool isVirtualChild(FnSymbol* child, FnSymbol* parent) {
  bool retval = false;

  if (Vec<FnSymbol*>* children = virtualChildrenMap.get(parent)) {
    forv_Vec(FnSymbol*, candidateChild, *children) {
      if (candidateChild == child) {
        retval = true;
        break;
      }
    }
  }

  return retval;
}

static void clearRootsAndChildren() {
  Vec<Vec<FnSymbol*>*> rootValues;
  Vec<Vec<FnSymbol*>*> childValues;

  virtualRootsMap.get_values(rootValues);
  virtualChildrenMap.get_values(childValues);

  forv_Vec(Vec<FnSymbol*>, value, rootValues)  {
    delete value;
  }

  forv_Vec(Vec<FnSymbol*>, value, childValues) {
    delete value;
  }

  virtualRootsMap.clear();
  virtualChildrenMap.clear();
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void addVirtualMethodTableEntry(Type*     type,
                                       FnSymbol* fn,
                                       bool      exclusive);

static void buildVirtualMethodTable() {
  Vec<Type*> ctq;

  ctq.add(dtObject);

  for (int i = 0; i < virtualRootsMap.n; i++) {
    if (virtualRootsMap.v[i].key != NULL) {
      for (int j = 0; j < virtualRootsMap.v[i].value->n; j++) {
        FnSymbol* root = virtualRootsMap.v[i].value->v[j];

        addVirtualMethodTableEntry(root->_this->type, root, true);
      }
    }
  }

  forv_Vec(Type, t, ctq) {
    if (Vec<FnSymbol*>* parentFns = virtualMethodTable.get(t)) {
      forv_Vec(FnSymbol, pfn, *parentFns) {
        Vec<Type*> childSet;

        if (Vec<FnSymbol*>* childFns = virtualChildrenMap.get(pfn)) {
          forv_Vec(FnSymbol, cfn, *childFns) {
            if (AggregateType* at = toAggregateType(cfn->_this->type)) {
              forv_Vec(AggregateType, pt, at->dispatchParents) {
                if (pt == t) {
                  if (childSet.set_in(at) == NULL) {
                    addVirtualMethodTableEntry(at, cfn, false);

                    childSet.set_add(at);
                  }

                  break;
                }
              }
            }
          }
        }

        if (AggregateType* at = toAggregateType(t)) {
          forv_Vec(AggregateType, childType, at->dispatchChildren) {
            if (childSet.set_in(childType) == NULL) {
              addVirtualMethodTableEntry(childType, pfn, false);
            }
          }
        }
      }
    }

    if (AggregateType* at = toAggregateType(t)) {
      forv_Vec(AggregateType, child, at->dispatchChildren) {
        ctq.add(child);
      }
    }
  }

  // Reverse each value
  for (int i = 0; i < virtualMethodTable.n; i++) {
    if (virtualMethodTable.v[i].key != NULL) {
      virtualMethodTable.v[i].value->reverse();
    }
  }
}

// If exclusive == true, check for fn already existing in the virtual method
// table and do not add it a second time if it is already present.
static void addVirtualMethodTableEntry(Type*     type,
                                       FnSymbol* fn,
                                       bool      exclusive) {
  Vec<FnSymbol*>* fns   = virtualMethodTable.get(type);
  bool            found = false;

  if (fns == NULL) {
    fns = new Vec<FnSymbol*>();

  } else if (exclusive == true) {
    forv_Vec(FnSymbol, f, *fns) {
      if (f == fn) {
        found = true;
        break;
      }
    }
  }

  if (found == false) {
    fns->add(fn);

    virtualMethodTable.put(type, fns);
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void buildVirtualMethodMap() {
  for (int i = 0; i < virtualMethodTable.n; i++) {
    if (virtualMethodTable.v[i].key) {
      for (int j = 0; j < virtualMethodTable.v[i].value->n; j++) {
        virtualMethodMap.put(virtualMethodTable.v[i].value->v[j], j);
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void printDispatchInfo() {
  printf("Dynamic dispatch table:\n");

  for (int i = 0; i < virtualMethodTable.n; i++) {
    if (Type* t = virtualMethodTable.v[i].key) {
      printf("  %s\n", toString(t));

      for (int j = 0; j < virtualMethodTable.v[i].value->n; j++) {
        FnSymbol* fn = virtualMethodTable.v[i].value->v[j];

        printf("    %s", toString(fn));

        if (developer == true) {
          int index = virtualMethodMap.get(fn);

          printf(" index %i", index);

          if (fn->getFormal(2)->typeInfo() == t) {
            // print dispatch children if the function is the method in type t
            // (and not, say, the version in some parent class e.g. object).
            Vec<FnSymbol*>* childFns = virtualChildrenMap.get(fn);

            if (childFns != NULL) {
              printf(" %i children:\n", childFns->n);

              for (int k = 0; k < childFns->n; k++) {
                FnSymbol* childFn = childFns->v[k];

                printf("      %s\n", toString(childFn));
              }
            }

            if (childFns == NULL || childFns->n == 0) {
              printf("\n");
            }

          } else {
            printf(" inherited\n");
          }
        }
        printf("\n");
      }

      if (developer == true) {
        printf("\n");
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

// removes entries in virtualChildrenMap that are not in virtualMethodTable.
// such entries could not be called and should be dead-code eliminated.
static void filterVirtualChildren() {
  typedef MapElem<Type*,     Vec<FnSymbol*>*> VmtMapElem;
  typedef MapElem<FnSymbol*, Vec<FnSymbol*>*> ChildMapElem;

  std::set<FnSymbol*> fns_in_vmt;

  form_Map(VmtMapElem, el,  virtualMethodTable) {
    if (el->value) {
      forv_Vec(FnSymbol, fn, *el->value) {
        fns_in_vmt.insert(fn);
      }
    }
  }

  form_Map(ChildMapElem, el, virtualChildrenMap) {
    if (el->value) {
      Vec<FnSymbol*>* oldV = el->value;
      Vec<FnSymbol*>* newV = new Vec<FnSymbol*>();

      forv_Vec(FnSymbol, fn, *oldV) {
        if (fns_in_vmt.count(fn)) {
          newV->add(fn);
        }
      }

      el->value = newV;

      delete oldV;
    }
  }
}

/************************************* | **************************************
*                                                                             *
* At this point calls to class methods have been resolved to the most         *
* specific method based on the static type of the receiver.  This phase       *
* inspects every call to determine if 1 or more derived classes define an     *
* override for the method.  These calls are generally be converted to be uses *
* of PRIM_VIRTUAL_METHOD_CALL.  There are two exceptions:                     *
*                                                                             *
*    1) The receiver is "super".  This is intended to allow an overriding     *
*       method to invoke the most specific method that is being overridden    *
*       c.f. Java and Swift. It is likely that the current implementation     *
*       (7/6/2017) is more general than this.                                 *
*                                                                             *
*    2) The call is to an init method.  The intent is to support uses of      *
*       this.init(...) c.f. convenience initializers in Swift.  Note that     *
*       super.init() is covered by exception 1.                               *
*                                                                             *
************************************** | *************************************/

static bool wasSuperDot(CallExpr* call);

void insertDynamicDispatchCalls() {
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->parentSymbol != NULL) {
      if (FnSymbol* fn = call->resolvedFunction()) {

        if (virtualChildrenMap.get(fn) != NULL  &&   // There are overrides
            wasSuperDot(call)          == false &&   // Not super.<foo>()
            call->isNamed("init")      == false) {   // Not an initializer
          SET_LINENO(call);

          // The variable <cid> must have the same size as the type
          // of chpl__class_id / chpl_cid_* to ensure the value is
          // transmitted correctly for a remote class.
          // See test/classes/sungeun/remoteDynamicDispatch.chpl
          // (on certain machines and configurations).
          Type*      cidType = dtInt[INT_SIZE_32];
          VarSymbol* cid     = newTemp("_virtual_method_tmp_", cidType);

          Expr*      _this   = call->get(2);
          CallExpr*  getCid  = new CallExpr(PRIM_GETCID, _this->copy());

          Expr*      stmt    = call->getStmtExpr();

          stmt->insertBefore(new DefExpr(cid));
          stmt->insertBefore(new CallExpr(PRIM_MOVE, cid, getCid));

          // Note that call->get(1) is re-defined by each insertion
          call->get(1)->insertBefore(new SymExpr(cid));
          call->get(1)->insertBefore(call->baseExpr->remove());

          call->primitive = primitives[PRIM_VIRTUAL_METHOD_CALL];
        }
      }
    }
  }
}

// Return true if this call was originally super.<method>()
static bool wasSuperDot(CallExpr* call) {
  bool retval = false;

  if (SymExpr* base = toSymExpr(call->get(2))) {
    retval = base->symbol()->hasFlag(FLAG_SUPER_TEMP);
  }

  return retval;
}
