//
//    PRIM.CPP -- Code generator (IR back-end)
//
//        Primitive expressions
//
//    Copyright (c) 2004-2013, Cray Inc.
//    Copyright (c) 2013, Unicorn Enterprises SA
//

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

#include "irbase.h"
#include "genir.h"

//
//    GenIR
//

// implementation: primitive expressions

  GenRet GenIR::primUnknown(CallExpr *self) {
      return codegenBasicPrimitiveExpr(self);
      }
      
  void GenIR::primArraySet(CallExpr *self) {
    // get(1): (wide?) base pointer
    // get(2): index
    // get(3): value
    // get(4): src-line
    // get(5): src-file

    // Used to handle FLAG_WIDE_CLASS/FLAG_STAR_TUPLE specially,
    // but these should be taken care of by codegenElementPtr and
    // codegenAssign now.
      GenRet elementPtr = 
          codegenElementPtr(evalArg(self, 1), evalArg(self, 2));
      codegenAssign(elementPtr, evalArg(self, 3));
      }

  void GenIR::primArrayAlloc(CallExpr *self) {
      Expr *a1 = self->get(1);
      GenRet dst = eval(a1);
      GenRet alloced;
      INT_ASSERT(dst.isLVPtr);
      if (hasFlag(a1, FLAG_WIDE_CLASS)) {
          Type *eltType =
              getDataClassType(getFieldType(a1, "addr")->symbol)->typeInfo();
          GenRet locale = codegenRlocale(dst);
          GenRet call = 
              codegenCallExpr(
                  "chpl_wide_array_alloc",
                  codegenRnode(dst),
                  codegenValue(evalArg(self, 3)),
                  codegenSizeof(eltType),
                  evalArg(self, 4), 
                  evalArg(self, 5));
          call.chplType = a1->typeInfo(); 
          alloced = 
              codegenAddrOf(
                  codegenWideAddr(locale, call, call.chplType));
          }
      else {
          Type *eltType =
              getDataClassType(a1->typeInfo()->symbol)->typeInfo();
          alloced = 
              codegenCallExpr(
                  "chpl_array_alloc", 
                  codegenValue(evalArg(self, 3)),
                  codegenSizeof(eltType), 
                  evalArg(self, 4), 
                  evalArg(self, 5));
          }
      codegenAssign(dst, alloced);
      }

  void GenIR::primGpuAlloc(CallExpr *self) {
      Expr *a1 = self->get(1);
      if (hasFlag(a1, FLAG_WIDE_CLASS))
          codegenCall(
              "_GPU_ALLOC",
              eval(a1),
              evalSym(getDataClassType(getFieldType(a1, "addr")->symbol)),
              evalArg(self, 3));
      else
          codegenCall(
              "_GPU_ALLOC",
              eval(a1),
              ClassType_codegenClassStructType(toClassType(a1->typeInfo())),
              evalArg(self, 2),
              evalSym(self->get(3)->typeInfo()->symbol));
      }
      
  void GenIR::primCopyHostGpu(CallExpr *self) {
      codegenCall(
          "_GPU_COPY_HOST_GPU", 
          evalArg(self, 1), 
          evalArg(self, 2), 
          evalArg(self, 3), 
          evalArg(self, 4));
      }
      
  void GenIR::primCopyGpuHost(CallExpr *self) {
      codegenCall(
          "_GPU_COPY_GPU_HOST", 
          evalArg(self, 1), 
          evalArg(self, 2), 
          evalArg(self, 3), 
          evalArg(self, 4));
      }
      
  void GenIR::primGpuFree(CallExpr *self) {
      if (fNoMemoryFrees)
          return;
      codegenCall("_GPU_FREE", evalArg(self, 1));
      }
      
  void GenIR::primArrayFree(CallExpr *self) {
      if (fNoMemoryFrees)
          return;
      Expr *a1 = self->get(1);
      GenRet data = eval(a1);
      if (hasFlag(a1, FLAG_WIDE_CLASS)) {
          GenRet node = codegenRnode(data);
          GenRet ptr = codegenRaddr(data);
          codegenCall(
              "chpl_wide_array_free", 
              node, 
              ptr, 
              evalArg(self, 2), 
              evalArg(self, 3));
          }
      else
          codegenCall(
              "chpl_array_free", 
              data, 
              evalArg(self, 2), 
              evalArg(self, 3));
      }

  void GenIR::primArrayFreeElts(CallExpr *self) {
      if (fNoMemoryFrees)
          return;
    // This used to run a macro like this:
    // for (i = 0; i < (x)->size; i++) call
      INT_FATAL("PRIM_ARRAY_FREE_ELTS");
      }

  GenRet GenIR::primMove(CallExpr *self) {
      GenRet ret;
      Expr *left = self->get(1);
      Expr *right = self->get(2);
      if (left->typeInfo() == dtVoid) {
          ret = Expr_codegen(right);
          return ret;
          }
      CallExpr *call = toCallExpr(right);
      if (call != NULL) {
          Expr *r1 = (call->numActuals() > 0) ? call->get(1) : NULL;
          if (call->isPrimitive(PRIM_WIDE_GET_LOCALE)) {
              Type *type = dtLocaleID->typeInfo();
              if (hasFlag(r1, FLAG_WIDE)) {
                  if (hasFlag(r1->getValType(), FLAG_WIDE_CLASS)) {
                    // TODO: Is the remote get really necessary?
                    // i.e. will the localeID field of the remotely fetched pointer
                    // ever differ from that in our local copy of the pointer?
                    // I hope not.  Otherwise my understanding of a wide pointer
                    // is "incomplete". <hilde>

                    // To implement the test, replace the result of a get with a
                    // test that codegenRlocale(r1) == the current result,
                    // and if false
                    // print a runtime error.  If the error code never fires, we can
                    // just codegen the "else" form, i.e. :
                    //   codegenAssign(left, codegenRlocale(r1));

                    // get locale field of wide class via wide reference
                      GenRet locPtr = createTempVar(r1->typeInfo());
                    // used to be CHPL_COMM_WIDE_GET_LOCALE
                      codegenCall(
                          "chpl_gen_comm_get",
                          codegenCastToVoidStar(codegenLocalAddrOf(locPtr)), 
                          codegenRnode(eval(r1)), 
                          codegenRaddr(eval(r1)), 
                          codegenSizeof(type),
                          genTypeStructureIndex(type->symbol),
                          codegenOne(), 
                          evalArg(call, 2), 
                          evalArg(call, 3));
                      codegenAssign(eval(left), codegenRlocale(locPtr));
                      }
                  else
                      codegenAssign(eval(left), codegenRlocale(eval(r1)));
                  }
              else if (hasFlag(r1, FLAG_WIDE_CLASS))
                  codegenAssign(eval(left), codegenRlocale(eval(r1)));
              else
                // Needs to assign both fields.
                  codegenAssign(eval(left), codegenGetLocaleID());
              return ret;
              }
          if (call->isPrimitive(PRIM_WIDE_GET_NODE)) {
              Type *type = dtLocaleID->getField("node")->typeInfo();
              if (hasFlag(r1, FLAG_WIDE)) {
                  if (hasFlag(r1->getValType(), FLAG_WIDE_CLASS)) {
                    // get locale field of wide class via wide reference
                      GenRet locPtr = createTempVar(r1->typeInfo());
                    // used to be CHPL_COMM_WIDE_GET_LOCALE
                      codegenCall(
                          "chpl_gen_comm_get",
                          codegenCastToVoidStar(codegenLocalAddrOf(locPtr)), 
                          codegenRnode(eval(r1)), 
                          codegenRaddr(eval(r1)), 
                          codegenSizeof(type),
                          genTypeStructureIndex(type->symbol),
                          codegenOne(), 
                          evalArg(call, 2), 
                          evalArg(call, 3));
                      codegenAssign(eval(left), codegenRnode(locPtr));
                      }
                  else
                      codegenAssign(eval(left), codegenRnode(eval(r1)));
                  }
              else if (hasFlag(r1, FLAG_WIDE_CLASS))
                  codegenAssign(eval(left), codegenRnode(eval(r1)));
              else
                  codegenAssign(eval(left), codegenGetNodeID());
              return ret;
              }
          if (call->isPrimitive(PRIM_WIDE_GET_SUBLOC)) {
              Type *type = dtLocaleID->getField("subloc")->typeInfo();
              if (hasFlag(r1, FLAG_WIDE)) {
                  if (hasFlag(r1->getValType(), FLAG_WIDE_CLASS)) {
                    // get locale field of wide class via wide reference
                      GenRet locPtr = createTempVar(r1->typeInfo());
                    // used to be CHPL_COMM_WIDE_GET_LOCALE
                      codegenCall(
                          "chpl_gen_comm_get",
                          codegenCastToVoidStar(codegenLocalAddrOf(locPtr)), 
                          codegenRnode(eval(r1)), 
                          codegenRaddr(eval(r1)), 
                          codegenSizeof(type),
                          genTypeStructureIndex(type->symbol),
                          codegenOne(), 
                          evalArg(call, 2), 
                          evalArg(call, 3));
                      codegenAssign(eval(left), codegenRsubloc(locPtr));
                      }
                  else
                      codegenAssign(eval(left), codegenRsubloc(eval(r1)));
                  }
              else if (hasFlag(r1, FLAG_WIDE_CLASS))
                  codegenAssign(eval(left), codegenRsubloc(eval(r1)));
              else
                  codegenAssign(eval(left), codegenGetSublocID());
              return ret;
              }
          if (call->isPrimitive(PRIM_DEREF)) {
              if (hasFlag(r1, FLAG_WIDE) || hasFlag(r1, FLAG_WIDE_CLASS)) {
                  Type *valueType;
                  if (hasFlag(r1, FLAG_WIDE))
                      valueType = r1->getValType();
                  else
                      valueType = getFieldType(r1, "addr");
                  INT_ASSERT(valueType == left->typeInfo());
                  if (valueType == dtString) {
                      codegenCall(
                          "chpl_comm_wide_get_string",
                          codegenLocalAddrOf(eval(left)),
                          codegenLocalAddrOf(eval(r1)), 
                          genTypeStructureIndex(valueType->symbol),
                          evalArg(call, 2),
                          evalArg(call, 3));
                      }
                  else
                    // set left = *(r1);
                      codegenAssign(eval(left), codegenDeref(eval(r1)));
                  }
              else if (hasFlag(left, FLAG_STAR_TUPLE) ||
                      hasFlag(left, FLAG_FIXED_STRING))
                // star tuple handled in codegenAssign
                // set left = *(r1);
                  codegenAssign(eval(left), codegenDeref(eval(r1)));
              else if (r1->typeInfo() == dtString)
                // this should be illegal when wide strings are fixed
                // set left = r1;
                  codegenAssign(eval(left), eval(r1));
              else
                // set left = *(r1);
                  codegenAssign(eval(left), codegenDeref(eval(r1)));
              return ret;
              }
          if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
              SymExpr *se = toSymExpr(call->get(2));
              if (hasFlag(r1, FLAG_WIDE_CLASS)) {
                  INT_ASSERT(se);
                  if (se->var->hasFlag(FLAG_SUPER_CLASS)) {
                    // We're getting the super class pointer.
                      GenRet srcwide = eval(r1);
                      GenRet addr = 
                          codegenCast(
                              getFieldType(left, "addr"),
                              codegenRaddr(srcwide));
                      GenRet ref = 
                          codegenAddrOf(
                              codegenWideAddr(codegenRlocale(srcwide), addr));
                      codegenAssign(eval(left), ref);
                      }
                  else
                      codegenAssign(
                          eval(left), codegenFieldPtr(eval(r1), se));
                  }
              else if (hasFlag(r1, FLAG_WIDE)) {
                  INT_ASSERT(se);
                // codegenAssign will dereference.
                  codegenAssign(
                      eval(left), codegenFieldPtr(eval(r1), se));
                  }
              else if (hasFlag(right, FLAG_STAR_TUPLE)) {
                  INT_ASSERT(se);
                // codegenAssign will handle star tuple
                  codegenAssign(
                      eval(left), codegenFieldPtr(eval(r1), se));
                  }
              else {
                  INT_ASSERT(se);
                  if (se->var->hasFlag(FLAG_SUPER_CLASS)) {
                    // We're getting the super class pointer.
                      GenRet ref = codegenFieldPtr(eval(r1), se);
                    // Now we have a field pointer to object->super, but
                    // the pointer to super *is* actually the value of
                    // the super class. So we just set isPtr to Value.
                      ref.isLVPtr = GEN_VAL; 
                      codegenAssign(eval(left), ref);
                      }
                  else
                      codegenAssign(
                          eval(left), codegenFieldPtr(eval(r1), se));
                  }
              return ret;
              }
          if (call->isPrimitive(PRIM_GET_MEMBER)) {
            // Get a pointer to a member
              SymExpr *se = toSymExpr(call->get(2));
              if (hasFlag(r1, FLAG_WIDE_CLASS)) {
                  codegenAssign(
                      eval(left), 
                      codegenAddrOf(codegenFieldPtr(eval(r1), se)));
                  return ret;
                  }
              else if (hasFlag(r1, FLAG_WIDE)) {
                  codegenAssign(
                      eval(left), 
                      codegenAddrOf(codegenFieldPtr(eval(r1), se)));
                  return ret;
                  }
              else if (hasFlag(right, FLAG_STAR_TUPLE)) {
                  codegenAssign(
                      eval(left), 
                      codegenAddrOf(codegenFieldPtr(eval(r1), se)));
                  return ret;
                  } 
              }
          if (call->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
              if (hasFlag(r1, FLAG_WIDE)) {
                // Get a pointer to the i'th element of a homogenous tuple
                  GenRet elemPtr = 
                      codegenElementPtr(
                          eval(r1), 
                          codegenExprMinusOne(call->get(2)));
                  elemPtr = codegenAddrOf(elemPtr);
                  codegenAssign(eval(left), elemPtr);
                  return ret;
                  }
              }
          if (call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE)) {
            // Get the i'th value from a homogeneous tuple
              if (hasFlag(r1, FLAG_WIDE))
                // codegenElementPtr/codegenAssign handle wide pointers
                  codegenAssign(
                      eval(left),
                      codegenElementPtr(
                          eval(r1), 
                          codegenExprMinusOne(call->get(2))));
              else
                  codegenAssign(
                      eval(left),
                      codegenElementPtr(
                          eval(r1), 
                          codegenExprMinusOne(call->get(2))));
              return ret;
              }
          if (call->isPrimitive(PRIM_ARRAY_GET)) {
            // Get a pointer to the i'th array element
              if (hasFlag(r1, FLAG_WIDE_CLASS))
                  codegenAssign(
                      eval(left),
                      codegenAddrOf(
                          codegenElementPtr(eval(r1), evalArg(call, 2))));
              else
                  codegenAssign(
                      eval(left),
                      codegenAddrOf(
                          codegenElementPtr(eval(r1), evalArg(call, 2))));
              return ret;
              }
          if (call->isPrimitive(PRIM_ARRAY_GET_VALUE)) {
              if (hasFlag(r1, FLAG_WIDE_CLASS))
                  codegenAssign(
                      eval(left),
                      codegenElementPtr(eval(r1), evalArg(call, 2)));
              else
                  codegenAssign(
                      eval(left),
                      codegenElementPtr(eval(r1), evalArg(call, 2)));
              return ret;
              }
          if (call->isPrimitive(PRIM_GPU_GET_VALUE)) {
              codegenCall(
                  "_GPU_GET_VALUE", 
                  eval(left), 
                  eval(r1),
                  evalArg(call, 2));
              return ret;
              }
          if (call->isPrimitive(PRIM_GPU_GET_VAL)) {
              codegenCall(
                  "_GPU_GET_VAL", 
                  eval(left), 
                  eval(r1), 
                  evalArg(call, 2));
              return ret;
              }
          if (call->isPrimitive(PRIM_GPU_GET_ARRAY)) {
              codegenCall("_GPU_GET_ARRAY", eval(left), eval(r1));
              return ret;
              }
          if (call->isPrimitive(PRIM_GET_UNION_ID)) {
              if (hasFlag(r1, FLAG_WIDE)) {
                  codegenAssign(
                      eval(left), codegenFieldUidPtr(eval(r1)));
                  return ret;
                  }
              }
          if (call->isPrimitive(PRIM_TESTCID)) {
            // set left to
            //   r1->chpl_class_id == chpl__cid_"r2"
              if (hasFlag(r1, FLAG_WIDE_CLASS)) {
                  GenRet tmp = codegenFieldCidPtr(eval(r1));
                  codegenAssign(
                      eval(left),
                      codegenEquals(
                          tmp, codegenUseCid(call->get(2)->typeInfo())));
                  return ret;
                  }
              }
          if (call->isPrimitive(PRIM_GETCID)) {
              if (hasFlag(r1, FLAG_WIDE_CLASS)) {
                  GenRet tmp = codegenFieldCidPtr(eval(r1));
                  codegenAssign(eval(left), tmp);
                  return ret;
                  }
              }
          if (call->isPrimitive(PRIM_CAST)) {
              if (hasFlag(call, FLAG_WIDE_CLASS) ||
                      hasFlag(call, FLAG_WIDE)) {
                  GenRet tmp = evalArg(call, 2);
                  tmp = 
                      codegenWideAddr(
                          codegenRlocale(tmp),
                          codegenCast(
                              r1->typeInfo(), 
                              codegenRaddr(tmp)));
                  codegenAssign(eval(left), codegenAddrOf(tmp));
                  return ret;
                  }
              }
          if (call->isPrimitive(PRIM_DYNAMIC_CAST)) {
              if (hasFlag(call, FLAG_WIDE_CLASS)) {
                  Type *type = getFieldType(call, "addr");
                  GenRet wideFrom = codegenValue(evalArg(call, 2));
                  GenRet wideFromAddr = codegenRaddr(wideFrom);
                  GenRet wideFromLocale = codegenRlocale(wideFrom);
                  GenRet value = codegenValue(codegenFieldCidPtr(wideFrom));
                  GenRet ok = codegenDynamicCastCheck(value, type);
                // if ok, (typeInfo()->symbol->cname) (value)
                // otherwise, NULL
                  GenRet cast = codegenCast(type, wideFromAddr);
                  GenRet nul = codegenCast(type, codegenNullPointer());
                  GenRet addr = codegenTernary(ok, cast, nul);
                  GenRet wide = 
                      codegenAddrOf(
                          codegenWideAddr(
                              wideFromLocale, addr, call->typeInfo()));
                  codegenAssign(eval(left), wide);
                  return ret;
                  }
              }
          if (call->isPrimitive(PRIM_GET_PRIV_CLASS)) {
              GenRet r = 
                  codegenCallExpr(
                      "chpl_getPrivatizedClass", evalArg(call, 2));
              if (hasFlag(left, FLAG_WIDE_CLASS))
                  r = 
                      codegenAddrOf(
                          codegenWideHere(r, left->typeInfo()));
              codegenAssign(eval(left), r);
              return ret;
              }
          }
          
      if (hasFlag(left, FLAG_WIDE_CLASS) && 
              !hasFlag(right, FLAG_WIDE_CLASS)) {
          if (right->typeInfo() != dtString)
              codegenAssign(
                  eval(left), 
                  codegenAddrOf(
                      codegenWideHere(eval(right))));
          else
              codegenCall(
                  "chpl_string_widen", 
                  codegenAddrOf(eval(left)), 
                  eval(right)); 
          return ret;
          }
      if (hasFlag(left, FLAG_WIDE) && hasFlag(right, FLAG_REF)) {
          codegenAssign(
              eval(left), 
              codegenAddrOf(
                  codegenWideHere(eval(right))));
          return ret;
          }
      if (hasFlag(left, FLAG_WIDE) &&
              !hasFlag(right, FLAG_WIDE) &&
              !hasFlag(right, FLAG_REF)) {
          GenRet to_ptr = codegenDeref(eval(left));
          codegenAssign(to_ptr, eval(right));
          return ret;
          }
      if (hasFlag(left, FLAG_REF) && hasFlag(right, FLAG_WIDE)) {
        // left = Raddr(right);
          codegenAssign(eval(left), codegenRaddr(eval(right))); 
          return ret;
          }
      if (!hasFlag(left, FLAG_WIDE_CLASS) &&
              !hasFlag(left, FLAG_REF) &&
              hasFlag(right, FLAG_WIDE_CLASS)) {
        // left = Raddr(right);
          codegenAssign(eval(left), codegenRaddr(eval(right))); 
          return ret;
          }
      if (hasFlag(right, FLAG_STAR_TUPLE) || 
              hasFlag(right, FLAG_FIXED_STRING)) {
          if (hasFlag(left, FLAG_REF))
            // codegenAssign handles tuple copy opt
              codegenAssign(
                  codegenDeref(eval(left)), eval(right));
          else
            // codegenAssign handles tuple copy opt
              codegenAssign(eval(left), eval(right)); 
          return ret;
          }
      if (hasFlag(left, FLAG_REF) && !hasFlag(right, FLAG_REF))
          codegenAssign(
              codegenDeref(eval(left)), eval(right));
      else
          codegenAssign(eval(left), eval(right));
      return ret;
      }

  GenRet GenIR::primAddrOf(CallExpr *self) {
      return codegenAddrOf(evalArg(self, 1));
      }
      
  GenRet GenIR::primRefToString(CallExpr *self) {
      GenRet ret;
      Expr *a1 = self->get(1);
      if (hasFlag(a1, FLAG_WIDE) || hasFlag(a1, FLAG_WIDE_CLASS)) {
          GenRet wide = eval(a1);
          ret = 
              codegenCallExpr(
                  "chpl_wideRefToString",
                  codegenRnode(wide), 
                  codegenRaddr(wide));
          }
      else
          ret = codegenCallExpr("chpl_refToString", eval(a1));
      return ret;
      }
      
  GenRet GenIR::primReturn(CallExpr *self) {
      GenRet ret;
      if (self->typeInfo() == dtVoid)
          ret.val = ir->genReturn();
      else {
          ret = codegenValue(evalArg(self, 1));
          ret.val = ir->genReturn(ret.val);
          }
      return ret;
      }

  GenRet GenIR::primUnaryMinus(CallExpr *self) {
      return codegenNeg(evalArg(self, 1));
      }
      
  GenRet GenIR::primUnaryPlus(CallExpr *self) {
      GenRet ret;
      GenRet tmp = codegenValue(evalArg(self, 1));
      ret.val = ir->genPlus(tmp.val);
      return ret;
      }
      
  GenRet GenIR::primUnaryNot(CallExpr *self) {
      GenRet ret;
      GenRet tmp = codegenValue(evalArg(self, 1));
      ret.val = ir->genNot(tmp.val);
      return ret;
      }
      
  GenRet GenIR::primUnaryLnot(CallExpr *self) {
      return codegenIsZero(evalArg(self, 1));
      }
      
  GenRet GenIR::primAdd(CallExpr *self) {
      return codegenAdd(evalArg(self, 1), evalArg(self, 2));
      }
      
  GenRet GenIR::primSubtract(CallExpr *self) {
      return codegenSub(evalArg(self, 1), evalArg(self, 2));
      }
      
  GenRet GenIR::primMult(CallExpr *self) {
      return codegenMul(evalArg(self, 1), evalArg(self, 2));
      }
      
  GenRet GenIR::primDiv(CallExpr *self) {
      GenRet ret;
      GenRet a = codegenValue(evalArg(self, 1));
      GenRet b = codegenValue(evalArg(self, 2));
      ret.val = ir->genDiv(a.val, b.val);
      return ret;
      }

  GenRet GenIR::primMod(CallExpr *self) {
      GenRet ret;
      GenRet a = codegenValue(evalArg(self, 1));
      GenRet b = codegenValue(evalArg(self, 2));
      ret.val = ir->genMod(a.val, b.val);
      return ret;
      }
      
  GenRet GenIR::primLsh(CallExpr *self) {
      GenRet ret;
      GenRet a = codegenValue(evalArg(self, 1));
      GenRet b = codegenValue(evalArg(self, 2));
      ret.val = ir->genLsh(a.val, b.val);
      return ret;
      }
      
  GenRet GenIR::primRsh(CallExpr *self) {
      GenRet ret;
      GenRet a = codegenValue(evalArg(self, 1));
      GenRet b = codegenValue(evalArg(self, 2));
      ret.val = ir->genRsh(a.val, b.val);
      return ret;
      }
      
  GenRet GenIR::primEqual(CallExpr *self) {
      GenRet ret;
      Expr *a1 = self->get(1);
      Expr *a2 = self->get(2);
    // TODO: Need subloc field as well.
      if (hasFlag(a1, FLAG_WIDE_CLASS) && hasFlag(a2, FLAG_WIDE_CLASS)) {
          GenRet a = eval(a1);
          GenRet b = eval(a2);
          GenRet addrNe = 
              codegenNotEquals(codegenRaddr(a), codegenRaddr(b));
          GenRet locNe = 
              codegenNotEquals(codegenRnode(a), codegenRnode(b));
          GenRet rh = 
              codegenLogicalAnd(
                  codegenIsNotZero(codegenRaddr(a)), locNe);
          GenRet ne = codegenLogicalOr(addrNe, rh);
          ret = codegenIsZero(ne);
          }
      else if (hasFlag(a1, FLAG_WIDE_CLASS) && a2->typeInfo() == dtNil)
          ret = codegenIsZero(eval(a1));
      else if (hasFlag(a2, FLAG_WIDE_CLASS) && a1->typeInfo() == dtNil)
          ret = codegenIsZero(eval(a2));
      else
          ret = codegenEquals(eval(a1), eval(a2));
      return ret;
      }
      
  GenRet GenIR::primNotequal(CallExpr *self) {
      GenRet ret;
      Expr *a1 = self->get(1);
      Expr *a2 = self->get(2);
    // TODO: Need subloc field as well.
      if (hasFlag(a1, FLAG_WIDE_CLASS) && hasFlag(a2, FLAG_WIDE_CLASS)) {
          GenRet a = eval(a1);
          GenRet b = eval(a2);
          GenRet addrNe = 
              codegenNotEquals(codegenRaddr(a), codegenRaddr(b));
          GenRet locNe = 
              codegenNotEquals(codegenRnode(a), codegenRnode(b));
          GenRet rh = 
              codegenLogicalAnd(
                  codegenIsNotZero(codegenRaddr(a)), locNe);
          ret = codegenLogicalOr(addrNe, rh);
          }
      else if (hasFlag(a1, FLAG_WIDE_CLASS) && a2->typeInfo() == dtNil)
          ret = codegenIsNotZero(eval(a1));
      else if (hasFlag(a2, FLAG_WIDE_CLASS) && a1->typeInfo() == dtNil)
          ret = codegenIsNotZero(eval(a2));
      else
          ret = codegenNotEquals(eval(a1), eval(a2));
      return ret;
      }
      
  GenRet GenIR::primLessorequal(CallExpr *self) {
      GenRet ret;
      GenRet a = codegenValue(evalArg(self, 1));
      GenRet b = codegenValue(evalArg(self, 2));
      ret.val = ir->genLe(a.val, b.val);
      return ret;
      }
      
  GenRet GenIR::primGreaterorequal(CallExpr *self) {
      GenRet ret;
      GenRet a = codegenValue(evalArg(self, 1));
      GenRet b = codegenValue(evalArg(self, 2));
      ret.val = ir->genGe(a.val, b.val);
      return ret;
      }
      
  GenRet GenIR::primLess(CallExpr *self) {
      GenRet ret;
      GenRet a = codegenValue(evalArg(self, 1));
      GenRet b = codegenValue(evalArg(self, 2));
      ret.val = ir->genLt(a.val, b.val);
      return ret;
      }
      
  GenRet GenIR::primGreater(CallExpr *self) {
      GenRet ret;
      GenRet a = codegenValue(evalArg(self, 1));
      GenRet b = codegenValue(evalArg(self, 2));
      ret.val = ir->genGt(a.val, b.val);
      return ret;
      }
      
  GenRet GenIR::primAnd(CallExpr *self) {
      GenRet ret;
      GenRet a = codegenValue(evalArg(self, 1));
      GenRet b = codegenValue(evalArg(self, 2));
      ret.val = ir->genAnd(a.val, b.val);
      return ret;
      }
      
  GenRet GenIR::primOr(CallExpr *self) {
      GenRet ret;
      GenRet a = codegenValue(evalArg(self, 1));
      GenRet b = codegenValue(evalArg(self, 2));
      ret.val = ir->genOr(a.val, b.val);
      return ret;
      }
      
  GenRet GenIR::primXor(CallExpr *self) {
      GenRet ret;
      GenRet a = codegenValue(evalArg(self, 1));
      GenRet b = codegenValue(evalArg(self, 2));
      ret.val = ir->genXor(a.val, b.val);
      return ret;
      }
      
  GenRet GenIR::primPow(CallExpr *self) {
      return codegenCallExpr("pow", evalArg(self, 1), evalArg(self, 2));
      }

  GenRet GenIR::primMin(CallExpr *self) {
      GenRet ret;
      Type *t = self->get(1)->typeInfo();
      if (is_arithmetic_type(t)) {
          int w = get_width(t);
          if (is_int_type(t))
              ret = codegenUseGlobal("MIN_INT"+numToString(w));
          else if (is_uint_type(t))
              ret = codegenUseGlobal("MIN_UINT"+numToString(w));
          else if (is_real_type(t))
              ret = 
                  codegenNeg(
                      codegenUseGlobal("MAX_FLOAT"+numToString(w)));
          else if (is_imag_type(t))
              ret = 
                  codegenNeg(
                      codegenUseGlobal("MAX_FLOAT"+numToString(w)));
          else if (is_complex_type(t)) {
              std::string fname = "_chpl_complex" + numToString(w);
              ret = 
                  codegenCallExpr(
                      fname.c_str(),
                      codegenNeg(
                          codegenUseGlobal("MAX_FLOAT"+numToString(w/2))),
                      codegenNeg(
                          codegenUseGlobal("MAX_FLOAT"+numToString(w/2))));
              }
          else
              INT_FATAL(t, "cannot do min on supplied type");
          }
      else
          INT_FATAL(t, "not arithmetic type");
      return ret;
      }
      
  GenRet GenIR::primMax(CallExpr *self) {
      GenRet ret;
      Type *t = self->get(1)->typeInfo();
      if (is_arithmetic_type(t)) {
          int w = get_width(t);
          if (is_int_type(t))
              ret = codegenUseGlobal("MAX_INT"+numToString(w));
          else if (is_uint_type(t))
              ret = codegenUseGlobal("MAX_UINT"+numToString(w));
          else if (is_real_type(t))
              ret = codegenUseGlobal("MAX_FLOAT"+numToString(w));
          else if (is_imag_type(t))
              ret = codegenUseGlobal("MAX_FLOAT"+numToString(w));
          else if (is_complex_type(t)) {
              std::string fname = "_chpl_complex" + numToString(w);
              ret = 
                  codegenCallExpr(
                      fname.c_str(),
                      codegenUseGlobal("MAX_FLOAT"+numToString(w/2)),
                      codegenUseGlobal("MAX_FLOAT"+numToString(w/2)));
              }
          else
              INT_FATAL(t, "cannot do max on supplied type");
          }
      else
          INT_FATAL(t, "not arithmetic type");
      return ret;
      }

  void GenIR::primSetcid(CallExpr *self) {
    // get(1) is the object 
    // (type=chpl__class_id,
    //  tid=CHPL_TYPE_int32_t,
    //  wide=get(1),
    //  local=chpl__cid_<type>,
    //  stype=dtObject->typeInfo(),
    //  sfield=chpl__cid,
    //  ln=get(2),
    //  fn=get(3))
      Expr *a1 = self->get(1);
      if (hasFlag(a1, FLAG_NO_OBJECT) && !hasFlag(a1, FLAG_OBJECT_CLASS)) {
        // Don't set cid for an extern class.
        // This should probably be an error in the future.
          }
      else {
          Type *classType;
          if (hasFlag(a1, FLAG_WIDE_CLASS))
              classType = getFieldType(a1, "addr");
          else
              classType = a1->typeInfo();
          GenRet ref = codegenFieldCidPtr(eval(a1));
          codegenAssign(ref, codegenUseCid(classType));
          }
      }
      
  GenRet GenIR::primGetcid(CallExpr *self) {
      Expr *a1 = self->get(1);
      INT_ASSERT(a1->typeInfo() != dtNil);
      if (hasFlag(a1, FLAG_NO_OBJECT) && !hasFlag(a1, FLAG_OBJECT_CLASS))
          INT_ASSERT(0);
      GenRet ref = codegenFieldCidPtr(eval(a1));
      return codegenValue(ref);
      }
      
  GenRet GenIR::primTestcid(CallExpr *self) {
    // AP(1) is an object to test, AP(2) we just use the type of it.
      Expr *a1 = self->get(1);
      Expr *a2 = self->get(2);
      INT_ASSERT(a1->typeInfo() != dtNil);
      if (hasFlag(a1, FLAG_NO_OBJECT) && !hasFlag(a1, FLAG_OBJECT_CLASS))
          INT_ASSERT(0);
      GenRet ref = codegenFieldCidPtr(eval(a1));
      return codegenEquals(ref, codegenUseCid(a2->typeInfo()));
      }
      
  void GenIR::primSetUnionId(CallExpr *self) {
    // get(1)->_uid = get(2)
      GenRet ref = codegenFieldUidPtr(evalArg(self, 1));
      codegenAssign(ref, evalArg(self, 2));
      }
      
  GenRet GenIR::primGetUnionId(CallExpr *self) {
    // returns uid from AP(1)
      GenRet ref = codegenFieldUidPtr(evalArg(self, 1));
      return codegenValue(ref);
      }
      
  void GenIR::primSetSvecMember(CallExpr *self) {
    // set tuple base=AP(1) at index=AP(2) to value=AP(3)
      GenRet ptr = 
          codegenElementPtr(
              evalArg(self, 1), codegenExprMinusOne(self->get(2)));
      codegenAssign(ptr, evalArg(self, 3));
      }
      
  GenRet GenIR::primGetMember(CallExpr *self) {
    // base=get(1) field symbol=get(2)
      GenRet ret = codegenFieldPtr(evalArg(self, 1), self->get(2));
    // Used to only do addrOf if
    // !get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)
    // but that unnaturaly depends on the type of the field.
      ret = codegenAddrOf(ret);
      return ret;
      }
      
  GenRet GenIR::primGetSvecMember(CallExpr *self) {
    // get tuple base=AP(1) at index=AP(2)
      Expr *a1 = self->get(1);
      Type *tupleType = a1->getValType();
      GenRet ret = 
          codegenElementPtr(eval(a1), codegenExprMinusOne(self->get(2)));
      if (!hasFlag(getFieldType(tupleType, "x1"), FLAG_REF))
          ret = codegenAddrOf(ret);
      return ret;
      }
      
  void GenIR::primSetMember(CallExpr *self) {
    // base=get(1) field=get(2) value=get(3)
      GenRet ptr = codegenFieldPtr(evalArg(self, 1), self->get(2));
      GenRet val = evalArg(self, 3);
      codegenAssign(ptr, val);
      }
      
  void GenIR::primCheckNil(CallExpr *self) {
      Expr *a1 = self->get(1);
      GenRet ptr = eval(a1);
      if (hasFlag(a1, FLAG_WIDE_CLASS))
          ptr = codegenRaddr(ptr);
      codegenCall(
          "chpl_check_nil", 
          ptr, 
          evalInt(infoLineno()), 
          evalStr(infoFilename())); 
      }

  void GenIR::primLocalCheck(CallExpr *self) {
    // arguments are (wide ptr, line, function/file, error string)
      Expr *a1 = self->get(1);
      const char *error;
      INT_ASSERT(hasFlag(a1, FLAG_WIDE) || hasFlag(a1, FLAG_WIDE_CLASS));
      if (hasFlag(a1, FLAG_WIDE_CLASS) &&
              hasFlag(getFieldType(a1, "addr"), FLAG_EXTERN))
          error = "cannot pass non-local extern class to extern procedure";
      else
          error = "cannot access remote data in local block";
      codegenCall(
          "chpl_test_local",
          codegenRnode(eval(a1)), 
          evalArg(self, 2), 
          evalArg(self, 3), 
          evalStr(error)); 
      }
      
  void GenIR::primSyncInit(CallExpr *self) {
      GenRet fieldPtr = 
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "sync_aux"));
      const char *fn =
          (self->primitive->tag == PRIM_SYNC_INIT) ?
              "chpl_sync_initAux" : "chpl_sync_destroyAux";
      codegenCall(fn, fieldPtr);
      }
      
  void GenIR::primSyncLock(CallExpr *self) {
      codegenCall(
          "chpl_sync_lock",
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "sync_aux")));
      }
      
  void GenIR::primSyncUnlock(CallExpr *self) {
      codegenCall(
          "chpl_sync_unlock", 
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "sync_aux")));
      }
      
  void GenIR::primSyncWaitFull(CallExpr *self) {
    // sync var, lineno, filename
      codegenCall(
          "chpl_sync_waitFullAndLock",
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "sync_aux")),
          evalArg(self, 2), 
          evalArg(self, 3));
      }
      
  void GenIR::primSyncWaitEmpty(CallExpr *self) {
    // sync var, lineno, filename
      codegenCall(
          "chpl_sync_waitEmptyAndLock",
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "sync_aux")),
          evalArg(self, 2), 
          evalArg(self, 3));
      }
      
  void GenIR::primSyncSignalFull(CallExpr *self) {
      codegenCall(
          "chpl_sync_markAndSignalFull",
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "sync_aux")));
      }
      
  void GenIR::primSyncSignalEmpty(CallExpr *self) {
      codegenCall(
          "chpl_sync_markAndSignalEmpty",
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "sync_aux")));
      }

  void GenIR::primSingleInit(CallExpr *self) {
      const char *fn =
          (self->primitive->tag == PRIM_SINGLE_INIT) ?
              "chpl_single_initAux" : "chpl_single_destroyAux";
      codegenCall(
          fn, 
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "single_aux")));
      }
      
  void GenIR::primSingleLock(CallExpr *self) {
      codegenCall(
          "chpl_single_lock",
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "single_aux")));
      }
      
  void GenIR::primSingleUnlock(CallExpr *self) {
      codegenCall(
          "chpl_single_unlock",
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "single_aux")));
      }
      
  void GenIR::primSingleWaitFull(CallExpr *self) {
    // single, lineno, filename
      codegenCall(
          "chpl_single_waitFullAndLock",
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "single_aux")),
          evalArg(self, 2), 
          evalArg(self, 3));

      }
      
  void GenIR::primSingleSignalFull(CallExpr *self) {
      codegenCall(
          "chpl_single_markAndSignalFull",
          codegenLocalAddrOf(
              codegenFieldPtr(evalArg(self, 1), "single_aux")));
      }
      
  void GenIR::primWriteef(CallExpr *self) {
    // get(1) is argument (class, wide or not), get(2) is what to write.
      Expr *a1 = self->get(1);
      GenRet s;
      if (hasFlag(a1, FLAG_WIDE_CLASS))
          s = codegenRaddr(eval(a1));
      else
          s = eval(a1);
      codegenCall("chpl_write_EF", s, evalArg(self, 2));
      }
      
  void GenIR::primWriteff(CallExpr *self) {
      const char *fn;
      if (self->primitive->tag == PRIM_WRITEFF) 
          fn = "chpl_write_FF";
      else    // primitive->tag == PRIM_WRITEXF 
          fn = "chpl_write_XF";
    // get(1) is argument (class, wide or not), AP(2) is what to write.
      Expr *a1 = self->get(1);
      GenRet s;
      if (hasFlag(a1, FLAG_WIDE_CLASS))
          s = codegenRaddr(eval(a1));
      else
          s = eval(a1);
      codegenCall(fn, s, evalArg(self, 2));
      }
      
  void GenIR::primSyncReset(CallExpr *self) {
    // get(1) is argument (class, wide or not)
      Expr *a1 = self->get(1);
      GenRet s;
      if (hasFlag(a1, FLAG_WIDE_CLASS))
          s = codegenRaddr(eval(a1));
      else
          s = eval(a1);
      codegenCall("chpl_sync_reset", s);
      }
      
  GenRet GenIR::primReadfe(CallExpr *self) {
      const char *fn;
      if (self->primitive->tag == PRIM_READFE) 
          fn = "chpl_read_FE";
      else if (self->primitive->tag == PRIM_READFF) 
          fn = "chpl_read_FF";
      else    // primitive->tag == PRIM_READXX 
          fn = "chpl_read_XX";
      Expr *a1 = self->get(1);
      GenRet s;
      if (hasFlag(a1, FLAG_WIDE_CLASS))
          s = codegenRaddr(eval(a1));
      else
          s = eval(a1);
      return codegenCallExpr(fn, s);
      }
      
  GenRet GenIR::primSyncIsFull(CallExpr *self) {
    // get(1) is sync var, get(2) is isSimpleSyncBaseType(arg)
      GenRet s = evalArg(self, 1);
      GenRet val_ptr = codegenLocalAddrOf(codegenFieldPtr(s, "value"));
      GenRet aux = codegenLocalAddrOf(codegenFieldPtr(s, "sync_aux"));
      return codegenCallExpr(
          "chpl_sync_isFull", val_ptr, aux, evalArg(self, 2));
      }
      
  void GenIR::primSingleWriteef(CallExpr *self) {
    // get(1) is argument (class, wide or not), get(2) is what to write.
      Expr *a1 = self->get(1);
      GenRet s;
      if (hasFlag(a1, FLAG_WIDE_CLASS))
          s = codegenRaddr(eval(a1));
      else
          s = eval(a1);
      codegenCall("chpl_single_write_EF", s, evalArg(self, 2));
      }
      
  void GenIR::primSingleReset(CallExpr *self) {
    // get(1) is argument (class, wide or not)
      Expr *a1 = self->get(1);
      GenRet s;
      if (hasFlag(a1, FLAG_WIDE_CLASS))
          s = codegenRaddr(eval(a1));
      else
          s = eval(a1);
      codegenCall("chpl_single_reset", s);
      }
      
  GenRet GenIR::primSingleReadff(CallExpr *self) {
      const char *fn;
      if (self->primitive->tag == PRIM_SINGLE_READFF)
          fn = "chpl_single_read_FF";
      else    // primitive->tag == PRIM_SINGLE_READXX
          fn = "chpl_single_read_XX";
      Expr *a1 = self->get(1);
      GenRet s;
      if (hasFlag(a1, FLAG_WIDE_CLASS))
          s = codegenRaddr(eval(a1));
      else
          s = eval(a1);
      return codegenCallExpr(fn, s);
      }
      
  GenRet GenIR::primSingleIsFull(CallExpr *self) {
    // get(1) is sync var, get(2) is isSimpleSyncBaseType(arg)
      GenRet s = evalArg(self, 1);
      GenRet val_ptr = codegenLocalAddrOf(codegenFieldPtr(s, "value"));
      GenRet aux = codegenLocalAddrOf(codegenFieldPtr(s, "single_aux"));
      return codegenCallExpr(
          "chpl_single_isFull", val_ptr, aux, evalArg(self, 2));
      }
      
  void GenIR::primProcessTaskList(CallExpr *self) {
      GenRet taskListPtr = codegenFieldPtr(evalArg(self, 1), "taskList");
      codegenCall("chpl_task_processTaskList", codegenValue(taskListPtr));
      }
      
  void GenIR::primExecuteTasksInList(CallExpr *self) {
      codegenCall("chpl_task_executeTasksInList", evalArg(self, 1));
      }
      
  void GenIR::primFreeTaskList(CallExpr *self) {
      if (fNoMemoryFrees)
          return;
      codegenCall("chpl_task_freeTaskList", evalArg(self, 1));
      }
      
  GenRet GenIR::primGetSerial(CallExpr *self) {
      return codegenCallExpr("chpl_task_getSerial");
      }
      
  void GenIR::primSetSerial(CallExpr *self) {
      codegenCall("chpl_task_setSerial", codegenValue(evalArg(self, 1)));
      }
      
  GenRet GenIR::primGetSublocId(CallExpr *self) {
      return codegenCallExpr("chpl_task_getSubLoc");
      }
      
  void GenIR::primSetSublocId(CallExpr *self) {
      codegenCall("chpl_task_setSubLoc", codegenValue(evalArg(self, 1)));
      }
      
  GenRet GenIR::primLocGetNode(CallExpr *self) {
      return codegenValue(codegenFieldPtr(evalArg(self, 1), "node"));
      }
      
  GenRet GenIR::primLocGetSubloc(CallExpr *self) {
      return codegenValue(codegenFieldPtr(evalArg(self, 1), "subloc"));
      }
      
  void GenIR::primChplCommGet(CallExpr *self) {
    // args are:
    //   localvar, locale, remote addr, eltSize, get(4)==length, line, file
      const char *fn;
      if (self->primitive->tag == PRIM_CHPL_COMM_GET)
          fn = "chpl_gen_comm_get";
      else
          fn = "chpl_gen_comm_put";
      TypeSymbol *dt;
      Expr *a1 = self->get(1);
      GenRet localAddr = codegenValuePtr(eval(a1));

    // destination data array
      if (hasFlag(a1, FLAG_WIDE)) {
          Symbol *sym = a1->typeInfo()->getField("addr", true);
          INT_ASSERT(sym);
          dt = sym->typeInfo()->getValType()->symbol;
          localAddr = codegenRaddr(localAddr);
          }
      else {
          dt = a1->typeInfo()->getValType()->symbol;
          if (hasFlag(a1, FLAG_REF))
              localAddr = codegenDeref(localAddr);
          }

      Expr *a2 = self->get(2);
      GenRet locale;
      if (hasEitherFlag(a2, FLAG_WIDE, FLAG_REF))
          locale = codegenValue(codegenDeref(eval(a2)));
      else
          locale = codegenValue(eval(a2));

    // source data array
      Expr *a3 = self->get(3);
      GenRet remoteAddr = eval(a3);
      SymExpr *sym = toSymExpr(a3);
      INT_ASSERT(sym);
      if (hasFlag(sym, FLAG_WIDE))
          remoteAddr = codegenRaddr(remoteAddr);
      else if (!hasFlag(sym, FLAG_REF))
          remoteAddr = codegenAddrOf(remoteAddr);
    /*
      if (remoteAddrArg.isLVPtr == GEN_WIDE_PTR)
          remoteAddr = codegenRaddr(remoteAddrArg);
      else
          remoteAddr = codegenValuePtr(remoteAddrArg);
    */
    
      Expr *a4 = self->get(4);
      GenRet eltSize = codegenSizeof(dt->typeInfo());
      GenRet len;
      if (hasEitherFlag(a4, FLAG_WIDE, FLAG_REF))
          len = codegenValue(codegenDeref(eval(a4)));
      else
          len = codegenValue(eval(a4));

      codegenCall(
          fn, 
          codegenCastToVoidStar(localAddr), 
          locale, 
          remoteAddr, 
          eltSize,
          genTypeStructureIndex(dt), 
          len,
          evalArg(self, 5), 
          evalArg(self, 6));
      }
      
  void GenIR::primChplCommGetStrd(CallExpr *self) {
    // Strided versions of get and put
    // args are: localvar, dststr addr, locale, remote addr, srcstr addr
    // count addr, strlevels, elem
      const char *fn;
      if (self->primitive->tag == PRIM_CHPL_COMM_GET_STRD)
          fn = "chpl_gen_comm_get_strd";
      else
          fn = "chpl_gen_comm_put_strd";
      TypeSymbol *dt;
      Expr *a1 = self->get(1);
      GenRet localAddr = codegenValuePtr(eval(a1));

    // destination data array
      if (hasFlag(a1, FLAG_WIDE)) {
          Symbol *sym = a1->typeInfo()->getField("addr", true);
          INT_ASSERT(sym);
          dt = sym->typeInfo()->getValType()->symbol;
          localAddr = codegenRaddr(localAddr);
          }
      else {
          dt = a1->typeInfo()->getValType()->symbol;
          if (hasFlag(a1, FLAG_REF))
              localAddr = codegenDeref(localAddr);
          }

    // destination strides local array
      Expr *a2 = self->get(2);
      GenRet dststr = codegenValuePtr(eval(a2));
      if (hasFlag(a2, FLAG_WIDE)) {
          Symbol *sym = a2->typeInfo()->getField("addr", true);
          INT_ASSERT(sym);
          dststr = codegenRaddr(dststr);
          }
      else if (hasFlag(a2, FLAG_REF))
          dststr = codegenDeref(dststr);

    // locale id
      Expr *a3 = self->get(3);
      GenRet locale;
      if (hasEitherFlag(a3, FLAG_WIDE, FLAG_REF))
          locale = codegenValue(codegenDeref(eval(a3)));
      else
          locale = codegenValue(eval(a3)); 

    // source data array
      Expr *a4 = self->get(4);
      GenRet remoteAddr = eval(a4);
      SymExpr *sym = toSymExpr(a4);
      INT_ASSERT(sym);
      if (hasFlag(sym, FLAG_WIDE))
          remoteAddr = codegenRaddr(remoteAddr);
      else if (!hasFlag(sym, FLAG_REF))
          remoteAddr = codegenAddrOf(remoteAddr);

    // source strides local array
      Expr *a5 = self->get(5);
      GenRet srcstr = codegenValuePtr(eval(a5));
      if (hasFlag(a5, FLAG_WIDE)) {
          Symbol *sym = a5->typeInfo()->getField("addr", true);
          INT_ASSERT(sym);
          srcstr = codegenRaddr(srcstr);
          }
      else if (hasFlag(a5, FLAG_REF))
          srcstr = codegenDeref(srcstr);

    // count local array
      Expr *a6 = self->get(6);
      GenRet count = codegenValuePtr(eval(a6));
      if (hasFlag(a6, FLAG_WIDE)) {
          Symbol *sym = a6->typeInfo()->getField("addr", true);
          INT_ASSERT(sym);
          count = codegenRaddr(count);
          }
      else if (hasFlag(a6, FLAG_REF))
          count = codegenDeref(count);

    // stridelevels
      Expr *a7 = self->get(7);
      GenRet stridelevels;
      if (hasEitherFlag(a7, FLAG_WIDE, FLAG_REF))
          stridelevels = codegenValue(codegenDeref(eval(a7)));
      else
          stridelevels = codegenValue(eval(a7));

    // eltSize
      GenRet eltSize = codegenSizeof(dt->typeInfo());

      codegenCall(
          fn,
          codegenCastToVoidStar(localAddr),
          codegenCastToVoidStar(dststr),
          locale, 
          remoteAddr, 
          codegenCastToVoidStar(srcstr),
          codegenCastToVoidStar(count), 
          stridelevels, 
          eltSize,
          genTypeStructureIndex(dt), 
          evalArg(self, 8), 
          evalArg(self, 9));
      }
      
  GenRet GenIR::primChplAlloc(CallExpr *self) {
      GenRet size;
      
    // If Chapel class or record
      if (ClassType *ct = 
              toClassType(toTypeSymbol(self->typeInfo()->symbol)->type))
          size = codegenSizeof(ct->classStructName(true));
      else
          size = codegenSizeof(self->typeInfo());

      const char *fn = "chpl_mem_alloc";
      GenRet description = 
          codegenAdd(evalArg(self, 2), codegenUseGlobal("CHPL_RT_MD_NUM"));
      GenRet allocated = 
          codegenCallExpr(
              fn, 
              size, 
              description, 
              evalArg(self, 3), 
              evalArg(self, 4));

      return codegenCast(self->typeInfo()->symbol->cname, allocated);
      }
      
  void GenIR::primChplFree(CallExpr *self) {
      if (fNoMemoryFrees)
          return;
      INT_ASSERT(self->numActuals() == 3);
      Expr  *ptrExpr = self->get(1);
      if (hasFlag(ptrExpr, FLAG_DATA_CLASS))
          INT_FATAL(self, "cannot delete data class");
      GenRet ptr = codegenValue(eval(ptrExpr));
      if (hasFlag(ptrExpr, FLAG_WIDE_CLASS))
          ptr = codegenRaddr(ptr);
      codegenCall(
          "chpl_mem_free", 
          codegenCastToVoidStar(ptr), 
          evalArg(self, 2), 
          evalArg(self, 3));
    // MPF - note we do not set the pointer to NULL here
    // because it would not change any copies of the pointer
    // and we're toast in any case of use-after-free.
    // Arguably, it could be put back in for earlier error detection.
      }
      
  GenRet GenIR::primCast(CallExpr *self) {
      Expr *a1 = self->get(1);
      Expr *a2 = self->get(2);

      if (hasFlag(self, FLAG_WIDE_CLASS) || hasFlag(self, FLAG_WIDE)) {
          GenRet tmp = eval(a2);
          return codegenWideAddr(
              codegenRlocale(tmp),
              codegenCast(a1->typeInfo(), 
              codegenRaddr(tmp)));
          }

      GenRet ret;
      
      Type *dst = a1->typeInfo();
      Type *src = a2->typeInfo();
      if (dst == src)
          ret = eval(a2);
      else if ((is_int_type(dst) || is_uint_type(dst)) && src == dtTaskID) {
          GenRet v = codegenValue(eval(a2));
        // cast like this: (type) (intptr_t) v
          ret = codegenCast(self->typeInfo(), codegenCast("intptr_t", v));
          }
      else if (dst == dtString || src == dtString) {
          const char *dst_cname = dst->symbol->cname;
          const char *src_cname = src->symbol->cname;
          std::string fn;
          if (dst->symbol->cname[0] == '_') {
              fn += src_cname;
              fn += "_to";
              fn += dst_cname;
              }
          else {
              fn += src_cname;
              fn += "_to_";
              fn += dst_cname;
              }
          if (src == dtString)
            // TODO?
              ret = 
                  codegenCallExpr(
                      fn.c_str(), 
                      eval(a2), 
                      evalArg(self, 3), 
                      evalArg(self, 4));
          else
              ret = codegenCallExpr(fn.c_str(), codegenValue(eval(a2)));
          }
      else {
          if (isRecord(self->typeInfo()) || isUnion(self->typeInfo())) {
              INT_FATAL("TODO - don't like type-punning record/union");
            /*
              fprintf(outfile, "(*((");
              typeInfo()->codegen(outfile);
              fprintf(outfile, "*)(&(");
              a2->codegen(outfile);
              fprintf(outfile, "))))");
            */
              }
          else {
              GenRet v = codegenValue(eval(a2));
              ret = codegenCast(self->typeInfo()->symbol->cname, v);
              }
          }

      return ret;
      }
      
  GenRet GenIR::primDynamicCast(CallExpr *self) {
      if (hasFlag(self, FLAG_WIDE_CLASS))
          INT_FATAL(self, "wide class dynamic cast is not normal");
      GenRet tmp = codegenFieldCidPtr(evalArg(self, 2));
      GenRet value = codegenValue(tmp);
      GenRet ok = codegenDynamicCastCheck(value, self->typeInfo());
    // if ok, (typeInfo()->symbol->cname) (value)
    // otherwise, NULL
      GenRet cast = 
          codegenCast(
              self->typeInfo(), codegenValue(evalArg(self, 2)));
      GenRet nul = 
          codegenCast(
              self->typeInfo(), codegenNullPointer());
      return codegenTernary(ok, cast, nul);
      }
      
  GenRet GenIR::primNodeId(CallExpr *self) {
      return codegenGetNodeID();
      }
      
  GenRet GenIR::primOnLocaleNum(CallExpr *self) {
      GenRet ret;
      if (self->numActuals() < 2)
          ret = 
              codegenLocaleID(
                  codegenValue(evalArg(self, 1)), 
                  codegenZero());
      else
          ret = 
              codegenLocaleID(
                  codegenValue(evalArg(self, 1)), 
                  codegenValue(evalArg(self, 2)));
      return ret;
      }
      
  void GenIR::primAllocGvr(CallExpr *self) {
      codegenCall(
          "chpl_comm_alloc_registry",
          evalSym(new_IntSymbol(numGlobalsOnHeap, INT_SIZE_32)));
      }
      
  void GenIR::primHeapRegisterGlobalVar(CallExpr *self) {
      GenRet idx = codegenValue(evalArg(self, 1));
      GenRet var = evalArg(self, 2);
      codegenCall("CHPL_HEAP_REGISTER_GLOBAL_VAR", idx, var);
      }
      
  void GenIR::primHeapBroadcastGlobalVars(CallExpr *self) {
      codegenCall("chpl_gen_comm_broadcast_global_vars", evalArg(self, 1)); 
      }
      
  void GenIR::primPrivateBroadcast(CallExpr *self) {
      Expr *a2 = self->get(2);
      codegenCall(
          "chpl_comm_broadcast_private", 
          evalArg(self, 1), 
          codegenSizeof(a2->typeInfo()),
          genTypeStructureIndex(a2->typeInfo()->symbol));
      }
      
  void GenIR::primIntError(CallExpr *self) {
      codegenCall(
          "chpl_internal_error",
          evalStr("compiler generated error"));
      }
      
  GenRet GenIR::primStringCopy(CallExpr *self) {
      GenRet ret;
      Expr *a1 = self->get(1);
      if (hasFlag(a1, FLAG_WIDE_CLASS)) {
          GenRet cpyTo = Expr_codegen(a1);
          cpyTo.isLVPtr = GEN_VAL;
          ret = 
              codegenCallExpr(
                  "chpl_wide_string_copy", 
                  cpyTo, 
                  evalArg(self, 2), 
                  evalArg(self, 3));
          }
      else
          ret = codegenBasicPrimitiveExpr(self);
      return ret;
      }
      
  GenRet GenIR::primRtError(CallExpr *self) {
      return codegenBasicPrimitiveExpr(self);
      }
      
  void GenIR::primNewPrivClass(CallExpr *self) {
      Expr *a1 = self->get(1);
      GenRet arg = eval(a1);
      if (hasFlag(a1, FLAG_WIDE_CLASS))
          arg = codegenRaddr(arg);
      codegenCall("chpl_newPrivatizedClass", arg);
      }
      
  GenRet GenIR::primNumPrivClasses(CallExpr *self) {
      return codegenCallExpr("chpl_numPrivatizedClasses");
      }
      
  GenRet GenIR::primFtableCall(CallExpr *self) {
      Expr *a2 = self->get(2);
      Expr *a3 = self->get(3);
      
    // indirect function call via a function pointer
      GenRet index = codegenValue(evalArg(self, 1));
      GenRet fngen;
    // Generate a cast based upon the arguments.
      Type *t2 = a2->typeInfo();
      Type *t3 = a2->typeInfo();
      bool is_ptr2 = argMustUseCPtr(t2);
      bool is_ptr3 = argMustUseCPtr(t3);
      fngen.val = 
          ir->genFtablePtr(
              t2->symbol->cname,
              is_ptr2,
              t3->symbol->cname,
              is_ptr3,
              index.val);

      GenRet args[2];
      GenRet arg = eval(a2);
      if (is_ptr2) 
          arg = codegenLocalAddrOf(arg);
      args[0] = arg;
      arg = eval(a3);
      if (is_ptr3) 
          arg = codegenLocalAddrOf(arg);
      args[1] = arg;

      return codegenCallExpr(fngen, 2, args, NULL, true);
      }
      
  GenRet GenIR::primVmtCall(CallExpr *self) {
      GenRet ptrToFnPtr;
      GenRet index;
      FnSymbol *fn = NULL;
      int startArgs = 3;
      SymExpr *se = toSymExpr(self->get(1));
      INT_ASSERT(se);
      fn = toFnSymbol(se->var);
      INT_ASSERT(fn);
      GenRet i = codegenValue(evalArg(self, 2));
      GenRet j = evalInt(virtualMethodMap.get(fn));
      INT_ASSERT(gMaxVMT >= 0);
      GenRet maxVMTConst = evalInt(gMaxVMT);
    // indexExpr = maxVMT * i + j
      index = codegenAdd(codegenMul(maxVMTConst, i), j);
      GenRet base;
      base = codegenUseGlobal("chpl_vmtable");
      ptrToFnPtr = codegenElementPtr(base, index);
    // the function expression to call.
      GenRet fngen = FnSymbol_codegenCast(fn, codegenValue(ptrToFnPtr));

      int mark = startArg();
      int k = startArgs;
      for_formals(arg, fn) {
          pushArg(eval(self->get(k++)));
          }
      endArg(mark);

      return codegenCallExpr(fngen, argNum(), argPtr(), fn, true);
      }
      
  void GenIR::primBlockXmtPragmaNoalias(CallExpr *self) {
    // This case allows __primitive("noalias pragma") to generate 
    // _Pragma("mta assert noalias <variable list>")
    // EVERY variable within a block is added to the variable list,
    // which means several variables will spawn warnings (ignored in
    // Makefile.cray-mta). Eventually, we'll only generate invariant 
    // pointer variables.
      if (strcmp(CHPL_TARGET_PLATFORM, "cray-xmt"))
          return;
      Vec<SymExpr *> se;
      Vec<DefExpr *> de_parent;
      Vec<VarSymbol *> vs_parent;
      collectSymExprs(self->next, se);
      collectDefExprs(self->parentExpr, de_parent);
      forv_Vec(DefExpr*, def, de_parent) {
          VarSymbol *vs = toVarSymbol(def->sym);
          if (vs) 
              vs_parent.add(vs);
          }
      forv_Vec(SymExpr *, sym, se) {
          if (isVarSymbol(sym->var) && (!(isPrimitiveType(sym->var->type)))) {
              VarSymbol *vs = toVarSymbol(sym->var);
              if (vs_parent.in(vs))
                  ir->genMtaAssertNoalias(sym->var->cname);
              }
          }
      self->remove();
      }
      
  void GenIR::invalidPrim(CallExpr *self) {
      INT_FATAL(
          self, "primitive codegen fail; should it still be in the AST?");
      ir->genInvalidPrim(self->primitive->name);
      }

// implementation: function calls

  void GenIR::genBeginBlock(CallExpr *self, FnSymbol *fn) {
    // get(1) is a class containing bundled arguments
      GenRet args[7];
      args[0] = evalInt(ftableMap.get(fn));

      Expr *a1 = self->get(1);
      if (a1 != NULL)
          args[1] = codegenCastToVoidStar(codegenValue(eval(a1)));
      else
          args[1] = codegenNullPointer();

    // ACHTUNG: What if (a1 == NULL)? Can this happen?
      ClassType *bundledArgsType = toClassType(toSymExpr(a1)->typeInfo());
    // lastField is the _endCount field.
      int lastField = bundledArgsType->fields.length;
      GenRet endCountPtr = 
          codegenValue(
              codegenFieldPtr(
                  eval(a1), bundledArgsType->getField(lastField)));
    // endCount is either an address or {locale, ptr}
      GenRet endCountValue = codegenValue(endCountPtr);
      GenRet taskList;
      Type *lastFieldType = 
          bundledArgsType->getField(lastField)->typeInfo();

      if (hasFlag(lastFieldType, FLAG_WIDE_CLASS)) {
          GenRet node = codegenRnode(endCountValue);
          endCountValue = codegenRaddr(endCountValue);
          taskList = 
              codegenLocalAddrOf(
                  codegenFieldPtr(endCountValue, "taskList"));
          taskList = 
              codegenTernary(
                  codegenNotEquals(node, codegenGetNodeID()),
                  codegenNullPointer(),
                  taskList);
              }
      else
          taskList = 
              codegenLocalAddrOf(
                  codegenFieldPtr(endCountValue, "taskList"));
      args[2] = taskList;
      if (hasFlag(lastFieldType, FLAG_WIDE_CLASS))
          args[3] = codegenRnode(endCountPtr);
      else
          args[3] = codegenGetNodeID();

      args[4] = evalSym(new_BoolSymbol(true, BOOL_SIZE_8));
      args[5] = evalInt(fn->linenum());
      args[6] = evalStr(fn->fname());

      codegenCall("chpl_task_addToTaskList", 7, args);
      }
      
  void GenIR::genCobeginOrCoforallBlock(CallExpr *self, FnSymbol *fn) {
    // self(1) is a class containing bundled arguments
      GenRet args[7];
      args[0] = evalInt(ftableMap.get(fn));

      Expr *a1 = self->get(1);
      if (a1 != NULL)
          args[1] = codegenCastToVoidStar(codegenValue(eval(a1)));
      else
          args[1] = codegenNullPointer();
    // ACHTUNG: What if (a1 == NULL)? Can this happen?
      ClassType *bundledArgsType = toClassType(toSymExpr(a1)->typeInfo());
      int endCountField = 0;
      for (int i = 1; i <= bundledArgsType->fields.length; i++) {
          const char *name = 
              bundledArgsType->getField(i)->typeInfo()->symbol->name;
          if (!strcmp(name, "_ref(_EndCount)") || 
                  !strcmp(name, "__wide__ref__EndCount") || 
                  !strcmp(name, "_EndCount") || 
                  !strcmp(name, "__wide__EndCount")) {
              endCountField = i;
              break;
              }
          }
      if (endCountField == 0)
          INT_FATAL(
              self, "cobegin/codegen codegen - _EndCount field not found");

      GenRet endCountPtr = 
          codegenValue(
              codegenFieldPtr(
                  eval(a1), bundledArgsType->getField(endCountField)));
      Type *endCountType = bundledArgsType->getField(endCountField)->typeInfo();
    // endCount is either an address or {locale, ptr} -- it is a class.
      GenRet endCountValue = codegenValue(endCountPtr);
      GenRet taskList;

      if (hasFlag(endCountType, FLAG_WIDE)) {
          GenRet node = codegenRnode(endCountValue);
          while (hasEitherFlag(endCountValue.chplType, FLAG_WIDE, FLAG_REF))
              endCountValue = codegenLocalDeref(endCountValue);
        // Now, we should have a wide pointer to a class
        // make it into a local pointer to a class.
          endCountValue = codegenRaddr(endCountValue);
          taskList = 
              codegenLocalAddrOf(
                  codegenFieldPtr(endCountValue, "taskList")); 
          taskList = 
              codegenTernary(
                  codegenNotEquals(node, codegenGetNodeID()),
                  codegenNullPointer(),
                  taskList);
          }
      else if (hasFlag(endCountType, FLAG_WIDE_CLASS)) {
          GenRet node = codegenRnode(endCountValue);
          endCountValue = codegenRaddr(endCountValue);
          taskList = 
              codegenLocalAddrOf(
                  codegenFieldPtr(endCountValue, "taskList"));
          taskList = 
              codegenTernary(
                  codegenNotEquals(node, codegenGetNodeID()),
                  codegenNullPointer(),
                  taskList);
          }
      else if (hasFlag(endCountType, FLAG_REF)) {
          endCountValue = codegenDeref(endCountValue);
          taskList = 
              codegenLocalAddrOf(
                  codegenFieldPtr(endCountValue, "taskList"));
          }
      else
          taskList = 
              codegenLocalAddrOf(
                  codegenFieldPtr(endCountValue, "taskList"));

      args[2] = taskList;
      args[3] = codegenGetNodeID(),
      args[4] = evalSym(new_BoolSymbol(false, BOOL_SIZE_8));
      args[5] = evalInt(fn->linenum());
      args[6] = evalStr(fn->fname());

      codegenCall("chpl_task_addToTaskList", 7, args);
      }
      
  void GenIR::genOnBlock(CallExpr *self, FnSymbol *fn) {
      const char *fname = NULL;
      if (fn->hasFlag(FLAG_NON_BLOCKING))
          fname = "chpl_comm_nonblocking_on";
      else if (fn->hasFlag(FLAG_FAST_ON))
          fname = "chpl_comm_fork_fast";
      else
          fname = "chpl_comm_fork";

      Expr *a2 = self->get(2);
      TypeSymbol *argType = toTypeSymbol(a2->typeInfo()->symbol);
      if (argType == NULL)
          INT_FATAL("could not get a type symbol");

      ClassType *ct = toClassType(argType->typeInfo());
      if (ct == NULL)
          INT_FATAL("Expected a class type in %s argument", fname);
      std::string ctype = ct->classStructName(true);

      GenRet nodeVal = 
          codegenValue(codegenFieldPtr(evalArg(self, 1), "node"));
    // Note that we select just the node portion of the localeID.
    // The comm routines only care about nodes, not sublocales.
    // The sublocale ID is passed to the called routine via the bundled args.
      codegenCall(
          fname,
          nodeVal,
          evalSym(new_IntSymbol(ftableMap.get(fn), INT_SIZE_32)),
          eval(a2),
          codegenSizeof(ctype.c_str()),
          genTypeStructureIndex(argType));
      }

  GenRet GenIR::genFnCall(CallExpr *self, FnSymbol *fn) {
      GenRet ret;
  
      GenRet base = Expr_codegen(self->baseExpr);

      int mark = startArg();

      for_formals_actuals(formal, actual, self) {
          Type *actualType = actual->typeInfo();

          GenRet arg = eval(actual);

          SymExpr *se = toSymExpr(actual);
          if (se && isFnSymbol(se->var))
              arg = codegenCast("chpl_fn_p", arg);

        // Handle passing strings to externs
          if (fn->hasFlag(FLAG_EXTERN)) {
              if (actualType == dtString)
                  arg = codegenCastToCharStar(codegenValue(arg));
              else if (passingWideStringToExtern(actualType))
                // checks for ref(widestr)
                  arg = 
                      codegenAddrOf(
                          codegenRaddrMaybePtr(codegenDeref(arg)));
              else if (hasFlag(actualType, FLAG_WIDE) ||
                      arg.isLVPtr == GEN_WIDE_PTR)
                  arg = codegenRaddr(codegenValue(arg));
              else if (hasFlag(formal->type, FLAG_REF) &&
                      hasFlag(
                          formal->type->symbol->getValType(), 
                          FLAG_STAR_TUPLE) &&
                      hasFlag(actualType, FLAG_REF))
                // ACHTUNG: target-dependent
                // In C, a fixed-size-array lvalue is already a pointer,
                // so we deref here. But for LLVM, if we deref we will
                // end up passing the tuple by value, which is not right.
                  arg = codegenDeref(arg);
              }

          pushArg(arg);
          }

      endArg(mark);

    // handle any special cases for which 
    // bool isBuiltinExternCFunction(const char *cname) returns true.
    //
    // special case: for CallExpr sizeof(..)
    //     C: irrelevant
    //     LLVM: relevant
      FnSymbol *fsym = self->isResolved();
      if (fsym && 
              fsym->hasFlag(FLAG_EXTERN) && 
              !strcmp(fsym->name, "sizeof")) {
          assert(argNum() == 1);
          ret.chplType = SIZE_TYPE;
          const GenRet *arg0 = argPtr();
        // ACHTUNG: assumed that either 'type' or 'val' is NULL
        //     Is it possible to eliminate this special case somehow?
          if (arg0->type.info)
              ret.val = ir->genSizeof(arg0->type);
          else
              ret.val = ir->genSizeof(codegenValue(*arg0).val);
          return ret;
          }

      ret = codegenCallExpr(base, argNum(), argPtr(), fn, true);

      if (self->getStmtExpr() == self)
          ir->genStmtExpr(ret.val);

      return ret;
      }

