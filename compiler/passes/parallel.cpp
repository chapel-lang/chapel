// This pass performs some transformations to enable parallel code
// generation.

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "stmt.h"
#include "symbol.h"
#include "stringutil.h"
#include "driver.h"
#include "runtime.h"         // for prelude


// This pass moves cobegin statements into nested functions. Currently,
// each statement is moved to within it's own function and the
// appropriate function def and call expressions are added. 
void
cobegin_setup() {
  int  ufid = 1;

  forv_Vec( ModuleSymbol, mod, allModules) {
    Vec<BaseAST*> asts;
    collect_asts( &asts, mod);
    forv_Vec( BaseAST, ast, asts) {
      if (BlockStmt *b = dynamic_cast<BlockStmt*>(ast)) {
        if (BLOCK_COBEGIN == b->blockTag) {
          // replace with a new cobegin block w/ each stmt -> function call
          BlockStmt *newb = new BlockStmt();
          newb->blockTag = BLOCK_COBEGIN;
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
  if (parallelPass) {
    cobegin_setup();      // move cobegin stmts to within a function
  }
}


// Package args into a class and call a wrapper function with that
// object. The wrapper function will then call the function
// created by the previous parallel pass. This is a way to pass along
// multiple args through the limitation of one arg in the runtime's
// thread creation interface. 
void
cobegin_thread_args() {
  forv_Vec( ModuleSymbol, mod, allModules) {
    Vec<BaseAST*> asts;
    collect_asts( &asts, mod);
    forv_Vec( BaseAST, ast, asts) {
      if (BlockStmt *b = dynamic_cast<BlockStmt*>(ast)) {
        if (BLOCK_COBEGIN == b->blockTag) {
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
                char     *wrapn = stringcat("wrap", fname);
                FnSymbol *wrap_fn = new FnSymbol( wrapn, NULL);
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
    cobegin_thread_args();
  }
}
