//
//    TYPE.CPP -- Code generator (IR back-end)
//
//        Types
//
//    Copyright (c) 2004-2013, Cray Inc.
//    Copyright (c) 2013, Unicorn Enterprises SA
//

// disabled to avoid duplicate dtXXX definitions
//#define TYPE_EXTERN

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

#include "irbase.h"
#include "genir.h"

//
//    GenIR
//

// interface: Type codegen

  GenRet GenIR::Type_codegen(Type *self) {
      Symbol *symbol = self->symbol;
      
      if (self == dtUnknown)
          INT_FATAL(self, "Cannot generate unknown type");
      return Symbol_codegen(symbol);
      }

  void GenIR::Type_codegenDef(Type *self) {
      switch (self->astTag) {
      case E_PrimitiveType:
          PrimitiveType_codegenDef((PrimitiveType *)self);
          break;
      case E_EnumType:
          EnumType_codegenDef((EnumType *)self);
          break;
      case E_ClassType:
          ClassType_codegenDef((ClassType *)self);
          break;
      default:
          INT_FATAL(self, "Unexpected call to Type::codegenDef");
          break;
          }
      }

  void GenIR::Type_codegenPrototype(Type *self) {
      switch (self->astTag) {
      case E_ClassType:
          ClassType_codegenPrototype((ClassType *)self);
          break;
      default:
          break;
          }
      }

  GenRet GenIR::ClassType_codegenClassStructType(ClassType *self) {
      ClassTag classTag = self->classTag;

      GenRet ret;
      if (classTag == CLASS_CLASS)
          ret.type = ir->genType(self->classStructName(true));
      else {
          INT_FATAL("codegenClassStructType on non-class");
          ret = Type_codegen(self);
          }
      return ret;
      }

// interface: global codegen

  GenRet GenIR::genTypeStructureIndex(TypeSymbol *typesym) {
      GenRet ret;
      ret.val = ir->genTypeStructureIndex(typesym->cname);
      ret.chplType = dtInt[INT_SIZE_32];
      return ret;
      }

// implementation: Type codegen

  void GenIR::PrimitiveType_codegenDef(PrimitiveType *self) { }

  void GenIR::EnumType_codegenDef(EnumType *self) {
      PrimitiveType *ty = self->getIntegerType();
      GenRet type = Type_codegen(ty);
      ir->startEnum(self->symbol->cname, type.type);
      for_enums(constant, self) {
          GenRet init;
          if (constant->init) {
              VarSymbol *s = toVarSymbol(toSymExpr(constant->init)->var);
              INT_ASSERT(s);
              INT_ASSERT(s->immediate);
              VarSymbol *sizedImmediate = resizeImmediate(s, ty);
              init = VarSymbol_codegen(sizedImmediate);
              }
          else 
              INT_FATAL("enum init not defined");
          ir->addEnum(constant->sym->cname, init.val);
          }
      ir->endEnum();
      }

  void GenIR::ClassType_codegenDef(ClassType *self) {
      TypeSymbol *symbol = self->symbol;
      if (symbol->hasFlag(FLAG_STAR_TUPLE)) {
          GenRet base = Type_codegen(self->getField("x1")->type);
          ir->defStarTuple(symbol->cname, base.type, self->fields.length);
          }
      else if (symbol->hasFlag(FLAG_FIXED_STRING)) {
          int size = (int)
              toVarSymbol(symbol->type->substitutions.v[0].value)->
                  immediate->int_value();
          ir->defFixedString(symbol->cname, size);
          }
      else if (symbol->hasFlag(FLAG_REF)) {
          GenRet base = Type_codegen(self->getField(1)->type);
          ir->defRef(symbol->cname, base.type);
          }
      else if (symbol->hasFlag(FLAG_DATA_CLASS)) {
          GenRet base = Symbol_codegen(getDataClassType(symbol));
          ir->defDataClass(symbol->cname, base.type);
          }
      else {
          ClassTag classTag = self->classTag;
          const char *name = self->classStructName(true);
          if (symbol->hasFlag(FLAG_OBJECT_CLASS) && classTag == CLASS_CLASS)
              ir->startClass(name);
          else if (classTag == CLASS_UNION)
              ir->startUnion(name);
          else
              ir->startStruct(name);

          if (classTag == CLASS_CLASS && self->dispatchParents.n > 0) {
            // Add a comment to class definitions listing super classes
              ir->startSuper();
              forv_Vec(Type, parent, self->dispatchParents) {
                  if (parent) {
                      GenRet super = Symbol_codegen(parent->symbol);
                      ir->addSuper(super.type);
                      }
                  }
              ir->endSuper();
              }
                  
          if (self->fields.length != 0) {
              for_fields(field, self) {
                  Symbol_codegenDef(field);
                  }
              }
          
          if (symbol->hasFlag(FLAG_OBJECT_CLASS) && classTag == CLASS_CLASS)
              ir->endClass();
          else if (classTag == CLASS_UNION)
              ir->endUnion();
          else
              ir->endStruct();
          }
      }

  void GenIR::ClassType_codegenPrototype(ClassType *self) {
    // Only generates prototypes for CLASS_CLASS (ie a Chapel class,
    // not a record or wide pointer)
      if (self->classTag == CLASS_CLASS)
          ir->protoClass(
              self->symbol->cname, 
              self->classStructName(true));
      }
