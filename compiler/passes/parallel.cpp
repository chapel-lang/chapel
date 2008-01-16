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
      new_c->addPragma("no wide class");

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


static void
buildWideClass(Type* type) {
  ClassType* wide = new ClassType(CLASS_RECORD);
  TypeSymbol* wts = new TypeSymbol(astr("_wide_", type->symbol->cname), wide);
  wts->addPragma("wide class");
  theProgram->block->insertAtTail(new DefExpr(wts));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtInt[INT_SIZE_32])));
  wide->fields.insertAtTail(new DefExpr(new VarSymbol("addr", type)));
  wideClassMap.put(type, wide);

  //
  // build reference type for wide class type
  //
  ClassType* ref = new ClassType(CLASS_CLASS);
  TypeSymbol* rts = new TypeSymbol(astr("_ref_wide_", type->symbol->cname), ref);
  rts->addPragma("ref");
  theProgram->block->insertAtTail(new DefExpr(rts));
  ref->fields.insertAtTail(new DefExpr(new VarSymbol("_val", type)));
  wide->refType = ref;
}


//
// change all classes into wide classes
// change all references into wide references
//
void
insertWideReferences(void) {
  if (fLocal)
    return;

  wideClassMap.clear();

  //
  // build wide class type for every class type
  //
  forv_Vec(TypeSymbol, ts, gTypes) {
    ClassType* ct = toClassType(ts->type);
    if (ct && ct->classTag == CLASS_CLASS && !ts->hasPragma("ref") && !ts->hasPragma("no wide class")) {

      //
      // do not yet support remote sync and single variables
      //
      if (ts->hasPragma("sync") || ts->hasPragma("single"))
        continue;

      buildWideClass(ct);
    }
  }
  buildWideClass(dtObject);

  //
  // change all classes into wide classes
  //
  forv_Vec(BaseAST, ast, gAsts) {
    if (DefExpr* def = toDefExpr(ast)) {

      //
      // do not change class field in wide class type
      //
      if (TypeSymbol* ts = toTypeSymbol(def->parentSymbol))
        if (ts->hasPragma("wide class"))
          continue;

      if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (Type* wide = wideClassMap.get(fn->retType))
          fn->retType = wide;
      } else if (!isTypeSymbol(def->sym)) {
        if (Type* wide = wideClassMap.get(def->sym->type))
          def->sym->type = wide;
      }
    }
  }

  //
  // change arrays of classes into arrays of wide classes
  //
  forv_Vec(TypeSymbol, ts, gTypes) {
    if (ts->hasPragma("data class")) {
      if (Type* nt = wideClassMap.get(toType(ts->type->substitutions.v[0].value))) {
        ts->type->substitutions.v[0].value = nt;
      }
    }
  }

  wideRefMap.clear();

  //
  // build wide reference type for every reference type
  //
  forv_Vec(TypeSymbol, ts, gTypes) {
    if (ts->hasPragma("ref")) {
      ClassType* wide = new ClassType(CLASS_RECORD);
      TypeSymbol* wts = new TypeSymbol(astr("_wide_", ts->cname), wide);
      wts->addPragma("wide");
      theProgram->block->insertAtTail(new DefExpr(wts));
      wide->fields.insertAtTail(new DefExpr(new VarSymbol("locale", dtInt[INT_SIZE_32])));
      wide->fields.insertAtTail(new DefExpr(new VarSymbol("addr", ts->type)));
      wideRefMap.put(ts->type, wide);
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
        if (Type* wide = wideRefMap.get(fn->retType))
          fn->retType = wide;
      } else if (!isTypeSymbol(def->sym)) {
        if (Type* wide = wideRefMap.get(def->sym->type))
          def->sym->type = wide;
      }
    }
  }
}
