//
//    SUBR.CPP -- Code generator (C back-end)
//
//        Subroutines for expressions
//
//    Copyright (c) 2004-2013, Cray Inc.
//    Copyright (c) 2013, Unicorn Enterprises SA
//

#ifndef __STDC_FORMAT_MACROS
#define __STDC_FORMAT_MACROS
#endif

#include <cstdlib>
#include <cstring>
#include <inttypes.h>
#include "astutil.h"
#include "expr.h"
#include "files.h"
#include "misc.h"
#include "passes.h"
#include "stmt.h"
#include "stringutil.h"
#include "type.h"

#include "genc.h"

//
//    GenC
//

// implementation (target-idependent)

  GenRet GenC::codegenFieldPtr(GenRet base, Expr *field) {
      const char *cname = NULL;
      const char *name = NULL;
      if (DefExpr *de = toDefExpr(field)) {
          cname = de->sym->cname;
          name = de->sym->name;
          }
      else if (SymExpr *se = toSymExpr(field)) {
          cname = se->var->cname;
          name = se->var->name;
          }
      else if (NamedExpr *ne = toNamedExpr(field))
          cname = name = ne->name;
      else
          INT_FATAL("Unknown field in codegenFieldPtr");
      return codegenFieldPtr(base, cname, name, field_normal);
      }

  GenRet GenC::codegenFieldPtr(GenRet base, Symbol *field) {
      return codegenFieldPtr(base, field->cname, field->name, field_normal);
      }

  GenRet GenC::codegenFieldPtr(GenRet base, const char *field) {
      return codegenFieldPtr(base, field, field, field_normal);
      }

  GenRet GenC::codegenFieldCidPtr(GenRet base) {
      GenRet ret = codegenFieldPtr(base, "chpl__cid", NULL, field_cid);
    /*
      if (!ret.chplType) 
          ret.chplType = CLASS_ID_TYPE;
    */
      return ret;
      }

  GenRet GenC::codegenFieldUidPtr(GenRet base) {
      GenRet ret = codegenFieldPtr(base, "_uid", NULL, field_uid);
    /*
      if (!ret.chplType) 
          ret.chplType = UNION_ID_TYPE;
    */
      return ret;
      }

// use this function for chplTypes
  GenRet GenC::createTempVar(Type *t) {
      GenRet ret = createTempVar(t->symbol->cname);
      ret.chplType = t;
      return ret;
      }

// Create a temporary
// value holding r and return a pointer to it.
// If r is already a pointer, do nothing.
// Does not handle homogeneous tuples.
// Does not handle wide pointers.
  GenRet GenC::codegenValuePtr(GenRet r) {
      GenRet ret = r;

      if (ret.isLVPtr == GEN_PTR) 
          return ret;

      ret.isLVPtr = GEN_PTR;

      if (r.chplType) {
          bool isStarTuple = hasFlag(r.chplType, FLAG_STAR_TUPLE);
          INT_ASSERT(!isStarTuple);
          } 

      INT_ASSERT(r.isLVPtr != GEN_WIDE_PTR);
      ret = createTempVarWith(r);
      return ret;
      }

// Converts an L-value pointer into a
// pointer value, so that it can for example
// be stored in another pointer.
  GenRet GenC::codegenAddrOf(GenRet r) {
      GenRet ret = r;

      if (r.isLVPtr == GEN_WIDE_PTR) {
          if (r.chplType) {
              Type *refType = getOrMakeRefTypeDuringCodegen(r.chplType);
              ret.chplType = getOrMakeWideTypeDuringCodegen(refType);
              }
          ret.isLVPtr = GEN_VAL;
          return ret;
          }
      else if (r.isLVPtr == GEN_PTR) {
          if (r.chplType) 
              ret.chplType = getOrMakeRefTypeDuringCodegen(r.chplType);
          ret.isLVPtr = GEN_VAL;
          }
      else
          INT_FATAL("misuse of codegenAddrOf");

      return ret;
      }
      
// Converts an L-value pointer into a
// pointer value, so that it can for example
// be stored in another pointer.
// If we start with a wide pointer, we just discard
// the locale portion (ie assume it is local).
  GenRet GenC::codegenLocalAddrOf(GenRet r) {
      if (r.isLVPtr == GEN_WIDE_PTR)
          return codegenRaddr(r);
      else
          return codegenAddrOf(r);
      }

  GenRet GenC::codegenLocalDeref(GenRet r) {
      GenRet ret;
    // LocalDeref on a wide pointer should just give
    // the address field as a reference.
      if (r.chplType && hasFlag(r.chplType, FLAG_WIDE)) {
          ret = codegenRaddr(r);
          return ret;
          }
    // For some reason, ArgSymbol might not have a real Chapel
    // reference type, so we have this function as a workaround
    // (instead of running codegenDeref with chplType=type->refType)
      ret = codegenValue(r);
      ret.isLVPtr = GEN_PTR;
      if (r.chplType) 
          ret.chplType = r.chplType->getValType(); 
      return ret;
      }

// codegenValue(r) to remove & or add * (if & already removed) and sets isLVPtr
  GenRet GenC::codegenDeref(GenRet r) {
      GenRet ret;

      INT_ASSERT(r.chplType);
      if (hasFlag(r.chplType, FLAG_WIDE)) {
          ret = codegenValue(r);
          ret.isLVPtr = GEN_WIDE_PTR;
          ret.chplType = r.chplType->getValType(); 
          }
      else if (hasFlag(r.chplType, FLAG_REF))
          return codegenLocalDeref(r);
      else
        // not a reference.
          INT_ASSERT(0);
          
      return ret;
      }

  GenRet GenC::codegenDynamicCastCheck(GenRet cid, Type *type) {
      GenRet ret = codegenEquals(cid, codegenUseCid(type));
      forv_Vec(Type, child, type->dispatchChildren) {
          ret = codegenLogicalOr(ret, codegenDynamicCastCheck(cid, child));
          }
      return ret;
      }

  GenRet GenC::codegenArgForFormal(
          GenRet arg,
          ArgSymbol *formal,
          bool defaultToValues,
          bool isExtern) {
    // NOTE -- VMT call had add & if arg isRecord.
      if (formal) {
          if (!isExtern &&
                  formal->requiresCPtr() &&
                  !hasFlag(formal->type, FLAG_REF)) { 
              if (arg.isLVPtr == GEN_VAL)
                  arg = codegenValuePtr(arg);
              }
          else {
              if (arg.isLVPtr != GEN_VAL)
                  arg = codegenValue(arg);
              }
          }
      else {
          if (defaultToValues) {
              if (arg.isLVPtr != GEN_VAL)
                  arg = codegenValue(arg);
              } 
        // otherwise, leave it be.
          }
      return arg;
      }

  GenRet GenC::codegenBasicPrimitiveExpr(CallExpr *call) {
      int mark = startArg();

      for_actuals(actual, call) {
          GenRet gen = eval(actual);
          Type *t = actual->typeInfo();

        // Make wide pointers/classes local
          if (hasFlag(t, FLAG_WIDE_CLASS) || hasFlag(t, FLAG_WIDE))
              gen = codegenRaddr(gen);

        // Dereference reference or now-local wide reference
          if (hasFlag(t, FLAG_REF) || hasFlag(t, FLAG_WIDE))
              gen = codegenDeref(gen);

          gen = codegenValue(gen);
          pushArg(gen);
          }

      endArg(mark);
      return codegenCallExpr(call->primitive->name, argNum(), argPtr());
      }

  GenRet GenC::codegenZero() {
      return Symbol_codegen(new_IntSymbol(0, INT_SIZE_64));
      }

  GenRet GenC::codegenOne() {
      return Symbol_codegen(new_IntSymbol(1, INT_SIZE_64));
      }

  GenRet GenC::codegenSizeof(Type *t) {
      return codegenSizeof(t->symbol->cname);
      }

  bool GenC::isTupleOfTuple(BaseAST *e) {
      Type *t = e->typeInfo();
      if (!hasFlag(t, FLAG_STAR_TUPLE))
          return false;
      t = toDefExpr(toClassType(t)->fields.head)->sym->type;
      return hasFlag(t, FLAG_TUPLE);
      }

  GenRet GenC::codegenExprMinusOne(Expr *expr) {
      int64_t i;
      IF1_int_type width = INT_SIZE_64;
      if (get_width(expr->typeInfo()) <= 8) 
          width = INT_SIZE_8;
      else if (get_width(expr->typeInfo()) <= 16) 
          width = INT_SIZE_16;
      else if (get_width(expr->typeInfo()) <= 32) 
          width = INT_SIZE_32;

      if (get_int(expr, &i))
          return Symbol_codegen(new_IntSymbol(i-1, width));
      else
          return codegenSub(eval(expr), evalSym(new_IntSymbol(1, width)));
      }

  void GenC::callExprHelper(CallExpr *call, BaseAST *arg) {
      if (!arg)
          return;
      if (toSymbol(arg) || toExpr(arg))
          call->insertAtTail(arg);
      else
          INT_FATAL(call, "Bad argList in CallExpr constructor");
      }

  GenRet GenC::codegenUseGlobal(std::string str) {
      return codegenUseGlobal(str.c_str());
      }

  GenRet GenC::codegenUseGlobal(const char *global) {
      GenRet ret;
      ret.c = global;
      ret.isLVPtr = GEN_VAL;
      return ret;
      }

  GenRet GenC::codegenUseCid(Type *classType) {
      std::string varname;
      varname = varname + "chpl__cid_" + classType->symbol->cname;
      GenRet ret = codegenUseGlobal(varname.c_str());
      ret.chplType = CLASS_ID_TYPE;
      return ret;
      }

// A construct which gives the current node ID (int32_t).
  GenRet GenC::codegenGetNodeID() {
      GenRet ret =  codegenUseGlobal("chpl_nodeID");
      ret.chplType = NODE_ID_TYPE;
      return ret;
      }

// A construct which gives the current sublocale ID.
  GenRet GenC::codegenGetSublocID() {
      GenRet ret =  codegenCallExpr("chpl_task_getSubLoc");
      ret.chplType = SUBLOC_ID_TYPE;
      return ret;
      }

// A construct which gives the current localeID (c_localeID_t)
// This is a partial implementation; it always uses 0 for the sublocale
// portion of the locale ID.
  GenRet GenC::codegenGetLocaleID() {
      return codegenLocaleID(codegenGetNodeID(), codegenZero());
      }

  GenRet GenC::codegenWideHere(GenRet addr, Type *wideType/*= NULL*/) {
      GenRet locale = codegenGetLocaleID();
      GenRet addrVal = codegenValue(addr);
      GenRet ret = codegenWideAddr(locale, addrVal, wideType);
      return ret;
      }

  GenRet GenC::codegenRaddr(GenRet wide) {
      return codegenValue(codegenRaddrMaybePtr(wide));
      }

  GenRet GenC::codegenRlocale(GenRet wide) {
      return codegenValue(codegenRlocaleMaybePtr(wide));
      }

  GenRet GenC::codegenRnode(GenRet wide) {
      GenRet locId = codegenValuePtr(codegenRlocaleMaybePtr(wide));
      GenRet ret = codegenFieldPtr(locId, "node");
      return ret;
      }

  GenRet GenC::codegenRsubloc(GenRet wide) {
      GenRet locId = codegenValuePtr(codegenRlocaleMaybePtr(wide));
      GenRet ret = codegenFieldPtr(locId, "subloc");
      return ret;
      }

// implementation (partially target-specific, to be split)

// Generates code to produce a pointer to the array element
// works with getDataClassType ie "data class" ie _ddata
//  and with homogenous tuples (ie star tuple).
// If base is a wide pointer data type, this will generate
// a wide pointer data type.
// This is equivalent to C (assuming ptr is a pointer type)
//   ptr + i
//
// Local pointers to index values are OK, but not wide pointers.
//
  GenRet GenC::codegenElementPtr(GenRet base, GenRet index) {
      GenRet ret;
      Type *baseType = base.chplType;
      Type *eltType = NULL;
      std::string addr;

      if (base.isLVPtr == GEN_WIDE_PTR) {
          ret = 
              codegenWideAddr(
                  codegenRlocale(base),
                  codegenElementPtr(codegenRaddr(base), index));
          return ret;
          }
      if (baseType) {
          if (hasFlag(baseType, FLAG_REF)) {
            // dereference base if needed 
              GenRet baseVal = codegenDeref(base);
              ret = codegenElementPtr(baseVal, index);
              return ret;
              }
        // for wide class base 
          if (hasEitherFlag(baseType, FLAG_WIDE, FLAG_WIDE_CLASS)) {
              base = codegenValue(base);
              ret = 
                  codegenWideAddr(
                      codegenRlocale(base), 
                      codegenElementPtr(codegenRaddr(base), index));
              return ret;
              }
          }
      if (baseType) {
          if (hasFlag(baseType, FLAG_STAR_TUPLE))
              eltType = baseType->getField("x1")->typeInfo();
          if (hasFlag(baseType, FLAG_DATA_CLASS))
              eltType = getDataClassType(baseType->symbol)->typeInfo();
          INT_ASSERT(eltType);
          ret.chplType = eltType;
          }

      ret.isLVPtr = GEN_PTR; 

      index = codegenValue(index);

      base = codegenValue(base);
      ret.c = "(" + base.c + " + " + index.c + ")"; 

      return ret;
      }

// Generates code to produce a pointer to the member (ie a field).
// Does not actually do any loads,stores,puts,or gets;
//  just does address arithmetic.
//  was codegen_member_ptr
//
// special is field_normal, field_cid, or field_uid
  GenRet GenC::codegenFieldPtr(
          GenRet base,
          const char *c_field_name,
          const char *chpl_field_name,
          int special) {
      GenRet ret;
      Type *baseType = base.chplType;
      if (special == field_normal) 
          INT_ASSERT(baseType);
      ClassType *ct = toClassType(baseType);
      INT_ASSERT(ct);
      Type *castType = NULL;

      if (base.isLVPtr == GEN_WIDE_PTR) {
        // TODO remove for debugging
          if (!strcmp(c_field_name, "addr"))
              INT_FATAL("use Raddr to get the address of a wide pointer");
        // If we have a wide base pointer, generate a wide reference.
          ret = 
              codegenWideAddr(
                  codegenRlocale(base),
                  codegenFieldPtr(
                      codegenRaddr(base), 
                      c_field_name, 
                      chpl_field_name, 
                      special));
          return ret;
          }
      if (baseType) {
          if (hasFlag(baseType, FLAG_REF)) {
            // dereference base if needed 
              GenRet baseVal = codegenDeref(base);
              ret = 
                  codegenFieldPtr(
                      baseVal, 
                      c_field_name, 
                      chpl_field_name, 
                      special);
              return ret;
              }
        // for wide class base 
          if (hasEitherFlag(baseType, FLAG_WIDE, FLAG_WIDE_CLASS)) {
              GenRet baseVal = codegenValue(base);
              ret = 
                  codegenWideAddr(
                      codegenRlocale(baseVal),
                      codegenFieldPtr(
                          codegenRaddr(baseVal),
                          c_field_name, 
                          chpl_field_name, 
                          special));
              return ret;
              }
          }

    // No Chapel field name? it must be special.
      if (!chpl_field_name && !special) 
          special = field_other;

      if (special) {
          if (special == field_cid) {
              INT_ASSERT(isClass(ct));
              ret.chplType = CLASS_ID_TYPE;
              castType = dtObject;
              }
          else if (special == field_uid)
              ret.chplType = UNION_ID_TYPE;
          else
              ret.chplType = NULL;
          }
      else {
        // The field might be in a base class, so we
        // cast to the right base class type. If the field
        // is in the class, there is no need to cast.
          Symbol *fieldSymbol = ct->getField(chpl_field_name);
          castType = fieldSymbol->defPoint->parentSymbol->typeInfo();
          if (castType == ct) 
              castType = NULL;
          ret.chplType = fieldSymbol->typeInfo();
          }

      ret.isLVPtr = GEN_PTR;
      if (isClass(ct))
          base = codegenValue(base);
      else
        // not a class. base is a lvalue pointer.
          INT_ASSERT(base.isLVPtr == GEN_PTR);

// ACHTUNG: Target-specific part starts here
      ret.c = '&';
      ret.c += "(";
      if (castType)
          ret.c += codegenCast(castType, base).c;
      else 
          ret.c += "(" + base.c + ")";
      ret.c += "->";
      if (isUnion(ct) && !special)
          ret.c += "_u.";
      ret.c += c_field_name;
      ret.c += ")";
      return ret;
      }

// Generates code to perform an "assignment" operation, given
//  a destination pointer and a value.
// That's basically
//  (*)to_ptr = (*)from
// but for a homogeneous tuple, we will copy element-by-element
// or possibly call memcpy (in order to copy more that the first element).
// 
// If to_ptr or from_ptr is a wide reference type (but not both),
// we will generate a PUT or a GET.
//
// from_type is used (in C) to create a temporary in case that
// is needed.
// 
// This function will always copy some value. If that is not
// desired, other functions should be used.
//
// We handle ref to ref and value to value assignments. Use 
// codegenStoreRef for value to ref assignments.
  void GenC::codegenAssign(GenRet to_ptr, GenRet from) {

    // To must be a pointer.
      INT_ASSERT(to_ptr.isLVPtr);

    // Both can't be wide
      INT_ASSERT(
          !(to_ptr.isLVPtr == GEN_WIDE_PTR && 
              from.isLVPtr == GEN_WIDE_PTR));

      Type *type = from.chplType;
      if (!type) 
          type = to_ptr.chplType;
      INT_ASSERT(type);

      bool isStarTuple = hasFlag(type, FLAG_STAR_TUPLE);
      int starTupleLength = 0;
      if (isStarTuple) 
          starTupleLength = toClassType(type)->fields.length;

    // if from is a wide ptr a ref to dtNil, set from to
    // a nil pointer of the correct type.
      if (from.chplType && to_ptr.chplType) {
          ClassType *ct = toClassType(from.chplType);
          if (ct && hasEitherFlag(ct, FLAG_WIDE, FLAG_WIDE_CLASS)) {
              Symbol *valField = ct->getField("addr");
              if (valField && valField->getValType() == dtNil)
                  from = 
                      codegenAddrOf(
                          codegenWideHere(
                              codegenNullPointer(), to_ptr.chplType));
              }
          }

      if (to_ptr.isLVPtr != GEN_WIDE_PTR && from.isLVPtr != GEN_WIDE_PTR) {
        // Neither are wide.
          if (isStarTuple) {
            // Homogenous tuples are pointers even when GEN_VAL is set.
            // Homogeneous tuples are copied specially
              if (!fNoTupleCopyOpt &&
                      starTupleLength <= tuple_copy_limit &&
                      !isTupleOfTuple(type)) {
                // tuple copy optimization
                  int i = 0;
                  for_fields(field, toClassType(type)) {
                      GenRet to_i = codegenElementPtr(to_ptr, evalInt(i));
                      GenRet from_i = codegenElementPtr(from, evalInt(i));
                      codegenAssign(to_i, from_i);
                      i++;
                      }
                  }
              else {
                // tuple copy but use memcpy
                // to is already a pointer.
                  GenRet from_ptr = codegenValuePtr(from);
                  codegenCall(
                      "memcpy",
                      codegenAddrOf(to_ptr),
                      codegenAddrOf(from_ptr),
                      codegenSizeof(type));
                  }
              }
// ACHTUNG: start target-specific
          else {          
            // not a homogeneous tuple copy
              std::string stmt = codegenValue(to_ptr).c + " = ";
              stmt += codegenValue(from).c; 
              stmt += ";\n";
              writeStatement(stmt);
              }
          }
// end target-specific
      else {
        // both should not be wide
          if (from.isLVPtr == GEN_WIDE_PTR && to_ptr.isLVPtr == GEN_WIDE_PTR)
              INT_FATAL("Cannot assign two wide pointers");

        // One of the types is a wide pointer type, so we have to
        // call get or put.

          if (from.isLVPtr == GEN_WIDE_PTR) {
              INT_ASSERT(type);
            // would also be nice to call createTempVarWith to
            // store a temporary wide pointer so we don't get 
            // code like:
            //  chpl_comm_get(..., 
            //        ((wide type) {.locale = ..., .addr = ...}).locale, 
            //        ((wide type) {.locale = ..., .addr = ...}).addr, 
            //         ...);

            // Generate a GET
              codegenCall(
                  "chpl_gen_comm_get",
                  codegenCastToVoidStar(to_ptr),
                  codegenRnode(from),
                  codegenRaddr(from),
                  codegenSizeof(type),
                  genTypeStructureIndex(type->symbol),
                  codegenOne(),
                  evalInt(infoLineno()), 
                  evalStr(infoFilename()));
              }
          else {
            // Generate a PUT
            // to is already a pointer.
              codegenCall(
                  "chpl_gen_comm_put",
                  codegenCastToVoidStar(codegenValuePtr(from)),
                  codegenRnode(to_ptr),
                  codegenRaddr(to_ptr),
                  codegenSizeof(type),
                  genTypeStructureIndex(type->symbol),
                  codegenOne(),
                  evalInt(infoLineno()), 
                  evalStr(infoFilename()));
              } 
          }
      }

// Generates code to load the wide version of an address.
  GenRet GenC::codegenWideAddr(
          GenRet locale, GenRet raddr, Type *wideType/*= NULL*/) {
      GenRet ret;
      ret.isLVPtr = GEN_WIDE_PTR;
      Type *wideRefType = NULL;
      if (raddr.chplType && !wideType) {
          INT_ASSERT(raddr.isLVPtr != GEN_WIDE_PTR);
          Type *refType;
          if (raddr.isLVPtr == GEN_VAL) {
            // Then we should have a ref or a class.
              INT_ASSERT(raddr.chplType == dtNil ||
                  isClass(raddr.chplType) ||
                  hasFlag(raddr.chplType, FLAG_REF));
              refType = raddr.chplType;
              }
          else
            // GEN_REF
              refType = getOrMakeRefTypeDuringCodegen(raddr.chplType);
          wideRefType = getOrMakeWideTypeDuringCodegen(refType);
          INT_ASSERT(wideRefType);
          }
      else
          wideRefType = wideType;

      INT_ASSERT(wideRefType);
      ret.chplType = wideRefType->getValType();     // or raddr.chplType; ?

// ACHTUNG: start target-specific

      ret.c += "(" + Type_codegen(wideRefType).c + ")";

    // This requires C99 - we could also do it with an
    // inline function and a cast, or with a local
    // variable and some stores.
      ret.c += "{ .locale = " + locale.c + ", .addr = " + raddr.c + " }";
      return ret;
      }

// Generates code to load the remote address from a wide address
// Returns an l-value to the local reference (address field)
// unless the wide pointer is not an lvalue, and then it
// will return a value.
  GenRet GenC::codegenRaddrMaybePtr(GenRet wide) {
      GenRet ret;
      if (wide.chplType) {
        // Set the resulting Chapel type.
          if (wide.isLVPtr == GEN_WIDE_PTR)
            // wide lv-pointer, e.g. to int,
            // so we return a reference to int.
              ret.chplType = getOrMakeRefTypeDuringCodegen(wide.chplType);
          else {
              if (hasEitherFlag(wide.chplType, FLAG_WIDE, FLAG_WIDE_CLASS))
                  ret.chplType = getFieldType(wide.chplType, "addr");
              else
                  INT_ASSERT(0);
              }
          }

// ACHTUNG: start target-specific

      if (wide.isLVPtr == GEN_PTR) {
          ret.isLVPtr = GEN_PTR;
          ret.c += "&((" + wide.c + ")->addr)";
          }
      else {
        // we don't have an l-value for this one.
        // Could be wide lv-ptr or GEN_VAL wide ref.
          ret.isLVPtr = GEN_VAL;
          ret.c += "(" + wide.c + ").addr";
          }
      return ret;
      }

// Generates code to load the remote locale from a wide address
// whether it is a wide class or a wide reference
  GenRet GenC::codegenRlocaleMaybePtr(GenRet wide) {
      GenRet ret;
      Type *type = LOCALE_ID_TYPE;
      ret.chplType = type;

      if (wide.isLVPtr == GEN_PTR) {
          ret.isLVPtr = GEN_PTR;
          ret.c += "&((" + wide.c + ")->locale)";
          }
      else {
        // we don't have an l-value
        // it could be a wide-lv-ptr or a GEN_VAL wide ref.
          ret.isLVPtr = GEN_VAL;
          ret.c += "(" + wide.c + ").locale";
          }
      return ret;
      }

// implementation (target-specific)

  GenRet GenC::codegenEquals(GenRet a, GenRet b) {
      GenRet ret;
      GenRet av = codegenValue(a);
      GenRet bv = codegenValue(b);
      ret.chplType = dtBool;
      ret.c = "(" + av.c + " == " + bv.c + ")";
      return ret;
      }

  GenRet GenC::codegenNotEquals(GenRet a, GenRet b) {
      GenRet ret;
      GenRet av = codegenValue(a);
      GenRet bv = codegenValue(b);
      ret.chplType = dtBool;
      ret.c = "(" + av.c + " != " + bv.c + ")";
      return ret;
      }

  GenRet GenC::codegenLogicalOr(GenRet a, GenRet b) {
      GenRet ret;
      GenRet av = codegenValue(a);
      GenRet bv = codegenValue(b);
      ret.chplType = dtBool;
      ret.c = "(" + av.c + " || " + bv.c + ")";
      return ret;
      }
      
  GenRet GenC::codegenLogicalAnd(GenRet a, GenRet b) {
      GenRet ret;
      GenRet av = codegenValue(a);
      GenRet bv = codegenValue(b);
      ret.chplType = dtBool;
      ret.c = "(" + av.c + " && " + bv.c + ")";
      return ret;
      }

  GenRet GenC::codegenAdd(GenRet a, GenRet b) {
      GenRet ret;
      GenRet av = codegenValue(a);
      GenRet bv = codegenValue(b);
      ret.c = "(" + av.c + " + " + bv.c + ")";
      return ret;
      }

  GenRet GenC::codegenSub(GenRet a, GenRet b) {
      GenRet ret;
      GenRet av = codegenValue(a);
      GenRet bv = codegenValue(b);
      ret.c = "(" + av.c + " - " + bv.c + ")";
      return ret;
      }

  GenRet GenC::codegenNeg(GenRet a) {
      GenRet ret;
      GenRet av = codegenValue(a);
      ret.c = "(-" + av.c + ")";
      return ret;
      }

  GenRet GenC::codegenMul(GenRet a, GenRet b) {
      GenRet ret;
      GenRet av = codegenValue(a);
      GenRet bv = codegenValue(b);
      ret.c = "(" + av.c + " * " + bv.c + ")";
      return ret;
      }

  GenRet GenC::codegenTernary(GenRet cond, GenRet ifTrue, GenRet ifFalse) {
      GenRet ret;
      Type *type = ifTrue.chplType;
      if (!type) 
          type = ifFalse.chplType;
      ret.chplType = type;
      ret.c = "(" + cond.c + ")?(" + ifTrue.c + "):(" + ifFalse.c + ")";
      return ret;
      }

// AKA == null
  GenRet GenC::codegenIsZero(GenRet x) {
      GenRet ret;
      if (hasEitherFlag(x.chplType, FLAG_WIDE, FLAG_WIDE_CLASS)) {
          x = codegenRaddr(x);
          ret.c = x.c;
          ret.c += " == nil";
          }
      else {
          GenRet xv = codegenValue(x);
          ret.c = "(! " + xv.c + ")";
          }
      return ret;
      }

// AKA != null
  GenRet GenC::codegenIsNotZero(GenRet x) {
      GenRet ret;
      if (hasEitherFlag(x.chplType, FLAG_WIDE, FLAG_WIDE_CLASS)) {
          x = codegenRaddr(x);
          ret.c = x.c;
          ret.c += " != nil";
          }
      else { 
          GenRet xv = codegenValue(x);
          ret.c = "(!(! " + xv.c + "))";
          }
      return ret;
      }

  GenRet GenC::createTempVar(const char *ctype) {
      GenRet ret;
      char name[32];
      sprintf(name, "chpl_macro_tmp_%d", codegen_tmp++);

      ret.isLVPtr = GEN_PTR;
    // Add a temporary variable
      writeLocalDecl(std::string(ctype) + " " + name);
      ret.c = std::string("&") + name;
      return ret;
      }

  GenRet GenC::createTempVarWith(GenRet v) {
      Type *t = v.chplType;
      INT_ASSERT(t);
      GenRet ret = createTempVar(t);
      ret.isUnsigned = v.isUnsigned;
    // now store into the temp var the value we have.
      std::string stmt = 
          codegenValue(ret).c + " = " + codegenValue(v).c + ";\n";
      writeStatement(stmt);
      return ret;
      }

// For C code generation
// Codegen *(ptr), but we optimize away
// & or &(something)
// if GenRet is a wide pointer, we will emit a 'get'.
// For a star tuple, if we have a reference to a star tuple,
// returns the base address.
  GenRet GenC::codegenValue(GenRet r) {
      GenRet ret = r;
      ret.isLVPtr = GEN_VAL;

      if (r.isLVPtr == GEN_VAL) 
          return ret;
      if (r.isLVPtr == GEN_WIDE_PTR) {
          assert(r.chplType);

        // Emit a temporary.
        // Assign from wide pointer value into temporary
        // Return local pointer to temporary
          ret = createTempVar(r.chplType);
          codegenAssign(ret, r);
          return codegenValue(ret);
          }

    // At this point r.isPtr == GEN_PTR.
      if (r.chplType) {
        // If we have a Chapel type, propagate it.
          ret.chplType = r.chplType;
        // NOT value type if it's a reference, since
        // codegenValue on a Chapel reference just returns the pointer!
          }

      INT_ASSERT(r.c.length() > 0);
      if (r.c[0] == '&') {
          if (r.c[1] == '(' && r.c[r.c.length()-1] == ')')
            // we have &(something)
              ret.c = r.c.substr(2, r.c.length()-3);
          else
            // we have &something
              ret.c = r.c.substr(1, r.c.length()-1);
          }
      else if (r.c[0] == '(' && r.c[r.c.length()-1] == ')')
        // we have (something)
          ret.c = "*" + r.c;
      else
          ret.c = "*(" + r.c + ")";
      return ret;
      }

  GenRet GenC::codegenNullPointer() {
      GenRet ret;
      ret.chplType = dtNil;
      ret.c = "NULL";
      return ret;
      }

  GenRet GenC::codegenSizeof(const char *name) {
      GenRet ret; 
      ret.chplType = SIZE_TYPE;
      ret.c = "sizeof(";
      ret.c += name;
      ret.c += ')';
      return ret;
      }

// MPF - perhaps this should always call codegenValue
// and return isLVPtr = GEN_VAL.
  GenRet GenC::codegenCast(Type *t, GenRet value, bool Cparens/*=true*/) {
      GenRet ret;
      ret.chplType = t;
      ret.isLVPtr = value.isLVPtr;

    // if we are casting a C99 wide pointer, parens around the value
    // will result in an error, hence the Cparens parameter
    // e.g. ((chpl____wide_DefaultRectangularArr_locale_1_int64_t_F)(
    //         { .locale = chpl_nodeID, .addr = nil }))
    // won't compile

      ret.c = "((" + Type_codegen(t).c + ")";
      if (Cparens)
          ret.c += "(";
      ret.c += value.c;
      if (Cparens)
          ret.c += ")";
      ret.c += ")";
      return ret;
      }

  GenRet GenC::codegenCast(
          const char *typeName, GenRet value, bool Cparens/*=true*/) {
      GenRet ret;
      ret.isLVPtr = value.isLVPtr;
      ret.chplType = getNamedType(std::string(typeName));

      ret.c = "((";
      ret.c += typeName; 
      ret.c += ")";

      if (Cparens)
          ret.c += "(";
      ret.c += value.c;
      if (Cparens)
          ret.c += ")";
      ret.c += ")";

    /*
      ret.c = "((";
      ret.c += typeName;
      ret.c += ")(";
      ret.c += value.c;
      ret.c += "))";
    */
      return ret;
      }
      
  GenRet GenC::codegenCastToVoidStar(GenRet value) {
      GenRet ret;
      ret.c = "((void*)(";
      ret.c += value.c;
      ret.c += "))";
      return ret;
      }

  GenRet GenC::codegenCastToCharStar(GenRet value) {
      GenRet ret;
      ret.c = "((char*)(";
      ret.c += value.c;
      ret.c += "))";
      return ret;
      }

// if fSym is non-NULL, we use that to decide what to dereference.
// Otherwise, if defaultToValues=true, we will codegenValue() the arguments,
//            and if it is false, they will pass by reference if they
//            are references.
  GenRet GenC::codegenCallExpr(
          GenRet function,
          int num,
          const GenRet *args,
          FnSymbol *fSym,
          bool defaultToValues) {
      GenRet ret;

      ret.c = function.c;
      ret.c += '(';
      bool first_actual = true;
      for (int i = 0; i < num; i++) {
        // Convert formals if we have fSym
          ArgSymbol *formal = NULL;
          bool isExtern = true;
          if (fSym) {
              Expr *e = fSym->formals.get(i+1);
              DefExpr *de = toDefExpr(e);
              formal = toArgSymbol(de->sym);
              INT_ASSERT(formal);
              if (!fSym->hasFlag(FLAG_EXTERN))
                  isExtern = false;
              }
          GenRet temp =
              codegenArgForFormal(
                  args[i], formal, defaultToValues, isExtern);

          if (first_actual)
              first_actual = false;
          else
              ret.c += ", ";
          ret.c += temp.c;
          }
      ret.c += ')';
      return ret;
      }

  GenRet GenC::codegenLocaleID(GenRet node, GenRet subloc) {
      GenRet ret;
      ret.c = "(" + Type_codegen(dtLocaleID).c + ")";
      ret.c += "{ .node = " + node.c + ", .subloc = " + subloc.c + " }";
      ret.chplType = LOCALE_ID_TYPE;
      return ret;
      }

// implementation: helpers for calls (target-independent)

  GenRet GenC::codegenCallExpr(
          const char *fnName,
          int num,
          const GenRet *args,
          bool defaultToValues/*=true*/) {
      GenRet fn;
      fn.c = fnName;
      return codegenCallExpr(fn, num, args, NULL, defaultToValues);
      }

  void GenC::codegenCall(const char *fnName, int num, const GenRet *args) {
      GenRet ret = codegenCallExpr(fnName, num, args, true);
      writeStatement(ret.c + ";\n");
      }

/*
 * These overloads of codegenCall are a bit boring-looking,
 * but they make it much easier to write the primitive call
 * generation in Expr::codegen
 */
  GenRet GenC::codegenCallExpr(const char *fnName) {
      return codegenCallExpr(fnName, 0, NULL);
      }
      
  GenRet GenC::codegenCallExpr(const char *fnName, GenRet a1) {
      GenRet args[1];
      args[0] = a1;
      return codegenCallExpr(fnName, 1, args);
      }
      
  GenRet GenC::codegenCallExpr(const char *fnName, GenRet a1, GenRet a2) {
      GenRet args[2];
      args[0] = a1;
      args[1] = a2;
      return codegenCallExpr(fnName, 2, args);
      }
      
  GenRet GenC::codegenCallExpr(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3) {
      GenRet args[3];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      return codegenCallExpr(fnName, 3, args);
      }

  GenRet GenC::codegenCallExpr(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3, 
          GenRet a4) {
      GenRet args[4];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      args[3] = a4;
      return codegenCallExpr(fnName, 4, args);
      }

  GenRet GenC::codegenCallExpr(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5) {
      GenRet args[5];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      args[3] = a4;
      args[4] = a5;
      return codegenCallExpr(fnName, 5, args);
      }

/* 
  void GenC::codegenCall(const char *fnName) {
      codegenCall(fnName, 0, NULL);
      }
*/

  void GenC::codegenCall(const char *fnName, GenRet a1) {
      GenRet args[1];
      args[0] = a1;
      codegenCall(fnName, 1, args);
      }
      
  void GenC::codegenCall(const char *fnName, GenRet a1, GenRet a2) {
      GenRet args[2];
      args[0] = a1;
      args[1] = a2;
      codegenCall(fnName, 2, args);
      }

  void GenC::codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3) {
      GenRet args[3];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      codegenCall(fnName, 3, args);
      }

  void GenC::codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3, 
          GenRet a4) {
      GenRet args[4];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      args[3] = a4;
      codegenCall(fnName, 4, args);
      }

  void GenC::codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5) {
      GenRet args[5];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      args[3] = a4;
      args[4] = a5;
      codegenCall(fnName, 5, args);
      }
      
/*
  void GenC::codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6) {
      GenRet args[6];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      args[3] = a4;
      args[4] = a5;
      args[5] = a6;
      codegenCall(fnName, 6, args);
      }
      
  void GenC::codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6, 
          GenRet a7) {
      GenRet args[7];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      args[3] = a4;
      args[4] = a5;
      args[5] = a6;
      args[6] = a7;
      codegenCall(fnName, 7, args);
      }
*/

  void GenC::codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6, 
          GenRet a7, 
          GenRet a8) {
      GenRet args[8];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      args[3] = a4;
      args[4] = a5;
      args[5] = a6;
      args[6] = a7;
      args[7] = a8;
      codegenCall(fnName, 8, args);
      }
      
/*
  void GenC::codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6, 
          GenRet a7, 
          GenRet a8,
          GenRet a9) {
      GenRet args[9];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      args[3] = a4;
      args[4] = a5;
      args[5] = a6;
      args[6] = a7;
      args[7] = a8;
      args[8] = a9;
      codegenCall(fnName, 9, args);
      }

  void GenC::codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6, 
          GenRet a7, 
          GenRet a8,
          GenRet a9, 
          GenRet a10) {
      GenRet args[10];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      args[3] = a4;
      args[4] = a5;
      args[5] = a6;
      args[6] = a7;
      args[7] = a8;
      args[8] = a9;
      args[9] = a10;
      codegenCall(fnName, 10, args);
      }
*/

  void GenC::codegenCall(
          const char *fnName, 
          GenRet a1, 
          GenRet a2, 
          GenRet a3,
          GenRet a4, 
          GenRet a5, 
          GenRet a6, 
          GenRet a7, 
          GenRet a8,
          GenRet a9, 
          GenRet a10, 
          GenRet a11) {
      GenRet args[11];
      args[0] = a1;
      args[1] = a2;
      args[2] = a3;
      args[3] = a4;
      args[4] = a5;
      args[5] = a6;
      args[6] = a7;
      args[7] = a8;
      args[8] = a9;
      args[9] = a10;
      args[10] = a11;
      codegenCall(fnName, 11, args);
      }
