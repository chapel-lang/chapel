#define TYPE_EXTERN
#include "astutil.h"
#include "build.h"
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "vec.h"

#include "intlimits.h"


Type::Type(AstTag astTag, Symbol* init_defaultVal) :
  BaseAST(astTag),
  scalarPromotionType(NULL),
  symbol(NULL),
  defaultValue(init_defaultVal),
  initializer(NULL),
  defaultTypeConstructor(NULL),
  destructor(NULL),
  hasGenericDefaults(false),
  instantiatedFrom(NULL),
  refType(NULL)
{}


Type::~Type() { }


void Type::verify() { }


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


int Type::codegenStructure(FILE* outfile, const char* baseoffset) {
  INT_FATAL(this, "Unexpected type in codegenStructure: %s", symbol->name);
  return 0;
}


Symbol* Type::getField(const char* name, bool fatal) {
  INT_FATAL(this, "getField not called on ClassType");
  return NULL;
}


PrimitiveType::PrimitiveType(Symbol *init, bool internalType) :
  Type(E_PrimitiveType, init)
{
  isInternalType = internalType;
  gPrimitiveTypes.add(this);
}


PrimitiveType*
PrimitiveType::copyInner(SymbolMap* map) {
  INT_FATAL(this, "Unexpected call to PrimitiveType::copyInner");
  return this;
}


void PrimitiveType::replaceChild(BaseAST* old_ast, BaseAST* new_ast) {
  INT_FATAL(this, "Unexpected case in PrimitiveType::replaceChild");
}



void PrimitiveType::verify() {
  Type::verify();
  if (astTag != E_PrimitiveType) {
    INT_FATAL(this, "Bad PrimitiveType::astTag");
  }
}

int PrimitiveType::codegenStructure(FILE* outfile, const char* baseoffset) {
  if (!isInternalType) {
    Symbol* cgsym = symbol;

    fprintf(outfile, "{CHPL_TYPE_%s, %s},\n", cgsym->cname, baseoffset);
  } else
    INT_FATAL(this, "Cannot codegen an internal type");
  return 1;
}

EnumType::EnumType() :
  Type(E_EnumType, NULL),
  constants(), integerType(NULL)
{
  gEnumTypes.add(this);
  constants.parent = this;
}


EnumType::~EnumType() { }


void EnumType::verify() {
  Type::verify();
  if (astTag != E_EnumType) {
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
  INT_FATAL(this, "Unexpected case in EnumType::replaceChild");
}


void EnumType::codegenDef(FILE* outfile) {

  // Make sure that we've computed all of the enum values..
  getIntegerType();

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

int EnumType::codegenStructure(FILE* outfile, const char* baseoffset) {
  fprintf(outfile, "{CHPL_TYPE_enum, %s},\n", baseoffset);
  return 1;
}

void EnumType::sizeAndNormalize() {
  bool first = true;
  bool issigned = false;
  int64_t v;
  uint64_t uv;
  int64_t min_v, max_v;
  uint64_t min_uv, max_uv;
  int num_bytes;
  uint64_t max;
  int64_t min;
  PrimitiveType* ret = NULL;

  // First, look for negative numbers in the enum.
  // If there are any, we have to store all
  // the values in negative numbers.
  for_enums(constant, this) {
    if( constant->init ) {
      if( get_int( constant->init, &v ) ) {
        if( v < 0 ) {
          issigned = true;
        }
      } else if( get_uint( constant->init, &uv ) ) {
        // OK!
      } else {
        // If we get here, then the initializer does not have an immediate
        // value associated with it....
        SymExpr* sym = toSymExpr(constant->init);
        // We think that all params should have init values by now.
        INT_ASSERT(sym && !sym->var->hasFlag(FLAG_PARAM));
        // So we're going to blame this on the user.
        USR_FATAL(constant, "enumerator '%s' is not an integer param value", 
                  constant->sym->name);
        // And unfortunately, if we get here, we don't know how to proceed,
        // which is why no USR_FATAL_CONT().
      }
    }
  }

  // Set initializers for all enum symbols and
  // compute the minimum and maximum values.
  v = 1;
  uv = 1;
  min_v = max_v = 1;
  min_uv = max_uv = 1;

  first = true;
  for_enums(constant, this) {
    if( constant->init ) {
      // set v and uv to the initializer value
      if( get_int( constant->init, &v ) ) {
        if( v >= 0 ) uv = v;
        else uv = 1;
      } else if( get_uint( constant->init, &uv ) ) {
        v = uv;
      }
    } else {
      // create initializer with v
      if( issigned ) {
        if( v >= INT32_MIN && v <= INT32_MAX ) {
          constant->init = new SymExpr(new_IntSymbol(v, INT_SIZE_32));
        } else {
          constant->init = new SymExpr(new_IntSymbol(v, INT_SIZE_64));
        }
      } else {
        if( uv <= UINT32_MAX ) {
          constant->init = new SymExpr(new_UIntSymbol(uv, INT_SIZE_32));
        } else {
          constant->init = new SymExpr(new_UIntSymbol(uv, INT_SIZE_64));
        }
      }
      constant->init->parentExpr = constant;
      constant->init->parentSymbol = constant->parentSymbol;
    }
    if( first ) {
      min_v = v;
      max_v = v;
      min_uv = uv;
      max_uv = uv;
      first = false;
    } else {
      if( min_v > v ) min_v = v;
      if( max_v < v ) max_v = v;
      if( min_uv > uv ) min_uv = uv;
      if( max_uv < uv ) max_uv = uv;
    }
    // Increment v for the next one, in case it is not set.
    v++;
    uv++;
  }

  num_bytes = 0;

  // Now figure out, based on min/max values, what integer
  // size we must use.
  if( issigned ) {
    int num_bytes_neg = 0;
    if( min_v >= INT8_MIN ) {
      num_bytes_neg = 1;
    } else if( min_v >= INT16_MIN ) {
      num_bytes_neg = 2;
    } else if( min_v >= INT32_MIN ) {
      num_bytes_neg = 4;
    } else {
      num_bytes_neg = 8;
    }

    if( max_v <= INT8_MAX ) {
      num_bytes = 1;
    } else if( max_v <= INT16_MAX ) {
      num_bytes = 2;
    } else if( max_v <= INT32_MAX ) {
      num_bytes = 4;
    } else {
      num_bytes = 8;
    }

    if( num_bytes < num_bytes_neg ) num_bytes = num_bytes_neg;
  } else {
    if( max_v <= UINT8_MAX ) {
      num_bytes = 1;
    } else if( max_v <= UINT16_MAX ) {
      num_bytes = 2;
    } else if( max_v <= UINT32_MAX ) {
      num_bytes = 4;
    } else {
      num_bytes = 8;
    }
  }

  // Now figure out field min and max values.
  // and set et->integerType
  min = max = 0;

  if( num_bytes == 1 ) {
    if( issigned ) {
      max = INT8_MAX;
      min = INT8_MIN;
      ret = dtInt[INT_SIZE_8];
    } else {
      max = UINT8_MAX;
      ret = dtUInt[INT_SIZE_8];
    }
  } else if( num_bytes == 2 ) {
    if( issigned ) {
      max = INT16_MAX;
      min = INT16_MIN;
      ret = dtInt[INT_SIZE_16];
    } else {
      max = UINT16_MAX;
      ret = dtUInt[INT_SIZE_16];
    }
  } else if( num_bytes == 4 ) {
    if( issigned ) {
      max = INT32_MAX;
      min = INT32_MIN;
      ret = dtInt[INT_SIZE_32];
    } else {
      max = UINT32_MAX;
      ret = dtUInt[INT_SIZE_32];
    }
  } else if( num_bytes == 8 ) {
    if( issigned ) {
      max = INT64_MAX;
      min = INT64_MIN;
      ret = dtInt[INT_SIZE_64];
    } else {
      max = UINT64_MAX;
      ret = dtUInt[INT_SIZE_64];
    }
  }

  // At the end of it all, check that each enum
  // symbol fits into the assigned type.
  // This check is necessary because we might have
  // had the impossible-to-fit enum
  // because it has e.g. UINT64_MAX and INT64_MIN.
  for_enums(constant, this) {
    if( get_int( constant->init, &v ) ) {
      if( v < min || (v > 0 && (uint64_t)v > max) ) {
        INT_FATAL(constant, "Does not fit in enum integer type");
      }
    } else if( get_uint( constant->init, &uv ) ) {
      if( uv > max ) {
        INT_FATAL(constant, "Does not fit in enum integer type");
      }
    }
  }

  integerType = ret;
}

PrimitiveType* EnumType::getIntegerType() {
  if( ! integerType ) {
    sizeAndNormalize();
  }
  return integerType;
}


ClassType::ClassType(ClassTag initClassTag) :
  Type(E_ClassType, NULL),
  classTag(initClassTag),
  fields(),
  inherits(),
  outer(NULL),
  doc(NULL)
{
  if (classTag == CLASS_CLASS) { // set defaultValue to nil to keep it
                                 // from being constructed
    defaultValue = gNil;
  }
  methods.clear();
  fields.parent = this;
  inherits.parent = this;
  gClassTypes.add(this);
}


ClassType::~ClassType() { }


void ClassType::verify() {
  Type::verify();
  if (astTag != E_ClassType) {
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
    if (fn->_this) {
      // get the name used in the type binding clause
      // this is the way it comes from the parser (see fn_decl_stmt_inner)
      ArgSymbol* thisArg = toArgSymbol(fn->_this);  INT_ASSERT(thisArg);
      INT_ASSERT(thisArg->type == dtUnknown);
      BlockStmt* bs = thisArg->typeExpr;  INT_ASSERT(bs && bs->length() == 1);
      Expr* firstexpr = bs->body.first();  INT_ASSERT(firstexpr);
      UnresolvedSymExpr* sym = toUnresolvedSymExpr(firstexpr); INT_ASSERT(sym);
      const char* name = sym->unresolved;
      // ... then report it to the user
      USR_FATAL_CONT(fn->_this, "Type binding clauses ('%s.' in this case) are not supported in declarations within a class, record or union", name);
    } else {
      fn->_this = new ArgSymbol(fn->thisTag, "this", ct);
      fn->_this->addFlag(FLAG_ARG_THIS);
      fn->insertFormalAtHead(new DefExpr(fn->_this));
      fn->insertFormalAtHead(new DefExpr(new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken)));
      fn->addFlag(FLAG_METHOD);
    }
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
  INT_FATAL(this, "Unexpected case in ClassType::replaceChild");
}


void ClassType::codegenDef(FILE* outfile) {
  if (symbol->hasFlag(FLAG_STAR_TUPLE)) {
    fprintf(outfile, "typedef ");
    getField("x1")->type->codegen(outfile);
    fprintf(outfile, " ");
    symbol->codegen(outfile);
    fprintf(outfile, "[%d];\n\n", fields.length);
    return;
  }
  else if (symbol->hasFlag(FLAG_FIXED_STRING)) {
    int size = toVarSymbol(symbol->type->substitutions.v[0].value)->immediate->int_value();
    fprintf(outfile, "typedef char %s[%i];\n", symbol->cname, size);
    return;
  }
  else if (symbol->hasFlag(FLAG_REF)) {
    fprintf(outfile, "typedef %s *%s;\n", getField(1)->type->symbol->cname,
            symbol->cname);
    return;
  }
  else if (symbol->hasFlag(FLAG_DATA_CLASS)) {
    fprintf(outfile, "typedef ");
    getDataClassType(symbol)->codegen(outfile);
    fprintf(outfile, " *");
    symbol->codegen(outfile);
    fprintf(outfile, ";\n");
    return;
  }

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
    fprintf(outfile, "chpl__class_id chpl__cid;\n");
  } else if (classTag == CLASS_UNION) {
    fprintf(outfile, "int64_t _uid;\n");
    if (this->fields.length != 0)
      fprintf(outfile, "union {\n");
  } else if (this->fields.length == 0) {
    fprintf(outfile, "int dummyFieldToAvoidWarning;\n");
  }

  if (this->fields.length != 0) {
    for_fields(field, this) {
      field->codegenDef(outfile);
    }
  }

  if (classTag == CLASS_UNION) {
    if (this->fields.length != 0)
      fprintf(outfile, "} _u;\n");
  }
  fprintf(outfile, "} ");
  if (classTag == CLASS_CLASS)
    fprintf(outfile, "_");
  symbol->codegen(outfile);
  fprintf(outfile, ";\n\n");
}


void ClassType::codegenPrototype(FILE* outfile) {
  if (symbol->hasFlag(FLAG_REF)) return; // done in codegenDef
  else if (symbol->hasFlag(FLAG_DATA_CLASS)) return; // done in codegenDef
  else if (classTag == CLASS_CLASS)
    fprintf(outfile, "typedef struct __%s *%s;\n",
            symbol->cname, symbol->cname);
}


int ClassType::codegenStructure(FILE* outfile, const char* baseoffset) {
  switch (classTag) {
  case CLASS_CLASS:
    fprintf(outfile, "{CHPL_TYPE_CLASS_REFERENCE, %s},\n", baseoffset);
    return 1;
  case CLASS_RECORD:
    return codegenFieldStructure(outfile, true, baseoffset);
  case CLASS_UNION:
    INT_FATAL(this, "Don't know how to codegenStructure for unions yet");
    return 0;
  default:
    INT_FATAL(this, "Unexpected case in ClassType::codegenStructure");
    return 0;
  }
}


// BLC: I'm not understanding why special cases would need to be called
// out here
static const char* genUnderscore(Symbol* sym) {
  ClassType* classtype = toClassType(sym->type);
  if (classtype && classtype->classTag == CLASS_CLASS && 
      !sym->hasFlag(FLAG_REF)) {
    return "_";
  } else {
    return "";
  }
}


static const char* genChplTypeEnumString(TypeSymbol* typesym) {
  return astr("chpl_rt_type_id_", genUnderscore(typesym), typesym->cname);
}


static const char* genSizeofStr(TypeSymbol* typesym) {
  return astr("sizeof(", genUnderscore(typesym), typesym->cname, ")");
}


static const char* genNewBaseOffsetString(TypeSymbol* typesym, int fieldnum,
                                          const char* baseoffset, Symbol* field,
                                          ClassType* classtype) {
  if (classtype->symbol->hasFlag(FLAG_STAR_TUPLE)) {
    char fieldnumstr[64];
    sprintf(fieldnumstr, "%d", fieldnum);
    return astr(baseoffset, " + (", fieldnumstr, "* ", 
                genSizeofStr(field->type->symbol), ")");
  } else {
    return astr(baseoffset, " + offsetof(", 
                genUnderscore(classtype->symbol),
                classtype->symbol->cname, ", ", 
                classtype->classTag == CLASS_UNION ? "_u." : "",
                field->cname, ")");
  }
}

int ClassType::codegenFieldStructure(FILE* outfile, bool nested, 
                                     const char* baseoffset) {
  // Handle ref types as pointers
  if (symbol->hasFlag(FLAG_REF)) {
    fprintf(outfile, "{CHPL_TYPE_CLASS_REFERENCE, 0},\n");
    fprintf(outfile, "{CHPL_TYPE_DONE, -1}\n");
    return 1;
  }

  int totfields = 0;
  int locfieldnum = 0;
  for_fields(field, this) {
    const char* newbaseoffset = genNewBaseOffsetString(symbol, locfieldnum,
                                                       baseoffset, field,
                                                       this);
    int numfields = field->type->codegenStructure(outfile, newbaseoffset);
    fprintf(outfile, " /* %s */\n", field->name);
    totfields += numfields;
    locfieldnum++;
  }
  if (!nested) {
    fprintf(outfile, "{CHPL_TYPE_DONE, -1}\n");
    totfields += 1;
  }
  return totfields;
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
    const char *className = (char*)"<no name>";
    if (this->symbol) { // this is always true?
      className = this->symbol->name;
    }
    // TODO: report as a user error in certain cases
    INT_FATAL(this, "no field '%s' in class '%s' in getField()",
              name, className);
  }
  return NULL;
}


Symbol* ClassType::getField(int i) {
  return toDefExpr(fields.get(i))->sym;
}


static PrimitiveType* 
createPrimitiveType(const char *name, const char *cname, bool internalType=false) {
  PrimitiveType* pt = new PrimitiveType(NULL, internalType);
  TypeSymbol* ts = new TypeSymbol(name, pt);
  ts->cname = cname;
  ts->addFlag(FLAG_GLOBAL_TYPE_SYMBOL); // Is attached to a global variable;
                                        // should not be deleted!
  rootModule->block->insertAtTail(new DefExpr(ts));
  return pt;
}

static PrimitiveType*
createInternalType(const char *name, const char *cname) {
  return createPrimitiveType(name, cname, true /* internalType */);
}


// Create new primitive type for integers. Specify name for now. Though it will 
// probably be something like int1, int8, etc. in the end. In that case
// we can just specify the width (i.e., size).
#define INIT_PRIM_BOOL(name, width)                                \
  dtBools[BOOL_SIZE_##width] = createPrimitiveType(name, "chpl_bool" #width); \
  dtBools[BOOL_SIZE_##width]->defaultValue = new_BoolSymbol( false, BOOL_SIZE_##width)

#define INIT_PRIM_INT( name, width)                                 \
  dtInt[INT_SIZE_ ## width] = createPrimitiveType (name, "int" #width "_t"); \
  dtInt[INT_SIZE_ ## width]->defaultValue = new_IntSymbol( 0, INT_SIZE_ ## width)

#define INIT_PRIM_UINT( name, width)                                  \
  dtUInt[INT_SIZE_ ## width] = createPrimitiveType (name, "uint" #width "_t"); \
  dtUInt[INT_SIZE_ ## width]->defaultValue = new_UIntSymbol( 0, INT_SIZE_ ## width)

#define INIT_PRIM_REAL( name, width)                                     \
  dtReal[FLOAT_SIZE_ ## width] = createPrimitiveType (name, "_real" #width); \
  dtReal[FLOAT_SIZE_ ## width]->defaultValue = new_RealSymbol( "0.0", 0.0, FLOAT_SIZE_ ## width)
  
#define INIT_PRIM_IMAG( name, width)                               \
  dtImag[FLOAT_SIZE_ ## width] = createPrimitiveType (name, "_imag" #width); \
  dtImag[FLOAT_SIZE_ ## width]->defaultValue = new_ImagSymbol( "0.0", 0.0, FLOAT_SIZE_ ## width)
  
#define INIT_PRIM_COMPLEX( name, width)                                   \
  dtComplex[COMPLEX_SIZE_ ## width]= createPrimitiveType (name, "_complex" #width); \
  dtComplex[COMPLEX_SIZE_ ## width]->defaultValue = new_ComplexSymbol(         \
                                  "_chpl_complex" #width "(0.0, 0.0)",         \
                                   0.0, 0.0, COMPLEX_SIZE_ ## width)

#define CREATE_DEFAULT_SYMBOL(primType, gSym, name)     \
  gSym = new VarSymbol (name, primType);                \
  gSym->addFlag(FLAG_CONST);                          \
  rootModule->block->insertAtTail(new DefExpr(gSym));   \
  primType->defaultValue = gSym


void initChplProgram(void) {
  // Inititalize the outermost module
  rootModule = new ModuleSymbol("_root", MOD_INTERNAL, new BlockStmt());
  rootModule->filename = astr("<internal>");
  rootModule->addFlag(FLAG_NO_DEFAULT_USE);

  theProgram = new ModuleSymbol("chpl__Program", MOD_INTERNAL, new BlockStmt());
  theProgram->filename = astr("<internal>");
  theProgram->addFlag(FLAG_NO_DEFAULT_USE);

  rootModule->block->insertAtTail(new DefExpr(theProgram));
}

// This should probably be renamed since it creates primitive types, as
//  well as internal types and other types used in the generated code
void initPrimitiveTypes(void) {
  dtNil = createInternalType ("_nilType", "_nilType");
  CREATE_DEFAULT_SYMBOL (dtNil, gNil, "nil");

  dtUnknown = createInternalType ("_unknown", "_unknown");
  CREATE_DEFAULT_SYMBOL (dtUnknown, gUnknown, "_gunknown");

  dtVoid = createInternalType ("void", "void");
  CREATE_DEFAULT_SYMBOL (dtVoid, gVoid, "_void");

  dtBool = createPrimitiveType ("bool", "chpl_bool");

  dtObject = new ClassType(CLASS_CLASS);
  dtValue = createInternalType("value", "_chpl_value");

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

  gTryToken = new VarSymbol("chpl__tryToken", dtBool);
  gTryToken->addFlag(FLAG_CONST);
  rootModule->block->insertAtTail(new DefExpr(gTryToken));

  
  INIT_PRIM_BOOL("bool(8)", 8);
  INIT_PRIM_BOOL("bool(16)", 16);
  INIT_PRIM_BOOL("bool(32)", 32);
  INIT_PRIM_BOOL("bool(64)", 64);
  dtBools[BOOL_SIZE_SYS] = dtBool;

  // WAW: could have a loop, but the following unrolling is more explicit.
  INIT_PRIM_INT( "int(8)", 8);
  INIT_PRIM_INT( "int(16)", 16);
  INIT_PRIM_INT( "int(32)", 32);
  INIT_PRIM_INT( "int", 64);            // default size

  INIT_PRIM_UINT( "uint(8)", 8);
  INIT_PRIM_UINT( "uint(16)", 16);
  INIT_PRIM_UINT( "uint(32)", 32);
  INIT_PRIM_UINT( "uint", 64);          // default size

  INIT_PRIM_REAL( "real(32)", 32);
  INIT_PRIM_REAL( "real", 64);            // default size

  INIT_PRIM_IMAG( "imag(32)", 32);
  INIT_PRIM_IMAG( "imag", 64);            // default size

  INIT_PRIM_COMPLEX( "complex(64)", 64);
  INIT_PRIM_COMPLEX( "complex", 128);       // default size

  dtString = createPrimitiveType( "string", "chpl_string");
  dtString->defaultValue = new_StringSymbol("");

  dtLocaleID = createPrimitiveType("_locale_id", "c_locale_t");
  CREATE_DEFAULT_SYMBOL(dtLocaleID, gLocaleID, "_rootLocaleID");

  dtSymbol = createPrimitiveType( "symbol", "_symbol"); 

  dtFile = createPrimitiveType ("_file", "_cfile");
  CREATE_DEFAULT_SYMBOL(dtFile, gFile, "0");

  dtOpaque = createPrimitiveType("opaque", "chpl_opaque");
  CREATE_DEFAULT_SYMBOL(dtOpaque, gOpaque, "_nullOpaque");
  gOpaque->cname = "NULL";

  dtTimer = createPrimitiveType("_timervalue", "_timervalue");
  CREATE_DEFAULT_SYMBOL(dtTimer, gTimer, "chpl_new_timer()");

  dtTaskID = createPrimitiveType("chpl_taskID_t", "chpl_taskID_t");
  CREATE_DEFAULT_SYMBOL(dtTaskID, gTaskID, "chpl_nullTaskID");

  dtSyncVarAuxFields = createPrimitiveType( "_sync_aux_t", "chpl_sync_aux_t");
  CREATE_DEFAULT_SYMBOL (dtSyncVarAuxFields, gSyncVarAuxFields, "_nullSyncVarAuxFields");
  gSyncVarAuxFields->cname = "NULL";
  dtSingleVarAuxFields = createPrimitiveType( "_single_aux_t", "chpl_single_aux_t");
  CREATE_DEFAULT_SYMBOL (dtSingleVarAuxFields, gSingleVarAuxFields, "_nullSingleVarAuxFields");
  gSingleVarAuxFields->cname = "NULL";

  dtTaskList = createPrimitiveType( "_task_list", "chpl_task_list_p");
  CREATE_DEFAULT_SYMBOL (dtTaskList, gTaskList, "_nullTaskList");
  gTaskList->cname = "NULL";

  dtAny = createInternalType ("_any", "_any");
  dtAny->symbol->addFlag(FLAG_GENERIC);
  dtIntegral = createInternalType ("integral", "integral");
  dtIntegral->symbol->addFlag(FLAG_GENERIC);
  dtAnyComplex = createInternalType("chpl_anycomplex", "complex");
  dtAnyComplex->symbol->addFlag(FLAG_GENERIC);
  dtNumeric = createInternalType ("numeric", "numeric");
  dtNumeric->symbol->addFlag(FLAG_GENERIC);
  dtIteratorRecord = createInternalType("_iteratorRecord", "_iteratorRecord");
  dtIteratorRecord->symbol->addFlag(FLAG_GENERIC);
  dtIteratorClass = createInternalType("_iteratorClass", "_iteratorClass");
  dtIteratorClass->symbol->addFlag(FLAG_GENERIC);
  dtMethodToken = createInternalType ("_MT", "_MT");
  CREATE_DEFAULT_SYMBOL(dtMethodToken, gMethodToken, "_mt");
  dtTypeDefaultToken = createInternalType("_TypeDefaultT", "_TypeDefaultT");
  CREATE_DEFAULT_SYMBOL(dtTypeDefaultToken, gTypeDefaultToken, "_typeDefaultT");
  dtModuleToken = createInternalType("tmodule=", "tmodule=");
  CREATE_DEFAULT_SYMBOL(dtModuleToken, gModuleToken, "module=");

  dtAnyEnumerated = createInternalType ("enumerated", "enumerated");
  dtAnyEnumerated->symbol->addFlag(FLAG_GENERIC);
}

void initTheProgram(void) {
  createInitFn(theProgram);
  theProgram->initFn->addFlag(FLAG_EXPORT);     // Called from main.c

  theProgram->initFn->insertAtTail(
    new CallExpr(PRIM_USE, new UnresolvedSymExpr("ChapelBase")));

  // it may be better to add the following use after parsing
  // to simplify insertion of module guard sync var defs
  theProgram->initFn->insertAtTail(
    new CallExpr(PRIM_USE, new UnresolvedSymExpr("ChapelStandard")));
  
  // The base object class looks like this:
  //
  //   class object {
  //     chpl__class_id chpl__cid;
  //   }
  //
  // chpl__class_id is an int32_t field identifying the classes
  //  in the program.  We never create the actual field within the
  //  IR (it is directly generated in the C code).  It might
  //  be the right thing to do, so I made an attempt at adding the
  //  field.  Unfortunately, we would need some significant changes
  //  throughout compilation, and it seemed to me that the it might result
  //  in possibly more special case code.
  //
  DefExpr* objectDef = buildClassDefExpr("object", dtObject,
                                         NULL, new BlockStmt(), FLAG_UNKNOWN, NULL);
  objectDef->sym->addFlag(FLAG_OBJECT_CLASS);
  objectDef->sym->addFlag(FLAG_NO_OBJECT);
  theProgram->initFn->insertAtHead(objectDef);
}

void initCompilerGlobals(void) {

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

  gPrivatization = new VarSymbol("_privatization", dtBool);
  gPrivatization->addFlag(FLAG_PARAM);
  rootModule->block->insertAtTail(new DefExpr(gPrivatization));
  if (fNoPrivatization || fLocal) {
    gPrivatization->immediate = new Immediate;
    *gPrivatization->immediate = *gFalse->immediate;
  } else {
    gPrivatization->immediate = new Immediate;
    *gPrivatization->immediate = *gTrue->immediate;
  }

  gLocal = new VarSymbol("_local", dtBool);
  gLocal->addFlag(FLAG_PARAM);
  rootModule->block->insertAtTail(new DefExpr(gLocal));
  if (fLocal) {
    gLocal->immediate = new Immediate;
    *gLocal->immediate = *gTrue->immediate;
  } else {
    gLocal->immediate = new Immediate;
    *gLocal->immediate = *gFalse->immediate;
  }

}

bool is_bool_type(Type* t) {
  return 
    t == dtBools[BOOL_SIZE_SYS] ||
    t == dtBools[BOOL_SIZE_8] ||
    t == dtBools[BOOL_SIZE_16] ||
    t == dtBools[BOOL_SIZE_32] ||
    t == dtBools[BOOL_SIZE_64];
}


bool is_int_type(Type *t) {
  return
    t == dtInt[INT_SIZE_32] ||
    t == dtInt[INT_SIZE_8] ||
    t == dtInt[INT_SIZE_16] ||
    t == dtInt[INT_SIZE_64];
}


bool is_uint_type(Type *t) {
  return
    t == dtUInt[INT_SIZE_32] ||
    t == dtUInt[INT_SIZE_8] ||
    t == dtUInt[INT_SIZE_16] ||
    t == dtUInt[INT_SIZE_64];
}


bool is_real_type(Type *t) {
  return
    t == dtReal[FLOAT_SIZE_64] ||
    t == dtReal[FLOAT_SIZE_32];
}


bool is_imag_type(Type *t) {
  return
    t == dtImag[FLOAT_SIZE_64] ||
    t == dtImag[FLOAT_SIZE_32];
}


bool is_complex_type(Type *t) {
  return
    t == dtComplex[COMPLEX_SIZE_128] ||
    t == dtComplex[COMPLEX_SIZE_64];
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


bool isClass(Type* t) {
  if (ClassType* ct = toClassType(t))
    if (ct->classTag == CLASS_CLASS)
      return true;
  return false;
}


bool isRecord(Type* t) {
  if (ClassType* ct = toClassType(t))
    if (ct->classTag == CLASS_RECORD)
      return true;
  return false;
}

bool isUnion(Type* t) {
  if (ClassType* ct = toClassType(t))
    if (ct->classTag == CLASS_UNION)
      return true;
  return false;
}

bool isReferenceType(Type* t) {
  return t->symbol->hasFlag(FLAG_REF);
}

bool isRefCountedType(Type* t) {
  // We may eventually want to add a separate flag and provide users
  //  with an interface to declare reference counted types that will
  //  be "automatically" reference counted when needed

  // The set of reference counted types currently coincides with the set
  // of record-wrapped types, so we can reuse the flag set. 
  return getRecordWrappedFlags(t->symbol).any();
}

bool isRecordWrappedType(Type* t) {
  return getRecordWrappedFlags(t->symbol).any();
}

bool isSyncType(Type* t) {
  return getSyncFlags(t->symbol).any();
}

bool
isDistClass(Type* type) {
  if (type->symbol->hasFlag(FLAG_BASE_DIST))
    return true;
  forv_Vec(Type, pt, type->dispatchParents)
    if (isDistClass(pt))
      return true;
  return false;
}

bool
isDomainClass(Type* type) {
  if (type->symbol->hasFlag(FLAG_BASE_DOMAIN))
    return true;
  forv_Vec(Type, pt, type->dispatchParents)
    if (isDomainClass(pt))
      return true;
  return false;
}

bool
isArrayClass(Type* type) {
  if (type->symbol->hasFlag(FLAG_BASE_ARRAY))
    return true;
  forv_Vec(Type, t, type->dispatchParents)
    if (isArrayClass(t))
      return true;
  return false;
}


static Vec<TypeSymbol*> typesToStructurallyCodegen;
static Vec<TypeSymbol*> typesToStructurallyCodegenList;

void registerTypeToStructurallyCodegen(TypeSymbol* type) {
  //  printf("registering chpl_rt_type_id_%s\n", type->cname);
  if (!typesToStructurallyCodegen.set_in(type)) {
    typesToStructurallyCodegenList.add(type);
    typesToStructurallyCodegen.set_add(type);
  }
}

// ???
void genTypeStructureIndex(FILE *outfile, TypeSymbol* typesym) {
  if (fHeterogeneous) {
    // strings are special
    if (toPrimitiveType(typesym) == dtString) {
      fprintf(outfile, "-%s", typesym->cname);
    } else {
      fprintf(outfile, "%s", genChplTypeEnumString(typesym));
    }
  } else {
    fprintf(outfile, "-1");
  }
}

// Compare the cnames of different types alphabetically
static int compareCnames(const void* v1, const void* v2) {
  int retval;
  TypeSymbol* t1 = *(TypeSymbol**)v1;
  TypeSymbol* t2 = *(TypeSymbol**)v2;
  retval = strcmp(t1->cname, t2->cname);
  if (retval > 0)
    return 1;
  else if (retval < 0)
    return -1;
  else
    return 0;
}

#define TYPE_STRUCTURE_FILE "_type_structure.c"
static int maxFieldsPerType = 0;

void codegenTypeStructures(FILE* hdrfile) {
  fileinfo typeStructFile;
  openCFile(&typeStructFile, TYPE_STRUCTURE_FILE);
  FILE* outfile = typeStructFile.fptr;

  fprintf(outfile, "typedef enum {\n");
  forv_Vec(TypeSymbol*, typesym, typesToStructurallyCodegenList) {
    fprintf(outfile, "%s,\n", genChplTypeEnumString(typesym));
  }
  fprintf(outfile, "chpl_num_rt_type_ids\n");
  fprintf(outfile, "} chpl_rt_types;\n\n");

  fprintf(outfile, "chpl_fieldType chpl_structType[][CHPL_MAX_FIELDS_PER_TYPE] = {\n");
            
  qsort(typesToStructurallyCodegenList.v, typesToStructurallyCodegenList.n, sizeof(typesToStructurallyCodegenList.v[0]), compareCnames);
  int num = 0;
  forv_Vec(TypeSymbol*, typesym, typesToStructurallyCodegenList) {
    if (num) {
      fprintf(outfile, ",\n");
    }
    fprintf(outfile, "/* %s (%s) */\n", typesym->name, typesym->cname);
    fprintf(outfile, "{\n");
    if (ClassType* classtype = toClassType(typesym->type)) {
      int numfields = classtype->codegenFieldStructure(outfile, false, "0");
      if (numfields > maxFieldsPerType) {
        maxFieldsPerType = numfields;
      }
    } else {
      typesym->type->codegenStructure(outfile, "0");
      fprintf(outfile, "{CHPL_TYPE_DONE, -1}\n");
    }
    fprintf(outfile, "}");
    num++;
  }
  fprintf(outfile, "};\n\n");

  fprintf(outfile, "size_t chpl_sizeType[] = {\n");
  num = 0;
  forv_Vec(TypeSymbol*, typesym, typesToStructurallyCodegenList) {
    if (num) {
      fprintf(outfile, ",\n");
    }
    fprintf(outfile, "%s", genSizeofStr(typesym));
    num++;
  }
  fprintf(outfile, "\n};\n\n");

  fprintf(outfile, "chplType chpl_getFieldType(int typeNum, int fieldNum) {\n");
  fprintf(outfile, "return chpl_structType[typeNum][fieldNum].type;\n");
  fprintf(outfile, "}\n\n");

  fprintf(outfile, "size_t chpl_getFieldOffset(int typeNum, int fieldNum) {\n");
  fprintf(outfile, "return chpl_structType[typeNum][fieldNum].offset;\n");
  fprintf(outfile, "}\n\n");

  fprintf(outfile, "size_t chpl_getFieldSize(int typeNum) {\n");
  fprintf(outfile, "return chpl_sizeType[typeNum];\n");
  fprintf(outfile, "}\n\n");
  closeCFile(&typeStructFile);
  fprintf(hdrfile, "#define CHPL_MAX_FIELDS_PER_TYPE %d\n", maxFieldsPerType);
  fprintf(hdrfile, "const int chpl_max_fields_per_type = %d;\n", maxFieldsPerType);
}


void codegenTypeStructureInclude(FILE* outfile) {
  fprintf(outfile, "#include \"" TYPE_STRUCTURE_FILE "\"\n");
}
