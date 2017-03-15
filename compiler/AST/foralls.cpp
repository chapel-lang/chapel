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

// foralls.h, foralls.cpp - support for forall loops

#include "foralls.h"
#include "astutil.h"
#include "stlUtil.h"
#include "passes.h" // for normalized, resolved
#include "AstVisitor.h"

const char* tfiTagDescrString(TFITag tfiTag) {
  switch (tfiTag) {
    case TFI_DEFAULT:   return "default";
    case TFI_CONST:     return "const";
    case TFI_IN:        return "in";
    case TFI_CONST_IN:  return "const in";
    case TFI_REF:       return "ref";
    case TFI_CONST_REF: return "const ref";
    case TFI_REDUCE:    return "reduce";
  }
  INT_ASSERT(false);
  return "";
}

//
// ForallIntents
//

// constructor
ForallIntents::ForallIntents() :
  iterRec(NULL),
  leadIdx(NULL),
  leadIdxCopy(NULL)
{}

ForallIntents* ForallIntents::copy(SymbolMap* map, bool internal) {
  // If this fails, see localMap and update_symbols() in DECLARE_COPY().
  // NB we can't DECLARE_COPY(ForallIntents) because it has _this->astloc.
  INT_ASSERT(map && internal);

  // Start would-be copyInner().
  ForallIntents* _this = new ForallIntents();

  // alas std::vector does not have a constructor that "reserves" slots
  int nv = numVars();
  _this->fiVars.reserve(nv);
  _this->fIntents.reserve(nv);
  _this->riSpecs.reserve(nv);

  for (int i = 0; i < nv; i++) {
    _this->fiVars  .push_back(COPY_INT(fiVars[i]));
    _this->fIntents.push_back(         fIntents[i]);
    _this->riSpecs .push_back(COPY_INT(riSpecs[i]));
  }

  _this->iterRec     = COPY_INT(iterRec);
  _this->leadIdx     = COPY_INT(leadIdx);
  _this->leadIdxCopy = COPY_INT(leadIdxCopy);

  // Finish would-be copyInner().
  // No update_symbols() because !internal is false.

  return _this;
}

//
// Returns true if the child has been found (and replaced).
//
bool ForallIntents::replaceChildFI(Expr* oldAst, Expr* newAst) {
  INT_ASSERT(oldAst != NULL);
#define UPDATE(dest, newNode) \
    if ((dest) == oldAst) { (dest) = newNode; return true; }

  for (std::vector<Expr*>::iterator itv = fiVars.begin();
       itv != fiVars.end(); itv++)
    UPDATE(*itv, newAst);
    
  for (std::vector<Expr*>::iterator itr = riSpecs.begin();
       itr != riSpecs.end(); itr++)
    UPDATE(*itr, newAst);

  // Does this ever get invoked with newAst==NULL?
  // If so, the 'if' should read "if (newSE || !newAst)".
  INT_ASSERT(newAst);
  if (SymExpr* newSE = toSymExpr(newAst)) {
    UPDATE(iterRec,     newSE);
    UPDATE(leadIdx,     newSE);
    UPDATE(leadIdxCopy, newSE);
  }
  
  // nothing matched
#undef UPDATE
  return false;
}

// This is intended to mimick Expr::remove(), without 'this' being an Expr.
void ForallIntents::removeFI(Expr* parentB) {
  // If this fails need to use trace_remove() instead of remove_help()
  // - see Expr::remove().
  INT_ASSERT(parentB->parentSymbol);

  // "Remove" all ASTs that 'this' contains.
#define REMOVE(dest) if (dest) remove_help(dest, 'r')

  for_vector(Expr, var, fiVars) REMOVE(var);
  for_riSpecs_vector(ri, this)  REMOVE(ri);
  REMOVE(iterRec);
  REMOVE(leadIdx);
  REMOVE(leadIdxCopy);

#undef REMOVE    
}

void ForallIntents::verifyFI(BlockStmt* parentB) {
  Expr* parentE = (Expr*)parentB;
  int nv = numVars();
  INT_ASSERT((int)(fiVars.size())   == nv);
  INT_ASSERT((int)(fIntents.size()) == nv);
  INT_ASSERT((int)(riSpecs.size())  == nv);

  for (int i = 0; i < nv; i++) {
    Expr* fiVar = fiVars[i];
    if (SymExpr* fiVarSE = toSymExpr(fiVar)) {
      INT_ASSERT(isVarSymbol(fiVarSE->symbol()) ||
                 isArgSymbol(fiVarSE->symbol()));  // no modules, fns, etc.
    } else {
      // fiVars[i] is either resolved or unresolved sym expr; never NULL.
      INT_ASSERT(isUnresolvedSymExpr(fiVar));
      // These should be resolved during scopeResolve.
      INT_ASSERT(!normalized);
    }
    verifyNotOnList(fiVar);
    INT_ASSERT(fiVar->parentExpr == parentE);

    Expr* ri = riSpecs[i];
    INT_ASSERT(isReduce(i) == !!ri);
    if (ri) {
      // ri can be UnresolvedSymExpr, SymExpr, CallExpr, ... (?)
      verifyNotOnList(ri);
      INT_ASSERT(ri->parentExpr == parentE);
    }
  }

  INT_ASSERT(!iterRec || iterRec->parentExpr == parentE);
  INT_ASSERT(!leadIdx || leadIdx->parentExpr == parentE);
  INT_ASSERT(!leadIdxCopy || leadIdxCopy->parentExpr == parentE);
  verifyNotOnList(iterRec);
  verifyNotOnList(leadIdx);
  verifyNotOnList(leadIdxCopy);

  // ForallIntents are gone during resolve().
  INT_ASSERT(!resolved);
}

void ForallIntents::acceptFI(AstVisitor* visitor) {
  visitor->visitForallIntents(this);
}


//
// parser support
// These functions report a user error for an unexpected intent.
//

static TFITag it2tfi(Expr* ref, IntentTag intent) {
  switch (intent) {
  case INTENT_IN:        return TFI_IN;
  case INTENT_CONST:     return TFI_CONST;
  case INTENT_CONST_IN:  return TFI_CONST_IN;
  case INTENT_REF:       return TFI_REF;
  case INTENT_CONST_REF: return TFI_CONST_REF;
  case INTENT_BLANK:     return TFI_DEFAULT;

  case INTENT_OUT:
  case INTENT_INOUT:
  case INTENT_PARAM:
  case INTENT_TYPE:
  case INTENT_REF_MAYBE_CONST:
    USR_FATAL_CONT(ref, "%s is not supported in a 'with' clause",
                        intentDescrString(intent));
    return TFI_DEFAULT;
  }
  INT_ASSERT(false); // unexpected IntentTag; 'intent' contains garbage?
  return TFI_DEFAULT; // dummy
}

void addForallIntent(ForallIntents* fi, Expr* var, IntentTag intent, Expr* ri) {
  TFITag tfi = ri ? TFI_REDUCE : it2tfi(var, intent);
  fi->fiVars.push_back(var);
  fi->fIntents.push_back(tfi);
  fi->riSpecs.push_back(ri);
}

//
// Returns true if 'ast' is directly under 'fi'.
// BTW in this case fi's enclosing BlockStmt is ast's parentExpr.
// BTW we could use AST_CALL_STDVEC/AST_CALL_CHILD for a "directly or
//  indirectly under fi" check, although we'd need extra effort
//  to make them return a bool and abort early if the answer is known.
//
bool astUnderFI(const Expr* ast, ForallIntents* fi) {
  if (!fi) return false;

#define CHECK(arg) { if (((Expr*)(arg)) == ast) return true; }

  for_vector(Expr, var, fi->fiVars) CHECK(var);
  for_riSpecs_vector(ri, fi)        CHECK(ri);
  CHECK(fi->iterRec);
  CHECK(fi->leadIdx);
  CHECK(fi->leadIdxCopy);

#undef CHECK
  // none found
  return false;
}  
