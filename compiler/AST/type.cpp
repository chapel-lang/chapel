#define TYPE_EXTERN
#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "runtime.h"
#include "stringutil.h"
#include "symbol.h"
#include "symscope.h"
#include "type.h"


Type::Type(AstTag astTag, Symbol* init_defaultVal) :
  BaseAST(astTag),
  scalarPromotionType(NULL),
  symbol(NULL),
  defaultValue(init_defaultVal),
  defaultConstructor(NULL),
  defaultTypeConstructor(NULL),
  isGeneric(false),
  instantiatedFrom(NULL),
  instantiatedWith(NULL),
  refType(NULL)
{}


Type::~Type() {
  if (instantiatedWith)
    delete instantiatedWith;
}


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
Type::copyInner(ASTMap* map) {
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


void Type::codegenDefaultFormat(FILE* outfile, bool isRead) {
  fprintf(outfile, "_default_format");
  if (isRead) {
    fprintf(outfile, "_read");
  } else {
    fprintf(outfile, "_write");
  }
  this->codegen(outfile);
}


bool Type::implementedUsingCVals(void) {
  if (this == dtBool ||
      this == dtInt[INT_SIZE_8]  ||
      this == dtInt[INT_SIZE_16] ||
      this == dtInt[INT_SIZE_32] ||
      this == dtInt[INT_SIZE_64] ||
      this == dtUInt[INT_SIZE_1]  ||
      this == dtUInt[INT_SIZE_8]  ||
      this == dtUInt[INT_SIZE_16] ||
      this == dtUInt[INT_SIZE_32] ||
      this == dtUInt[INT_SIZE_64] ||
      this == dtReal[FLOAT_SIZE_32] ||
      this == dtReal[FLOAT_SIZE_64]) {
    return true;
  } else {
    return false;
 }
}


Symbol* Type::getField(const char* name) {
  INT_FATAL(this, "getField not called on ClassType");
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
  constants(),
  enumScope(NULL)
{
  constants.parent = this;
}


EnumType::~EnumType() {
  if (enumScope)
    delete enumScope;
}


void EnumType::verify() {
  Type::verify();
  if (astTag != TYPE_ENUM) {
    INT_FATAL(this, "Bad EnumType::astTag");
  }
  if (constants.parent != this)
    INT_FATAL(this, "Bad AList::parent in EnumType");
}


EnumType*
EnumType::copyInner(ASTMap* map) {
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


void EnumType::codegenDefaultFormat(FILE* outfile, bool isRead) {
  fprintf(outfile, "_default_format");
  if (isRead) {
    fprintf(outfile, "_read");
  } else {
    fprintf(outfile, "_write");
  }
  fprintf(outfile, "_enum");
}


bool EnumType::implementedUsingCVals(void) {
  return true;
}


ClassType::ClassType(ClassTag initClassTag) :
  Type(TYPE_CLASS, NULL),
  classTag(initClassTag),
  isIterator(false),
  structScope(NULL),
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


ClassType::~ClassType() {
  if (structScope)
    delete structScope;
}


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
}


ClassType*
ClassType::copyInner(ASTMap* map) {
  ClassType* copy_type = new ClassType(classTag);
  copy_type->isIterator = isIterator;
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


void ClassType::addDeclarations(Expr* expr, bool tail) {
  Vec<BaseAST*> asts;

  collect_top_asts(&asts, expr);

  forv_Vec(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        methods.add(fn);
        if (fn->_this) {
          // must be constructor of nested class
          fn->_outer = new ArgSymbol(INTENT_BLANK, "outer", this);
          fn->insertFormalAtHead(new DefExpr(fn->_outer));
        } else {
          fn->_this = new ArgSymbol(INTENT_BLANK, "this", this);
          fn->insertFormalAtHead(new DefExpr(fn->_this));
        }
        fn->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken)));
        fn->isMethod = true;
      }
      if (def->parentSymbol || def->list)
        def->remove();
      if (tail)
        fields.insertAtTail(def);
      else
        fields.insertAtHead(def);
    }
  }
}


void ClassType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  if (old_ast == defaultValue) {
    defaultValue = toSymbol(new_ast);
  } else {
    INT_FATAL(this, "Unexpected case in Type::replaceChild");
  }
}


bool
ClassType::isNominalType() {
  return classTag == CLASS_CLASS;
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
  bool printedSomething = false; // BLC: this is to avoid empty structs, illegal in C
  if (symbol->hasPragma("object class") && classTag == CLASS_CLASS) {
    if (fCopyCollect) {
      fprintf(outfile, "union {\n");
      fprintf(outfile, "_class_id _cid;\n");
      fprintf(outfile, "void* _padding_for_copy_collection_use;\n");
      fprintf(outfile, "} __class_id;\n");
    } else {
      fprintf(outfile, "_class_id _cid;\n");
    }
    printedSomething = true;
  } else if (classTag == CLASS_UNION) {
    fprintf(outfile, "int64_t _uid;\n");
    fprintf(outfile, "union {\n");
  }
  for_fields(field, this) {
    field->codegenDef(outfile);
    printedSomething = true;
  }
  if (classTag == CLASS_UNION) {
    if (!printedSomething)
      fprintf(outfile, "int _emptyUnionPlaceholder;\n");
    fprintf(outfile, "} _u;\n");
    printedSomething = true;
  }
  if (symbol->hasPragma("data class")) {
    toType(substitutions.v[0].value)->symbol->codegen(outfile);
    fprintf(outfile, "* _data;\n");
  }
  if (!printedSomething) {
    fprintf(outfile, "int _emptyStructPlaceholder;\n");
  }
  fprintf(outfile, "} ");
  if (classTag == CLASS_CLASS)
    fprintf(outfile, "_");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n\n");
}


void ClassType::codegenPrototype(FILE* outfile) {
  if (symbol->hasPragma("ref"))
    fprintf(outfile, "typedef %s *%s;\n", getField(1)->type->symbol->cname,
            symbol->cname);
  else if (classTag == CLASS_CLASS)
    fprintf(outfile, "typedef struct __%s *%s;\n",
            symbol->cname, symbol->cname);
}


bool ClassType::implementedUsingCVals(void) {
  return false;
}


Symbol* ClassType::getField(const char* name) {
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
  //INT_FATAL(this, "field '%s' not in class in getField", name);
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
  rootScope->define(ts);
  return pt;
}


// Create new primitive type for integers. Specify name for now. Though it will 
// probably be something like int1, int8, etc. in the end. In that case
// we can just specify the width (i.e., size).
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

#define CREATE_DEFAULT_SYMBOL(primType, gSym, name) \
  gSym = new VarSymbol (name, primType);            \
  toVarSymbol(gSym)->isConst = true;                \
  rootScope->define(gSym);                          \
  primType->defaultValue = gSym


void initPrimitiveTypes(void) {
  rootScope  = new SymScope(NULL, NULL);
  dtNil = createPrimitiveType ("_nilType", "_nilType");
  CREATE_DEFAULT_SYMBOL (dtNil, gNil, "nil");
  
  dtUnknown = createPrimitiveType ("_unknown", "_unknown");
  CREATE_DEFAULT_SYMBOL (dtUnknown, gUnknown, "_gunknown");

  dtVoid = createPrimitiveType ("void", "void");
  CREATE_DEFAULT_SYMBOL (dtVoid, gVoid, "_void");

  DefExpr* objectDef = buildClassDefExpr("object", new ClassType(CLASS_CLASS), new BlockStmt());
  objectDef->sym->addPragma("object class");
  objectDef->sym->addPragma("no object");
  dtObject = objectDef->sym->type;
  dtValue = createPrimitiveType("value", "_chpl_value");

  dtBool = createPrimitiveType ("bool", "_chpl_bool");

  // Inititalize the outermost module
  theProgram = new ModuleSymbol("_Program", MOD_STANDARD, new BlockStmt());
  theProgram->block->parentScope = rootScope;
  theProgram->block->blkScope = new SymScope(theProgram->block, rootScope);
  rootScope->define(theProgram);
  createInitFn(theProgram);
  if (fNoStdIncs)
    theProgram->initFn->insertAtHead(new CallExpr(PRIMITIVE_USE,
                                       new SymExpr("ChapelBase")));
  else
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
  gTrue->isConst = true;
  // SJD: Should intrinsics have DefExprs?
  rootScope->define(gTrue);
  gTrue->immediate = new Immediate;
  gTrue->immediate->v_bool = true;
  gTrue->immediate->const_kind = NUM_KIND_UINT;
  gTrue->immediate->num_index = INT_SIZE_1;
  uniqueConstantsHash.put(gTrue->immediate, gTrue);

  gBoundsChecking = new VarSymbol("boundsChecking", dtBool);
  gBoundsChecking->isConst = true;
  rootScope->define(gBoundsChecking);
  if (fNoBoundsChecks) {
    gBoundsChecking->immediate = new Immediate;
    *gBoundsChecking->immediate = *gFalse->immediate;
  } else {
    gBoundsChecking->immediate = new Immediate;
    *gBoundsChecking->immediate = *gTrue->immediate;
  }

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

  dtTimer = createPrimitiveType("_timervalue", "_timervalue");
  CREATE_DEFAULT_SYMBOL(dtTimer, gTimer, "_new_timer()");

  dtSyncVarAuxFields = createPrimitiveType( "_sync_aux_t", "_chpl_sync_aux_t");
  CREATE_DEFAULT_SYMBOL (dtSyncVarAuxFields, gSyncVarAuxFields, "NULL");
  dtSingleVarAuxFields = createPrimitiveType( "_single_aux_t", "_chpl_single_aux_t");
  CREATE_DEFAULT_SYMBOL (dtSingleVarAuxFields, gSingleVarAuxFields, "NULL");

  dtAny = createPrimitiveType ("_any", "_any");
  dtAny->isGeneric = true;
  dtIntegral = createPrimitiveType ("integral", "integral");
  dtIntegral->isGeneric = true;
  dtNumeric = createPrimitiveType ("numeric", "numeric");
  dtNumeric->isGeneric = true;
  dtIterator = createPrimitiveType("_iteratorClass", "_iteratorClass");
  dtIterator->isGeneric = true;
  dtMethodToken = createPrimitiveType ("_MT", "_MT");
  CREATE_DEFAULT_SYMBOL (dtMethodToken, gMethodToken, "_mt");
  dtEnumerated = createPrimitiveType ("enumerated", "enumerated");
  dtEnumerated->isGeneric = true;
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
  if (t == dtInt[INT_SIZE_8] || t == dtUInt[INT_SIZE_8])
    return 8;
  if (t == dtInt[INT_SIZE_16] || t == dtUInt[INT_SIZE_16])
    return 16;
  if (t == dtInt[INT_SIZE_32] || 
      t == dtUInt[INT_SIZE_32] ||
      t == dtReal[FLOAT_SIZE_32] ||
      t == dtImag[FLOAT_SIZE_32])
    return 32;
  if (t == dtInt[INT_SIZE_64] || 
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

bool isReference(Type* t) {
  return !t->refType;
}

Type* getValueType(Type* type) {
  if (ClassType* ct = toClassType(type)) {
    if (ct->symbol->hasPragma("ref")) {
      return ct->getField(1)->type;
    }
  }
  return NULL;
}
