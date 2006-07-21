// This pass performs some transformations to enable parallel code
// generation.

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
          FnSymbol *fn = new FnSymbol( fname, NULL);
          fn->retType = dtVoid;
          for_alist( Stmt, stmt, b->body) {
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
          for_alist( Stmt, stmt, b->body) {
            char *fname = stringcat( "_cobegin_stmt", intstring( ufid++));
            FnSymbol *fn = new FnSymbol( fname, NULL);
            fn->retType = dtVoid;
            stmt->remove();
            fn->insertAtTail( stmt);            // move stmt to new function
            newb->body->insertAtHead( new DefExpr (fn));
            newb->body->insertAtTail( new CallExpr (fname));
          }
          b->insertBefore (newb);
          b->remove ();
          delete b;
        }
      }
    }
  }
}


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
static void
begin_mark_locals() {
  Vec<SymExpr*> arglist;

  // Find all the args that should be heap allocated -> arglist
  forv_Vec( ModuleSymbol, mod, allModules) {
    Vec<BaseAST*> asts;
    collect_asts( &asts, mod);
    forv_Vec( BaseAST, ast, asts) {
      BlockStmt *b = dynamic_cast<BlockStmt*>(ast);
      if (b && (BLOCK_BEGIN == b->blockTag)) {
        // note, should only be one call expr in the body of the begin
        for_alist( Stmt, stmt, b->body) {
          if (ExprStmt *estmt = dynamic_cast<ExprStmt*>( stmt)) {
            if (CallExpr *fcall = dynamic_cast<CallExpr*>( estmt->expr)) {
              // add the args that need to be heap allocated
              for_alist( Expr, arg, fcall->argList) {
                if (SymExpr *s = dynamic_cast<SymExpr*>(arg)) {
                  arglist.add( s);
                }
              }
            } else {
              INT_FATAL( b, "not a call expression in begin block");
            }
          }
        }
      }
    }
  }

  // do a buch of stuff:
  //  - mark locals as heap allocated
  //  - create associated mutex + reference counter
  //  - add mutex + ref-counter to nested function's arg list
  //  - add calls to init ref-counter, touch, and free
  forv_Vec( SymExpr, se, arglist) {
    VarSymbol *local = (VarSymbol*)(se->var);  // var that is referenced
    Stmt      *localdef = local->defPoint->parentStmt;

    if (!local->on_heap) {        // no reference counter associated yet
      local->on_heap = true;
      // create reference counter
      char      *refcname = stringcat( "_", stringcat(local->name, "_refc"));
      VarSymbol *refc = new VarSymbol( refcname,
                                       dtInt[INT_SIZE_64],
                                       VAR_NORMAL,
                                       VAR_VAR);
      refc->on_heap = true;
      localdef->insertBefore( new DefExpr( refc));
      local->refc = refc;
      // create reference counter mutex
      char      *mname = stringcat( "_", stringcat(local->name, "_refcmutex"));
      VarSymbol *m = new VarSymbol( mname, dtMutex, VAR_NORMAL, VAR_VAR);
      m->on_heap = true;          // needs to be heap allocated
      localdef->insertBefore( new DefExpr( m));
      local->refcMutex = m;
    }

    // add refc and mutex args as both actuals and formals
    CallExpr  *ce = (CallExpr*)se->parentExpr;
    ce->argList->insertAtTail( new SymExpr( local->refc));
    ce->argList->insertAtTail( new SymExpr( local->refcMutex)); 
    ArgSymbol *rc_arg = new ArgSymbol( INTENT_REF, 
                                       local->refc->name, 
                                       dtInt[INT_SIZE_64]);
    ArgSymbol *rcm_arg = new ArgSymbol( INTENT_REF, 
                                        local->refcMutex->name, 
                                        dtMutex);
    FnSymbol  *fn = (FnSymbol*) ((SymExpr*)ce->baseExpr)->var;
    fn->formals->insertAtTail(new DefExpr( rc_arg));
    fn->formals->insertAtTail(new DefExpr( rcm_arg));

    localdef->insertAfter( new CallExpr( PRIMITIVE_REFC_TOUCH, 
                                         local,
                                         local->refc,
                                         local->refcMutex));
    localdef->insertAfter( new CallExpr( PRIMITIVE_REFC_INIT, 
                                         local,
                                         local->refc,
                                         local->refcMutex));
    BlockStmt *mainfb = dynamic_cast<BlockStmt*>(localdef->parentStmt);
    Stmt      *laststmt = mainfb->body->last();
    if (dynamic_cast<ReturnStmt*>(laststmt)) {
      laststmt->insertBefore( new CallExpr( PRIMITIVE_REFC_RELEASE, 
                                            local,
                                            local->refc,
                                            local->refcMutex));
    } else {
      laststmt->insertAfter( new CallExpr( PRIMITIVE_REFC_RELEASE, 
                                           local,
                                           local->refc,
                                           local->refcMutex));
    }

    // add touch + release for the begin block
    se->parentStmt->parentStmt->insertBefore( new CallExpr( PRIMITIVE_REFC_TOUCH, 
                                                            local,
                                                            local->refc,
                                                            local->refcMutex));
    ArgSymbol *fa = (ArgSymbol*) actual_to_formal( se);
    fn->body->body->last()->insertBefore( new CallExpr( PRIMITIVE_REFC_RELEASE,
                                                        fa,
                                                        rc_arg,
                                                        rcm_arg));
  }


  // for each on_heap variable, add call to allocate it
  forv_Vec(ModuleSymbol, mod, allModules) {
    Vec<BaseAST*> asts;
    asts.clear();
    collect_asts_postorder(&asts, mod);
    forv_Vec(BaseAST, ast, asts) {
      if (VarSymbol *vs = dynamic_cast<VarSymbol*>(ast)) {
        if (vs->on_heap) {
          CallExpr *alloc = new CallExpr( PRIMITIVE_CHPL_ALLOC, 
                                          vs->type->symbol, 
                                          new_StringLiteral("heap alloc'd via begin"));
          Stmt     *vsdef = vs->defPoint->parentStmt;
          vsdef->insertAfter( new CallExpr( PRIMITIVE_SET_HEAPVAR,
                                            vs->defPoint->sym,
                                            alloc));
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
          for_alist( Stmt, stmt, b->body) {
            if (ExprStmt *estmt = dynamic_cast<ExprStmt*>( stmt)) {
              if (CallExpr *fcall = dynamic_cast<CallExpr*>( estmt->expr)) {
                // create a new class to capture refs to locals
                char* fname = ((SymExpr*)fcall->baseExpr)->var->name;
                ClassType* ctype = new ClassType( CLASS_CLASS);
                TypeSymbol* new_c = new TypeSymbol( stringcat("_class_locals", 
                                                              fname),
                                                    ctype);


                // add the function args as fields in the class
                AList<Stmt>* vlist = new AList<Stmt>();
                for_alist( Expr, arg, fcall->argList) {
                  SymExpr   *s = dynamic_cast<SymExpr*>(arg);
                  ArgSymbol *var = (ArgSymbol*)(s->var);
                  vlist->insertAtTail(new DefExpr(new VarSymbol(var->name,
                                                                var->type,
                                                                VAR_NORMAL,
                                                                VAR_VAR,
                                                                true)));
                }
                ctype->addDeclarations( vlist);
                mod->stmts->insertAtHead( new DefExpr( new_c));

                // create the class variable instance and allocate it
                VarSymbol *tempc = new VarSymbol( stringcat( "_args_for", 
                                                             fname),
                                                  ctype);
                b->insertBefore( new DefExpr( tempc));
                CallExpr *tempc_alloc = new CallExpr( PRIMITIVE_CHPL_ALLOC,
                                                      ctype->symbol,
                                                      new_StringLiteral( stringcat( "instance of class ", ctype->symbol->name)));
                b->insertBefore( new CallExpr( PRIMITIVE_MOVE,
                                               tempc,
                                               tempc_alloc));

                // set the references in the class instance
                for_alist( Expr, arg, fcall->argList) {
                  SymExpr   *s = dynamic_cast<SymExpr*>(arg);
                  ArgSymbol *var = (ArgSymbol*)(s->var);
                  CallExpr *setc=new CallExpr(PRIMITIVE_SET_MEMBER_REF_TO,
                                              tempc,
                                              new_StringLiteral(var->name),
                                              var);
                  b->insertBefore( setc);
                }

                // create wrapper-function that uses the class instance
                FnSymbol *wrap_fn = new FnSymbol( stringcat("wrap", fname), NULL);
                DefExpr  *fcall_def= ((SymExpr*)fcall->baseExpr)->var->defPoint;
                ArgSymbol *wrap_c = new ArgSymbol( INTENT_BLANK, "c", ctype);
                wrap_fn->formals->insertAtTail( wrap_c);
                mod->stmts->insertAtTail( new DefExpr( wrap_fn));
                b->insertAtHead( new CallExpr( wrap_fn, tempc));

                // translate the original cobegin function
                CallExpr *new_cofn = new CallExpr( ((SymExpr*)fcall->baseExpr)->var);
                forv_Vec( Symbol, field, ctype->fields) {  // insert args
                  new_cofn->insertAtTail( new CallExpr(PRIMITIVE_GET_MEMBER_REF_TO,
                                                       wrap_c,
                                                       new_StringSymbol(field->name)));
                }
                wrap_fn->retType = dtVoid;
                fcall->parentStmt->remove();               // rm orig. call
                wrap_fn->insertAtHead( new_cofn);          // add new call
                fcall_def->parentStmt->remove();           // move orig. def
                mod->stmts->insertAtTail( fcall_def);      // to top-level

                build( wrap_fn);
              }
            }
          }
        }
      }
    }
  }
}


void
parallel2 (void) {
  if (parallelPass) {
    begin_mark_locals();
    thread_args();
  }
}
