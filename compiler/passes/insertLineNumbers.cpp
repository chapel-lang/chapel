#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"

//
// insertLineNumbers() inserts line numbers and filenames into
// functions and calls so that runtime errors show line number and
// filename information.  The line numbers and filenames always point
// to user code, i.e., code that is not in a standard module or in a
// compiler-generated function.  To this effect, functions that are in
// a standard module or are compiler-generated are passed a line
// number and a filename.
//

//
// The queue keeps track of the functions to which line number and
// filename arguments have been added so that calls to these functions
// can be updated with new actual arguments.
//
static Vec<FnSymbol*> queue;

static Map<FnSymbol*,ArgSymbol*> linenoMap; // fn to line number argument
static Map<FnSymbol*,ArgSymbol*> filenameMap; // fn to filename argument

//
// insert a line number and filename actual into a call; add line
// number and filename formal arguments to the function in which this
// call lives if necessary, and add it to the queue.
//
static void
insertLineNumber(CallExpr* call) {
  FnSymbol* fn = call->getFunction();
  ModuleSymbol* mod = fn->getModule();
  if (!strcmp(fn->name, "_heapAllocateGlobals") ||
      (mod->modTag == MOD_USER && !fn->hasFlag(FLAG_TEMP) && !fn->hasFlag(FLAG_INLINE)) ||
      (developer == true && strcmp(fn->name, "halt"))) {
    // call is in user code; insert AST line number and filename
    // or developer flag is on and the call is not the halt() call
    if (call->isResolved() &&
        call->isResolved()->hasFlag(FLAG_COMMAND_LINE_SETTING)) {
      call->insertAtTail(new_IntSymbol(0));
      FnSymbol* fn = call->isResolved();
      INT_ASSERT(fn);
      INT_ASSERT(fn->substitutions.n);
      VarSymbol* var = toVarSymbol(fn->substitutions.v[0].value);
      INT_ASSERT(var);
      INT_ASSERT(var->immediate);
      INT_ASSERT(var->immediate->const_kind == CONST_KIND_STRING);
      call->insertAtTail(new_StringSymbol(astr("<command line setting of '",
                                               var->immediate->v_string,
                                               "'>")));
    } else {
      call->insertAtTail(new_IntSymbol(call->lineno));
      call->insertAtTail(new_StringSymbol(call->getModule()->filename));
    }
  } else if (linenoMap.get(fn)) {
    // call is in non-user code, but the function already has line
    // number and filename arguments
    call->insertAtTail(linenoMap.get(fn));
    call->insertAtTail(filenameMap.get(fn));
  } else {
    // call is in non-user code, and the function requires new line
    // number and filename arguments
    ArgSymbol* line = new ArgSymbol(INTENT_BLANK, "_ln", dtInt[INT_SIZE_32]);
    ArgSymbol* file = new ArgSymbol(INTENT_BLANK, "_fn", dtString);
    fn->insertFormalAtTail(line);
    fn->insertFormalAtTail(file);
    call->insertAtTail(line);
    call->insertAtTail(file);
    linenoMap.put(fn, line);
    filenameMap.put(fn, file);
    queue.add(fn);
  }
}


static bool isClassMethodCall(CallExpr* call) {
  FnSymbol* fn = call->isResolved();
  if (fn && fn->hasFlag(FLAG_METHOD) && fn->_this) {
    if (ClassType* ct = toClassType(fn->_this->typeInfo())) {
      if (fn->numFormals() > 0 &&
          fn->getFormal(1)->typeInfo() == fn->_this->typeInfo()) {
        if (ct->classTag == CLASS_CLASS || ct->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          return true;
        }
      }
    }
  }
  return false;
}


void insertLineNumbers() {
  compute_call_sites();

  // insert nil checks primitives in front of all member accesses
  if (!fNoNilChecks) {
    forv_Vec(CallExpr, call, gCallExprs) {
      if (call->isPrimitive(PRIMITIVE_GET_MEMBER) ||
          call->isPrimitive(PRIMITIVE_GET_MEMBER_VALUE) ||
          call->isPrimitive(PRIMITIVE_SET_MEMBER) ||
          call->isPrimitive(PRIMITIVE_GETCID) ||
          isClassMethodCall(call)) {
        Expr* stmt = call->getStmtExpr();
        SET_LINENO(stmt);
        ClassType* ct = toClassType(call->get(1)->typeInfo());
        if (ct && (ct->classTag == CLASS_CLASS ||
                   ct->symbol->hasFlag(FLAG_WIDE_CLASS))) {
          stmt->insertBefore(
            new CallExpr(PRIMITIVE_CHECK_NIL, call->get(1)->copy()));
        }
      }
    }
  }

  // loop over all primitives that require a line number and filename
  // and pass them an actual line number and filename
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->primitive && call->primitive->passLineno) {
      insertLineNumber(call);
    }
  }

  // loop over all functions in the queue and all calls to these
  // functions, and pass the calls an actual line number and filename
  forv_Vec(FnSymbol, fn, queue) {
    forv_Vec(CallExpr, call, *fn->calledBy) {
      insertLineNumber(call);
    }
  }

  // pass line number and filename arguments to functions that are
  // forked via the argument class
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isResolved()) {
      if ((call->numActuals() > 2 && call->isResolved()->hasFlag(FLAG_ON_BLOCK)) ||
          (call->numActuals() > 1 && call->isResolved()->hasFlag(FLAG_BEGIN_BLOCK))) {
        Expr* filename = call->argList.tail->remove();
        Expr* lineno = call->argList.tail->remove();
        Expr* argClass = call->argList.tail;
        ClassType* ct = toClassType(argClass->typeInfo());
        VarSymbol* linenoField = newTemp("_ln", lineno->typeInfo());
        ct->fields.insertAtTail(new DefExpr(linenoField));
        VarSymbol* filenameField = newTemp("_fn", filename->typeInfo());
        ct->fields.insertAtTail(new DefExpr(filenameField));
        call->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, argClass->copy(), linenoField, lineno));
        call->insertBefore(new CallExpr(PRIMITIVE_SET_MEMBER, argClass->copy(), filenameField, filename));
        FnSymbol* fn = call->isResolved();
        DefExpr* filenameFormal = toDefExpr(fn->formals.tail);
        filenameFormal->remove();
        DefExpr* linenoFormal = toDefExpr(fn->formals.tail);
        linenoFormal->remove();
        DefExpr* argClassFormal = toDefExpr(fn->formals.tail);
        VarSymbol* filenameLocal = newTemp("_fn", filename->typeInfo());
        VarSymbol* linenoLocal = newTemp("_ln", lineno->typeInfo());
        fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, filenameLocal, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, argClassFormal->sym, filenameField)));
        fn->insertAtHead(new DefExpr(filenameLocal));
        fn->insertAtHead(new CallExpr(PRIMITIVE_MOVE, linenoLocal, new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, argClassFormal->sym, linenoField)));
        fn->insertAtHead(new DefExpr(linenoLocal));
        SymbolMap update;
        update.put(filenameFormal->sym, filenameLocal);
        update.put(linenoFormal->sym, linenoLocal);
        update_symbols(fn->body, &update);
      }
    }
  }
}
