/*
 * Copyright 2004-2017 Cray Inc.
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
#include "expr.h"
#include "iterator.h"
#include "resolution.h"
#include "stmt.h"
#include "symbol.h"

#include <set>

bool                            inDynamicDispatchResolution = false;

Map<FnSymbol*, int>             virtualMethodMap;
Map<Type*,     Vec<FnSymbol*>*> virtualMethodTable;
Map<FnSymbol*, Vec<FnSymbol*>*> virtualChildrenMap;
Map<FnSymbol*, Vec<FnSymbol*>*> virtualRootsMap;

static bool isSubType(Type* sub, Type* super);

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void buildVirtualMaps();
static void addAllToVirtualMaps(FnSymbol* fn,  AggregateType* pct);
static void addToVirtualMaps   (FnSymbol* pfn, AggregateType* ct);
static void collectMethodsForVirtualMaps(Vec<FnSymbol*>& methods,
                                         Type*           ct,
                                         FnSymbol*       pfn);
static void collectInstantiatedAggregateTypes(Vec<Type*>& icts,
                                              Type*       ct);

static void addVirtualMethodTableEntry(Type*     type,
                                       FnSymbol* fn,
                                       bool      exclusive);

static void filterVirtualChildren();

static bool isVirtualChild(FnSymbol* child, FnSymbol* parent);

static bool signatureMatch(FnSymbol* fn, FnSymbol* gn);
static bool possibleSignatureMatch(FnSymbol* fn, FnSymbol* gn);

void resolveDynamicDispatches() {
  int numTypes = 0;

  inDynamicDispatchResolution = true;

  do {
    numTypes = gTypeSymbols.n;

    {
      Vec<Vec<FnSymbol*>*> values;

      virtualChildrenMap.get_values(values);

      forv_Vec(Vec<FnSymbol*>, value, values) {
        delete value;
      }
    }

    virtualChildrenMap.clear();

    {
      Vec<Vec<FnSymbol*>*> values;

      virtualRootsMap.get_values(values);

      forv_Vec(Vec<FnSymbol*>, value, values) {
        delete value;
      }
    }

    virtualRootsMap.clear();

    buildVirtualMaps();

  } while (numTypes != gTypeSymbols.n);

  for (int i = 0; i < virtualRootsMap.n; i++) {
    if (virtualRootsMap.v[i].key) {
      for (int j = 0; j < virtualRootsMap.v[i].value->n; j++) {
        FnSymbol* root = virtualRootsMap.v[i].value->v[j];

        addVirtualMethodTableEntry(root->_this->type, root, true);
      }
    }
  }

  Vec<Type*> ctq;

  ctq.add(dtObject);

  forv_Vec(Type, ct, ctq) {
    if (Vec<FnSymbol*>* parentFns = virtualMethodTable.get(ct)) {
      forv_Vec(FnSymbol, pfn, *parentFns) {
        // Each subtype can contribute only one function to the
        // virtual method table.
        Vec<Type*> childSet;

        if (Vec<FnSymbol*>* childFns = virtualChildrenMap.get(pfn)) {
          forv_Vec(FnSymbol, cfn, *childFns) {
            forv_Vec(Type, pt, cfn->_this->type->dispatchParents) {
              if (pt == ct) {
                if (!childSet.set_in(cfn->_this->type)) {
                  addVirtualMethodTableEntry(cfn->_this->type, cfn, false);

                  childSet.set_add(cfn->_this->type);
                }

                break;
              }
            }
          }
        }

        forv_Vec(Type, childType, ct->dispatchChildren) {
          if (!childSet.set_in(childType)) {
            addVirtualMethodTableEntry(childType, pfn, false);
          }
        }
      }
    }

    forv_Vec(Type, child, ct->dispatchChildren) {
      ctq.add(child);
    }
  }

  // reverse the entries in the virtual method table
  // populate the virtualMethodMap
  for (int i = 0; i < virtualMethodTable.n; i++) {
    if (virtualMethodTable.v[i].key) {
      virtualMethodTable.v[i].value->reverse();

      for (int j = 0; j < virtualMethodTable.v[i].value->n; j++) {
        virtualMethodMap.put(virtualMethodTable.v[i].value->v[j], j);
      }
    }
  }

  // remove entries in virtualChildrenMap that are not in
  // virtualMethodTable. When a parent has a generic method and
  // a subclass has a specific one, the virtualChildrenMap might
  // get multiple entries while the logic above with childSet
  // ensures that the virtualMethodTable only has one entry.
  filterVirtualChildren();

  inDynamicDispatchResolution = false;

  if (fPrintDispatch) {
    printf("Dynamic dispatch table:\n");
    for (int i = 0; i < virtualMethodTable.n; i++) {
      if (Type* t = virtualMethodTable.v[i].key) {
        printf("  %s\n", toString(t));
        for (int j = 0; j < virtualMethodTable.v[i].value->n; j++) {
          FnSymbol* fn = virtualMethodTable.v[i].value->v[j];
          printf("    %s", toString(fn));
          if (developer) {
            int index = virtualMethodMap.get(fn);
            printf(" index %i", index);
            if ( fn->getFormal(2)->typeInfo() == t ) {
              // print dispatch children if the function is
              // the method in type t (and not, say, the version in
              // some parent class e.g. object).
              Vec<FnSymbol*>* childFns = virtualChildrenMap.get(fn);
              if (childFns) {
                printf(" %i children:\n", childFns->n);
                for (int k = 0; k < childFns->n; k++) {
                  FnSymbol* childFn = childFns->v[k];
                  printf("      %s\n", toString(childFn));
                }
              }
              if( childFns == NULL || childFns->n == 0 ) printf("\n");
            } else {
              printf(" inherited\n");
            }
          }
          printf("\n");
        }
        if (developer)
          printf("\n");
      }
    }
  }
}

static void buildVirtualMaps() {
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_WRAPPER))
      // Only "true" functions are used to populate virtual maps.
      continue;

    if (! fn->isResolved())
      // Only functions that are actually used go into the virtual map.
      continue;

    if (fn->hasFlag(FLAG_NO_PARENS))
      // Parentheses-less functions are statically bound; that is, they are not
      // dispatched through the virtual table.
      continue;

    if (fn->retTag == RET_PARAM || fn->retTag == RET_TYPE)
      // Only run-time functions populate the virtual map.
      continue;

    if (fn->numFormals() > 1 && fn->getFormal(1)->type == dtMethodToken) {
      // Only methods go in the virtual function table.
      if (AggregateType* pt = toAggregateType(fn->getFormal(2)->type)) {

        if (isClass(pt) && !pt->symbol->hasFlag(FLAG_GENERIC)) {
          // MPF - note the check for generic seemed to originate
          // in SVN revision 7103
          addAllToVirtualMaps(fn, pt);
        }
      }
    }
  }
}

// Add overrides of fn to virtual maps down the inheritance hierarchy
static void addAllToVirtualMaps(FnSymbol* fn, AggregateType* pct) {
  forv_Vec(Type, t, pct->dispatchChildren) {
    AggregateType* ct = toAggregateType(t);

    if (ct->defaultTypeConstructor &&
        (ct->defaultTypeConstructor->hasFlag(FLAG_GENERIC) ||
         ct->defaultTypeConstructor->isResolved())) {
      addToVirtualMaps(fn, ct);
    }

    // add overrides of method fn by children of ct to virtual maps
    addAllToVirtualMaps(fn, ct);
  }
}

// addToVirtualMaps itself goes through each method in ct and if
// that method could override pfn, adds it to the virtual maps
static void addToVirtualMaps(FnSymbol* pfn, AggregateType* ct) {
  // Collect methods from ct and ct->instantiatedFrom.
  // Does not include generic instantiations - sometimes we
  // have to make the instantiation here, so we always run
  // through a code path that instantiates.
  Vec<FnSymbol*> methods;

  collectMethodsForVirtualMaps(methods, ct, pfn);

  forv_Vec(FnSymbol, cfn, methods) {
    if (cfn && !cfn->instantiatedFrom) {
      Vec<Type*> types;

      if (ct->symbol->hasFlag(FLAG_GENERIC)) {
        collectInstantiatedAggregateTypes(types, ct);
      } else {
        types.add(ct);
      }

      forv_Vec(Type, type, types) {
        SymbolMap subs;

        if (ct->symbol->hasFlag(FLAG_GENERIC) ||
            cfn->getFormal(2)->type->symbol->hasFlag(FLAG_GENERIC)) {
          // instantiateSignature handles subs from formal to a type
          subs.put(cfn->getFormal(2), type->symbol);
        }

        for (int i = 3; i <= cfn->numFormals(); i++) {
          ArgSymbol* arg = cfn->getFormal(i);

          if (arg->intent == INTENT_PARAM) {
            subs.put(arg, paramMap.get(pfn->getFormal(i)));
          } else if (arg->type->symbol->hasFlag(FLAG_GENERIC)) {
            subs.put(arg, pfn->getFormal(i)->type->symbol);
          }
        }

        FnSymbol* fn = cfn;

        if (subs.n) {
          fn = instantiate(fn, subs);

          if (fn) {
            if (type->defaultTypeConstructor->instantiationPoint)
              fn->instantiationPoint = type->defaultTypeConstructor->instantiationPoint;
            else
              fn->instantiationPoint = toBlockStmt(type->defaultTypeConstructor->defPoint->parentExpr);
            INT_ASSERT(fn->instantiationPoint);
          }
        }

        if (fn) {
          resolveFormals(fn);

          if (signatureMatch(pfn, fn) && evaluateWhereClause(fn)) {
            resolveFns(fn);

            if (fn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD) &&
                pfn->retType->symbol->hasFlag(FLAG_ITERATOR_RECORD)) {
              AggregateType* fnRetType  = toAggregateType(fn->retType);
              IteratorInfo*  fnInfo     = fnRetType->iteratorInfo;
              AggregateType* pfnRetType = toAggregateType(pfn->retType);
              IteratorInfo*  pfnInfo    = pfnRetType->iteratorInfo;

              if (!isSubType(fnInfo->getValue->retType,
                             pfnInfo->getValue->retType)) {
                USR_FATAL_CONT(pfn, "conflicting return type specified for '%s: %s'", toString(pfn),
                               pfnInfo->getValue->retType->symbol->name);

                USR_FATAL_CONT(fn, "  overridden by '%s: %s'", toString(fn),
                               fnInfo->getValue->retType->symbol->name);
                USR_STOP();

              } else {
                pfn->retType->dispatchChildren.add_exclusive(fn->retType);
                fn->retType->dispatchParents.add_exclusive(pfn->retType);
                Type* pic = pfnInfo->iclass;
                Type* ic = fnInfo->iclass;
                INT_ASSERT(ic->symbol->hasFlag(FLAG_ITERATOR_CLASS));

                // Iterator classes are created as normal top-level classes (inheriting
                // from dtObject).  Here, we want to re-parent ic with pic, so
                // we need to remove and replace the object base class.
                INT_ASSERT(ic->dispatchParents.n == 1);
                Type* parent = ic->dispatchParents.only();
                if (parent == dtObject)
                {
                  int item = parent->dispatchChildren.index(ic);
                  parent->dispatchChildren.remove(item);
                  ic->dispatchParents.remove(0);
                }
                pic->dispatchChildren.add_exclusive(ic);
                ic->dispatchParents.add_exclusive(pic);
                continue; // do not add to virtualChildrenMap; handle in _getIterator
              }
            } else if (!isSubType(fn->retType, pfn->retType)) {
              USR_FATAL_CONT(pfn, "conflicting return type specified for '%s: %s'", toString(pfn), pfn->retType->symbol->name);
              USR_FATAL_CONT(fn, "  overridden by '%s: %s'", toString(fn), fn->retType->symbol->name);
              USR_STOP();
            }
            {
              Vec<FnSymbol*>* fns = virtualChildrenMap.get(pfn);
              if (!fns) fns = new Vec<FnSymbol*>();
              fns->add(fn);
              virtualChildrenMap.put(pfn, fns);
              fn->addFlag(FLAG_VIRTUAL);
              pfn->addFlag(FLAG_VIRTUAL);
            }
            {
              Vec<FnSymbol*>* fns = virtualRootsMap.get(fn);
              if (!fns) fns = new Vec<FnSymbol*>();
              bool added = false;

              //
              // check if parent or child already exists in vector
              //
              for (int i = 0; i < fns->n; i++) {
                //
                // if parent already exists, do not add child to vector
                //
                if (isVirtualChild(pfn, fns->v[i])) {
                  added = true;
                  break;
                }

                //
                // if child already exists, replace with parent
                //
                if (isVirtualChild(fns->v[i], pfn)) {
                    fns->v[i] = pfn;
                    added = true;
                    break;
                }
              }

              if (!added)
                fns->add(pfn);

              virtualRootsMap.put(fn, fns);
            }
          }
        }
      }
    }
  }
}

//
// add methods that possibly match pfn to vector,
// but does not add instantiated generics, since addToVirtualMaps
// will instantiate again.
static void collectMethodsForVirtualMaps(Vec<FnSymbol*>& methods,
                                         Type*           ct,
                                         FnSymbol*       pfn) {
  Vec<FnSymbol*>      tmp;
  std::set<FnSymbol*> generics;

  // Gather the generic, concrete, instantiated methods
  for (Type* fromType = ct;
       fromType != NULL;
       fromType = fromType->instantiatedFrom) {

    forv_Vec(FnSymbol, cfn, fromType->methods) {
      if (cfn && possibleSignatureMatch(pfn, cfn))
        tmp.add(cfn);
    }
  }

  // Don't add instantiations of generics if we were already
  // going to add the generic version. addToVirtualMaps will
  // re-instantiate.

  // So, gather a set of generic versions.
  forv_Vec(FnSymbol, cfn, tmp)
    if (cfn->hasFlag(FLAG_GENERIC))
      generics.insert(cfn);

  // Then, add anything not instantiated from something in
  // the set.
  forv_Vec(FnSymbol, cfn, tmp) {
    if (cfn->instantiatedFrom && generics.count(cfn->instantiatedFrom)) {
      // skip it
    } else {
      // add it
      methods.add(cfn);
    }
  }
}

//
// add to vector icts all types instantiated from ct
//
static void collectInstantiatedAggregateTypes(Vec<Type*>& icts, Type* ct) {
  forv_Vec(TypeSymbol, ts, gTypeSymbols) {
    if (ts->type->defaultTypeConstructor)
      if (!ts->hasFlag(FLAG_GENERIC) &&
          ts->type->defaultTypeConstructor->instantiatedFrom ==
          ct->defaultTypeConstructor) {
        icts.add(ts->type);

        INT_ASSERT(isInstantiation(ts->type, ct));
        INT_ASSERT(ts->type->instantiatedFrom == ct);
      }
  }
}

// if exclusive=true, check for fn already existing in the virtual method
// table and do not add it a second time if it is already present.
static void addVirtualMethodTableEntry(Type*     type,
                                       FnSymbol* fn,
                                       bool      exclusive) {
  Vec<FnSymbol*>* fns = virtualMethodTable.get(type);

  if (fns == NULL) {
    fns = new Vec<FnSymbol*>();
  }

  if (exclusive) {
    forv_Vec(FnSymbol, f, *fns) {
      if (f == fn) {
        return;
      }
    }
  }

  fns->add(fn);

  virtualMethodTable.put(type, fns);
}


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

//
// return true if child overrides parent in dispatch table
//
static bool isVirtualChild(FnSymbol* child, FnSymbol* parent) {
  if (Vec<FnSymbol*>* children = virtualChildrenMap.get(parent)) {
    forv_Vec(FnSymbol*, candidateChild, *children) {
      if (candidateChild == child) {
        return true;
      }
    }
  }

  return false;
}

// Checks that types match.
// Note - does not currently check that instantiated params match.
static bool signatureMatch(FnSymbol* fn, FnSymbol* gn) {
  if (fn->name != gn->name) {
    return false;
  }

  if (fn->numFormals() != gn->numFormals()) {
    return false;
  }

  for (int i = 3; i <= fn->numFormals(); i++) {
    ArgSymbol* fa = fn->getFormal(i);
    ArgSymbol* ga = gn->getFormal(i);

    if (strcmp(fa->name, ga->name)) {
      return false;
    }

    if (fa->type != ga->type) {
      return false;
    }
  }

  return true;
}

static bool possibleSignatureMatch(FnSymbol* fn, FnSymbol* gn) {
  if (fn->name != gn->name) {
    return false;
  }

  if (fn->numFormals() != gn->numFormals()) {
    return false;
  }

  for (int i = 3; i <= fn->numFormals(); i++) {
    ArgSymbol* fa = fn->getFormal(i);
    ArgSymbol* ga = gn->getFormal(i);

    if (strcmp(fa->name, ga->name)) {
      return false;
    }
  }

  return true;
}




/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void insertDynamicDispatchCalls() {
  // Select resolved calls whose function appears in the virtualChildrenMap.
  // These are the dynamically-dispatched calls.
  forv_Vec(CallExpr, call, gCallExprs) {
    if (!call->parentSymbol) continue;
    if (!call->getStmtExpr()) continue;

    FnSymbol* key = call->resolvedFunction();

    if (!key) continue;

    Vec<FnSymbol*>* fns = virtualChildrenMap.get(key);

    if (!fns) continue;

    SET_LINENO(call);

    bool isSuperAccess = false;
    if (SymExpr* base = toSymExpr(call->get(2))) {
      isSuperAccess = base->symbol()->hasFlag(FLAG_SUPER_TEMP);
    }

    if ((fns->n + 1 > fConditionalDynamicDispatchLimit) && !isSuperAccess ) {
      //
      // change call of root method into virtual method call;
      // Insert function SymExpr and virtual method temp at head of argument
      // list.
      //
      // N.B.: The following variable must have the same size as the type of
      // chpl__class_id / chpl_cid_* -- otherwise communication will cause
      // problems when it tries to read the cid of a remote class.  See
      // test/classes/sungeun/remoteDynamicDispatch.chpl (on certain
      // machines and configurations).
      VarSymbol* cid = newTemp("_virtual_method_tmp_", dtInt[INT_SIZE_32]);
      call->getStmtExpr()->insertBefore(new DefExpr(cid));
      call->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, cid, new CallExpr(PRIM_GETCID, call->get(2)->copy())));
      call->get(1)->insertBefore(new SymExpr(cid));
      // "remove" here means VMT calls are not really "resolved".
      // That is, calls to isResolved() return NULL.
      call->get(1)->insertBefore(call->baseExpr->remove());
      call->primitive = primitives[PRIM_VIRTUAL_METHOD_CALL];
      // This clause leads to necessary reference temporaries not being inserted,
      // while the clause below works correctly. <hilde>
      // Increase --conditional-dynamic-dispatch-limit to see this.
    } else {
      forv_Vec(FnSymbol, fn, *fns) {
        Type* type = fn->getFormal(2)->type;
        CallExpr* subcall = call->copy();
        SymExpr* tmp = new SymExpr(gNil);

      // Build the IF block.
        BlockStmt* ifBlock = new BlockStmt();
        VarSymbol* cid = newTemp("_dynamic_dispatch_tmp_", dtBool);
        ifBlock->insertAtTail(new DefExpr(cid));

        Expr* getCid = NULL;
        if (isSuperAccess) {
          // We're in a call on the super type.  We already know the answer to
          // this if conditional
          getCid = new SymExpr(gFalse);
        } else {
          getCid = new CallExpr(PRIM_TESTCID, call->get(2)->copy(),
                                type->symbol);
        }

        ifBlock->insertAtTail(new CallExpr(PRIM_MOVE, cid, getCid));
        VarSymbol* _ret = NULL;
        if (key->retType != dtVoid) {
          _ret = newTemp("_return_tmp_", key->retType);
          ifBlock->insertAtTail(new DefExpr(_ret));
        }
      // Build the TRUE block.
        BlockStmt* trueBlock = new BlockStmt();
        if (fn->retType == key->retType) {
          if (_ret)
            trueBlock->insertAtTail(new CallExpr(PRIM_MOVE, _ret, subcall));
          else
            trueBlock->insertAtTail(subcall);
        } else if (isSubType(fn->retType, key->retType)) {
          // Insert a cast to the overridden method's return type
          VarSymbol* castTemp = newTemp("_cast_tmp_", fn->retType);
          trueBlock->insertAtTail(new DefExpr(castTemp));
          trueBlock->insertAtTail(new CallExpr(PRIM_MOVE, castTemp,
                                               subcall));
          INT_ASSERT(_ret);
          trueBlock->insertAtTail(new CallExpr(PRIM_MOVE, _ret,
                                    new CallExpr(PRIM_CAST,
                                                 key->retType->symbol,
                                                 castTemp)));
        } else
          INT_FATAL(key, "unexpected case");

      // Build the FALSE block.
        BlockStmt* falseBlock = NULL;
        if (_ret)
          falseBlock = new BlockStmt(new CallExpr(PRIM_MOVE, _ret, tmp));
        else
          falseBlock = new BlockStmt(tmp);

        ifBlock->insertAtTail(new CondStmt(
                                new SymExpr(cid),
                                trueBlock,
                                falseBlock));
        if (key->retType == dtUnknown)
          INT_FATAL(call, "bad parent virtual function return type");
        call->getStmtExpr()->insertBefore(ifBlock);
        if (_ret)
          call->replace(new SymExpr(_ret));
        else
          call->remove();
        tmp->replace(call);
        subcall->baseExpr->replace(new SymExpr(fn));
        if (SymExpr* se = toSymExpr(subcall->get(2))) {
          VarSymbol* tmp = newTemp("_cast_tmp_", type);
          se->getStmtExpr()->insertBefore(new DefExpr(tmp));
          se->getStmtExpr()->insertBefore(new CallExpr(PRIM_MOVE, tmp, new
                CallExpr(PRIM_CAST, type->symbol, se->symbol())));
          se->replace(new SymExpr(tmp));
        } else if (CallExpr* ce = toCallExpr(subcall->get(2)))
          if (ce->isPrimitive(PRIM_CAST))
            ce->get(1)->replace(new SymExpr(type->symbol));
          else
            INT_FATAL(subcall, "unexpected");
        else
          INT_FATAL(subcall, "unexpected");
      }
    }
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static bool isSubType(Type* sub, Type* super) {
  if (sub == super)
    return true;

  forv_Vec(Type, parent, sub->dispatchParents) {
    if (isSubType(parent, super))
      return true;
  }

  return false;
}



