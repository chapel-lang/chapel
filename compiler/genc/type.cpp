//
//    TYPE.CPP -- Code generator (C back-end)
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

#include "genc.h"

//
//    GenC
//

// interface: Type codegen

  GenRet GenC::Type_codegen(Type *self) {
      Symbol *symbol = self->symbol;
      
      if (self == dtUnknown)
          INT_FATAL(self, "Cannot generate unknown type");
      return Symbol_codegen(symbol);
      }

  void GenC::Type_codegenDef(Type *self) {
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

  void GenC::Type_codegenPrototype(Type *self) {
      switch (self->astTag) {
      case E_ClassType:
          ClassType_codegenPrototype((ClassType *)self);
          break;
      default:
          break;
          }
      }

  int GenC::Type_codegenStructure(
          Type *self, FILE *outfile, const char *baseoffset) {
      Symbol *symbol = self->symbol;

      switch (self->astTag) {
      case E_PrimitiveType:
          return PrimitiveType_codegenStructure(
              (PrimitiveType *)self, outfile, baseoffset);
      case E_EnumType:
          return EnumType_codegenStructure(
              (EnumType *)self, outfile, baseoffset);
      case E_ClassType:
          return ClassType_codegenStructure(
              (ClassType *)self, outfile, baseoffset);
      default:
          INT_FATAL(self, 
              "Unexpected type in codegenStructure: %s", symbol->name);
          return 0;
          }
      }

  int GenC::ClassType_codegenFieldStructure(
          ClassType *self,
          FILE *outfile, 
          bool nested, 
          const char *baseoffset) {
      TypeSymbol *symbol = self->symbol;
          
    // Handle ref types as pointers
      if (symbol->hasFlag(FLAG_REF)) {
          fprintf(outfile, "{CHPL_TYPE_CLASS_REFERENCE, 0},\n");
          fprintf(outfile, "{CHPL_TYPE_DONE, -1}\n");
          return 1;
          }

      int totfields = 0;
      int locfieldnum = 0;
      for_fields(field, self) {
          const char *newbaseoffset = genNewBaseOffsetString(
              symbol, locfieldnum, baseoffset, field, self);
          int numfields = 
              Type_codegenStructure(field->type, outfile, newbaseoffset);
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

  GenRet GenC::ClassType_codegenClassStructType(ClassType *self) {
      ClassTag classTag = self->classTag;

      GenRet ret;
      if (classTag == CLASS_CLASS)
          ret.c = std::string(self->classStructName(true));
      else {
          INT_FATAL("codegenClassStructType on non-class");
          ret = Type_codegen(self);
          }
      return ret;
      }

// interface: global codegen

  void GenC::registerTypeToStructurallyCodegen(TypeSymbol *type) {
    //  printf("registering chpl_rt_type_id_%s\n", type->cname);
      if (!typesToStructurallyCodegen.set_in(type)) {
          typesToStructurallyCodegenList.add(type);
          typesToStructurallyCodegen.set_add(type);
          }
      }

  GenRet GenC::genTypeStructureIndex(TypeSymbol *typesym) {
      GenRet ret;
      if (fHeterogeneous) {
        // strings are special
          if (toPrimitiveType(typesym) == dtString)
              ret.c = std::string("-") + typesym->cname;
          else
              ret.c = genChplTypeEnumString(typesym);
          }
      else
          ret.c = "-1";
      ret.chplType = dtInt[INT_SIZE_32];
      return ret;
      }

#define TYPE_STRUCTURE_FILE "_type_structure.c"

  void GenC::codegenTypeStructures(FILE *hdrfile) {
      fileinfo typeStructFile;
      openCFile(&typeStructFile, TYPE_STRUCTURE_FILE);
      FILE *outfile = typeStructFile.fptr;

      fprintf(outfile, "typedef enum {\n");
      forv_Vec(TypeSymbol*, typesym, typesToStructurallyCodegenList) {
          fprintf(outfile, "%s,\n", genChplTypeEnumString(typesym));
          }
      fprintf(outfile, "chpl_num_rt_type_ids\n");
      fprintf(outfile, "} chpl_rt_types;\n\n");

      fprintf(outfile,
          "chpl_fieldType chpl_structType[][CHPL_MAX_FIELDS_PER_TYPE] = {\n");

      qsort(
          typesToStructurallyCodegenList.v,
          typesToStructurallyCodegenList.n,
          sizeof(typesToStructurallyCodegenList.v[0]),
          compareCnames);
      int num = 0;
      forv_Vec(TypeSymbol*, typesym, typesToStructurallyCodegenList) {
          if (num)
              fprintf(outfile, ",\n");
          fprintf(outfile, "/* %s (%s) */\n", typesym->name, typesym->cname);
          fprintf(outfile, "{\n");
          if (ClassType *classtype = toClassType(typesym->type)) {
              int numfields = 
                  ClassType_codegenFieldStructure(
                      classtype, outfile, false, "0");
              if (numfields > maxFieldsPerType)
                  maxFieldsPerType = numfields;
              }
          else {
              Type_codegenStructure(typesym->type, outfile, "0");
              fprintf(outfile, "{CHPL_TYPE_DONE, -1}\n");
              }
          fprintf(outfile, "}");
          num++;
          }
      fprintf(outfile, "};\n\n");

      fprintf(outfile, "size_t chpl_sizeType[] = {\n");
      num = 0;
      forv_Vec(TypeSymbol*, typesym, typesToStructurallyCodegenList) {
          if (num)
              fprintf(outfile, ",\n");
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

  void GenC::codegenTypeStructureInclude(FILE *outfile) {
      fprintf(outfile, "#include \"" TYPE_STRUCTURE_FILE "\"\n");
      }

// implementation: Type codegen

  void GenC::PrimitiveType_codegenDef(PrimitiveType *self) { }

  int GenC::PrimitiveType_codegenStructure(
          PrimitiveType *self, FILE *outfile, const char *baseoffset) {
      TypeSymbol *symbol = self->symbol;
      bool isInternalType = self->isInternalType;

      if (!isInternalType) {
          Symbol *cgsym = symbol;
          fprintf(outfile, "{CHPL_TYPE_%s, %s},\n", cgsym->cname, baseoffset);
          }
      else
          INT_FATAL(self, "Cannot codegen an internal type");
      return 1;

      }

  void GenC::EnumType_codegenDef(EnumType *self) {
      TypeSymbol *symbol = self->symbol;

      writeC("typedef enum {");
      bool first = true;
      for_enums(constant, self) {
          if (!first)
              writeC(", ");
          writeC("%s", Symbol_codegen(constant->sym).c.c_str());
          if (constant->init)
              writeC(" = %s", Expr_codegen(constant->init).c.c_str());
          else 
              INT_FATAL("enum init not defined");
          first = false;
          }
      writeC("} ");
      writeC("%s", Symbol_codegen(symbol).c.c_str());
      writeC(";\n");
      }

  int GenC::EnumType_codegenStructure(
          EnumType *self, FILE *outfile, const char *baseoffset) {
      fprintf(outfile, "{CHPL_TYPE_enum, %s},\n", baseoffset);
      return 1;
      }

  void GenC::ClassType_codegenDef(ClassType *self) {
      TypeSymbol *symbol = self->symbol;
      ClassTag classTag = self->classTag;

      if (symbol->hasFlag(FLAG_STAR_TUPLE)) {
          writeC("typedef ");
          writeC("%s", Type_codegen(self->getField("x1")->type).c.c_str());
          writeC(" %s", Symbol_codegen(symbol).c.c_str());
          writeC("[%d];\n\n", self->fields.length);
          return;
          }
      else if (symbol->hasFlag(FLAG_FIXED_STRING)) {
          int size = 
              toVarSymbol(symbol->type->substitutions.v[0].value)->
                  immediate->int_value();
          writeC("typedef char %s[%i];\n", symbol->cname, size);
          return;
          }
      else if (symbol->hasFlag(FLAG_REF)) {
          TypeSymbol *base = self->getField(1)->type->symbol;
          const char *baseType = base->cname;
          writeC("typedef %s *%s;\n", baseType, symbol->cname);
          return;
          }
      else if (symbol->hasFlag(FLAG_DATA_CLASS)) {
          TypeSymbol *base = getDataClassType(symbol);
          const char *baseType = base->cname;
          writeC("typedef %s *%s;\n", baseType, symbol->cname);
          }
      else {
          writeC("typedef struct %s", self->classStructName(false));
          if (classTag == CLASS_CLASS && self->dispatchParents.n > 0) {
            // Add a comment to class definitions listing super classes
              bool first = true;
              writeC(" /* : ");
              forv_Vec(Type, parent, self->dispatchParents) {
                  if (parent) {
                      if (!first)
                          writeC(", ");
                      writeC("%s", Symbol_codegen(parent->symbol).c.c_str());
                      first = false;
                      }
                  }
              writeC(" */");
              }
          writeC(" {\n");
          if (symbol->hasFlag(FLAG_OBJECT_CLASS) && classTag == CLASS_CLASS)
              writeC("chpl__class_id chpl__cid;\n");
          else if (classTag == CLASS_UNION) {
              writeC("int64_t _uid;\n");
              if (self->fields.length != 0)
                  writeC("union {\n");
              }
          else if (self->fields.length == 0)
              writeC("int dummyFieldToAvoidWarning;\n");

          if (self->fields.length != 0) {
              for_fields(field, self) {
                  Symbol_codegenDef(field);
                  }
              }
          flushStatements();

          if (classTag == CLASS_UNION) {
              if (self->fields.length != 0)
                  writeC("} _u;\n");
              }
          writeC("} %s;\n\n", self->classStructName(true));
          }
      }

  void GenC::ClassType_codegenPrototype(ClassType *self) {
      TypeSymbol *symbol = self->symbol;
      ClassTag classTag = self->classTag;

    // Only generates prototypes for CLASS_CLASS (ie a Chapel class,
    // not a record or wide pointer)
      if (classTag == CLASS_CLASS)
          writeC("typedef struct %s* %s;\n", 
              self->classStructName(false), symbol->cname);
      }

  int GenC::ClassType_codegenStructure(
          ClassType *self, FILE *outfile, const char *baseoffset) {
      ClassTag classTag = self->classTag;

      switch (classTag) {
      case CLASS_CLASS:
          fprintf(outfile, "{CHPL_TYPE_CLASS_REFERENCE, %s},\n", baseoffset);
          return 1;
      case CLASS_RECORD:
          return ClassType_codegenFieldStructure(
              self, outfile, true, baseoffset);
      case CLASS_UNION:
          INT_FATAL(self, "Don't know how to codegenStructure for unions yet");
          return 0;
      default:
          INT_FATAL(self, "Unexpected case in ClassType::codegenStructure");
          return 0;
          }
      }

// implementation: global codegen

  const char *GenC::genNewBaseOffsetString(
          TypeSymbol *typesym, 
          int fieldnum,
          const char *baseoffset, 
          Symbol *field,
          ClassType *classtype) {
      if (classtype->symbol->hasFlag(FLAG_STAR_TUPLE)) {
          char fieldnumstr[64];
          sprintf(fieldnumstr, "%d", fieldnum);
          return astr(
              baseoffset, " + (", 
              fieldnumstr, 
              "* ", 
              genSizeofStr(field->type->symbol), 
              ")");
          }
      return astr(
          baseoffset, 
          " + offsetof(", 
          genUnderscore(classtype->symbol),
          classtype->symbol->cname, ", ", 
          classtype->classTag == CLASS_UNION ? "_u." : "",
          field->cname, 
          ")");
      }

  const char *GenC::genSizeofStr(TypeSymbol *typesym) {
      return astr("sizeof(", genUnderscore(typesym), typesym->cname, ")");
      }

  const char *GenC::genChplTypeEnumString(TypeSymbol *typesym) {
      return astr("chpl_rt_type_id_", genUnderscore(typesym), typesym->cname);
      }

  const char *GenC::genUnderscore(Symbol *sym) {
      ClassType *classtype = toClassType(sym->type);
      if (classtype && 
              classtype->classTag == CLASS_CLASS && 
              !sym->hasFlag(FLAG_REF))
          return "_";
      else
          return "";
      }

  int GenC::compareCnames(const void *v1, const void *v2) {
      int retval;
      TypeSymbol *t1 = *(TypeSymbol **)v1;
      TypeSymbol *t2 = *(TypeSymbol **)v2;
      retval = strcmp(t1->cname, t2->cname);
      if (retval > 0)
          return 1;
      else if (retval < 0)
          return -1;
      else
          return 0;
      }

