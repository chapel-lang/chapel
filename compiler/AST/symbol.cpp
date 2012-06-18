#define __STDC_FORMAT_MACROS
#include <cstdlib>
#include <inttypes.h>
#include <stdint.h>
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

#include "intlimits.h"

//
// The function that represents the compiler-generated entry point
//
FnSymbol *chpl_gen_main = NULL;

ModuleSymbol* rootModule = NULL;
ModuleSymbol* theProgram = NULL;
ModuleSymbol* mainModule = NULL;
ModuleSymbol* baseModule = NULL;
ModuleSymbol* standardModule = NULL;
Symbol *gNil = NULL;
Symbol *gUnknown = NULL;
Symbol *gMethodToken = NULL;
Symbol *gTypeDefaultToken = NULL;
Symbol *gLeaderTag = NULL, *gFollowerTag = NULL;
Symbol *gModuleToken = NULL;
Symbol *gVoid = NULL;
Symbol *gFile = NULL;
Symbol *gOpaque = NULL;
Symbol *gTimer = NULL;
Symbol *gTaskID = NULL;
Symbol *gSyncVarAuxFields = NULL;
Symbol *gSingleVarAuxFields = NULL;
Symbol *gTaskList = NULL;

VarSymbol *gTrue = NULL;
VarSymbol *gFalse = NULL;
VarSymbol *gTryToken = NULL;
VarSymbol *gBoundsChecking = NULL;
VarSymbol* gPrivatization = NULL;
VarSymbol* gLocal = NULL;
Symbol *gCLine = NULL;
Symbol *gCFile = NULL;

Map<FnSymbol*,int> ftableMap;
Vec<FnSymbol*> ftableVec;

Map<Type*,Vec<FnSymbol*>*> virtualMethodTable;
Map<FnSymbol*,int> virtualMethodMap;
Map<FnSymbol*,Vec<FnSymbol*>*> virtualChildrenMap;
Map<FnSymbol*,Vec<FnSymbol*>*> virtualRootsMap;


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
  CHECK_FLAG(flag);
  return flags[flag];
}


void Symbol::addFlag(Flag flag) {
  CHECK_FLAG(flag);
  flags.set(flag);
}


void Symbol::copyFlags(Symbol* other) {
  flags |= other->flags;
}


void Symbol::removeFlag(Flag flag) {
  CHECK_FLAG(flag);
  flags.reset(flag);
}


bool Symbol::isImmediate() {
  return false;
}


VarSymbol::VarSymbol(const char *init_name,
                     Type    *init_type) :
  Symbol(E_VarSymbol, init_name, init_type),
  immediate(NULL)
{
  gVarSymbols.add(this);
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
  newVarSymbol->cname = cname;
  INT_ASSERT(!newVarSymbol->immediate);
  return newVarSymbol;
}


void VarSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in VarSymbol::replaceChild");
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
    int64_t iconst = immediate->int_value();
    if (iconst == (1ll<<63)) {
      fprintf(outfile, "-INT64(9223372036854775807) - INT64(1)");
    } else if (iconst <= -2147483648ll || iconst >= 2147483647ll) {
      fprintf(outfile, "INT64(%"PRId64")", iconst);
    } else {
      fprintf(outfile, "%"PRId64, iconst);
    }
  } else if (immediate &&
             immediate->const_kind == NUM_KIND_UINT) {
    uint64_t uconst = immediate->uint_value();
    if( uconst <= (uint64_t) INT32_MAX ) {
      fprintf(outfile, "%"PRIu64, uconst);
    } else {
      fprintf(outfile, "UINT64(%"PRIu64")", uconst);
    }
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
  if (this->hasFlag(FLAG_EXTERN)) {
    return;
  }
  if (type == dtVoid)
    return;
  if (this->hasFlag(FLAG_SUPER_CLASS))
    fprintf(outfile, "_");
  type->codegen(outfile);
  fprintf(outfile, " ");
  fprintf(outfile, "%s", cname);
  if (ClassType* ct = toClassType(type)) {
    if (ct->classTag == CLASS_CLASS) {
      if (isFnSymbol(defPoint->parentSymbol)) {
        fprintf(outfile, " = NULL");
      }
    } else if (ct->symbol->hasFlag(FLAG_WIDE) ||
               ct->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      if (isFnSymbol(defPoint->parentSymbol))
        fprintf(outfile, " = {0,NULL}");
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
  Symbol(E_ArgSymbol, iName, iType),
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
  // If this one has a successor but the successor is not an argsymbol
  // the formal parameter list is corrupted.
  if (defPoint && defPoint->next && (!toDefExpr(defPoint->next)->sym ||
                                     !toArgSymbol(toDefExpr(defPoint->next)->sym)))
    INT_FATAL(this, "Bad ArgSymbol::defPoint->next");
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
    INT_FATAL(this, "Unexpected case in ArgSymbol::replaceChild");
}


bool ArgSymbol::requiresCPtr(void) {
  if (intent == INTENT_REF)
    return true;
  if (hasFlag(FLAG_ARG_THIS)) {
      if (is_complex_type(type))
        return true;
  }
  if (isRecord(type) || isUnion(type))
    return true;
  return false;
}


bool ArgSymbol::isConstant(void) {
  return (intent == INTENT_BLANK || intent == INTENT_CONST) &&
    !isReferenceType(type) &&
    !isRecordWrappedType(type) /* array, domain, distribution */;
}


bool ArgSymbol::isParameter(void) {
  return (intent == INTENT_PARAM);
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
  Symbol(E_TypeSymbol, init_name, init_type)
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
  new_type_symbol->cname = cname;
  return new_type_symbol;
}


void TypeSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in TypeSymbol::replaceChild");
}


void TypeSymbol::codegenPrototype(FILE* outfile) {
  if (!hasFlag(FLAG_EXTERN)) {
    type->codegenPrototype(outfile);
  }
}


void TypeSymbol::codegenDef(FILE* outfile) {
  if (!hasFlag(FLAG_EXTERN)) {
    type->codegenDef(outfile);
  }
}


FnSymbol::FnSymbol(const char* initName) :
  Symbol(E_FnSymbol, initName),
  formals(),
  setter(NULL),
  retType(dtUnknown),
  where(NULL),
  retExprType(NULL),
  body(new BlockStmt()),
  thisTag(INTENT_BLANK),
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
  gFnSymbols.add(this);
  formals.parent = this;
}


FnSymbol::~FnSymbol() {
  if (iteratorInfo)
    delete iteratorInfo;
  BasicBlock::clear(this);
  delete basicBlocks; basicBlocks = 0;
  if (calledBy)
    delete calledBy;
}


void FnSymbol::verify() {
  Symbol::verify();
  if (astTag != E_FnSymbol) {
    INT_FATAL(this, "Bad FnSymbol::astTag");
  }
  if (normalized && !hasFlag(FLAG_AUTO_II)) {
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
}


FnSymbol* FnSymbol::getFnSymbol(void) {
  return this;
}


FnSymbol*
FnSymbol::copyInner(SymbolMap* map) {
  FnSymbol* copy = new FnSymbol(name);
  if (hasFlag(FLAG_CONSTRUCTOR))
    copy->addFlag(FLAG_CONSTRUCTOR);
  for_formals(formal, this) {
    copy->insertFormalAtTail(COPY_INT(formal->defPoint));
  }
  copy->setter = COPY_INT(setter);
  copy->retType = retType;
  copy->where = COPY_INT(where);
  copy->body = COPY_INT(body);
  copy->thisTag = thisTag;
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
  // Prepend function header with necessary __global__ declaration
  if (hasFlag(FLAG_GPU_ON))
    fprintf(outfile, "__global__ ");
  retType->codegen(outfile);
  fprintf(outfile, " ");
  fprintf(outfile, "%s", cname);
  fprintf(outfile, "(");
  if (numFormals() == 0) {
    fprintf(outfile, "void");
  } else {
    int count = 0;
    bool first = true;
    for_formals(formal, this) {
      if (formal->defPoint == formals.head && hasFlag(FLAG_ON_BLOCK))
        continue; // do not print locale argument for on blocks
      if (hasFlag(FLAG_GPU_ON) && count < 2) {
        count++;
        continue; // do not print nBlocks and numThreadsPerBlock
      }
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
  if (isClass(ct))
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
  if (strcmp(saveCDir, "")) {
   if (const char* rawname = fname()) {
    const char* name = strrchr(rawname, '/');
    name = name ? name + 1 : rawname;
    fprintf(outfile, "/* %s:%d */\n", name, linenum());
   }
  }

  codegenHeader(outfile);

  fprintf(outfile, " {\n");
  if (fNoRepositionDefExpr) {
    Vec<BaseAST*> asts;
    collect_top_asts(body, asts);

    forv_Vec(BaseAST, ast, asts) {
      if (DefExpr* def = toDefExpr(ast))
        if (!toTypeSymbol(def->sym)) {
          if (fGenIDS && isVarSymbol(def->sym))
            fprintf(outfile, "/* %7d */ ", def->sym->id);
          def->sym->codegenDef(outfile);
        }
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
  CallExpr* ret = toCallExpr(body->body.last());
  if (!ret || !ret->isPrimitive(PRIM_RETURN))
    INT_FATAL(this, "function is not normal");
  SymExpr* sym = toSymExpr(ret->get(1));
  if (!sym)
    INT_FATAL(this, "function is not normal");
  return sym->var;
}


void
FnSymbol::insertBeforeReturn(Expr* ast) {
  CallExpr* ret = toCallExpr(body->body.last());
  if (!ret || !ret->isPrimitive(PRIM_RETURN))
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
  if (!ret || !ret->isPrimitive(PRIM_RETURN))
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
  return formals.length;
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


// Tag the given function as generic.
// Returns true if there was a change, false otherwise.
bool FnSymbol::tag_generic() {
  if (hasFlag(FLAG_GENERIC))
    return false;  // Already generic, no change.

  if (int result = hasGenericArgs(this)) {
    // This function has generic arguments, so mark it as generic.
    addFlag(FLAG_GENERIC);

    // If the return type is not completely unknown (which is generic enough)
    // and this function is a type constructor function,
    // then mark its return type as generic.
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
  Symbol(E_EnumSymbol, init_name)
{
  gEnumSymbols.add(this);
}


void EnumSymbol::verify() {
  Symbol::verify();
  if (astTag != E_EnumSymbol) {
    INT_FATAL(this, "Bad EnumSymbol::astTag");
  }
}


EnumSymbol*
EnumSymbol::copyInner(SymbolMap* map) {
  return new EnumSymbol(name);
}

void EnumSymbol::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in EnumSymbol::replaceChild");
}

bool EnumSymbol::isParameter(void) { return true; }

void EnumSymbol::codegenDef(FILE* outfile) { }


ModuleSymbol::ModuleSymbol(const char* iName, ModTag iModTag, BlockStmt* iBlock) :
  Symbol(E_ModuleSymbol, iName),
  modTag(iModTag),
  block(iBlock),
  initFn(NULL),
  filename(NULL)
{
  block->parentSymbol = this;
  registerModule(this);
  gModuleSymbols.add(this);
}


ModuleSymbol::~ModuleSymbol() { }


void ModuleSymbol::verify() {
  Symbol::verify();
  if (astTag != E_ModuleSymbol) {
    INT_FATAL(this, "Bad ModuleSymbol::astTag");
  }
  if (block && block->parentSymbol != this)
    INT_FATAL(this, "Bad ModuleSymbol::block::parentSymbol");
  if (initFn && !toFnSymbol(initFn))
    INT_FATAL(this, "Bad ModuleSymbol::initFn");
}


ModuleSymbol*
ModuleSymbol::copyInner(SymbolMap* map) {
  INT_FATAL(this, "Illegal call to ModuleSymbol::copy");
  return NULL;
}


static int compareLineno(const void* v1, const void* v2) {
  FnSymbol* fn1 = *(FnSymbol**)v1;
  FnSymbol* fn2 = *(FnSymbol**)v2;
  if (fn1->linenum() > fn2->linenum())
    return 1;
  else if (fn1->linenum() < fn2->linenum())
    return -1;
  else
    return 0;
}


void ModuleSymbol::codegenDef(FILE* outfile) {
  fileinfo gpufile;
  gpufile.fptr = NULL;
 
  Vec<FnSymbol*> fns;
  for_alist(expr, block->body) {
    if (DefExpr* def = toDefExpr(expr))
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        // Ignore external and prototype functions.
        if (fn->hasFlag(FLAG_EXTERN) || fn->hasFlag(FLAG_FUNCTION_PROTOTYPE))
          continue;
        fns.add(fn);
      }
  }
  qsort(fns.v, fns.n, sizeof(fns.v[0]), compareLineno);
  forv_Vec(FnSymbol, fn, fns) {
    // Create external file to be compiled by GPU compiler
    if (fn->hasFlag(FLAG_GPU_ON) || fn->hasFlag(FLAG_GPU_CALL)) {
      appendCFile(&gpufile,"chplGPU","cu");
      fn->codegenDef(gpufile.fptr);
      closeCFile(&gpufile); 
    }
    else
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
      INT_FATAL(this, "label's iterResumeGoto does not point back to the label");
  }
}

LabelSymbol* 
LabelSymbol::copyInner(SymbolMap* map) {
  LabelSymbol* copy = new LabelSymbol(name);
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

void LabelSymbol::codegenDef(FILE* outfile) { }

static int literal_id = 1;
HashMap<Immediate *, ImmHashFns, VarSymbol *> uniqueConstantsHash;

VarSymbol *new_StringSymbol(const char *str) {
  Immediate imm;
  imm.const_kind = CONST_KIND_STRING;
  imm.v_string = astr(str);
  VarSymbol *s = uniqueConstantsHash.get(&imm);
  PrimitiveType* dtRetType = dtString;
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
  PrimitiveType* dtRetType = dtReal[size];
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
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
  PrimitiveType* dtRetType = dtImag[size];
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
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
  PrimitiveType* dtRetType = dtComplex[size];
  if (s) {
    return s;
  }
  s = new VarSymbol(astr("_literal_", istr(literal_id++)), dtRetType);
  rootModule->block->insertAtTail(new DefExpr(s));
  s->immediate = new Immediate;
  s->cname = astr(n);
  *s->immediate = imm;
  uniqueConstantsHash.put(s->immediate, s);
  return s;
}

static PrimitiveType*
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

VarSymbol *new_ImmediateSymbol(Immediate *imm) {
  VarSymbol *s = uniqueConstantsHash.get(imm);
  if (s)
    return s;
  PrimitiveType *t = immediate_type(imm);
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

FlagSet getSyncFlags(Symbol* s) {
  static FlagSet mask;
  if (mask.none()) {
    mask.set(FLAG_SYNC);
    mask.set(FLAG_SINGLE);
  }

  return s->flags & mask;
}

VarSymbol* newTemp(const char* name, Type* type) {
  if (!name) {
    if (localTempNames)
      name = astr("_t", istr(tempID++), "_");
    else
      name = "_tmp";
  }
  VarSymbol* vs = new VarSymbol(name, type);
  vs->addFlag(FLAG_TEMP);
  return vs;
}


VarSymbol* newTemp(Type* type) {
  return newTemp((const char*)NULL, type);
}
