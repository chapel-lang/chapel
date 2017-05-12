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
#include "stlUtil.h"
#include "stringutil.h"
#include "symbol.h"

#include <map>
#include <set>
#include <vector>

/*
   The process of finding visible functions works with some global
   tables. The global tables map

     block id -> function name -> FnSymbol*s

   In this way, the symbols declared in each block are stored.
   These tables do not save the transitive symbols available.
   To do that, code using the table needs to go up blocks/scopes
   and explicitly consider module 'use's.

 */

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

struct UseDistance {
  int distance;
  bool onlyOrExcept;
  UseDistance()
    : distance(-1), onlyOrExcept(true)
  { }

  UseDistance(int distance, bool onlyOrExcept)
    : distance(distance), onlyOrExcept(onlyOrExcept)
  { }
};

// moduleUsesTable[(m1->id,m2->id)] = i
//   where m1 and m2 are modules.
//
// i is -1 if m1 does not use m2
// i is 0 if m1 == m2
// i is 1 if m1 uses m2 directly
// i is 2 if m1 uses another module B that uses m2
// ...
// in other words, i is the distance between the modules.
static std::map<std::pair<int,int>, UseDistance>     moduleUsesTable;

// This one stores the transitive closure.
// i.e. it includes symbols from 'use'd functions
// does *not* include functions available directly in that module!
// that part comes from visibleFunctionMap.
typedef std::map<ModuleSymbol*, VisibleFunctionBlock*> ModuleVisibleFunctionMapType;
static ModuleVisibleFunctionMapType moduleVisibleFunctionMap;

// TODO - would it help to store a map from
// block to the set of modules "in use" at that block?

// This one stores the modules already included in the transitive
// closure above
typedef std::map<ModuleSymbol*, std::set<ModuleSymbol*> > ModuleVisibleModuleMapType;
static ModuleVisibleModuleMapType moduleVisibleModuleMap;


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

void findVisibleFunctions(CallInfo&           info,
                          Vec<FnSymbol*>&     visibleFns,
                          Vec<int>&           visibilityDistances) {
  CallExpr* call = info.call;

  //
  // update visible function map as necessary
  //
  if (gFnSymbols.n != nVisibleFunctions) {
    buildVisibleFunctionMap();
  }

  if (!call->isResolved()) {
    if (!info.scope) {
      getVisibleFunctions(info.name, call, visibleFns, visibilityDistances);
    } else {
      if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(info.scope)) {
        if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(info.name)) {
          visibleFns.append(*fns);
          forv_Vec(FnSymbol, fn, *fns) {
            // scope 0 is acceptable because MyMod.fn() is already
            // explicitly scoped and so should never compete with candidates
            // with different visibility
            visibilityDistances.push_back(0);
          }
        }
      }
    }
  } else {
    visibleFns.push_back(call->resolvedFunction());
    // scope 0 is acceptable because the call is already resolved
    // and so should never compete with other candidates
    visibilityDistances.push_back(0);
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

// Returns UseDistance between module m1 and module m2.
// E.g. is m2 available in m1? If so, through how many 'use'
// statements in different modules?
static UseDistance computeUseDistance(ModuleSymbol* m1, ModuleSymbol* m2)
{

  std::pair<int,int> key(m1->id, m2->id);

  // this is a base case for recursion
  if (m1 == m2)
    return UseDistance(0,false);

  // Distance from anything to theProgram / rootModule is 0
  // because these symbols are available in all modules.
  //if (//(m1 == theProgram || m1 == rootModule) ||
  //    (m2 == theProgram || m2 == rootModule))
  //  return UseDistance(0,false);

  if (moduleUsesTable.count(key) == 0) {


    UseDistance dist(-1, true);

    // Put something in the table to avoid an infinite loop
    // for modules that use each other.
    moduleUsesTable[key] = dist;

    BlockStmt* block = m1->block;
    if (block->modUses != NULL) {
      for_actuals(expr, block->modUses) {
        UseStmt* use = toUseStmt(expr);
        INT_ASSERT(use);
        SymExpr* se = toSymExpr(use->src);
        INT_ASSERT(se);
        bool onlyOrExcept = !use->isPlainUse();
// TODO
// use A only ;
// does not count as a use for this purpose.
//
// neither does using a private module.?
        // only consider uses of modules (not uses of enums)
        if (ModuleSymbol* usedMod = toModuleSymbol(se->symbol())) {
          // Does 'usedMod' use 'm2', transitively?
          UseDistance gotDist = computeUseDistance(usedMod, m2);
          // record only/except as a different "use level"
          if (onlyOrExcept)
            gotDist.onlyOrExcept = true;
          if (gotDist.distance != -1) {
            // Account for it being a 'use'
            gotDist.distance += 1;
	    // merge gotDist want global dist
	    // onlyOrExcept is set only if both are that way.
	    dist.onlyOrExcept = dist.onlyOrExcept && gotDist.onlyOrExcept;
	    // distance is the minimum use distance
	    if (dist.distance == -1 ||
		dist.distance > gotDist.distance)
              dist.distance = gotDist.distance;
          }
        }
      }
    }

    //printf("computeUseDistance(%s, %s) = %i\n",
    //       m1->name, m2->name, dist.distance);

    moduleUsesTable[key] = dist;
  }

  return moduleUsesTable[key];
}

// returns true for rootModule, theProgram,
// and any module 'use'd by those.
static bool isModuleGloballyVisible(ModuleSymbol* mod) {
  // theProgram and rootModule are parents of all other modules,
  // so they are globally visible.
  if (mod == theProgram || mod == rootModule)
    return true;
  // theProgram can use other modules; in that event, symbols
  // from those modules are globally visible.
  UseDistance d = computeUseDistance(theProgram, mod);
  if (d.distance != -1)
    return true;

  return false;
}

static UseDistance computeVisibilityDistance(ModuleSymbol* m1, ModuleSymbol* m2)
{
  UseDistance d;
  // Symbols visible from theProgram or rootModule
  // should be added to all other modules.
  if (isModuleGloballyVisible(m2))
    d = UseDistance(0, false);
  else
    d = computeUseDistance(m1, m2);
  return d;
}

static void buildVisibleFunctionMap() {
  typedef std::map<ModuleSymbol*,std::vector<FnSymbol*> > ModToNewFnsMap;
  ModToNewFnsMap updates;

  //printf("buildVisibleFunctionMap %i\n", gFnSymbols.n);
  for (int i = nVisibleFunctions; i < gFnSymbols.n; i++) {
    FnSymbol* fn = gFnSymbols.v[i];
    if (!fn->hasFlag(FLAG_INVISIBLE_FN) && fn->defPoint->parentSymbol && !isArgSymbol(fn->defPoint->parentSymbol)) {
      BlockStmt* block = NULL;
      if (fn->hasFlag(FLAG_AUTO_II)) {
        block = theProgram->block;
      } else {
        block = getVisibilityBlock(fn->defPoint);

/*
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
*/
        // Treat theProgram the same as the root module
        //if (theProgram->block == block)
        //  block = rootModule->block;
      }
      ModuleSymbol* mod = toModuleSymbol(block->parentSymbol);
      if (mod) {
        updates[mod].push_back(fn);
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
      //printf("Adding fn %s (%i) to block %i mod %s\n",
      //       fn->name, fn->id, block->id, mod?mod->name:"");
      fns->add(fn);
    }
  }

  //printf("computing transitives\n");

  for (ModToNewFnsMap::iterator it = updates.begin();
       it != updates.end();
       ++it) {
    ModuleSymbol* mod = it->first;
    std::vector<FnSymbol*> &vec = it->second;

    // Any module that could use mod transitively might need to be updated
    // TODO - do this in a smarter manner... possible transitive map
    // from module to modules that use it? And handle theProgram/rootModule
    // in a separate loop?
    forv_Vec(ModuleSymbol, otherMod, gModuleSymbols) {


      // Don't bother updating theProgram or rootModule
      if (/*otherMod == theProgram || */otherMod == rootModule)
        continue;

      UseDistance d = computeVisibilityDistance(otherMod, mod);

      //printf("distance from %s to %s is %i %i\n",
      //       otherMod->name, mod->name, d.distance, d.onlyOrExcept);

      // TODO -- should handle onlyOrExcept, not rule it out.
      if (mod != otherMod && d.distance != -1 && d.onlyOrExcept == false) {

        // Update moduleVisibleModuleMap.
        moduleVisibleModuleMap[otherMod].insert(mod);

        // otherMod uses mod with some use distance d.distance.
        // Update transitive closure of visible function maps.
	for_vector(FnSymbol, fn, vec) {
          if (moduleVisibleFunctionMap.count(otherMod) == 0) {
            // Create an entry, set use distance
            VisibleFunctionBlock* vfb = new VisibleFunctionBlock();
            moduleVisibleFunctionMap.insert(std::make_pair(otherMod, vfb));
          }
          VisibleFunctionBlock* vfb = moduleVisibleFunctionMap[otherMod];
          Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(fn->name);
          if (!fns) {
            fns = new Vec<FnSymbol*>();
            vfb->visibleFunctions.put(fn->name, fns);
          }
          //printf("transitive adding fn %s (%i) to transitive module %s\n",
          //       fn->name, fn->id, otherMod->name);
          fns->add(fn);
        }
      }
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
                                      std::set<ModuleSymbol*>& visited,
                                      Vec<FnSymbol*>&       visibleFns,
                                      Vec<int>&             visibilityDistances,
                                      int                   distance);
static void getVisibleFnsFromUsedModule(const char*      name,
                                        ModuleSymbol*    mod,
                                        std::set<ModuleSymbol*>& visited,
                                        Vec<FnSymbol*>&  visibleFns,
                                        Vec<int>&        visibilityDistances,
                                        int              distance);
 
void getVisibleFunctions(const char*            name,
                         CallExpr*              call,
                         Vec<FnSymbol*>&       visibleFns,
                         Vec<int>&             visibilityDistances) {
  BlockStmt*           block    = getVisibilityBlock(call);
  std::set<ModuleSymbol*> visited;

  getVisibleFunctions(name, call, block, visited,
                      visibleFns, visibilityDistances, 0);
}

/* Gather any definitions from 'use' statements in module mod */
static void getVisibleFnsFromUsesInModule(const char*      name,
                                          ModuleSymbol*    mod,
                                          std::set<ModuleSymbol*>& visited,
                                          Vec<FnSymbol*>&  visibleFns,
                                          Vec<int>&        visibilityDistances,
                                          int              distance) {

  // root module cannot use other modules currently.
  if (mod == rootModule)
    return;

  // Gather any definitions available from 'use' of other modules
  ModuleVisibleFunctionMapType::iterator it;
  it = moduleVisibleFunctionMap.find(mod);
  if (it == moduleVisibleFunctionMap.end())
    return; // module might have defined no symbols, used no modules

  if (0 == strcmp("foo", name) && 0 == strcmp("TestTwo", mod->name))
    gdbShouldBreakHere();

  //printf("getVisibleFnsFromUsesInModule (have %i) %s mod %s\n",
  //       visibleFns.n, name, mod->name);


  VisibleFunctionBlock* vfb = it->second;

  bool anyAlreadyVisited = false;

  std::set<ModuleSymbol*>& usedModulesSet = moduleVisibleModuleMap[mod];
  for_set(ModuleSymbol, usedMod, usedModulesSet) {
    if (visited.find(usedMod) != visited.end()) {
      //printf("module %s already visited!\n", usedMod->name);
      anyAlreadyVisited = true;
      break;
    }
    //printf("module %s not visited yet\n", usedMod->name);
  }

  if (anyAlreadyVisited) {
    // If any were already visited, add symbols from each non-visited
    // module directly.
    for_set(ModuleSymbol, usedMod, usedModulesSet) {
      if (visited.find(usedMod) == visited.end()) {
        UseDistance d = computeVisibilityDistance(mod, usedMod);
        INT_ASSERT(d.distance>=0);

        getVisibleFnsFromUsedModule(name, usedMod, visited,
                                    visibleFns, visibilityDistances,
                                    distance+d.distance);
      }
    }
  } else {
    // Otherwise, add all symbols from the module
    if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name)) {
      forv_Vec(FnSymbol, fn, *fns) {
        
        ModuleSymbol* fnModule = fn->getModule();
        UseDistance d = computeVisibilityDistance(mod, fnModule);
        INT_ASSERT(d.distance>=0);

        visibleFns.push_back(fn);
        visibilityDistances.push_back(distance + d.distance);
      }
    }
    for_set(ModuleSymbol, usedMod, usedModulesSet) {
      visited.insert(usedMod);
    }
  }
  
  //printf("getVisibleFnsFromUsesInModule finishing, now have %i\n",
  //       visibleFns.n);
}


/* Gather any definitions from module 'mod' that are available to a
   'use' of that module.
   
   Does not included transitively available definitions
   (that is covered in getVisibleFnsFromUsesInModule)
 */
static void getVisibleFnsFromUsedModule(const char*      name,
                                        ModuleSymbol*    mod,
                                        std::set<ModuleSymbol*>& visited,
                                        Vec<FnSymbol*>&  visibleFns,
                                        Vec<int>&        visibilityDistances,
                                        int              distance) {
  BlockStmt* block = mod->block;
  
  //
  // visit modules only once
  //
  if (visited.find(mod) != visited.end())
    return;
  visited.insert(mod);

  //printf("getVisibleFnsFromUsedModule (have %i) %s mod %s\n",
  //       visibleFns.n, name, mod->name);

  // Gather any non-private definitions declared directly in the module
  if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(block)) {
    if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name)) {
      forv_Vec(FnSymbol, fn, *fns) {
        visibleFns.push_back(fn);
        visibilityDistances.push_back(distance);
      }
    }
  }
}


static BlockStmt* getVisibleFunctions(const char*           name,
                                      CallExpr*             call,
                                      BlockStmt*            block,
                                      std::set<ModuleSymbol*>& visited,
                                      Vec<FnSymbol*>&       visibleFns,
                                      Vec<int>&             visibilityDistances,
                                      int                   distance) {
  BlockStmt* retval = NULL;


/*
  // all functions in standard modules are stored in a single block
  //
  // Lydia NOTE 09/12/16: The computation of the standardModuleSet is not
  // tied to what is actually placed within theProgram->block.  As such
  // there could be bugs where that implementation differs.  We have
  // already encountered some with qualified access to default-included
  // modules like List and Sort.  This implementation needs to be linked
  // to the computation of the standardModuleSet.
  //
// TODO
  if (standardModuleSet.set_in(block)) {
    block = theProgram->block;
  }
*/
  // Treat theProgram the same as the root module
  // This enables most symbols to resolve through
  // the transitive module uses computed above.
  //if (theProgram->block == block)
  //  block = rootModule->block;


 
  ModuleSymbol* mod = toModuleSymbol(block->parentSymbol);

  // But it has to be *the* block of the module,
  // not a nested block.
  if (mod && mod->block != block)
    mod = NULL;

  //
  // visit modules only once
  //
  if (mod != NULL) {
    if (visited.find(mod) != visited.end())
      return NULL;
    visited.insert(mod);
  }

  //ModuleSymbol* inModule = call->getModule();
  //printf("getVisibleFunctions (have %i) %s for call id %i mod %s (in mod %s)\n",
  //       visibleFns.n, name, call->id, mod?mod->name:"", inModule->name);
  //if (call->id == 691)
  //  gdbShouldBreakHere();

  bool canSkipThisBlock = true;

  // at this point block may or may not be module scope
  // but here we just look for local definitions
  // this allows us to find private definitions.
  if (VisibleFunctionBlock* vfb = visibleFunctionMap.get(block)) {
    canSkipThisBlock = false; // cannot skip if this block defines functions

    if (Vec<FnSymbol*>* fns = vfb->visibleFunctions.get(name)) {
      forv_Vec(FnSymbol, fn, *fns) {
        visibleFns.push_back(fn);
        visibilityDistances.push_back(distance);
      }
    }
  }

  // Have we reached module scope?
  // if so, use the pre-computed module-level table
  // to check for symbols from 'use'd modules.

  if (mod != NULL) {
    // above code would have already handled declarations
    // directly in this module.
    getVisibleFnsFromUsesInModule(name, mod, visited,
                                  visibleFns, visibilityDistances,
                                  distance);
    // Can't return yet
    //   - need to find symbols from fancy 'use'
    //   - need to find symbols from parent modules in case of nested modules
  }

  if (block->modUses != NULL) {
    for_actuals(expr, block->modUses) {
      UseStmt* use = toUseStmt(expr);

      INT_ASSERT(use);

      if (mod == NULL || !use->isPlainUse()) {
        // simple module-level uses are handled above

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
                getVisibleFnsFromUsedModule(use->getRename(name),
                                            mod, visited,
                                            visibleFns,
                                            visibilityDistances,
                                            distance+1);
                getVisibleFnsFromUsesInModule(use->getRename(name),
                                              mod, visited,
                                              visibleFns,
                                              visibilityDistances,
                                              distance+1);
              } else {
                getVisibleFnsFromUsedModule(name,
                                            mod, visited,
                                            visibleFns,
                                            visibilityDistances,
                                            distance+1);
                getVisibleFnsFromUsesInModule(name, mod, visited,
                                              visibleFns,
                                              visibilityDistances,
                                              distance+1);
              }
            }
          }
        }
      }
    }
  }

  //printf("getVisibleFunctions processing parents (have %i) %s for call id %i mod %s\n",
  //       visibleFns.n, name, call->id, mod?mod->name:"");
 
  //
  // visibilityBlockCache may contain the next block
  //
  if (BlockStmt* cachedNext = visibilityBlockCache.get(block)) {
    //printf("cached\n");
    getVisibleFunctions(name, call, cachedNext, visited,
                        visibleFns, visibilityDistances, distance+1);

    retval = (canSkipThisBlock) ? cachedNext : block;

  } else if (block != rootModule->block) {
    BlockStmt* next  = getVisibilityBlock(block);
    bool isModuleContainedInTheProgram = false;

    if (mod != NULL && next == theProgram->block)
      isModuleContainedInTheProgram = true;

    // Continue if:
    //   we are processing a call in theProgram somewhere
    //   or, parent visibility block is not rootModule / theProgram
    //
    // Symbols from theProgram/rootModule are generally covered by
    // getVisibleFnsFromUsesInModule above.
    if (isModuleContainedInTheProgram == false) {
      BlockStmt* cache = getVisibleFunctions(name,
                                             call,
                                             next,
                                             visited,
                                             visibleFns,
                                             visibilityDistances,
                                             distance+1);

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
      if (fn && fn->instantiationPoint && fn->instantiationPoint->parentSymbol)
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
