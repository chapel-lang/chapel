//
// Mark On statements/blocks as "fast" (no communication, small, etc.)
//
// The comm layer can provide a "fast" option, for example, run within
//  the handler (rather than creating a new task).
//

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#ifdef DEBUG
#include "stmt.h"
#endif

//
// Return true if this primitive function is safe for fast on optimization
// (e.g., no communication, no sync/single accesses)
//
static bool
isFastPrimitive(CallExpr *call) {
  INT_ASSERT(call->primitive);
  // Check primitives for communication
  switch (call->primitive->tag) {
    // TODO: Add PRIM_UNKNOWN that are side-effect free
  case PRIM_NOOP:
  case PRIM_REF2STR:
  case PRIM_RETURN:
  case PRIM_UNARY_MINUS:
  case PRIM_UNARY_PLUS:
  case PRIM_UNARY_NOT:
  case PRIM_UNARY_LNOT:
  case PRIM_ADD:
  case PRIM_SUBTRACT:
  case PRIM_MULT:
  case PRIM_DIV:
  case PRIM_MOD:
  case PRIM_LSH:
  case PRIM_RSH:
  case PRIM_EQUAL:
  case PRIM_NOTEQUAL:
  case PRIM_LESSOREQUAL:
  case PRIM_GREATEROREQUAL:
  case PRIM_LESS:
  case PRIM_GREATER:
  case PRIM_AND:
  case PRIM_OR:
  case PRIM_XOR:
  case PRIM_POW:
  case PRIM_MIN:
  case PRIM_MAX:
  case PRIM_PROD_ID:
  case PRIM_BAND_ID:
  case PRIM_BOR_ID:
  case PRIM_BXOR_ID:
  case PRIM_LAND_ID:
  case PRIM_LOR_ID:
  case PRIM_LXOR_ID:

  case PRIM_GET_MEMBER:
  case PRIM_GET_SVEC_MEMBER:
  case PRIM_GET_PRIV_CLASS:

  case PRIM_CHECK_NIL:
  case PRIM_NEW:
  case PRIM_GET_REAL:
  case PRIM_GET_IMAG:

  case PRIM_ADDR_OF:

  case PRIM_INIT_FIELDS:
  case PRIM_PTR_EQUAL:
  case PRIM_PTR_NOTEQUAL:
  case PRIM_CAST:
  case PRIM_ISSUBTYPE:
  case PRIM_TYPEOF:
  case PRIM_GET_ITERATOR_RETURN:
  case PRIM_USE:
  case PRIM_USED_MODULES_LIST:
  case PRIM_TUPLE_EXPAND:
  case PRIM_TUPLE_AND_EXPAND:

  case PRIM_ERROR:
  case PRIM_WARNING:
  case PRIM_TYPE_TO_STRING:
  case PRIM_WHEN:

  case PRIM_BLOCK_LOCAL:

  case PRIM_TO_LEADER:
  case PRIM_TO_FOLLOWER:

  case PRIM_DELETE:

  case PRIM_LOCALE_ID:

  case PRIM_STRING_COPY:

  case PRIM_NEXT_UINT32:
  case PRIM_GET_USER_LINE:
  case PRIM_GET_USER_FILE:
  
#ifdef DEBUG
    printf(" *** OK (default): %s\n", call->primitive->name);
#endif
    return true;

  case PRIM_MOVE:
    if (call->get(1)->typeInfo() == dtVoid) {
#ifdef DEBUG
      printf(" *** OK (PRIM_MOVE 0): %s\n", call->primitive->name);
#endif
      return true;
    }
    if (!isCallExpr(call->get(2))) {
      if (!(call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
            !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
            !call->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)))
#ifdef DEBUG
        printf(" *** OK (PRIM_MOVE 1): %s\n", call->primitive->name);
#endif
        return true;
    } else {
#ifdef DEBUG
      printf(" *** OK (PRIM_MOVE 3): %s\n", call->primitive->name);
#endif
      return true;
    }
    break;

  case PRIM_GET_LOCALE_ID:
    // It is invalid for both flags to be present.
    if (!(call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
          call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS))) {
#ifdef DEBUG
      printf(" *** OK (PRIM_GET_LOCALE_ID %s\n", call->primitive->name);
#endif
      return true;
    }
    break;

  case PRIM_UNION_SETID:
  case PRIM_UNION_GETID:
  case PRIM_GET_MEMBER_VALUE:
  case PRIM_GET_SVEC_MEMBER_VALUE:
    if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE)) {
      return true;
#ifdef DEBUG
      printf(" *** OK (PRIM_GET_UNION, etc.): %s\n", call->primitive->name);
#endif
    }
    break;

  case PRIM_ARRAY_SET:
  case PRIM_ARRAY_SET_FIRST:
  case PRIM_SETCID:
  case PRIM_ARRAY_GET:
  case PRIM_ARRAY_GET_VALUE:
  case PRIM_TESTCID:
  case PRIM_DYNAMIC_CAST:
    if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
#ifdef DEBUG
      printf(" *** OK (PRIM_ARRAY_SET, etc.): %s\n", call->primitive->name);
#endif
      return true;
    }
    break;

  case PRIM_DEREF:
  case PRIM_SET_MEMBER:
  case PRIM_SET_SVEC_MEMBER:
    if (!call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE) &&
        !call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
#ifdef DEBUG
      printf(" *** OK (PRIM_DEREF, etc.): %s\n", call->primitive->name);
#endif
      return true;
    }
    break;
  default:
    break;
  }
  return false;
}

static bool
markFastSafeFn(FnSymbol *fn, int recurse, Vec<FnSymbol*> *visited) {
  if (fn->hasFlag(FLAG_FAST_ON))
    return true;

  if (fn->hasFlag(FLAG_NON_BLOCKING))
    return false;

  if (fn->hasFlag(FLAG_EXTERN))
    return false;

  visited->add_exclusive(fn);

  Vec<CallExpr*> calls;

  collectCallExprs(fn, calls);

  forv_Vec(CallExpr, call, calls) {
#ifdef DEBUG
    printf("\tcall %p (id=%d): ", call, call->id);
#endif
    if (!call->primitive) {
#ifdef DEBUG
      printf("(non-primitive CALL)\n");
#endif
      if ((recurse>0) && call->isResolved()) {
        if (call->isResolved()->hasFlag(FLAG_ON_BLOCK)) {
          visited->add_exclusive(call->isResolved());
          call->isResolved()->removeFlag(FLAG_FAST_ON);
#ifdef DEBUG
          printf("%d: recurse FAILED (nested on block, id=%d).\n",
                 recurse-1, call->id);
#endif
          return false;
        }
        if (!visited->in(call->isResolved())) {
#ifdef DEBUG
          printf("%d: recurse %p (block=%p, id=%d)\n", recurse-1,
                 call->isResolved(), call->isResolved()->body,
                 call->isResolved()->id);
          printf("\tlength=%d\n", call->isResolved()->body->length());
#endif
          if (!markFastSafeFn(call->isResolved(), recurse-1, visited)) {
#ifdef DEBUG
            printf("%d: recurse FAILED (id=%d).\n", recurse-1, call->id);
#endif
            return false;
          }
        } else {
#ifdef DEBUG
          printf("%d: recurse ALREADY VISITED %p (id=%d)\n", recurse-1,
                 call->isResolved(), call->isResolved()->id);
          printf("\tlength=%d\n", call->isResolved()->body->length());
#endif
        }
#ifdef DEBUG
        printf("%d: recurse DONE.\n", recurse-1);
#endif
      } else {
        // No function calls allowed
#ifdef DEBUG
        printf("%d: recurse FAILED (%s, id=%d).\n", recurse-1,
               recurse == 1 ? "too deep" : "function not resolved", call->id);
#endif
        return false;
      }
    } else if (isFastPrimitive(call)) {
#ifdef DEBUG
      printf(" (FAST primitive CALL)\n");
#endif
    } else {
#ifdef DEBUG
      printf("%d: FAILED (non-FAST primitive CALL: %s, id=%d)\n",
             recurse-1, call->primitive->name, call->id);
#endif
      return false;
    }
  }
  return true;
}


void
optimizeOnClauses(void) {
  if (fNoOptimizeOnClauses)
    return;

  compute_call_sites();

  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (!fn->hasFlag(FLAG_ON_BLOCK))
      continue;
#ifdef DEBUG
    printf("%p (%s in %s:%d): FLAG_ON_BLOCK (block=%p, id=%d)\n",
           fn, fn->cname, toModuleSymbol(fn->defPoint->parentSymbol)->filename,
           fn->linenum(), fn->body, fn->id);
    printf("\tlength=%d\n", fn->body->length());
#endif
    Vec<FnSymbol*> visited;

    if (markFastSafeFn(fn, optimize_on_clause_limit, &visited)) {
#ifdef DEBUG
      printf("\t[CANDIDATE FOR FAST FORK]\n");
#endif
      fn->addFlag(FLAG_FAST_ON);

      if (fReportOptimizedOn) {
        ModuleSymbol *mod = toModuleSymbol(fn->defPoint->parentSymbol);
        INT_ASSERT(mod);
        if (developer ||
            ((mod->modTag != MOD_INTERNAL) && (mod->modTag != MOD_STANDARD))) {
          printf("Optimized on clause (%s) in module %s (%s:%d)\n",
                 fn->cname, mod->name, fn->fname(), fn->linenum());
        }
      }
    }
  }
}
