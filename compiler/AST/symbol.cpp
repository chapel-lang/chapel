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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "symbol.h"

#include "astutil.h"
#include "bb.h"
#include "build.h"
#include "docsDriver.h"
#include "driver.h"
#include "expandVarArgs.h"
#include "expr.h"
#include "files.h"
#include "ForallStmt.h"
#include "intlimits.h"
#include "iterator.h"
#include "misc.h"
#include "optimizations.h"
#include "PartialCopyData.h"
#include "passes.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stlUtil.h"
#include "stmt.h"
#include "stringutil.h"
#include "type.h"

#include "AstToText.h"
#include "AstVisitor.h"
#include "CollapseBlocks.h"

#include <algorithm>
#include <cstdlib>
#include <inttypes.h>
#include <iostream>
#include <sstream>
#include <stdint.h>

//
// The function that represents the compiler-generated entry point
//
FnSymbol *chpl_gen_main = NULL;

Symbol *gNil = NULL;
Symbol *gUnknown = NULL;
Symbol *gMethodToken = NULL;
Symbol *gTypeDefaultToken = NULL;
Symbol *gLeaderTag = NULL, *gFollowerTag = NULL, *gStandaloneTag = NULL;
Symbol *gModuleToken = NULL;
Symbol *gNoInit = NULL;
Symbol *gVoid = NULL;
Symbol *gFile = NULL;
Symbol *gStringC = NULL;
Symbol *gStringCopy = NULL;
Symbol *gCVoidPtr = NULL;
Symbol *gOpaque = NULL;
Symbol *gTimer = NULL;
Symbol *gTaskID = NULL;
Symbol *gSyncVarAuxFields = NULL;
Symbol *gSingleVarAuxFields = NULL;

VarSymbol *gTrue = NULL;
VarSymbol *gFalse = NULL;
VarSymbol *gTryToken = NULL;
VarSymbol *gBoundsChecking = NULL;
VarSymbol *gCastChecking = NULL;
VarSymbol *gDivZeroChecking = NULL;
VarSymbol* gPrivatization = NULL;
VarSymbol* gLocal = NULL;
VarSymbol* gNodeID = NULL;
VarSymbol *gModuleInitIndentLevel = NULL;
FnSymbol* gAddModuleFn = NULL;
FnSymbol *gGenericTupleTypeCtor = NULL;
FnSymbol *gGenericTupleInit = NULL;
FnSymbol *gGenericTupleDestroy = NULL;

std::map<FnSymbol*,int> ftableMap;
std::vector<FnSymbol*> ftableVec;

void verifyInTree(BaseAST* ast, const char* msg) {
  if (ast != NULL && ast->inTree() == false) {
    INT_FATAL(ast, "%s is not in AST", msg);
  }
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

Symbol::Symbol(AstTag astTag, const char* init_name, Type* init_type) :
  BaseAST(astTag),
  qual(QUAL_UNKNOWN),
  type(init_type),
  flags(),
  fieldQualifiers(NULL),
  defPoint(NULL),
  symExprsHead(NULL),
  symExprsTail(NULL)
{
  if (init_name) {
    name = astr(init_name);
  } else {
    name = astr("");
  }
  cname = name;
}


Symbol::~Symbol() {
  if (fieldQualifiers)
    delete [] fieldQualifiers;
}

void Symbol::verify() {
  if (defPoint) {
    if (!defPoint->parentSymbol && this != rootModule)
      INT_FATAL(this, "Symbol::defPoint is not in AST");
    if (this != defPoint->sym)
      INT_FATAL(this, "Symbol::defPoint != Sym::defPoint->sym");
  } else {
    if (this != rootModule)
      INT_FATAL(this, "Symbol without a defPoint");
  }
  verifyInTree(type, "Symbol::type");

  if (symExprsHead) {
    if (symExprsHead->symbolSymExprsPrev != NULL)
      INT_FATAL(this, "Symbol's SymExpr list is malformed (head)");
    if (symExprsHead->symbol() != this)
      INT_FATAL(this, "Symbol's SymExpr head has other symbol");
    if (symExprsHead->inTree() == false)
      INT_FATAL(this, "Symbol's SymExpr head not in tree");
  }

  if (symExprsTail) {
    if (symExprsTail->symbolSymExprsNext != NULL)
      INT_FATAL(this, "Symbol's SymExpr list is malformed (tail)");
    if (symExprsTail->symbol() != this)
      INT_FATAL(this, "Symbol's SymExpr tail has other symbol");
    if (symExprsTail->inTree() == false)
      INT_FATAL(this, "Symbol's SymExpr tail not in tree");
  }
}


bool Symbol::inTree() {
  if (this == rootModule)
    return true;
  if (defPoint)
    return defPoint->inTree();
  else
    return false;
}


static Qualifier qualifierForArgIntent(IntentTag intent)
{
  switch (intent) {
    case INTENT_IN:        return QUAL_VAL;
    case INTENT_OUT:       return QUAL_REF;
    case INTENT_INOUT:     return QUAL_REF;
    case INTENT_CONST:     return QUAL_CONST;
    case INTENT_CONST_IN:  return QUAL_CONST_VAL;
    case INTENT_REF:       return QUAL_REF;
    case INTENT_CONST_REF: return QUAL_CONST_REF;
    case INTENT_PARAM:     return QUAL_PARAM; // TODO
    case INTENT_TYPE:      return QUAL_UNKNOWN; // TODO
    case INTENT_BLANK:     return QUAL_UNKNOWN;
    case INTENT_REF_MAYBE_CONST:
           return QUAL_REF; // a white lie until cullOverReferences

    // no default to get compiler warning if other intents are added
  }
  return QUAL_UNKNOWN;
}

QualifiedType Symbol::qualType() {
  QualifiedType ret(dtUnknown, QUAL_UNKNOWN);

  if (ArgSymbol* arg = toArgSymbol(this)) {
    Qualifier q = qualifierForArgIntent(arg->intent);
    if (qual == QUAL_WIDE_REF && (q == QUAL_REF || q == QUAL_CONST_REF)) {
      q = QUAL_WIDE_REF;
      // MPF: Should this be CONST_WIDE_REF in some cases?
    }
    ret = QualifiedType(type, q);
  } else {
    ret = QualifiedType(type, qual);
    if (hasFlag(FLAG_CONST))
      ret = ret.toConst();
  }

  return ret;
}


bool Symbol::isConstant() const {
  return false;
}

bool Symbol::isConstValWillNotChange() {
  return false;
}

bool Symbol::isParameter() const {
  return false;
}

bool Symbol::isRenameable() const {
  return !(hasFlag(FLAG_EXPORT) || hasFlag(FLAG_EXTERN));
}

bool Symbol::isRef() {
  QualifiedType q = qualType();
  return (type != NULL) && (q.isRef() || type->symbol->hasFlag(FLAG_REF));
}

bool Symbol::isWideRef() {
  QualifiedType q = qualType();
  return (q.isWideRef() || type->symbol->hasFlag(FLAG_WIDE_REF));
}

bool Symbol::isRefOrWideRef() {
  return isRef() || isWideRef();
}


// Returns the scope in which the given symbol is declared; NULL otherwise.
BlockStmt* Symbol::getDeclarationScope() const {
  return (defPoint != NULL) ? defPoint->getScopeBlock() : NULL;
}


bool Symbol::hasFlag(Flag flag) const {
  CHECK_FLAG(flag);
  return flags[flag];
}


void Symbol::addFlag(Flag flag) {
  CHECK_FLAG(flag);
  flags.set(flag);
}


void Symbol::copyFlags(const Symbol* other) {
  flags |= other->flags;
  qual = other->qual;
}


void Symbol::removeFlag(Flag flag) {
  CHECK_FLAG(flag);
  flags.reset(flag);
}

bool Symbol::hasEitherFlag(Flag aflag, Flag bflag) const {
  return hasFlag(aflag) || hasFlag(bflag);
}

// Don't generate documentation for this symbol, either because it is private,
// or because the symbol should not be documented independent of privacy
bool Symbol::noDocGen() const {
  return hasFlag(FLAG_NO_DOC) || hasFlag(FLAG_PRIVATE);
}


void Symbol::addSymExpr(SymExpr* se) {

  // MPF 2016-11-08: Consider not tracking SymExprs
  // that refer to Symbols that have an immediate.
  // The reason is that immediates are usually
  // unique-ified and so the symbol for 3 would refer
  // to all the uses of 3, and that probably isn't adding
  // any value.

  if (symExprsTail == NULL) {
    se->symbolSymExprsPrev = NULL;
    se->symbolSymExprsNext = NULL;
    symExprsHead = se;
    symExprsTail = se;
  } else {
    SymExpr* oldTail = symExprsTail;
    se->symbolSymExprsPrev = oldTail;
    se->symbolSymExprsNext = NULL;
    symExprsTail = se;
    oldTail->symbolSymExprsNext = se;
  }
}

void Symbol::removeSymExpr(SymExpr* se) {
  SymExpr*& prev = se->symbolSymExprsPrev;
  SymExpr*& next = se->symbolSymExprsNext;
  if (next)
    next->symbolSymExprsPrev = prev;
  else
    symExprsTail = prev;

  if (prev)
    prev->symbolSymExprsNext = next;
  else
    symExprsHead = next;

  next = NULL;
  prev = NULL;
}


SymExpr* Symbol::firstSymExpr() const {
  return symExprsHead;
}

SymExpr* Symbol::lastSymExpr() const {
  return symExprsTail;
}

int Symbol::countDefs(int max) const {
  int ret = 0;
  for_SymbolDefs(def, this) {
    ret += 1;
    if (ret >= max) break;
  }
  return ret;
}

int Symbol::countUses(int max) const {
  int ret = 0;
  for_SymbolUses(use, this) {
    ret += 1;
    if (ret >= max) break;
  }
  return ret;
}

bool Symbol::isUsed() const {
  return (this->countUses(1) >= 1);
}

bool Symbol::isDefined() const {
  return (this->countDefs(1) >= 1);
}

SymExpr* Symbol::getSingleUse() const {
  SymExpr* ret = NULL;
  for_SymbolUses(use, this) {
    if (ret != NULL) return NULL;
    ret = use;
  }
  return ret;
}

SymExpr* Symbol::getSingleDef() const {
  SymExpr* ret = NULL;
  for_SymbolDefs(def, this) {
    if (ret != NULL) return NULL;
    ret = def;
  }
  return ret;
}


Expr* Symbol::getInitialization() const {
  // In theory, this should be the first "def" for the symbol,
  // but that might be obfuscated by PRIM_ADDR_OF.

  FnSymbol* fn = toFnSymbol(defPoint->parentSymbol);
  ModuleSymbol* mod = toModuleSymbol(defPoint->parentSymbol);
  if (fn == NULL && mod != NULL ) {
    // Global variables are initialized in their module init function
    fn = mod->initFn;
  }

  Expr* stmt;
  // We'll search statements starting with stmt for the one
  // initializing our variable.
  if (mod != NULL) {
    stmt = fn->body->body.head;
  } else {
    stmt = defPoint->getStmtExpr()->next;
  }

  const Symbol *curSym = this;
  const Symbol *refSym = NULL;

  while (stmt != NULL) {
    std::vector<SymExpr*> symExprs;
    collectSymExprs(stmt, symExprs);

    bool isDef = false;
    bool isUse = false;

    for_vector(SymExpr, se, symExprs) {
      Symbol* sym = se->symbol();
      if (sym == curSym || sym == refSym) {
        int result = isDefAndOrUse(se);
        isDef |= (result & 1);
        isUse |= (result & 2);
      }
    }

    if (isDef) {
      // first use/def of the variable is a def (normal case)
      return stmt->getStmtExpr();

    } else if (isUse) {
      bool handled = false;

      // handle PRIM_MOVE refTmp, PRIM_ADDR_OF curSym or
      // handle PRIM_MOVE refTmp, PRIM_SET_REFERENCE curSym
      if (CallExpr* call = toCallExpr(stmt)) {
        if (call->isPrimitive(PRIM_MOVE)) {
          SymExpr* dstSe = toSymExpr(call->get(1));
          CallExpr* getRef = toCallExpr(call->get(2));

          if (getRef != NULL) {
            if (getRef->isPrimitive(PRIM_ADDR_OF) ||
                getRef->isPrimitive(PRIM_SET_REFERENCE)) {
              // Start looking for the first def of the captured reference

              INT_ASSERT(dstSe);
              // Doesn't handle multiple refs before finding initialization
              INT_ASSERT(refSym == NULL);
              refSym = dstSe->symbol();
              handled = true;
            }
          }
        }
      }

      INT_ASSERT(handled); // did we encounter new AST pattern?
    }
    stmt = stmt->next;
  }

  INT_FATAL(defPoint, "couldn't find initialization");
  return NULL;
}


bool Symbol::isImmediate() const {
  return false;
}

bool isString(Symbol* symbol) {
  return isString(symbol->type);
}

bool isUserDefinedRecord(Symbol* symbol) {
  return isUserDefinedRecord(symbol->type);
}

/******************************** | *********************************
*                                                                   *
* Common base class for ArgSymbol and VarSymbol.                    *
* Also maintains a small amount of IPE specific state.              *
*                                                                   *
********************************* | ********************************/

LcnSymbol::LcnSymbol(AstTag      astTag,
                     const char* initName,
                     Type*       initType) :
  Symbol(astTag, initName, initType)
{
  mDepth  = -1;
  mOffset = -1;
}

LcnSymbol::~LcnSymbol()
{

}

void LcnSymbol::locationSet(int depth, int offset)
{
  mDepth  = depth;
  mOffset = offset;
}

int LcnSymbol::depth() const
{
  return mDepth;
}

int LcnSymbol::offset() const
{
  return mOffset;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

VarSymbol::VarSymbol(const char *init_name,
                     Type    *init_type) :
  LcnSymbol(E_VarSymbol, init_name, init_type),
  immediate(NULL),
  doc(NULL),
  isField(false),
  llvmDIGlobalVariable(NULL),
  llvmDIVariable(NULL)
{
  gVarSymbols.add(this);
  if (type == dtUnknown || type->symbol == NULL) {
    this->qual = QUAL_UNKNOWN;
  } else if (type->symbol->hasFlag(FLAG_REF)) {
    this->qual = QUAL_REF;
  } else if (type->symbol->hasFlag(FLAG_WIDE_REF)) {
    this->qual = QUAL_WIDE_REF;
  } else {
    this->qual = QUAL_VAL;
  }
}

VarSymbol::VarSymbol(AstTag astTag, const char* initName, Type* initType) :
  LcnSymbol(astTag, initName, initType),
  immediate(NULL),
  doc(NULL),
  isField(false),
  llvmDIGlobalVariable(NULL),
  llvmDIVariable(NULL)
{
  // The subclass is to take care of the rest.
}


VarSymbol::~VarSymbol() {
  if (immediate)
    delete immediate;
}


void VarSymbol::verify() {
  Symbol::verify();
  if (astTag != E_VarSymbol)
    INT_FATAL(this, "Bad VarSymbol::astTag");
  if (!type)
    INT_FATAL(this, "VarSymbol::type is NULL");
}


VarSymbol*
VarSymbol::copyInner(SymbolMap* map) {
  VarSymbol* newVarSymbol = new VarSymbol(name, type);
  newVarSymbol->copyFlags(this);
  newVarSymbol->cname = cname;
  INT_ASSERT(!newVarSymbol->immediate);
  return newVarSymbol;
}


void VarSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in VarSymbol::replaceChild");
}


bool VarSymbol::isConstant() const {
  return hasFlag(FLAG_CONST);
}


bool VarSymbol::isConstValWillNotChange() {
  // todo: how about QUAL_CONST ?
  return qual == QUAL_CONST_VAL         ||
         hasFlag(FLAG_REF_TO_IMMUTABLE) ||
         (hasFlag(FLAG_CONST) &&
          !(hasFlag(FLAG_REF_VAR) || isRef()));
}


bool VarSymbol::isParameter() const {
  return hasFlag(FLAG_PARAM) || immediate;
}


bool VarSymbol::isType() const {
  return hasFlag(FLAG_TYPE_VARIABLE);
}


std::string VarSymbol::docsDirective() {
  std::string result;
  if (fDocsTextOnly) {
    result = "";
  } else {
    // Global type aliases become type directives. Types that are also fields
    // could be generics, so let them be treated as regular fields (i.e. use
    // the attribute directive).
    if (this->isType() && !this->isField) {
      result = ".. type:: ";
    } else if (this->isField) {
      result = ".. attribute:: ";
    } else {
      result = ".. data:: ";
    }
  }
  return this->hasFlag(FLAG_CONFIG) ? result + "config " : result;
}


void VarSymbol::printDocs(std::ostream *file, unsigned int tabs) {
  if (this->noDocGen() || this->hasFlag(FLAG_SUPER_CLASS)) {
      return;
  }

  this->printTabs(file, tabs);
  *file << this->docsDirective();

  if (this->isType()) {
    *file << "type ";
  } else if (this->isConstant()) {
    *file << "const ";
  } else if (this->isParameter()) {
    *file << "param ";
  } else {
    *file << "var ";
  }

  AstToText info;
  info.appendVarDef(this);
  *file << info.text();

  *file << std::endl;

  // For .rst mode, put a line break after the .. data:: directive and
  // its description text.
  if (!fDocsTextOnly) {
    *file << std::endl;
  }

  if (this->doc != NULL) {
    this->printDocsDescription(this->doc, file, tabs + 1);
    if (!fDocsTextOnly) {
      *file << std::endl;
    }
  }
}


/*
 * For docs, when VarSymbol is used for class fields, identify them as such by
 * calling this function.
 */
void VarSymbol::makeField() {
  this->isField = true;
}

bool VarSymbol::isImmediate() const {
  return immediate != NULL;
}

void VarSymbol::accept(AstVisitor* visitor) {
  visitor->visitVarSym(this);
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

ArgSymbol::ArgSymbol(IntentTag iIntent, const char* iName,
                     Type* iType, Expr* iTypeExpr,
                     Expr* iDefaultExpr, Expr* iVariableExpr) :
  LcnSymbol(E_ArgSymbol, iName, iType),
  intent(iIntent),
  typeExpr(NULL),
  defaultExpr(NULL),
  variableExpr(NULL),
  instantiatedFrom(NULL),
  llvmDIFormal(NULL)
{
  if (intentsResolved) {
    if (iIntent == INTENT_BLANK || iIntent == INTENT_CONST) {
      INT_FATAL(this, "You can't create an argument with blank/const intent once intents have been resolved; please be more specific");
      // NOTE: One way to be more specific is to use the blankIntentForType()/
      // constIntentForType() routines to map a (possibly unknown) type to
      // the intent that blank/const would use for that type.
    }
  }
  if (!iTypeExpr)
    typeExpr = NULL;
  else if (BlockStmt* block = toBlockStmt(iTypeExpr))
    typeExpr = block;
  else
    typeExpr = new BlockStmt(iTypeExpr, BLOCK_TYPE);
  if (!iDefaultExpr)
    defaultExpr = NULL;
  else if (BlockStmt* block = toBlockStmt(iDefaultExpr))
    defaultExpr = block;
  else
    defaultExpr = new BlockStmt(iDefaultExpr, BLOCK_SCOPELESS);
  if (!iVariableExpr)
    variableExpr = NULL;
  else if (BlockStmt* block = toBlockStmt(iVariableExpr))
    variableExpr = block;
  else
    variableExpr = new BlockStmt(iVariableExpr, BLOCK_SCOPELESS);
  gArgSymbols.add(this);
}


void ArgSymbol::verify() {
  Symbol::verify();
  if (astTag != E_ArgSymbol) {
    INT_FATAL(this, "Bad ArgSymbol::astTag");
  }
  if (typeExpr && typeExpr->parentSymbol != this)
    INT_FATAL(this, "Bad ArgSymbol::typeExpr::parentSymbol");
  if (defaultExpr && defaultExpr->parentSymbol != this)
    INT_FATAL(this, "Bad ArgSymbol::defaultExpr::parentSymbol");
  if (variableExpr && variableExpr->parentSymbol != this)
    INT_FATAL(this, "Bad ArgSymbol::variableExpr::parentSymbol");
  // ArgSymbols appear only in formal parameter lists.
  if (defPoint) {
    FnSymbol* pfs = toFnSymbol(defPoint->parentSymbol);
    INT_ASSERT(pfs);
    INT_ASSERT(defPoint->list == &(pfs->formals));
  }
  if (intentsResolved) {
    if (intent == INTENT_BLANK || intent == INTENT_CONST) {
      INT_FATAL(this, "Arg '%s' (%d) has blank/const intent post-resolve", this->name, this->id);
    }
  }
  if (hasFlag(FLAG_REF_TO_IMMUTABLE))
    INT_ASSERT(intent == INTENT_CONST_REF);
  verifyNotOnList(typeExpr);
  verifyNotOnList(defaultExpr);
  verifyNotOnList(variableExpr);
  verifyInTree(instantiatedFrom, "ArgSymbol::instantiatedFrom");
}


ArgSymbol*
ArgSymbol::copyInner(SymbolMap* map) {
  ArgSymbol *ps = new ArgSymbol(intent, name, type, COPY_INT(typeExpr),
                                COPY_INT(defaultExpr), COPY_INT(variableExpr));
  ps->copyFlags(this);
  ps->cname = cname;
  ps->instantiatedFrom = instantiatedFrom;
  return ps;
}


void ArgSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == typeExpr)
    typeExpr = toBlockStmt(new_ast);
  else if (old_ast == defaultExpr)
    defaultExpr = toBlockStmt(new_ast);
  else if (old_ast == variableExpr)
    variableExpr = toBlockStmt(new_ast);
  else
    INT_FATAL(this, "Unexpected case in ArgSymbol::replaceChild");
}

bool ArgSymbol::isConstant() const {
  bool retval = false;

  switch (intent) {
  case INTENT_BLANK:
    retval = type->isDefaultIntentConst();
    break;

  case INTENT_CONST:
  case INTENT_CONST_IN:
  case INTENT_CONST_REF:
    retval = true;
    break;

  default:
    retval = false;
    break;
  }

  return retval;
}

// For an abstract 'intent', set absIntent=true.
// For a concrete 'intent', set absIntent=false and result=
// whether the formal with that intent does not change.
static void isConstValWillNotChangeHelp(IntentTag intent,
                                        bool& result, bool& absIntent) {
  switch (intent)
  {
    case INTENT_CONST_IN:
    case INTENT_PARAM:
      result = true; absIntent = false; return;

    case INTENT_IN:
    case INTENT_OUT:
    case INTENT_INOUT:
    case INTENT_REF:
    case INTENT_CONST_REF:
    case INTENT_REF_MAYBE_CONST:
      result = false; absIntent = false; return;

    case INTENT_CONST:
    case INTENT_BLANK:
      result = false; absIntent = true; return;

    case INTENT_TYPE:
      INT_ASSERT(false); // caller responsibility to avoid this
      result = false; absIntent = true; return; // dummy
  }

  return; // dummy
}  

bool ArgSymbol::isConstValWillNotChange() {
  if (hasFlag(FLAG_REF_TO_IMMUTABLE))
    return true;

  bool absIntent = false;
  bool result    = false;
  isConstValWillNotChangeHelp(intent, result, absIntent);

  if (absIntent) {
    // Try again with the corresponding concrete intent.
    // Caller is responsible that concreteIntent() succeeds.
    isConstValWillNotChangeHelp(concreteIntent(intent, type->getValType()),
                                result, absIntent);
    INT_ASSERT(!absIntent);
  }
  return result;
}

bool ArgSymbol::isParameter() const {
  return (intent == INTENT_PARAM);
}

bool ArgSymbol::isVisible(BaseAST* scope) const {
  return true;
}


const char* retTagDescrString(RetTag retTag) {
  switch (retTag) {
    case RET_VALUE:     return "value";
    case RET_REF:       return "ref";
    case RET_CONST_REF: return "const ref";
    case RET_PARAM:     return "param";
    case RET_TYPE:      return "type";
    default:            return "<unknown RetTag>";
  }
}


// describes this argument's intent (for use in an English sentence)
const char* ArgSymbol::intentDescrString() {
  switch (intent) {
    case INTENT_BLANK: return "default intent";
    case INTENT_IN: return "'in'";
    case INTENT_INOUT: return "'inout'";
    case INTENT_OUT: return "'out'";
    case INTENT_CONST: return "'const'";
    case INTENT_CONST_IN: return "'const in'";
    case INTENT_CONST_REF: return "'const ref'";
    case INTENT_REF_MAYBE_CONST: return "'const? ref'";
    case INTENT_REF: return "'ref'";
    case INTENT_PARAM: return "'param'";
    case INTENT_TYPE: return "'type'";
  }

  INT_FATAL(this, "unknown intent");
  return "unknown intent";
}

// describes the given intent (for use in an English sentence)
const char* intentDescrString(IntentTag intent) {
  switch (intent) {
    case INTENT_BLANK:     return "default intent";
    case INTENT_IN:        return "'in' intent";
    case INTENT_INOUT:     return "'inout' intent";
    case INTENT_OUT:       return "'out' intent";
    case INTENT_CONST:     return "'const' intent";
    case INTENT_CONST_IN:  return "'const in' intent";
    case INTENT_CONST_REF: return "'const ref' intent";
    case INTENT_REF_MAYBE_CONST: return "'const? ref' intent";
    case INTENT_REF:       return "'ref' intent";
    case INTENT_PARAM:     return "'param' intent";
    case INTENT_TYPE:      return "'type' intent";
    default:               return "<unknown intent>";
  }
}

void ArgSymbol::accept(AstVisitor* visitor) {
  if (visitor->enterArgSym(this) == true) {

    if (typeExpr)
      typeExpr->accept(visitor);

    if (defaultExpr)
      defaultExpr->accept(visitor);

    if (variableExpr)
      variableExpr->accept(visitor);

    visitor->exitArgSym(this);
  }
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

// todo: a constructor that also gives a type (and qualifier)?

ShadowVarSymbol::ShadowVarSymbol(ForallIntentTag iIntent, const char* name, Expr* spec):
  VarSymbol(E_ShadowVarSymbol, name, dtUnknown),
  intent(iIntent),
  // For task-private variables, set 'outerVarRep' to NULL.
  outerVarRep(new UnresolvedSymExpr(name)),
  specBlock(NULL),
  reduceGlobalOp(NULL),
  pruneit(false)
{
  if (intentsResolved)
    if (intent == TFI_DEFAULT || intent == TFI_CONST)
      INT_FATAL(this, "must be a concrete intent");

  // According to CallExpr::verify(), each CallExpr shall have a parentExpr.
  if (spec)
    specBlock = new BlockStmt(spec);

  gShadowVarSymbols.add(this);
}

void ShadowVarSymbol::verify() {
  Symbol::verify();
  if (astTag != E_ShadowVarSymbol)
    INT_FATAL(this, "Bad ShadowVarSymbol::astTag");
  if (!iteratorsLowered && !isReduce())
    INT_ASSERT(outerVarRep);
  if (outerVarRep && outerVarRep->parentSymbol != this)
    INT_FATAL(this, "Bad ShadowVarSymbol::outerVarRep::parentSymbol");
  if (specBlock && specBlock->parentSymbol != this)
    INT_FATAL(this, "Bad ShadowVarSymbol::specBlock::parentSymbol");
  if (outerVarRep) {
    verifyNotOnList(outerVarRep);
    // this assert holds already after scopeResolve
    INT_ASSERT(!normalized || isSymExpr(outerVarRep));
  }
  // for VarSymbol
  if (!type)
    INT_FATAL(this, "ShadowVarSymbol::type is NULL");
  verifyNotOnList(specBlock);
  if (!resolved) {
    // Verify that this symbol is on a ForallStmt::intentVariables() list.
    ForallStmt* pfs = toForallStmt(defPoint->parentExpr);
    INT_ASSERT(pfs);
    INT_ASSERT(defPoint->list == &(pfs->intentVariables()));
  }
  INT_ASSERT(isReduce() == (intent == TFI_REDUCE));
  if (!iteratorsLowered && specBlock != NULL)
    INT_ASSERT(isReduce());
}

void ShadowVarSymbol::accept(AstVisitor* visitor) {
  visitor->visitVarSym(this);
  if (outerVarRep)
    outerVarRep->accept(visitor);
  if (specBlock)
    specBlock->accept(visitor);
}

ShadowVarSymbol* ShadowVarSymbol::copyInner(SymbolMap* map) {
  ShadowVarSymbol* ss = new ShadowVarSymbol(intent, name, NULL);
  ss->type = type;
  ss->qual = qual;
  ss->outerVarRep = COPY_INT(outerVarRep);
  ss->specBlock   = COPY_INT(specBlock);
  ss->copyFlags(this);
  ss->cname = cname;
  return ss;
}

void ShadowVarSymbol::replaceChild(BaseAST* oldAst, BaseAST* newAst) {
  if (oldAst == outerVarRep)
    outerVarRep = toSymExpr(newAst);
  else if (oldAst == specBlock)
    specBlock = toBlockStmt(newAst);
  else
    INT_FATAL(this, "Unexpected case in ShadowVarSymbol::replaceChild");
}

bool ShadowVarSymbol::isConstant() const {
  switch (intent) {
    case TFI_DEFAULT:
      return type->isDefaultIntentConst();
    case TFI_CONST:
    case TFI_CONST_IN:
    case TFI_CONST_REF:
      return true;
    case TFI_IN:
    case TFI_REF:
    case TFI_REDUCE:
      return false;
  }
  return false; // dummy
}

bool ShadowVarSymbol::isConstValWillNotChange() {
  //
  // This is written to only be called post resolveIntents
  //
  INT_ASSERT(intent != TFI_DEFAULT && intent != TFI_CONST);
  return intent == TFI_CONST_IN;
}

// describes the intent (for use in an English sentence)
const char* ShadowVarSymbol::intentDescrString() const {
  switch (intent) {
    case TFI_DEFAULT:   return "default intent";
    case TFI_CONST:     return "'const' intent";
    case TFI_IN:        return "'in' intent";
    case TFI_CONST_IN:  return "'const in' intent";
    case TFI_REF:       return "'ref' intent";
    case TFI_CONST_REF: return "'const ref' intent";
    case TFI_REDUCE:    return "'reduce' intent";
  }
  INT_FATAL(this, "unknown intent");
  return "unknown intent"; //dummy
}

Expr* ShadowVarSymbol::reduceOpExpr() const {
  if (!specBlock)
    return NULL;
  INT_ASSERT(specBlock->body.length == 1);
  INT_ASSERT(isReduce());
  return specBlock->body.head;
}

void ShadowVarSymbol::removeSupportingReferences() {
  if (outerVarRep) outerVarRep->remove();
  if (specBlock)   specBlock->remove();
}

bool isOuterVarOfShadowVar(Expr* expr) {
  if (ShadowVarSymbol* ss = toShadowVarSymbol(expr->parentSymbol))
    if (expr == ss->outerVarSE())
      return true;
  return false;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

TypeSymbol::TypeSymbol(const char* init_name, Type* init_type) :
  Symbol(E_TypeSymbol, init_name, init_type),
    llvmType(NULL),
    llvmTbaaTypeDescriptor(NULL),
    llvmTbaaAccessTag(NULL), llvmConstTbaaAccessTag(NULL),
    llvmTbaaStructCopyNode(NULL), llvmConstTbaaStructCopyNode(NULL),
    llvmDIType(NULL),
    doc(NULL)
{
  addFlag(FLAG_TYPE_VARIABLE);
  if (!type)
    INT_FATAL(this, "TypeSymbol constructor called without type");
  type->addSymbol(this);
  gTypeSymbols.add(this);
}


void TypeSymbol::verify() {
  Symbol::verify();
  if (astTag != E_TypeSymbol) {
    INT_FATAL(this, "Bad TypeSymbol::astTag");
  }
  if (type->symbol != this)
    INT_FATAL(this, "TypeSymbol::type->symbol != TypeSymbol");
}


TypeSymbol*
TypeSymbol::copyInner(SymbolMap* map) {
  Type* new_type = COPY_INT(type);
  TypeSymbol* new_type_symbol = new TypeSymbol(name, new_type);
  new_type->addSymbol(new_type_symbol);
  new_type_symbol->copyFlags(this);
  new_type_symbol->cname = cname;
  return new_type_symbol;
}


void TypeSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in TypeSymbol::replaceChild");
}

void TypeSymbol::accept(AstVisitor* visitor) {
  if (visitor->enterTypeSym(this) == true) {

    if (type)
      type->accept(visitor);

    visitor->exitTypeSym(this);
  }
}

void TypeSymbol::renameInstantiatedMulti(SymbolMap& subs, FnSymbol* fn) {
  renameInstantiatedStart();

  bool notFirst = false;
  for_formals(formal, fn) {
    if (Symbol* value = subs.get(formal)) {
      if (!notFirst) {
        if (TypeSymbol* ts = toTypeSymbol(value)) {
          if (this->hasFlag(FLAG_TUPLE)) {
            if (this->hasFlag(FLAG_STAR_TUPLE)) {
              this->name = astr(istr(fn->numFormals()-1), "*", ts->name);
              this->cname = astr(this->cname, "star_", ts->cname);
              return;
            } else {
              this->name = astr("(");
            }
          }
        }
        notFirst = true;
      } else {
        this->name = astr(this->name, ",");
        this->cname = astr(this->cname, "_");
      }
      renameInstantiatedIndividual(value);
    }
  }

  renameInstantiatedEnd();
}

void TypeSymbol::renameInstantiatedSingle(Symbol* sym) {
  renameInstantiatedStart();
  if (this->hasFlag(FLAG_TUPLE)) {
    USR_FATAL(sym, "initializers don't handle tuples yet, sorry!");
  } else {
    renameInstantiatedIndividual(sym);
  }
  renameInstantiatedEnd();
}

void TypeSymbol::renameInstantiatedFromSuper(TypeSymbol* superSym) {
  renameInstantiatedStart();
  const char* afterParentName = std::find(superSym->name,
                                          superSym->name+strlen(superSym->name),
                                          '(');
  const char* afterParentCname = std::find(superSym->cname,
                                           superSym->cname+strlen(superSym->cname),
                                           '_');
  this->name  = astr(this->name , afterParentName+1);
  this->cname = astr(this->cname, afterParentCname+1);
  // Don't call renameInstantiatedEnd() because the parent name already has the
  // end parenthesis.
}

void TypeSymbol::renameInstantiatedStart() {
  if (this->name[strlen(this->name)-1] == ')') {
    // avoid "strange" instantiated type names based on partial instantiation
    //  instead of C(int,real)(imag) this results in C(int,real,imag)
    char* buf = (char*)malloc(strlen(this->name) + 1);
    memcpy(buf, this->name, strlen(this->name));
    buf[strlen(this->name)-1] = '\0';
    this->name = astr(buf, ",");
    free(buf);
  } else {
    this->name = astr(this->name, "(");
  }
  this->cname = astr(this->cname, "_");
}

void TypeSymbol::renameInstantiatedIndividual(Symbol* sym) {
  if (TypeSymbol* ts = toTypeSymbol(sym)) {
    if (!this->hasFlag(FLAG_STAR_TUPLE)) {
      this->name = astr(this->name, ts->name);
      this->cname = astr(this->cname, ts->cname);
    }
  } else {
    VarSymbol* var = toVarSymbol(sym);
    if (var && var->immediate) {
      Immediate* immediate = var->immediate;
      if (var->type == dtString || var->type == dtStringC)
        renameInstantiatedTypeString(this, var);
      else if (immediate->const_kind == NUM_KIND_BOOL) {
        // Handle boolean types specially.
        const char* name4bool = immediate->bool_value() ? "true" : "false";
        const char* cname4bool = immediate->bool_value() ? "T" : "F";
        this->name = astr(this->name, name4bool);
        this->cname = astr(this->cname, cname4bool);
      } else {
        const size_t bufSize = 128;
        char imm[bufSize];
        snprint_imm(imm, bufSize, *var->immediate);
        this->name = astr(this->name, imm);
        this->cname = astr(this->cname, imm);
      }
    } else {
      this->name = astr(this->name, sym->cname);
      this->cname = astr(this->cname, sym->cname);
    }
  }
}

void TypeSymbol::renameInstantiatedEnd() {
  this->name = astr(this->name, ")");
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
************************************** | *************************************/

FnSymbol::FnSymbol(const char* initName) : Symbol(E_FnSymbol, initName) {
  retType            = dtUnknown;
  where              = NULL;
  retExprType        = NULL;
  body               = new BlockStmt();
  thisTag            = INTENT_BLANK;
  retTag             = RET_VALUE;
  iteratorInfo       = NULL;
  _this              = NULL;
  _outer             = NULL;
  instantiatedFrom   = NULL;
  instantiationPoint = NULL;
  basicBlocks        = NULL;
  calledBy           = NULL;
  userString         = NULL;
  valueFunction      = NULL;
  codegenUniqueNum   = 1;
  doc                = NULL;
  retSymbol          = NULL;
  llvmDISubprogram   = NULL;
  _throwsError       = false;

  substitutions.clear();

  gFnSymbols.add(this);

  formals.parent = this;
}


FnSymbol::~FnSymbol() {
  if (iteratorInfo) {
    // Also set iterator class and iterator record iteratorInfo = NULL.
    if (iteratorInfo->iclass) {
      iteratorInfo->iclass->iteratorInfo = NULL;
    }

    if (iteratorInfo->irecord) {
      iteratorInfo->irecord->iteratorInfo = NULL;
    }

    delete iteratorInfo;
  }

  BasicBlock::clear(this);

  delete basicBlocks;

  if (calledBy) {
    delete calledBy;
  }
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
    if (!last || !last->isPrimitive(PRIM_RETURN))
      INT_FATAL(this, "Last statement in normalized function is not a return");
  }
  if (formals.parent != this)
    INT_FATAL(this, "Bad AList::parent in FnSymbol");
  if (where && where->parentSymbol != this)
    INT_FATAL(this, "Bad FnSymbol::where::parentSymbol");
  if (retExprType && retExprType->parentSymbol != this)
    INT_FATAL(this, "Bad FnSymbol::retExprType::parentSymbol");
  if (body && body->parentSymbol != this)
    INT_FATAL(this, "Bad FnSymbol::body::parentSymbol");

  for_alist(fExpr, formals) {
    DefExpr* argDef = toDefExpr(fExpr);
    INT_ASSERT(argDef);
    INT_ASSERT(isArgSymbol(argDef->sym));
  }

  verifyInTree(retType, "FnSymbol::retType");
  verifyNotOnList(where);
  verifyNotOnList(retExprType);
  verifyNotOnList(body);
  verifyInTree(_this, "FnSymbol::_this");
  verifyInTree(_outer, "FnSymbol::_outer");
  verifyInTree(instantiatedFrom, "FnSymbol::instantiatedFrom");
  verifyInTree(instantiationPoint, "FnSymbol::instantiationPoint");
  // TODO: do we want to go over this->substitutions, basicBlocks, calledBy ?
  // Should those even persist between passes?
  verifyInTree(valueFunction, "FnSymbol::valueFunction");
  verifyInTree(retSymbol, "FnSymbol::retSymbol");
}


FnSymbol*
FnSymbol::copyInner(SymbolMap* map) {
  // Copy members that are common to innerCopy and partialCopy.
  FnSymbol* copy = this->copyInnerCore(map);

  // Copy members that weren't set by copyInnerCore.
  copy->where       = COPY_INT(this->where);
  copy->body        = COPY_INT(this->body);
  copy->retExprType = COPY_INT(this->retExprType);
  copy->_this       = this->_this;

  return copy;
}


/** Copy over members common to both copyInner and partialCopy.
 *
 * \param map Map from symbols in the old function to symbols in the new one
 */
FnSymbol*
FnSymbol::copyInnerCore(SymbolMap* map) {
  FnSymbol* newFn = new FnSymbol(this->name);

  /* Copy the flags.
   *
   * TODO: See if it is necessary to copy flags both here and in the copy
   * method.
   */
  newFn->copyFlags(this);
  if (this->throwsError())
    newFn->throwsErrorInit();

  for_formals(formal, this) {
    newFn->insertFormalAtTail(COPY_INT(formal->defPoint));
  }

  // Copy members that are needed by both copyInner and partialCopy.
  newFn->astloc             = this->astloc;
  newFn->retType            = this->retType;
  newFn->thisTag            = this->thisTag;
  newFn->cname              = this->cname;
  newFn->_outer             = this->_outer;
  newFn->retTag             = this->retTag;
  newFn->instantiatedFrom   = this->instantiatedFrom;
  newFn->instantiationPoint = this->instantiationPoint;
  newFn->numPreTupleFormals = this->numPreTupleFormals;

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

  // Copy and insert the retExprType if it is present.
  if (this->retExprType != NULL) {
    newFn->retExprType = COPY_INT(this->retExprType);

    insert_help(newFn->retExprType, NULL, newFn);
  }

  /*
   * Because we are not copying the function's body we need to make the return
   * symbol available through other means.  To do this we first have to find
   * where the symbol is defined.  It may either be void, the _this symbol, a
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

      this->removeFlag(FLAG_EXPANDED_VARARGS);

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
     * Cases where the return symbol is gVoid or this->_this don't require any
     * additional actions.
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

  } else if (oldAst == retExprType) {
    retExprType = toBlockStmt(newAst);

  } else {
    INT_FATAL(this, "Unexpected case in FnSymbol::replaceChild");
  }
}

void
FnSymbol::insertAtHead(Expr* ast) {
  body->insertAtHead(ast);
}


void
FnSymbol::insertAtTail(Expr* ast) {
  body->insertAtTail(ast);
}


void
FnSymbol::insertAtHead(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertAtHead(new_Expr(format, args));
  va_end(args);
}


void
FnSymbol::insertAtTail(const char* format, ...) {
  va_list args;

  va_start(args, format);
  insertAtTail(new_Expr(format, args));
  va_end(args);
}


Symbol*
FnSymbol::getReturnSymbol() {
  if (this->retSymbol != NULL) {
    return this->retSymbol;

  } else {
    CallExpr* ret = toCallExpr(body->body.last());
    if (!ret || !ret->isPrimitive(PRIM_RETURN))
      INT_FATAL(this, "function is not normal");
    SymExpr* sym = toSymExpr(ret->get(1));
    if (!sym)
      INT_FATAL(this, "function is not normal");
    return sym->symbol();
  }
}


// Replace the return symbol with 'newRetSymbol',
// return the previous return symbol.
// If newRetType != NULL, also update fn->retType.
Symbol*
FnSymbol::replaceReturnSymbol(Symbol* newRetSymbol, Type* newRetType)
{
  // follows getReturnSymbol()
  CallExpr* ret = toCallExpr(this->body->body.last());
  if (!ret || !ret->isPrimitive(PRIM_RETURN))
    INT_FATAL(this, "function is not normal");
  SymExpr* sym = toSymExpr(ret->get(1));
  if (!sym)
    INT_FATAL(this, "function is not normal");
  Symbol* prevRetSymbol = sym->symbol();

  // updating
  sym->setSymbol(newRetSymbol);
  this->retSymbol = newRetSymbol;
  if (newRetType)
    this->retType = newRetType;

  return prevRetSymbol;
}


void
FnSymbol::insertBeforeEpilogue(Expr* ast) {
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


void
FnSymbol::insertIntoEpilogue(Expr* ast) {
  getOrCreateEpilogueLabel(); // always inserting into an epilogue
  CallExpr* ret = toCallExpr(body->body.last());
  ret->insertBefore(ast);
}


LabelSymbol*
FnSymbol::getEpilogueLabel() {
  CallExpr* ret = toCallExpr(body->body.last());
  if (!ret || !ret->isPrimitive(PRIM_RETURN))
    INT_FATAL(this, "function is not normal");
  for (Expr* last = ret; last; last = last->prev) {
    if (DefExpr* def = toDefExpr(last->prev)) {
      if (LabelSymbol* label = toLabelSymbol(def->sym)) {
        if (label->hasFlag(FLAG_EPILOGUE_LABEL)) {
          return label;
        }
      }
    }
  }
  return NULL;
}


LabelSymbol*
FnSymbol::getOrCreateEpilogueLabel() {
  LabelSymbol* label = getEpilogueLabel();
  if (!label) {
    label = new LabelSymbol(astr("_end", name));
    label->addFlag(FLAG_EPILOGUE_LABEL);

    CallExpr* ret = toCallExpr(body->body.last());
    ret->insertBefore(new DefExpr(label));
  }
  return label;
}

void
FnSymbol::insertFormalAtHead(BaseAST* ast) {
  Expr* toInsert = NULL;
  if (ArgSymbol* arg = toArgSymbol(ast))
    toInsert = new DefExpr(arg);
  else if (DefExpr* def = toDefExpr(ast))
    toInsert = def;
  else
    INT_FATAL(ast, "Bad argument to FnSymbol::insertFormalAtHead");
  formals.insertAtHead(toInsert);
  parent_insert_help(this, toInsert);
}


void
FnSymbol::insertFormalAtTail(BaseAST* ast) {
  Expr* toInsert = NULL;
  if (ArgSymbol* arg = toArgSymbol(ast))
    toInsert = new DefExpr(arg);
  else if (DefExpr* def = toDefExpr(ast))
    toInsert = def;
  else
    INT_FATAL(ast, "Bad argument to FnSymbol::insertFormalAtTail");
  formals.insertAtTail(toInsert);
  parent_insert_help(this, toInsert);
}


int
FnSymbol::numFormals() const {
  return formals.length;
}


ArgSymbol*
FnSymbol::getFormal(int i) {
  return toArgSymbol(toDefExpr(formals.get(i))->sym);
}

void
FnSymbol::collapseBlocks() {
  CollapseBlocks visitor;

  body->accept(&visitor);
}


//
// If the function is not currently marked as generic
//    then if it is generic
//      1) Update some flags
//      2) Return true to indicate the status has been modified
//
bool FnSymbol::tagIfGeneric() {
  bool retval = false;

  if (hasFlag(FLAG_GENERIC) == false) {
    int result = hasGenericFormals();

    // If this function has at least 1 generic formal
    if (result > 0) {
      addFlag(FLAG_GENERIC);

      if (retType != dtUnknown && hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
        retType->symbol->addFlag(FLAG_GENERIC);

        if (result == 2) {
          retType->hasGenericDefaults = true;
        }
      }

      retval = true;
    }
  }

  return retval;
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
int FnSymbol::hasGenericFormals() const {
  bool hasGenericFormal   = false;
  bool hasGenericDefaults =  true;
  int  retval             =     0;

  bool resolveInit = false;
  if (this->hasFlag(FLAG_METHOD) && _this) {
    if (AggregateType* at = toAggregateType(_this->type)) {
      if (at->initializerStyle != DEFINES_CONSTRUCTOR  &&
          strcmp(name, "init") == 0) {
        resolveInit = true;
      }
    }
  }

  for_formals(formal, this) {
    bool isGeneric = false;

    if (formal->intent == INTENT_PARAM) {
      isGeneric = true;

    } else if (formal->type->symbol->hasFlag(FLAG_GENERIC) == true) {
      if (formal->type->hasGenericDefaults     == false ||
          formal->hasFlag(FLAG_MARKED_GENERIC) == true ||
          formal                               == _this ||
          formal->hasFlag(FLAG_IS_MEME)        == true) {
        if (!(formal == _this && resolveInit)) {
          isGeneric = true;
        }
      }
    }

    if (isGeneric == true) {
      if (hasFlag(FLAG_EXPORT)) {
        if (!hasGenericFormal) {
          USR_FATAL_CONT(this,
                         "exported function `%s` can't be generic", name);
        }
        USR_PRINT(this,
                  "   formal argument '%s' causes it to be", formal->name);
      }
      hasGenericFormal = true;

      if (formal->defaultExpr == NULL) {
        hasGenericDefaults = false;
      }
    }
  }

  if (hasGenericFormal == false) {
    retval = 0;

  } else if (hasGenericDefaults == false) {
    retval = 1;

  } else if (hasGenericDefaults ==  true) {
    retval = 2;

  } else {
    INT_ASSERT(false);
  }

  return retval;
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

    if (retExprType) {
      retExprType->accept(visitor);
    }

    visitor->exitFnSym(this);
  }
}

// This function is a method on an aggregate type
bool FnSymbol::isMethod() const {
  return hasFlag(FLAG_METHOD);
}

// This function is a method on an aggregate type, defined within its
// declaration
bool FnSymbol::isPrimaryMethod() const {
  return hasFlag(FLAG_METHOD_PRIMARY);
}

// This function is a method on an aggregate type, defined outside its
// definition
bool FnSymbol::isSecondaryMethod() const {
  return isMethod() && !isPrimaryMethod();
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
  if (retTag == RET_REF)
    q = QUAL_REF;
  else if(retTag == RET_CONST_REF)
    q = QUAL_CONST_REF;
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


void FnSymbol::printDocs(std::ostream *file, unsigned int tabs) {
  if (this->noDocGen()) {
    return;
  }

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

// psahabu: this should eventually be in the constructor for
// FnSymbol, but it is needed for buildFunctionDecl()
void FnSymbol::throwsErrorInit() {
  _throwsError = true;
}

bool FnSymbol::throwsError() const {
  return _throwsError;
}

bool FnSymbol::retExprDefinesNonVoid() const {
  bool retval = true;

  if (retExprType == NULL) {
    retval = false;

  } else if (retExprType->length() != 1) {
    retval = true;

  } else if (SymExpr* expr = toSymExpr(retExprType->body.get(1))) {
    retval = expr->symbol()->type != dtVoid ? true : false;

  } else {
    retval = true;
  }

  return retval;
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
    int  start      =     1;
    bool first      =  true;
    bool skipParens = false;

    if (developer == true) {
      // report the name as-is and include all args
      retval = fn->name;

    } else {
      if (fn->instantiatedFrom != NULL) {
        fn = fn->instantiatedFrom;
      }

      if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) == true) {
        // if not, make sure 'str' is built as desired
        INT_ASSERT(strncmp("_type_construct_", fn->name, 16) == 0);
        retval = astr(fn->name + 16);

      } else if (fn->hasFlag(FLAG_CONSTRUCTOR) == true) {
        if (strncmp("_construct_", fn->name, 11) == 0) {
          retval = astr(fn->name + 11, ".init");

        } else if (strcmp("init", fn->name) == 0) {
          retval = "init";

        } else {
          INT_FATAL(fn,
                    "flagged as constructor but not named "
                    "_construct_ or init");
        }

      } else if (fn->isPrimaryMethod() == true) {
        Flag flag = FLAG_FIRST_CLASS_FUNCTION_INVOCATION;

        if (fn->name == astrThis) {
          INT_ASSERT(fn->hasFlag(flag) == true);

          retval = astr(toString(fn->getFormal(2)->type));
          start  = 2;

        } else {
          INT_ASSERT(fn->hasFlag(flag) == false);

          retval = astr(toString(fn->getFormal(2)->type), ".", fn->name);
          start  = 3;
        }

      } else if (fn->hasFlag(FLAG_MODULE_INIT) == true) {
        INT_ASSERT(strncmp("chpl__init_", fn->name, 11) == 0);

        retval = astr("top-level module statements for ", fn->name + 11);

      } else {
        retval = astr(fn->name);
      }
    }

    if        (fn->hasFlag(FLAG_NO_PARENS)        == true) {
      skipParens =  true;

    } else if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) == true &&
               fn->numFormals()                   ==    0) {
      skipParens =  true;

    } else if (fn->hasFlag(FLAG_MODULE_INIT)      == true &&
               developer                          == false) {
      skipParens =  true;

    } else {
      skipParens = false;
      retval     = astr(retval, "(");
    }

    for (int i = start; i <= fn->numFormals(); i++) {
      ArgSymbol* arg = fn->getFormal(i);

      if (arg->hasFlag(FLAG_IS_MEME) == false) {
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

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

EnumSymbol::EnumSymbol(const char* init_name) :
  Symbol(E_EnumSymbol, init_name) {
  gEnumSymbols.add(this);
}

void EnumSymbol::verify() {
  Symbol::verify();

  if (astTag != E_EnumSymbol) {
    INT_FATAL(this, "Bad EnumSymbol::astTag");
  }
}

EnumSymbol* EnumSymbol::copyInner(SymbolMap* map) {
  EnumSymbol* copy = new EnumSymbol(this->name);
  copy->copyFlags(this);
  return copy;
}

void EnumSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in EnumSymbol::replaceChild");
}

bool EnumSymbol::isParameter() const {
  return true;
}

Immediate* EnumSymbol::getImmediate() {
  if (SymExpr* init = toSymExpr(defPoint->init)) {
    if (VarSymbol* initvar = toVarSymbol(init->symbol())) {
      return initvar->immediate;
    }
  }
  return NULL;
}

void EnumSymbol::accept(AstVisitor* visitor) {
  visitor->visitEnumSym(this);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

LabelSymbol::LabelSymbol(const char* init_name) :
  Symbol(E_LabelSymbol, init_name, NULL),
  iterResumeGoto(NULL)
{
  gLabelSymbols.add(this);
}


void LabelSymbol::verify() {
  Symbol::verify();
  if (astTag != E_LabelSymbol) {
    INT_FATAL(this, "Bad LabelSymbol::astTag");
  }
  if (GotoStmt* igs = iterResumeGoto) {
    if (!isAlive(igs))
      INT_FATAL(this, "label's iterResumeGoto is not in AST");
    if (igs->gotoTag != GOTO_ITER_RESUME)
      INT_FATAL(this, "label's iterResumeGoto has unexpected gotoTag");
    if (getGotoLabelSymbol(igs) != this)
      INT_FATAL(this,"label's iterResumeGoto does not point back to the label");
  }
  // iterResumeGoto references a statement that is located somewhere in the AST
  // and so can be on a list.
}

LabelSymbol*
LabelSymbol::copyInner(SymbolMap* map) {
  LabelSymbol* copy = new LabelSymbol(name);
  copy->copyFlags(this);
  copy->cname = cname;
  if (iterResumeGoto) {
    MapElem<GotoStmt*,GotoStmt*>* rec =
      copiedIterResumeGotos.get_record(iterResumeGoto);
    if (rec) {
      // we gotta have the mapping because we handle each goto exactly once
      INT_ASSERT(rec->value);
      // update the copy
      copy->iterResumeGoto = rec->value;
      // indicate we are done with it
      rec->value = NULL;
      // printf("LabelSymbol-copy %d > %d  irg %d > %d\n", this->id, copy->id,
      //        iterResumeGoto->id, copy->iterResumeGoto->id);
    } else {
      // to be handled later - in GotoStmt::copyInner
      // printf("LabelSymbol-copy %d > %d  irg %d no action\n",
      //        this->id, copy->id, iterResumeGoto->id);
    }
  }
  return copy;
}

void LabelSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in LabelSymbol::replaceChild");
}

void LabelSymbol::accept(AstVisitor* visitor) {
  visitor->visitLabelSym(this);
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

std::string unescapeString(const char* const str, BaseAST *astForError) {
  std::string newString = "";
  char nextChar;
  int pos = 0;

  while((nextChar = str[pos++]) != '\0') {
    if(nextChar != '\\') {
      newString += nextChar;
      continue;
    }

    // handle \ escapes
    nextChar = str[pos++];
    switch(nextChar) {
      case '\'':
      case '\"':
      case '?':
      case '\\':
        newString += nextChar;
        break;
      case 'a':
        newString += '\a';
        break;
      case 'b':
        newString += '\b';
        break;
      case 'f':
        newString += '\f';
        break;
      case 'n':
        newString += '\n';
        break;
      case 'r':
        newString += '\r';
        break;
      case 't':
        newString += '\t';
        break;
      case 'v':
        newString += '\v';
        break;
      case 'x':
        {
          char buf[3];
          long num;
          buf[0] = buf[1] = buf[2] = '\0';
          if( str[pos] ) buf[0] = str[pos++];
          if( str[pos] ) buf[1] = str[pos++];
          num = strtol(buf, NULL, 16);
          newString += (char) num;
        }
        break;
      default:
        USR_FATAL(astForError, "Unexpected string escape: '\\%c'",  nextChar);
        break;
    }
  }
  return newString;
}

static int literal_id = 1;
HashMap<Immediate *, ImmHashFns, VarSymbol *> uniqueConstantsHash;
HashMap<Immediate *, ImmHashFns, VarSymbol *> stringLiteralsHash;
FnSymbol* initStringLiterals = NULL;
LabelSymbol* initStringLiteralsEpilogue = NULL;

void createInitStringLiterals() {
  SET_LINENO(stringLiteralModule);
  initStringLiterals = new FnSymbol("chpl__initStringLiterals");
  // We need to initialize strings literals on every locale, so we make this an
  // exported function that will be called in the runtime
  initStringLiterals->addFlag(FLAG_EXPORT);
  initStringLiterals->addFlag(FLAG_LOCAL_ARGS);
  initStringLiterals->retType = dtVoid;
  initStringLiterals->insertAtTail(new CallExpr(PRIM_RETURN, gVoid));
  stringLiteralModule->block->insertAtTail(new DefExpr(initStringLiterals));
}

// Note that string immediate values are stored
// with C escapes - that is newline is 2 chars \ n
// so this function expects a string that could be in "" in C
VarSymbol *new_StringSymbol(const char *str) {

  // Hash the string and return an existing symbol if found.
  // Aka. uniquify all string literals
  Immediate imm;
  imm.const_kind = CONST_KIND_STRING;
  imm.string_kind = STRING_KIND_STRING;
  imm.v_string = astr(str);
  VarSymbol *s = stringLiteralsHash.get(&imm);
  if (s) {
    return s;
  }

  if (resolved) {
    INT_FATAL("new_StringSymbol called after function resolution.");
  }

  // String (as record) literals are inserted from the very beginning on the
  // parser all the way through resolution (postFold). Since resolution happens
  // after normalization we need to insert everything in normalized form. We
  // also need to disable parts of normalize from running on literals inserted
  // at parse time.

  VarSymbol* cptrTemp = newTemp("call_tmp");
  cptrTemp->addFlag(FLAG_TYPE_VARIABLE);
  CallExpr *cptrCall = new CallExpr(PRIM_MOVE,
      cptrTemp,
      new CallExpr("_type_construct_c_ptr", new SymExpr(dtUInt[INT_SIZE_8]->symbol)));

  VarSymbol* castTemp = newTemp("call_tmp");
  CallExpr *castCall = new CallExpr(PRIM_MOVE,
      castTemp,
      createCast(new_CStringSymbol(str), cptrTemp));

  int strLength = unescapeString(str, castCall).length();

  CallExpr *ctor = new CallExpr("_construct_string",
      castTemp,
      new_IntSymbol(strLength),   // length
      new_IntSymbol(strLength ? strLength+1 : 0), // size, empty string needs 0
      gFalse);                    // owned = false
  ctor->insertAtTail(gFalse);     // needToCopy = false

  s = new VarSymbol(astr("_str_literal_", istr(literal_id++)), dtString);
  s->addFlag(FLAG_NO_AUTO_DESTROY);
  s->addFlag(FLAG_CONST);
  s->addFlag(FLAG_LOCALE_PRIVATE);
  s->addFlag(FLAG_CHAPEL_STRING_LITERAL);

  DefExpr* stringLitDef = new DefExpr(s);
  // DefExpr(s) always goes into the module scope to make it a global
  stringLiteralModule->block->insertAtTail(stringLitDef);

  CallExpr* ctorCall = new CallExpr(PRIM_MOVE, new SymExpr(s), ctor);

  if (initStringLiterals == NULL) {
    createInitStringLiterals();
    initStringLiteralsEpilogue = initStringLiterals->getOrCreateEpilogueLabel();
  }

  Expr* insertPt = initStringLiteralsEpilogue->defPoint;

  insertPt->insertBefore(new DefExpr(cptrTemp));
  insertPt->insertBefore(cptrCall);
  insertPt->insertBefore(new DefExpr(castTemp));
  insertPt->insertBefore(castCall);
  insertPt->insertBefore(ctorCall);

  s->immediate = new Immediate;
  *s->immediate = imm;
  stringLiteralsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_CStringSymbol(const char *str) {
  Immediate imm;
  imm.const_kind = CONST_KIND_STRING;
  imm.string_kind = STRING_KIND_C_STRING;
  imm.v_string = astr(str);
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  PrimitiveType* dtRetType = dtStringC;
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_cstr_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol* new_BoolSymbol(bool b, IF1_bool_type size) {
  Immediate imm;
  switch (size) {
  default:
    INT_FATAL( "unknown BOOL_SIZE");

  case BOOL_SIZE_1  :
  case BOOL_SIZE_SYS:
  case BOOL_SIZE_8  :
  case BOOL_SIZE_16 :
  case BOOL_SIZE_32 :
  case BOOL_SIZE_64 :
    break;
    // case BOOL_SIZE_128: imm.v_bool = b; break;
  }
  imm.v_bool = b;
  imm.const_kind = NUM_KIND_BOOL;
  imm.num_index = size;
  VarSymbol *s;
  // doesn't use uniqueConstantsHash because new_BoolSymbol is only
  // called to initialize dtBools[i]->defaultValue.
  // gTrue and gFalse are set up directly in initPrimitiveTypes.
  PrimitiveType* dtRetType = dtBools[size];
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  return s;
}

VarSymbol *new_IntSymbol(int64_t b, IF1_int_type size) {
  Immediate imm;
  switch (size) {
  case INT_SIZE_8  : imm.v_int8   = b; break;
  case INT_SIZE_16 : imm.v_int16  = b; break;
  case INT_SIZE_32 : imm.v_int32  = b; break;
  case INT_SIZE_64 : imm.v_int64  = b; break;
    // case INT_SIZE_128: imm.v_int128 = b; break;
  default:
    INT_FATAL( "unknown INT_SIZE");
  }
  imm.const_kind = NUM_KIND_INT;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  PrimitiveType* dtRetType = dtInt[size];
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_UIntSymbol(uint64_t b, IF1_int_type size) {
  Immediate imm;
  switch (size) {
  case INT_SIZE_8  : imm.v_uint8   = b; break;
  case INT_SIZE_16 : imm.v_uint16  = b; break;
  case INT_SIZE_32 : imm.v_uint32  = b; break;
  case INT_SIZE_64 : imm.v_uint64  = b; break;
    // case INT_SIZE_128: imm.v_uint128 = b; break;
  default:
    INT_FATAL( "unknown INT_SIZE");
  }
  imm.const_kind = NUM_KIND_UINT;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  PrimitiveType* dtRetType = dtUInt[size];
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

static VarSymbol* new_FloatSymbol(const char* n,
                                  IF1_float_type size, IF1_num_kind kind,
                                  Type* type) {
  Immediate imm;
  const char* normalized = NULL;

  switch (size) {
    case FLOAT_SIZE_32:
      imm.v_float32  = strtof(n, NULL);
      break;
    case FLOAT_SIZE_64:
      imm.v_float64  = strtod(n, NULL);
      break;
    default:
      INT_FATAL( "unknown FLOAT_SIZE");
  }
  imm.const_kind = kind;
  imm.num_index = size;

  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), type);
  rootModule->block->insertAtTail(new DefExpr(s));

  // Normalize the number for C99
  if (!strchr(n, '.') && !strchr(n, 'e') && !strchr(n, 'E') &&
      !strchr(n, 'p') && !strchr(n, 'P') ) {
    // Add .0 for floating point literals without a decimal point
    // or exponent.
    normalized = astr(n, ".0");
  } else if( n[0] == '0' && (n[1] == 'x' || n[1] == 'X') &&
             !strchr(n, 'p') && !strchr(n, 'P') ) {
    // Add p0 for hex floating point literals without an exponent
    // since C99 requires it (because f needs to be a suffix for
    // floating point numbers)
    normalized = astr(n, "p0");
  } else {
    normalized = astr(n);
  }

  // Use the normalized number when code-genning the literal
  s->cname = normalized;

  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_RealSymbol(const char *n, IF1_float_type size) {
  return new_FloatSymbol(n, size, NUM_KIND_REAL, dtReal[size]);
}

VarSymbol *new_ImagSymbol(const char *n, IF1_float_type size) {
  return new_FloatSymbol(n, size, NUM_KIND_IMAG, dtImag[size]);
}

VarSymbol *new_ComplexSymbol(const char *n, long double r, long double i,
                             IF1_complex_type size) {
  Immediate imm;
  switch (size) {
  case COMPLEX_SIZE_64:
    imm.v_complex64.r  = r;
    imm.v_complex64.i  = i;
    break;
  case COMPLEX_SIZE_128:
    imm.v_complex128.r = r;
    imm.v_complex128.i = i;
    break;
  default:
    INT_FATAL( "unknown COMPLEX_SIZE for complex");
  }
  imm.const_kind = NUM_KIND_COMPLEX;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s) {
    return s;
  }
  Type* dtRetType = dtComplex[size];
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  s->cname = astr(n);
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol* new_CommIDSymbol(int64_t b) {
  IF1_int_type size = INT_SIZE_64;
  Immediate imm;
  imm.v_int64 = b;

  imm.const_kind = NUM_KIND_COMMID;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  PrimitiveType* dtRetType = dtInt[size];
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

static Type*
immediate_type(Immediate *imm) {
  switch (imm->const_kind) {
    case CONST_KIND_STRING: {
      if (imm->string_kind == STRING_KIND_STRING) {
        return dtString;
      } else if (imm->string_kind == STRING_KIND_C_STRING) {
        return dtStringC;
      } else {
        INT_FATAL("unhandled string immediate type");
        break;
      }
    }
    case NUM_KIND_BOOL:
      return dtBools[imm->num_index];
    case NUM_KIND_UINT:
      return dtUInt[imm->num_index];
    case NUM_KIND_INT:
      return dtInt[imm->num_index];
    case NUM_KIND_REAL:
      return dtReal[imm->num_index];
    case NUM_KIND_IMAG:
      return dtImag[imm->num_index];
    case NUM_KIND_COMPLEX:
      return dtComplex[imm->num_index];
    default:
      USR_FATAL("bad immediate type");
      break;
  }
  return NULL;
}

VarSymbol *new_ImmediateSymbol(Immediate *imm) {
  VarSymbol *s = uniqueConstantsHash.get(imm);
  if (s)
    return s;
  Type *t = immediate_type(imm);
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), t);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  const size_t bufSize = 512;
  char str[bufSize];
  const char* ss = str;
  if (imm->const_kind == CONST_KIND_STRING)
    ss = imm->v_string;
  else
    snprint_imm(str, bufSize, *imm);
  s->cname = astr(ss);
  *s->immediate = *imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

// enable locally-unique temp names?
bool localTempNames = false;

// used to number the temps uniquely to top-level statements
// (give them smaller numbers, for readability of AST printouts)
static int tempID = 1;

void resetTempID() {
  tempID = 1;
}

FlagSet getRecordWrappedFlags(Symbol* s) {
  static FlagSet mask;
  if (mask.none()) {
    // Initialize the function-scope static variable.
    mask.set(FLAG_ARRAY);
    mask.set(FLAG_DOMAIN);
    mask.set(FLAG_DISTRIBUTION);
  }

  return s->flags & mask;
}


// cache some popular strings

const char* astrSdot = NULL;
const char* astrSequals = NULL;
const char* astr_cast = NULL;
const char* astrInit = NULL;
const char* astrDeinit = NULL;
const char* astrTag = NULL;
const char* astrThis = NULL;

void initAstrConsts() {
  astrSdot    = astr(".");
  astrSequals = astr("=");
  astr_cast   = astr("_cast");
  astrInit    = astr("init");
  astrDeinit  = astr("deinit");
  astrTag     = astr("tag");
  astrThis    = astr("this");
}

/************************************* | **************************************
*                                                                             *
* Create a temporary, with FLAG_TEMP and (optionally) FLAG_CONST.             *
*                                                                             *
************************************** | *************************************/

VarSymbol* newTemp(const char* name, QualifiedType qt) {
  VarSymbol* vs = newTemp(name, qt.type());
  vs->qual = qt.getQual();
  return vs;
}

VarSymbol* newTemp(QualifiedType qt) {
  return newTemp((const char*)NULL, qt);
}

VarSymbol* newTemp(const char* name, Type* type) {
  if (!name) {
    if (localTempNames)
      name = astr("_t", istr(tempID++), "_");
    else
      name = "tmp";
  }
  VarSymbol* vs = new VarSymbol(name, type);
  vs->addFlag(FLAG_TEMP);
  return vs;
}

VarSymbol* newTemp(Type* type) {
  return newTemp((const char*)NULL, type);
}

VarSymbol* newTempConst(const char* name, Type* type) {
  VarSymbol* result = newTemp(name, type);
  result->addFlag(FLAG_CONST);
  return result;
}

VarSymbol* newTempConst(Type* type) {
  VarSymbol* result = newTemp(type);
  result->addFlag(FLAG_CONST);
  return result;
}

VarSymbol* newTempConst(const char* name, QualifiedType qt) {
  VarSymbol* result = newTemp(name, qt);
  result->addFlag(FLAG_CONST);
  return result;
}

VarSymbol* newTempConst(QualifiedType qt) {
  VarSymbol* result = newTemp(qt);
  result->addFlag(FLAG_CONST);
  return result;
}
