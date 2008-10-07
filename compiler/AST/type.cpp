#define TYPE_EXTERN
#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"


Type::Type(AstTag astTag, Symbol* init_defaultVal) :
  BaseAST(astTag),
  scalarPromotionType(NULL),
  symbol(NULL),
  defaultValue(init_defaultVal),
  defaultConstructor(NULL),
  defaultTypeConstructor(NULL),
  destructor(NULL),
  hasGenericDefaults(false),
  instantiatedFrom(NULL),
  refType(NULL)
{}


Type::~Type() { }


void Type::verify() {
  BaseAST::verify();
}


void Type::addSymbol(TypeSymbol* newsymbol) {
  symbol = newsymbol;
}


bool Type::inTree(void) {
  if (symbol)
    return symbol->inTree();
  else
    return false;
}


Type* Type::typeInfo(void) {
  return this;
}


Type*
Type::copyInner(SymbolMap* map) {
  INT_FATAL(this, "Illegal call to Type::copy");
  return NULL;
}


void Type::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = toSymbol(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


void Type::codegen(FILE* outfile) {
  if (this == dtUnknown) {
    INT_FATAL(this, "Cannot generate unknown type");
  }
  symbol->codegen(outfile);
}

void Type::codegenDef(FILE* outfile) {
  INT_FATAL(this, "Unexpected call to Type::codegenDef");
}


void Type::codegenPrototype(FILE* outfile) { }


Symbol* Type::getField(const char* name, bool fatal) {
  INT_FATAL(this, "getField not called on ClassType");
  return NULL;
}


Type* Type::getValueType() {
  if (symbol->hasFlag(FLAG_REF)) {
    if (this == dtNilRef)
      return dtNil;
    ClassType* ct = toClassType(this);
    INT_ASSERT(ct);
    return ct->getField("_val")->type;
  }
  if (symbol->hasFlag(FLAG_WIDE)) {
    ClassType* ct = toClassType(this);
    INT_ASSERT(ct);
    return ct->getField("addr")->type->getValueType();
  }
  return NULL;
}

Type* Type::getReferenceType() {
  if (!symbol->hasFlag(FLAG_REF)) {
    INT_ASSERT(refType);
    return refType;
  }
  return NULL;
}


PrimitiveType::PrimitiveType(Symbol *init) :
  Type(TYPE_PRIMITIVE, init)
{}


void PrimitiveType::verify() {
  Type::verify();
  if (astTag != TYPE_PRIMITIVE) {
    INT_FATAL(this, "Bad PrimitiveType::astTag");
  }
}


EnumType::EnumType() :
  Type(TYPE_ENUM, NULL),
  constants()
{
  constants.parent = this;
}


EnumType::~EnumType() { }


void EnumType::verify() {
  Type::verify();
  if (astTag != TYPE_ENUM) {
    INT_FATAL(this, "Bad EnumType::astTag");
  }
  if (constants.parent != this)
    INT_FATAL(this, "Bad AList::parent in EnumType");
  for_alist(expr, constants) {
    if (expr->parentSymbol != symbol)
      INT_FATAL(this, "Bad EnumType::constants::parentSymbol");
  }
}


EnumType*
EnumType::copyInner(SymbolMap* map) {
  EnumType* copy = new EnumType();
  for_enums(def, this)
    copy->constants.insertAtTail(COPY_INT(def));
  copy->addSymbol(symbol);
  return copy;
}


void EnumType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = toSymbol(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


void EnumType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef enum {");
  bool first = true;
  for_enums(constant, this) {
    if (!first) {
      fprintf(outfile, ", ");
    }
    constant->sym->codegen(outfile);
    if (constant->init) {
      fprintf(outfile, " = ");
      constant->init->codegen(outfile);
    } else if (first) {
      fprintf(outfile, " = 1");
    }
    first = false;
  }
  fprintf(outfile, "} ");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n");
}


ClassType::ClassType(ClassTag initClassTag) :
  Type(TYPE_CLASS, NULL),
  classTag(initClassTag),
  fields(),
  inherits(),
  outer(NULL)
{
  if (classTag == CLASS_CLASS) { // set defaultValue to nil to keep it
                                 // from being constructed
    defaultValue = gNil;
  }
  methods.clear();
  fields.parent = this;
  inherits.parent = this;
}


ClassType::~ClassType() { }


void ClassType::verify() {
  Type::verify();
  if (astTag != TYPE_CLASS) {
    INT_FATAL(this, "Bad ClassType::astTag");
  }
  if (classTag != CLASS_CLASS &&
      classTag != CLASS_RECORD &&
      classTag != CLASS_UNION)
    INT_FATAL(this, "Bad ClassType::classTag");
  if (fields.parent != this || inherits.parent != this)
    INT_FATAL(this, "Bad AList::parent in ClassType");
  for_alist(expr, fields) {
    if (expr->parentSymbol != symbol)
      INT_FATAL(this, "Bad ClassType::fields::parentSymbol");
  }
  for_alist(expr, inherits) {
    if (expr->parentSymbol != symbol)
      INT_FATAL(this, "Bad ClassType::inherits::parentSymbol");
  }
}


ClassType*
ClassType::copyInner(SymbolMap* map) {
  ClassType* copy_type = new ClassType(classTag);
  copy_type->outer = outer;
  for_alist(expr, fields)
    copy_type->fields.insertAtTail(COPY_INT(expr));
  for_alist(expr, inherits)
    copy_type->inherits.insertAtTail(COPY_INT(expr));
  for_fields(field, copy_type) {
    if (FnSymbol* fn = toFnSymbol(field))
      copy_type->methods.add(fn);
  }
  return copy_type;
}


static void
addDeclaration(ClassType* ct, DefExpr* def, bool tail) {
  if (FnSymbol* fn = toFnSymbol(def->sym)) {
    ct->methods.add(fn);
    INT_ASSERT(!fn->_this);
    fn->_this = new ArgSymbol(INTENT_BLANK, "this", ct);
    fn->insertFormalAtHead(new DefExpr(fn->_this));
    fn->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken)));
    fn->addFlag(FLAG_METHOD);
  }
  if (def->parentSymbol || def->list)
    def->remove();
  if (tail)
    ct->fields.insertAtTail(def);
  else
    ct->fields.insertAtHead(def);
}


void ClassType::addDeclarations(Expr* expr, bool tail) {
  if (DefExpr* def = toDefExpr(expr)) {
    addDeclaration(this, def, tail);
  } else if (BlockStmt* block = toBlockStmt(expr)) {
    for_alist(expr, block->body) {
      addDeclarations(expr, tail);
    }
  } else {
    INT_FATAL(expr, "unexpected case");
  }
}


void ClassType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = toSymbol(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


void ClassType::codegenDef(FILE* outfile) {
  fprintf(outfile, "typedef struct __");
  symbol->codegen(outfile);

  if (classTag == CLASS_CLASS && dispatchParents.n > 0) {
    /* Add a comment to class definitions listing super classes */
    bool first = true;
    fprintf(outfile, " /* : ");
    forv_Vec(Type, parent, dispatchParents) {
      if (parent) {
        if (!first) {
          fprintf(outfile, ", ");
        }
        parent->symbol->codegen(outfile);
        first = false;
      }
    }
    fprintf(outfile, " */");
  }
  fprintf(outfile, " {\n");
  if (symbol->hasFlag(FLAG_OBJECT_CLASS) && classTag == CLASS_CLASS) {
    if (fCopyCollect) {
      fprintf(outfile, "union {\n");
      fprintf(outfile, "chpl__class_id chpl__cid;\n");
      fprintf(outfile, "void* _padding_for_copy_collection_use;\n");
      fprintf(outfile, "} chpl__cid_union;\n");
    } else {
      fprintf(outfile, "chpl__class_id chpl__cid;\n");
    }
  } else if (classTag == CLASS_UNION) {
    fprintf(outfile, "int64_t _uid;\n");
    fprintf(outfile, "union {\n");
  }
  for_fields(field, this) {
    field->codegenDef(outfile);
  }
  if (classTag == CLASS_UNION) {
    fprintf(outfile, "} _u;\n");
  }
  if (symbol->hasFlag(FLAG_DATA_CLASS)) {
    toTypeSymbol(substitutions.v[0].value)->codegen(outfile);
    fprintf(outfile, "* _data;\n");
  }
  fprintf(outfile, "} ");
  if (classTag == CLASS_CLASS)
    fprintf(outfile, "_");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n\n");
}


void ClassType::codegenPrototype(FILE* outfile) {
  if (symbol->hasFlag(FLAG_REF))
    fprintf(outfile, "typedef %s *%s;\n", getField(1)->type->symbol->cname,
            symbol->cname);
  else if (classTag == CLASS_CLASS)
    fprintf(outfile, "typedef struct __%s *%s;\n",
            symbol->cname, symbol->cname);
}


Symbol* ClassType::getField(const char* name, bool fatal) {
  Vec<Type*> next, current;
  Vec<Type*>* next_p = &next, *current_p = &current;
  current_p->set_add(this);
  while (current_p->n != 0) {
    forv_Vec(Type, t, *current_p) {
      if (ClassType* ct = toClassType(t)) {
        for_fields(sym, ct) {
          if (!strcmp(sym->name, name))
            return sym;
        }
        forv_Vec(Type, parent, ct->dispatchParents) {
          if (parent)
            next_p->set_add(parent);
        }
      }
    }
    Vec<Type*>* temp = next_p;
    next_p = current_p;
    current_p = temp;
    next_p->clear();
  }
  if (fatal) {
    INT_FATAL(this, "field '%s' not in class in getField", name);
  }
  return NULL;
}


Symbol* ClassType::getField(int i) {
  return toDefExpr(fields.get(i))->sym;
}


static PrimitiveType* 
createPrimitiveType(const char *name, const char *cname) {
  PrimitiveType* pt = new PrimitiveType(NULL);
  TypeSymbol* ts = new TypeSymbol(name, pt);
  ts->cname = cname;
  rootModule->block->insertAtTail(new DefExpr(ts));
  return pt;
}


// Create new primitive type for integers. Specify name for now. Though it will 
// probably be something like int1, int8, etc. in the end. In that case
// we can just specify the width (i.e., size).
#define INIT_PRIMITIVE_BOOL(name, width)                                \
  dtBools[BOOL_SIZE_##width] = createPrimitiveType(name, "chpl_bool" #width); \
  dtBools[BOOL_SIZE_##width]->defaultValue = new_BoolSymbol( false, BOOL_SIZE_##width)

#define INIT_PRIMITIVE_INT( name, width)                                 \
  dtInt[INT_SIZE_ ## width] = createPrimitiveType (name, "int" #width "_t"); \
  dtInt[INT_SIZE_ ## width]->defaultValue = new_IntSymbol( 0, INT_SIZE_ ## width)

#define INIT_PRIMITIVE_UINT( name, width)                                  \
  dtUInt[INT_SIZE_ ## width] = createPrimitiveType (name, "uint" #width "_t"); \
  dtUInt[INT_SIZE_ ## width]->defaultValue = new_UIntSymbol( 0, INT_SIZE_ ## width)

#define INIT_PRIMITIVE_REAL( name, width)                                     \
  dtReal[FLOAT_SIZE_ ## width] = createPrimitiveType (name, "_real" #width); \
  dtReal[FLOAT_SIZE_ ## width]->defaultValue = new_RealSymbol( "0.0", 0.0, FLOAT_SIZE_ ## width)
  
#define INIT_PRIMITIVE_IMAG( name, width)                               \
  dtImag[FLOAT_SIZE_ ## width] = createPrimitiveType (name, "_imag" #width); \
  dtImag[FLOAT_SIZE_ ## width]->defaultValue = new_ImagSymbol( "0.0", 0.0, FLOAT_SIZE_ ## width)
  
#define INIT_PRIMITIVE_COMPLEX( name, width)                                   \
  dtComplex[COMPLEX_SIZE_ ## width]= createPrimitiveType (name, "_complex" #width); \
  dtComplex[COMPLEX_SIZE_ ## width]->defaultValue = new_ComplexSymbol(         \
                                  "_chpl_complex" #width "(0.0, 0.0)",         \
                                   0.0, 0.0, COMPLEX_SIZE_ ## width)

#define CREATE_DEFAULT_SYMBOL(primType, gSym, name)     \
  gSym = new VarSymbol (name, primType);                \
  gSym->addFlag(FLAG_CONST);                          \
  rootModule->block->insertAtTail(new DefExpr(gSym));   \
  primType->defaultValue = gSym


void initPrimitiveTypes(void) {
  // Inititalize the outermost module
  rootModule = new ModuleSymbol("_root", MOD_STANDARD, new BlockStmt());
  rootModule->filename = astr("<internal>");

  theProgram = new ModuleSymbol("_Program", MOD_STANDARD, new BlockStmt());
  theProgram->filename = astr("<internal>");
  rootModule->block->insertAtTail(new DefExpr(theProgram));

  dtNil = createPrimitiveType ("_nilType", "_nilType");
  CREATE_DEFAULT_SYMBOL (dtNil, gNil, "nil");

  dtNilRef = createPrimitiveType ("_nilRefType", "_nilRefType");
  CREATE_DEFAULT_SYMBOL (dtNilRef, gNilRef, "nilRef");
  dtNilRef->symbol->addFlag(FLAG_REF);
  
  dtUnknown = createPrimitiveType ("_unknown", "_unknown");
  CREATE_DEFAULT_SYMBOL (dtUnknown, gUnknown, "_gunknown");

  dtVoid = createPrimitiveType ("void", "void");
  CREATE_DEFAULT_SYMBOL (dtVoid, gVoid, "_void");

  dtBool = createPrimitiveType ("bool", "chpl_bool");

  DefExpr* objectDef = buildClassDefExpr("object", new ClassType(CLASS_CLASS), new BlockStmt());
  objectDef->sym->addFlag(FLAG_OBJECT_CLASS);
  objectDef->sym->addFlag(FLAG_NO_OBJECT);
  dtObject = objectDef->sym->type;
  dtValue = createPrimitiveType("value", "_chpl_value");

  createInitFn(theProgram);
  if (!fRuntime)
    theProgram->initFn->insertAtHead(new CallExpr(PRIMITIVE_USE,
                                       new SymExpr("ChapelStandard")));

  theProgram->initFn->insertAtHead(objectDef);
  CREATE_DEFAULT_SYMBOL (dtBool, gFalse, "false");
  gFalse->immediate = new Immediate;
  gFalse->immediate->v_bool = false;
  gFalse->immediate->const_kind = NUM_KIND_UINT;
  gFalse->immediate->num_index = INT_SIZE_1;
  uniqueConstantsHash.put(gFalse->immediate, gFalse);
  dtBool->defaultValue = gFalse;

  gTrue = new VarSymbol("true", dtBool);
  gTrue->addFlag(FLAG_CONST);
  rootModule->block->insertAtTail(new DefExpr(gTrue));
  gTrue->immediate = new Immediate;
  gTrue->immediate->v_bool = true;
  gTrue->immediate->const_kind = NUM_KIND_UINT;
  gTrue->immediate->num_index = INT_SIZE_1;
  uniqueConstantsHash.put(gTrue->immediate, gTrue);

  gBoundsChecking = new VarSymbol("boundsChecking", dtBool);
  gBoundsChecking->addFlag(FLAG_CONST);
  rootModule->block->insertAtTail(new DefExpr(gBoundsChecking));
  if (fNoBoundsChecks) {
    gBoundsChecking->immediate = new Immediate;
    *gBoundsChecking->immediate = *gFalse->immediate;
  } else {
    gBoundsChecking->immediate = new Immediate;
    *gBoundsChecking->immediate = *gTrue->immediate;
  }

  
  INIT_PRIMITIVE_BOOL("bool(8)", 8);
  INIT_PRIMITIVE_BOOL("bool(16)", 16);
  INIT_PRIMITIVE_BOOL("bool(32)", 32);
  INIT_PRIMITIVE_BOOL("bool(64)", 64);
  dtBools[BOOL_SIZE_SYS] = dtBool;

  // WAW: could have a loop, but the following unrolling is more explicit.
  INIT_PRIMITIVE_INT( "int(8)", 8);
  INIT_PRIMITIVE_INT( "int(16)", 16);
  INIT_PRIMITIVE_INT( "int", 32);
  INIT_PRIMITIVE_INT( "int(64)", 64);            // default size

  INIT_PRIMITIVE_UINT( "uint(8)", 8);
  INIT_PRIMITIVE_UINT( "uint(16)", 16);
  INIT_PRIMITIVE_UINT( "uint", 32);
  INIT_PRIMITIVE_UINT( "uint(64)", 64);          // default size

  INIT_PRIMITIVE_REAL( "real(32)", 32);
  INIT_PRIMITIVE_REAL( "real", 64);            // default size

  INIT_PRIMITIVE_IMAG( "imag(32)", 32);
  INIT_PRIMITIVE_IMAG( "imag", 64);            // default size

  INIT_PRIMITIVE_COMPLEX( "complex(64)", 64);
  INIT_PRIMITIVE_COMPLEX( "complex", 128);       // default size

  dtString = createPrimitiveType( "string", "_string");
  dtString->defaultValue = new_StringSymbol("");

  dtSymbol = createPrimitiveType( "symbol", "_symbol"); 

  dtFile = createPrimitiveType ("_file", "_cfile");
  CREATE_DEFAULT_SYMBOL(dtFile, gFile, "0");

  dtOpaque = createPrimitiveType("opaque", "chpl_opaque");
  CREATE_DEFAULT_SYMBOL(dtOpaque, gOpaque, "_nullOpaque");
  gOpaque->cname = "NULL";

  dtTimer = createPrimitiveType("_timervalue", "_timervalue");
  CREATE_DEFAULT_SYMBOL(dtTimer, gTimer, "_new_timer()");

  dtSyncVarAuxFields = createPrimitiveType( "_sync_aux_t", "chpl_sync_aux_t");
  CREATE_DEFAULT_SYMBOL (dtSyncVarAuxFields, gSyncVarAuxFields, "_nullSyncVarAuxFields");
  gSyncVarAuxFields->cname = "NULL";
  dtSingleVarAuxFields = createPrimitiveType( "_single_aux_t", "chpl_single_aux_t");
  CREATE_DEFAULT_SYMBOL (dtSingleVarAuxFields, gSingleVarAuxFields, "_nullSingleVarAuxFields");
  gSingleVarAuxFields->cname = "NULL";

  dtTaskList = createPrimitiveType( "_task_list", "chpl_task_list_p");
  CREATE_DEFAULT_SYMBOL (dtTaskList, gTaskList, "_nullTaskList");
  gTaskList->cname = "NULL";

  dtAny = createPrimitiveType ("_any", "_any");
  dtAny->symbol->addFlag(FLAG_GENERIC);
  dtIntegral = createPrimitiveType ("integral", "integral");
  dtIntegral->symbol->addFlag(FLAG_GENERIC);
  dtNumeric = createPrimitiveType ("numeric", "numeric");
  dtNumeric->symbol->addFlag(FLAG_GENERIC);
  dtIterator = createPrimitiveType("_iteratorClass", "_iteratorClass");
  dtIterator->symbol->addFlag(FLAG_GENERIC);
  dtMethodToken = createPrimitiveType ("_MT", "_MT");
  CREATE_DEFAULT_SYMBOL(dtMethodToken, gMethodToken, "_mt");
  dtLeaderToken = createPrimitiveType("_LT", "_LT");
  CREATE_DEFAULT_SYMBOL(dtLeaderToken, gLeaderToken, "_lt");
  dtModuleToken = createPrimitiveType("tmodule=", "tmodule=");
  CREATE_DEFAULT_SYMBOL(dtModuleToken, gModuleToken, "module=");
  dtEnumerated = createPrimitiveType ("enumerated", "enumerated");
  dtEnumerated->symbol->addFlag(FLAG_GENERIC);
}


bool is_bool_type(Type* t) {
  return 
    t == dtBools[BOOL_SIZE_8] ||
    t == dtBools[BOOL_SIZE_16] ||
    t == dtBools[BOOL_SIZE_32] ||
    t == dtBools[BOOL_SIZE_64] ||
    t == dtBools[BOOL_SIZE_SYS];
}


bool is_int_type(Type *t) {
  return
    t == dtInt[INT_SIZE_8] ||
    t == dtInt[INT_SIZE_16] ||
    t == dtInt[INT_SIZE_32] ||
    t == dtInt[INT_SIZE_64];
}


bool is_uint_type(Type *t) {
  return
    t == dtUInt[INT_SIZE_8] ||
    t == dtUInt[INT_SIZE_16] ||
    t == dtUInt[INT_SIZE_32] ||
    t == dtUInt[INT_SIZE_64];
}


bool is_real_type(Type *t) {
  return
    t == dtReal[FLOAT_SIZE_32] ||
    t == dtReal[FLOAT_SIZE_64];
}


bool is_imag_type(Type *t) {
  return
    t == dtImag[FLOAT_SIZE_32] ||
    t == dtImag[FLOAT_SIZE_64];
}


bool is_complex_type(Type *t) {
  return
    t == dtComplex[COMPLEX_SIZE_64] ||
    t == dtComplex[COMPLEX_SIZE_128];
}


bool is_enum_type(Type *t) {
  return toEnumType(t);
}


int get_width(Type *t) {
  if (t == dtBools[BOOL_SIZE_SYS]) {
    return 1; 
    // BLC: This is a lie, but one I'm hoping we can get away with
    // based on how this function is used
  }
  if (t == dtBools[BOOL_SIZE_8] || 
      t == dtInt[INT_SIZE_8] || 
      t == dtUInt[INT_SIZE_8])
    return 8;
  if (t == dtBools[BOOL_SIZE_16] || 
      t == dtInt[INT_SIZE_16] || 
      t == dtUInt[INT_SIZE_16])
    return 16;
  if (t == dtBools[BOOL_SIZE_32] ||
      t == dtInt[INT_SIZE_32] || 
      t == dtUInt[INT_SIZE_32] ||
      t == dtReal[FLOAT_SIZE_32] ||
      t == dtImag[FLOAT_SIZE_32])
    return 32;
  if (t == dtBools[BOOL_SIZE_64] ||
      t == dtInt[INT_SIZE_64] || 
      t == dtUInt[INT_SIZE_64] ||
      t == dtReal[FLOAT_SIZE_64] ||
      t == dtImag[FLOAT_SIZE_64] ||
      t == dtComplex[COMPLEX_SIZE_64])
    return 64;
  if (t == dtComplex[COMPLEX_SIZE_128])
    return 128;
  INT_FATAL(t, "Unknown bit width");
  return 0;
}


bool isRecordType(Type* t) {
  ClassType* ct = toClassType(t);
  if (ct && ct->classTag == CLASS_RECORD)
    return true;
  return false;
}

bool isUnionType(Type* t) {
  ClassType* ct = toClassType(t);
  if (ct && ct->classTag == CLASS_UNION)
    return true;
  return false;
}

bool isReferenceType(Type* t) {
  return t->symbol->hasFlag(FLAG_REF);
}
