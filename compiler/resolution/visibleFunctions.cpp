/*
 * Copyright 2004-2017 Cray Inc.
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

#include "visibleFunctions.h"

#include "callInfo.h"
#include "expr.h"
#include "map.h"
#include "resolution.h"
#include "resolveIntents.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"

#include <map>
#include <set>

class VisibleFunctionBlock {
public:
                                        VisibleFunctionBlock();

  Map<const char*, Vec<FnSymbol*>*>     visibleFunctions;
};

// map: (block id) -> (map: sym -> sym)
typedef std::map<int, SymbolMap*> CapturedValueMap;

static CapturedValueMap                       capturedValues;

static Map<BlockStmt*, VisibleFunctionBlock*> visibleFunctionMap;
static Map<BlockStmt*, BlockStmt*>            visibilityBlockCache;

static int                                    nVisibleFunctions       = 0;

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

static void  buildVisibleFunctionMap();

static void  handleTaskIntentArgs(CallExpr* call,
                                  FnSymbol* taskFn,
                                  CallInfo& info);

static bool  isConstValWillNotChange(Symbol* sym);

static void  captureTaskIntentValues(int        argNum,
                                     ArgSymbol* formal,
                                     Expr*      actual,
                                     Symbol*    varActual,
                                     CallInfo&  info,
                                     CallExpr*  call,
                                     FnSymbol*  taskFn);

static void  verifyTaskFnCall(BlockStmt* parent, CallExpr* call);

static Expr* parentToMarker(BlockStmt* parent, CallExpr* call);

void findVisibleFunctions(CallInfo&       info,
                          Vec<FnSymbol*>& visibleFns) {
  CallExpr* call = info.call;

  //
  // update visible function map as necessary
  //
  if (gFnSymbols.n != nVisibleFunctions) {
    buildVisibleFunctionMap();
  }

  if (!call->isResolved()) {
    if (!info.scope) {
      getVisibleFunctions(info.name, call, visibleFns);
    } else {
      if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(info.scope)) {
        if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(info.name)) {
          visibleFns.append(*fns);
        }
      }
    }
  } else {
    visibleFns.add(call->resolvedFunction());
    handleTaskIntentArgs(call, call->resolvedFunction(), info);
  }

  if ((explainCallLine && explainCallMatch(call)) ||
      call->id == explainCallID) {
    USR_PRINT(call, "call: %s", toString(&info));

    if (visibleFns.n == 0)
      USR_PRINT(call, "no visible functions found");

    bool first = true;

    forv_Vec(FnSymbol, visibleFn, visibleFns) {
      USR_PRINT(visibleFn, "%s %s",
                first ? "visible functions are:" : "                      ",
                toString(visibleFn));
      first = false;
    }
  }
}



static void buildVisibleFunctionMap() {
  for (int i = nVisibleFunctions; i < gFnSymbols.n; i++) {
    FnSymbol* fn = gFnSymbols.v[i];
    if (!fn->hasFlag(FLAG_INVISIBLE_FN) && fn->defPoint->parentSymbol && !isArgSymbol(fn->defPoint->parentSymbol)) {
      BlockStmt* block = NULL;
      if (fn->hasFlag(FLAG_AUTO_II)) {
        block = theProgram->block;
      } else {
        block = getVisibilityBlock(fn->defPoint);
        //
        // add all functions in standard modules to theProgram
        //
        // Lydia NOTE 09/12/16: The computation of the standardModuleSet is not
        // tied to what is actually placed within theProgram->block.  As such
        // there could be bugs where that implementation differs.  We have
        // already encountered some with qualified access to default-included
        // modules like List and Sort.  This implementation needs to be linked
        // to the computation of the standardModuleSet.
        //
        if (standardModuleSet.set_in(block))
          block = theProgram->block;
      }
      VisibleFunctionBlock* vfb = visibleFunctionMap.get(block);
      if (!vfb) {
        vfb = new VisibleFunctionBlock();
        visibleFunctionMap.put(block, vfb);
      }
      Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(fn->name);
      if (!fns) {
        fns = new Vec<FnSymbol*>();
        vfb->visibleFunctions.put(fn->name, fns);
      }
      fns->add(fn);
    }
  }
  nVisibleFunctions = gFnSymbols.n;
}

//
// Copy the type of the actual into the type of the corresponding formal
// of a task function. (I think resolution wouldn't make this happen
// automatically and correctly in all cases.)
// Also do captureTaskIntentValues() when needed.
//
static void handleTaskIntentArgs(CallExpr* call, FnSymbol* taskFn,
                                 CallInfo& info)
{
  INT_ASSERT(taskFn);
  if (!needsCapture(taskFn)) {
    // A task function should have args only if it needsCapture.
    if (taskFn->hasFlag(FLAG_ON)) {
      // Documenting the current state: fn_on gets a chpl_localeID_t arg.
      INT_ASSERT(call->numActuals() == 1);
    } else {
      INT_ASSERT(!isTaskFun(taskFn) || call->numActuals() == 0);
    }
    return;
  }

  int argNum = -1;
  for_formals_actuals(formal, actual, call) {
    argNum++;
    SymExpr* symexpActual = toSymExpr(actual);
    if (!symexpActual) {
      // We add NamedExpr args in propagateExtraLeaderArgs().
      NamedExpr* namedexpActual = toNamedExpr(actual);
      INT_ASSERT(namedexpActual);
      symexpActual = toSymExpr(namedexpActual->actual);
    }
    INT_ASSERT(symexpActual); // because of how we invoke a task function
    Symbol* varActual = symexpActual->symbol();

    // If 'call' is in a generic function, it is supposed to have been
    // instantiated by now. Otherwise our task function has to remain generic.
    INT_ASSERT(!varActual->type->symbol->hasFlag(FLAG_GENERIC));

    // Need to copy varActual->type even for type variables.
    // BTW some formals' types may have been set in createTaskFunctions().
    formal->type = varActual->type;

    // If the actual is a ref, still need to capture it => remove ref.
    if (isReferenceType(varActual->type)) {
      Type* deref = varActual->type->getValType();
      // todo: replace needsCapture() with always resolveArgIntent(formal)
      // then checking (formal->intent & INTENT_FLAG_IN)
      if (needsCapture(deref)) {
        formal->type = deref;
        // If the formal has a ref intent, DO need a ref type => restore it.
        resolveArgIntent(formal);
        if (formal->intent & INTENT_FLAG_REF) {
          formal->type = varActual->type;
        }
        if (varActual->isConstant()) {
          int newIntent = formal->intent | INTENT_FLAG_CONST;
          // and clear INTENT_FLAG_MAYBE_CONST flag
          newIntent &= ~ INTENT_FLAG_MAYBE_CONST;
          formal->intent = (IntentTag)(newIntent);
        }
      }
    }

    if (varActual->hasFlag(FLAG_TYPE_VARIABLE))
      formal->addFlag(FLAG_TYPE_VARIABLE);

    // This does not capture records/strings that are passed
    // by blank or const intent. As of this writing (6'2015)
    // records and strings are (incorrectly) captured at the point
    // when the task function/arg bundle is created.
    if (taskFn->hasFlag(FLAG_COBEGIN_OR_COFORALL) &&
        !isConstValWillNotChange(varActual) &&
        (concreteIntent(formal->intent, formal->type->getValType())
         & INTENT_FLAG_IN))
      // skip dummy_locale_arg: chpl_localeID_t
      if (argNum != 0 || !taskFn->hasFlag(FLAG_ON))
        captureTaskIntentValues(argNum, formal, actual, varActual, info, call,
                                taskFn);
  }

  // Even if some formals are (now) types, if 'taskFn' remained generic,
  // gatherCandidates() would not instantiate it, for some reason.
  taskFn->removeFlag(FLAG_GENERIC);
}

//
// Allow invoking isConstValWillNotChange() even on formals
// with blank and 'const' intents.
//
static bool isConstValWillNotChange(Symbol* sym) {
  if (ArgSymbol* arg = toArgSymbol(sym)) {
    IntentTag cInt = concreteIntent(arg->intent, arg->type->getValType());
    return cInt == INTENT_CONST_IN;
  }
  return sym->isConstValWillNotChange();
}

//
// Generate code to store away the value of 'varActual' before
// the cobegin or the coforall loop starts. Use this value
// instead of 'varActual' as the actual to the task function,
// meaning (later in compilation) in the argument bundle.
//
// This is to ensure that all task functions use the same value
// for their respective formal when that has an 'in'-like intent,
// even if 'varActual' is modified between creations of
// the multiple task functions.
//
static void captureTaskIntentValues(int        argNum,
                                    ArgSymbol* formal,
                                    Expr*      actual,
                                    Symbol*    varActual,
                                    CallInfo&  info,
                                    CallExpr*  call,
                                    FnSymbol*  taskFn) {
  BlockStmt* parent = toBlockStmt(call->parentExpr);
  INT_ASSERT(parent);
  if (taskFn->hasFlag(FLAG_ON) && !parent->isForLoop()) {
    // coforall ... { on ... { .... }} ==> there is an intermediate BlockStmt
    parent = toBlockStmt(parent->parentExpr);
    INT_ASSERT(parent);
  }
  if (fVerify && (argNum == 0 || (argNum == 1 && taskFn->hasFlag(FLAG_ON))))
    verifyTaskFnCall(parent, call); //assertions only
  Expr* marker = parentToMarker(parent, call);
  if (varActual->hasFlag(FLAG_NO_CAPTURE_FOR_TASKING)) {
      // No need to worry about this.
      return;
  }
  if (varActual->defPoint->parentExpr == parent) {
    // Index variable of the coforall loop? Do not capture it!
    INT_ASSERT(varActual->hasFlag(FLAG_COFORALL_INDEX_VAR));
    return;
  }
  SymbolMap*& symap = capturedValues[parent->id];
  Symbol* captemp = NULL;
  if (symap)
    captemp = symap->get(varActual);
  else
    symap = new SymbolMap();
  if (!captemp) {
    captemp = newTemp(astr(formal->name, "_captemp"), formal->type);
    marker->insertBefore(new DefExpr(captemp));
    // todo: once AMM is in effect, drop chpl__autoCopy - do straight move
    if (hasAutoCopyForType(formal->type)) {
      FnSymbol* autoCopy = getAutoCopy(formal->type);
      marker->insertBefore("'move'(%S,%S(%S))", captemp, autoCopy, varActual);
    } else if (isReferenceType(varActual->type) &&
             !isReferenceType(captemp->type))
      marker->insertBefore("'move'(%S,'deref'(%S))", captemp, varActual);
    else
      marker->insertBefore("'move'(%S,%S)", captemp, varActual);
    symap->put(varActual, captemp);
  }
  actual->replace(new SymExpr(captemp));
  Symbol*& iact = info.actuals.v[argNum];
  INT_ASSERT(iact == varActual);
  iact = captemp;
}


// Ensure 'parent' is the block before which we want to do the capturing.
static void verifyTaskFnCall(BlockStmt* parent, CallExpr* call) {
  if (call->isNamed("coforall_fn") || call->isNamed("on_fn")) {
    INT_ASSERT(parent->isForLoop());
  } else if (call->isNamed("cobegin_fn")) {
    DefExpr* first = toDefExpr(parent->getFirstExpr());
    // just documenting the current state
    INT_ASSERT(first && !strcmp(first->sym->name, "_cobeginCount"));
  } else {
    INT_ASSERT(call->isNamed("begin_fn"));
  }
}

//
// Returns the expression that we want to capture before.
//
// Why not just 'parent'? In users/shetag/fock/fock-dyn-prog-cntr.chpl,
// we cannot do parent->insertBefore() because parent->list is null.
// That's because we have: if ... then cobegin ..., so 'parent' is
// immediately under CondStmt. This motivated me for cobegins to capture
// inside of the 'parent' block, at the beginning of it.
//
static Expr* parentToMarker(BlockStmt* parent, CallExpr* call) {
  if (call->isNamed("cobegin_fn")) {
    // I want to be cute and keep _cobeginCount def and move
    // as the first two statements in the block.
    DefExpr* def = toDefExpr(parent->body.head);
    INT_ASSERT(def);
    // Just so we know what we are doing.
    INT_ASSERT(!strcmp((def->sym->name), "_cobeginCount"));
    CallExpr* move = toCallExpr(def->next);
    INT_ASSERT(move);
    SymExpr* arg1 = toSymExpr(move->get(1));
    INT_ASSERT(arg1->symbol() == def->sym);
    // And this is where we want to insert:
    return move->next;
  }
  // Otherwise insert before 'parent'
  return parent;
}

/************************************* | **************************************
*                                                                             *
* Collects functions called 'name' visible in 'block' and up the visibility   *
* chain, which is defined by getVisibilityBlock().                            *
* The functions defined/visible in a block are given by 'visibleFunctionMap'. *
*                                                                             *
* 'visibilityBlockCache' maps a block to the nearest block up the visibility  *
* chain that contains any functions, according to visibleFunctionMap.         *
* In other words, it skips those blocks that do not define any visible        *
* functions (of any name).                                                    *
* Todo: some blocks only define if-functions (i.e. _if_fnNNN); such blocks    *
* probably should not be present in visibleFunctionMap.                       *
*                                                                             *
* getVisibleFunctions returns the block appropriate for visibilityBlockCache  *
* or NULL if there is none, e.g. when the next block up is the rootModule.    *
*                                                                             *
************************************** | *************************************/

static BlockStmt* getVisibleFunctions(const char*           name,
                                      CallExpr*             call,
                                      BlockStmt*            block,
                                      std::set<BlockStmt*>& visited,
                                      Vec<FnSymbol*>&       visibleFns);

void getVisibleFunctions(const char*      name,
                         CallExpr*        call,
                         Vec<FnSymbol*>&  visibleFns) {
  BlockStmt*           block    = getVisibilityBlock(call);
  std::set<BlockStmt*> visited;

  getVisibleFunctions(name, call, block, visited, visibleFns);
}

static BlockStmt* getVisibleFunctions(const char*           name,
                                      CallExpr*             call,
                                      BlockStmt*            block,
                                      std::set<BlockStmt*>& visited,
                                      Vec<FnSymbol*>&       visibleFns) {
  BlockStmt* retval = NULL;

  //
  // all functions in standard modules are stored in a single block
  //
  // Lydia NOTE 09/12/16: The computation of the standardModuleSet is not
  // tied to what is actually placed within theProgram->block.  As such
  // there could be bugs where that implementation differs.  We have
  // already encountered some with qualified access to default-included
  // modules like List and Sort.  This implementation needs to be linked
  // to the computation of the standardModuleSet.
  //
  if (standardModuleSet.set_in(block)) {
    block = theProgram->block;
  }

  //
  // avoid infinite recursion due to modules with mutual uses
  //
  if (visited.find(block) == visited.end()) {
    bool canSkipThisBlock = true;

    if (isModuleSymbol(block->parentSymbol)) {
      visited.insert(block);
    }

    if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(block)) {
      canSkipThisBlock = false; // cannot skip if this block defines functions

      if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name)) {
        forv_Vec(FnSymbol, fn, *fns) {
          if (fn->isVisible(call) == true) {
            // isVisible checks if the function is private to its defining
            // module (and in that case, if we are under its defining module)
            // This ensures that private functions will not be used outside
            // of their proper scope.
            visibleFns.add(fn);
          }
        }
      }
    }

    if (block->modUses != NULL) {
      for_actuals(expr, block->modUses) {
        UseStmt* use = toUseStmt(expr);

        INT_ASSERT(use);

        if (use->skipSymbolSearch(name) == false) {
          SymExpr* se = toSymExpr(use->src);

          INT_ASSERT(se);

          if (ModuleSymbol* mod = toModuleSymbol(se->symbol())) {
            // The use statement could be of an enum instead of a module,
            // but only modules can define functions.
            // cannot skip if this block uses modules
            canSkipThisBlock = false;

            if (mod->isVisible(call) == true) {
              if (use->isARename(name) == true) {
                getVisibleFunctions(use->getRename(name),
                                    call,
                                    mod->block,
                                    visited,
                                    visibleFns);
              } else {
                getVisibleFunctions(name,
                                    call,
                                    mod->block,
                                    visited,
                                    visibleFns);
              }
            }
          }
        }
      }
    }

    //
    // visibilityBlockCache contains blocks that can be skipped
    //
    if (BlockStmt* next = visibilityBlockCache.get(block)) {
      getVisibleFunctions(name, call, next, visited, visibleFns);

      retval = (canSkipThisBlock) ? next : block;

    } else if (block != rootModule->block) {
      BlockStmt* next  = getVisibilityBlock(block);
      BlockStmt* cache = getVisibleFunctions(name,
                                             call,
                                             next,
                                             visited,
                                             visibleFns);

      if (cache) {
        visibilityBlockCache.put(block, cache);
      }

      retval = (canSkipThisBlock) ? cache : block;
    }
  }

  return retval;
}

/************************************* | **************************************
*                                                                             *
* return the innermost block for searching for visible functions              *
*                                                                             *
************************************** | *************************************/

static bool isTryTokenCond(Expr* expr);

BlockStmt* getVisibilityBlock(Expr* expr) {
  if (BlockStmt* block = toBlockStmt(expr->parentExpr)) {
    if (block->blockTag == BLOCK_SCOPELESS)
      return getVisibilityBlock(block);
    else if (block->parentExpr && isTryTokenCond(block->parentExpr)) {
      // Make the visibility block of the then and else blocks of a
      // conditional using chpl__tryToken be the block containing the
      // conditional statement.  Without this, there were some cases where
      // a function gets instantiated into one side of the conditional but
      // used in both sides, then the side with the instantiation gets
      // folded out leaving expressions with no visibility block.
      // test/functions/iterators/angeles/dynamic.chpl is an example that
      // currently fails without this.
      return getVisibilityBlock(block->parentExpr);
    } else
      return block;
  } else if (expr->parentExpr) {
    return getVisibilityBlock(expr->parentExpr);
  } else if (Symbol* s = expr->parentSymbol) {
      FnSymbol* fn = toFnSymbol(s);
      if (fn && fn->instantiationPoint)
        return fn->instantiationPoint;
      else
        return getVisibilityBlock(s->defPoint);
  } else {
    INT_FATAL(expr, "Expression has no visibility block.");
    return NULL;
  }
}

//
// return true if expr is a CondStmt with chpl__tryToken as its condition
//
static bool isTryTokenCond(Expr* expr) {
  CondStmt* cond = toCondStmt(expr);

  if (!cond) return false;

  SymExpr* sym = toSymExpr(cond->condExpr);

  if (!sym) return false;

  return sym->symbol() == gTryToken;
}


/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

void visibleFunctionsClear() {
  Vec<VisibleFunctionBlock*> vfbs;

  visibleFunctionMap.get_values(vfbs);

  forv_Vec(VisibleFunctionBlock, vfb, vfbs) {
    Vec<Vec<FnSymbol*>*> vfns;

    vfb->visibleFunctions.get_values(vfns);

    forv_Vec(Vec<FnSymbol*>, vfn, vfns) {
      delete vfn;
    }

    delete vfb;
  }

  visibleFunctionMap.clear();

  visibilityBlockCache.clear();

  for (std::map<int, SymbolMap*>::iterator it = capturedValues.begin();
       it != capturedValues.end();
       ++it) {
    delete it->second;
  }
}

/************************************* | **************************************
*                                                                             *
*                                                                             *
*                                                                             *
************************************** | *************************************/

VisibleFunctionBlock::VisibleFunctionBlock() {

}
