//
//    SYMBOL.CPP -- Code generator (IR back-end)
//
//        Symbols
//
//    Copyright (c) 2004-2013, Cray Inc.
//    Copyright (c) 2013, Unicorn Enterprises SA
//

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

#include "irbase.h"
#include "genir.h"

//
//    GenIR
//

// interface: codegen
      
  GenRet GenIR::Symbol_codegen(Symbol *self) {
      switch (self->astTag) {
      case E_VarSymbol:
          return VarSymbol_codegen((VarSymbol *)self);
      case E_ArgSymbol:
          return ArgSymbol_codegen((ArgSymbol *)self);
      case E_TypeSymbol:
          return TypeSymbol_codegen((TypeSymbol *)self);
      case E_FnSymbol:
          return FnSymbol_codegen((FnSymbol *)self);
      case E_EnumSymbol:
          return EnumSymbol_codegen((EnumSymbol *)self);
      case E_ModuleSymbol:
          return ModuleSymbol_codegen((ModuleSymbol *)self);
      case E_LabelSymbol:
          return LabelSymbol_codegen((LabelSymbol *)self);
      default:
          {
              GenRet ret;
              INT_FATAL(self, "Unanticipated call to Symbol::codegen");
              return ret;
              }
          }
      }
      
  void GenIR::Symbol_codegenDef(Symbol *self) {
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
      
  void GenIR::Symbol_codegenPrototype(Symbol *self) {
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

  void GenIR::VarSymbol_codegenGlobalDef(VarSymbol *self) {
      VarSymbol_codegenDefC(self);
      }
      
  GenRet GenIR::FnSymbol_codegenCast(FnSymbol *self, GenRet fnPtr) {
      GenRet fngen;
      GenRet t = FnSymbol_codegenFunctionType(self);
    // Cast to right function type.
      fngen.val = ir->genCast(t.type, fnPtr.val);
      return fngen;
      }

// implementation: Symbol variants

  GenRet GenIR::VarSymbol_codegen(VarSymbol *self) {
      GenRet ret;
      ret.chplType = self->typeInfo();

      Immediate *immediate = self->immediate;
      if (immediate) {
          ret.isLVPtr = GEN_VAL;
          switch (immediate->const_kind) {
          case CONST_KIND_STRING:
              ret.val = ir->genString(immediate->v_string);
              break;
          case NUM_KIND_INT:
              switch (immediate->num_index) {
              case INT_SIZE_1:
                  ret.val = ir->genBool(immediate->v_bool != 0);
                  break;
              case INT_SIZE_8:
                  ret.val = ir->genInt8(immediate->v_int8);
                  break;
              case INT_SIZE_16:
                  ret.val = ir->genInt16(immediate->v_int16);
                  break;
              case INT_SIZE_32:
                  ret.val = ir->genInt32(immediate->v_int32);
                  break;
              case INT_SIZE_64:
                  ret.val = ir->genInt64(immediate->v_int64);
                  break;
              default:
                  INT_FATAL(self, "Unsupported int size");
                  break;
                  }
              break;
          case NUM_KIND_UINT:
              switch (immediate->num_index) {
              case INT_SIZE_1:
                  ret.val = ir->genBool(immediate->v_bool != 0);
                  break;
              case INT_SIZE_8:
                  ret.val = ir->genUint8(immediate->v_uint8);
                  break;
              case INT_SIZE_16:
                  ret.val = ir->genUint16(immediate->v_uint16);
                  break;
              case INT_SIZE_32:
                  ret.val = ir->genUint32(immediate->v_uint32);
                  break;
              case INT_SIZE_64:
                  ret.val = ir->genUint64(immediate->v_uint64);
                  break;
              default:
                  INT_FATAL(self, "Unsupported uint size");
                  break;
                  }
              break;
          case NUM_KIND_FLOAT:
          case NUM_KIND_IMAG:       // can this happen?
              switch (immediate->num_index) {
              case FLOAT_SIZE_32:
                  ret.val = ir->genFloat32(immediate->v_float32);
                  break;
              case FLOAT_SIZE_64:
                  ret.val = ir->genFloat64(immediate->v_float64);
                  break;
              default:
                  INT_FATAL(self, "Unsupported float size");
                  break;
                  }
              break;
          case NUM_KIND_COMPLEX:    // can this happen?
              switch (immediate->num_index) {
              case COMPLEX_SIZE_64:
                  ret.val = 
                      ir->genComplex64(
                          immediate->v_complex64.r, 
                          immediate->v_complex64.i);
                  break;
              case COMPLEX_SIZE_128:
                  ret.val = 
                      ir->genComplex128(
                          immediate->v_complex128.r, 
                          immediate->v_complex128.i);
                  break;
              default:
                  INT_FATAL(self, "Unsupported complex size");
                  break;
                  }
              break;
          default:
              INT_FATAL(self, "Unsupported immediate kind");
              break;
              }
          }
      else if (self->typeInfo() == dtNil && !strcmp(self->cname, "nil"))
          ret.val = ir->genNullPtr();   
      else {
        // not immediate
          ret.val = ir->genVar(self->cname);
          ret.isLVPtr = GEN_PTR;
          }
      return ret;
      }
      
  void GenIR::VarSymbol_codegenDef(VarSymbol *self) {
      VarSymbol_codegenDefC(self);
      }
      
  void GenIR::VarSymbol_codegenDefC(VarSymbol *self) {
      if (self->hasFlag(FLAG_EXTERN))
          return;
      if (self->type == dtVoid)
          return;
      ClassType *ct = toClassType(self->type);
      GenRet type;
      if (self->hasFlag(FLAG_SUPER_CLASS))
          type.type = ir->genType(ct->classStructName(true));
      else
          type = Type_codegen(self->type);
      ir->defVar(self->cname, type.type);
    // TODO: Implement null initialization for some special cases
    //     (see original code for exact details)
      }

  GenRet GenIR::ArgSymbol_codegen(ArgSymbol *self) {
      GenRet ret;

      ret.val = ir->genArg(self->cname);
      ret.isLVPtr = GEN_PTR;

      if (self->requiresCPtr()) {
      // Don't try to use chplType.
          ret.chplType = NULL;
          ret = codegenLocalDeref(ret);
          }

      ret.chplType = self->typeInfo();

      return ret;
      }
      
  GenRet GenIR::ArgSymbol_codegenType(ArgSymbol *self) {
      GenRet ret;
      GenRet type = Type_codegen(self->type);
      ret.type = type.type;
      if (self->requiresCPtr())
          ret.type = ir->genCPtr(ret.type);
      return ret;
      }

  GenRet GenIR::TypeSymbol_codegen(TypeSymbol *self) {
      GenRet ret;
      ret.chplType = self->typeInfo();
      ret.type = ir->genType(self->cname);
      return ret;
      }
      
  void GenIR::TypeSymbol_codegenDef(TypeSymbol *self) {
    // ACHTUNG: Introduce generic debugger support
      if (breakOnCodegenCname[0] &&
              !strcmp(self->cname, breakOnCodegenCname))
          gdbShouldBreakHere();

#if 0    // TODO: Remove or revise this
      if (!self->hasFlag(FLAG_EXTERN))
          Type_codegenDef(self->type);
#endif
      Type_codegenDef(self->type);

      self->codegenned = true;

    // no action required.
      }
      
  void GenIR::TypeSymbol_codegenPrototype(TypeSymbol *self) {
#if 0    // TODO: Remove or revise this
      if (!self->hasFlag(FLAG_EXTERN))
          Type_codegenPrototype(self->type);
#endif
      if (!self->hasFlag(FLAG_EXTERN))
          Type_codegenPrototype(self->type);
      else
          ir->externType(self->cname);
      }
      
  GenRet GenIR::FnSymbol_codegen(FnSymbol *self) {
      GenRet ret;
      ret.val = ir->genFn(self->cname);
      return ret;
      }
      
  void GenIR::FnSymbol_codegenDef(FnSymbol *self) {
      if (breakOnCodegenCname[0] &&
              !strcmp(self->cname, breakOnCodegenCname))
          gdbShouldBreakHere();

      if (self->hasFlag(FLAG_NO_CODEGEN)) 
          return;

      ir->startFn();

      FnSymbol_codegenHeaderC(self);

      if (fNoRepositionDefExpr) {
          Vec<BaseAST*> asts;
          collect_top_asts(self->body, asts);

          forv_Vec(BaseAST, ast, asts) {
              if (DefExpr *def = toDefExpr(ast)) {
                  if (!toTypeSymbol(def->sym)) {
                       if (isVarSymbol(def->sym))
                          ir->logId(def->sym->id);
                      Symbol_codegenDef(def->sym);
                      }
                  }
              }
          }

      Expr_codegen(self->body);

      ir->endFn();
      }
      
  void GenIR::FnSymbol_codegenPrototype(FnSymbol *self) {
if (self->hasFlag(FLAG_EXTERN))  
fprintf(stderr, "@@@ FN-EXTERN [%s]\n", self->cname);
      if (self->hasFlag(FLAG_EXTERN) && !genExternPrototypes) 
          return;
      if (self->hasFlag(FLAG_NO_PROTOTYPE)) 
          return;
      if (self->hasFlag(FLAG_NO_CODEGEN)) 
          return;

if (self->hasFlag(FLAG_EXTERN))  
fprintf(stderr, "@@@ ... passed\n");
      if (breakOnCodegenCname[0] &&
              !strcmp(self->cname, breakOnCodegenCname))
          gdbShouldBreakHere();

    // In C, we don't need to generate prototypes for external
    // functions, since these prototypes will presumably be
    // present in some C header file.
      ir->startProtoFn();
      FnSymbol_codegenHeaderC(self);
      ir->endProtoFn();
      }
      
  void GenIR::FnSymbol_codegenHeaderC(FnSymbol *self) {
      ir->logId(self->id);
      GenRet retType = Type_codegen(self->retType);
      ir->startFnHeader(self->cname, retType.type);
      for_formals(formal, self) {
          GenRet arg = ArgSymbol_codegenType(formal);
          ir->addFormal(formal->cname, arg.type);
          }
      ir->endFnHeader();
      }

  GenRet GenIR::FnSymbol_codegenFunctionType(FnSymbol *self) {
      GenRet ret;

      ret.chplType = self->typeInfo();
    //just documenting the current state
      INT_ASSERT(ret.chplType == dtUnknown);

      GenRet retType = Type_codegen(self->retType);
      ir->startFnType(retType.type);
      for_formals(formal, self) {
          GenRet arg = ArgSymbol_codegenType(formal);
          ir->addFormalType(arg.type);
          }
      ret.type = ir->endFnType();

      return ret;
      }

  GenRet GenIR::EnumSymbol_codegen(EnumSymbol *self) {
      GenRet ret;
      ret.val = ir->genEnum(self->cname);      
      return ret;
      }

  void GenIR::EnumSymbol_codegenDef(EnumSymbol *self) { }
      
// ACHTUNG: Do we really need this method? 
  GenRet GenIR::ModuleSymbol_codegen(ModuleSymbol *self) {
      GenRet ret;
      ret.val = ir->genModule(self->cname);      
      return ret;
      }

  void GenIR::ModuleSymbol_codegenDef(ModuleSymbol *self) {
      setInfoFilename(self->fname());
      setInfoLineno(self->linenum());

      ir->startModule(self->cname);

      Vec<FnSymbol*> fns;
      for_alist(expr, self->block->body) {
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
      ir->endModule();
      }
      
  GenRet GenIR::LabelSymbol_codegen(LabelSymbol *self) {
      GenRet ret;
      ret.val = ir->genLabel(self->cname);      
      return ret;
      }

  void GenIR::LabelSymbol_codegenDef(LabelSymbol *self) { }

// implementation: helpers

  int GenIR::compareLineno(const void *v1, const void *v2) {
      FnSymbol *fn1 = *(FnSymbol **)v1;
      FnSymbol *fn2 = *(FnSymbol **)v2;
      if (fn1->linenum() > fn2->linenum())
          return 1;
      else if (fn1->linenum() < fn2->linenum())
          return -1;
      else
          return 0;
      }

