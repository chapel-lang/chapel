/*
 * Copyright 2020-2026 Hewlett Packard Enterprise Development LP
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

#include "externCResolve.h"

#include "astutil.h"
#include "build.h"
#include "buildDefaultFunctions.h"
#include "clangUtil.h"
#include "codegen.h"
#include "driver.h"
#include "expr.h"
#include "passes.h"
#include "resolution.h"
#include "resolveFunction.h"
#include "scopeResolve.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "vec.h"
#include "global-ast-vecs.h"

#include <map>
#include <set>
#include <utility>

#ifdef HAVE_LLVM
// clang headers
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Type.h"
#include "clang/Lex/MacroInfo.h"
#include "llvm/ADT/SmallSet.h"

// Functions for converting parsed Clang AST (ie C declarations)
// into Chapel. Note that these functions might create new
// UnresolvedSymExprs that need to be handled in scopeResolve.


//For naming of variadic function variables (and temporary names for void *'s).
static int query_uid = 1;

struct CImportScope {
  std::map<const char*, Symbol*> symbols;
  std::vector<DefExpr*> pending;
};

static std::map<ModuleSymbol*, CImportScope, AstIdLess> cImportScopes;

static Symbol* lookupCImport(ModuleSymbol* module, const char* name,
                            std::set<ModuleSymbol*, AstIdLess>& visited) {
  if (!visited.insert(module).second)
    return nullptr;
  auto scope = cImportScopes.find(module);
  if (scope != cImportScopes.end()) {
    auto symbol = scope->second.symbols.find(astr(name));
    if (symbol != scope->second.symbols.end())
      return symbol->second;
  }
  for (auto used: module->modUseList)
    if (auto symbol = lookupCImport(used, name, visited))
      return symbol;
  return nullptr;
}

static Symbol* lookupCImport(ModuleSymbol* module, const char* name) {
  name = astr(name);
  if (!normalized) {
    int ignoredCount = 0;
    auto symbol = lookupInModuleOrBuiltins(module, name, ignoredCount);
    if (symbol)
      cImportScopes[module].symbols[name] = symbol;
    return symbol;
  }
  std::set<ModuleSymbol*, AstIdLess> visited;
  return lookupCImport(module, name, visited);
}

void prepareExternMacroTypes() {
  bool needed = false;
  forv_Vec(FnSymbol, fn, gFnSymbols)
    if (fn->inTree() && fn->hasFlag(FLAG_EXTERN_MACRO))
      needed = true;
  if (!needed) {
    cImportScopes.clear();
    return;
  }
  // Scope tables do not survive into resolution. Retain the type symbols the
  // shared C importer needs when a concrete macro signature becomes available.
  static const char* names[] = {
    "bool", "c_char", "c_schar", "c_uchar", "c_short", "c_ushort",
    "c_int", "c_uint", "c_long", "c_ulong", "c_longlong", "c_ulonglong",
    "c_float", "c_double", "c_ptr", "c_ptrConst", "c_array", "c_fn_ptr"
  };
  forv_Vec(ModuleSymbol, module, gModuleSymbols) {
    if (!module->inTree())
      continue;
    auto& symbols = cImportScopes[module].symbols;
    for_alist(expr, module->block->body) {
      if (auto def = toDefExpr(expr))
        if (isTypeSymbol(def->sym) || def->sym->hasFlag(FLAG_TYPE_VARIABLE))
          symbols[def->sym->name] = def->sym;
    }
    if (module->extern_info)
      for (auto name : names)
        lookupCImport(module, name);
  }
}

void cleanupExternMacroTypes() {
  cImportScopes.clear();
}


static void addCDef(ModuleSymbol* module, DefExpr* def);

static Expr* tryCResolveExpr(ModuleSymbol* module, const char* name);
static Expr* lookupExpr(ModuleSymbol* module, const char* name);

static const char* convertTypedef(ModuleSymbol* module,
                                  clang::TypedefNameDecl* tdn);


static Expr* convertToChplType(ModuleSymbol* module,
                               const clang::Type *type,
                               const char* typedefName=NULL);

static Expr* convertTypedefToChplType(ModuleSymbol* module,
                                      const clang::TypedefType *td,
                                      const char* typedefName=NULL) {

  // Get the typedef decl for that
  clang::TypedefNameDecl* tdn = td->getDecl();

  const char* typedef_name = convertTypedef(module, tdn);

  return tryCResolveExpr(module, typedef_name);
}

static Expr* convertPointerToChplType(ModuleSymbol* module,
                                      const clang::QualType pointeeType,
                                      const char* typedefName=NULL) {



  // Pointers to C functions become c_fn_ptr
  if (pointeeType.getTypePtr()->isFunctionType()) {
    return tryCResolveExpr(module, "c_fn_ptr");
  }

  Expr* pointee = convertToChplType(module, pointeeType.getTypePtr());

  // Other pointers are represented as a call to c_ptr or c_ptrConst.
  if (pointeeType.isConstQualified()) {
    return new CallExpr(tryCResolveExpr(module, "c_ptrConst"), pointee);
  } else {
    return new CallExpr(tryCResolveExpr(module, "c_ptr"), pointee);
  }
}

static unsigned getMinSignedBits(const llvm::APInt& size) {
#if HAVE_LLVM_VER >= 170
  return size.getSignificantBits();
#else
  return size.getMinSignedBits();
#endif
}

static
Expr* convertFixedSizeArrayToChplType(ModuleSymbol* module,
                                      const clang::ConstantArrayType* arrayType,
                                      const char* typedefName=NULL) {

  llvm::APInt size = arrayType->getSize();
  clang::QualType eltType = arrayType->getElementType();

  Expr* eltTypeChapel = convertToChplType(module, eltType.getTypePtr());

  if (getMinSignedBits(size) > 64)
    USR_FATAL("C array is too large");

  int64_t isize = size.getSExtValue();
  Symbol* isym = new_IntSymbol(isize, INT_SIZE_64);

  return new CallExpr(tryCResolveExpr(module, "c_array"),
                      eltTypeChapel, new SymExpr(isym));
}

static
Expr* convertArrayToChplType(ModuleSymbol* module,
                             const clang::ArrayType* arrayType,
                             const char* typedefName=NULL) {

  clang::QualType eltType = arrayType->getElementType();

  Expr* eltTypeChapel = convertToChplType(module, eltType.getTypePtr());

  // For now, just represent it as a c_ptr
  if (eltType.isConstQualified()) {
    return new CallExpr(tryCResolveExpr(module, "c_ptrConst"), eltTypeChapel);
  } else {
    return new CallExpr(tryCResolveExpr(module, "c_ptr"), eltTypeChapel);
  }
}

static
Expr* convertStructToChplType(ModuleSymbol* module,
                              const clang::RecordType* structType,
                              const char* typedefName=NULL) {

  clang::RecordDecl *rd = structType->getDecl();
  const char* chpl_name = astr(rd->getNameAsString().c_str());
  const char* cname = chpl_name;

  if (!fLlvmCodegen) {
    if (structType->isUnionType())
      cname = astr("union ", cname);
    else
      cname = astr("struct ", cname);
  }

  if (chpl_name[0] == '\0') {
    if (typedefName) {
      // For handling typedef struct { } bar
      //   ie an anonymous struct, use the name in the typedef.
      chpl_name = typedefName;
    } else {
      // and for other ones, create a name
      chpl_name = getGeneratedAnonTypeName(structType);
    }
    cname = chpl_name;
  }

  // Don't convert it if it's already converted
  if (Symbol* sym = lookupCImport(module, chpl_name))
    return new SymExpr(sym);

  // Create an empty struct and add it to the AST
  // This allows fields of struct* type or other recursion.

  AggregateTag tag = AGGREGATE_RECORD;
  if (structType->isUnionType())
    tag = AGGREGATE_UNION;

  AggregateType* ct = new AggregateType(tag);
  ct->defaultValue = NULL;
  TypeSymbol* ts = new TypeSymbol(chpl_name, ct);
  ts->cname = cname;
  ts->addFlag(FLAG_EXTERN);
  if (structType->isIncompleteType())
    ts->addFlag(FLAG_INCOMPLETE);
  DefExpr* def = new DefExpr(ts);

  addCDef(module, def);

  Expr* ret = lookupExpr(module, chpl_name);
  INT_ASSERT(ret != NULL);

  // Convert the fields
  BlockStmt* fields = new BlockStmt();

  for (clang::RecordDecl::field_iterator it = rd->field_begin();
       it != rd->field_end();
       ++it) {
    clang::FieldDecl* field      = (*it);
    const clang::Type* type = field->getType().getTypePtr();

    const char* field_name = astr(field->getNameAsString().c_str());
    Expr* field_type = convertToChplType(module, type);
    VarSymbol* varSym = new VarSymbol(field_name);
    DefExpr* varDefn = new DefExpr(varSym, NULL, field_type);

    if (type->isArrayType() && !type->isConstantArrayType()) {
      varSym->addFlag(FLAG_C_FLEXIBLE_ARRAY_FIELD);
    }

    BlockStmt* stmt = buildChapelStmt(varDefn);

    fields->insertAtTail(buildVarDecls(stmt));
  }

  // Add the fields to the struct
  ct->addDeclarations(fields);

  ct->processGenericFields();

  return ret;
}

// Given a clang type, returns the corresponding chapel type.
// Returns e.g. a SymExpr(TypeSymbol) or a CallExpr to a type constructor.
static Expr* convertToChplType(ModuleSymbol* module,
                               const clang::Type *type,
                               const char* typedefName) {

  //typedefs
  if (auto td = type->getAs<clang::TypedefType>()) {

    return convertTypedefToChplType(module, td, typedefName);

  //pointers
  } else if (type->isPointerType()) {

    clang::QualType pointeeType = type->getPointeeType();

    return convertPointerToChplType(module, pointeeType, typedefName);

  //arrays
  } else if (type->isArrayType()) {
    // TODO: `getAsArrayTypeUnsafe` is required to desugar types. A better
    // solution which doesn't discard qualifiers would be
    // `Ctx.getAsConstantArrayType(type)` where `Ctx` is a `clang::AstContext`,
    // however that would require some heavy rewrites
    const clang::ArrayType* at = type->getAsArrayTypeUnsafe();

    if (type->isConstantArrayType()) {
      const clang::ConstantArrayType* cat =
        llvm::dyn_cast<clang::ConstantArrayType>(at);

      return convertFixedSizeArrayToChplType(module, cat, typedefName);
    } else {

      return convertArrayToChplType(module, at, typedefName);
    }

  //structs
  } else if (type->isStructureType()) {

    return convertStructToChplType(module, type->getAsStructureType(),
                                   typedefName);

  // unions
  } else if (type->isUnionType()) {

    return convertStructToChplType(module, type->getAsUnionType(),
                                   typedefName);

  } else if (type->isFunctionType()) {
    // This should be handled in the pointer-to-function case above
    USR_FATAL("C function types (vs pointers to them) not yet supported");

  // complex
  } else if (type->isComplexType()) {
    const clang::ComplexType* ct = llvm::dyn_cast<clang::ComplexType>(type);

    clang::QualType eltTypeQ = ct->getElementType();
    const clang::Type* eltType = eltTypeQ.getTypePtr();

    Type* chapelType = NULL;
    if (eltType->isSpecificBuiltinType(clang::BuiltinType::Float))
      chapelType = dtComplex[COMPLEX_SIZE_64];
    else if (eltType->isSpecificBuiltinType(clang::BuiltinType::Double))
      chapelType = dtComplex[COMPLEX_SIZE_128];
    else
      USR_FATAL("Unsupported complex type in extern \"C\" block.");

    return new SymExpr(chapelType->symbol);
  } else {
    // Check for enum types, which are really some sort of integer type
    if (type->isEnumeralType()) {
      clang::QualType qType = type->getCanonicalTypeInternal();
      const clang::Type* cType = qType.getTypePtrOrNull();
      const clang::EnumType* e = llvm::dyn_cast<clang::EnumType>(cType);
      clang::EnumDecl* ed = e->getDecl()->getCanonicalDecl();
      clang::QualType iType = ed->getCanonicalDecl()->getIntegerType();
      type = iType.getTypePtrOrNull();
      INT_ASSERT(type && "Could not get enum integer type pointer");
    }

    if (type->isVoidType()) {
      return new SymExpr(dtVoid->symbol);
    }

    // handle numeric types

    //Unsigned types
    const char* t = NULL;
    if (type->isSpecificBuiltinType(clang::BuiltinType::Bool))
      t = "bool";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::Char_U))
      t = "c_char";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::UChar))
      t = "c_uchar";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::UShort))
      t = "c_ushort";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::UInt))
      t = "c_uint";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::ULong))
      t = "c_ulong";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::ULongLong))
      t = "c_ulonglong";
    //Signed types
    else if (type->isSpecificBuiltinType(clang::BuiltinType::Char_S))
      t = "c_char";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::SChar))
      t = "c_schar";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::Short))
      t = "c_short";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::Int))
      t = "c_int";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::Long))
      t = "c_long";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::LongLong))
      t = "c_longlong";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::Float))
      t = "c_float";
    else if (type->isSpecificBuiltinType(clang::BuiltinType::Double))
      t = "c_double";

    if (t != NULL)
      return lookupExpr(module, t);
  }

  //give up...
  USR_FATAL("Unsupported type in extern \"C\" block.");
  return NULL;
}

static void convertMacroToChpl(ModuleSymbol* module,
                               const char*   name,
                               Type*         chplType,
                               Expr*         chplTypeExpr,
                               bool          writable) {
  INT_ASSERT(module->extern_info);

  VarSymbol* v = NULL;
  if (chplType)
    v = new VarSymbol(name, chplType);
  else
    v = new VarSymbol(name);

  v->addFlag(FLAG_EXTERN);
  if (!writable)
    v->addFlag(FLAG_CONST);

  DefExpr* def = NULL;
  if (chplTypeExpr)
    def = new DefExpr(v, NULL, chplTypeExpr);
  else
    def = new DefExpr(v);

  addCDef(module, def);
}

static const char* convertTypedef(ModuleSymbol*           module,
                                  clang::TypedefNameDecl* tdn) {
  //handle typedefs

  bool do_typedef = true;
  const char* typedef_name = astr(tdn->getNameAsString().c_str());
  const clang::Type* contents_type = tdn->getUnderlyingType().getTypePtr();

  if (contents_type->isStructureType()) {
    clang::RecordDecl *rd = contents_type->getAsStructureType()->getDecl();
    const char* struct_name = astr(rd->getNameAsString().c_str());
    // We already make 'struct some_structure { .. }' create a
    // Chapel type for 'some_structure'. So if this is a typedef
    // creating an alias for 'struct some_structure' == 'some_structure',
    // just return the result of adding the structure.
    if( typedef_name == struct_name ) {
      convertToChplType(module, contents_type);
      do_typedef = false;
    }
    // If the struct is unnamed, we don't need to make
    // an alias (instead, we use the name of this typedef
    // to name the structure by passing the typedefName
    // argument to convertToChplType).
    if( struct_name[0] == '\0' ) {
      convertToChplType(module, contents_type, typedef_name);
      do_typedef = false;
    }
  }

  if( do_typedef ) {
    // Don't convert it if it's already converted
    if (lookupCImport(module, typedef_name))
      return typedef_name;

    // Create a a DefExpr without the initializing expression
    // (we'll fill that in later) to allow for recursive uses of types.
    VarSymbol* v = new VarSymbol(typedef_name);
    v->addFlag(FLAG_TYPE_VARIABLE);
    v->addFlag(FLAG_EXTERN);
    Expr* dummyInitExpr = new CallExpr(PRIM_NOOP);
    DefExpr* def = new DefExpr(v, dummyInitExpr, NULL);

    // Add the def before converting the type do base it on
    addCDef(module, def);

    // Determine the type to base it on
    Expr* initExpr = convertToChplType(module, contents_type, typedef_name);
    dummyInitExpr->replace(initExpr);
  }

  return typedef_name;
}



// Returns a DefExpr already added to the AST that defines
// the C object with name cname
static
void convertDeclToChpl(ModuleSymbol* module,
                       const char*   cname) {
  INT_ASSERT(cname != NULL);
  INT_ASSERT(fAllowExternC);
  INT_ASSERT(cname != astrSdot);
  INT_ASSERT(cname[0] != '\0');
  INT_ASSERT(module != NULL);
  INT_ASSERT(module->extern_info != NULL);

  // Don't convert it if it's already converted
  if (lookupCImport(module, cname))
    return;

  if (auto macro = getFunctionMacroInExternBlock(module, cname)) {
    if (macro->isVariadic())
      USR_FATAL("variadic C macro '%s' is not supported", cname);
    auto fn = new FnSymbol(cname);
    fn->addFlag(FLAG_EXTERN);
    fn->addFlag(FLAG_LOCAL_ARGS);
    fn->addFlag(FLAG_EXTERN_MACRO);
    for (unsigned i = 0; i < macro->getNumParams(); i++) {
      const char* name = astr(macro->params()[i]->getName().str().c_str());
      fn->insertFormalAtTail(new ArgSymbol(INTENT_IN, name, dtAny));
    }
    auto declaration = buildFunctionDecl(fn, RET_VALUE, nullptr, false,
                                          nullptr, nullptr, nullptr);
    auto def = toDefExpr(declaration->body.first());
    def->remove();
    addCDef(module, def);
    return;
  }

  clang::TypeDecl* cType = NULL;
  clang::ValueDecl* cValue = NULL;
  const char* cCastedToType = NULL;
  Type* chplType = NULL;
  astlocT astloc(0, NULL);

  bool got = lookupInExternBlock(module, cname,
                                 &cType, &cValue, &cCastedToType,
                                 &chplType, &astloc);

  // If we've got nothing... give up.
  if (got == false)
    return;

  // use currentAstLoc if astloc was empty
  if (astloc.lineno() == 0)
    astloc = currentAstLoc;
  // Use the astloc from lookup if possible
  astlocMarker markAstLoc(astloc);

  // Now, if we have no cdecl, it may be a macro.
  if( (!cType) && (!cValue) ) {
    Expr* chplTypeExpr = NULL;
    if (auto getter = getMacroGetterInExternBlock(module, cname)) {
      chplType = NULL;
      chplTypeExpr =
          convertToChplType(module, getter->getReturnType().getTypePtr());
    } else if (cCastedToType) {
      // If the macro contained a cast, replace the Chapel type
      // with a type expression.
      chplType = NULL;
      chplTypeExpr = tryCResolveExpr(module, cCastedToType);
    }

    convertMacroToChpl(module, cname, chplType, chplTypeExpr,
                       isMacroLvalueInExternBlock(module, cname));
    return;
  }

  //struct
  if (clang::RecordDecl *rd =
      llvm::dyn_cast_or_null<clang::RecordDecl>(cType)) {
    auto& ctx = rd->getASTContext();
    convertToChplType(module, getClangASTType(ctx, rd));
  }

  //enum constant
  if (clang::EnumConstantDecl *ed =
      llvm::dyn_cast_or_null<clang::EnumConstantDecl>(cValue)) {
    VarSymbol* v = new VarSymbol(cname);
    v->addFlag(FLAG_EXTERN);
    v->addFlag(FLAG_CONST);
    Expr* t = convertToChplType(module, ed->getType().getTypePtr());
    DefExpr* def = new DefExpr(v, NULL, t);
    addCDef(module, def);
  }


  //vars
  if (clang::VarDecl *vd =
      llvm::dyn_cast_or_null<clang::VarDecl>(cValue)) {
    VarSymbol* v = new VarSymbol(cname);
    v->addFlag(FLAG_EXTERN);
    Expr* t = convertToChplType(module, vd->getType().getTypePtr());
    DefExpr* def = new DefExpr(v, NULL, t);
    addCDef(module, def);
  }

  //typedefs
  if (clang::TypedefNameDecl *tdn =
      llvm::dyn_cast_or_null<clang::TypedefNameDecl>(cType)) {
    convertTypedef(module, tdn);
  }

  //functions
  if (clang::FunctionDecl *fd =
      llvm::dyn_cast_or_null<clang::FunctionDecl>(cValue)) {
    clang::QualType resultType = fd->getReturnType();
    FnSymbol* f = new FnSymbol(cname);
    f->addFlag(FLAG_EXTERN);
    f->addFlag(FLAG_LOCAL_ARGS);
    Expr* retT = convertToChplType(module, resultType.getTypePtr());
    BlockStmt* result = buildFunctionDecl( f, // fn
                                           RET_VALUE, // retTag
                                           retT, // ret type
                                           false,  // throws
                                           NULL, // where
                                           NULL, // lifetime constraints
                                           NULL // body
                                         );

    //convert args
    for (clang::FunctionDecl::param_iterator it=fd->param_begin(); it < fd->param_end(); ++it) {
      clang::ParmVarDecl* parm = (*it);
      const char* parm_name = astr(parm->getNameAsString().c_str());
      const clang::Type* parmCType = parm->getType().getTypePtr();
      Expr* parmChapelType = convertToChplType(module, parmCType);
      IntentTag intent = INTENT_IN;
      if (parmCType->isArrayType())
        intent = INTENT_REF;
      f = buildFunctionFormal(f,
                              buildArgDefExpr(intent, parm_name,
                                              parmChapelType, NULL, NULL));
    }

    //handle variadic function
    if (fd->isVariadic()) {
      DefExpr* variadic = new DefExpr(new VarSymbol(astr("variadic", istr(query_uid++))));
      variadic->sym->addFlag(FLAG_PARAM);
      f = buildFunctionFormal(f, buildArgDefExpr(INTENT_BLANK, "chpl__query_c", NULL, NULL, variadic));
    }

    DefExpr* def = toDefExpr(result->body.first());
    INT_ASSERT(def);
    def->remove();

    addCDef(module, def);
  }
}

Symbol* tryCResolveLocally(ModuleSymbol* module, const char* name) {
  // Is it resolveable in this module?
  if (module->extern_info != NULL) {
    // Convert it and update the scope table
    convertDeclToChpl(module, name);
    // Look it up again using the scope table
    return lookupCImport(module, name);
  }

  return NULL;
}

static Symbol* doTryCResolve(ModuleSymbol* module,
                             const char* name,
                             llvm::SmallSet<ModuleSymbol*, 24, AstIdLess> &visited) {

  if (module == NULL) {
    return NULL;

  } else if (visited.insert(module).second) {
    // we added it to the set, so continue.

  } else {
    // It was already in the set.
    return NULL;
  }

  // Try the modules used by this module.
  // It is important to consider them first, so that a module that is 'use'd
  // that has an extern block will generate the Defs there, rather than
  // getting another copy here.
  // TODO: handle only, except, etc
  for (ModuleSymbol* usedMod : module->modUseList) {
    if (usedMod->modTag == MOD_USER) { // no extern blocks in internal code
      Symbol* got = doTryCResolve(usedMod, name, visited);
      if (got != NULL)
        return got;
    }
  }

  return tryCResolveLocally(module, name);
}

Symbol* tryCResolve(ModuleSymbol* mod, const char* name) {
  Symbol* retval = NULL;

  // Try looking up the symbol with scope resolve's tables.
  // This covers the case of finding a symbol already converted.
  int nSymbolsFound = 0;
  Symbol* got = lookupInModuleOrBuiltins(mod, name, nSymbolsFound);
  if (nSymbolsFound != 0)
    return got;

  if (fAllowExternC == true) {
    llvm::SmallSet<ModuleSymbol*, 24, AstIdLess> visited;

    retval = doTryCResolve(mod, name, visited);
  }

  return retval;
}

static void addCDef(ModuleSymbol* module, DefExpr* def) {
  if (normalized && isVarSymbol(def->sym))
    module->initFn->insertAtHead(def);
  else
    module->block->insertAtHead(def);
  cImportScopes[module].symbols[def->sym->name] = def->sym;
  if (normalized)
    cImportScopes[module].pending.push_back(def);
  else
    addToSymbolTable(def);
}

static Expr* tryCResolveExpr(ModuleSymbol* module, const char* name) {
  Symbol* sym = tryCResolveLocally(module, astr(name));
  if (sym == NULL)
    USR_FATAL("Could not find C type %s", name);

  return new SymExpr(sym);
}

static Expr* lookupExpr(ModuleSymbol* module, const char* name) {
  Symbol* sym = lookupCImport(module, name);
  if (sym == NULL)
    USR_FATAL("Could not find C type %s", name);

  return new SymExpr(sym);
}

void resolveExternMacroReturnType(FnSymbol* fn) {
  SET_LINENO(fn);
  auto module = fn->getModule();
  auto wrapper = resolveMacroFunctionInExternBlock(fn);
  INT_ASSERT(wrapper);
  Expr* type = convertToChplType(module, wrapper->getReturnType().getTypePtr());
  fn->retExprType = new BlockStmt(type, BLOCK_TYPE);
  insert_help(fn->retExprType, nullptr, fn);

  auto pending = std::move(cImportScopes[module].pending);
  std::vector<TypeSymbol*> types;
  for (auto def : pending) {
    if (auto ts = toTypeSymbol(def->sym)) {
      normalize(def);
      types.push_back(ts);
    }
  }
  buildDefaultFunctionsForTypes(types);
  for (auto def : pending) {
    if (auto alias = toVarSymbol(def->sym)) {
      INT_ASSERT(alias->hasFlag(FLAG_TYPE_VARIABLE));
      auto use = new SymExpr(alias);
      fn->retExprType->insertAtHead(use);
      alias->type = resolveTypeAlias(use);
      use->remove();
      if (def->init)
        def->init->remove();
    }
  }
  normalize(fn->retExprType);
  resolveSpecifiedReturnType(fn);
}

#endif
