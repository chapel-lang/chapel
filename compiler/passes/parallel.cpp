// This pass performs some transformations to enable parallel code
// generation for begin and cobegin blocks.

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "stringutil.h"
#include "driver.h"
#include "files.h"


// Move begin blocks into their own functions so that it can be later 
// interfaced with the expected thread interface. 
static void
begin_encapsulation() {
  int  ufid = 1;

  forv_Vec( ModuleSymbol, mod, allModules) {
    Vec<BaseAST*> asts;
    collect_asts( &asts, mod);
    forv_Vec( BaseAST, ast, asts) {
      if (BlockStmt *b = dynamic_cast<BlockStmt*>(ast)) {
        if (BLOCK_BEGIN == b->blockTag) {
          // replace with a new block w/ each stmt -> function call
          BlockStmt *newb = new BlockStmt();
          newb->blockTag = b->blockTag;

          char *fname = stringcat( "_begin_block", intstring( ufid++));
          FnSymbol *fn = new FnSymbol( fname);
          fn->retType = dtVoid;
          for_alist(Expr, stmt, b->body) {
            stmt->remove();
            fn->insertAtTail( stmt);        // move stmts to new begin function
          }
          b->insertAtTail( new DefExpr( fn));
          b->insertAtTail( new CallExpr( fname));
        }
      }
    }
  }
}


// This pass moves each cobegin statement into nested functions. Currently,
// each statement is moved to within it's own function and the
// appropriate function def and call expressions are added. 
static void
cobegin_encapsulation() {
  int  ufid = 1;

  forv_Vec( ModuleSymbol, mod, allModules) {
    Vec<BaseAST*> asts;
    collect_asts( &asts, mod);
    forv_Vec( BaseAST, ast, asts) {
      if (BlockStmt *b = dynamic_cast<BlockStmt*>(ast)) {
        if (BLOCK_COBEGIN == b->blockTag) {
          // replace with a new block w/ each stmt -> function call
          BlockStmt *newb = new BlockStmt();
          newb->blockTag = b->blockTag;
          for_alist(Expr, stmt, b->body) {
            char *fname = stringcat( "_cobegin_stmt", intstring( ufid++));
            FnSymbol *fn = new FnSymbol( fname);
            fn->retType = dtVoid;
            stmt->remove();
            fn->insertAtTail( stmt);            // move stmt to new function
            newb->insertAtHead( new DefExpr (fn));
            newb->insertAtTail( new CallExpr (fname));
          }
          b->insertBefore (newb);
          b->remove ();
        }
      }
    }
  }
}


// First pass of the parallel transformations for begin and cobegin blocks.
void
parallel1 (void) {
  addLibInfo ("-lpthread");
  if (parallelPass) {
    begin_encapsulation();     // move begin block to within a function
    cobegin_encapsulation();   // move cobegin stmts to within a function
  }
}


// Mark locals that should be heap allocated and insert a call to allocate
// them on the heap.  This is for begin blocks where the forked child thread 
// and parent thread may have different lifetimes.  The locals cannot live 
// on a thread's stack.  
//   In addition to moving these vars to the heap, we will also use
// reference counting to garbage collect.  Will need a counter and mutex 
// for each var.  Of course, those will be heap allocated also.
static void
begin_mark_locals() {
  Vec<SymExpr*> heapList;

  compute_sym_uses();

  // Find all the args that should be heap allocated -> heapList
  forv_Vec(BaseAST, ast, gAsts) {
    if (BlockStmt* block = dynamic_cast<BlockStmt*>(ast)) {
      if (block->blockTag == BLOCK_BEGIN) {
        CallExpr* call = dynamic_cast<CallExpr*>(block->body->tail);
        assert(call);
        for_alist(Expr, expr, block->body) {
          if (CallExpr* move = dynamic_cast<CallExpr*>(expr)) {
            if (move->isPrimitive(PRIMITIVE_MOVE)) {
              if (CallExpr* ref = dynamic_cast<CallExpr*>(move->get(2))) {
                if (ref->isPrimitive(PRIMITIVE_SET_REF)) {
                  SymExpr* se = dynamic_cast<SymExpr*>(ref->get(1));
                  assert(se);
                  VarSymbol* var = dynamic_cast<VarSymbol*>(se->var);
                  assert(var);
                  DefExpr* def = var->defPoint;

                  SymExpr* lse = dynamic_cast<SymExpr*>(move->get(1));
                  assert(lse->var->uses.n == 1);
                  assert(lse->var->defs.n == 1);
 
                  SymExpr* use = lse->var->uses.v[0];
                  lse->var->defPoint->remove();
                  use->var = var;
                  move->remove();

                  if (!var->refc) { // no reference counter associated yet
                    assert(var->type->refType);

                    forv_Vec(SymExpr, se, var->uses) {
                      if (!se->parentSymbol)
                        continue;
                      CallExpr* call = dynamic_cast<CallExpr*>(se->parentExpr);
                      if (call && call->isPrimitive(PRIMITIVE_SET_REF)) {
                        call->replace(se->remove());
                      } else if (!call || !(call->isPrimitive(PRIMITIVE_SET_MEMBER) || call->isPrimitive(PRIMITIVE_GET_MEMBER))) {
                        VarSymbol* tmp = new VarSymbol("_tmp", var->type);
                        se->getStmtExpr()->insertBefore(new DefExpr(tmp));
                        se->getStmtExpr()->insertBefore(new CallExpr(PRIMITIVE_MOVE, tmp, new CallExpr(PRIMITIVE_GET_REF, var)));
                        se->var = tmp;
                      }
                    }

                    var->type = var->type->refType;
                    
                    // create reference counter
                    VarSymbol *refc = new VarSymbol(astr("_", var->name, "_refc"),
                                                    dtInt[INT_SIZE_32]->refType,
                                                    VAR_NORMAL,
                                                    VAR_VAR);
                    def->insertBefore(new DefExpr(refc));
                    var->refc = refc;
                    
                    // create reference counter mutex
                    VarSymbol *m = new VarSymbol(astr("_", var->name, "_refcmutex"),
                                                 dtMutex_p,
                                                 VAR_NORMAL,
                                                 VAR_VAR);
                    def->insertBefore(new DefExpr(m));
                    var->refcMutex = m;

                    def->insertBefore(
                      new CallExpr(PRIMITIVE_SET_HEAPVAR, m,
                        new CallExpr(PRIMITIVE_CHPL_ALLOC, dtMutex->symbol, 
                          new_StringSymbol("alloc begin heap"))));
                    def->insertBefore(
                      new CallExpr(PRIMITIVE_SET_HEAPVAR, refc,
                        new CallExpr(PRIMITIVE_CHPL_ALLOC, dtInt[INT_SIZE_32]->symbol, 
                          new_StringSymbol("alloc begin heap"))));
                    def->insertBefore(
                      new CallExpr(PRIMITIVE_SET_HEAPVAR, var,
                        new CallExpr(PRIMITIVE_CHPL_ALLOC, getValueType(var->type)->symbol, 
                          new_StringSymbol("alloc begin heap"))));
                  }

                  call->insertAtTail(var->refc);
                  call->insertAtTail(var->refcMutex);
                  ArgSymbol *rc_arg = new ArgSymbol( INTENT_BLANK, 
                                                     var->refc->name, 
                                                     dtInt[INT_SIZE_32]->refType);
                  ArgSymbol *rcm_arg = new ArgSymbol(INTENT_BLANK, 
                                                      var->refcMutex->name, 
                                                      dtMutex_p);
                  FnSymbol  *fn = call->isResolved();
                  fn->insertFormalAtTail(new DefExpr( rc_arg));
                  fn->insertFormalAtTail(new DefExpr( rcm_arg));

                  def->insertAfter( new CallExpr( PRIMITIVE_REFC_TOUCH, 
                                                  var,
                                                  var->refc,
                                                  var->refcMutex));
                  def->insertAfter( new CallExpr( PRIMITIVE_REFC_INIT, 
                                                  var,
                                                  var->refc,
                                                  var->refcMutex));
                  BlockStmt *mainfb = dynamic_cast<BlockStmt*>(def->parentExpr);
                  Expr      *laststmt = mainfb->body->last();
                  CallExpr* ret = dynamic_cast<CallExpr*>(laststmt);
                  if (ret && ret->isPrimitive(PRIMITIVE_RETURN)) {
                    laststmt->insertBefore( new CallExpr( PRIMITIVE_REFC_RELEASE, 
                                                          var,
                                                          var->refc,
                                                          var->refcMutex));
                  } else {
                    laststmt->insertAfter( new CallExpr( PRIMITIVE_REFC_RELEASE, 
                                                         var,
                                                         var->refc,
                                                         var->refcMutex));
                  }
                  
                  // add touch + release for the begin block
                  block->insertBefore( new CallExpr( PRIMITIVE_REFC_TOUCH, 
                                                     var,
                                                     var->refc,
                                                     var->refcMutex));
                  fn->insertBeforeReturn(new CallExpr(PRIMITIVE_REFC_RELEASE,
                                                      actual_to_formal(use),
                                                      rc_arg,
                                                      rcm_arg));
                }
              }
            }
          }
        }
      }
    }
  }
}


// Package args into a class and call a wrapper function with that
// object. The wrapper function will then call the function
// created by the previous parallel pass. This is a way to pass along
// multiple args through the limitation of one arg in the runtime's
// thread creation interface. 
static void
thread_args() {
  forv_Vec( ModuleSymbol, mod, allModules) {
    Vec<BaseAST*> asts;
    collect_asts( &asts, mod);
    forv_Vec( BaseAST, ast, asts) {
      if (BlockStmt *b = dynamic_cast<BlockStmt*>(ast)) {
        if ((BLOCK_BEGIN == b->blockTag) ||
            (BLOCK_COBEGIN == b->blockTag)) {
          BlockStmt *newb = new BlockStmt();
          newb->blockTag = b->blockTag;
          for_alist(Expr, expr, b->body) {
            CallExpr* fcall = dynamic_cast<CallExpr*>(expr);
            if (!fcall || !fcall->isResolved()) {
              b->insertBefore(expr->remove());
              continue;
            } else {

              // create a new class to capture refs to locals
              char* fname = (dynamic_cast<SymExpr*>(fcall->baseExpr))->var->name;
              ClassType* ctype = new ClassType( CLASS_CLASS);
              TypeSymbol* new_c = new TypeSymbol( stringcat("_class_locals", 
                                                            fname),
                                                  ctype);

              // disable reference counting, handle manually
              new_c->addPragma("no gc");
              
              // add the function args as fields in the class
              int i = 1;
              for_actuals(arg, fcall) {
                SymExpr *s = dynamic_cast<SymExpr*>(arg);
                Symbol  *var = s->var; // arg or var
                var->isConcurrent = true;
                VarSymbol* field = new VarSymbol(astr("_", intstring(i), "_", var->name), var->type);
                ctype->fields->insertAtTail(new DefExpr(field));
                i++;
              }
              mod->block->insertAtHead(new DefExpr(new_c));
              
              // create the class variable instance and allocate it
              VarSymbol *tempc = new VarSymbol( stringcat( "_args_for", 
                                                           fname),
                                                ctype);
              b->insertBefore( new DefExpr( tempc));
              CallExpr *tempc_alloc = new CallExpr( PRIMITIVE_CHPL_ALLOC,
                                                    ctype->symbol,
                                                    new_StringSymbol( stringcat( "instance of class ", ctype->symbol->name)));
              b->insertBefore( new CallExpr( PRIMITIVE_MOVE,
                                             tempc,
                                             tempc_alloc));
              
              // set the references in the class instance
              i = 1;
              for_actuals(arg, fcall) {
                SymExpr *s = dynamic_cast<SymExpr*>(arg);
                Symbol  *var = s->var; // var or arg
                CallExpr *setc=new CallExpr(PRIMITIVE_SET_MEMBER,
                                            tempc,
                                            ctype->getField(i),
                                            var);
                b->insertBefore( setc);
                i++;
              }
              
              // create wrapper-function that uses the class instance
              FnSymbol *wrap_fn = new FnSymbol( stringcat("wrap", fname));
              DefExpr  *fcall_def= (dynamic_cast<SymExpr*>( fcall->baseExpr))->var->defPoint;
              ArgSymbol *wrap_c = new ArgSymbol( INTENT_BLANK, "c", ctype);
              wrap_fn->insertFormalAtTail( wrap_c);
              mod->block->insertAtTail(new DefExpr(wrap_fn));
              newb->insertAtTail( new CallExpr( wrap_fn, tempc));
              wrap_fn->insertAtTail(new CallExpr(PRIMITIVE_CHPL_FREE, wrap_c));

              // translate the original cobegin function
              CallExpr *new_cofn = new CallExpr( (dynamic_cast<SymExpr*>(fcall->baseExpr))->var);
              for_fields(field, ctype) {  // insert args
                new_cofn->insertAtTail( new CallExpr(PRIMITIVE_GET_MEMBER_VALUE,
                                                     wrap_c,
                                                     field));
              }

              wrap_fn->retType = dtVoid;
              fcall->remove();                     // rm orig. call
              wrap_fn->insertAtHead(new_cofn);     // add new call
              wrap_fn->insertAtHead(new CallExpr(PRIMITIVE_THREAD_INIT));
              fcall_def->remove();                 // move orig. def
              mod->block->insertAtTail(fcall_def); // to top-level
              normalize(wrap_fn);
            }
          }
          b->replace(newb);
        }
      }
    }
  }
}


// Second pass of the parallel transformations for begin and cobegin blocks.
void
parallel2 (void) {
  if (parallelPass) {
    begin_mark_locals();
    thread_args();
  }
}
