/*
 * Copyright 2004-2015 Cray Inc.
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
#include "codegen.h"
#include "expr.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "vec.h"

#ifdef HAVE_LLVM

// Functions for converting parsed Clang AST (ie C declarations)
// into Chapel. Note that these functions might create new
// UnresolvedSymExprs that need to be handled in scopeResolve.


//For naming of variadic function variables (and temporary names for void *'s).
static int query_uid = 1;


//Given a clang type, returns the corresponding chapel type (usually as 
//  an UnresolvedSymExpr to be resolved by scopeResolve).
static Expr* convertToChplType(ModuleSymbol* module, const clang::Type *type, Vec<Expr*> & results, const char* typedefName=NULL) {
  //pointers
  if (type->isPointerType()) {
    Expr* pointee = convertToChplType(module, type->getPointeeType().getTypePtr(), results);
    if (UnresolvedSymExpr* se = toUnresolvedSymExpr(pointee)) {
      const char* name = se->unresolved;
      //Pointers to c_char must be converted to Chapel's C string type.
      //Question: Should this only apply to const char*?
      if ( 0 == strcmp(name, "c_char") )
        return new UnresolvedSymExpr("c_string"); 
    }

    // void *  generates as c_void_ptr.
    if(!pointee) {
      return new UnresolvedSymExpr("c_void_ptr");
    }

    //Pointers (other than char*) are represented as calls to
    //_ddata(chapel_type).
    // PRIM_ACTUALS_LIST is not needed here.
    return new CallExpr(new UnresolvedSymExpr("c_ptr"), pointee);

  //structs
  } else if (type->isStructureType()) { 
      clang::RecordDecl *rd = type->getAsStructureType()->getDecl();
      const char* tmp_name = astr(rd->getNameAsString().c_str());
      const char* cname = tmp_name;

      if (!llvmCodegen) { 
        cname = astr("struct ", cname);
      }

      // For handling typedef struct { } bar
      //   ie an anonymous struct, use the name in the typedef.
      if( tmp_name[0] == '\0' && typedefName ) {
        cname = tmp_name = typedefName;
      }

      //don't create a new struct if it already exists
      // So we've found something.... set it as converted.
      if( ! alreadyConvertedExtern(module, cname) ) {
        setAlreadyConvertedExtern(module, cname);

        //convert the struct to Chapel
        BlockStmt* fields = new BlockStmt();

        for (clang::RecordDecl::field_iterator it = rd->field_begin(); it != rd->field_end(); ++it) {
          clang::FieldDecl* field      = (*it);
          const char*       field_name = astr(field->getNameAsString().c_str());
          Expr*             field_type = convertToChplType(module, field->getType().getTypePtr(), results);
          DefExpr*          varDefn    = new DefExpr(new VarSymbol(field_name), NULL, field_type);
          BlockStmt*        stmt       = buildChapelStmt(varDefn);
          std::set<Flag>    flags;

          fields->insertAtTail(buildVarDecls(stmt, flags, NULL));
        }

        DefExpr* strct = buildClassDefExpr(tmp_name, NULL,
                                           new AggregateType(AGGREGATE_RECORD),
                                           NULL, fields, FLAG_EXTERN, NULL);

        //...and patch up the resulting struct so that its cname is
        //  correct and codegen can find it.       
        if (strct) {
          strct->sym->cname = cname;
          results.add(strct);
        }
      }

      return new UnresolvedSymExpr(tmp_name);
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

    // handle numeric types
    
    //Unsigned types
    if (type->isSpecificBuiltinType(clang::BuiltinType::Bool))
      return new UnresolvedSymExpr("bool");
    if (type->isSpecificBuiltinType(clang::BuiltinType::Char_U))
      return new UnresolvedSymExpr("c_char");
    if (type->isSpecificBuiltinType(clang::BuiltinType::UChar))
      return new UnresolvedSymExpr("c_uchar");
    if (type->isSpecificBuiltinType(clang::BuiltinType::UShort))
      return new UnresolvedSymExpr("c_ushort");
    if (type->isSpecificBuiltinType(clang::BuiltinType::UInt))
      return new UnresolvedSymExpr("c_uint");
    if (type->isSpecificBuiltinType(clang::BuiltinType::ULong))
      return new UnresolvedSymExpr("c_ulong");
    if (type->isSpecificBuiltinType(clang::BuiltinType::ULongLong))
      return new UnresolvedSymExpr("c_ulonglong");
    //Signed types
    if (type->isSpecificBuiltinType(clang::BuiltinType::Char_S))
      return new UnresolvedSymExpr("c_char");
    if (type->isSpecificBuiltinType(clang::BuiltinType::SChar))
      return new UnresolvedSymExpr("c_schar");
    if (type->isSpecificBuiltinType(clang::BuiltinType::Short))
      return new UnresolvedSymExpr("c_short");
    if (type->isSpecificBuiltinType(clang::BuiltinType::Int))
      return new UnresolvedSymExpr("c_int");
    if (type->isSpecificBuiltinType(clang::BuiltinType::Long))
      return new UnresolvedSymExpr("c_long");
    if (type->isSpecificBuiltinType(clang::BuiltinType::LongLong))
      return new UnresolvedSymExpr("c_longlong");
    if (type->isSpecificBuiltinType(clang::BuiltinType::Float))
      return new UnresolvedSymExpr("c_float");
    if (type->isSpecificBuiltinType(clang::BuiltinType::Double))
      return new UnresolvedSymExpr("c_double");

    if (type->isVoidType()) return NULL;

  } 
  //give up...
  INT_FATAL("Unsupported type in extern \"C\" block.");
  return NULL;
}

static void convertMacroToChpl(ModuleSymbol* module, const char* name, Type* chplType, Vec<Expr*> & results) {
  if( ! module->extern_info ) return;

  VarSymbol* v = new VarSymbol(name, chplType);
  v->addFlag(FLAG_EXTERN);
  v->addFlag(FLAG_CONST);
  results.add(new DefExpr(v));
  forv_Vec(Expr*, result, results) {
    if (!result->inTree()) {
      SET_LINENO(result);
      module->block->insertAtHead(result);
    }
  }
  setAlreadyConvertedExtern(module, name);
}

void convertDeclToChpl(ModuleSymbol* module, const char* name, Vec<Expr*> & results) {
  if (name == NULL || !externC || !strcmp(".", name) || !strcmp("", name))
   return;

  //If module doesn't have an extern block, we shouldn't be here.
  if (!module->extern_info) return;

  //If we've already converted this, return immediately to
  //  avoid multiple Chapel definitions.
  if( alreadyConvertedExtern(module, name) ) return;

  clang::NamedDecl* cdecl = NULL;
  Type* chplType = NULL;

  // If we've got nothing... give up.
  if(!lookupInExternBlock(module, name, &cdecl, &chplType)) return;

  // Now, if we have no cdecl, it may be a macro.
  if( ! cdecl ) {
    convertMacroToChpl(module, name, chplType, results);
    return;
  }

  //struct
  if (clang::RecordDecl *rd = llvm::dyn_cast<clang::RecordDecl>(cdecl)) {
    results.add(convertToChplType(module, rd->getTypeForDecl(), results));
  }

  //enum constant
  if (clang::EnumConstantDecl *ed = 
      llvm::dyn_cast<clang::EnumConstantDecl>(cdecl)) {
    //results.add(convertToChplType(module, rd->getTypeForDecl(), results));
    VarSymbol* v = new VarSymbol(name);
    v->addFlag(FLAG_EXTERN);
    v->addFlag(FLAG_CONST);
    results.add(new DefExpr(v, NULL, convertToChplType(module, ed->getType().getTypePtr(), results)));
  }


  //vars
  else if (clang::VarDecl *vd = llvm::dyn_cast<clang::VarDecl>(cdecl)) {
    VarSymbol* v = new VarSymbol(name);
    v->addFlag(FLAG_EXTERN);
    results.add(new DefExpr(v, NULL, convertToChplType(module, vd->getType().getTypePtr(), results)));
  }

  //typedefs
  else if (clang::TypedefNameDecl *tdn = llvm::dyn_cast<clang::TypedefNameDecl>(cdecl)) {

    bool do_typedef = true;
    const clang::Type* contents_type = tdn->getUnderlyingType().getTypePtr();
    if( contents_type->isStructureType() ) {
      clang::RecordDecl *rd = contents_type->getAsStructureType()->getDecl();
      const char* struct_name = rd->getNameAsString().c_str();
      // We already make 'struct some_structure { .. }' create a
      // Chapel type for 'some_structure'. So if this is a typedef
      // creating an alias for 'struct some_structure' == 'some_structure',
      // just return the result of adding the structure.
      if( 0 == strcmp(name, struct_name) ) {
        convertToChplType(module, contents_type, results);
        do_typedef = false;
      }
      // If the struct is unnamed, we don't need to make
      // an alias (instead, we use the name of this typedef
      // to name the structure).
      if( struct_name[0] == '\0' ) {
        convertToChplType(module, contents_type, results, name);
        do_typedef = false;
      }
    }

    if( do_typedef ) {
      //emulate chapel's type foo = blarg; behavior
      VarSymbol* v = new VarSymbol(name);
      v->addFlag(FLAG_TYPE_VARIABLE);

      DefExpr* type_expr = new DefExpr(v, convertToChplType(module, contents_type, results, name), NULL);
      results.add(convertTypesToExtern(buildChapelStmt(type_expr)));
    }
    
  //functions
  } else if (clang::FunctionDecl *fd = llvm::dyn_cast<clang::FunctionDecl>(cdecl)) { 
#if HAVE_LLVM_VER >= 35
    clang::QualType resultType = fd->getReturnType();
#else
    clang::QualType resultType = fd->getResultType();
#endif
    FnSymbol* f = new FnSymbol(name);
    f->addFlag(FLAG_EXTERN);
    f->addFlag(FLAG_LOCAL_ARGS);
    f->addFlag(FLAG_FUNCTION_PROTOTYPE);
    Expr* chpl_type = convertToChplType(module, resultType.getTypePtr(), results);
    BlockStmt* result = buildFunctionDecl(
       f, RET_VALUE, chpl_type, NULL, NULL, NULL);

    //convert args
    for (clang::FunctionDecl::param_iterator it=fd->param_begin(); it < fd->param_end(); ++it) {
      clang::ParmVarDecl* parm = (*it);
      const char* parm_name = astr(parm->getNameAsString().c_str());
      Expr* parm_type = convertToChplType(module, parm->getType().getTypePtr(), results);
      f = buildFunctionFormal(f, buildArgDefExpr(INTENT_BLANK, parm_name, parm_type, NULL, NULL));
    }

    //handle variadic function
    if (fd->isVariadic()) {
      DefExpr* variadic = new DefExpr(new VarSymbol(astr("variadic", istr(query_uid++))));
      variadic->sym->addFlag(FLAG_PARAM);
      f = buildFunctionFormal(f, buildArgDefExpr(INTENT_BLANK, "chpl__query_c", NULL, NULL, variadic));
    }

    //Functions have to be inserted in a different location
    //  in the AST than other nodes.
    if (!result->inTree()) {
      SET_LINENO(result);
      module->block->insertAtHead(result);
    }

    //flatten scopeless block (as in cleanup.cpp)
    for_alist(stmt, result->body) {
      stmt->remove();
      result->insertBefore(stmt);
      results.add(stmt);
    }
    result->remove();
  }

  // So we've found something.... set it as converted.
  setAlreadyConvertedExtern(module, name);

  //Add results to the AST.
  forv_Vec(Expr*, result, results) {
    if (!result->inTree()) {
      SET_LINENO(result);
      module->block->insertAtHead(result);
    }
  }
}

#endif

