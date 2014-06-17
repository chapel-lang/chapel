//
//    EXPR.CPP -- Code generator (IR back-end)
//
//        Expressions
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

// interface: Expr variants

  GenRet GenIR::SymExpr_codegen(SymExpr *self) {
      GenRet ret;
      if (self->getStmtExpr() == self)
          logStmt(self);
      ret = Symbol_codegen(self->var);
      return ret;  
      }
      
  GenRet GenIR::UnresolvedSymExpr_codegen(UnresolvedSymExpr *self) {
      GenRet ret;
      INT_FATAL(self, "UnresolvedSymExpr::codegen called");
      ret.val = ir->genUnresolved(self->unresolved);
      return ret;  
      }
      
  GenRet GenIR::DefExpr_codegen(DefExpr *self) {  
      GenRet ret;
      Symbol *sym = self->sym;
      if (toLabelSymbol(sym))
          ir->defLabel(sym->cname);
      return ret;
      }
      
/* Notes about code generation:
 *  Intermediate expressions are returned from Expr::codegen
 *  Local variables, array elements, tuple elements, and fields
 *    are always generated as the address of that value (ie lvalues
 *    are pointers)
 *  Expressions may be actual values, not addresses
 *  Chapel includes class instances - which may be remote or local.
 *   Note that the variable in question for a class instance *is*
 *    a wide (or not) reference (to the allocated object), but these
 *    references are considered "values" rather than "lvalue pointers"
 *    by the code generator. Thus a "reference to CLASS_CLASS" is
 *    actually a reference to a reference.. Note also that an "ARRAY"
 *    in the code generator is actually an instance of the e.g. _ddata
 *    class (and so the pointer to the data is again treated as a value).
 *    Lastly, Chapel references are considered "values" rather
 *    than "lvalue pointers", similarly to class instances, so that
 *    the generated code can set what a reference is pointing to
 *    (even though that is not allowed in Chapel).
 */
  GenRet GenIR::CallExpr_codegen(CallExpr *self) {  
      GenRet ret;
      
    // TODO: Revise SET_LINENO policy
      SET_LINENO(self);

    // Note (for debugging), function name is in parentSymbol->cname.

      if (self->getStmtExpr() == self)
          logStmt(self);

      PrimitiveOp *primitive = self->primitive;

      if (primitive != NULL) {
          switch (primitive->tag) {
          case PRIM_UNKNOWN:
              ret = primUnknown(self);
              break;
          case PRIM_ARRAY_SET:
          case PRIM_ARRAY_SET_FIRST:
              primArraySet(self);
              break;
          case PRIM_ARRAY_ALLOC:
              primArrayAlloc(self);
              break;
          case PRIM_GPU_ALLOC:
              primGpuAlloc(self);
              break;
          case PRIM_COPY_HOST_GPU:
              primCopyHostGpu(self);
              break;
          case PRIM_COPY_GPU_HOST:
              primCopyGpuHost(self);
              break;
          case PRIM_GPU_FREE:
              primGpuFree(self);
              break;
          case PRIM_ARRAY_FREE:
              primArrayFree(self);
              break;
          case PRIM_ARRAY_FREE_ELTS:
              primArrayFreeElts(self);
              break;
          case PRIM_NOOP:
              break;
          case PRIM_MOVE:
              ret = primMove(self);
              break;
          case PRIM_DEREF:
          case PRIM_GET_SVEC_MEMBER_VALUE:
          case PRIM_GET_MEMBER_VALUE:
          case PRIM_WIDE_GET_LOCALE:
          case PRIM_WIDE_GET_NODE:
          case PRIM_WIDE_GET_SUBLOC:
          case PRIM_GET_PRIV_CLASS:
          case PRIM_ARRAY_GET:
          case PRIM_ARRAY_GET_VALUE:
          case PRIM_GPU_GET_VALUE:
          case PRIM_GPU_GET_VAL:
          case PRIM_GPU_GET_ARRAY:
            // generated during generation of PRIM_MOVE
              break;
          case PRIM_ADDR_OF:
              ret = primAddrOf(self);
              break;
          case PRIM_REF_TO_STRING:
              ret = primRefToString(self);
              break;
          case PRIM_RETURN:
              ret = primReturn(self);
              break;
          case PRIM_UNARY_MINUS:
              ret = primUnaryMinus(self);
              break;
          case PRIM_UNARY_PLUS:
              ret = primUnaryPlus(self);
              break;
          case PRIM_UNARY_NOT:
              ret = primUnaryNot(self);
              break;
          case PRIM_UNARY_LNOT:
              ret = primUnaryLnot(self);
              break;
          case PRIM_ADD:
              ret = primAdd(self);
              break;
          case PRIM_SUBTRACT:
              ret = primSubtract(self);
              break;
          case PRIM_MULT:
              ret = primMult(self);
              break;
          case PRIM_DIV:
              ret = primDiv(self);
              break;
          case PRIM_MOD:
              ret = primMod(self);
              break;
          case PRIM_LSH:
              ret = primLsh(self);
              break;
          case PRIM_RSH:
              ret = primRsh(self);
              break;
          case PRIM_PTR_EQUAL:
          case PRIM_EQUAL:
              ret = primEqual(self);
              break;
          case PRIM_PTR_NOTEQUAL:
          case PRIM_NOTEQUAL:
              ret = primNotequal(self);
              break;
          case PRIM_LESSOREQUAL:
              ret = primLessorequal(self);
              break;
          case PRIM_GREATEROREQUAL:
              ret = primGreaterorequal(self);
              break;
          case PRIM_LESS:
              ret = primLess(self);
              break;
          case PRIM_GREATER:
              ret = primGreater(self);
              break;
          case PRIM_AND:
              ret = primAnd(self);
              break;
          case PRIM_OR:
              ret = primOr(self);
              break;
          case PRIM_XOR:
              ret = primXor(self);
              break;
          case PRIM_POW:
              ret = primPow(self);
              break;
          case PRIM_MIN: 
              ret = primMin(self);
              break;
          case PRIM_MAX: 
              ret = primMax(self);
              break;
          case PRIM_SETCID:
              primSetcid(self);
              break;
          case PRIM_GETCID:
              ret = primGetcid(self);
              break;
          case PRIM_TESTCID:
              ret = primTestcid(self);
              break;
          case PRIM_SET_UNION_ID:
              primSetUnionId(self);
              break;
          case PRIM_GET_UNION_ID:
              ret = primGetUnionId(self);
              break;
          case PRIM_SET_SVEC_MEMBER: 
              primSetSvecMember(self);
              break;
          case PRIM_GET_MEMBER: 
              ret = primGetMember(self);
              break;
          case PRIM_GET_SVEC_MEMBER: 
              ret = primGetSvecMember(self);
              break;
          case PRIM_SET_MEMBER: 
              primSetMember(self);
              break;
          case PRIM_CHECK_NIL: 
              primCheckNil(self);
              break;
          case PRIM_LOCAL_CHECK: 
              primLocalCheck(self);
              break;
          case PRIM_SYNC_INIT:
          case PRIM_SYNC_DESTROY:
              primSyncInit(self);
              break;
          case PRIM_SYNC_LOCK:
              primSyncLock(self);
              break;
          case PRIM_SYNC_UNLOCK:
              primSyncUnlock(self);
              break;
          case PRIM_SYNC_WAIT_FULL:
              primSyncWaitFull(self);
              break;
          case PRIM_SYNC_WAIT_EMPTY:
              primSyncWaitEmpty(self);
              break;
          case PRIM_SYNC_SIGNAL_FULL:
              primSyncSignalFull(self);
              break;
          case PRIM_SYNC_SIGNAL_EMPTY:
              primSyncSignalEmpty(self);
              break;
          case PRIM_SINGLE_INIT:
          case PRIM_SINGLE_DESTROY:
              primSingleInit(self);
              break;
          case PRIM_SINGLE_LOCK:
              primSingleLock(self);
              break;
          case PRIM_SINGLE_UNLOCK:
              primSingleUnlock(self);
              break;
          case PRIM_SINGLE_WAIT_FULL:
              primSingleWaitFull(self);
              break;
          case PRIM_SINGLE_SIGNAL_FULL:
              primSingleSignalFull(self);
              break;
          case PRIM_WRITEEF: 
              primWriteef(self);
              break;
          case PRIM_WRITEFF:
          case PRIM_WRITEXF:
              primWriteff(self);
              break;
          case PRIM_SYNC_RESET: 
              primSyncReset(self);
              break;
          case PRIM_READFE:
          case PRIM_READFF:
          case PRIM_READXX:
              ret = primReadfe(self);
              break;
          case PRIM_SYNC_IS_FULL:
              ret = primSyncIsFull(self);
              break; 
          case PRIM_SINGLE_WRITEEF:
              primSingleWriteef(self);
              break;
          case PRIM_SINGLE_RESET:
              primSingleReset(self);
              break;
          case PRIM_SINGLE_READFF:
          case PRIM_SINGLE_READXX:
              ret = primSingleReadff(self);
              break;
          case PRIM_SINGLE_IS_FULL:
              ret = primSingleIsFull(self);
              break;
          case PRIM_PROCESS_TASK_LIST: 
              primProcessTaskList(self);
              break;
          case PRIM_EXECUTE_TASKS_IN_LIST:
              primExecuteTasksInList(self);
              break;
          case PRIM_FREE_TASK_LIST:
              primFreeTaskList(self);
              break;
          case PRIM_GET_SERIAL:
              ret = primGetSerial(self);
              break;
          case PRIM_SET_SERIAL:
              primSetSerial(self);
              break;
          case PRIM_GET_SUBLOC_ID:
              ret = primGetSublocId(self);
              break;
          case PRIM_SET_SUBLOC_ID:
              primSetSublocId(self);
              break;
          case PRIM_LOC_GET_NODE:
              ret = primLocGetNode(self);
              break;
          case PRIM_LOC_GET_SUBLOC:
              ret = primLocGetSubloc(self);
              break;
          case PRIM_CHPL_COMM_GET:
          case PRIM_CHPL_COMM_PUT:
              primChplCommGet(self);
              break;
          case PRIM_CHPL_COMM_PUT_STRD: 
          case PRIM_CHPL_COMM_GET_STRD:
              primChplCommGetStrd(self);
              break;
          case PRIM_CHPL_ALLOC:
              ret = primChplAlloc(self);
              break;
          case PRIM_CHPL_FREE: 
              primChplFree(self);
              break;
          case PRIM_CAST: 
              ret = primCast(self);
              break;
          case PRIM_DYNAMIC_CAST: 
              ret = primDynamicCast(self);
              break;
          case PRIM_GC_CC_INIT:
          case PRIM_GC_ADD_ROOT:
          case PRIM_GC_ADD_NULL_ROOT:
          case PRIM_GC_DELETE_ROOT:
          case PRIM_GC_CLEANUP:
              INT_FATAL("GC primitives not supported");
              break;
          case PRIM_NODE_ID:
              ret = primNodeId(self);
              break;
          case PRIM_ON_LOCALE_NUM:
              ret = primOnLocaleNum(self);
              break;
          case PRIM_ALLOC_GVR:
              primAllocGvr(self);
              break;
          case PRIM_HEAP_REGISTER_GLOBAL_VAR: 
              primHeapRegisterGlobalVar(self);
              break;
          case PRIM_HEAP_BROADCAST_GLOBAL_VARS:
              primHeapBroadcastGlobalVars(self);
              break;
          case PRIM_PRIVATE_BROADCAST:
              primPrivateBroadcast(self);
              break;
          case PRIM_INT_ERROR:
              primIntError(self);
              break;
          case PRIM_STRING_COPY:
              ret = primStringCopy(self);
              break;
          case PRIM_RT_ERROR:
          case PRIM_RT_WARNING:
              ret = primRtError(self);
              break;
          case PRIM_NEW_PRIV_CLASS: 
              primNewPrivClass(self);
              break;
          case PRIM_NUM_PRIV_CLASSES:
              ret = primNumPrivClasses(self);
              break;
          case PRIM_WARNING:
            // warning issued, continue codegen
              break;
          case PRIM_FTABLE_CALL: 
              ret = primFtableCall(self);
              break;
          case PRIM_VMT_CALL: 
              ret = primVmtCall(self);
              break;
          case NUM_KNOWN_PRIMS:
              INT_FATAL(self, "impossible");
              break;
          case PRIM_BLOCK_XMT_PRAGMA_NOALIAS:
              primBlockXmtPragmaNoalias(self);
              break;
          default:
              invalidPrim(self);
              break;
              }
          if (self->getStmtExpr() == self)
              ir->genStmtExpr(ret.val);
          return ret;
          }

      FnSymbol *fn = self->isResolved();
      INT_ASSERT(fn);

      if (fn->hasFlag(FLAG_BEGIN_BLOCK)) {
          genBeginBlock(self, fn);
          return ret;
          }
      if (fn->hasFlag(FLAG_COBEGIN_OR_COFORALL_BLOCK)) {
          genCobeginOrCoforallBlock(self, fn);
          return ret;
          }
      if (fn->hasFlag(FLAG_ON_BLOCK)) {
          genOnBlock(self, fn);
          return ret;
          }

    // Do not code generate calls to functions marked FLAG_NO_CODEGEN.
      if (FnSymbol *fsym = self->isResolved()) {
          if (fsym->hasFlag(FLAG_NO_CODEGEN)) 
              return ret;
          }

      ret = genFnCall(self, fn);
          
      return ret;
      }

  GenRet GenIR::NamedExpr_codegen(NamedExpr *self) {
      GenRet ret;
      INT_FATAL(self, "NamedExpr::codegen not implemented");
      return ret;  
      }

// implementation: evaluation helpers

  GenRet GenIR::eval(Expr *expr) {
      GenRet ret = Expr_codegen(expr);
      ret.chplType = expr->typeInfo();
      ret.isUnsigned = !is_signed(ret.chplType);
      return ret;  
      }

  GenRet GenIR::evalSym(Symbol *sym) {
      GenRet ret = Symbol_codegen(sym);
      ret.chplType = sym->typeInfo();
      ret.isUnsigned = !is_signed(ret.chplType);
      return ret;  
      }
