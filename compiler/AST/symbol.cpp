#include "astutil.h"
#include "bb.h"
#include "build.h"
#include "files.h"
#include "iterator.h"
#include "misc.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "passes.h"


FnSymbol *chpl_main = NULL;

ModuleSymbol* rootModule = NULL;
ModuleSymbol* theProgram = NULL;
ModuleSymbol* baseModule = NULL;
ModuleSymbol* standardModule = NULL;
Symbol *gNil = NULL;
Symbol *gNilRef = NULL;
Symbol *gUnknown = NULL;
Symbol *gUnspecified = NULL;
Symbol *gMethodToken = NULL;
Symbol *gLeaderToken = NULL;
Symbol *gLeaderTag = NULL, *gFollowerTag = NULL;
Symbol *gModuleToken = NULL;
Symbol *gVoid = NULL;
Symbol *gFile = NULL;
Symbol *gOpaque = NULL;
Symbol *gTimer = NULL;
Symbol *gSyncVarAuxFields = NULL;
Symbol *gSingleVarAuxFields = NULL;
Symbol *gTaskList = NULL;

VarSymbol *gTrue = NULL;
VarSymbol *gFalse = NULL;
VarSymbol *gBoundsChecking = NULL;


Symbol::Symbol(AstTag astTag, const char* init_name, Type* init_type) :
  BaseAST(astTag),
  name(astr(init_name)),
  cname(name),
  type(init_type),
  defPoint(NULL),
  flags()
{}


Symbol::~Symbol() {
}


void Symbol::verify() {
  BaseAST::verify();
  if (defPoint && !defPoint->parentSymbol && !toModuleSymbol(this))
    INT_FATAL(this, "Symbol::defPoint is not in AST");
  if (defPoint && this != defPoint->sym)
    INT_FATAL(this, "Symbol::defPoint != Sym::defPoint->sym");
}


bool Symbol::inTree(void) {
  if (this == rootModule)
    return true;
  if (defPoint)
    return defPoint->inTree();
  else
    return false;
}


Type* Symbol::typeInfo(void) {
  return type;
}


Symbol*
Symbol::copyInner(SymbolMap* map) {
  INT_FATAL(this, "Illegal call to Symbol::copy");
  return NULL;
}


void Symbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected call to Symbol::replaceChild");
}


bool Symbol::isConstant(void) {
  return false;
}


bool Symbol::isParameter(){
  return false;
}


void Symbol::codegen(FILE* outfile) {
  fprintf(outfile, "%s", cname);
}


void Symbol::codegenDef(FILE* outfile) {
  INT_FATAL(this, "Unanticipated call to Symbol::codegenDef");
}


void Symbol::codegenPrototype(FILE* outfile) { }


FnSymbol* Symbol::getFnSymbol(void) {
  return NULL;
}


bool Symbol::hasFlag(Flag flag) {
  return flags[flag];
}


void Symbol::addFlag(Flag flag) {
  flags.set(flag);
}


void Symbol::addFlags(Vec<const char*>* strs) {
  forv_Vec(const char, str, *strs) {
    flags.set(str2flag(str));
  }
}


void Symbol::copyFlags(Symbol* other) {
  flags |= other->flags;
}


void Symbol::removeFlag(Flag flag) {
  flags.reset(flag);
}


bool Symbol::isImmediate() {
  return false;
}


VarSymbol::VarSymbol(const char *init_name,
                     Type    *init_type) :
  Symbol(SYMBOL_VAR, init_name, init_type),
  immediate(NULL)
{ }


VarSymbol::~VarSymbol() {
  if (immediate)
    delete immediate;
}


void VarSymbol::verify() {
  Symbol::verify();
  if (astTag != SYMBOL_VAR) {
    INT_FATAL(this, "Bad VarSymbol::astTag");
  }
}


VarSymbol*
VarSymbol::copyInner(SymbolMap* map) {
  VarSymbol* newVarSymbol = new VarSymbol(name, type);
  newVarSymbol->cname = cname;
  INT_ASSERT(!newVarSymbol->immediate);
  return newVarSymbol;
}


void VarSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  type->replaceChild(old_ast, new_ast);
}


bool VarSymbol::isConstant(void) {
  return hasFlag(FLAG_CONST);
}


bool VarSymbol::isParameter(void){
  return hasFlag(FLAG_PARAM) || immediate;
}


void VarSymbol::codegen(FILE* outfile) {
  if (immediate && immediate->const_kind == CONST_KIND_STRING) {
    fprintf(outfile, "\"%s\"", immediate->v_string);
  } else if (immediate &&
             immediate->const_kind == NUM_KIND_UINT &&
             immediate->num_index == INT_SIZE_1) {
    fprintf(outfile, immediate->uint_value() ? "true" : "false");
  } else if (immediate &&
             immediate->const_kind == NUM_KIND_INT) {
    int64 iconst = immediate->int_value();
    if (iconst == (1ll<<63)) {
      fprintf(outfile, "-INT64(9223372036854775807) - INT64(1)");
    } else if (iconst <= -2147483648ll || iconst >= 2147483647ll) {
      fprintf(outfile, "INT64(%lld)", iconst);
    } else {
      fprintf(outfile, "%lld", iconst);
    }
  } else if (immediate &&
             immediate->const_kind == NUM_KIND_UINT) {
    uint64 uconst = immediate->uint_value();
    fprintf(outfile, "UINT64(%llu)", uconst);
  } else {
    fprintf(outfile, "%s", cname);
  }
}


static void zeroInitializeRecord(FILE* outfile, ClassType* ct) {
  bool first = true;
  fprintf(outfile, "{");
  for_fields(field, ct) {
    if (!first) {
      fprintf(outfile, ", ");
    }
    if (ClassType* ct = toClassType(field->type)) {
      if (ct->classTag == CLASS_RECORD) {
        zeroInitializeRecord(outfile, ct);
      } else if (ct->classTag == CLASS_UNION) {
        fprintf(outfile, "{0, 0}");
      } else {
        fprintf(outfile, "NULL");
      }
    } else {
      fprintf(outfile, "0");
    }
    first = false;
  }
  fprintf(outfile, "}");
}


void VarSymbol::codegenDef(FILE* outfile) {
  if (type == dtVoid)
    return;
  if (this->hasFlag(FLAG_SUPER_CLASS))
    fprintf(outfile, "_");
  type->codegen(outfile);
  fprintf(outfile, " ");
  fprintf(outfile, "%s", cname);
  if (ClassType* ct = toClassType(type)) 
    if (ct->classTag == CLASS_CLASS) {
      if (isFnSymbol(defPoint->parentSymbol))
        fprintf(outfile, " = NULL");
    } else if (ct->symbol->hasFlag(FLAG_WIDE) ||
               ct->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      if (isFnSymbol(defPoint->parentSymbol))
        fprintf(outfile, " = {0,NULL}");
    } else if (fCopyCollect && ct->classTag == CLASS_RECORD) {
      /* If garbage collection is on, zero-initialize records */
      if (isFnSymbol(defPoint->parentSymbol)) {
        fprintf(outfile, " = ");
        zeroInitializeRecord(outfile, ct);
      }
    }
  fprintf(outfile, ";\n");
}


bool VarSymbol::isImmediate() {
  return immediate != NULL;
}


ArgSymbol::ArgSymbol(IntentTag iIntent, const char* iName, 
                     Type* iType, Expr* iTypeExpr,
                     Expr* iDefaultExpr, Expr* iVariableExpr) :
  Symbol(SYMBOL_ARG, iName, iType),
  intent(iIntent),
  typeExpr(NULL),
  defaultExpr(NULL),
  variableExpr(NULL),
  instantiatedFrom(NULL),
  instantiatedParam(false),
  markedGeneric(false)
{
  if (!iTypeExpr)
    typeExpr = NULL;
  else if (BlockStmt* block = toBlockStmt(iTypeExpr))
    typeExpr = block;
  else
    typeExpr = new BlockStmt(iTypeExpr, BLOCK_SCOPELESS);
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
}


void ArgSymbol::verify() {
  Symbol::verify();
  if (astTag != SYMBOL_ARG) {
    INT_FATAL(this, "Bad ArgSymbol::astTag");
  }
  if (typeExpr && typeExpr->parentSymbol != this)
    INT_FATAL(this, "Bad ArgSymbol::typeExpr::parentSymbol");
  if (defaultExpr && defaultExpr->parentSymbol != this)
    INT_FATAL(this, "Bad ArgSymbol::defaultExpr::parentSymbol");
  if (variableExpr && variableExpr->parentSymbol != this)
    INT_FATAL(this, "Bad ArgSymbol::variableExpr::parentSymbol");
}


ArgSymbol*
ArgSymbol::copyInner(SymbolMap* map) {
  ArgSymbol *ps = new ArgSymbol(intent, name, type, COPY_INT(typeExpr),
                                COPY_INT(defaultExpr), COPY_INT(variableExpr));
  ps->cname = cname;
  ps->instantiatedFrom = instantiatedFrom;
  ps->instantiatedParam = instantiatedParam;
  ps->markedGeneric = markedGeneric;
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
    type->replaceChild(old_ast, new_ast);
}


bool ArgSymbol::requiresCPtr(void) {
  if (intent == INTENT_REF ||
      (!strcmp(name, "this") && is_complex_type(type)))
    return true;
  ClassType* ct = toClassType(type);
  if (ct && ct->classTag != CLASS_CLASS)
    return true;
  return false;
}


bool ArgSymbol::isConstant(void) {
  return (intent == INTENT_BLANK || intent == INTENT_CONST) &&
    !isReferenceType(type) &&
    !type->symbol->hasFlag(FLAG_ARRAY) &&
    !type->symbol->hasFlag(FLAG_DOMAIN);
}


void ArgSymbol::codegen(FILE* outfile) {
  if (requiresCPtr())
    fprintf(outfile, "(*");
  fprintf(outfile, "%s", cname);
  if (requiresCPtr())
    fprintf(outfile, ")");
}


void ArgSymbol::codegenDef(FILE* outfile) {
  type->codegen(outfile);
  if (requiresCPtr())
    fprintf(outfile, "* const");
  fprintf(outfile, " ");
  fprintf(outfile, "%s", cname);
}


TypeSymbol::TypeSymbol(const char* init_name, Type* init_type) :
  Symbol(SYMBOL_TYPE, init_name, init_type)
{
  addFlag(FLAG_TYPE_VARIABLE);
  if (!type)
    INT_FATAL(this, "TypeSymbol constructor called without type");
  type->addSymbol(this);
  gTypes.add(this);
}


void TypeSymbol::verify() {
  Symbol::verify();
  if (astTag != SYMBOL_TYPE) {
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
  new_type_symbol->cname = cname;
  return new_type_symbol;
}


void TypeSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  type->replaceChild(old_ast, new_ast);
}


void TypeSymbol::codegenPrototype(FILE* outfile) {
  type->codegenPrototype(outfile);
}


void TypeSymbol::codegenDef(FILE* outfile) {
  type->codegenDef(outfile);
}


FnSymbol::FnSymbol(const char* initName) :
  Symbol(SYMBOL_FN, initName),
  formals(),
  setter(NULL),
  retType(dtUnknown),
  where(NULL),
  retExprType(NULL),
  body(new BlockStmt()),
  retTag(RET_VALUE),
  iteratorInfo(NULL),
  _this(NULL),
  _outer(NULL),
  instantiatedFrom(NULL),
  instantiationPoint(NULL),
  basicBlocks(NULL),
  calledBy(NULL),
  userString(NULL),
  valueFunction(NULL)
{
  substitutions.clear();
  gFns.add(this);
  formals.parent = this;
}


FnSymbol::~FnSymbol() {
  if (iteratorInfo)
    delete iteratorInfo;
  if (basicBlocks) {
    forv_Vec(BasicBlock, bb, *basicBlocks) {
      delete bb;
    }
    delete basicBlocks;
  }
  if (calledBy)
    delete calledBy;
}


void FnSymbol::verify() {
  Symbol::verify();
  if (astTag != SYMBOL_FN) {
    INT_FATAL(this, "Bad FnSymbol::astTag");
  }
  if (normalized && !hasFlag(FLAG_AUTO_II)) {
    CallExpr* last = toCallExpr(body->body.last());
    if (!last || !last->isPrimitive(PRIMITIVE_RETURN))
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
}


FnSymbol* FnSymbol::getFnSymbol(void) {
  return this;
}


FnSymbol*
FnSymbol::copyInner(SymbolMap* map) {
  FnSymbol* copy = new FnSymbol(name);
  for_formals(formal, this) {
    copy->insertFormalAtTail(COPY_INT(formal->defPoint));
  }
  copy->setter = COPY_INT(setter);
  copy->retType = retType;
  copy->where = COPY_INT(where);
  copy->body = COPY_INT(body);
  copy->retTag = retTag;
  copy->retExprType = COPY_INT(retExprType);
  copy->cname = cname;
  copy->_this = _this;
  copy->_outer = _outer;
  copy->instantiatedFrom = instantiatedFrom;
  copy->instantiationPoint = instantiationPoint;
  return copy;
}


void FnSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == body) {
    body = toBlockStmt(new_ast);
  } else if (old_ast == where) {
    where = toBlockStmt(new_ast);
  } else if (old_ast == setter) {
    setter = toDefExpr(new_ast);
  } else if (old_ast == retExprType) {
    retExprType = toBlockStmt(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in FnSymbol::replaceChild");
  }
}


void FnSymbol::codegenHeader(FILE* outfile) {
  if (fGenIDS)
    fprintf(outfile, "/* %7d */ ", id);
  retType->codegen(outfile);
  fprintf(outfile, " ");
  fprintf(outfile, "%s", cname);
  fprintf(outfile, "(");
  if (numFormals() == 0) {
    fprintf(outfile, "void");
  } else {
    bool first = true;
    for_formals(formal, this) {
      if (formal->defPoint == formals.head && hasFlag(FLAG_ON_BLOCK))
        continue; // do not print locale argument for on blocks
      if (!first) {
        fprintf(outfile, ", ");
      }
      formal->codegenDef(outfile);
      first = false;
    }
  }
  fprintf(outfile, ")");
}


void FnSymbol::codegenPrototype(FILE* outfile) {
  INT_ASSERT(!hasFlag(FLAG_EXTERN));
  codegenHeader(outfile);
  fprintf(outfile, ";\n");
}


static void
codegenNullAssignments(FILE* outfile, const char* cname, ClassType* ct) {
  if (ct->classTag == CLASS_CLASS)
    fprintf(outfile, "%s = NULL;\n", cname);
  else {
    for_fields(field, ct) {
      if (ClassType* fct = toClassType(field->type)) {
        char buffer[1024];
        strcpy(buffer, cname);
        strcat(buffer, ".");
        strcat(buffer, field->cname);
        codegenNullAssignments(outfile, buffer, fct);
      }
    }
  }
}


void FnSymbol::codegenDef(FILE* outfile) {
  if (strcmp(saveCDir, "") && getModule()->filename) {
    const char* name = strrchr(getModule()->filename, '/');
    name = (!name) ? getModule()->filename : name + 1;
    fprintf(outfile, "/* %s:%d */\n", name, lineno);
  }

  codegenHeader(outfile);

  fprintf(outfile, " {\n");
  Vec<BaseAST*> asts;
  collect_top_asts(body, asts);

  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast))
      if (!toTypeSymbol(def->sym)) {
        if (fGenIDS)
          fprintf(outfile, "/* %7d */ ", def->sym->id);
        def->sym->codegenDef(outfile);
      }
  }
  body->codegen(outfile);
  fprintf(outfile, "}\n\n");
}


void
FnSymbol::insertAtHead(Expr* ast) {
  body->insertAtHead(ast);
}


void
FnSymbol::insertAtTail(Expr* ast) {
  body->insertAtTail(ast);
}


Symbol*
FnSymbol::getReturnSymbol() {
  CallExpr* ret = toCallExpr(body->body.last());
  if (!ret || !ret->isPrimitive(PRIMITIVE_RETURN))
    INT_FATAL(this, "function is not normal");
  SymExpr* sym = toSymExpr(ret->get(1));
  if (!sym)
    INT_FATAL(this, "function is not normal");
  return sym->var;
}


void
FnSymbol::insertBeforeReturn(Expr* ast) {
  CallExpr* ret = toCallExpr(body->body.last());
  if (!ret || !ret->isPrimitive(PRIMITIVE_RETURN))
    INT_FATAL(this, "function is not normal");
  Expr* last = ret;
  if (DefExpr* def = toDefExpr(last->prev))
    if (toLabelSymbol(def->sym))
      last = last->prev; // label before return
  last->insertBefore(ast);
}


void
FnSymbol::insertBeforeReturnAfterLabel(Expr* ast) {
  CallExpr* ret = toCallExpr(body->body.last());
  if (!ret || !ret->isPrimitive(PRIMITIVE_RETURN))
    INT_FATAL(this, "function is not normal");
  ret->insertBefore(ast);
}


void
FnSymbol::insertFormalAtHead(BaseAST* ast) {
  if (ArgSymbol* arg = toArgSymbol(ast))
    formals.insertAtHead(new DefExpr(arg));
  else if (DefExpr* def = toDefExpr(ast))
    formals.insertAtHead(def);
  else
    INT_FATAL(ast, "Bad argument to FnSymbol::insertFormalAtHead");
}


void
FnSymbol::insertFormalAtTail(BaseAST* ast) {
  if (ArgSymbol* arg = toArgSymbol(ast))
    formals.insertAtTail(new DefExpr(arg));
  else if (DefExpr* def = toDefExpr(ast))
    formals.insertAtTail(def);
  else
    INT_FATAL(ast, "Bad argument to FnSymbol::insertFormalAtTail");
}


int
FnSymbol::numFormals() {
  return formals.length();
}


ArgSymbol*
FnSymbol::getFormal(int i) {
  return toArgSymbol(toDefExpr(formals.get(i))->sym);
}


//
// returns 1 if generic
// returns 2 if they all have defaults
//
static int
hasGenericArgs(FnSymbol* fn) {
  bool isGeneric = false;
  bool hasGenericDefaults = true;
  for_formals(formal, fn) {
    if ((formal->type->symbol->hasFlag(FLAG_GENERIC) &&
         (!formal->type->hasGenericDefaults ||
          formal->markedGeneric ||
          formal == fn->_this ||
          formal->hasFlag(FLAG_IS_MEME))) ||
        formal->intent == INTENT_PARAM) {
      isGeneric = true;
      if (!formal->defaultExpr)
        hasGenericDefaults = false;
    }
  }
  if (isGeneric && !hasGenericDefaults)
    return 1;
  else if (isGeneric && hasGenericDefaults)
    return 2;
  else
    return 0;
}


bool FnSymbol::tag_generic() {
  if (hasFlag(FLAG_GENERIC))
    return false;
  if (int result = hasGenericArgs(this)) {
    addFlag(FLAG_GENERIC);
    if (retType != dtUnknown && hasFlag(FLAG_TYPE_CONSTRUCTOR)) {
      retType->symbol->addFlag(FLAG_GENERIC);
      if (result == 2)
        retType->hasGenericDefaults = true;
    }
    return true;
  }
  return false;
}



EnumSymbol::EnumSymbol(const char* init_name) :
  Symbol(SYMBOL_ENUM, init_name)
{ }


void EnumSymbol::verify() {
  Symbol::verify();
  if (astTag != SYMBOL_ENUM) {
    INT_FATAL(this, "Bad EnumSymbol::astTag");
  }
}


EnumSymbol*
EnumSymbol::copyInner(SymbolMap* map) {
  return new EnumSymbol(name);
}

bool EnumSymbol::isParameter(void) { return true; }

void EnumSymbol::codegenDef(FILE* outfile) { }


ModuleSymbol::ModuleSymbol(const char* iName, ModTag iModTag, BlockStmt* iBlock) :
  Symbol(SYMBOL_MODULE, iName),
  modTag(iModTag),
  block(iBlock),
  initFn(NULL),
  guard(NULL),
  filename(NULL)
{
  block->parentSymbol = this;
  registerModule(this);
}


ModuleSymbol::~ModuleSymbol() { }


void ModuleSymbol::verify() {
  Symbol::verify();
  if (astTag != SYMBOL_MODULE) {
    INT_FATAL(this, "Bad ModuleSymbol::astTag");
  }
  if (block && block->parentSymbol != this)
    INT_FATAL(this, "Bad ModuleSymbol::block::parentSymbol");
}


ModuleSymbol*
ModuleSymbol::copyInner(SymbolMap* map) {
  INT_FATAL(this, "Illegal call to ModuleSymbol::copy");
  return NULL;
}


static int compareLineno(const void* v1, const void* v2) {
  FnSymbol* fn1 = *(FnSymbol**)v1;
  FnSymbol* fn2 = *(FnSymbol**)v2;
  if (fn1->lineno > fn2->lineno)
    return 1;
  else if (fn1->lineno < fn2->lineno)
    return -1;
  else
    return 0;
}


void ModuleSymbol::codegenDef(FILE* outfile) {
  Vec<FnSymbol*> fns;
  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr))
      if (FnSymbol* fn = toFnSymbol(def->sym))
        if (!fn->hasFlag(FLAG_EXTERN))
          fns.add(fn);
  }
  qsort(fns.v, fns.n, sizeof(fns.v[0]), compareLineno);
  forv_Vec(FnSymbol, fn, fns) {
    fn->codegenDef(outfile);
  }
}


void ModuleSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == block) {
    block = toBlockStmt(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in ModuleSymbol::replaceChild");
  }
}


LabelSymbol::LabelSymbol(const char* init_name) :
  Symbol(SYMBOL_LABEL, init_name, NULL)
{ 
 
}


void LabelSymbol::verify() {
  Symbol::verify();
  if (astTag != SYMBOL_LABEL) {
    INT_FATAL(this, "Bad LabelSymbol::astTag");
  }
}

LabelSymbol* 
LabelSymbol::copyInner(SymbolMap* map) {
  LabelSymbol* copy = new LabelSymbol(name);
  copy->cname = cname;
  return copy;
}

void LabelSymbol::codegenDef(FILE* outfile) { }

static int literal_id = 1;
HashMap<Immediate *, ImmHashFns, VarSymbol *> uniqueConstantsHash;


VarSymbol *new_StringSymbol(const char *str) {
  Immediate imm;
  imm.const_kind = CONST_KIND_STRING;
  imm.v_string = astr(str);
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtString);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}


VarSymbol* new_BoolSymbol(bool b, IF1_bool_type size) {
  Immediate imm;
  switch (size) {
  case BOOL_SIZE_8  : imm.v_bool = b; break;
  case BOOL_SIZE_16 : imm.v_bool = b; break;
  case BOOL_SIZE_32 : imm.v_bool = b; break;
  case BOOL_SIZE_64 : imm.v_bool = b; break;
    // case BOOL_SIZE_128: imm.v_bool = b; break;
  default:
    INT_FATAL( "unknown BOOL_SIZE");
  }
  imm.const_kind = NUM_KIND_UINT;
  imm.num_index = INT_SIZE_1;
  VarSymbol *s;
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtBools[size]);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  return s;
}

VarSymbol *new_IntSymbol(long long int b, IF1_int_type size) {
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
  if (s)
    return s;
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtInt[size]);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_UIntSymbol(unsigned long long int b, IF1_int_type size) {
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
  if (s)
    return s;
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtUInt[size]);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_RealSymbol(const char *n, long double b, IF1_float_type size) {
  Immediate imm;
  switch (size) {
  case FLOAT_SIZE_32  : imm.v_float32  = b; break;
  case FLOAT_SIZE_64  : imm.v_float64  = b; break;
  default:
    INT_FATAL( "unknown FLOAT_SIZE");
  }
  imm.const_kind = NUM_KIND_FLOAT;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtReal[size]);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->cname = astr(n);
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_ImagSymbol(const char *n, long double b, IF1_float_type size) {
  Immediate imm;
  switch (size) {
  case FLOAT_SIZE_32  : imm.v_float32  = b; break;
  case FLOAT_SIZE_64  : imm.v_float64  = b; break;
  default:
    INT_FATAL( "unknown FLOAT_SIZE");
  }
  imm.const_kind = NUM_KIND_IMAG;
  imm.num_index = size;
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  if (s)
    return s;
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtImag[size]);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->cname = astr(n);
  s->immediate = new Immediate;
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_ComplexSymbol(const char *n, long double r, long double i, IF1_complex_type size) {
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
  if (s)
    return s;
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtComplex[size]);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  s->cname = astr(n);
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

VarSymbol *new_ImmediateSymbol(Immediate *imm) {
  VarSymbol *s = uniqueConstantsHash.get(imm);
  if (s)
    return s;
  PrimitiveType *t = immediate_type(imm);
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), t);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  char str[512];
  const char* ss = str;
  if (imm->const_kind == CONST_KIND_STRING)
    ss = imm->v_string;
  else
    sprint_imm(str, *imm);
  s->cname = astr(ss);
  *s->immediate = *imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

PrimitiveType *
immediate_type(Immediate *imm) {
  switch (imm->const_kind) {
    case CONST_KIND_STRING: 
      return dtString;
    case NUM_KIND_UINT:
      return dtUInt[imm->num_index];
    case NUM_KIND_INT:
      return dtInt[imm->num_index];
    case NUM_KIND_FLOAT:
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


VarSymbol* newTemp(const char* name, Type* type) {
  VarSymbol* vs = new VarSymbol(name ? name : "_tmp", type);
  vs->addFlag(FLAG_TEMP);
  return vs;
}


VarSymbol* newTemp(Type* type) {
  VarSymbol* vs = new VarSymbol("_tmp", type);
  vs->addFlag(FLAG_TEMP);
  return vs;
}


int
compareSymbol(const void* v1, const void* v2) {
  Symbol* s1 = *(Symbol**)v1;
  Symbol* s2 = *(Symbol**)v2;
  ModuleSymbol* m1 = s1->getModule();
  ModuleSymbol* m2 = s2->getModule();
  if (m1 != m2) {
    if (m1->modTag < m2->modTag)
      return -1;
    if (m1->modTag > m2->modTag)
      return 1;
    return strcmp(m1->cname, m2->cname);
  }
  int result = strcmp(s1->type->symbol->cname, s2->type->symbol->cname);
  if (!result)
    result = strcmp(s1->cname, s2->cname);
  return result;
}
