/*
 * Copyright 2004-2015 Cray Inc.
 * Other additional copyright holders may be indicated within.
 *
 * The entirety of this work is licensed under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 *
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "build.h"

#include "astutil.h"
#include "stlUtil.h"
#include "baseAST.h"
#include "config.h"
#include "expr.h"
#include "files.h"
#include "ForLoop.h"
#include "ParamForLoop.h"
#include "parser.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"

static BlockStmt* findStmtWithTag(PrimitiveTag tag, BlockStmt* blockStmt);

static void
checkControlFlow(Expr* expr, const char* context) {
  Vec<const char*> labelSet; // all labels in expr argument
  Vec<BaseAST*> loopSet;     // all asts in a loop in expr argument
  Vec<BaseAST*> innerFnSet;  // all asts in a function in expr argument
  std::vector<BaseAST*> asts;
  collect_asts(expr, asts);

  //
  // compute labelSet and loopSet
  //
  for_vector(BaseAST, ast, asts) {
    if (DefExpr* def = toDefExpr(ast)) {
      if (LabelSymbol* ls = toLabelSymbol(def->sym))
        labelSet.set_add(ls->name);
      else if (FnSymbol* fn = toFnSymbol(def->sym)) {
        if (!innerFnSet.set_in(fn)) {
          std::vector<BaseAST*> innerAsts;
          collect_asts(fn, innerAsts);
          for_vector(BaseAST, ast, innerAsts) {
            innerFnSet.set_add(ast);
          }
        }
      }
    } else if (BlockStmt* block = toBlockStmt(ast)) {
      if (block->isLoopStmt() && !loopSet.set_in(block)) {
        if (block->userLabel != NULL) {
          labelSet.set_add(block->userLabel);
        }
        std::vector<BaseAST*> loopAsts;
        collect_asts(block, loopAsts);
        for_vector(BaseAST, ast, loopAsts) {
          loopSet.set_add(ast);
        }
      }
    }
  }

  //
  // check for illegal control flow
  //
  for_vector(BaseAST, ast1, asts) {
    if (CallExpr* call = toCallExpr(ast1)) {
      if (innerFnSet.set_in(call))
        continue; // yield or return is in nested function/iterator
      if (call->isPrimitive(PRIM_RETURN)) {
        USR_FATAL_CONT(call, "return is not allowed in %s", context);
      } else if (call->isPrimitive(PRIM_YIELD)) {
        if (!strcmp(context, "begin statement"))
          USR_FATAL_CONT(call, "yield is not allowed in %s", context);
      }
    } else if (GotoStmt* gs = toGotoStmt(ast1)) {
      if (labelSet.set_in(gs->getName()))
        continue; // break or continue target is in scope
      if (toSymExpr(gs->label) && toSymExpr(gs->label)->var == gNil && loopSet.set_in(gs))
        continue; // break or continue loop is in scope
      if (!strcmp(context, "on statement")) {
        USR_PRINT(gs, "the following error is a current limitation");
      }
      if (gs->gotoTag == GOTO_BREAK) {
        USR_FATAL_CONT(gs, "break is not allowed in %s", context);
      } else if (gs->gotoTag == GOTO_CONTINUE) {
        USR_FATAL_CONT(gs, "continue is not allowed in %s", context);
      } else {
        USR_FATAL_CONT(gs, "illegal 'goto' usage; goto is deprecated anyway");
      }
    }
  }
}


static void addPragmaFlags(Symbol* sym, Vec<const char*>* pragmas) {
  forv_Vec(const char, str, *pragmas) {
    Flag flag = pragma2flag(str);
    if (flag == FLAG_UNKNOWN)
      USR_FATAL_CONT(sym, "unknown pragma: \"%s\"", str);
    else {
      sym->addFlag(flag);

      if (flag == FLAG_RUNTIME_TYPE_INIT_FN) {
        //
        // These functions must be marked as type functions early in
        // compilation, as calls to them are inserted by the compiler
        // at the declaration points for arrays and domains.  In the
        // past, they had to be defined as type functions in the
        // modules, but most of us found that very confusing because
        // the code in the functions actually returns a value.  See
        // buildRuntimTypeToValueFns() in functionResolution.cpp for
        // more info on what happens to these functions.
        //
        FnSymbol* fn = toFnSymbol(sym);
        INT_ASSERT(fn);
        if (fn->retTag != RET_VALUE) {
          USR_WARN(fn, "function's return type is not a value type.  Ignoring.");
        }
        fn->retTag = RET_TYPE;
      }
    }
  }
}

BlockStmt* buildPragmaStmt(Vec<const char*>* pragmas,
                           BlockStmt* stmt) {
  bool error = false;
  for_alist(expr, stmt->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      addPragmaFlags(def->sym, pragmas);
    } else {
      error = true;
      break;
    }
  }
  if (error && pragmas->n > 0) {
    USR_FATAL_CONT(stmt, "cannot attach pragmas to this statement");
    USR_PRINT(stmt, "   %s \"%s\"",
              pragmas->n == 1 ? "pragma" : "starting with pragma",
              pragmas->v[0]);
  }
  delete pragmas;

  return stmt;
}


//
// A helper function that's useful in a few places in this file to
// conditionally convert an Expr to a 'const char*' in the event that
// it's an immediate string.
//
static const char* toImmediateString(Expr* expr) {
  if (SymExpr* se = toSymExpr(expr)) {
    if (VarSymbol* var = toVarSymbol(se->var)) {
      if (var->isImmediate()) {
        Immediate* imm = var->immediate;
        if (imm->const_kind == CONST_KIND_STRING) {
          return imm->v_string;
        }
      }
    }
  }
  return NULL;
}


CallExpr* buildOneTuple(Expr* elem) {
  return new CallExpr("_build_tuple", elem);
}

CallExpr* buildTuple(CallExpr* call) {
  // The call is expected to be a PRIM_ACTUALS_LIST.
  INT_ASSERT(call->isPrimitive(PRIM_ACTUALS_LIST));
  return new CallExpr("_build_tuple", call);
}


Expr* buildSquareCallExpr(Expr* base, CallExpr* args) {
  CallExpr* call = new CallExpr(base, args);
  call->square = true;
  return call;
}


Expr* buildNamedActual(const char* name, Expr* expr) {
  return new NamedExpr(name, expr);
}


Expr* buildNamedAliasActual(const char* name, Expr* expr) {
  return new CallExpr(PRIM_ACTUALS_LIST,
           new NamedExpr(name, expr),
           new NamedExpr(astr("chpl__aliasField_", name), new SymExpr(gTrue)));
}


Expr* buildFormalArrayType(Expr* iterator, Expr* eltType, Expr* index) {
  if (index) {
    CallExpr* indexCall = toCallExpr(index);
    INT_ASSERT(indexCall);
    if (indexCall->numActuals() != 1)
      USR_FATAL(iterator, "invalid index expression");
    return new CallExpr("chpl__buildArrayRuntimeType",
             new CallExpr("chpl__ensureDomainExpr", iterator),
             eltType, indexCall->get(1)->remove(),
             new CallExpr("chpl__ensureDomainExpr", iterator->copy()));
  } else {
    CallExpr* call = toCallExpr(iterator);
    if (call->numActuals() == 1 && isDefExpr(call->get(1))) {
      return new CallExpr("chpl__buildArrayRuntimeType", call->get(1)->remove(), eltType);
    } else
      return new CallExpr("chpl__buildArrayRuntimeType",
               new CallExpr("chpl__ensureDomainExpr", iterator), eltType);
  }
}

Expr* buildIntLiteral(const char* pch) {
  uint64_t ull;
  if (!strncmp("0b", pch, 2) || !strncmp("0B", pch, 2))
    ull = binStr2uint64(pch);
  else if (!strncmp("0o", pch, 2) || !strncmp("0O", pch, 2))
    // The second case is difficult to read, but is zero followed by a capital
    // letter 'o'
    ull = octStr2uint64(pch);
  else if (!strncmp("0x", pch, 2) || !strncmp("0X", pch, 2))
    ull = hexStr2uint64(pch);
  else
    ull = str2uint64(pch);
  if (ull <= 9223372036854775807ull)
    return new SymExpr(new_IntSymbol(ull, INT_SIZE_64));
  else
    return new SymExpr(new_UIntSymbol(ull, INT_SIZE_64));
}


Expr* buildRealLiteral(const char* pch) {
  return new SymExpr(new_RealSymbol(pch));
}


Expr* buildImagLiteral(const char* pch) {
  char* str = strdup(pch);
  str[strlen(pch)-1] = '\0';
  SymExpr* se = new SymExpr(new_ImagSymbol(str));
  free(str);
  return se;
}


Expr* buildStringLiteral(const char* pch) {
  return new SymExpr(new_StringSymbol(pch));
}

Expr* buildCStringLiteral(const char* pch) {
  return new SymExpr(new_CStringSymbol(pch));
}


Expr* buildDotExpr(BaseAST* base, const char* member) {
  // The following optimization was added to avoid calling
  // chpl_localeID_to_locale when all we end up doing is extracting
  // the locale id or the node id, thus: OPTIMIZATION:
  //   chpl_localeID_to_locale(_get_locale_id(x)).localeid ==>
  //      _get_locale_id(x)
  // AND
  //   chpl_localeID_to_locale(_get_locale_id(x)).id ==>
  //      _get_locale_node_id(x)
  //
  // <hilde complaints paraphrased by sungeun, who agrees>
  // - We shouldn't have optimizations in the parser
  // - These strcmps suck
  if (!strcmp("id", member) || !strcmp("localeid", member)) {
    if (CallExpr* intToLocale = toCallExpr(base))
      if (intToLocale->isNamed("chpl_localeID_to_locale"))
        if (CallExpr* getLocale = toCallExpr(intToLocale->get(1)))
          if (getLocale->isPrimitive(PRIM_WIDE_GET_LOCALE)) {
            if (!strcmp("id", member))
              return new CallExpr(PRIM_WIDE_GET_NODE, getLocale->get(1)->remove());
            else
              return new CallExpr(PRIM_WIDE_GET_LOCALE, getLocale->get(1)->remove());
          }
  }

  if (!strcmp("locale", member))
    // MAGIC: "x.locale" member access expressions are rendered as
    // chpl_localeID_to_locale(_wide_get_node(x)).
    return new CallExpr("chpl_localeID_to_locale",
                        new CallExpr(PRIM_WIDE_GET_LOCALE, base));
  else
    return new CallExpr(".", base, new_CStringSymbol(member));
}


Expr* buildDotExpr(const char* base, const char* member) {
  return buildDotExpr(new UnresolvedSymExpr(base), member);
}


static Expr* buildLogicalAndExpr(BaseAST* left, BaseAST* right) {
  VarSymbol* lvar = newTemp();

  lvar->addFlag(FLAG_MAYBE_PARAM);

  FnSymbol*  ifFn = buildIfExpr(new CallExpr("isTrue", lvar),
                                new CallExpr("isTrue", right),
                                new SymExpr(gFalse));

  VarSymbol* eMsg = new_CStringSymbol("cannot promote short-circuiting && operator");

  ifFn->insertAtHead(new CondStmt(new CallExpr("_cond_invalid", lvar),
                                  new CallExpr("compilerError", eMsg)));

  ifFn->insertAtHead(new CallExpr(PRIM_MOVE, lvar, left));
  ifFn->insertAtHead(new DefExpr(lvar));

  return new CallExpr(new DefExpr(ifFn));
}


static Expr* buildLogicalOrExpr(BaseAST* left, BaseAST* right) {
  VarSymbol* lvar = newTemp();

  lvar->addFlag(FLAG_MAYBE_PARAM);

  FnSymbol*  ifFn = buildIfExpr(new CallExpr("isTrue", lvar),
                               new SymExpr(gTrue),
                               new CallExpr("isTrue", right));

  VarSymbol* eMsg = new_CStringSymbol("cannot promote short-circuiting || operator");

  ifFn->insertAtHead(new CondStmt(new CallExpr("_cond_invalid", lvar),
                                  new CallExpr("compilerError", eMsg)));

  ifFn->insertAtHead(new CallExpr(PRIM_MOVE, lvar, left));
  ifFn->insertAtHead(new DefExpr(lvar));

  return new CallExpr(new DefExpr(ifFn));
}


BlockStmt* buildChapelStmt(Expr* expr) {
  return new BlockStmt(expr, BLOCK_SCOPELESS);
}


static void addModuleToSearchList(CallExpr* newUse, BaseAST* module) {
  UnresolvedSymExpr* modNameExpr = toUnresolvedSymExpr(module);
  if (modNameExpr) {
    addModuleToParseList(modNameExpr->unresolved, newUse);
  } else if (CallExpr* callExpr = toCallExpr(module)) {
    addModuleToSearchList(newUse, callExpr->argList.first());
  }
}


static BlockStmt* buildUseList(BaseAST* module, BlockStmt* list) {
  CallExpr* newUse = new CallExpr(PRIM_USE, module);
  addModuleToSearchList(newUse, module);
  if (list == NULL) {
    return buildChapelStmt(newUse);
  } else {
    list->insertAtTail(newUse);
    return list;
  }
}

//
// Given a string literal argument from a 'use' statement, process
// that argument.  We assume it's either a "-llib" flag,
// or a source filename like "foo.h", "foo.c", "foo.o", etc.  
//
// - For the former, pass to addLibInfo(), the same function that
//   handles command line -l flags.
//
// - Otherwise, assume it's the latter and pass it to our source file
//   handler (which itself handles cases it doesn't recognize).
//
static void processStringInUseStmt(const char* str) {
  if (strncmp(str, "-l", 2) == 0) {
    addLibInfo(str);
  } else {
    addSourceFile(str);
  }
}


//
// Build a 'use' statement
//
BlockStmt* buildUseStmt(CallExpr* args) {
  BlockStmt* list = NULL;

  //
  // Iterate over the expressions being 'use'd, processing them
  //
  for_actuals(expr, args) {
    Expr* useArg = expr->remove();

    //
    // if this is a string argument to 'use', process it
    //
    if (const char* str = toImmediateString(useArg)) {
      processStringInUseStmt(str);
    } else {
      //
      // Otherwise, handle it in the traditional way
      //
      list = buildUseList(useArg, list);
    }
  }

  //
  // If all of them are consumed, replace the use statement by a no-op
  //
  if (list == NULL) {
    list = buildChapelStmt(new CallExpr(PRIM_NOOP));
  }
  
  return list;
}


static void
buildTupleVarDeclHelp(Expr* base, BlockStmt* decls, Expr* insertPoint) {
  int count = 1;
  for_alist(expr, decls->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (strcmp(def->sym->name, "chpl__tuple_blank")) {
        def->init = new CallExpr(base->copy(), new_IntSymbol(count));
        insertPoint->insertBefore(def->remove());
      } else {
        def->remove();
      }
    } else if (BlockStmt* blk = toBlockStmt(expr)) {
      buildTupleVarDeclHelp(new CallExpr(base, new_IntSymbol(count)),
                            blk, insertPoint);
    } else {
      INT_FATAL(expr, "unexpected expression in buildTupleVarDeclHelp");
    }
    count++;
  }
  decls->remove();
}


BlockStmt*
buildTupleVarDeclStmt(BlockStmt* tupleBlock, Expr* type, Expr* init) {
  VarSymbol* tmp = newTemp();
  int count = 1;
  for_alist(expr, tupleBlock->body) {
    if (DefExpr* def = toDefExpr(expr)) {
      if (strcmp(def->sym->name, "chpl__tuple_blank")) {
        def->init = new CallExpr(tmp, new_IntSymbol(count));
      } else {
        def->remove();
      }
    } else if (BlockStmt* blk = toBlockStmt(expr)) {
      buildTupleVarDeclHelp(new CallExpr(tmp, new_IntSymbol(count)), blk, expr);
    }
    count++;
  }
  //
  // Add compiler errors if tmp is not a tuple or if tmp.size is not the
  // same as the number of variables.  These checks will get inserted in
  // buildVarDecls after it asserts that only DefExprs are in this block.
  //
  tupleBlock->blockInfoSet(new CallExpr("_check_tuple_var_decl", tmp, new_IntSymbol(count-1)));
  tupleBlock->insertAtHead(new DefExpr(tmp, init, type));
  return tupleBlock;
}


BlockStmt*
buildLabelStmt(const char* name, Expr* stmt) {
  BlockStmt* block = toBlockStmt(stmt);

  if (block) {
    Expr* breakLabelStmt = block->body.tail;

    if (!isDefExpr(breakLabelStmt) && isDefExpr(breakLabelStmt->prev)) {
      // the last statement in the block could be a call to _freeIterator()
      breakLabelStmt = breakLabelStmt->prev;
    }

    if (LoopStmt* loop = toLoopStmt(breakLabelStmt->prev)) {
      if (loop->breakLabelGet() == 0 || loop->continueLabelGet() == 0) {
        USR_FATAL(stmt, "cannot label parallel loop");
      } else {
        loop->userLabel = astr(name);
      }
    } else {
      USR_FATAL(stmt, "cannot label non-loop statement");
    }

  } else {
    USR_FATAL(stmt, "cannot label non-loop statement");
  }

  return block;
}


BlockStmt*
buildIfStmt(Expr* condExpr, Expr* thenExpr, Expr* elseExpr) {
  if (UnresolvedSymExpr* use = toUnresolvedSymExpr(condExpr))
    if (!strcmp(use->unresolved, gTryToken->name))
      return buildChapelStmt(new CondStmt(condExpr, thenExpr, elseExpr));
  return buildChapelStmt(new CondStmt(new CallExpr("_cond_test", condExpr), thenExpr, elseExpr));
}

BlockStmt*
buildExternBlockStmt(const char* c_code) {
  return buildChapelStmt(new ExternBlockStmt(c_code));
}

ModuleSymbol* buildModule(const char* name, BlockStmt* block, const char* filename, bool priv, const char* docs) {
  ModuleSymbol* mod = new ModuleSymbol(name, currentModuleType, block);
  if (currentFileNamedOnCommandLine) {
    mod->addFlag(FLAG_MODULE_FROM_COMMAND_LINE_FILE);
  }
  if (priv) {
    mod->addFlag(FLAG_PRIVATE);
  }

  mod->filename = astr(filename);
  mod->doc      = docs;

  return mod;
}



CallExpr* buildPrimitiveExpr(CallExpr* exprs) {
  INT_ASSERT(exprs->isPrimitive(PRIM_ACTUALS_LIST));
  if (exprs->argList.length == 0)
    INT_FATAL("primitive has no name");
  Expr* expr = exprs->get(1);
  expr->remove();
  if (const char* primname = toImmediateString(expr)) {
    if (PrimitiveOp* prim = primitives_map.get(primname)) {
      return new CallExpr(prim, exprs);
    } else {
      INT_FATAL(expr, "primitive not found '%s'", primname);
    }
  } else {
    INT_FATAL(expr, "primitive with non-literal string name");
  }
  return NULL;
}


FnSymbol* buildIfExpr(Expr* e, Expr* e1, Expr* e2) {
  static int uid = 1;

  if (!e2)
    USR_FATAL("if-then expressions currently require an else-clause");

  FnSymbol* ifFn = new FnSymbol(astr("_if_fn", istr(uid++)));
  ifFn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
  ifFn->addFlag(FLAG_INLINE);
  VarSymbol* tmp1 = newTemp();
  VarSymbol* tmp2 = newTemp();
  tmp1->addFlag(FLAG_MAYBE_PARAM);
  tmp2->addFlag(FLAG_MAYBE_TYPE);

  ifFn->addFlag(FLAG_MAYBE_PARAM);
  ifFn->addFlag(FLAG_MAYBE_TYPE);
  ifFn->insertAtHead(new DefExpr(tmp1));
  ifFn->insertAtHead(new DefExpr(tmp2));
  ifFn->insertAtTail(new CallExpr(PRIM_MOVE, new SymExpr(tmp1), new CallExpr("_cond_test", e)));
  ifFn->insertAtTail(new CondStmt(
    new SymExpr(tmp1),
    new CallExpr(PRIM_MOVE,
                 new SymExpr(tmp2),
                 new CallExpr(PRIM_LOGICAL_FOLDER,
                              new SymExpr(tmp1),
                              new CallExpr(PRIM_DEREF, e1))),
    new CallExpr(PRIM_MOVE,
                 new SymExpr(tmp2),
                 new CallExpr(PRIM_LOGICAL_FOLDER,
                              new SymExpr(tmp1),
                              new CallExpr(PRIM_DEREF, e2)))));
  ifFn->insertAtTail(new CallExpr(PRIM_RETURN, tmp2));
  return ifFn;
}


CallExpr* buildLetExpr(BlockStmt* decls, Expr* expr) {
  static int uid = 1;
  FnSymbol* fn = new FnSymbol(astr("_let_fn", istr(uid++)));
  fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
  fn->addFlag(FLAG_INLINE);
  fn->insertAtTail(decls);
  fn->insertAtTail(new CallExpr(PRIM_RETURN, expr));
  return new CallExpr(new DefExpr(fn));
}

BlockStmt* buildSerialStmt(Expr* cond, BlockStmt* body) {
  cond = new CallExpr("_cond_test", cond);
  BlockStmt *sbody = new BlockStmt();
  VarSymbol *serial_state = newTemp();
  sbody->insertAtTail(new DefExpr(serial_state, new CallExpr(PRIM_GET_SERIAL)));
  sbody->insertAtTail(new CondStmt(cond, new CallExpr(PRIM_SET_SERIAL, gTrue)));
  sbody->insertAtTail(body);
  sbody->insertAtTail(new CallExpr(PRIM_SET_SERIAL, serial_state));
  return sbody;
}


//
// check validity of indices in loops and expressions
//
void
checkIndices(BaseAST* indices) {
  if (CallExpr* call = toCallExpr(indices)) {
    if (!call->isNamed("_build_tuple"))
      USR_FATAL(indices, "invalid index expression");
    for_actuals(actual, call)
      checkIndices(actual);
  } else if (!isSymExpr(indices) && !isUnresolvedSymExpr(indices))
    USR_FATAL(indices, "invalid index expression");
}


void
destructureIndices(BlockStmt* block,
                   BaseAST* indices,
                   Expr* init,
                   bool coforall) {
  if (CallExpr* call = toCallExpr(indices)) {
    if (call->isNamed("_build_tuple")) {
      int i = 1;
      for_actuals(actual, call) {
        if (UnresolvedSymExpr* use = toUnresolvedSymExpr(actual)) {
          if (!strcmp(use->unresolved, "chpl__tuple_blank")) {
            i++;
            continue;
          }
        }
        destructureIndices(block, actual,
                           new CallExpr(init->copy(), new_IntSymbol(i)),
                           coforall);
        i++;
      }
    } else {
      INT_FATAL("Unexpected call type");
    }
  } else if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(indices)) {
    VarSymbol* var = new VarSymbol(sym->unresolved);
    block->insertAtHead(new CallExpr(PRIM_MOVE, var, init));
    block->insertAtHead(new DefExpr(var));
    var->addFlag(FLAG_INDEX_VAR);
    if (coforall)
      var->addFlag(FLAG_COFORALL_INDEX_VAR);
#ifndef HILDE_MM
    var->addFlag(FLAG_INSERT_AUTO_DESTROY);
#endif
  } else if (SymExpr* sym = toSymExpr(indices)) {
    block->insertAtHead(new CallExpr(PRIM_MOVE, sym->var, init));
    sym->var->addFlag(FLAG_INDEX_VAR);
    if (coforall)
      sym->var->addFlag(FLAG_COFORALL_INDEX_VAR);
#ifndef HILDE_MM
    sym->var->addFlag(FLAG_INSERT_AUTO_DESTROY);
#endif
  } else {
    INT_FATAL("Unexpected");
  }
}


static BlockStmt*
handleArrayTypeCase(FnSymbol* fn, Expr* indices, Expr* iteratorExpr, Expr* expr) {
  BlockStmt* block = new BlockStmt();
  fn->addFlag(FLAG_MAYBE_TYPE);
  bool hasSpecifiedIndices = !!indices;
  if (!hasSpecifiedIndices)
    indices = new UnresolvedSymExpr("chpl__elidedIdx");
  checkIndices(indices);

  //
  // nested function to compute isArrayType which is set to true if
  // the inner expression is a type and false otherwise
  //
  // this nested function is called in a type block so that it is
  // never executed; placing all this code in a separate function
  // inside the type block is essential for two reasons:
  //
  // first, so that the iterators in any nested parallel loop
  // expressions are not pulled all the way out during cleanup
  //
  // second, so that types and functions declared in this nested
  // function do not get removed from the IR when the type block gets
  // removed
  //
  FnSymbol* isArrayTypeFn = new FnSymbol("_isArrayTypeFn");
  isArrayTypeFn->addFlag(FLAG_INLINE);

  Symbol* isArrayType = newTemp("_isArrayType");
  isArrayType->addFlag(FLAG_MAYBE_PARAM);
  fn->insertAtTail(new DefExpr(isArrayType));

  VarSymbol* iteratorSym = newTemp("_iterator");
  isArrayTypeFn->insertAtTail(new DefExpr(iteratorSym));
  isArrayTypeFn->insertAtTail(new CallExpr(PRIM_MOVE, iteratorSym,
                                new CallExpr("_getIterator", iteratorExpr->copy())));
  VarSymbol* index = newTemp("_indexOfInterest");
  index->addFlag(FLAG_INDEX_OF_INTEREST);
  isArrayTypeFn->insertAtTail(new DefExpr(index));
  isArrayTypeFn->insertAtTail(new CallExpr(PRIM_MOVE, index,
                                new CallExpr("iteratorIndex", iteratorSym)));
  BlockStmt* indicesBlock = new BlockStmt();
  destructureIndices(indicesBlock, indices->copy(), new SymExpr(index), false);
  indicesBlock->blockTag = BLOCK_SCOPELESS;
  isArrayTypeFn->insertAtTail(indicesBlock);
  isArrayTypeFn->insertAtTail(new CondStmt(
                                new CallExpr("isType", expr->copy()),
                                new CallExpr(PRIM_MOVE, isArrayType, gTrue),
                                new CallExpr(PRIM_MOVE, isArrayType, gFalse)));
  fn->insertAtTail(new DefExpr(isArrayTypeFn));
  BlockStmt* typeBlock = new BlockStmt();
  typeBlock->blockTag = BLOCK_TYPE;
  typeBlock->insertAtTail(new CallExpr(isArrayTypeFn));
  fn->insertAtTail(typeBlock);

  Symbol* arrayType = newTemp("_arrayType");
  arrayType->addFlag(FLAG_EXPR_TEMP);
  arrayType->addFlag(FLAG_MAYBE_TYPE);
  BlockStmt* thenStmt = new BlockStmt();
  thenStmt->insertAtTail(new DefExpr(arrayType));
  Symbol* domain = newTemp("_domain");
  domain->addFlag(FLAG_EXPR_TEMP);
  thenStmt->insertAtTail(new DefExpr(domain));
  // note that we need the below autoCopy until we start reference
  // counting domains within runtime array types
  // TODO AMM: Check if the explicit insertion of an autoCopy is necessary here.
  thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, domain,
                           new CallExpr("chpl__autoCopy",
                             new CallExpr("chpl__ensureDomainExpr",
                                          iteratorExpr->copy()))));
  if (hasSpecifiedIndices) {
    // we want to swap something like the below commented-out
    // statement with the compiler error statement but skyline
    // arrays are not yet supported...
    thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, arrayType, new CallExpr("compilerError", new_CStringSymbol("unimplemented feature: if you are attempting to use skyline arrays, they are not yet supported; if not, remove the index expression from this array type specification"))));
    //      thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, arrayType,
    //                                          new CallExpr("chpl__buildArrayRuntimeType",
    //                                                       domain, expr->copy(),
    //                                                       indices->copy(), domain)));
  } else {
    thenStmt->insertAtTail(new CallExpr(PRIM_MOVE, arrayType,
                             new CallExpr("chpl__buildArrayRuntimeType",
                                          domain, expr->copy())));
  }
  thenStmt->insertAtTail(new CallExpr(PRIM_RETURN, arrayType));
  fn->insertAtTail(new CondStmt(new SymExpr(isArrayType), thenStmt, block));
  return block;
}


//
// build serial iterator function
//
static void buildSerialIteratorFn(FnSymbol* fn, const char* iteratorName,
                                  Expr* expr, Expr* cond, Expr* indices,
                                  bool zippered, Expr*& stmt)
{
  FnSymbol* sifn = new FnSymbol(iteratorName);
  sifn->addFlag(FLAG_ITERATOR_FN);
  ArgSymbol* sifnIterator = new ArgSymbol(INTENT_BLANK, "iterator", dtAny);
  sifn->insertFormalAtTail(sifnIterator);
  fn->insertAtHead(new DefExpr(sifn));
  stmt = new CallExpr(PRIM_YIELD, expr);
  if (cond)
    stmt = new CondStmt(new CallExpr("_cond_test", cond), stmt);
  sifn->insertAtTail(ForLoop::buildForLoop(indices, new SymExpr(sifnIterator), new BlockStmt(stmt), false, zippered));
}


static int loopexpr_uid = 1;

// builds body of for expression iterator
CallExpr*
buildForLoopExpr(Expr* indices, Expr* iteratorExpr, Expr* expr, Expr* cond, bool maybeArrayType, bool zippered) {
  FnSymbol* fn = new FnSymbol(astr("_seqloopexpr", istr(loopexpr_uid++)));
  fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
  BlockStmt* block = fn->body;

  if (maybeArrayType) {
    INT_ASSERT(!cond);
    block = handleArrayTypeCase(fn, indices, iteratorExpr, expr);
  }

  VarSymbol* iterator = newTemp("_iterator");
  iterator->addFlag(FLAG_EXPR_TEMP);
  block->insertAtTail(new DefExpr(iterator));
  block->insertAtTail(new CallExpr(PRIM_MOVE, iterator, new CallExpr("_checkIterator", iteratorExpr)));
  const char* iteratorName = astr("_iterator_for_loopexpr", istr(loopexpr_uid-1));
  block->insertAtTail(new CallExpr(PRIM_RETURN, new CallExpr(iteratorName, iterator)));

  Expr* stmt = NULL; // Initialized by buldSerialIteratorFn
  buildSerialIteratorFn(fn, iteratorName, expr, cond, indices, zippered, stmt);

  return new CallExpr(new DefExpr(fn));
}


//
// build leader iterator function
//
static void buildLeaderIteratorFn(FnSymbol* fn, const char* iteratorName,
                                  bool zippered)
{
  FnSymbol* lifn = new FnSymbol(iteratorName);

  Expr* tag = buildDotExpr(buildDotExpr(new UnresolvedSymExpr("ChapelBase"),
                                        iterKindTypename),
                           iterKindLeaderTagname);
  ArgSymbol* lifnTag = new ArgSymbol(INTENT_PARAM, "tag", dtUnknown,
                                     new CallExpr(PRIM_TYPEOF, tag));
  lifn->insertFormalAtTail(lifnTag);
  ArgSymbol* lifnIterator = new ArgSymbol(INTENT_BLANK, "iterator", dtAny);
  lifn->insertFormalAtTail(lifnIterator);

  lifn->where = new BlockStmt(new CallExpr("==", lifnTag, tag->copy()));
  fn->insertAtHead(new DefExpr(lifn));

  VarSymbol* leaderIterator = newTemp("_leaderIterator");
  leaderIterator->addFlag(FLAG_EXPR_TEMP);
  lifn->insertAtTail(new DefExpr(leaderIterator));

  if( !zippered ) {
    lifn->insertAtTail(new CallExpr(PRIM_MOVE, leaderIterator, new CallExpr("_toLeader", lifnIterator)));
  } else {
    lifn->insertAtTail(new CallExpr(PRIM_MOVE, leaderIterator, new CallExpr("_toLeaderZip", lifnIterator)));
  }

  lifn->insertAtTail(new CallExpr(PRIM_RETURN, leaderIterator));
}


//
// build follower iterator function
//
static FnSymbol* buildFollowerIteratorFn(FnSymbol* fn, const char* iteratorName,
                                         bool zippered, VarSymbol*& followerIterator)
{
  FnSymbol* fifn = new FnSymbol(iteratorName);
  fifn->addFlag(FLAG_ITERATOR_FN);

  Expr* tag = buildDotExpr(buildDotExpr(new UnresolvedSymExpr("ChapelBase"),
                                  iterKindTypename), iterKindFollowerTagname);
  ArgSymbol* fifnTag = new ArgSymbol(INTENT_PARAM, "tag", dtUnknown,
                                     new CallExpr(PRIM_TYPEOF, tag));
  fifn->insertFormalAtTail(fifnTag);
  ArgSymbol* fifnFollower = new ArgSymbol(INTENT_BLANK, iterFollowthisArgname, dtAny);
  fifn->insertFormalAtTail(fifnFollower);
  ArgSymbol* fifnIterator = new ArgSymbol(INTENT_BLANK, "iterator", dtAny);
  fifn->insertFormalAtTail(fifnIterator);

  fifn->where = new BlockStmt(new CallExpr("==", fifnTag, tag->copy()));
  fn->insertAtHead(new DefExpr(fifn));
  followerIterator = newTemp("_followerIterator");
  followerIterator->addFlag(FLAG_EXPR_TEMP);
  fifn->insertAtTail(new DefExpr(followerIterator));

  if( !zippered ) {
    fifn->insertAtTail(new CallExpr(PRIM_MOVE, followerIterator, new CallExpr("_toFollower", fifnIterator, fifnFollower)));
  } else {
    fifn->insertAtTail(new CallExpr(PRIM_MOVE, followerIterator, new CallExpr("_toFollowerZip", fifnIterator, fifnFollower)));
  }
  return fifn;
}


CallExpr*
buildForallLoopExpr(Expr* indices, Expr* iteratorExpr, Expr* expr, Expr* cond, bool maybeArrayType, bool zippered) {
  FnSymbol* fn = new FnSymbol(astr("_parloopexpr", istr(loopexpr_uid++)));
  fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
  BlockStmt* block = fn->body;

  if (maybeArrayType) {
    INT_ASSERT(!cond);
    block = handleArrayTypeCase(fn, indices, iteratorExpr, expr);
  }

  VarSymbol* iterator = newTemp("_iterator");
  iterator->addFlag(FLAG_EXPR_TEMP);
  block->insertAtTail(new DefExpr(iterator));
  block->insertAtTail(new CallExpr(PRIM_MOVE, iterator, new CallExpr("_checkIterator", iteratorExpr)));
  const char* iteratorName = astr("_iterator_for_loopexpr", istr(loopexpr_uid-1));
  block->insertAtTail(new CallExpr(PRIM_RETURN, new CallExpr(iteratorName, iterator)));

  Expr* stmt = NULL; // Initialized by buildSerialIteratorFn.
  buildSerialIteratorFn(fn, iteratorName, expr, cond, indices, zippered, stmt);
  buildLeaderIteratorFn(fn, iteratorName, zippered);
  VarSymbol* followerIterator; // Initialized by buildFollowerIteratorFn.
  FnSymbol* fifn = buildFollowerIteratorFn(fn, iteratorName, zippered, followerIterator);

  // do we need to use this map since symbols have not been resolved?
  SymbolMap map;
  Expr* indicesCopy = (indices) ? indices->copy(&map) : NULL;
  Expr* bodyCopy = stmt->copy(&map);
  fifn->insertAtTail(ForLoop::buildForLoop(indicesCopy, new SymExpr(followerIterator), new BlockStmt(bodyCopy), false, zippered));

  return new CallExpr(new DefExpr(fn));
}


//
// This is a helper function that takes a chpl_buildArrayRuntimeType(...)
// CallExpr and converts it into a forall loop expression.  See the
// commit messages of r20820 and the commit that added this comment
// for (a few) more details.
//
CallExpr* buildForallLoopExprFromArrayType(CallExpr* buildArrRTTypeCall,
                                           bool recursiveCall) {
  // Is this a call to chpl__buildArrayRuntimeType?
  UnresolvedSymExpr* ursym = toUnresolvedSymExpr(buildArrRTTypeCall->baseExpr);
  if (!ursym) {
    INT_FATAL("Unexpected CallExpr format in buildForallLoopExprFromArrayType");
  }
  if (strcmp(ursym->unresolved, "chpl__buildArrayRuntimeType") == 0) {
    // If so, let's process it...

    Expr* EltExpr = buildArrRTTypeCall->get(2)->remove();
    Expr* DomExpr = buildArrRTTypeCall->get(1)->remove();

    // if the element type is itself an array, we need to do this same
    // conversion to forall loops recursively
    if (CallExpr* EltExprAsCall = toCallExpr(EltExpr)) {
      EltExpr = buildForallLoopExprFromArrayType(EltExprAsCall, true);
    }
    return buildForallLoopExpr(NULL, DomExpr, EltExpr, NULL, true);
  } else {
    // if we get something other than a "build array runtime type" call...
    if (recursiveCall) {
      // ...we're in the base case if this was a recursive call
      return buildArrRTTypeCall;
    } else {
      // ...or something is wrong if we're not
      INT_FATAL("buildForallLoopExprFromArrayType() wasn't called with a call to chpl__buildArrayRuntimeType as expected");
      return NULL;
    }
  }
}

static BlockStmt*
buildFollowLoop(VarSymbol* iter,
                VarSymbol* leadIdxCopy,
                VarSymbol* followIter,
                VarSymbol* followIdx,
                Expr*      indices,
                BlockStmt* loopBody,
                bool       fast,
                bool       zippered) {
  BlockStmt* followBlock = new BlockStmt();
  ForLoop*   followBody  = new ForLoop(followIdx, followIter, loopBody, zippered);

  destructureIndices(followBody, indices, new SymExpr(followIdx), false);

  followBlock->insertAtTail(new DefExpr(followIter));

  if (fast) {

    if (zippered) {
      followBlock->insertAtTail("'move'(%S, _getIteratorZip(_toFastFollowerZip(%S, %S)))", followIter, iter, leadIdxCopy);
    } else {
      followBlock->insertAtTail("'move'(%S, _getIterator(_toFastFollower(%S, %S)))",       followIter, iter, leadIdxCopy);
    }
  } else {

    if (zippered) {
      followBlock->insertAtTail("'move'(%S, _getIteratorZip(_toFollowerZip(%S, %S)))",     followIter, iter, leadIdxCopy);
    } else {
      followBlock->insertAtTail("'move'(%S, _getIterator(_toFollower(%S, %S)))",           followIter, iter, leadIdxCopy);
    }
  }

  followBlock->insertAtTail(new DefExpr(followIdx));
  followBlock->insertAtTail("{TYPE 'move'(%S, iteratorIndex(%S)) }", followIdx, followIter);

  followBlock->insertAtTail(followBody);
  followBlock->insertAtTail(new CallExpr("_freeIterator", followIter));

  return followBlock;
}

// Do whatever is needed for a reduce intent.
// Return the globalOp symbol.
static void setupOneReduceIntent(VarSymbol* iterRec, BlockStmt* parLoop,
                                 Expr* reduceOp, Expr* reduceVar,
                                 Expr* otherROp)
{
  // Otherwise reduceVar->copy() may not be adequate.
  INT_ASSERT(isUnresolvedSymExpr(reduceVar));

  VarSymbol* globalOp = newTemp("chpl__reduceGlob");
  iterRec->defPoint->insertBefore(new DefExpr(globalOp));
  reduceOp->replace(new SymExpr(globalOp));
  if (otherROp)
    otherROp->replace(new SymExpr(globalOp));

  // globalOp = new reduceOp(eltType = reduceVar.type)
  Expr* eltType = new NamedExpr("eltType",
                    new_Expr("'typeof'(%E)", reduceVar->copy()));
  iterRec->defPoint->insertBefore(new_Expr("'move'(%S, 'new'(%E(%E)))",
                                           globalOp, reduceOp, eltType));
  // reduceVar = globalOp.generate(); delete globalOp;
  parLoop->insertAfter("'delete'(%S)", globalOp);
  parLoop->insertAfter(new_Expr("'='(%E,.(%S, 'generate')())",
                                reduceVar->copy(), globalOp));
}

// Setup for forall intents
static void setupForallIntents(CallExpr* withClause,
                               CallExpr* otherWith,
                               VarSymbol* iterRec,
                               VarSymbol* leadIdx,
                               VarSymbol* leadIdxCopy,
                               BlockStmt* parLoop)
{
  // To iterate over two withClause in parallel.
  Expr* otherActual = otherWith ? otherWith->argList.head : NULL;
  Expr* otherNext   = otherActual ? otherActual->next : NULL;

  // Handle reduce intents, if any.
  // Keep in sync with markOuterVarsWithIntents().
  bool markerTurn = true;
  Expr* reduceOp = NULL;
  Expr* otherROp = NULL;
  for_actuals(actual, withClause) {
    if (markerTurn) {
      markerTurn = false;
      if (SymExpr* se = toSymExpr(actual)) {
        ArgSymbol* tiMarker =  toArgSymbol(se->var);
        INT_ASSERT(tiMarker); // confirm my thinking
      } else {
        reduceOp = actual;
        if (otherActual) otherROp = otherActual;
      }
    } else {
      markerTurn = true;
      if (reduceOp) {
        setupOneReduceIntent(iterRec, parLoop, reduceOp, actual, otherROp);
        reduceOp = NULL;
        otherROp = NULL;
      }
    }
    // Advance the iteration over otherWith.
    otherActual = otherNext;
    otherNext = otherNext ? otherNext->next: NULL;
  }
  INT_ASSERT(markerTurn);
  INT_ASSERT(!reduceOp);
  INT_ASSERT(!otherROp);
  INT_ASSERT(!otherActual);

  // ForallLeaderArgs: stash references so we know where things are.
  INT_ASSERT(withClause->isPrimitive(PRIM_FORALL_LOOP));
  withClause->insertAtHead(leadIdxCopy); // 3rd arg
  withClause->insertAtHead(leadIdx);     // 2nd arg
  withClause->insertAtHead(iterRec);     // 1st arg
}

/*
 * Build a forall loop that has only one level instead of a nested leader
 * follower loop. This single level loop will be handled similarily to
 * the leader loop in a leader/follower based forall.
 */
static BlockStmt*
buildStandaloneForallLoopStmt(Expr* indices,
                              Expr* iterExpr,
                              BlockStmt* loopBody) {
  VarSymbol* iterRec   = newTemp("chpl__iterSA"); // serial iter, SA case
  // these variables correspond to leadXXX vars in buildForallLoopStmt()
  VarSymbol* saIter    = newTemp("chpl__saIter");
  VarSymbol* saIdx     = newTemp("chpl__saIdx");
  VarSymbol* saIdxCopy = newTemp("chpl__saIdxCopy");

  iterRec->addFlag(FLAG_CHPL__ITER);
  saIter->addFlag(FLAG_EXPR_TEMP);
  saIdx->addFlag(FLAG_INDEX_OF_INTEREST);
  saIdx->addFlag(FLAG_INDEX_VAR);
  saIdxCopy->addFlag(FLAG_INDEX_VAR);

  BlockStmt* SABlock = buildChapelStmt();

  SABlock->insertAtTail(new DefExpr(iterRec));
  SABlock->insertAtTail(new DefExpr(saIter));
  SABlock->insertAtTail(new DefExpr(saIdx));
  SABlock->insertAtTail("'move'(%S, _checkIterator(%E))", iterRec, iterExpr);
  SABlock->insertAtTail("'move'(%S, _getIterator(_toStandalone(%S)))", saIter, iterRec);
  SABlock->insertAtTail("{TYPE 'move'(%S, iteratorIndex(%S)) }", saIdx, saIter);

  ForLoop* SABody = new ForLoop(saIdx, saIter, NULL, false);
  destructureIndices(SABody, indices, new SymExpr(saIdxCopy), false);
  SABody->insertAtHead("'move'(%S, %S)", saIdxCopy, saIdx);
  SABody->insertAtHead(new DefExpr(saIdxCopy));

  SABody->insertAtTail(loopBody);
  SABlock->insertAtTail(SABody);
  SABlock->insertAtTail("_freeIterator(%S)", saIter);
  setupForallIntents(loopBody->byrefVars, NULL,
                     iterRec, saIdx, saIdxCopy, SABody);
  return SABlock;
}


/*
 * Build a leader-follower forall loop.  If this is not a zipper iteration,
 * also build a standalone forall loop. Use the chpl__tryToken to run the
 * standalone version when a standalone parallel iterator exists, and otherwise
 * use the leader-follower version.
 *
 * When both versions are created, it will end up as a normalized form of:
 *
 * if chpl__tryToken then
 *   for idx in iter(standalone) do
 *     body(idx);
 * else
 *   for block in iter(leader) {
 *     if doing fast follower then
 *       for idx in iter(follower, block, fast=true) do
 *         body(idx);
 *     else
 *       for idx in iter(follower, block) do
 *         body(idx);
 *   }
 */
BlockStmt*
buildForallLoopStmt(Expr*      indices,
                    Expr*      iterExpr,
                    CallExpr*  byref_vars,
                    BlockStmt* loopBody,
                    bool       zippered) {
  checkControlFlow(loopBody, "forall statement");

  SET_LINENO(loopBody);

  //
  // insert temporary index when elided by user
  //
  if (!indices)
    indices = new UnresolvedSymExpr("chpl__elidedIdx");

  checkIndices(indices);

  //
  // 'byrefVars' will contain a PRIM_FORALL_LOOP, whose "arguments"
  // are variables listed in the forall's with(ref...) clause.
  // This list is processed during implementForallIntents1().
  //
  INT_ASSERT(!loopBody->byrefVars);
  if (byref_vars) {
    INT_ASSERT(byref_vars->isPrimitive(PRIM_ACTUALS_LIST));
    byref_vars->primitive = primitives[PRIM_FORALL_LOOP];
  } else {
    byref_vars = new CallExpr(PRIM_FORALL_LOOP);
  }
  loopBody->byrefVars = byref_vars;

  // ensure it's normal; prevent flatten_scopeless_block() in cleanup.cpp
  loopBody->blockTag = BLOCK_NORMAL;

  // NB these copies do not get byref_vars updates below.
  BlockStmt* loopBodyForFast =
                     (fNoFastFollowers == false) ? loopBody->copy() : NULL;
  BlockStmt* loopBodyForStandalone = (!zippered) ? loopBody->copy() : NULL;

  BlockStmt* resultBlock     = new BlockStmt();

  VarSymbol* iterRec         = newTemp("chpl__iterLF"); // serial iter, LF case

  VarSymbol* leadIter        = newTemp("chpl__leadIter");
  VarSymbol* leadIdx         = newTemp("chpl__leadIdx");
  VarSymbol* leadIdxCopy     = newTemp("chpl__leadIdxCopy");
  ForLoop*   leadForLoop     = new ForLoop(leadIdx, leadIter, NULL, zippered);

  VarSymbol* followIdx       = newTemp("chpl__followIdx");
  VarSymbol* followIter      = newTemp("chpl__followIter");
  BlockStmt* followBlock     = NULL;

  iterRec->addFlag(FLAG_EXPR_TEMP);
  iterRec->addFlag(FLAG_CHPL__ITER);

  leadIdxCopy->addFlag(FLAG_INDEX_VAR);
#ifndef HILDE_MM
  leadIdxCopy->addFlag(FLAG_INSERT_AUTO_DESTROY);
#endif
  followIdx->addFlag(FLAG_INDEX_OF_INTEREST);

  resultBlock->insertAtTail(new DefExpr(iterRec));
  resultBlock->insertAtTail(new DefExpr(leadIter));
  resultBlock->insertAtTail(new DefExpr(leadIdx));

  resultBlock->insertAtTail("'move'(%S, _checkIterator(%E))", iterRec, iterExpr->copy());

  if (zippered == false)
    resultBlock->insertAtTail("'move'(%S, _getIterator(_toLeader(%S)))",    leadIter, iterRec);
  else
    resultBlock->insertAtTail("'move'(%S, _getIterator(_toLeaderZip(%S)))", leadIter, iterRec);

  resultBlock->insertAtTail("{TYPE 'move'(%S, iteratorIndex(%S)) }", leadIdx, leadIter);

  leadForLoop->insertAtTail(new DefExpr(leadIdxCopy));
  leadForLoop->insertAtTail("'move'(%S, %S)", leadIdxCopy, leadIdx);

  followBlock = buildFollowLoop(iterRec,
                                leadIdxCopy,
                                followIter,
                                followIdx,
                                indices,
                                loopBody,
                                false,
                                zippered);

  if (fNoFastFollowers == false) {
    Symbol* T1 = newTemp();
    Symbol* T2 = newTemp();

    VarSymbol* fastFollowIdx   = newTemp("chpl__fastFollowIdx");
    VarSymbol* fastFollowIter  = newTemp("chpl__fastFollowIter");
    BlockStmt* fastFollowBlock = NULL;


    T1->addFlag(FLAG_EXPR_TEMP);
    T1->addFlag(FLAG_MAYBE_PARAM);

    T2->addFlag(FLAG_EXPR_TEMP);
    T2->addFlag(FLAG_MAYBE_PARAM);

    leadForLoop->insertAtTail(new DefExpr(T1));
    leadForLoop->insertAtTail(new DefExpr(T2));

    if (zippered == false) {
      leadForLoop->insertAtTail("'move'(%S, chpl__staticFastFollowCheck(%S))",    T1, iterRec);
      leadForLoop->insertAtTail(new CondStmt(new SymExpr(T1),
                                          new_Expr("'move'(%S, chpl__dynamicFastFollowCheck(%S))",    T2, iterRec),
                                          new_Expr("'move'(%S, %S)", T2, gFalse)));
    } else {
      leadForLoop->insertAtTail("'move'(%S, chpl__staticFastFollowCheckZip(%S))", T1, iterRec);
      leadForLoop->insertAtTail(new CondStmt(new SymExpr(T1),
                                          new_Expr("'move'(%S, chpl__dynamicFastFollowCheckZip(%S))", T2, iterRec),
                                          new_Expr("'move'(%S, %S)", T2, gFalse)));
    }

    fastFollowBlock = buildFollowLoop(iterRec,
                                      leadIdxCopy,
                                      fastFollowIter,
                                      fastFollowIdx,
                                      indices,
                                      loopBodyForFast,
                                      true,
                                      zippered);

    leadForLoop->insertAtTail(new CondStmt(new SymExpr(T2), fastFollowBlock, followBlock));
  } else {
    leadForLoop->insertAtTail(followBlock);
  }

  resultBlock->insertAtTail(leadForLoop);
  resultBlock->insertAtTail("_freeIterator(%S)", leadIter);
  setupForallIntents(byref_vars,
                     loopBodyForFast ? loopBodyForFast->byrefVars : NULL,
                     iterRec, leadIdx, leadIdxCopy, leadForLoop);

  if (!zippered) {
    BlockStmt* SALoop = buildStandaloneForallLoopStmt(indices,
                                                      iterExpr,
                                                      loopBodyForStandalone);
    BlockStmt* result = new BlockStmt();
    result->insertAtTail(
      new CondStmt(new SymExpr(gTryToken), SALoop, resultBlock));
    return result;
  }

  return resultBlock;
}

static void
addByrefVars(BlockStmt* target, CallExpr* byrefVarsSource) {
  // nothing to do if there is no 'ref' clause
  if (!byrefVarsSource) return;

  //
  // 'byrefVars' will contain a CallExpr, whose "arguments"
  // are variables listed in the with(ref...) clause
  // of the enclosing begin/cobegin/coforall.
  // This list is processed during createTaskFunctions().
  //
  // Could set byrefVars->parentExpr/Symbol right here.
  target->byrefVars = byrefVarsSource;

  // Note: the UnresolvedSymExprs in byrefVars
  // will be automatically resolved in resolve().
}

BlockStmt* buildCoforallLoopStmt(Expr* indices,
                                 Expr* iterator,
                                 CallExpr* byref_vars,
                                 BlockStmt* body,
                                 bool zippered)
{
  checkControlFlow(body, "coforall statement");

  //
  // insert temporary index when elided by user
  //
  if (!indices)
    indices = new UnresolvedSymExpr("chpl__elidedIdx");

  checkIndices(indices);

  //
  // detect on-statement directly inside coforall-loop
  //
  BlockStmt* onBlock = findStmtWithTag(PRIM_BLOCK_ON, body);

  SET_LINENO(body);

  if (onBlock) {
    //
    // optimization of on-statements directly inside coforall-loops
    //
    //   In this case, the on-statement is made into a non-blocking
    //   on-statement and the coforall is serialized (rather than
    //   wasting threads that would do nothing other than wait on the
    //   on-statement.
    //
    VarSymbol* coforallCount = newTemp("_coforallCount");
    BlockStmt* block = ForLoop::buildForLoop(indices, iterator, body, true, zippered);
    block->insertAtHead(new CallExpr(PRIM_MOVE, coforallCount, new CallExpr("_endCountAlloc")));
    block->insertAtHead(new DefExpr(coforallCount));
    body->insertAtHead(new CallExpr("_upEndCount", coforallCount, gFalse));
    block->insertAtTail(new CallExpr("_waitEndCount", coforallCount, gFalse));
    block->insertAtTail(new CallExpr("_endCountFree", coforallCount));
    onBlock->blockInfoGet()->primitive = primitives[PRIM_BLOCK_COFORALL_ON];
    addByrefVars(onBlock, byref_vars);
    BlockStmt* innerOnBlock = new BlockStmt();
    for_alist(tmp, onBlock->body) {
      innerOnBlock->insertAtTail(tmp->remove());
    }
    onBlock->insertAtHead(innerOnBlock);
    onBlock->insertAtTail(new CallExpr("_downEndCount", coforallCount));
    return block;
  } else {
    VarSymbol* coforallCount = newTemp("_coforallCount");
    BlockStmt* beginBlk = new BlockStmt();
    beginBlk->blockInfoSet(new CallExpr(PRIM_BLOCK_COFORALL));
    addByrefVars(beginBlk, byref_vars);
    beginBlk->insertAtHead(body);
    beginBlk->insertAtTail(new CallExpr("_downEndCount", coforallCount));
    BlockStmt* block = ForLoop::buildForLoop(indices, iterator, beginBlk, true, zippered);
    block->insertAtHead(new CallExpr(PRIM_MOVE, coforallCount, new CallExpr("_endCountAlloc")));
    block->insertAtHead(new DefExpr(coforallCount));
    block->insertAtTail(new CallExpr(PRIM_PROCESS_TASK_LIST, coforallCount));
    beginBlk->insertBefore(new CallExpr("_upEndCount", coforallCount));
    block->insertAtTail(new CallExpr("_waitEndCount", coforallCount));
    block->insertAtTail(new CallExpr("_endCountFree", coforallCount));
    return block;
  }
}

BlockStmt* buildParamForLoopStmt(const char* index, Expr* range, BlockStmt* stmts) {
  VarSymbol* indexVar = new VarSymbol(index);

  return ParamForLoop::buildParamForLoop(indexVar, range, stmts);
}

BlockStmt*
buildAssignment(Expr* lhs, Expr* rhs, const char* op) {
  INT_ASSERT(op != NULL);
  return buildChapelStmt(new CallExpr(op, lhs, rhs));
}

BlockStmt* buildLAndAssignment(Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = new BlockStmt();
  VarSymbol* ltmp = newTemp();

  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIM_MOVE, ltmp, new CallExpr(PRIM_ADDR_OF, lhs)));
  stmt->insertAtTail(new CallExpr("=",       ltmp, buildLogicalAndExpr(ltmp, rhs)));

  return stmt;
}


BlockStmt* buildLOrAssignment(Expr* lhs, Expr* rhs) {
  BlockStmt* stmt = new BlockStmt();
  VarSymbol* ltmp = newTemp();

  stmt->insertAtTail(new DefExpr(ltmp));
  stmt->insertAtTail(new CallExpr(PRIM_MOVE, ltmp, new CallExpr(PRIM_ADDR_OF, lhs)));
  stmt->insertAtTail(new CallExpr("=",       ltmp, buildLogicalOrExpr(ltmp, rhs)));

  return stmt;
}


BlockStmt* buildSelectStmt(Expr* selectCond, BlockStmt* whenstmts) {
  BlockStmt* block = new BlockStmt();
  CondStmt* otherwise = NULL;
  CondStmt* top = NULL;
  CondStmt* condStmt = NULL;

  VarSymbol* tmp = newTemp();

  tmp->addFlag(FLAG_MAYBE_PARAM);
  tmp->addFlag(FLAG_MAYBE_TYPE);
  tmp->addFlag(FLAG_EXPR_TEMP);

  block->insertAtTail(new DefExpr(tmp));
  block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, selectCond));

  for_alist(stmt, whenstmts->body) {
    CondStmt* when = toCondStmt(stmt);
    if (!when)
      INT_FATAL("error in buildSelectStmt");
    CallExpr* conds = toCallExpr(when->condExpr);
    if (!conds || !conds->isPrimitive(PRIM_WHEN))
      INT_FATAL("error in buildSelectStmt");
    if (conds->numActuals() == 0) {
      if (otherwise)
        USR_FATAL(selectCond, "Select has multiple otherwise clauses");
      otherwise = when;
    } else {
      Expr* expr = NULL;
      for_actuals(whenCond, conds) {
        whenCond->remove();
        if (!expr)
          expr = new CallExpr("==", tmp, whenCond);
        else
          expr = buildLogicalOrExpr(expr, new CallExpr("==",
                                                   tmp,
                                                   whenCond));
      }
      if (!condStmt) {
        condStmt = new CondStmt(new CallExpr("_cond_test", expr), when->thenStmt);
        top = condStmt;
      } else {
        CondStmt* next = new CondStmt(new CallExpr("_cond_test", expr), when->thenStmt);
        condStmt->elseStmt = new BlockStmt(next);
        condStmt = next;
      }
    }
  }
  if (otherwise) {
    if (!condStmt)
      USR_FATAL(selectCond, "Select has no when clauses");
    condStmt->elseStmt = otherwise->thenStmt;
  }

  block->insertAtTail(top);
  return block;
}


static void
buildReduceScanPreface(FnSymbol* fn, Symbol* data, Symbol* eltType, Symbol* globalOp,
                       Expr* opExpr, Expr* dataExpr, bool zippered=false) {
  if (UnresolvedSymExpr* sym = toUnresolvedSymExpr(opExpr)) {
    if (!strcmp(sym->unresolved, "max"))
      sym->unresolved = astr("MaxReduceScanOp");
    else if (!strcmp(sym->unresolved, "min"))
      sym->unresolved = astr("MinReduceScanOp");
  }

  data->addFlag(FLAG_EXPR_TEMP);
  eltType->addFlag(FLAG_MAYBE_TYPE);

  fn->insertAtTail(new DefExpr(data));
  fn->insertAtTail(new DefExpr(eltType));
  fn->insertAtTail(new DefExpr(globalOp));
  fn->insertAtTail("'move'(%S, %E)", data, dataExpr);

  if( !zippered ) {
    fn->insertAtTail("{TYPE 'move'(%S, 'typeof'(chpl__initCopy(iteratorIndex(_getIterator(%S)))))}", eltType, data);
  } else {
    fn->insertAtTail("{TYPE 'move'(%S, 'typeof'(chpl__initCopy(iteratorIndex(_getIteratorZip(%S)))))}", eltType, data);
  }

  fn->insertAtTail("'move'(%S, 'new'(%E(%E)))", globalOp, opExpr, new NamedExpr("eltType", new SymExpr(eltType)));
}


CallExpr* buildReduceExpr(Expr* opExpr, Expr* dataExpr, bool zippered) {
  static int uid = 1;

  FnSymbol* fn = new FnSymbol(astr("chpl__reduce", istr(uid++)));
  fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
  fn->addFlag(FLAG_DONT_DISABLE_REMOTE_VALUE_FORWARDING);
  fn->addFlag(FLAG_INLINE);

  VarSymbol* data = newTemp();
  VarSymbol* eltType = newTemp();
  VarSymbol* globalOp = newTemp();

  buildReduceScanPreface(fn, data, eltType, globalOp, opExpr, dataExpr, zippered);

  BlockStmt* serialBlock = buildChapelStmt();
  VarSymbol* index = newTemp("_index");
  serialBlock->insertAtTail(new DefExpr(index));
  serialBlock->insertAtTail(ForLoop::buildForLoop(new SymExpr(index),
                                                  new SymExpr(data),
                                                  new BlockStmt(new CallExpr(new CallExpr(".", globalOp,
                                                                                          new_CStringSymbol("accumulate")), index)),
                                                  false,
                                                  zippered));

  VarSymbol* leadIdx     = newTemp("chpl__leadIdx");
  VarSymbol* leadIter    = newTemp("chpl__leadIter");
  VarSymbol* leadIdxCopy = newTemp("chpl__leadIdxCopy");
  VarSymbol* followIdx   = newTemp("chpl__followIdx");
  VarSymbol* followIter  = newTemp("chpl__followIter");
  VarSymbol* localOp     = newTemp();

  leadIdxCopy->addFlag(FLAG_INDEX_VAR);
#ifndef HILDE_MM
  leadIdxCopy->addFlag(FLAG_INSERT_AUTO_DESTROY);
#endif

  ForLoop* followBody = new ForLoop(followIdx, followIter, NULL, zippered);

  followBody->insertAtTail(".(%S, 'accumulate')(%S)", localOp, followIdx);

  BlockStmt* followBlock = new BlockStmt();

  followBlock->insertAtTail(new DefExpr(followIter));
  followBlock->insertAtTail(new DefExpr(followIdx));
  followBlock->insertAtTail(new DefExpr(localOp));

  if( !zippered ) {
    followBlock->insertAtTail("'move'(%S, _getIterator(_toFollower(%S, %S)))", followIter, data, leadIdxCopy);
  } else {
    followBlock->insertAtTail("'move'(%S, _getIteratorZip(_toFollowerZip(%S, %S)))", followIter, data, leadIdxCopy);
  }

  followBlock->insertAtTail("{TYPE 'move'(%S, iteratorIndex(%S))}", followIdx, followIter);
  followBlock->insertAtTail("'move'(%S, 'new'(%E(%E)))", localOp, opExpr->copy(), new NamedExpr("eltType", new SymExpr(eltType)));
  followBlock->insertAtTail(followBody);
  followBlock->insertAtTail("chpl__reduceCombine(%S, %S)", globalOp, localOp);
  followBlock->insertAtTail("'delete'(%S)", localOp);
  followBlock->insertAtTail("_freeIterator(%S)", followIter);

  ForLoop* leadBody = new ForLoop(leadIdx, leadIter, NULL, zippered);

  leadBody->insertAtTail(new DefExpr(leadIdxCopy));
  leadBody->insertAtTail("'move'(%S, %S)", leadIdxCopy, leadIdx);
  leadBody->insertAtTail(followBlock);

  BlockStmt* leadBlock = buildChapelStmt();
  leadBlock->insertAtTail(new DefExpr(leadIdx));
  leadBlock->insertAtTail(new DefExpr(leadIter));

  if( !zippered ) {
    leadBlock->insertAtTail("'move'(%S, _getIterator(_toLeader(%S)))", leadIter, data);
  } else {
    leadBlock->insertAtTail("'move'(%S, _getIterator(_toLeaderZip(%S)))", leadIter, data);
  }

  leadBlock->insertAtTail("{TYPE 'move'(%S, iteratorIndex(%S))}", leadIdx, leadIter);
  leadBlock->insertAtTail(leadBody);
  leadBlock->insertAtTail("_freeIterator(%S)", leadIter);
  serialBlock->insertAtHead("compilerWarning('reduce has been serialized (see note in $CHPL_HOME/STATUS)')");

  fn->insertAtTail(new CondStmt(new SymExpr(gTryToken), leadBlock, serialBlock));

  VarSymbol* result = new VarSymbol("result");
  fn->insertAtTail(new DefExpr(result, new CallExpr(new CallExpr(".", globalOp, new_CStringSymbol("generate")))));
  fn->insertAtTail("'delete'(%S)", globalOp);
  fn->insertAtTail("'return'(%S)", result);
  return new CallExpr(new DefExpr(fn));
}


CallExpr* buildScanExpr(Expr* opExpr, Expr* dataExpr, bool zippered) {
  static int uid = 1;

  FnSymbol* fn = new FnSymbol(astr("chpl__scan", istr(uid++)));
  fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);

  VarSymbol* data = newTemp();
  VarSymbol* eltType = newTemp();
  VarSymbol* globalOp = newTemp();

  buildReduceScanPreface(fn, data, eltType, globalOp, opExpr, dataExpr, zippered);

  fn->insertAtTail("compilerWarning('scan has been serialized (see note in $CHPL_HOME/STATUS)')");

  if( !zippered ) {
    fn->insertAtTail("'return'(chpl__scanIterator(%S, %S))", globalOp, data);
  } else {
    fn->insertAtTail("'return'(chpl__scanIteratorZip(%S, %S))", globalOp, data);
  }

  return new CallExpr(new DefExpr(fn));
}


static void
backPropagateInitsTypes(BlockStmt* stmts) {
  Expr* init = NULL;
  Expr* type = NULL;
  DefExpr* prev = NULL;
  for_alist_backward(stmt, stmts->body) {
    if (DefExpr* def = toDefExpr(stmt)) {
      if (def->init || def->exprType) {
        init = def->init;
        type = def->exprType;
      } else {
        if (type)
          prev->exprType =
            new CallExpr(PRIM_TYPEOF, new UnresolvedSymExpr(def->sym->name));
        if (init) {
          if (init->isNoInitExpr()) {
            prev->init = init->copy();
          } else if (type) {
            prev->init = new CallExpr("chpl__readXX",
                                      new UnresolvedSymExpr(def->sym->name));
          } else
            prev->init = new UnresolvedSymExpr(def->sym->name);
        }
        def->init = init;
        def->exprType = type;
      }
      prev = def;
    } else
      INT_FATAL(stmt, "expected DefExpr in backPropagateInitsTypes");
  }
}


BlockStmt* buildVarDecls(BlockStmt* stmts, std::set<Flag> flags, const char* docs) {
  for_alist(stmt, stmts->body) {
    if (DefExpr* defExpr = toDefExpr(stmt)) {
      if (VarSymbol* var = toVarSymbol(defExpr->sym)) {
        if (flags.count(FLAG_EXTERN) && flags.count(FLAG_PARAM))
          USR_FATAL(var, "external params are not supported");

        for (std::set<Flag>::iterator it = flags.begin(); it != flags.end(); ++it) {
          if (*it != FLAG_UNKNOWN) {
            var->addFlag(*it);
          }
        }

        if (var->hasFlag(FLAG_CONFIG)) {
          if (Expr *configInit = getCmdLineConfig(var->name)) {
            // config var initialized on the command line
            if (!isUsedCmdLineConfig(var->name)) {
              useCmdLineConfig(var->name);
              // drop the original init expression on the floor
              if (Expr* a = toExpr(configInit))
                defExpr->init = a;
              else if (Symbol* a = toSymbol(configInit))
                defExpr->init = new SymExpr(a);
              else
                INT_FATAL(stmt, "DefExpr initialized with bad exprType config ast");
            } else {
              // name is ambiguous, must specify module name
              USR_FATAL(var, "Ambiguous config param or type name (%s)", var->name);
            }
          }
        }
        var->doc = docs;
        continue;
      }
    }
    INT_FATAL(stmt, "Major error in setVarSymbolAttributes");
  }
  backPropagateInitsTypes(stmts);
  //
  // If blockInfo is set, this is a tuple variable declaration.
  // Add checks that the expression on the right is a tuple and that
  // the tuple size matches the number of variables. If not, issue
  // compilerErrors. blockInfo has the form:
  // call("_check_tuple_var_decl", rhsTuple, numVars)
  //
  if (stmts->blockInfoGet()) {
    INT_ASSERT(stmts->blockInfoGet()->isNamed("_check_tuple_var_decl"));
    SymExpr* tuple = toSymExpr(stmts->blockInfoGet()->get(1));
    Expr* varCount = stmts->blockInfoGet()->get(2);
    tuple->var->defPoint->insertAfter(
      buildIfStmt(new CallExpr("!=", new CallExpr(".", tuple->remove(),
                                                  new_CStringSymbol("size")),
                               varCount->remove()),
                  new CallExpr("compilerError", new_CStringSymbol("tuple size must match the number of grouped variables"), new_IntSymbol(0))));

    tuple->var->defPoint->insertAfter(
      buildIfStmt(new CallExpr("!", new CallExpr("isTuple", tuple->copy())),
                  new CallExpr("compilerError", new_CStringSymbol("illegal tuple variable declaration with non-tuple initializer"), new_IntSymbol(0))));
    stmts->blockInfoSet(NULL);
  }
  return stmts;
}


DefExpr*
buildClassDefExpr(const char* name,
                  const char* cname,
                  Type*       type,
                  Expr*       inherit,
                  BlockStmt*  decls,
                  Flag        isExtern,
                  const char* docs) {
  AggregateType* ct = toAggregateType(type);
  // Hook the string type in the modules
  // We have to do this here so we can reason about dtString as soon as
  // possible in the compiler. gatherWellKnownTypes runs too late to be of use
  // to us.
  if (strcmp("string", name) == 0) {
    *dtString = *ct;
    // These fields get overwritten with `ct` by the assignment. These fields are
    // set to `this` by the AggregateType constructor so they should still be
    // `dtString`. Fix them back up.
    dtString->fields.parent = dtString;
    dtString->inherits.parent = dtString;
    gAggregateTypes.remove(gAggregateTypes.index(ct));
    delete ct;
    ct = dtString;
  }
  INT_ASSERT(ct);
  TypeSymbol* ts = new TypeSymbol(name, ct);
  DefExpr* def = new DefExpr(ts);
  ct->addDeclarations(decls);
  if (isExtern == FLAG_EXTERN) {
    if (cname) {
      ts->cname = astr(cname);
    }
    ts->addFlag(FLAG_EXTERN);
    ts->addFlag(FLAG_NO_OBJECT);
    ct->defaultValue=NULL;
    if (inherit)
      USR_FATAL_CONT(inherit, "External types do not currently support inheritance");
  }
  if (inherit)
    ct->inherits.insertAtTail(inherit);
  ct->doc = docs;
  return def;
}


//
// If an argument has intent 'INTENT_TYPE', this function sets up the
// ArgSymbol the way downstream passes expect it to be.
//
void setupTypeIntentArg(ArgSymbol* arg) {
  arg->intent = INTENT_BLANK;
  arg->addFlag(FLAG_TYPE_VARIABLE);
  arg->type = dtAny;
}


DefExpr*
buildArgDefExpr(IntentTag tag, const char* ident, Expr* type, Expr* init, Expr* variable) {
  ArgSymbol* arg = new ArgSymbol(tag, ident, dtUnknown, type, init, variable);
  if (arg->intent == INTENT_TYPE) {
    setupTypeIntentArg(arg);
  } else if (!type && !init)
    arg->type = dtAny;
  return new DefExpr(arg);
}


//
// create a single argument and store the tuple-grouped args in the
// variable argument list; these will be moved out of the variable
// argument list in the call to destructureTupleGroupedArgs when the
// formals are added to the formals list in the function (in
// buildFunctionFormal)
//
DefExpr*
buildTupleArgDefExpr(IntentTag tag, BlockStmt* tuple, Expr* type, Expr* init) {
  ArgSymbol* arg = new ArgSymbol(tag, "chpl__tuple_arg_temp", dtUnknown,
                                 type, init, tuple);
  arg->addFlag(FLAG_TEMP);
  if (arg->intent != INTENT_BLANK)
    USR_FATAL(tuple, "intents on tuple-grouped arguments are not yet supported");
  if (!type)
    arg->type = dtTuple;
  return new DefExpr(arg);
}


//
// Destructure tuple function arguments.  Add to the function's where
// clause to match the shape of the tuple being destructured.
//
static void
destructureTupleGroupedArgs(FnSymbol* fn, BlockStmt* tuple, Expr* base) {
  int i = 0;
  for_alist(expr, tuple->body) {
    i++;
    if (DefExpr* def = toDefExpr(expr)) {
      def->init = new CallExpr(base->copy(), new_IntSymbol(i));
      if (!strcmp(def->sym->name, "chpl__tuple_blank")) {
        def->remove();
      } else {
        fn->insertAtHead(def->remove());
      }
    } else if (BlockStmt* inner = toBlockStmt(expr)) {
      destructureTupleGroupedArgs(fn, inner, new CallExpr(base->copy(), new_IntSymbol(i)));
    }
  }

  Expr* where =
    buildLogicalAndExpr(
      new CallExpr("isTuple", base->copy()),
      new CallExpr("==", new_IntSymbol(i),
        new CallExpr(".", base->copy(), new_CStringSymbol("size"))));

  if (fn->where) {
    where = buildLogicalAndExpr(fn->where->body.head->remove(), where);
    fn->where->body.insertAtHead(where);
  } else {
    fn->where = new BlockStmt(where);
  }
}

FnSymbol* buildLambda(FnSymbol *fn) {
  static unsigned int nextId = 0;
  char buffer[100];

  /*
   * The snprintf function is used to prevent a buffer overflow from occurring.
   * Technically, an overflow can only occur if Chapel is compiled on a machine
   * where an unsigned integer can represent numbers larger than 10^86, but it
   * is better to guard against this behavior then leaving someone wondering
   * why we didn't.
   */ 
  if (snprintf(buffer, 100, "_chpl_lambda_%i", nextId++) >= 100) {
    INT_FATAL("Too many lambdas.");
  }
  
  if (!fn) {
    fn = new FnSymbol(astr(buffer));
  } else {
    fn->name = astr(buffer);
    fn->cname = fn->name;
  }
  
  fn->addFlag(FLAG_COMPILER_NESTED_FUNCTION);
  return fn;
}


// Replaces the dummy function name "_" with the real name, sets the 'this'
// intent tag. For methods, it also adds a method tag and "this" declaration.
FnSymbol*
buildFunctionSymbol(FnSymbol*   fn,
                    const char* name,
                    IntentTag   thisTag,
                    const char* class_name)
{
  fn->cname   = fn->name = astr(name);
  fn->thisTag = thisTag;

  if (fn->name[0] == '~' && fn->name[1] != '\0')
    fn->addFlag(FLAG_DESTRUCTOR);

  if (class_name)
  {
    ArgSymbol* arg = new ArgSymbol(thisTag,
                                   "this",
                                   dtUnknown,
                                   new UnresolvedSymExpr(class_name));
    fn->_this = arg;
    if (thisTag == INTENT_TYPE) {
      setupTypeIntentArg(arg);
    }

    arg->addFlag(FLAG_ARG_THIS);
    fn->insertFormalAtHead(new DefExpr(arg));

    ArgSymbol* mt = new ArgSymbol(INTENT_BLANK, "_mt", dtMethodToken);

    fn->addFlag(FLAG_METHOD);
    fn->insertFormalAtHead(new DefExpr(mt));
  }

  return fn;
}

// Called like:
// buildFunctionDecl($4, $6, $7, $8, $9, @$.comment);
BlockStmt*
buildFunctionDecl(FnSymbol*   fn,
                  RetTag      optRetTag,
                  Expr*       optRetType,
                  Expr*       optWhere,
                  BlockStmt*  optFnBody,
                  const char* docs)
{
  fn->retTag = optRetTag;

  if (optRetTag == RET_REF)
  {
    if (fn->hasFlag(FLAG_EXTERN))
      USR_FATAL_CONT(fn, "Extern functions cannot be setters.");

    fn->setter = new DefExpr(new ArgSymbol(INTENT_BLANK, "setter", dtBool));
  }

  if (optRetType)
    fn->retExprType = new BlockStmt(optRetType, BLOCK_SCOPELESS);
  else if (fn->hasFlag(FLAG_EXTERN))
    fn->retType     = dtVoid;

  if (optWhere)
  {
    if (fn->hasFlag(FLAG_EXTERN))
      USR_FATAL_CONT(fn, "Extern functions cannot have where clauses.");

    fn->where = new BlockStmt(optWhere);
  }

  if (optFnBody)
  {
    if (fn->hasFlag(FLAG_EXTERN))
      USR_FATAL_CONT(fn, "Extern functions cannot have a body.");

    if (fn->body->length() == 0)
    {
      // Copy the statements from optFnBody to the function's
      // body to preserve line numbers
      for_alist(expr, optFnBody->body)
      {
        fn->body->insertAtTail(expr->remove());
      }

    }
    else
    {
      fn->insertAtTail(optFnBody);
    }

  }
  else
  {
    // Looks like this flag is redundant with FLAG_EXTERN. <hilde>
    fn->addFlag(FLAG_FUNCTION_PROTOTYPE);
  }

  fn->doc = docs;

  return buildChapelStmt(new DefExpr(fn));
}

void applyPrivateToBlock(BlockStmt* block) {
  for_alist(stmt, block->body) {
    if (DefExpr* defExpr = toDefExpr(stmt)) {
      defExpr->sym->addFlag(FLAG_PRIVATE);
    }
  }
}


FnSymbol*
buildFunctionFormal(FnSymbol* fn, DefExpr* def) {
  if (!fn)
    fn = new FnSymbol("_");
  if (!def)
    return fn;
  ArgSymbol* arg = toArgSymbol(def->sym);
  INT_ASSERT(arg);
  fn->insertFormalAtTail(def);
  if (!strcmp(arg->name, "chpl__tuple_arg_temp")) {
    destructureTupleGroupedArgs(fn, arg->variableExpr, new SymExpr(arg));
    arg->variableExpr = NULL;
  }
  return fn;
}


BlockStmt* buildLocalStmt(Expr* stmt) {
  BlockStmt* block = buildChapelStmt();

  if (fLocal) {
    block->insertAtTail(stmt);
    return block;
  }

  BlockStmt* localBlock = new BlockStmt(stmt);
  localBlock->blockInfoSet(new CallExpr(PRIM_BLOCK_LOCAL));
  block->insertAtTail(localBlock);
  return block;
}


static Expr* extractLocaleID(Expr* expr) {
  // If the on <x> expression is a primitive_on_locale_num, we just
  // return the primitive.
  // PRIM_ON_LOCAL_NUM is now passed through to codegen,
  // but we don't want to wrap it in PRIM_WIDE_GET_LOCALE.
  if (CallExpr* call = toCallExpr(expr)) {
    if (call->isPrimitive(PRIM_ON_LOCALE_NUM)) {
      // Can probably use some semantic checks, like the number of args being 1 or 2, etc.
      return expr;
    }
  }

  // Otherwise, we need to wrap the expression in a primitive to query
  // the locale ID of the expression
  // TODO: Review all clients of this routine and see whether they expect the whole
  // locale ID or just the node ID.  Split this routine into extractLocaleID()
  // and extractNodeID().
  // The current implementation expects localeID == nodeID, so we return
  // just the node portion of the localeID (so this is really extractNodeID()).
  return new CallExpr(PRIM_WIDE_GET_LOCALE, expr);
}


BlockStmt*
buildOnStmt(Expr* expr, Expr* stmt) {
  checkControlFlow(stmt, "on statement");

  CallExpr* onExpr = new CallExpr(PRIM_DEREF, extractLocaleID(expr));

  BlockStmt* body = toBlockStmt(stmt);

  //
  // detect begin statement directly inside on-statement
  //
  BlockStmt* beginBlock = findStmtWithTag(PRIM_BLOCK_BEGIN, body);

  // If the locale model doesn't require outlined on functions and this is a
  // --local compile, then we take the on-expression, execute it to evaluate
  // it for side effects, and then evaluate the body directly.
  if (!requireOutlinedOn()) {
    BlockStmt* block = new BlockStmt(stmt);
    block->insertAtHead(onExpr); // evaluate the expression for side effects
    return buildChapelStmt(block);
  }

  if (beginBlock) {
    // OPTIMIZATION: If "on x" is immediately followed by a "begin", then collapse
    // remote_fork (node) {
    //   branch /*local*/ { foo(); }
    // } wait;
    // to
    // remote_fork (node) { foo(); } // no wait();

    // Execute the construct "on x begin ..." asynchronously.
    Symbol* tmp = newTemp();
    body->insertAtHead(new CallExpr(PRIM_MOVE, tmp, onExpr));
    body->insertAtHead(new DefExpr(tmp));
    beginBlock->blockInfoSet(new CallExpr(PRIM_BLOCK_BEGIN_ON, tmp));
    // If there are beginBlock->byrefVars, they will be preserved.
    return body;
  } else {
    // Otherwise, wait for the "on" statement to complete before proceeding.
    BlockStmt* block = buildChapelStmt();
    Symbol* tmp = newTemp();
    block->insertAtTail(new DefExpr(tmp));
    block->insertAtTail(new CallExpr(PRIM_MOVE, tmp, onExpr));
    BlockStmt* onBlock = new BlockStmt(stmt);
    onBlock->blockInfoSet(new CallExpr(PRIM_BLOCK_ON, tmp));
    block->insertAtTail(onBlock);
    return block;
  }
}


BlockStmt*
buildBeginStmt(CallExpr* byref_vars, Expr* stmt) {
  checkControlFlow(stmt, "begin statement");

  BlockStmt* body = toBlockStmt(stmt);

  //
  // detect on-statement directly inside begin statement
  //
  BlockStmt* onBlock = findStmtWithTag(PRIM_BLOCK_ON, body);

  if (onBlock) {
    body->insertAtHead(new CallExpr("_upEndCount", gFalse));
    onBlock->insertAtTail(new CallExpr("_downEndCount"));
    onBlock->blockInfoGet()->primitive = primitives[PRIM_BLOCK_BEGIN_ON];
    addByrefVars(onBlock, byref_vars);
    return body;
  } else {
    BlockStmt* block = buildChapelStmt();
    block->insertAtTail(new CallExpr("_upEndCount"));
    BlockStmt* beginBlock = new BlockStmt();
    beginBlock->blockInfoSet(new CallExpr(PRIM_BLOCK_BEGIN));
    addByrefVars(beginBlock, byref_vars);
    beginBlock->insertAtHead(stmt);
    beginBlock->insertAtTail(new CallExpr("_downEndCount"));
    block->insertAtTail(beginBlock);
    return block;
  }
}


BlockStmt*
buildSyncStmt(Expr* stmt) {
  checkControlFlow(stmt, "sync statement");
  BlockStmt* block = new BlockStmt();
  VarSymbol* endCountSave = newTemp("_endCountSave");
  block->insertAtTail(new DefExpr(endCountSave));
  block->insertAtTail(new CallExpr(PRIM_MOVE, endCountSave, new CallExpr(PRIM_GET_END_COUNT)));
  block->insertAtTail(new CallExpr(PRIM_SET_END_COUNT, new CallExpr("_endCountAlloc")));
  block->insertAtTail(stmt);
  block->insertAtTail(new CallExpr("_waitEndCount"));
  block->insertAtTail(new CallExpr("_endCountFree", new CallExpr(PRIM_GET_END_COUNT)));
  block->insertAtTail(new CallExpr(PRIM_SET_END_COUNT, endCountSave));
  return block;
}


BlockStmt*
buildCobeginStmt(CallExpr* byref_vars, BlockStmt* block) {
  BlockStmt* outer = block;

  checkControlFlow(block, "cobegin statement");

  if (block->blockTag == BLOCK_SCOPELESS) {
    block = toBlockStmt(block->body.only());
    INT_ASSERT(block);
    block->remove();
  }

  if (block->length() < 2) {
    USR_WARN(outer, "cobegin has no effect if it contains fewer than 2 statements");
    // dropping byref_vars, if any
    return buildChapelStmt(block);
  }

  VarSymbol* cobeginCount = newTemp("_cobeginCount");
  cobeginCount->addFlag(FLAG_TEMP);

  for_alist(stmt, block->body) {
    BlockStmt* beginBlk = new BlockStmt();
    beginBlk->blockInfoSet(new CallExpr(PRIM_BLOCK_COBEGIN));
    beginBlk->astloc = stmt->astloc;
    // the original byref_vars is dead - will be clean_gvec-ed
    addByrefVars(beginBlk, byref_vars ? byref_vars->copy() : NULL);
    stmt->insertBefore(beginBlk);
    beginBlk->insertAtHead(stmt->remove());
    beginBlk->insertAtTail(new CallExpr("_downEndCount", cobeginCount));
    block->insertAtHead(new CallExpr("_upEndCount", cobeginCount));
  }

  block->insertAtHead(new CallExpr(PRIM_MOVE, cobeginCount, new CallExpr("_endCountAlloc")));
  block->insertAtHead(new DefExpr(cobeginCount));
  block->insertAtTail(new CallExpr(PRIM_PROCESS_TASK_LIST, cobeginCount));
  block->insertAtTail(new CallExpr("_waitEndCount", cobeginCount));
  block->insertAtTail(new CallExpr("_endCountFree", cobeginCount));

  return block;
}


BlockStmt*
buildGotoStmt(GotoTag tag, const char* name) {
  return buildChapelStmt(new GotoStmt(tag, name));
}

BlockStmt* buildPrimitiveStmt(PrimitiveTag tag, Expr* e1, Expr* e2) {
  return buildChapelStmt(new CallExpr(tag, e1, e2));
}

BlockStmt*
buildAtomicStmt(Expr* stmt) {
  static bool atomic_warning = false;

  if (!atomic_warning) {
    atomic_warning = true;
    USR_WARN(stmt, "atomic keyword is ignored (not implemented)");
  }
  return buildChapelStmt(new BlockStmt(stmt));
}


CallExpr* buildPreDecIncWarning(Expr* expr, char sign) {
  if (sign == '+') {
    USR_WARN(expr, "++ is not a pre-increment");
    return new CallExpr("+", new CallExpr("+", expr));
  } else if (sign == '-') {
    USR_WARN(expr, "-- is not a pre-decrement");
    return new CallExpr("-", new CallExpr("-", expr));
  } else {
    INT_FATAL(expr, "Error in parser");
  }
  return NULL;
}

BlockStmt* convertTypesToExtern(BlockStmt* blk) {
  for_alist(node, blk->body) {
    if (DefExpr* de = toDefExpr(node)) {
      if (!de->init) {
        Symbol* vs = de->sym;
        PrimitiveType* pt = new PrimitiveType(NULL);

        TypeSymbol* ts = new TypeSymbol(vs->name, pt);
        if (VarSymbol* theVs = toVarSymbol(vs)) {
          ts->doc = theVs->doc;
        }
        DefExpr* newde = new DefExpr(ts);

        de->replace(newde);
        de = newde;
      }
      de->sym->addFlag(FLAG_EXTERN);
    } else {
      INT_FATAL("Got non-DefExpr in type_alias_decl_stmt");
    }
  }
  return blk;
}

BlockStmt* handleConfigTypes(BlockStmt* blk) {
  for_alist(node, blk->body) {
    if (DefExpr* defExpr = toDefExpr(node)) {
      if (VarSymbol* var = toVarSymbol(defExpr->sym)) {
        var->addFlag(FLAG_CONFIG);
        if (Expr *configInit = getCmdLineConfig(var->name)) {
          // config var initialized on the command line
          if (!isUsedCmdLineConfig(var->name)) {
            useCmdLineConfig(var->name);
            // drop the original init expression on the floor
            if (Expr* a = toExpr(configInit))
              defExpr->init = a;
            else if (Symbol* a = toSymbol(configInit))
              defExpr->init = new SymExpr(a);
            else
              INT_FATAL(node, "Type alias initialized to invalid exprType");
          } else {
            // name is ambiguous, must specify module name
            USR_FATAL(var, "Ambiguous config param or type name (%s)", var->name);
          }
        }
      }
    } else {
      INT_FATAL("Got non-DefExpr in type_alias_decl_stmt");
    }
  }
  return blk;
}

// Attempt to find a stmt with a specific PrimitiveTag in a blockStmt
//
// For the case we're interested in we anticipate that the parser
// has constructed the following structure
//
//   BlockStmt
//     Scopeless BlockStmt
//       DefExpr  to define a tmp var
//       CallExpr to implement a move to the tmp var
//       BlockStmt with blockInfo
//
// So
//   1) We're trying to test the tail position of a blockStmt
//   2) We may need to step down one or more levels of blockStmt
//
// Finally BlockStmt is currently overloaded to represent a number of
// Stmts e.g. Loops, OnStmts, LocalStmts etc.
// We need to take care to discriminate among these

static BlockStmt* findStmtWithTag(PrimitiveTag tag, BlockStmt* blockStmt) {
  BlockStmt* retval = NULL;

  while (blockStmt != NULL && retval == NULL) {
    BlockStmt* tail = toBlockStmt(blockStmt->body.tail);

    // This is the stmt we're looking for
    if (tail != NULL && tail->isBlockType(tag)) {
      retval    = tail;

    // Stop if the current blockStmt is not of length 1
    } else if (blockStmt->length() != 1) {
      blockStmt = NULL;

    // Stop if the tail is not a "real" BlockStmt (e.g. a Loop etc)
    } else if (tail == NULL || tail->isRealBlockStmt() == false) {
      blockStmt = NULL;

    // Step in to the block and try again
    } else {
      blockStmt = tail;

    }
  }

  return retval;
}
