/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "astutil.h"
#include "baseAST.h"
#include "callInfo.h"
#include "DecoratedClassType.h"
#include "driver.h"
#include "expandVarArgs.h"
#include "expr.h"
#include "iterator.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "stmt.h"
#include "symbol.h"

#include <set>
#include <vector>

bool                            inDynamicDispatchResolution = false;

typedef MapElem<FnSymbol*, Vec<FnSymbol*>*> VirtualMapElem;
Map<FnSymbol*, Vec<FnSymbol*>*> virtualRootsMap;
Map<FnSymbol*, Vec<FnSymbol*>*> virtualParentsMap;
Map<FnSymbol*, Vec<FnSymbol*>*> virtualChildrenMap;

Map<Type*,     Vec<FnSymbol*>*> virtualMethodTable;
Map<FnSymbol*, int>             virtualMethodMap;

static bool buildVirtualMaps();

static void clearRootsAndChildren();

static void buildVirtualMethodTable();

static void buildVirtualMethodMap();

static void filterVirtualChildren();

static void checkMethodsOverride();

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

  checkMethodsOverride();

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
static void virtualDispatchUpdateParents(FnSymbol* pfn, FnSymbol* cfn);
static void virtualDispatchUpdateRoots(FnSymbol* pfn, FnSymbol* cfn);

static bool isVirtualChild(FnSymbol* child, FnSymbol* parent);

static bool isSubType(Type* sub, Type* super);

static bool isOverrideableMethod(FnSymbol* fn);
static bool isVirtualizableMethod(FnSymbol* fn);

static AggregateType* getReceiverClassType(FnSymbol* fn);

// Returns true if the maps are "stable" i.e. set of types is unchanged
static bool buildVirtualMaps() {
  int numTypes = gTypeSymbols.n;

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (AggregateType* at = fn->getReceiver()) {
      if (at->isClass() == true) {
        if (at->isGeneric() == false) {
          if (fn->isResolved() && isVirtualizableMethod(fn)) {
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
    if (ct && ct->isGeneric() == false) {
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

// Returns cfn if it's not generic
// if it is generic, returns a function made by instantiating with
// the same types as pfn.
static FnSymbol* getInstantiatedFunction(FnSymbol* pfn,
                                         AggregateType* ct,
                                         FnSymbol* cfn) {
  ArgSymbol*     _this   = cfn->getFormal(2);
  AggregateType* _thisAt = getReceiverClassType(cfn);
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
    return cfn;

  } else {
    FnSymbol* fn = instantiate(cfn, subs);

    //
    // BHARSH 2018-04-06:
    //
    // Essential for arrays to be able to use initializers.
    //
    // A smaller test case:
    //   types/type_variables/deitz/test_point_of_instantiation3.chpl
    //
    fn->setInstantiationPoint(ct->symbol->instantiationPoint);

    return fn;
  }

}


static void addToVirtualMaps(FnSymbol*      pfn,
                             AggregateType* ct,
                             FnSymbol*      cfn) {

  FnSymbol* fn = getInstantiatedFunction(pfn, ct, cfn);
  resolveOverride(pfn, fn);
}

static void collectMethods(FnSymbol*               pfn,
                           AggregateType*          pct,
                           std::vector<FnSymbol*>& methods) {
  AggregateType* fromType = pct;

  while (fromType != NULL) {
    forv_Vec(FnSymbol, cfn, fromType->methods) {
      if (cfn && cfn->instantiatedFrom == NULL) {
        // if pfn is a filled in vararg function then cfn needs its
        // vararg stamped out here too.
        if (pfn->hasFlag(FLAG_EXPANDED_VARARGS)) {
          computeNonvirtualCallSites(pfn);
          forv_Vec(CallExpr, call, *pfn->calledBy) {
            CallInfo info;
            if (info.isWellFormed(call)) {
              SET_LINENO(cfn);
              if (FnSymbol* expCfn = expandIfVarArgs(cfn, info)) {
                cfn = expCfn;
              }
            } else {
              INT_FATAL("expected CallExpr to be well formed at this point");
            }
          }
        }
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

static bool checkOverrides(FnSymbol* fn) {
  ModuleSymbol* parentMod = fn->getModule();
  // check overrides for a given function if any of these are true...
          // (1) the flag is on and either
          //     (a) the function is not compiler-generated or
          //     (b) --devel is on
          //  or...
  return ((fOverrideChecking && (!fn->isCompilerGenerated() || developer)) ||
          // (2) the function is in the modules/ hierarchy
          //     (which we manage and want to keep clean)
          (parentMod && parentMod->modTag != MOD_USER)) &&
         !fn->hasFlag(FLAG_DEFAULT_ACTUAL_FUNCTION) ;
}

static bool ignoreOverrides(FnSymbol* fn) {
  return
    // ignore errors with deinit
    fn->name == astrDeinit ||
    // ignore errors with init
    fn->isInitializer() ||
    fn->isCopyInit() ||
    // ignore errors with postinit
    fn->isPostInitializer();
}

static void checkIntentsMatch(FnSymbol* pfn, FnSymbol* cfn) {
  AggregateType* ct = toAggregateType(cfn->_this->getValType());
  // these are really a preconditions for the following code
  INT_ASSERT(pfn->numFormals() == cfn->numFormals());
  INT_ASSERT(ct);

  int nFormals = pfn->numFormals();

  for (int i = 3; i <= nFormals; i++) {
    ArgSymbol* pa = pfn->getFormal(i);
    ArgSymbol* ca = cfn->getFormal(i);

    if (pa->originalIntent != ca->originalIntent) {
      USR_FATAL_CONT(cfn, "%s.%s conflicting intent for argument '%s' "
                          "in overriding method",
                           ct->symbol->name, cfn->name, ca->name);
      USR_FATAL_CONT(pa, "base method uses %s",
                          intentDescrString(pa->originalIntent));
      USR_FATAL_CONT(ca, "overriding method uses %s",
                          intentDescrString(ca->originalIntent));

      // "fix" the intent to report errors only once.
      // This would be totally unreasonable if the compiler were
      // to continue compiling beyond this pass.
      ca->originalIntent = pa->originalIntent;
    }
  }
}


static void resolveOverride(FnSymbol* pfn, FnSymbol* cfn) {
  resolveSignature(cfn);

  if (signatureMatch(pfn, cfn) &&
      evaluateWhereClause(cfn) &&
      evaluateWhereClause(pfn)) {

    resolveFunction(cfn);

    if (checkOverrides(cfn) &&
        !ignoreOverrides(cfn) &&
        !cfn->hasFlag(FLAG_OVERRIDE)) {
      const char* ptype = pfn->_this->type->symbol->name;
      const char* ctype = cfn->_this->type->symbol->name;
      USR_FATAL_CONT(cfn, "%s.%s overrides parent class method %s.%s but "
                          "missing override keyword",
                          ctype, cfn->name,
                          ptype, cfn->name);
      // Add the flag to avoid duplicate errors
      cfn->addFlag(FLAG_OVERRIDE);
    }
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

      // check that the intents match
      checkIntentsMatch(pfn, cfn);

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

  } else if (isAggregateType(sub) || isDecoratedClassType(sub)) {
    AggregateType* subAt = toAggregateType(sub);
    Type* useSuper = super;
    if (classesWithSameKind(sub, super)) {
      subAt = toAggregateType(canonicalDecoratedClassType(sub));
      useSuper = canonicalDecoratedClassType(super);
    }
    if (subAt) {
      forv_Vec(AggregateType, parent, subAt->dispatchParents) {
        if (isSubType(parent, useSuper) == true) {
          retval = true;
          break;
        }
      }
    }
  }

  return retval;
}

static bool isOverrideableMethod(FnSymbol* fn) {
  if (AggregateType* at = getReceiverClassType(fn)) {
    INT_ASSERT(at->isClass());

    return fn->name != astrInit &&
           !fn->hasFlag(FLAG_WRAPPER) &&
           !fn->hasFlag(FLAG_NO_PARENS);
  }

  return false;
}

static bool isVirtualizableMethod(FnSymbol *fn) {
  return isOverrideableMethod(fn) &&
         !fn->isTypeMethod() &&
         fn->retTag != RET_PARAM &&
         fn->retTag != RET_TYPE;
}

static void virtualDispatchUpdate(FnSymbol* pfn, FnSymbol* cfn) {
  cfn->addFlag(FLAG_VIRTUAL);
  pfn->addFlag(FLAG_VIRTUAL);

  // There is the potential for a data dependency between these
  virtualDispatchUpdateChildren(pfn, cfn);
  virtualDispatchUpdateParents(pfn, cfn);
  virtualDispatchUpdateRoots(pfn, cfn);
}

static void virtualDispatchUpdateChildren(FnSymbol* pfn, FnSymbol* cfn) {
  Vec<FnSymbol*>* fns = virtualChildrenMap.get(pfn);

  if (fns == NULL) {
    fns = new Vec<FnSymbol*>();
    virtualChildrenMap.put(pfn, fns);
  }

  fns->add(cfn);
}

static void virtualDispatchUpdateParents(FnSymbol* pfn, FnSymbol* cfn) {
  Vec<FnSymbol*>* fns = virtualParentsMap.get(cfn);

  if (fns == NULL) {
    fns = new Vec<FnSymbol*>();
    virtualParentsMap.put(cfn, fns);
  }

  fns->add(pfn);
}

static void virtualDispatchUpdateRoots(FnSymbol* pfn, FnSymbol* cfn) {
  Vec<FnSymbol*>* fns = virtualRootsMap.get(cfn);

  if (fns == NULL) {
    fns = new Vec<FnSymbol*>();
    virtualRootsMap.put(cfn, fns);

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

static void clearOneMap(Map<FnSymbol*, Vec<FnSymbol*>*>& map) {
  form_Map(VirtualMapElem, el, map)
    delete el->value;
  map.clear();
}

static void clearRootsAndChildren() {
  clearOneMap(virtualRootsMap);
  clearOneMap(virtualParentsMap);
  clearOneMap(virtualChildrenMap);
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
            if (childType && childSet.set_in(childType) == NULL) {
              addVirtualMethodTableEntry(childType, pfn, false);
            }
          }
        }
      }
    }

    if (AggregateType* at = toAggregateType(t)) {
      forv_Vec(AggregateType, child, at->dispatchChildren) {
        if (child)
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

  if (type->symbol->hasFlag(FLAG_GENERIC)) return;

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

// Remove from 'toTrim' the FnSymbols not in 'fns_in_vmt'.
static void trimVirtualMap(std::set<FnSymbol*>& fns_in_vmt,
                           Map<FnSymbol*, Vec<FnSymbol*>*>& toTrim)
{
  form_Map(VirtualMapElem, el, toTrim) {
    if (! fns_in_vmt.count(el->key)) {
      // We should not even be looking here.
      delete el->value;
      el->value = NULL;
      // Since Map does not remove entries well, keep 'el' there.
      continue;
    }

    Vec<FnSymbol*>* oldV = el->value;
    forv_Vec(FnSymbol, fn1, *oldV) {
      if (fns_in_vmt.count(fn1)) {
        // If all entries are in VMT, 'oldV' does not need adjustment.
      } else {
        // There is at least one entry in el->value that needs to be removed.
        // Build a new Vec. Redo the scan from the beginning.
        Vec<FnSymbol*>* newV = new Vec<FnSymbol*>();
        forv_Vec(FnSymbol, fn2, *oldV) {
          if (fns_in_vmt.count(fn2))
            newV->add(fn2);
        }
        delete oldV;
        el->value = newV;
        break;
      }
    }
  }
}

// removes entries in virtualChildrenMap that are not in virtualMethodTable.
// such entries could not be called and should be dead-code eliminated.
static void filterVirtualChildren() {
  typedef MapElem<Type*,     Vec<FnSymbol*>*> VmtMapElem;

  std::set<FnSymbol*> fns_in_vmt;

  form_Map(VmtMapElem, el,  virtualMethodTable) {
    if (el->value) {
      forv_Vec(FnSymbol, fn, *el->value) {
        fns_in_vmt.insert(fn);
      }
    }
  }

  trimVirtualMap(fns_in_vmt, virtualChildrenMap);
  trimVirtualMap(fns_in_vmt, virtualParentsMap);

  // Assume all "roots" are to stay, so just trim not-to-be-used Vecs.
  form_Map(VirtualMapElem, el, virtualRootsMap) {
    if (! fns_in_vmt.count(el->key)) {
      delete el->value;
      el->value = NULL;
    }
  }
}

// map from this type -> name -> fns
typedef std::map<const char*, std::vector<FnSymbol*> > NameToFns;
typedef std::map<AggregateType*, NameToFns > TypeToNameToFns;

static AggregateType* getReceiverClassType(FnSymbol* fn) {
  if (fn->isMethod() && fn->_this != NULL) {
    if (Type* cct = canonicalClassType(fn->_this->getValType())) {
      if (AggregateType* at = toAggregateType(cct)) {
        if (at->isClass()) {
          return at;
        }
      }
    }
  }

  return NULL;
}

static void findFunctionsProbablyMatching(TypeToNameToFns & map,
                                          FnSymbol* theFn,
                                          AggregateType* ct,
                                          std::vector<FnSymbol*> & matches) {
  // Find sub-map
  if (map.count(ct) &&
      map[ct].count(theFn->name)) {
    std::vector<FnSymbol*> & fns = map[ct][theFn->name];

    // Find functions probably matching here
    for_vector(FnSymbol, fn, fns) {
      if (possibleSignatureMatch(theFn, fn)) {
        matches.push_back(fn);
      }
    }
  }

  // Check parent types
  // TODO: Should we skip "object" for type method?
  forv_Vec(AggregateType, pt, ct->dispatchParents) {
    findFunctionsProbablyMatching(map, theFn, pt, matches);
  }
}

// methods on iterator class -> the iterator defining them
static FnSymbol* getOverrideCandidate(FnSymbol* fn) {
  FnSymbol* ret = fn;

  if (fn->_this && fn->hasFlag(FLAG_AUTO_II)) {
    // e.g. zip, advance, ...
    if (AggregateType* iteratorClass = toAggregateType(fn->_this->getValType()))
      if (iteratorClass->symbol->hasFlag(FLAG_ITERATOR_CLASS))
        ret = getTheIteratorFn(iteratorClass);
  }

  //
  // TODO: Merging these code paths triggers weird forwarding errors for
  // OwnedObject code. Might be because we discard "owned" in places
  // we should not?
  //
  if (fn->isTypeMethod()) {
    if (AggregateType* at = getReceiverClassType(ret)) {
      INT_ASSERT(at->isClass());
      return ret;
    }
  } else {
    if (ret->_this)
      if (AggregateType* at = toAggregateType(ret->_this->getValType()))
        if (at->isClass())
          return ret;
  }

  // otherwise, not a candidate.
  return NULL;
}

// This function helps avoid redundant errors for different
// instantiations of a generic
static FnSymbol* getOverrideCandidateGenericFn(FnSymbol* fn)
{
  while (fn->instantiatedFrom)
    fn = fn->instantiatedFrom;
  return fn;
}

// This function checks that the override keyword is used appropriately
// checkOverrides would also be a reasonable name for it.
static void checkMethodsOverride() {

  TypeToNameToFns map;

  std::set<FnSymbol*> erroredFunctions;

  // Populate the map with potential override candidates.
  forv_Vec(FnSymbol, aFn, gFnSymbols) {
    if (FnSymbol* fn = getOverrideCandidate(aFn)) {
      if (checkOverrides(fn)) {
        if (AggregateType* ct = getReceiverClassType(fn)) {
          if (isOverrideableMethod(fn)) {
            map[ct][fn->name].push_back(fn);
          }
        }
      }
    }
  }

  // now check each function
  forv_Vec(FnSymbol, aFn, gFnSymbols) {
    // output error for overriding for non-class methods
    if (aFn->hasFlag(FLAG_OVERRIDE)) {

      //
      // Type methods may have managed receiver types, which would normally
      // not be recognized as a class.
      //
      Type* thisType = canonicalClassType(aFn->_this->getValType());
      if (!isClass(thisType)) {
        const char* type = "non-class";
        if (isRecord(thisType))
          type = "record";
        FnSymbol* eFn = getOverrideCandidateGenericFn(aFn);
        if (erroredFunctions.count(eFn) == 0) {
          USR_FATAL_CONT(aFn, "%s.%s override keyword present but "
                         "%s methods cannot override",
                         thisType->symbol->name, aFn->name, type);
          erroredFunctions.insert(eFn);
          continue;
        }
      }
    }

    if (FnSymbol* fn = getOverrideCandidate(aFn)) {
      if (checkOverrides(fn) &&
          !ignoreOverrides(fn) &&
          // ignore duplicate errors
          erroredFunctions.count(fn) == 0) {

        AggregateType* ct = getReceiverClassType(fn);
        INT_ASSERT(ct && ct->isClass());

        // Do some initial basic checking
        if (fn->hasFlag(FLAG_OVERRIDE)) {
          const char* msg = NULL;

          if (fn->hasFlag(FLAG_NO_PARENS))
            msg = "parentheses-less methods cannot override";
          else if (!isOverrideableMethod(fn))
            msg = "signature is not overrideable";

          if (msg != NULL) {
            FnSymbol* eFn = getOverrideCandidateGenericFn(fn);
            if (erroredFunctions.count(eFn) == 0) {
              USR_FATAL_CONT(fn, "%s.%s override keyword present but %s",
                             ct->symbol->name, fn->name, msg);
              erroredFunctions.insert(eFn);
            }
            continue;
          }
        }

        {
          std::vector<FnSymbol*> matches;
          forv_Vec(AggregateType, pt, ct->dispatchParents) {
            findFunctionsProbablyMatching(map, fn, pt, matches);
          }

          bool ok = false;
          bool okKnown = false;

          if (fn->hasFlag(FLAG_OVERRIDE)) {
            // If it is marked override, check that there is a parent
            // method with the same signature.
            if (matches.size() == 0) {
              ok = false;
              okKnown = true;
            } else {
              // We don't really know yet
              ok = true;
              okKnown = false;
            }
          } else {
            // If it is not marked override, check that there is not
            // a parent method with the same signature.
            if (matches.size() == 0) {
              ok = true;
              okKnown = true;
            } else {
              // We don't really know yet because there might be
              // spurious matches
              ok = false;
              okKnown = false;
            }
          }

          if (okKnown == false) {
            bool foundMatch = false;
            bool foundUncertainty = false;

            for_vector(FnSymbol, pfn, matches) {
              if (fn->isResolved() && pfn->isResolved()) {
                // fn and pfn both concrete
                bool typeParamDiffers = false;

                // If they're both instantiated functions,
                // check they're instantiated with the same types/params.
                // If not, it's not a match.
                if (fn->instantiatedFrom != NULL &&
                    pfn->instantiatedFrom != NULL) {

                  int numFormals = fn->numFormals();
                  for (int i=3; i <= numFormals; i++) {
                    ArgSymbol* fnArg = fn->getFormal(i);
                    ArgSymbol* pfnArg = pfn->getFormal(i);
                    if (fnArg->getValType() != pfnArg->getValType() ||
                        (fnArg->originalIntent == INTENT_PARAM &&
                         pfnArg->originalIntent == INTENT_PARAM &&
                         paramMap.get(fnArg) != paramMap.get(pfnArg)))
                      typeParamDiffers = true;
                  }
                }

                if (typeParamDiffers == false &&
                    signatureMatch(fn, pfn) &&
                    evaluateWhereClause(pfn)) {
                  foundMatch = true;
                }
              } else if (fn->isResolved() && !pfn->isResolved()) {
                // pfn generic
                FnSymbol* pInst = getInstantiatedFunction(fn, ct, pfn);
                resolveSignature(pInst);
                if (signatureMatch(fn, pInst) && evaluateWhereClause(pInst)) {
                  foundMatch = true;
                }
              } else if (!fn->isResolved() && pfn->isResolved()) {
                // fn generic
                if (ct->isGeneric()) {
                  // If the receiver is generic, not much we can do
                  // (because we don't know which instantiation will be made)
                  // We might present other errors when checking instantiations
                  // made in the program.
                  foundUncertainty = true;
                } else {
                  FnSymbol* fnIns = getInstantiatedFunction(pfn, ct, fn);
                  resolveSignature(fnIns);
                  if (signatureMatch(pfn, fnIns) && evaluateWhereClause(pfn)) {
                    foundMatch = true;
                  }
                }
              } else {
                // What would we instantiate them to?
                // We might present other errors when checking instantiations
                // made in the program.
                foundUncertainty = true;
              }
            }

            if (foundUncertainty == false) {
              if (fn->hasFlag(FLAG_OVERRIDE)) {
                // If it is marked override, check that there is a parent
                // method with the same signature.
                ok = foundMatch;
              } else {
                // If it is not marked override, check that there is not
                // a parent method with the same signature.
                // Ignore this check if this method is not applicable
                // due to a where clause.
                ok = !foundMatch;
              }
              okKnown = true;
            }
          }

          if (okKnown && ok == false) {
            FnSymbol* eFn = getOverrideCandidateGenericFn(fn);
            if (erroredFunctions.count(eFn) == 0) {
              if (fn->hasFlag(FLAG_OVERRIDE)) {
                USR_FATAL_CONT(fn, "%s.%s override keyword present but no "
                                    "superclass method matches signature "
                                    "to override",
                                     ct->symbol->name, fn->name);
              } else {
                USR_FATAL_CONT(fn, "%s.%s override keyword required for method "
                                   "matching signature of superclass method",
                                   ct->symbol->name, fn->name);
              }

              erroredFunctions.insert(eFn);
            }
          }
        }
      }
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
    if (call->inTree()) {
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
