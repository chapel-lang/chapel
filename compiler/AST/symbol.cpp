/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include "encoding-support.h"
#include "symbol.h"

#include "AstToText.h"
#include "AstVisitor.h"
#include "astutil.h"
#include "driver.h"
#include "ForallStmt.h"
#include "passes.h"
#include "resolveIntents.h"
#include "resolution.h"
#include "stringutil.h"
#include "type.h"
#include "wellknown.h"
#include "chpl/uast/OpCall.h"
#include "chpl/util/filtering.h"

#include "global-ast-vecs.h"

#include <algorithm>
#include <regex>
#include <cstring>
#include <map>

//
// The function that represents the compiler-generated entry point
//
Symbol *gNil = NULL;
Symbol *gUnknown = NULL;
Symbol *gMethodToken = NULL;
Symbol *gDummyRef = NULL;
Symbol *gFixupRequiredToken = NULL;
Symbol *gTypeDefaultToken = NULL;
Symbol *gLeaderTag = NULL, *gFollowerTag = NULL, *gStandaloneTag = NULL;
Symbol *gModuleToken = NULL;
Symbol *gNoInit = NULL;
Symbol *gSplitInit = NULL;
Symbol *gVoid = NULL;
Symbol *gNone = NULL;
Symbol *gFile = NULL;
Symbol *gStringC = NULL;
Symbol *gStringCopy = NULL;
Symbol *gOpaque = NULL;
Symbol *gTimer = NULL;
Symbol *gTaskID = NULL;
Symbol *gSyncVarAuxFields = NULL;
Symbol *gSingleVarAuxFields = NULL;

VarSymbol *gTrue = NULL;
VarSymbol *gFalse = NULL;
VarSymbol *gBoundsChecking = NULL;
VarSymbol *gCastChecking = NULL;
VarSymbol *gNilChecking = NULL;
VarSymbol *gOverloadSetsChecks = NULL;
VarSymbol *gDivZeroChecking = NULL;
VarSymbol* gCacheRemote = NULL;
VarSymbol* gPrivatization = NULL;
VarSymbol* gLocal = NULL;
VarSymbol* gWarnUnstable = NULL;
VarSymbol* gIteratorBreakToken = NULL;
VarSymbol* gNodeID = NULL;
VarSymbol *gModuleInitIndentLevel = NULL;
VarSymbol *gInfinity = NULL;
VarSymbol *gNan = NULL;
VarSymbol *gUninstantiated = NULL;
VarSymbol *gUseIOSerializers = NULL;

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
  deprecationMsg(""),
  unstableMsg(""),
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

  if (name && name != astr(name))
    INT_FATAL("name is not an astr");

  if (cname && cname != astr(cname))
    INT_FATAL("cname is not an astr");

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
  if (defPoint)
    return defPoint->inTree();
  // rootModule->defPoint is always NULL
  if (this == rootModule)
    return true;
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
  INT_FATAL("unknown intent");
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
  // we can't rename symbols that we're exporting or that are extern
  // because the other language will require the name to be as specified.
  // and we can't rename symbols that say not to.
  if (hasFlag(FLAG_EXPORT) ||
      hasFlag(FLAG_EXTERN) ||
      hasFlag(FLAG_NO_RENAME)) {
    return false;
  }
  return true;
}

// Returns the scope in which the given symbol is declared; NULL otherwise.
BlockStmt* Symbol::getDeclarationScope() const {
  return (defPoint != NULL) ? defPoint->getScopeBlock() : NULL;
}


bool Symbol::isKnownToBeGeneric() {
  if (FnSymbol* fn = toFnSymbol(this))
    return fn->isGenericIsValid() && fn->isGeneric();
  else
    return hasFlag(FLAG_GENERIC);
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

SymExpr* Symbol::getSingleDefUnder(Symbol* parent) const {
  SymExpr* ret = NULL;
  for_SymbolDefs(def, this) {
    if (ret != NULL) return NULL;
    if (def->parentSymbol != parent) continue;
    ret = def;
  }
  return ret;
}


Expr* Symbol::getInitialization() const {
  // In theory, this should be the first "def" for the symbol,
  // but that might be obfuscated by PRIM_ADDR_OF.

  // Q: Does this routine only make sense for VarSymbols?

  FnSymbol* fn = toFnSymbol(defPoint->parentSymbol);
  ModuleSymbol* mod = toModuleSymbol(defPoint->parentSymbol);
  if (fn == NULL && mod != NULL ) {
    // Global variables are initialized in their module init function, unless
    // it's used in a loopexpr wrapper function for an array type.
    //
    // BHARSH 2018-10-03: A temporary at global scope whose first SymExpr
    // is inside a loopexpr wrapper *should* have been initialized in that
    // wrapper function.
    if (firstSymExpr()->getFunction()->hasFlag(FLAG_MAYBE_ARRAY_TYPE) &&
        this->hasFlag(FLAG_TEMP)) {
      fn = firstSymExpr()->getFunction();
    } else {
      fn = mod->initFn;
    }
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
    collectSymExprsFor(stmt, curSym, refSym, symExprs);

    bool isDef = false;
    bool isUse = false;

    for_vector(SymExpr, se, symExprs) {
        int result = isDefAndOrUse(se);
        isDef |= (result & 1);
        isUse |= (result & 2);
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

      if (handled == false)
        break;
    }
    stmt = stmt->next;
  }

  return NULL;
}

const char* Symbol::getDeprecationMsg() const {
  if (deprecationMsg[0] == '\0') {
    const char* msg = astr(name, " is deprecated");
    return msg;
  }
  else {
    return deprecationMsg.c_str();
  }
}

const char* Symbol::getUnstableMsg() const {
  if (unstableMsg[0] == '\0') {
    const char* msg = astr(name, " is unstable");
    return msg;
  }
  else {
    return unstableMsg.c_str();
  }
}

// When printing the deprecation message to the console we typically
// want to filter out inline markup used for Sphinx (which is useful
// for when generating the docs). See:
// https://chapel-lang.org/docs/latest/tools/chpldoc/chpldoc.html#inline-markup-2
// for information on the markup.
const char* Symbol::getSanitizedMsg(std::string msg) const {
  return astr(chpl::removeSphinxMarkup(msg));
}

void Symbol::maybeGenerateDeprecationWarning(Expr* context) {
  if (!this->hasFlag(FLAG_DEPRECATED)) return;

  Symbol* contextParent = context->parentSymbol;
  bool parentDeprecated = contextParent->hasFlag(FLAG_DEPRECATED);
  bool compilerGenerated = contextParent->hasFlag(FLAG_COMPILER_GENERATED);

  // Traverse until we find a deprecated parent symbol, a compiler generated
  // parent symbol, or until we reach the highest outer scope
  while (contextParent != NULL && contextParent->defPoint != NULL &&
         contextParent->defPoint->parentSymbol != NULL &&
         parentDeprecated != true && compilerGenerated != true) {
    contextParent = contextParent->defPoint->parentSymbol;
    parentDeprecated = contextParent->hasFlag(FLAG_DEPRECATED);
    compilerGenerated = contextParent->hasFlag(FLAG_COMPILER_GENERATED);
  }

  // Only generate the warning if the location with the reference is not
  // created by the compiler or also deprecated.
  if (!compilerGenerated && !parentDeprecated) {
    USR_WARN(context, "%s", getSanitizedMsg(getDeprecationMsg()));
  }
}

static bool isInvisibleModule(Symbol* sym) {
  return sym == rootModule || sym == theProgram;
}

static bool isUnstableContext(Symbol* sym) {
  if (sym->hasFlag(FLAG_UNSTABLE)) return true;
  if (auto mod = toModuleSymbol(sym)) {
    if (isInvisibleModule(mod)) return false;
    if (mod->modTag == MOD_INTERNAL) return !fWarnUnstableInternal;
    if (mod->modTag == MOD_STANDARD) return !fWarnUnstableStandard;
  }
  return false;
}

static bool isUnstableShouldWarn(Symbol* sym, Expr* initialContext) {
  if (!sym->hasFlag(FLAG_UNSTABLE)) return false;
  auto mod = initialContext->getModule();
  INT_ASSERT(mod);
  if (mod->modTag == MOD_INTERNAL) return fWarnUnstableInternal;
  if (mod->modTag == MOD_STANDARD) return fWarnUnstableStandard;
  INT_ASSERT(mod->modTag == MOD_USER);
  return fWarnUnstable;
}

//based on maybeGenerateDeprecationWarning
void Symbol::maybeGenerateUnstableWarning(Expr* context) {
  if (!isUnstableShouldWarn(this, context)) return;

  Symbol* contextParent = context->parentSymbol;
  bool parentUnstable = isUnstableContext(contextParent);
  bool parentDeprecated = contextParent->hasFlag(FLAG_DEPRECATED);
  bool compilerGenerated = contextParent->hasFlag(FLAG_COMPILER_GENERATED);

  // Traverse until we find an unstable parent symbol, a deprecated parent
  // symbol, a compiler generated parent symbol, or until we reach the highest
  // outer scope.
  while (contextParent != NULL && contextParent->defPoint != NULL &&
         contextParent->defPoint->parentSymbol != NULL &&
         !isInvisibleModule(contextParent) &&
         parentUnstable != true && compilerGenerated != true &&
         parentDeprecated != true) {
    contextParent = contextParent->defPoint->parentSymbol;
    parentUnstable = isUnstableContext(contextParent);
    parentDeprecated = contextParent->hasFlag(FLAG_DEPRECATED);
    compilerGenerated = contextParent->hasFlag(FLAG_COMPILER_GENERATED);
  }

  // Only generate the warning if the location with the reference is not
  // created by the compiler, is not unstable, and is not deprecated.
  if (!compilerGenerated && !parentUnstable && !parentDeprecated) {
    USR_WARN(context, "%s", getSanitizedMsg(getUnstableMsg()));
  }
}

bool Symbol::isImmediate() const {
  return false;
}

bool isString(Symbol* symbol) {
  return isString(symbol->type);
}

bool isBytes(Symbol* symbol) {
  return isBytes(symbol->type);
}

/******************************** | *********************************
*                                                                   *
* Common base class for ArgSymbol and VarSymbol.                    *
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

VarSymbol::VarSymbol(const char* init_name, QualifiedType qType) :
  LcnSymbol(E_VarSymbol, init_name, qType.type()),
  immediate(NULL),
  isField(false),
  llvmDIGlobalVariable(NULL),
  llvmDIVariable(NULL)
{
  gVarSymbols.add(this);

  this->qual = qType.getQual();
}

VarSymbol::VarSymbol(AstTag astTag, const char* initName, Type* initType) :
  LcnSymbol(astTag, initName, initType),
  immediate(NULL),
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
  newVarSymbol->qual = qual;
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
  originalIntent(iIntent),
  typeExprFromDefaultExpr(false),
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
    if (FnSymbol* pfs = toFnSymbol(defPoint->parentSymbol)) {
      INT_ASSERT(defPoint->list == &(pfs->formals));
    } else {
      INT_ASSERT(isTiMark(this));
    }
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
  ps->typeExprFromDefaultExpr = typeExprFromDefaultExpr;
  ps->cname = cname;
  ps->instantiatedFrom = instantiatedFrom;
  ps->originalIntent = this->originalIntent;
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
const char* ArgSymbol::intentDescrString() const {
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
  return "<unknown intent>";
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
  }
  INT_FATAL("unknown intent");
  return "<unknown intent>";
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

std::string ArgSymbol::demungeVarArgName(std::string* num) {
  std::string name = this->name;
  if (!this->hasFlag(FLAG_EXPANDED_VARARGS)) {
    INT_FATAL(this, "demungeVarArgName() called on non-vararg ArgSymbol");
  }
  std::string mynum = name;
  mynum.erase(0, 2); // remove _e
  std::string n = mynum; // ##_name
  mynum.resize(mynum.find('_')); // ##
  n.erase(0, n.find('_')+1); // name
  if (num != NULL) {
    *num = mynum;
  }
  return n;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

// todo: a constructor that also gives a type (and qualifier)?

ShadowVarSymbol::ShadowVarSymbol(ForallIntentTag iIntent,
                                 const char* name,
                                 SymExpr* outerVar,
                                 Expr* spec):
  VarSymbol(E_ShadowVarSymbol, name, dtUnknown),
  intent(iIntent),
  outerVarSE(outerVar),
  specBlock(NULL),
  svInitBlock(new BlockStmt()),
  svDeinitBlock(new BlockStmt()),
  svExplicit(false)
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

  if (outerVarSE && outerVarSE->parentSymbol != this)
    INT_FATAL(this, "Bad ShadowVarSymbol::outerVarSE::parentSymbol");
  if (specBlock && specBlock->parentSymbol != this)
    INT_FATAL(this, "Bad ShadowVarSymbol::specBlock::parentSymbol");
  verifyNotOnList(outerVarSE);
  // for VarSymbol
  if (!type)
    INT_FATAL(this, "ShadowVarSymbol::type is NULL");
  verifyNotOnList(specBlock);
  if (!resolved) {
    // Verify that this symbol is on a ForallStmt::shadowVariables() list.
    ForallStmt* pfs = toForallStmt(defPoint->parentExpr);
    INT_ASSERT(pfs);
    INT_ASSERT(defPoint->list == &(pfs->shadowVariables()));
  }
  if (specBlock != NULL)
    INT_ASSERT(intent == TFI_REDUCE || intent == TFI_REDUCE_OP);
  INT_ASSERT(!iteratorsLowered); // should be gone at lowerIterators
}

void ShadowVarSymbol::accept(AstVisitor* visitor) {
  visitor->visitVarSym(this);
  if (outerVarSE)
    outerVarSE->accept(visitor);
  if (specBlock)
    specBlock->accept(visitor);

  svInitBlock->accept(visitor);
  svDeinitBlock->accept(visitor);
}

ShadowVarSymbol* ShadowVarSymbol::copyInner(SymbolMap* map) {
  ShadowVarSymbol* ss = new ShadowVarSymbol(intent, name,
                                            COPY_INT(outerVarSE), NULL);
  ss->type = type;
  ss->qual = qual;
  ss->specBlock     = COPY_INT(specBlock);
  ss->svInitBlock   = COPY_INT(svInitBlock);
  ss->svDeinitBlock = COPY_INT(svDeinitBlock);

  ss->copyFlags(this);
  ss->cname = cname;
  ss->svExplicit = svExplicit;
  return ss;
}

void ShadowVarSymbol::replaceChild(BaseAST* oldAst, BaseAST* newAst) {
  if (oldAst == outerVarSE)
    outerVarSE = toSymExpr(newAst);
  else if (oldAst == specBlock)
    specBlock = toBlockStmt(newAst);
  else if (oldAst == svInitBlock)
    svInitBlock = toBlockStmt(newAst);
  else if (oldAst == svDeinitBlock)
    svDeinitBlock = toBlockStmt(newAst);
  else
    INT_FATAL(this, "Unexpected case in ShadowVarSymbol::replaceChild");
}

bool ShadowVarSymbol::isConstant() const {
  switch (intent)
  {
    case TFI_DEFAULT:
      return type->isDefaultIntentConst();

    case TFI_CONST:
    case TFI_CONST_IN:
    case TFI_CONST_REF:
    case TFI_IN_PARENT:
      return true;

    case TFI_IN:
    case TFI_REF:
    case TFI_REDUCE:
    case TFI_REDUCE_OP:
    case TFI_REDUCE_PARENT_AS:
    case TFI_REDUCE_PARENT_OP:
      return false;

    case TFI_TASK_PRIVATE:
      return VarSymbol::isConstant();
  }
  return false; // dummy
}

bool ShadowVarSymbol::isConstValWillNotChange() {
  switch (intent) {
    case TFI_DEFAULT:
    case TFI_CONST:
      // Caller responsibility - no abstract intents please.
      INT_ASSERT(false);
      return false;

    case TFI_CONST_IN:
    case TFI_IN_PARENT: // should this be here?
      return true;

    case TFI_CONST_REF:
    case TFI_IN:
    case TFI_REF:
    case TFI_REDUCE:
    case TFI_REDUCE_OP:
    case TFI_REDUCE_PARENT_AS:
    case TFI_REDUCE_PARENT_OP:
      return false;

    case TFI_TASK_PRIVATE:
      return VarSymbol::isConstValWillNotChange();
  }
  return false; // dummy
}

// describes the intent (for use in an English sentence)
const char* ShadowVarSymbol::intentDescrString() const {
  switch (intent) {
    case TFI_DEFAULT:       return "default intent";
    case TFI_CONST:         return "'const' intent";
    case TFI_IN_PARENT:     return "parent-in intent";
    case TFI_IN:            return "'in' intent";
    case TFI_CONST_IN:      return "'const in' intent";
    case TFI_REF:           return "'ref' intent";
    case TFI_CONST_REF:     return "'const ref' intent";
    case TFI_REDUCE:        return "'reduce' intent";
    case TFI_REDUCE_OP:        return "reduce-Op intent";
    case TFI_REDUCE_PARENT_AS: return "parent-reduce-AS intent";
    case TFI_REDUCE_PARENT_OP: return "parent-reduce-Op intent";
    case TFI_TASK_PRIVATE:  return "task-private intent";
  }
  INT_FATAL(this, "unknown intent");
  return "unknown intent"; //dummy
}

// in foralls.cpp: buildFromArgIntent(), buildFromReduceIntent()
// in expr.h: outerVarSym()

Expr* ShadowVarSymbol::reduceOpExpr() const {
  if (!specBlock)
    return NULL;
  INT_ASSERT(specBlock->body.length == 1);
  INT_ASSERT(isReduce());
  return specBlock->body.head;
}

ShadowVarSymbol* ShadowVarSymbol::ParentvarForIN() const {
  const ShadowVarSymbol* SI = this;
  DefExpr* soDef = toDefExpr(SI->defPoint->prev);
  ShadowVarSymbol* SO = toShadowVarSymbol(soDef->sym);
  INT_ASSERT(SO->intent == TFI_IN_PARENT);
  return SO;
}

ShadowVarSymbol* ShadowVarSymbol::INforParentvar() const {
  const ShadowVarSymbol* SO = this;
  DefExpr* siDef = toDefExpr(SO->defPoint->next);
  ShadowVarSymbol* SI = toShadowVarSymbol(siDef->sym);
  INT_ASSERT(SI->intent == TFI_IN || SI->intent == TFI_CONST_IN);
  return SI;
}

ShadowVarSymbol* ShadowVarSymbol::ReduceOpForAccumState() const {
  const ShadowVarSymbol* AS = this;
  DefExpr* rpDef = toDefExpr(AS->defPoint->prev);
  ShadowVarSymbol* RP = toShadowVarSymbol(rpDef->sym);
  INT_ASSERT(RP->intent == TFI_REDUCE_OP);
  return RP;
}

ShadowVarSymbol* ShadowVarSymbol::AccumStateForReduceOp() const {
  const ShadowVarSymbol* RP = this;
  DefExpr* asDef = toDefExpr(RP->defPoint->next);
  ShadowVarSymbol* AS = toShadowVarSymbol(asDef->sym);
  INT_ASSERT(AS->intent == TFI_REDUCE);
  return AS;
}

ShadowVarSymbol* ShadowVarSymbol::ReduceOpForParentRP() const {
  const ShadowVarSymbol* PRP = this;
  DefExpr* rpDef = toDefExpr(PRP->defPoint->next->next);
  ShadowVarSymbol* RP = toShadowVarSymbol(rpDef->sym);
  INT_ASSERT(RP->intent == TFI_REDUCE_OP);
  return RP;
}

ShadowVarSymbol* ShadowVarSymbol::AccumStateForParentAS() const {
  const ShadowVarSymbol* PAS = this;
  DefExpr* asDef = toDefExpr(PAS->defPoint->next->next);
  ShadowVarSymbol* AS = toShadowVarSymbol(asDef->sym);
  INT_ASSERT(AS->intent == TFI_REDUCE);
  return AS;
}

void ShadowVarSymbol::removeSupportingReferences() {
  if (outerVarSE)    outerVarSE->remove();
  if (specBlock)     specBlock->remove();
  if (svInitBlock)   svInitBlock->remove();
  if (svDeinitBlock) svDeinitBlock->remove();
}

bool isOuterVarOfShadowVar(Expr* expr) {
  if (ShadowVarSymbol* ss = toShadowVarSymbol(expr->parentSymbol))
    if (expr == ss->outerVarSE)
      return true;
  return false;
}

/******************************** | *********************************
*                                                                   *
*                                                                   *
********************************* | ********************************/

#ifdef HAVE_LLVM
static std::map<FunctionType*, llvm::FunctionType*>
chapelFunctionTypeToLlvmFunctionType;

bool llvmMapUnderlyingFunctionType(FunctionType* k, llvm::FunctionType* v) {
  auto it = chapelFunctionTypeToLlvmFunctionType.find(k);
  if (it != chapelFunctionTypeToLlvmFunctionType.end()) return false;
  chapelFunctionTypeToLlvmFunctionType.emplace_hint(it, k, v);
  return true;
}

llvm::FunctionType* llvmGetUnderlyingFunctionType(FunctionType* t) {
  auto it = chapelFunctionTypeToLlvmFunctionType.find(t);
  if (it != chapelFunctionTypeToLlvmFunctionType.end()) return it->second;
  return nullptr;
}
#endif

TypeSymbol::TypeSymbol(const char* init_name, Type* init_type) :
  Symbol(E_TypeSymbol, init_name, init_type),
    llvmImplType(NULL),
    llvmTbaaTypeDescriptor(NULL),
    llvmTbaaAccessTag(NULL), llvmConstTbaaAccessTag(NULL),
    llvmTbaaAggTypeDescriptor(NULL),
    llvmTbaaStructCopyNode(NULL), llvmConstTbaaStructCopyNode(NULL),
    llvmDIType(NULL),
    instantiationPoint(NULL),
    userInstantiationPointLoc(0, NULL)
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
  new_type_symbol->instantiationPoint = instantiationPoint;
  if (AggregateType* at = toAggregateType(new_type)) {
    for_fields(field, at) {
      insert_help(field->defPoint, NULL, new_type_symbol);
    }
  }
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

TemporaryConversionSymbol::TemporaryConversionSymbol(chpl::ID symId)
  : Symbol(E_TemporaryConversionSymbol, "<conv>", nullptr),
    symId(symId), sig(nullptr)
{
  gTemporaryConversionSymbols.add(this);
}

TemporaryConversionSymbol::TemporaryConversionSymbol(
    const chpl::resolution::TypedFnSignature* sig)
  : Symbol(E_TemporaryConversionSymbol, "<conv>", nullptr),
    symId(), sig(sig)
{
  gTemporaryConversionSymbols.add(this);
}

void TemporaryConversionSymbol::verify() {
}

TemporaryConversionSymbol*
TemporaryConversionSymbol::copyInner(SymbolMap* map) {
  TemporaryConversionSymbol* copy = nullptr;
  if (sig) {
    copy = new TemporaryConversionSymbol(sig);
  } else {
    copy = new TemporaryConversionSymbol(symId);
  }
  copy->copyFlags(this);
  return copy;
}

void TemporaryConversionSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in TemporaryConversionSymbol::replaceChild");
}

void TemporaryConversionSymbol::accept(AstVisitor* visitor) {
  visitor->visitTemporaryConversionSymbol(this);
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static int literal_id = 1;
HashMap<Immediate *, ImmHashFns, VarSymbol *> uniqueConstantsHash;

// stringLiteralsHash should never contain any invalid string
HashMap<Immediate *, ImmHashFns, VarSymbol *> stringLiteralsHash;
HashMap<Immediate *, ImmHashFns, VarSymbol *> bytesLiteralsHash;
typedef MapElem<Immediate*, VarSymbol*> StringLiteralHashElem;

LabelSymbol* initStringLiteralsEpilogue = NULL;

// This is called after all of the string literals are accumulated
// and after they are dead-code-eliminated.
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

  initStringLiteralsEpilogue = initStringLiterals->getOrCreateEpilogueLabel();

  if (fMinimalModules) {
    return;
  }

  // accumulate the string/bytes and prepare to sort them
  std::vector<std::pair<std::string, VarSymbol*>> literals;

  for_alist(expr, stringLiteralModule->block->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (VarSymbol* s = toVarSymbol(def->sym)) {
        if (s->hasFlag(FLAG_CHAPEL_STRING_LITERAL) ||
            s->hasFlag(FLAG_CHAPEL_BYTES_LITERAL)) {
          Immediate* imm = s->immediate;
          INT_ASSERT(imm);
          literals.push_back({imm->to_string(), s});
        }
      }
    }
  }

  // sort the strings/bytes so that they can be stored in a reliable order
  std::sort(literals.begin(), literals.end());

  // now emit initialization for them
  Expr* insertPt = initStringLiteralsEpilogue->defPoint;

  VarSymbol* buffer = new VarSymbol("literalsBuf", dtStringC);
  DefExpr* bufferDef = new DefExpr(buffer);
  insertPt->insertBefore(bufferDef);

  VarSymbol* offset = new VarSymbol("offset", SIZE_TYPE);
  insertPt->insertBefore(new DefExpr(offset));
  insertPt->insertBefore(new CallExpr(PRIM_MOVE, offset, new_IntSymbol(0)));

  VarSymbol* one = new_IntSymbol(1);

  int64_t bufferSize = 0;

  INT_ASSERT(gChplCreateStringWithLiteral != NULL);
  INT_ASSERT(gChplCreateBytesWithLiteral != NULL);

  // initialize the strings
  for (const auto& pair : literals) {
    VarSymbol* s = pair.second;

    // unescape the string and compute its length
    std::string unescapedString =
      chpl::unescapeStringC(s->immediate->to_string());
    int64_t numCodepoints = 0;

    if (s->hasFlag(FLAG_CHAPEL_STRING_LITERAL)) {
      // make sure the string is valid UTF-8
      const bool ok = isValidString(unescapedString, &numCodepoints);
      INT_ASSERT(ok); // should be checked earlier
    }

    int64_t strLength = unescapedString.length();
    const char* cstr = s->immediate->string_value();

    VarSymbol* strLenVar = new_IntSymbol(strLength);
    VarSymbol* cstrTemp = newTemp("call_tmp", dtStringC);
    CallExpr *cstrMove = new CallExpr(PRIM_MOVE, cstrTemp,
                                      new_CStringSymbol(cstr));

    CallExpr *initCall = nullptr;

    // call a function to initialize it appropriately
    if (s->hasFlag(FLAG_CHAPEL_STRING_LITERAL)) {
      initCall = new CallExpr(gChplCreateStringWithLiteral,
                              buffer,
                              offset,
                              cstrTemp,
                              strLenVar,
                              new_IntSymbol(numCodepoints));
    } else {
      INT_ASSERT(s->hasFlag(FLAG_CHAPEL_BYTES_LITERAL));
      initCall = new CallExpr(gChplCreateBytesWithLiteral,
                              buffer,
                              offset,
                              cstrTemp,
                              strLenVar);
    }

    CallExpr* moveCall = new CallExpr(PRIM_MOVE, s, initCall);
    CallExpr* addCall = new CallExpr(PRIM_ADD_ASSIGN, offset, strLenVar);//data
    CallExpr* incCall = new CallExpr(PRIM_ADD_ASSIGN, offset, one); //null

    insertPt->insertBefore(new DefExpr(cstrTemp));
    insertPt->insertBefore(cstrMove);
    insertPt->insertBefore(moveCall);
    insertPt->insertBefore(addCall);
    insertPt->insertBefore(incCall);

    bufferSize += strLength+1; // string data and null
  }

  // emit the call to allocate_string_literals_buf and put it
  // just after the buffer is defined
  CallExpr *allocCall = new CallExpr(gAllocateStringLiteralsBuf,
                                     new_IntSymbol(bufferSize));
  CallExpr *moveBuf = new CallExpr(PRIM_MOVE, buffer, allocCall);
  bufferDef->insertAfter(moveBuf);
}

bool isValidString(std::string str, int64_t* numCodepoints) {
  return chpl_enc_validate_buf(str.c_str(), str.length(), numCodepoints) == 0;
}

// Note that string immediate values are stored
// with C escapes - that is newline is 2 chars \ n
// so this function expects a string that could be in "" in C
VarSymbol *new_StringSymbol(const char *str) {

  // Hash the string and return an existing symbol if found.
  // Aka. uniquify all string literals
  size_t len = strlen(str);
  Immediate imm(gContext, str, len, STRING_KIND_STRING);
  VarSymbol *s = stringLiteralsHash.get(&imm);
  if (s) {
    return s;
  }

  if (resolved) {
    INT_FATAL("new_StringSymbol called after function resolution.");
  }

  bool invalid = false;

  // String (as record) literals are inserted from the very beginning on the
  // parser all the way through resolution (postFold). Since resolution happens
  // after normalization we need to insert everything in normalized form. We
  // also need to disable parts of normalize from running on literals inserted
  // at parse time.
  s = new VarSymbol(astr("_str_literal_", istr(literal_id++)), dtString);
  s->addFlag(FLAG_NO_AUTO_DESTROY);
  s->addFlag(FLAG_CONST);
  s->addFlag(FLAG_LOCALE_PRIVATE);
  s->addFlag(FLAG_CHAPEL_STRING_LITERAL);

  DefExpr* stringLitDef = new DefExpr(s);
  // DefExpr(s) always goes into the module scope to make it a global
  stringLiteralModule->block->insertAtTail(stringLitDef);

  s->immediate = new Immediate;
  *s->immediate = imm;

  std::string unescapedString = chpl::unescapeStringC(str);
  int64_t numCodepoints = 0;
  const bool ret = isValidString(unescapedString, &numCodepoints);
  if (!ret) {
    USR_FATAL_CONT(s, "Invalid string literal");

    // We want to keep the compilation going here so that we can catch other
    // invalid string literals without having to compile again. However,
    // returning `s` (i.e. NULL at this point) does not work well with the rest
    // of the compilation. At the same time we should avoid adding invalid
    // sequences to stringLiteralsHash. Therefore, set a flag to note that this
    // string is invalid and should not be added to stringLiteralsHash.
    invalid = true;
  }

  if (!invalid) {
    stringLiteralsHash.put(s->immediate, s);
  }

  // String literal init function should be not created yet.
  // Otherwise, the new string global will not be initialized.
  INT_ASSERT(initStringLiterals == NULL);
  return s;
}


VarSymbol *new_BytesSymbol(const char *str) {
  size_t len = strlen(str);
  Immediate imm(gContext, str, len, STRING_KIND_BYTES);
  VarSymbol *s = bytesLiteralsHash.get(&imm);
  if (s) {
    return s;
  }

  if (resolved) {
    INT_FATAL("new_BytesSymbol called after function resolution.");
  }

  // Bytes (as record) literals are inserted from the very beginning on the
  // parser all the way through resolution (postFold). Since resolution happens
  // after normalization we need to insert everything in normalized form. We
  // also need to disable parts of normalize from running on literals inserted
  // at parse time.
  s = new VarSymbol(astr("_bytes_literal_", istr(literal_id++)), dtBytes);
  s->addFlag(FLAG_NO_AUTO_DESTROY);
  s->addFlag(FLAG_CONST);
  s->addFlag(FLAG_LOCALE_PRIVATE);
  s->addFlag(FLAG_CHAPEL_BYTES_LITERAL);

  DefExpr* bytesLitDef = new DefExpr(s);
  // DefExpr(s) always goes into the module scope to make it a global
  stringLiteralModule->block->insertAtTail(bytesLitDef);

  s->immediate = new Immediate;
  *s->immediate = imm;
  bytesLiteralsHash.put(s->immediate, s);

  // String literal init function should be not created yet.
  // Otherwise, the new bytes global will not be initialized.
  INT_ASSERT(initStringLiterals == NULL);

  return s;
}

// Just a convenience function
VarSymbol *new_StringOrBytesSymbol(const char *str, AggregateType *t) {
  if (t == dtString) {
    return new_StringSymbol(str);
  }
  else if (t == dtBytes) {
    return new_BytesSymbol(str);
  }
  else {
    INT_FATAL("new_StringOrBytesSymbol accepts dtString and dtBytes only");
    return NULL;
  }
}

VarSymbol *new_CStringSymbol(const char *str) {
  size_t len = strlen(str);
  Immediate imm(gContext, str, len, STRING_KIND_C_STRING);
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

  case BOOL_SIZE_SYS:
  case BOOL_SIZE_8  :
  case BOOL_SIZE_16 :
  case BOOL_SIZE_32 :
  case BOOL_SIZE_64 :
    break;
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

static VarSymbol* new_FloatSymbol(const char* num,
                                  IF1_float_type size, IF1_num_kind kind,
                                  Type* type) {
  Immediate imm;
  int len = strlen(num);
  const char* normalized = NULL;
  char* n = (char*)malloc(len+1);

  /* Remove '_' separators from the number */
  int j = 0;
  for (int i=0; i<len; i++) {
    if (num[i] != '_') {
      n[j] = num[i];
      j++;
    }
  }
  n[j] = '\0';

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
  free(n);
  return s;
}

VarSymbol *new_RealSymbol(const char *n, IF1_float_type size) {
  return new_FloatSymbol(n, size, NUM_KIND_REAL, dtReal[size]);
}

VarSymbol *new_RealSymbol(float val) {
  Immediate imm;
  imm.v_float32 = val;
  imm.const_kind = NUM_KIND_REAL;
  imm.num_index = FLOAT_SIZE_32;
  return new_ImmediateSymbol(&imm);
}

VarSymbol *new_RealSymbol(double val) {
  Immediate imm;
  imm.v_float64 = val;
  imm.const_kind = NUM_KIND_REAL;
  imm.num_index = FLOAT_SIZE_64;
  return new_ImmediateSymbol(&imm);
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
      } else if (imm->string_kind == STRING_KIND_BYTES) {
        return dtBytes;
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

VarSymbol* new_ImmediateSymbol(Immediate *imm) {
  VarSymbol* s = uniqueConstantsHash.get(imm);

  if (s)
    return s;

  Type* t = immediate_type(imm);

  s = new VarSymbol(astr("_literal_", istr(literal_id++)), t);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  const size_t bufSize = 512;
  char str[bufSize];
  const char* ss = str;
  if (imm->const_kind == CONST_KIND_STRING) {
    ss = astr(imm->v_string.c_str());
  } else {
    snprint_imm(str, bufSize, *imm);
    ss = astr(ss);
  }
  s->cname = ss;
  *s->immediate = *imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

Immediate* getSymbolImmediate(Symbol* sym) {
  Immediate* imm = NULL;

  if (VarSymbol* var = toVarSymbol(sym)) {
    imm = var->immediate;
  }

  if (EnumSymbol* enumsym = toEnumSymbol(sym)) {
    imm = enumsym->getImmediate();
  }

  return imm;
}


// Return the expression PRIM_MOVE-ed into origSE->symbol().
// Return NULL if the def is not found or is uncertain.
Expr* getDefOfTemp(SymExpr* origSE)
{
  Symbol* origSym = origSE->symbol();
  if (!origSym->hasFlag(FLAG_TEMP)) return NULL;  // only temps

  SymExpr* otherSE = origSym->getSingleDef();


  if (otherSE == NULL) {
    // Sometimes the DefExpr for 'origSym' is hoisted to the module level -
    // see static 'globalTemps' in normalize.cpp. Then, 'origSym' does not get
    // cloned while instantiating the enclosing function, so we get two defs.
    // We have to run getSingleDef() first because origSE may be not in tree.
    // Tests:
    //   arrays/deitz/runtime_types/test_array_type4.chpl
    //   studies/kmeans/kmeans-blc.chpl
    otherSE = origSym->getSingleDefUnder(origSE->parentSymbol);
  }

  if (CallExpr* def = toCallExpr(otherSE->parentExpr))
    if (def->isPrimitive(PRIM_MOVE))
      if (otherSE == def->get(1))
        return def->get(2);

  // uncertain situation
  return NULL;
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

const char* astrSassign = NULL;
const char* astrSdot = NULL;
const char* astrSeq = NULL;
const char* astrSne = NULL;
const char* astrSgt = NULL;
const char* astrSgte = NULL;
const char* astrSlt = NULL;
const char* astrSlte = NULL;
const char* astrSswap = NULL;
const char* astrScolon = NULL;
const char* astr_defaultOf = NULL;
const char* astrInit = NULL;
const char* astrInitEquals = NULL;
const char* astrNew = NULL;
const char* astrDeinit = NULL;
const char* astrPostinit = NULL;
const char* astrTag = NULL;
const char* astrThis = NULL;
const char* astrSuper = NULL;
const char* astr_chpl_cname = NULL;
const char* astr_chpl_forward_tgt = NULL;
const char* astr_chpl_manager = NULL;
const char* astr_chpl_statementLevelSymbol = NULL;
const char* astr_chpl_waitDynamicEndCount = NULL;
const char* astr_forallexpr = NULL;
const char* astr_forexpr = NULL;
const char* astr_loopexpr_iter = NULL;
const char* astrPostfixBang = NULL;
const char* astrBorrow = NULL;
const char* astr_init_coerce_tmp = NULL;
const char* astr_autoCopy = NULL;
const char* astr_initCopy = NULL;
const char* astr_coerceCopy = NULL;
const char* astr_coerceMove = NULL;
const char* astr_autoDestroy = NULL;

void initAstrConsts() {
  astrSassign = astr("=");
  astrSdot    = astr(".");
  astrSeq = astr("==");
  astrSne = astr("!=");
  astrSgt = astr(">");
  astrSgte = astr(">=");
  astrSlt = astr("<");
  astrSlte = astr("<=");
  astrSswap = astr("<=>");
  astrScolon = astr(":");
  astr_defaultOf = astr("_defaultOf");
  astrInit    = astr("init");
  astrInitEquals = astr("init=");
  astrNew     = astr("_new");
  astrDeinit  = astr("deinit");
  astrPostinit  = astr("postinit");
  astrTag     = astr("tag");
  astrThis    = astr("this");
  astrSuper   = astr("super");
  astr_chpl_cname = astr("_chpl_cname");
  astr_chpl_forward_tgt = astr("_chpl_forward_tgt");
  astr_chpl_manager = astr("_chpl_manager");
  astr_chpl_statementLevelSymbol = astr("chpl_statementLevelSymbol");
  astr_chpl_waitDynamicEndCount = astr("chpl_waitDynamicEndCount");

  astr_forallexpr    = astr("chpl__forallexpr");
  astr_forexpr       = astr("chpl__forexpr");
  astr_loopexpr_iter = astr("chpl__loopexpr_iter");

  astrPostfixBang = astr("postfix!");

  astrBorrow = astr("borrow");
  astr_init_coerce_tmp = astr("init_coerce_tmp");

  astr_autoCopy = astr("chpl__autoCopy");
  astr_initCopy = astr("chpl__initCopy");
  astr_coerceCopy = astr("chpl__coerceCopy");
  astr_coerceMove = astr("chpl__coerceMove");

  astr_autoDestroy = astr("chpl__autoDestroy");
}

bool isAstrOpName(const char* name) {
  return chpl::uast::isOpName(UniqueString::get(gContext, name, strlen(name)));
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

const char* toString(ArgSymbol* arg, bool withTypeAndIntent) {
  const char* intent = "";
  if (withTypeAndIntent) {
    switch (arg->intent) {
      case INTENT_BLANK:           intent = "";           break;
      case INTENT_IN:              intent = "in ";        break;
      case INTENT_INOUT:           intent = "inout ";     break;
      case INTENT_OUT:             intent = "out ";       break;
      case INTENT_CONST:           intent = "const ";     break;
      case INTENT_CONST_IN:        intent = "const in ";  break;
      case INTENT_CONST_REF:       intent = "const ref "; break;
      case INTENT_REF_MAYBE_CONST: intent = "";           break;
      case INTENT_REF:             intent = "ref ";       break;
      case INTENT_PARAM:           intent = "param ";     break;
      case INTENT_TYPE:            intent = "type ";      break;
    }
  }

  const char* retval = "";
  if (arg->getValType() == dtAny ||
      arg->getValType() == dtUnknown ||
      withTypeAndIntent == false)
    retval = astr(intent, arg->name);
  else
    retval = astr(intent, arg->name, ": ", toString(arg->getValType()));

  if (developer  == true) {
    retval = astr(retval, " [", istr(arg->id), "]");
  }

  return retval;
}

const char* toString(VarSymbol* var, bool withType) {

  Immediate* imm = getSymbolImmediate(var);
  if (imm) {
    Type* t = var->getValType();
    if (imm->const_kind == NUM_KIND_BOOL) {
      return astr(imm->bool_value() ? "true" : "false");
    } else if (imm->const_kind == CONST_KIND_STRING) {
      std::string value;
      value = "";
      if (t == dtBytes)
        value += "b";
      value += '"';
      value += imm->string_value();
      value += '"';
      return astr(value.c_str());
    } else {
      std::string value;
      const size_t bufSize = 128;
      char buf[bufSize];
      snprint_imm(buf, bufSize, *imm);
      value = buf;
      // Add the type if it's not default
      if (t != dtUnknown && t != dtString && t != dtBytes) {
        if (withType && isNumericParamDefaultType(t) == false) {
          value += ": ";
          value += toString(t);
        }
      }
      return astr(value.c_str());
    }
  }

  // If it's a compiler temporary, find an assignment
  //  * from a user variable or field
  //  * to a user variable or field

  if (var->hasFlag(FLAG_USER_VARIABLE_NAME) || !var->hasFlag(FLAG_TEMP)) {
    if (withType)
      return astr(var->name, ": ", toString(var->getValType()));
    else
      return var->name;
  }

  Symbol* sym = var;
  // Compiler temporaries should have a single definition
  while (sym->hasFlag(FLAG_TEMP) && !sym->hasFlag(FLAG_USER_VARIABLE_NAME)) {
    SymExpr* singleDef = sym->getSingleDef();
    if (singleDef != NULL) {
      if (CallExpr* c = toCallExpr(singleDef->parentExpr)) {
        if (c->isPrimitive(PRIM_MOVE) ||
            c->isPrimitive(PRIM_ASSIGN)) {
          SymExpr* dstSe = toSymExpr(c->get(1));
          SymExpr* srcSe = toSymExpr(c->get(2));
          if (dstSe && srcSe && dstSe->symbol() == sym) {
            sym = srcSe->symbol();
            continue;
          }
        }
      }
    }

    // Give up
    sym = NULL;
    break;
  }

  const char* name = NULL;
  if (sym != NULL) {
    name = sym->name;
  } else {
    // Look for something using the temporary
    // e.g. field initialization

    sym = var;
    while (sym->hasFlag(FLAG_TEMP) && !sym->hasFlag(FLAG_USER_VARIABLE_NAME)) {
      Expr* cur = NULL;
      name = NULL;
      for (cur = sym->defPoint; cur; cur = cur->next) {
        if (CallExpr* c = toCallExpr(cur)) {
          if (c->isPrimitive(PRIM_MOVE) ||
              c->isPrimitive(PRIM_ASSIGN)) {
            SymExpr* dstSe = toSymExpr(c->get(1));
            SymExpr* srcSe = toSymExpr(c->get(2));
            if (dstSe && srcSe && srcSe->symbol() == sym) {
              sym = dstSe->symbol();
              name = sym->name;
              break;
            }
          }
          if (c->isPrimitive(PRIM_SET_MEMBER) ||
              c->isPrimitive(PRIM_SET_SVEC_MEMBER)) {
            SymExpr* fieldSe = toSymExpr(c->get(2));
            SymExpr* valueSe = toSymExpr(c->get(3));
            if (fieldSe && valueSe && valueSe->symbol() == sym) {
              sym = fieldSe->symbol();
              name = NULL;
              // Field access might be by name
              if (VarSymbol* v = toVarSymbol(sym))
                if (v->immediate)
                  if (v->immediate->const_kind == CONST_KIND_STRING)
                    name = astr("field ", v->immediate->v_string.c_str());

              if (name == NULL)
                name = astr("field ", sym->name);

              break;
            }
          }
        }
      }
      // Stop looking if the above code didn't find anything
      if (name == NULL)
        break;
    }
  }

  if (ArgSymbol* arg = toArgSymbol(sym)) {
    return toString(arg, withType);
  } else if (name != NULL) {
    if (withType)
      return astr(name, ": ", toString(var->getValType()));
    else
      return astr(name);
  } else {
    return astr("<temporary>");
  }
}
const char* toString(Symbol* sym, bool withType) {
  VarSymbol* var = toVarSymbol(sym);
  ArgSymbol* arg = toArgSymbol(sym);
  if (var != NULL)
    return toString(var, withType);
  if (arg != NULL)
    return toString(arg, withType);

  return sym->name;
}

struct SymbolPairComparator {
  bool operator()(SymbolMapKeyValue lhs, SymbolMapKeyValue rhs) {
    // use the same logic as other set/map ordering
    std::less<Symbol*> lessSym;

    return lessSym(lhs.key, rhs.key);
  }
};

SymbolMapVector sortedSymbolMapElts(const SymbolMap& map) {
  SymbolMapVector elts;
  form_Map(SymbolMapElem, e, map) {
    elts.push_back(SymbolMapKeyValue(e->key, e->value));
  }
  std::sort(elts.begin(), elts.end(), SymbolPairComparator());
  return elts;
}
