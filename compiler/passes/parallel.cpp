//
// Transformations for begin, cobegin, and on statements
//

#include "astutil.h"
#include "expr.h"
#include "passes.h"
#include "runtime.h"
#include "stmt.h"
#include "symbol.h"
#include "stringutil.h"
#include "driver.h"
#include "files.h"


// Package args into a class and call a wrapper function with that
// object. The wrapper function will then call the function
// created by the previous parallel pass. This is a way to pass along
// multiple args through the limitation of one arg in the runtime's
// thread creation interface. 
static void
bundleArgs(BlockStmt* block) {
  currentLineno = block->lineno;
  currentFilename = block->filename;
  ModuleSymbol* mod = block->getModule();
  BlockStmt *newb = new BlockStmt();
  newb->blockTag = block->blockTag;
  for_alist(expr, block->body) {
    CallExpr* fcall = toCallExpr(expr);
    if (!fcall || !fcall->isResolved()) {
      block->insertBefore(expr->remove());
      continue;
    } else {
      FnSymbol* fn = fcall->isResolved();

      // create a new class to capture refs to locals
      ClassType* ctype = new ClassType( CLASS_CLASS);
      TypeSymbol* new_c = new TypeSymbol( astr("_class_locals", 
                                               fn->name),
                                          ctype);

      // add the function args as fields in the class
      int i = 1;
      bool first = true;
      for_actuals(arg, fcall) {
        if (block->blockTag == BLOCK_ON && first) {
          first = false;
          continue;
        }
        SymExpr *s = toSymExpr(arg);
        Symbol  *var = s->var; // arg or var
        var->isConcurrent = true;
        VarSymbol* field = new VarSymbol(astr("_", istr(i), "_", var->name), var->type);
        ctype->fields.insertAtTail(new DefExpr(field));
        i++;
      }
      mod->block->insertAtHead(new DefExpr(new_c));
        
      // create the class variable instance and allocate it
      VarSymbol *tempc = new VarSymbol( astr( "_args_for", 
                                              fn->name),
                                        ctype);
      block->insertBefore( new DefExpr( tempc));
      CallExpr *tempc_alloc = new CallExpr( PRIMITIVE_CHPL_ALLOC,
                                            ctype->symbol,
                                            new_StringSymbol( astr( "instance of class ", ctype->symbol->name)));
      block->insertBefore( new CallExpr( PRIMITIVE_MOVE,
                                     tempc,
                                     tempc_alloc));
      
      // set the references in the class instance
      i = 1;
      first = true;
      for_actuals(arg, fcall) {
        if (block->blockTag == BLOCK_ON && first) {
          first = false;
          continue;
        }
        SymExpr *s = toSymExpr(arg);
        Symbol  *var = s->var; // var or arg
        CallExpr *setc=new CallExpr(PRIMITIVE_SET_MEMBER,
                                    tempc,
                                    ctype->getField(i),
                                    var);
        block->insertBefore( setc);
        i++;
      }
      
      // create wrapper-function that uses the class instance

      FnSymbol *wrap_fn = new FnSymbol( astr("wrap", fn->name));
      DefExpr  *fcall_def= (toSymExpr( fcall->baseExpr))->var->defPoint;
      if (block->blockTag == BLOCK_ON) {
        wrap_fn->addPragma("on block");
        ArgSymbol* locale = new ArgSymbol(INTENT_BLANK, "_dummy_locale_arg", dtInt[INT_SIZE_32]);
        wrap_fn->insertFormalAtTail(locale);
      }
      ArgSymbol *wrap_c = new ArgSymbol( INTENT_BLANK, "c", ctype);
      wrap_fn->insertFormalAtTail( wrap_c);

      mod->block->insertAtTail(new DefExpr(wrap_fn));
      if (block->blockTag == BLOCK_ON) {
        newb->insertAtTail(new CallExpr(wrap_fn, fcall->get(1)->remove(), tempc));
      } else
        newb->insertAtTail(new CallExpr(wrap_fn, tempc));

      if (block->blockTag == BLOCK_BEGIN || block->blockTag == BLOCK_COBEGIN)
        wrap_fn->insertAtHead(new CallExpr(PRIMITIVE_THREAD_INIT));
        
      // translate the original cobegin function
      CallExpr *new_cofn = new CallExpr( (toSymExpr(fcall->baseExpr))->var);
      if (block->blockTag == BLOCK_ON)
        new_cofn->insertAtTail(new_IntSymbol(0)); // bogus actual
      for_fields(field, ctype) {  // insert args

        VarSymbol* tmp = new VarSymbol("_tmp", field->type);
        tmp->isCompilerTemp = true;
        wrap_fn->insertAtTail(new DefExpr(tmp));
        wrap_fn->insertAtTail(
          new CallExpr(PRIMITIVE_MOVE, tmp,
            new CallExpr(PRIMITIVE_GET_MEMBER_VALUE, wrap_c, field)));
        new_cofn->insertAtTail(tmp);
      }
      
      wrap_fn->retType = dtVoid;
      fcall->remove();                     // rm orig. call
      wrap_fn->insertAtTail(new_cofn);     // add new call
      if (block->blockTag == BLOCK_ON)
        block->insertAfter(new CallExpr(PRIMITIVE_CHPL_FREE, tempc));
      else
        wrap_fn->insertAtTail(new CallExpr(PRIMITIVE_CHPL_FREE, wrap_c));

      fcall_def->remove();                 // move orig. def
      mod->block->insertAtTail(fcall_def); // to top-level
      normalize(wrap_fn);
    }
  }
  block->replace(newb);
}


void
parallel(void) {
  compute_sym_uses();

  Vec<BlockStmt*> blocks;
  forv_Vec(BaseAST, ast, gAsts) {
    if (BlockStmt* block = toBlockStmt(ast))
      blocks.add(block);
  }

  forv_Vec(BlockStmt, block, blocks) {
    if (block->blockTag == BLOCK_ON ||
        block->blockTag == BLOCK_BEGIN ||
        block->blockTag == BLOCK_COBEGIN)
      bundleArgs(block);
  }
}


//
// change all references into wide references
//
void
insertWideReferences(void) {
  if (fLocal)
    return;

  Map<Type*,Type*> wideMap; // reference -> wide reference

  //
  // build wide reference type for every reference type
  //
  forv_Vec(TypeSymbol, ref, gTypes) {
    if (ref->hasPragma("ref")) {
      ClassType* wide = new ClassType(CLASS_RECORD);
      TypeSymbol* wts = new TypeSymbol(astr("_wide_", ref->cname), wide);
      wts->addPragma("wide");
      theProgram->block->insertAtTail(new DefExpr(wts));
      wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtInt[INT_SIZE_32])));
      wide->fields.insertAtTail(new DefExpr(new VarSymbol("addr", ref->type)));
      wideMap.put(ref->type, wide);
    }
  }

  //
  // change all references into wide references
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = toDefExpr(ast)) {

      //
      // do not change reference field in wide reference type
      //
      if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol))
        if (ts->hasPragma("wide"))
          continue;

      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (Type* wide = wideMap.get(fn->retType))
          fn->retType = wide;
      } else if (!isTypeSymbol(def->sym)) {
        if (Type* wide = wideMap.get(def->sym->type))
          def->sym->type = wide;
      }
    }
  }
}
