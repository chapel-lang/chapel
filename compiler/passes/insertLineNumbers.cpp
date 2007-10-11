#include "astutil.h"
#include "expr.h"
#include "passes.h"
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
  if ((mod->modTag == MOD_USER && !fn->isCompilerTemp) ||
      (developer == true && strcmp(fn->name, "halt"))) {
    // call is in user code; insert AST line number and filename
    // or developer flag is on and the call is not the halt() call
    call->insertAtTail(new_IntSymbol(call->lineno));
    call->insertAtTail(new_StringSymbol(call->filename));
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

void insertLineNumbers() {
  compute_call_sites();

  // loop over all primitives that require a line number and filename
  // and pass them an actual line number and filename
  forv_Vec(BaseAST, ast, gAsts) {
    if (CallExpr * call = toCallExpr(ast)) {
      if (call->primitive && call->primitive->passLineno) {
        insertLineNumber(call);
      }
    }
  }

  // loop over all functions in the queue and all calls to these
  // functions, and pass the calls an actual line number and filename
  forv_Vec(FnSymbol, fn, queue) {
    forv_Vec(CallExpr, call, *fn->calledBy) {
      insertLineNumber(call);
    }
  }
}
