//
//    SYMBOL.CPP -- Code generator (C back-end)
//
//        Symbols
//
//    Copyright (c) 2004-2013, Cray Inc.
//    Copyright (c) 2013, Unicorn Enterprises SA
//

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

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
#include "type.h"
#include "expr.h"

#include "intlimits.h"

#include "genc.h"

//
//    GenC
//

// interface: codegen
      
  GenRet GenC::Symbol_codegen(Symbol *self) {
      switch (self->astTag) {
      case E_VarSymbol:
          return VarSymbol_codegen((VarSymbol *)self);
      case E_ArgSymbol:
          return ArgSymbol_codegen((ArgSymbol *)self);
      case E_TypeSymbol:
          return TypeSymbol_codegen((TypeSymbol *)self);
      case E_FnSymbol:
          return FnSymbol_codegen((FnSymbol *)self);
      default:
          {
              GenRet ret; 
              ret.c = self->cname;
              return ret;
              }
          }
      }
      
  void GenC::Symbol_codegenDef(Symbol *self) {
      switch (self->astTag) {
      case E_ModuleSymbol:
          ModuleSymbol_codegenDef((ModuleSymbol *)self);
          break;
      case E_VarSymbol:
          VarSymbol_codegenDef((VarSymbol *)self);
          break;
      case E_TypeSymbol:
          TypeSymbol_codegenDef((TypeSymbol *)self);
          break;
      case E_FnSymbol:
          FnSymbol_codegenDef((FnSymbol *)self);
          break;
      case E_EnumSymbol:
          EnumSymbol_codegenDef((EnumSymbol *)self);
          break;
      case E_LabelSymbol:
          LabelSymbol_codegenDef((LabelSymbol *)self);
          break;
      default:
          INT_FATAL(self, "Unanticipated call to Symbol::codegenDef");
          break;
          }
      }
      
  void GenC::Symbol_codegenPrototype(Symbol *self) {
      switch (self->astTag) {
      case E_TypeSymbol:
          TypeSymbol_codegenPrototype((TypeSymbol *)self);
          break;
      case E_FnSymbol:
          FnSymbol_codegenPrototype((FnSymbol *)self);
          break;
      default:
          break;
          }
      }

  void GenC::VarSymbol_codegenGlobalDef(VarSymbol *self) {
      VarSymbol_codegenDefC(self);
      }
      
  GenRet GenC::FnSymbol_codegenCast(FnSymbol *self, GenRet fnPtr) {
      GenRet fngen;
      GenRet t = FnSymbol_codegenFunctionType(self, false);
    // Cast to right function type.
      std::string str;
      str += "((";
      str += t.c;
      str  += ")";
      str += fnPtr.c;
      str += ")";
      fngen.c = str;
      return fngen;
      }

// implementation: Symbol variants

  GenRet GenC::VarSymbol_codegen(VarSymbol *self) {
      const char *cname = self->cname;
      Immediate *immediate = self->immediate;

      GenRet ret;
      ret.chplType = self->typeInfo();

      if (immediate) {
          ret.isLVPtr = GEN_VAL;
          if (immediate->const_kind == CONST_KIND_STRING) {
              ret.c += '"';
              ret.c += immediate->v_string;
              ret.c += '"';
              }
          else if (immediate->const_kind == NUM_KIND_UINT &&
                  immediate->num_index == INT_SIZE_1)
              ret.c =  immediate->uint_value() ? "true" : "false";
          else if (immediate->const_kind == NUM_KIND_INT) {
              int64_t iconst = immediate->int_value();
              if (iconst == (1ll << 63))
                  ret.c = "-INT64(9223372036854775807) - INT64(1)";
              else if (iconst <= -2147483648ll || iconst >= 2147483647ll)
                  ret.c = "INT64(" + int64_to_string(iconst) + ")";
              else
                  ret.c = int64_to_string(iconst);
              }
          else if (immediate->const_kind == NUM_KIND_UINT) {
              uint64_t uconst = immediate->uint_value();
              if (uconst <= (uint64_t) INT32_MAX)
                  ret.c = uint64_to_string(uconst);
              else
                  ret.c = "UINT64(" + uint64_to_string(uconst) + ")";
              }
          else
              ret.c = cname; // in C, all floating point literals are (double)
          }
      else {
        // not immediate
          ret.c = '&';
          ret.c += cname;
          ret.isLVPtr = GEN_PTR;
          }
      return ret;
      }
      
  void GenC::VarSymbol_codegenDef(VarSymbol *self) {
      VarSymbol_codegenDefC(self);
      }
      
  void GenC::VarSymbol_codegenDefC(VarSymbol *self) {
      const char *cname = self->cname;
      Type *type = self->type;
      DefExpr *defPoint = self->defPoint;

      if (self->hasFlag(FLAG_EXTERN))
          return;

      if (type == dtVoid)
          return;
      ClassType *ct = toClassType(type);
      std::string typestr =  
          (self->hasFlag(FLAG_SUPER_CLASS) ?
              std::string(ct->classStructName(true)) :
              Type_codegen(type).c);
      std::string str = typestr + " " + cname;
      if (ct) {
          if (ct->classTag == CLASS_CLASS) {
              if (isFnSymbol(defPoint->parentSymbol))
                  str += " = NULL";  
              }
          else if (ct->symbol->hasFlag(FLAG_WIDE) ||
                  ct->symbol->hasFlag(FLAG_WIDE_CLASS)) {
              if (isFnSymbol(defPoint->parentSymbol))
                  str += " = {{0,0},NULL}";
              }
          }
      writeLocalDecl(str);
      }

  GenRet GenC::ArgSymbol_codegen(ArgSymbol *self) {
      const char *cname = self->cname;

      GenRet ret;

      ret.c = '&';
      ret.c += cname;
      ret.isLVPtr = GEN_PTR;

      if (self->requiresCPtr()) {
      // Don't try to use chplType.
          ret.chplType = NULL;
          ret = codegenLocalDeref(ret);
          }

      ret.chplType = self->typeInfo();

      return ret;
      }
      
  GenRet GenC::ArgSymbol_codegenType(ArgSymbol *self) {
      Type *type = self->type;

      GenRet ret;
      ret.c = Type_codegen(type).c;
      if (self->requiresCPtr())
          ret.c += "* const";
      return ret;
      }

  GenRet GenC::TypeSymbol_codegen(TypeSymbol *self) {
      const char *cname = self->cname;

      GenRet ret;
      ret.chplType = self->typeInfo();
      ret.c = cname;
      return ret;
      }
      
  void GenC::TypeSymbol_codegenDef(TypeSymbol *self) {
      const char *cname = self->cname;
      Type *type = self->type;

      if (breakOnCodegenCname[0] &&
              !strcmp(cname, breakOnCodegenCname))
          gdbShouldBreakHere();

      if (!self->hasFlag(FLAG_EXTERN))
          Type_codegenDef(type);

      self->codegenned = true;

    // no action required.
      }
      
  void GenC::TypeSymbol_codegenPrototype(TypeSymbol *self) {
      Type *type = self->type;

      if (!self->hasFlag(FLAG_EXTERN))
          Type_codegenPrototype(type);
      }
      
  GenRet GenC::FnSymbol_codegen(FnSymbol *self) {
      const char *cname = self->cname;

      GenRet ret;
      ret.c = cname;
      return ret;
      }
      
  void GenC::FnSymbol_codegenDef(FnSymbol *self) {
      const char *cname = self->cname;
      BlockStmt *body = self->body;
  
      if (breakOnCodegenCname[0] &&
              !strcmp(cname, breakOnCodegenCname))
          gdbShouldBreakHere();

      if (self->hasFlag(FLAG_NO_CODEGEN)) 
          return;

      clearStatements();
      clearLocalDecls();

      if (strcmp(saveCDir, "")) {
          if (const char *rawname = self->fname()) {
              const char *name = strrchr(rawname, '/');
              name = name ? name + 1 : rawname;
              writeC("/* %s:%d */\n", name, self->linenum());
              }
          }

      FnSymbol_codegenHeaderC(self);

      writeC(" {\n");

      if (fNoRepositionDefExpr) {
          Vec<BaseAST*> asts;
          collect_top_asts(body, asts);

          forv_Vec(BaseAST, ast, asts) {
              if (DefExpr *def = toDefExpr(ast))
                  if (!toTypeSymbol(def->sym)) {
                      if (fGenIDS && isVarSymbol(def->sym))
                          genIdComment(def->sym->id);
                      Symbol_codegenDef(def->sym);
                      flushStatements();
                      }
              }
          }

      Expr_codegen(body);
      flushStatements();

      writeC("}\n\n");
      }
      
  void GenC::FnSymbol_codegenPrototype(FnSymbol *self) {
      const char *cname = self->cname;

      if (self->hasFlag(FLAG_EXTERN) && !genExternPrototypes) 
          return;
      if (self->hasFlag(FLAG_NO_PROTOTYPE)) 
          return;
      if (self->hasFlag(FLAG_NO_CODEGEN)) 
          return;

      if (breakOnCodegenCname[0] &&
              !strcmp(cname, breakOnCodegenCname))
          gdbShouldBreakHere();

    // In C, we don't need to generate prototypes for external
    // functions, since these prototypes will presumably be
    // present in some C header file.
      FnSymbol_codegenHeaderC(self);
      writeC(";\n");
      }
      
  void GenC::FnSymbol_codegenHeaderC(FnSymbol *self) {
      int id = self->id;

      if (fGenIDS)
          writeC("/* %7d */ ", id);
    // Prepend function header with necessary __global__ declaration
      if (self->hasFlag(FLAG_GPU_ON))
          writeC("__global__ ");

      writeC("%s", FnSymbol_codegenFunctionType(self, true).c.c_str());
      }

// forHeader == true when generating the C header.
  GenRet GenC::
          FnSymbol_codegenFunctionType(FnSymbol *self, bool forHeader) {
      const char *cname = self->cname;
      Type *retType = self->retType;
      
      GenRet ret;

      ret.chplType = self->typeInfo();
      INT_ASSERT(ret.chplType == dtUnknown); //just documenting the current state

    // Cast to right function type.
      std::string str;
      str += Type_codegen(retType).c.c_str();
      if (forHeader) {
          str += " ";
          str += cname;
          }
      else 
          str += "(*)";
      str += "(";
      if (self->numFormals() == 0)
          str += "void";
      else {
          int count = 0;
          for_formals(formal, self) {
              if (formal->defPoint == self->formals.head && 
                      self->hasFlag(FLAG_ON_BLOCK))
                  continue; // do not print locale argument for on blocks
              if (self->hasFlag(FLAG_GPU_ON) && count < 2) {
                  count++;
                  continue; // do not print nBlocks and numThreadsPerBlock
                  }
              if (count > 0)
                  str += ", ";
              str += ArgSymbol_codegenType(formal).c;
              if (forHeader) {
                  str += " ";
                  str += formal->cname;
                  }
              count++;
              }
          }
      str += ")";
      ret.c = str;
      return ret;
      }

  void GenC::EnumSymbol_codegenDef(EnumSymbol *self) { }
      
  void GenC::ModuleSymbol_codegenDef(ModuleSymbol *self) {
      BlockStmt *block = self->block;

    // ACHTUNG: GPU support removed
      setInfoFilename(self->fname());
      setInfoLineno(self->linenum());
      clearStatements();
      clearLocalDecls();

      Vec<FnSymbol*> fns;
      for_alist(expr, block->body) {
          if (DefExpr *def = toDefExpr(expr))
              if (FnSymbol *fn = toFnSymbol(def->sym)) {
                // Ignore external and prototype functions.
                  if (fn->hasFlag(FLAG_EXTERN) ||
                          fn->hasFlag(FLAG_FUNCTION_PROTOTYPE))
                      continue;
                  fns.add(fn);
                  }
          }
      qsort(fns.v, fns.n, sizeof(fns.v[0]), compareLineno);
      forv_Vec(FnSymbol, fn, fns) {
          FnSymbol_codegenDef(fn);
          }
      flushStatements();
      }
      
  void GenC::LabelSymbol_codegenDef(LabelSymbol *self) { }

// implementation: helpers

  int GenC::compareLineno(const void *v1, const void *v2) {
      FnSymbol *fn1 = *(FnSymbol **)v1;
      FnSymbol *fn2 = *(FnSymbol **)v2;
      if (fn1->linenum() > fn2->linenum())
          return 1;
      else if (fn1->linenum() < fn2->linenum())
          return -1;
      else
          return 0;
      }

