/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
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

#include "FnSymbol.h"

#include "AstToText.h"
#include "astutil.h"
#include "bb.h"
#include "CollapseBlocks.h"
#include "docsDriver.h"
#include "driver.h"
#include "expandVarArgs.h"
#include "iterator.h"
#include "PartialCopyData.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "visibleFunctions.h"

FnSymbol*                 chpl_gen_main         = NULL;
FnSymbol*                 initStringLiterals    = NULL;

FnSymbol*                 gAddModuleFn          = NULL;
FnSymbol*                 gGenericTupleTypeCtor = NULL;
FnSymbol*                 gGenericTupleDestroy  = NULL;

std::map<FnSymbol*, int>  ftableMap;
std::vector<FnSymbol*>    ftableVec;

FnSymbol::FnSymbol(const char* initName)
  : Symbol(E_FnSymbol, initName), userInstantiationPointLoc(0, NULL) {
  retType            = dtUnknown;
  where              = NULL;
  lifetimeConstraints= NULL;
  retExprType        = NULL;
  body               = new BlockStmt();
  thisTag            = INTENT_BLANK;
  retTag             = RET_VALUE;
  iteratorInfo       = NULL;
  iteratorGroup      = NULL;
  cacheInfo          = NULL;
  interfaceInfo      = NULL;
  _this              = NULL;
  instantiatedFrom   = NULL;
  _instantiationPoint = NULL;
  _backupInstantiationPoint = NULL;
  basicBlocks        = NULL;
  calledBy           = NULL;
  userString         = NULL;
  valueFunction      = NULL;
  codegenUniqueNum   = 1;
  doc                = NULL;
  retSymbol          = NULL;
  llvmDISubprogram   = NULL;
  mIsNormalized      = false;
  _throwsError       = false;
  mIsGeneric         = false;
  mIsGenericIsValid  = false;

  substitutions.clear();

  gFnSymbols.add(this);

  formals.parent = this;
}

FnSymbol::~FnSymbol() {
  cleanupIteratorInfo(this);
  cleanupIteratorGroup(this);
  cleanupCacheInfo(this);
  BasicBlock::clear(this);
  delete calledBy;
}

void FnSymbol::verify() {
  Symbol::verify();

  if (astTag != E_FnSymbol) {
    INT_FATAL(this, "Bad FnSymbol::astTag");
  }

  if (_this && _this->defPoint->parentSymbol != this)
    INT_FATAL(this, "Each method must contain a 'this' declaration.");

  if (normalized) {
    CallExpr* last = toCallExpr(body->body.last());

    if (last == NULL || last->isPrimitive(PRIM_RETURN) == false) {
      INT_FATAL(this, "Last statement in normalized function is not a return");
    }
  }

  if (formals.parent != this) {
    INT_FATAL(this, "Bad AList::parent in FnSymbol");
  }

  if (where && where->parentSymbol != this) {
    INT_FATAL(this, "Bad FnSymbol::where::parentSymbol");
  }

  if (lifetimeConstraints && lifetimeConstraints->parentSymbol != this) {
    INT_FATAL(this, "Bad FnSymbol::lifetimeConstraints::parentSymbol");
  }

  if (InterfaceInfo* ifcInfo = interfaceInfo) {
    // constrainedTypes: AList of DefExpr of ConstrainedType
    INT_ASSERT(ifcInfo->constrainedTypes.parent == this);
    for_alist(ctExpr, ifcInfo->constrainedTypes) {
      Symbol* ctSym = toDefExpr(ctExpr)->sym;
      ConstrainedType* ctType = toConstrainedType((toTypeSymbol(ctSym)->type));
      INT_ASSERT(ctType->ctUse == CT_CGFUN_FORMAL);
    }

    // interfaceConstraints: AList of IfcConstraint
    INT_ASSERT(ifcInfo->interfaceConstraints.parent == this);
    for_alist(ic, ifcInfo->interfaceConstraints)
      INT_ASSERT(isIfcConstraint(ic));

    // ifcInfo->repsForIfcSymbols is created during resolution
    // and disappears together with its parent function at the end of
    // resolution, so we never see it here.

    // CG functions are generic and should be pruned at end of resolution
    INT_ASSERT(!resolved);
  }

  if (retExprType && retExprType->parentSymbol != this) {
    INT_FATAL(this, "Bad FnSymbol::retExprType::parentSymbol");
  }

  if (body && body->parentSymbol != this) {
    INT_FATAL(this, "Bad FnSymbol::body::parentSymbol");
  }

  for_alist(fExpr, formals) {
    DefExpr* argDef = toDefExpr(fExpr);

    INT_ASSERT(argDef);
    INT_ASSERT(isArgSymbol(argDef->sym));
  }

  if (this->instantiatedFrom && !this->instantiatedFrom->inTree())
    INT_FATAL(this, "instantiatedFrom not in tree");

  // check substitutions
  form_Map(SymbolMapElem, e, this->substitutions) {
    if (e->key && !e->key->inTree())
      INT_FATAL(this, "Substitution key not in tree");
    if (e->value && !e->value->inTree())
      INT_FATAL(this, "Substitution value not in tree");
  }

  // check substitutionsPostResolve
  {
    size_t n = this->substitutionsPostResolve.size();
    for (size_t i = 0; i < n; i++) {
      const NameAndSymbol& ns = this->substitutionsPostResolve[i];
      if (ns.value && !ns.value->inTree())
        INT_FATAL(this, "Substitution value not in tree");
    }
  }

  verifyNotOnList(where);
  verifyNotOnList(lifetimeConstraints);
  verifyNotOnList(retExprType);
  verifyNotOnList(body);

  verifyInTree(retType,            "FnSymbol::retType");

  verifyInTree(_this,              "FnSymbol::_this");
  verifyInTree(instantiatedFrom,   "FnSymbol::instantiatedFrom");
  verifyInTree(_instantiationPoint, "FnSymbol::instantiationPoint");
  verifyInTree(_backupInstantiationPoint, "FnSymbol::backupInstantiationPoint");
  verifyInTree(valueFunction,      "FnSymbol::valueFunction");
  verifyInTree(retSymbol,          "FnSymbol::retSymbol");

  verifyIteratorGroup(this);
}

FnSymbol* FnSymbol::copyInner(SymbolMap* map) {
  // Copy members that are common to innerCopy and partialCopy.
  FnSymbol* copy = this->copyInnerCore(map);

  // Copy members that weren't set by copyInnerCore.
  copy->where       = COPY_INT(this->where);
  copy->lifetimeConstraints = COPY_INT(this->lifetimeConstraints);
  copy->body        = COPY_INT(this->body);
  copy->retExprType = COPY_INT(this->retExprType);
  copy->_this       = this->_this;

  size_t n = this->substitutionsPostResolve.size();
  for (size_t i = 0; i < n; i++) {
    const NameAndSymbol& ns = this->substitutionsPostResolve[i];
    copy->substitutionsPostResolve.push_back(ns);
  }

  if (InterfaceInfo* ifcInfoOld = this->interfaceInfo) {
    InterfaceInfo* ifcInfoCopy = new InterfaceInfo(copy);

    for_alist(ct, ifcInfoOld->constrainedTypes)
      ifcInfoCopy->addConstrainedType(toDefExpr(COPY_INT(ct)));

    for_alist(icon, ifcInfoOld->interfaceConstraints)
      ifcInfoCopy->addInterfaceConstraint(toIfcConstraint(COPY_INT(icon)));
  }

  return copy;
}


/** Copy over members common to both copyInner and partialCopy.
 *
 * \param map Map from symbols in the old function to symbols in the new one
 */
FnSymbol* FnSymbol::copyInnerCore(SymbolMap* map) {
  FnSymbol* newFn = new FnSymbol(this->name);

  /* Copy the flags.
   *
   * TODO: See if it is necessary to copy flags both here and in the copy
   * method.
   */
  newFn->copyFlags(this);

  if (this->throwsError() == true) {
    newFn->throwsErrorInit();
  }

  for_formals(formal, this) {
    newFn->insertFormalAtTail(COPY_INT(formal->defPoint));
  }

  // Copy members that are needed by both copyInner and partialCopy.
  newFn->astloc             = this->astloc;
  newFn->retType            = this->retType;
  newFn->thisTag            = this->thisTag;
  newFn->cname              = this->cname;
  newFn->retTag             = this->retTag;
  newFn->mIsGeneric         = this->mIsGeneric;
  newFn->mIsGenericIsValid  = this->mIsGenericIsValid;
  newFn->instantiatedFrom          = this->instantiatedFrom;
  newFn->_instantiationPoint       = this->_instantiationPoint;
  newFn->_backupInstantiationPoint = this->_backupInstantiationPoint;

  return newFn;
}


/** Copy just enough of the AST to get through filter candidate and
 *  disambiguate-by-match.
 *
 * This function selectively copies portions of the function's AST
 * representation.  The goal here is to copy exactly as many nodes as are
 * necessary to determine if a function is the best candidate for resolving a
 * call site and no more.  Special handling is necessary for the _this, where,
 * and retExprType members.  In addition, the return symbol needs to be made
 * available despite the fact that we have skipped copying the body.
 *
 * \param map Map from symbols in the old function to symbols in the new one
 */
FnSymbol* FnSymbol::partialCopy(SymbolMap* map) {
  FnSymbol*        newFn = this->copyInnerCore(map);

  // Indicate that we need to instantiate its body later.
  PartialCopyData& pci   = addPartialCopyData(newFn);

  pci.partialCopySource  = this;

  if (this->hasFlag(FLAG_RESOLVED))
    // Ensure 'newFn' is pruned if finalizeCopy() is never invoked.
    newFn->removeFlag(FLAG_RESOLVED);

  if (this->_this == NULL) {
    // Case 1: No _this pointer.
    newFn->_this = NULL;

  } else if (Symbol* replacementThis = map->get(this->_this)) {
    // Case 2: _this symbol is defined as one of the formal arguments.
    newFn->_this = replacementThis;

  } else {
    /*
     * Case 3: _this symbol is defined in the function's body.
     * A new symbol is created.  This symbol will have to be used
     * to replace some of the symbols generated from copying the
     * function's body during finalizeCopy.
     */

    DefExpr* defPoint = this->_this->defPoint;

    newFn->_this           = this->_this->copy(map);
    newFn->_this->defPoint = new DefExpr(newFn->_this,
                                         COPY_INT(defPoint->init),
                                         COPY_INT(defPoint->exprType));
  }

  // Copy and insert the where clause if it is present.
  if (this->where != NULL) {
    newFn->where = COPY_INT(this->where);

    insert_help(newFn->where, NULL, newFn);
  }

  // Copy and insert the lifetimeConstraints clause if it is present.
  if (this->lifetimeConstraints != NULL) {
    newFn->lifetimeConstraints = COPY_INT(this->lifetimeConstraints);

    insert_help(newFn->lifetimeConstraints, NULL, newFn);
  }

  // Copy and insert the retExprType if it is present.
  if (this->retExprType != NULL) {
    newFn->retExprType = COPY_INT(this->retExprType);

    insert_help(newFn->retExprType, NULL, newFn);
  }

  /*
   * Because we are not copying the function's body we need to make the return
   * symbol available through other means.  To do this we first have to find
   * where the symbol is defined.  It may either be nothing, the _this symbol, a
   * formal symbol, or a symbol defined in the function's body.  In the last
   * case a new symbol and definition point have to be generated; the
   * finalizeCopy method will replace their corresponding nodes from the body
   * appropriately.
   */
  if (this->getReturnSymbol() == gVoid) {
    // Case 1: Function returns void.
    newFn->retSymbol = gVoid;

  } else if (this->getReturnSymbol() == this->_this) {
    // Case 2: Function returns _this.
    newFn->retSymbol = newFn->_this;

  } else if (Symbol* replacementRet = map->get(this->getReturnSymbol())) {
    // Case 3: Function returns a formal argument.
    newFn->retSymbol = replacementRet;

  } else {
    // Case 4: Function returns a symbol defined in the body.
    DefExpr* defPoint = this->getReturnSymbol()->defPoint;

    newFn->retSymbol = COPY_INT(this->getReturnSymbol());

    newFn->retSymbol->defPoint = new DefExpr(newFn->retSymbol,
                                             COPY_INT(defPoint->init),
                                             COPY_INT(defPoint->exprType));

    update_symbols(newFn->retSymbol, map);
  }

  // Add a map entry from this FnSymbol to the newly generated one.
  map->put(this, newFn);

  // Update symbols in the sub-AST as is appropriate.
  update_symbols(newFn, map);

  // Copy over the partialCopyMap, to be used later in finalizeCopy.
  pci.partialCopyMap.copy(*map);

  return newFn;
}

/** Finish copying the function's AST after a partial copy.
 *
 * This function finishes the work started by partialCopy.  This involves
 * copying the setter and body, and repairing some inconsistencies in the
 * copied body.
 *
 * \param map Map from symbols in the old function to symbols in the new one
 */
void FnSymbol::finalizeCopy() {
  if (PartialCopyData* pci = getPartialCopyData(this)) {
    FnSymbol* const partialCopySource = pci->partialCopySource;

    // Make sure that the source has been finalized.
    partialCopySource->finalizeCopy();

    if (partialCopySource->hasFlag(FLAG_RESOLVED))
      this->addFlag(FLAG_RESOLVED);

    SET_LINENO(this);

    // Retrieve our old/new symbol map from the partial copy process.
    SymbolMap* map = &(pci->partialCopyMap);

    /*
     * When we reach this point we will be in one of three scenarios:
     *  1) The function's body is empty and needs to be copied over from the
     *     copy source.
     *  2) The function's body has been replaced and we don't need to do
     *     anything else.
     *  3) The function has had varargs expanded and we need to copy over the
     *     added statements from the old block to a new copy of the body from
     *     the source.
     */
    if (this->hasFlag(FLAG_EXPANDED_VARARGS)) {
      // Alias the old body and make a new copy of the body from the source.
      BlockStmt* varArgNodes = this->body;
      this->body->replace( COPY_INT(partialCopySource->body) );

      /*
       * Iterate over the statements that have been added to the function body
       * and add them to the new body.
       */
      for_alist_backward(node, varArgNodes->body) {
        this->body->insertAtHead(node->remove());
      }

    } else if (this->body->body.length == 0) {
      this->body->replace( COPY_INT(partialCopySource->body) );
    }

    Symbol* replacementThis = map->get(partialCopySource->_this);

    /*
     * Two cases may arise here.  The first is when the _this symbol is defined
     * in the formal arguments.  In this case no additional work needs to be
     * done.  In the second case the function's _this symbol is defined in the
     * function's body.  In this case we need to repair the new/old symbol map
     * and replace the definition point in the body with our existing def point.
     */
    if (replacementThis != this->_this) {
      /*
       * In Case 2:
       * partialCopySource->_this := A
       * this->_this              := B
       *
       * map[A] := C
       */

      // Set map[A] := B
      map->put(partialCopySource->_this, this->_this);
      // Set map[C] := B
      map->put(replacementThis, this->_this);

      // Replace the definition of _this in the body: def(C) -> def(B)
      replacementThis->defPoint->replace(this->_this->defPoint);
    }

    /*
     * Cases where the return symbol is gVoid or this->_this don't require
     * any additional actions.
     */
    if (this->retSymbol != gVoid && this->retSymbol != this->_this) {
      Symbol* replacementRet = map->get(partialCopySource->getReturnSymbol());

      if (replacementRet != this->retSymbol) {
        /*
         * We now know that retSymbol is defined in function's body.  We must
         * now replace the duplicate symbol and its definition point with the
         * ones generated in partialCopy.  This is the exact same process as
         * was done above for the _this symbol.
         */
        replacementRet->defPoint->replace(this->retSymbol->defPoint);

        map->put(partialCopySource->getReturnSymbol(), this->retSymbol);
        map->put(replacementRet, this->retSymbol);
      }
    }

    /*
     * Null out the return symbol so that future changes to the return symbol
     * will be reflected in calls to getReturnSymbol().
     */
    this->retSymbol = NULL;

    // Repair broken up-pointers.
    insert_help(this, this->defPoint, this->defPoint->parentSymbol);

    /*
     * Update all old symbols left in the function's AST with their appropriate
     * replacements.
     */
    update_symbols(this, map);

    // Replace vararg formal if appropriate.
    if (pci->varargOldFormal) {
      substituteVarargTupleRefs(this, pci);
    }

    // Clean up book keeping information.
    clearPartialCopyData(this);
  }
}


void FnSymbol::replaceChild(BaseAST* oldAst, BaseAST* newAst) {
  if (oldAst == body) {
    body = toBlockStmt(newAst);

  } else if (oldAst == where) {
    where = toBlockStmt(newAst);

  } else if (oldAst == lifetimeConstraints) {
    lifetimeConstraints = toBlockStmt(newAst);

  } else if (oldAst == retExprType) {
    retExprType = toBlockStmt(newAst);

  } else {
    INT_FATAL(this, "Unexpected case in FnSymbol::replaceChild");
  }
}


void FnSymbol::insertAtHead(Expr* ast) {
  body->insertAtHead(ast);
}


void FnSymbol::insertAtTail(Expr* ast) {
  body->insertAtTail(ast);
}


void FnSymbol::insertAtHead(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertAtHead(new_Expr(format, args));
  va_end(args);
}


void FnSymbol::insertAtTail(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertAtTail(new_Expr(format, args));
  va_end(args);
}


Symbol* FnSymbol::getReturnSymbol() {
  Symbol* retval = this->retSymbol;

  if (retval == NULL) {
    CallExpr* ret = toCallExpr(body->body.last());

    if (ret != NULL && ret->isPrimitive(PRIM_RETURN) == true) {
      if (SymExpr* sym = toSymExpr(ret->get(1))) {
        retval = sym->symbol();

      } else {
        INT_FATAL(this, "function is not normal");
      }
    }
  }

  if (retval == NULL) {
    INT_FATAL(this, "function is not normal");
  }

  return retval;
}

// Removes all statements from body and adds all statements from block.
void FnSymbol::replaceBodyStmtsWithStmts(BlockStmt* block) {
  for_alist(stmt, this->body->body) {
    stmt->remove();
  }
  for_alist(stmt, block->body) {
    this->body->insertAtTail(stmt->remove());
  }
}

// Removes all statements from body and adds expr
void FnSymbol::replaceBodyStmtsWithStmt(Expr* addStmt) {
  for_alist(stmt, this->body->body) {
    stmt->remove();
  }
  this->body->insertAtTail(addStmt);
}

void FnSymbol::setInstantiationPoint(Expr* expr) {
  if (expr == NULL) {
    this->_instantiationPoint = NULL;
    this->_backupInstantiationPoint = NULL;
  } else {
    BlockStmt* block = toBlockStmt(expr);
    if (block == NULL || block->blockTag == BLOCK_SCOPELESS)
      block = getInstantiationPoint(expr);
    this->_instantiationPoint = block;
    this->_backupInstantiationPoint = block->getFunction();
  }

  //if (expr != NULL)
  //  userInstantiationPointLoc = getUserInstantiationPoint(this);
}

BlockStmt* FnSymbol::instantiationPoint() const {
  if (this->_instantiationPoint && this->_instantiationPoint->parentSymbol)
    return this->_instantiationPoint;
  else if (this->_backupInstantiationPoint)
    return this->_backupInstantiationPoint->body;
  else
    return NULL;
}

void FnSymbol::insertBeforeEpilogue(Expr* ast) {
  LabelSymbol* label = getEpilogueLabel();

  if (label) {
    DefExpr* def = label->defPoint;

    def->insertBefore(ast);
  } else {
    // if an epilogue is later added, this will be excluded
    CallExpr* ret = toCallExpr(body->body.last());

    ret->insertBefore(ast);
  }
}


void FnSymbol::insertIntoEpilogue(Expr* ast) {
  getOrCreateEpilogueLabel(); // always inserting into an epilogue

  CallExpr* ret = toCallExpr(body->body.last());

  ret->insertBefore(ast);
}


LabelSymbol* FnSymbol::getEpilogueLabel() {
  CallExpr*    ret    = toCallExpr(body->body.last());
  LabelSymbol* retval = NULL;

  if (ret != NULL && ret->isPrimitive(PRIM_RETURN) == true) {
    for (Expr* last = ret; last != NULL && retval == NULL; last = last->prev) {
      if (DefExpr* def = toDefExpr(last->prev)) {
        if (LabelSymbol* label = toLabelSymbol(def->sym)) {
          if (label->hasFlag(FLAG_EPILOGUE_LABEL) == true) {
            retval = label;
          }
        }
      }
    }

  } else {
    INT_FATAL(this, "function is not normal");
  }

  return retval;
}


LabelSymbol* FnSymbol::getOrCreateEpilogueLabel() {
  LabelSymbol* label = getEpilogueLabel();

  if (label == NULL) {
    label = new LabelSymbol(astr("_end", name));

    label->addFlag(FLAG_EPILOGUE_LABEL);

    CallExpr* ret = toCallExpr(body->body.last());

    ret->insertBefore(new DefExpr(label));
  }

  return label;
}

void FnSymbol::insertFormalAtHead(BaseAST* ast) {
  Expr* toInsert = NULL;

  if (ArgSymbol* arg = toArgSymbol(ast)) {
    toInsert = new DefExpr(arg);

  } else if (DefExpr* def = toDefExpr(ast)) {
    toInsert = def;

  } else {
    INT_FATAL(ast, "Bad argument to FnSymbol::insertFormalAtHead");
  }

  formals.insertAtHead(toInsert);

  parent_insert_help(this, toInsert);
}


void FnSymbol::insertFormalAtTail(BaseAST* ast) {
  Expr* toInsert = NULL;

  if (ArgSymbol* arg = toArgSymbol(ast)) {
    toInsert = new DefExpr(arg);

  } else if (DefExpr* def = toDefExpr(ast)) {
    toInsert = def;

  } else {
    INT_FATAL(ast, "Bad argument to FnSymbol::insertFormalAtTail");
  }

  formals.insertAtTail(toInsert);

  parent_insert_help(this, toInsert);
}


int FnSymbol::numFormals() const {
  return formals.length;
}


ArgSymbol* FnSymbol::getFormal(int i) const {
  return toArgSymbol(toDefExpr(formals.get(i))->sym);
}

void FnSymbol::collapseBlocks() {
  CollapseBlocks visitor;

  body->accept(&visitor);
}

//
// If 'this' has a single use as the callee of a CallExpr,
// return that CallExpr. Otherwise return NULL.
//
CallExpr* FnSymbol::singleInvocation() const {
  SymExpr*  se     = firstSymExpr();
  CallExpr* retval = NULL;

  if (se == NULL) {
    // no uses at all
    retval = NULL;

  } else if (se != lastSymExpr()) {
    // more than one use
    retval = NULL;

  // Got exactly one use. Check how it is used.
  } else if (CallExpr* parent = toCallExpr(se->parentExpr)) {
    if (se == parent->baseExpr) {
      retval = parent;
    }
  }

  // The use is not as the callee, ex. as a FCF.
  return retval;
}


//
// Support for constrained generics.
//

InterfaceInfo::InterfaceInfo(FnSymbol* parentFn) {
  parentFn->interfaceInfo = this;
  constrainedTypes.parent = parentFn;
  interfaceConstraints.parent = parentFn;
}

void InterfaceInfo::addConstrainedType(DefExpr* def) {
  constrainedTypes.insertAtTail(def);
}

void InterfaceInfo::addInterfaceConstraint(IfcConstraint* icon) {
  interfaceConstraints.insertAtTail(icon);
}


//
// Labels this function as generic or non-generic.
// Returns:
// * TGR_NEWLY_TAGGED - if this function has not been labeled before,
// * TGR_ALREADY_TAGGED - otherwise,
// * TGR_TAGGING_ABORTED - if this invocation is recursive and so is aborted.
//
TagGenericResult FnSymbol::tagIfGeneric(SymbolMap* map, bool abortOK) {
  if (isGenericIsValid()) {
    // generic-ness has already been established
    return TGR_ALREADY_TAGGED;

  } else if (isConstrainedGeneric()) {
    setGeneric(true);
    return TGR_NEWLY_TAGGED;

  } else {
    // avoid recursing for the function.
    static std::set<Symbol*> seen;
    if (seen.count(this)) {
      INT_ASSERT(abortOK);
      return TGR_TAGGING_ABORTED;
    }
    seen.insert(this);

    // compute and set this function's genericity
    bool generic = hasGenericFormals(map);
    setGeneric(generic);
    seen.erase(this);
    return TGR_NEWLY_TAGGED;
  }
}


//
// Scan the formals and return:
//   2 is there is at least 1 generic formal and every generic
//     formal has a default value
//
//   1 if there is at least 1 generic formal
//
//   0 if there are no generic formals
//
// 'map' is expected to be non-NULL if this function has been instantiated.
//
bool FnSymbol::hasGenericFormals(SymbolMap* map) const {
  bool anyGeneric = false;

  std::vector<unsigned char> formalIsGeneric;

  int count = 0;
  for_formals(formal, this) {
    bool isGeneric = false;

    if (formal->type == dtUnknown && formal->typeExpr != NULL) {

      // If the type expression does not depend on a previous generic formal,
      // we can resolve it now, even if there was a previous generic formal.

      bool dependsOnPreviousGeneric = false;
      int i = 0;
      if (anyGeneric) {
        for_formals(prevFormal, this) {
          if (i >= count)
            break;

          int cursz = formalIsGeneric.size();
          if (i < cursz && formalIsGeneric[i] == 1) {
            // check to see if prevFormal is used in the typeExpr
            // for the current formal.
            if (findSymExprFor(formal->typeExpr, prevFormal) != NULL) {
              dependsOnPreviousGeneric = true;
              break;
            }
          }
          i++;
        }
        if (dependsOnPreviousGeneric) {
          // Stop resolving formals.
          INT_ASSERT(anyGeneric == true);
          break;
        }
      }

      resolveBlockStmt(formal->typeExpr);
      formal->type = formal->typeExpr->body.tail->getValType();
    }

    if (formal->intent == INTENT_PARAM) {
      isGeneric = true;

    } else if (toConstrainedType(formal->type)) {
      // A CG function is known to be generic, so we should not be
      // querying hasGenericFormals(). The only other functions
      // with CT formals are those in 'interface' declarations.
      INT_ASSERT(! isConstrainedGeneric());
      INT_ASSERT(isInterfaceSymbol(defPoint->parentSymbol));

    } else if (formal->type->symbol->hasFlag(FLAG_GENERIC) == true) {
      bool formalInstantiated = false;
      if (map != NULL && formal->hasFlag(FLAG_TYPE_VARIABLE)) {
        form_Map(SymbolMapElem, e, *map) {
          if (e->key->name == formal->name) {
            formalInstantiated = true;
            break;
          }
        }
      }

      if (!formalInstantiated) {
        bool typeHasGenericDefaults = false;
        if (AggregateType* at = toAggregateType(formal->type))
          typeHasGenericDefaults = at->isGenericWithDefaults();

        if (typeHasGenericDefaults               == false ||
            formal->hasFlag(FLAG_MARKED_GENERIC) == true ||
            formal                               == _this) {
          if (!(formal == _this && (isInitializer() || isCopyInit()))) {
            isGeneric = true;
          }
        }
      }

    }

    // init= on generic types need to be considered generic so that 'this.type'
    // stuff will resolve.
    if (map == NULL && formal == _this && isCopyInit() && _this->type->symbol->hasFlag(FLAG_GENERIC)) {
      isGeneric = true;
    }

    if (isGeneric == true) {
      if (hasFlag(FLAG_EXPORT)) {
        USR_FATAL_CONT(this,
                         "exported function `%s` can't be generic", name);
        USR_PRINT(this,
                  "   formal argument '%s' causes it to be", formal->name);
      }
    }
    anyGeneric = anyGeneric || isGeneric;
    if (isGeneric) {
      while ((int)formalIsGeneric.size() < count) {
        formalIsGeneric.push_back(0);
      }
      formalIsGeneric.push_back(1);
    }
    count++;
  }

  return anyGeneric;
}

bool FnSymbol::isNormalized() const {
  return mIsNormalized;
}

void FnSymbol::setNormalized(bool value) {
  mIsNormalized = value;
}

bool FnSymbol::isResolved() const {
  return hasFlag(FLAG_RESOLVED);
}

void FnSymbol::accept(AstVisitor* visitor) {
  if (visitor->enterFnSym(this) == true) {

    for_alist(next_ast, formals) {
      next_ast->accept(visitor);
    }

    if (body)
      body->accept(visitor);

    if (where)
      where->accept(visitor);

    if (lifetimeConstraints)
      lifetimeConstraints->accept(visitor);

    if (InterfaceInfo* ifcInfo = interfaceInfo) {
      for_alist(ct, ifcInfo->constrainedTypes)
        ct->accept(visitor);

      for_alist(icon, ifcInfo->interfaceConstraints)
        icon->accept(visitor);
    }

    if (retExprType) {
      retExprType->accept(visitor);
    }

    visitor->exitFnSym(this);
  }
}

AggregateType* FnSymbol::getReceiverType() const {
  if (isMethod()) {
    if (isResolved() && _this != NULL) {
      return toAggregateType(_this->getValType());
    } else if (numFormals() >= 2) {
      ArgSymbol* _mt   = getFormal(1);
      ArgSymbol* _this = getFormal(2);

      if (_mt->type == dtMethodToken) {
        return toAggregateType(_this->getValType());
      }
    }
  }

  return NULL;
}

bool FnSymbol::isMethod() const {
  return hasFlag(FLAG_METHOD);
}

bool FnSymbol::isMethodOnClass() const {
  bool retval = false;

  if (AggregateType* at = getReceiverType()) {
    retval = at->isClass();
  }

  return retval;
}

bool FnSymbol::isMethodOnRecord() const {
  bool retval = false;

  if (AggregateType* at = getReceiverType()) {
    retval = at->isRecord();
  }

  return retval;
}

bool FnSymbol::isTypeMethod() const {
  if (isMethod() && _this != NULL) {
    return _this->hasFlag(FLAG_TYPE_VARIABLE);
  }
  return false;
}

void FnSymbol::setMethod(bool value) {
  if (value == true) {
    addFlag(FLAG_METHOD);
  } else {
    removeFlag(FLAG_METHOD);
  }
}

// This function is a method on an aggregate type, defined within its
// declaration
bool FnSymbol::isPrimaryMethod() const {
  return hasFlag(FLAG_METHOD_PRIMARY);
}

// This function is a method on an aggregate type, defined outside its
// definition
bool FnSymbol::isSecondaryMethod() const {
  return isMethod() == true && isPrimaryMethod() == false;
}

bool FnSymbol::isCompilerGenerated() const {
  return (hasFlag(FLAG_COMPILER_GENERATED) ||
          hasFlag(FLAG_AUTO_II));
}

bool FnSymbol::isInitializer() const {
  return isMethod() == true && name == astrInit;
}

bool FnSymbol::isPostInitializer() const {
  return isMethod() == true && name == astrPostinit;
}

bool FnSymbol::isDefaultInit() const {
  return hasFlag(FLAG_COMPILER_GENERATED) &&
         hasFlag(FLAG_COPY_INIT) == false &&
         isInitializer();
}

bool FnSymbol::isCopyInit() const {
  return isMethod() && name == astrInitEquals;
}

// This function or method is an iterator (as opposed to a procedure).
bool FnSymbol::isIterator() const {
  return hasFlag(FLAG_ITERATOR_FN);
}

// This function returns by ref or const ref
bool FnSymbol::returnsRefOrConstRef() const {
  return (retTag == RET_REF || retTag == RET_CONST_REF);
}

QualifiedType FnSymbol::getReturnQualType() const {
  Qualifier q = QUAL_UNKNOWN;
  bool isWideRef = retType->symbol->hasFlag(FLAG_WIDE_REF);
  if (retTag == RET_REF)
    q = isWideRef ? QUAL_WIDE_REF : QUAL_REF;
  else if(retTag == RET_CONST_REF)
    q = isWideRef ? QUAL_CONST_WIDE_REF : QUAL_CONST_REF;
  return QualifiedType(retType, q);
}


std::string FnSymbol::docsDirective() {
  if (fDocsTextOnly) {
    return "";
  }

  if (this->isMethod() && this->isIterator()) {
    return ".. itermethod:: ";
  } else if (this->isIterator()) {
    return ".. iterfunction:: ";
  } else if (this->isMethod()) {
    return ".. method:: ";
  } else {
    return ".. function:: ";
  }
}


void FnSymbol::printDocs(std::ostream* file, unsigned int tabs) {
  if (this->noDocGen() == false) {
    // Print the rst directive, if one is needed.
    this->printTabs(file, tabs);

    *file << this->docsDirective();

    // Print export. Externs do not get a prefix, since the user doesn't
    // care whether it's an extern or not (they just want to use the function).
    // Inlines don't get a prefix for symmetry in modules like Math.chpl and
    // due to the argument that it's of negligible value in most cases.
    if (this->hasFlag(FLAG_EXPORT)) {
      *file << "export ";
    }

    if (this->hasFlag(FLAG_OVERRIDE)) {
      *file << "override ";
    }

    // Print iter/proc.
    if (this->isIterator()) {
      *file << "iter ";

    } else {
      *file << "proc ";
    }

    // Print name and arguments.
    AstToText info;

    info.appendNameAndFormals(this);

    *file << info.text();

    // Print return intent, if one exists.
    switch (this->retTag) {
    case RET_REF:
      *file << " ref";
      break;

    case RET_CONST_REF:
      *file << " const ref";
      break;

    case RET_PARAM:
      *file << " param";
      break;

    case RET_TYPE:
      *file << " type";
      break;

    default:
      break;
    }

    // Print return type.
    if (this->retExprType != NULL) {
      AstToText info;

      info.appendExpr(this->retExprType->body.tail, true);
      *file << ": ";
      *file << info.text();
    }

    // Print throws
    if (this->throwsError()) {
      *file << " throws";
    }

    *file << std::endl;

    if (!fDocsTextOnly) {
      *file << std::endl;
    }

    if (this->doc != NULL) {
      this->printDocsDescription(this->doc, file, tabs + 1);
      *file << std::endl;
    }
  }
}

void FnSymbol::throwsErrorInit() {
  _throwsError = true;
}

bool FnSymbol::throwsError() const {
  return _throwsError;
}

bool FnSymbol::isGeneric() const {
  INT_ASSERT(mIsGenericIsValid);
  return mIsGeneric;
}

bool FnSymbol::isGenericIsValid() const {
  return mIsGenericIsValid;
}

void FnSymbol::setGeneric(bool generic) {
  mIsGeneric = generic;
  mIsGenericIsValid = true;
}

void FnSymbol::clearGeneric() {
  mIsGeneric = mIsGenericIsValid = false;
}

bool FnSymbol::isConstrainedGeneric() const {
  return interfaceInfo != NULL;
}

InterfaceInfo* FnSymbol::ensureInterfaceInfo() {
  if (interfaceInfo == NULL)
    interfaceInfo = new InterfaceInfo(this);

  return interfaceInfo;
}

void FnSymbol::addConstrainedType(DefExpr* def) {
  ensureInterfaceInfo()->addConstrainedType(def);
}

void FnSymbol::addInterfaceConstraint(IfcConstraint* icon) {
  ensureInterfaceInfo()->addInterfaceConstraint(icon);
}

bool FnSymbol::retExprDefinesNonVoid() const {
  bool retval = true;

  if (retExprType == NULL) {
    retval = false;

  } else if (retExprType->length() != 1) {
    retval = true;

  } else if (SymExpr* expr = toSymExpr(retExprType->body.get(1))) {
    retval = expr->symbol()->type != dtVoid;

  } else {
    retval = true;
  }

  return retval;
}

Symbol* FnSymbol::getSubstitutionWithName(const char* name) const {

  if (fVerify) {
    INT_ASSERT(name == astr(name));
  }

  if (this->substitutions.n > 0) {
    // should only exist during resolution
    form_Map(SymbolMapElem, e, this->substitutions) {
      if (e->key && e->key->name == name)
        return e->value;
    }
  }

  // after resolution (or possibly during)
  size_t n = this->substitutionsPostResolve.size();
  for (size_t i = 0; i < n; i++) {
    const NameAndSymbol& ns = this->substitutionsPostResolve[i];
    if (ns.name == name)
      return ns.value;
  }

  return NULL;
}

static bool stringNeedsParens(const std::string& str) {
  for(std::string::const_iterator it = str.begin(); it != str.end(); ++it) {
      const char ch = *it;
      if (ch == ' ' || ch == '(' || ch == ')' || ch == ':')
        return true;
  }
  return false;
}

// Produces a string representing an arg when describing an instantiation;
// e.g. param x = 3:int(8)
// if isThisArg is true, omit the name, and use parens if the
// type/param-val string contains ' ', ':', or '('.
// e.g. param (1)
// e.g. param (3:int(8))
// e.g. (Bar(14))
static std::string argToString(FnSymbol* fn,
                               const char* formalName,
                               ArgSymbol* arg,
                               Symbol* substitution,
                               bool isThisArg,
                               bool isParam,
                               bool isType,
                               const char* startGeneric,
                               const char* endGeneric,
                               bool& printedGeneric) {

  Symbol* sym = NULL;
  if (arg)
    sym = arg;
  if (substitution)
    sym = substitution;

  Type* t = dtUnknown;
  if (arg && arg->getValType() != dtUnknown)
    t = arg->getValType();
  if (substitution && substitution->getValType() != dtUnknown)
    t = substitution->getValType();

  if (substitution == arg)
    substitution = NULL;

  bool isGeneric = (substitution != NULL) ||
                   (arg && arg->hasFlag(FLAG_DELAY_GENERIC_EXPANSION));

  // Don't print out generic instantiations for GenericRecord.init
  // if we can avoid it.
  if (isThisArg && fn->name == astrInit) {
    if (AggregateType* at = toAggregateType(t)) {
      while (at->instantiatedFrom != NULL)
        at = at->instantiatedFrom;

      t = at;
      isGeneric = false;
    }
  }

  std::string ret = "";
  std::string name = formalName;
  std::string type = toString(t);

  if (arg && arg->hasFlag(FLAG_EXPANDED_VARARGS)) {
    std::string num = name;
    std::string n = arg->demungeVarArgName(&num);
    name = n + "(" + num + ")";
  }

  // 'this' argument for a class type is generally 'borrowed'
  // but this isn't how they are declared so leave it out in that case.
  const char* borrowedSpace = "borrowed ";
  if (isThisArg && startsWith(type.c_str(), borrowedSpace)) {
    type = type.substr(strlen(borrowedSpace));
  }

  if (isParam) {
    std::string value = "";

    Immediate* imm = getSymbolImmediate(sym);
    EnumSymbol* enumSym = NULL;
    if (imm == NULL && arg != NULL) {
      // Also look in the defaultExpr. See e.g. recursive-leader-errr.chpl
      // and the iterKind enum.
      // Not sure why this pattern doesn't set the immediate.
      if (SymExpr* se = toSymExpr(arg->defaultExpr->body.tail)) {
        Symbol* sym = se->symbol();
        imm = getSymbolImmediate(sym);
        if (imm == NULL) {
          enumSym = toEnumSymbol(sym);
          if (enumSym != NULL) {
            value = toString(t);
            value += ".";
            value += sym->name;
          }
        }
      }
    }
    if (imm) {
      if (imm->const_kind == NUM_KIND_BOOL) {
        value = imm->bool_value() ? "true" : "false";
      } else if (imm->const_kind == CONST_KIND_STRING) {
        value = "";
        if (t == dtBytes)
          value += "b";
        value += '"';
        value += imm->string_value();
        value += '"';
      } else {
        const size_t bufSize = 128;
        char buf[bufSize];
        snprint_imm(buf, bufSize, *imm);
        value = buf;
      }
    }

    ret += "param ";

    if (isThisArg) {
      ret += "(";
    } else {
      ret += name;
      ret += " = ";
    }

    ret += startGeneric;

    ret += value;

    // Add the type if it's not default
    if (isNumericParamDefaultType(t) == false &&
        t != dtUnknown && t != dtString && t != dtBytes &&
        enumSym == NULL) {
      ret += ": ";
      ret += type;
    }

    ret += endGeneric;

    if (isThisArg)
      ret += ")";

    printedGeneric = true;

  } else if (isType) {
    ret += "type ";

    if (isThisArg && stringNeedsParens(type))
      ret += "(";

    if (!isThisArg) {
      ret += name;
      ret += " = ";
    }
    ret += startGeneric;
    ret += type;
    ret += endGeneric;
    printedGeneric = true;

    if (isThisArg && stringNeedsParens(type))
      ret += ")";

  } else {
    if (isThisArg && stringNeedsParens(type))
      ret += "(";

    if (isGeneric) {
      if (!isThisArg) {
        ret += name;
        ret += ": ";
      }
      ret += startGeneric;
      ret += type;
      ret += endGeneric;
      printedGeneric = true;
    } else {
      if (!isThisArg) {
        ret += name;
        ret += ": ";
      }
      ret += type;
    }

    if (isThisArg && stringNeedsParens(type))
      ret += ")";
  }

  return ret;
}

std::string FnSymbol::nameAndArgsToString(const char* sep,
                                          bool forError,
                                          bool& printedUnderline) const {
  if (sep == NULL || sep[0] == '\0')
    sep = ", ";

  // To help with printing out generics substitutions as underlined
  const char* startGeneric = "";
  const char* endGeneric = "";
  if (forError) {
    startGeneric = underlineErrorFormat();
    endGeneric = clearErrorFormat();
  }

  FnSymbol* fn = const_cast<FnSymbol*>(this);

  std::vector<const char*> formalNames;
  std::vector<unsigned char> isParamVec;
  std::vector<unsigned char> isTypeVec;

  // If available, use the generic function formal list
  if (fn->instantiatedFrom) {
    for_formals(formal, fn->instantiatedFrom) {
      formalNames.push_back(formal->name);
      isParamVec.push_back(formal->isParameter());
      isTypeVec.push_back(formal->originalIntent == INTENT_TYPE);
    }

  // or a saved generic function formal list
  } else if (fn->substitutionsPostResolve.size() > 0) {
    for (size_t i = 0; i < fn->substitutionsPostResolve.size(); i++) {
      formalNames.push_back(fn->substitutionsPostResolve[i].name);
      isParamVec.push_back(fn->substitutionsPostResolve[i].isParam);
      isTypeVec.push_back(fn->substitutionsPostResolve[i].isType);
    }

  // otherwise use the formals we have
  } else {
    for_formals(formal, fn) {
      formalNames.push_back(formal->name);
      isParamVec.push_back(formal->isParameter());
      isTypeVec.push_back(formal->originalIntent == INTENT_TYPE);
    }
  }

  std::vector<ArgSymbol*> formals;
  std::vector<Symbol*> substitutions;

  for (size_t i = 0; i < formalNames.size(); i++) {
    const char* name = formalNames[i];

    ArgSymbol* formal = NULL;
    Symbol* substitution = NULL;

    // Look for matching argument in fn
    for_formals(arg, fn) {
      if (arg->name == name) {
        formal = arg;
        substitution = arg;
      }
    }

    // find a formal in a generic function if needed
    if (formal == NULL && fn->instantiatedFrom) {
      for_formals(arg, fn->instantiatedFrom) {
        formal = arg;
        substitution = arg;
      }
    }

    // use a substitution if we have one
    if (Symbol* sym = getSubstitutionWithName(name)) {
      substitution = sym;
    }

    // Skip method token
    // Ignore arguments added by the compiler
    if (formal && (formal->type == dtMethodToken ||
                   formal->hasFlag(FLAG_RETARG) ||
                   formal->hasFlag(FLAG_TYPE_FORMAL_FOR_OUT))) {
      formalNames[i] = NULL;
      formal = NULL;
      substitution = NULL;
    }

    formals.push_back(formal);
    substitutions.push_back(substitution);
  }

  std::string ret = "";
  bool printedGeneric = false;

  // Handle printing any 'this' argument before function name
  if (fn->hasFlag(FLAG_METHOD)) {
    // find this formal and substitution based on name
    ArgSymbol* thisFormal = NULL;
    Symbol* thisSubstitution = NULL;
    bool foundThis = false;
    bool thisIsParam = false;
    bool thisIsType = false;
    for (size_t i = 0; i < formalNames.size(); i++) {
      if (formalNames[i] == astrThis) {
        thisFormal = formals[i];
        thisSubstitution = substitutions[i];
        thisIsParam = isParamVec[i];
        thisIsType = isTypeVec[i];
        foundThis = true;
        // clear it out so it doesn't print in argument list
        formalNames[i] = NULL;
      }
    }

    if (foundThis) {
      std::string argString = argToString(fn,
                                          name, thisFormal, thisSubstitution,
                                          /*isThis*/ true,
                                          thisIsParam, thisIsType,
                                          startGeneric, endGeneric,
                                          printedGeneric);
      ret += argString;
      ret += ".";
    }
  }

  // Add the function name
  ret += this->name;

  if (!fn->hasFlag(FLAG_NO_PARENS))
    ret += "(";

  bool firstArg = true;
  for (size_t i = 0; i < formalNames.size(); i++) {
    const char* name = formalNames[i];
    ArgSymbol* formal = formals[i];
    Symbol* substitution = substitutions[i];
    bool isParam = isParamVec[i];
    bool isType = isTypeVec[i];

    if (name != NULL) {
      std::string argString = argToString(fn,
                                          name, formal, substitution,
                                          /*isThis*/ false,
                                          isParam, isType,
                                          startGeneric, endGeneric,
                                          printedGeneric);

      // add a separator if this isn't the first one
      if (!firstArg)
        ret += sep;
      // add the arg string
      ret += argString;

      firstArg = false;
    }
  }

  if (!fn->hasFlag(FLAG_NO_PARENS))
    ret += ")";

  printedUnderline = printedGeneric && (startGeneric[0] != '\0');

  return ret;
}


const char* toString(FnSymbol* fn) {
  const char* retval = NULL;

  if (fn->userString != NULL) {
    if (developer == true) {
      retval = astr(fn->userString, " [", istr(fn->id), "]");
    } else {
      retval = fn->userString;
    }

  } else {
    bool first      =  true;
    bool skipParens = false;

    if (developer == true) {
      // report the name as-is and include all args
      retval = fn->name;

    } else {
      if (fn->instantiatedFrom != NULL) {
        fn = fn->instantiatedFrom;
      }

      if (fn->isMethod() && fn->_this != NULL) {
        retval = astr(toString(fn->_this->type, false), ".", fn->name);

      } else if (fn->hasFlag(FLAG_MODULE_INIT) == true) {
        INT_ASSERT(strncmp("chpl__init_", fn->name, 11) == 0);

        retval = astr("top-level module statements for ", fn->name + 11);

      } else {
        retval = astr(fn->name);
      }
    }

    if        (fn->hasFlag(FLAG_NO_PARENS)        == true) {
      skipParens =  true;

    } else if (fn->hasFlag(FLAG_MODULE_INIT)      == true &&
               developer                          == false) {
      skipParens =  true;

    } else {
      skipParens = false;
      retval     = astr(retval, "(");
    }

    for (int i = 1; i <= fn->numFormals(); i++) {
      ArgSymbol* arg = fn->getFormal(i);

      // skip method token etc
      if (arg->type == dtMethodToken ||
          arg->type == dtTypeDefaultToken ||
          arg->type == dtModuleToken)
        continue;

      // skip _this formal for methods in non-developer mode
      // because in non-developer mode it has already been printed
      // along with the method name (e.g. C.mymethod).
      if (developer == false && fn->isMethod() && arg == fn->_this)
        continue;

      if (first == true) {
        first = false;

        if (skipParens == true) {
          retval = astr(retval, " ");
        }
      } else {
        retval = astr(retval, ", ");
      }

      if (arg->intent                           == INTENT_PARAM ||
          arg->hasFlag(FLAG_INSTANTIATED_PARAM) == true) {
        retval = astr(retval, "param ");
      }

      if (arg->hasFlag(FLAG_TYPE_VARIABLE) == true) {
        retval = astr(retval, "type ", arg->name);

      } else if (arg->type == dtUnknown) {
        if (arg->typeExpr != NULL) {
          if (SymExpr* sym = toSymExpr(arg->typeExpr->body.tail)) {
            retval = astr(retval, arg->name, ": ", sym->symbol()->name);
          } else if (CallExpr* call = toCallExpr(arg->typeExpr->body.tail)) {
            // TODO: need to preserve the original string for function signatures...
            if (call->isPrimitive(PRIM_TYPEOF)) {
              SymExpr* se = toSymExpr(call->get(1));
              retval = astr(retval, arg->name, ": ", se->symbol()->name, ".type");
            } else {
              retval = astr(retval, arg->name);
            }

          } else {
            retval = astr(retval, arg->name);
          }

        } else {
          retval = astr(retval, arg->name);
        }

      } else if (arg->type == dtAny) {
        retval = astr(retval, arg->name);

      } else {
        retval = astr(retval, arg->name, ": ", toString(arg->type));
      }

      if (arg->variableExpr != NULL) {
        retval = astr(retval, " ...");
      }
    }

    if (skipParens == false) {
      retval = astr(retval, ")");
    }

    if (developer  == true) {
      retval = astr(retval, " [", istr(fn->id), "]");
    }
  }

  return retval;
}
