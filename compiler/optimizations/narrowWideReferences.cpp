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

#include "astutil.h"
#include "stringutil.h"
#include "expr.h"
#include "optimizations.h"
#include "passes.h"
#include "stmt.h"
#include "view.h"
#include <set>
#include <map>
#include "stlUtil.h"

//
// For debugging, uncomment the following macros for insights:
//
//   PRINT_NARROW_EFFECT_SUMMARY counts the number of references that
//   have been narrowed, and the number of references that remained
//   wide.
//
//   PRINT_NARROW_EFFECT marks the points at which references are narrowed
//   (basically to debug the process).
//
//   PRINT_NARROW_ANALYSIS provides debugging information about why a
//   variable was required to be wide.
//

//#define PRINT_NARROW_EFFECT_SUMMARY
//#define PRINT_NARROW_EFFECT
//#define PRINT_NARROW_ANALYSIS

#ifdef PRINT_NARROW_ANALYSIS
  #define DEBUG_PRINTF(...) printf(__VA_ARGS__)
#else
  #define DEBUG_PRINTF(...)
#endif

#ifdef PRINT_NARROW_EFFECT
  #define EFFECT_PRINTF(...) printf(__VA_ARGS__)
#else
  #define EFFECT_PRINTF(...)
#endif

//
// Count statistics
//
#ifdef PRINT_NARROW_EFFECT_SUMMARY
int narrowCount = 0, wideCount = 0;
#endif

static inline bool isWideType(Symbol* sym) {
  return sym->type->symbol->hasEitherFlag(FLAG_WIDE_CLASS, FLAG_WIDE_REF);
}

static inline bool isRef(Symbol* sym) {
  return sym->type->symbol->hasEitherFlag(FLAG_REF, FLAG_WIDE_REF);
}

//
// A graph to indicate that the symbol 'sym' can be narrowed if
// 'mustBeWide' is not set and all the symbols listed in inVec are
// narrowed.  In this algorithm, only outVec is necessary (and is used
// for the forward propagation).  When development on this pass
// finishes, inVec should either be removed or conditionally removed
// on a macro (if it is useful for debug).
//
class WideInfo {
public:
  Symbol* sym;
  bool mustBeWide;

  Type* wideType; // The original wide type of the symbol

  //
  // For references, we want to know the wideness of the type it
  // wraps.
  //
  bool valIsWide;

  FnSymbol* fnToNarrow; // function to narrow if sym is narrowed
  Vec<Symbol*> inVec;   // if the wide references in this vec are
                        // narrowed, then sym can be narrowed
  Vec<Symbol*> outVec;  // if sym is not narrowed, then the wide
                        // references in this vec cannot be narrowed

  std::vector<SymExpr*> exprsToWiden; // exprs to widen if sym is narrowed
  Vec<CallExpr*> callsToRemove; // calls to remove if sym is narrowed

  // TODO: replace with global lists
  Vec<SymExpr*> getMemberValueFields;

  // If this sym can be narrow, then references to it can be narrow
  Vec<Symbol*> refsToNarrow;

  // If a ref to a wide class becomes a ref to a narrow class,
  // then these symbols can be narrow as well.
  Vec<SymExpr*> destsToNarrow;

  WideInfo() : sym(NULL), mustBeWide(false), valIsWide(false), fnToNarrow(NULL) { }
  WideInfo(Symbol* isym) : sym(isym), mustBeWide(false), valIsWide(false), fnToNarrow(NULL) {
    if (isWideType(sym)) {
      wideType = sym->type;
    }
  }
};

//
// A static callgraph of the program with a transitive closure for each node.
// Used to determine whether or not a variable is used inside an on-clause.
// Currently used to determine if a global variable can be narrowed.
//
class CallGraph {
  public:
    class Node {
      public:
        FnSymbol* fn;
        bool visited, hasOn;
        int onDepth;
        std::set<Node*> transitive, children;

        Node(FnSymbol* f) {
          fn = f;
          onDepth = 0;
          visited = false;
          hasOn = false;
        }
    };

    Node* root;

    typedef std::map<FnSymbol*, Node*> FnNodeMap;
    FnNodeMap vertices;


    CallGraph() {
      root = new Node(NULL);

      // start building nodes at the program's entry points.
      forv_Vec(FnSymbol, fn, gFnSymbols) {
        if (fn->hasFlag(FLAG_MODULE_INIT)) {
          root->children.insert(buildChildren(fn));
        }
      }
      root->children.insert(buildChildren(chpl_gen_main));
      for_set(Node, n, root->children) {
        root->transitive.insert(n);
      }

      // build the transitive closure starting without any on-clauses
      buildTransitive(root);
      cleanVisited();
      computeOnDepth(root);
    }

    void print() {
      // start printing at the root node with zero tabs
      cleanVisited();
      for_set(Node, n, root->children) {
        printHelper(n, 0);
      }
    }

    void printHelper(Node* head, int level) {
      for (int i = 0; i < level; i++) DEBUG_PRINTF("\t");
      DEBUG_PRINTF("%s%s (%d) at level %d\n", head->fn->cname, head->hasOn ? "(on)" : "", head->fn->id, head->onDepth);
      head->visited = true;
      for_set(Node, n, head->children) {
        if (!(n->visited)) {
          printHelper(n, level+1);
        }
        else if (n->children.find(head) != n->children.end()) {
          for (int i = 0; i < level+1; i++) DEBUG_PRINTF("\t");
          DEBUG_PRINTF("%s (recursed into itself)\n", head->fn->cname);
        }
        else {
          for (int i = 0; i < level+1; i++) DEBUG_PRINTF("\t");
          DEBUG_PRINTF("(called %s, already printed)\n", n->fn->cname);
        }
      }
      head->visited = false;
    }

  private:

    void cleanVisited() {
      for (FnNodeMap::iterator it = vertices.begin(); it != vertices.end(); it++) {
        it->second->visited = false;
      }
    }

    Node* buildChildren(FnSymbol* head) {
      Node* ret = new Node(head);
      vertices[head] = ret;

      // calls inside this function
      std::vector<CallExpr*> ch;
      collectFnCallsSTL(head, ch);

      for_vector(CallExpr, call, ch) {
        if (FnSymbol* fn = call->isResolved()) {
          if (fn->hasFlag(FLAG_ON_BLOCK)) {
            ret->hasOn = true;
          }

          Node* toAdd;
          FnNodeMap::iterator it = vertices.find(fn);
          if (it != vertices.end()) {
            // If we see an existing node for a function, use that instead
            toAdd = it->second;
          } else {
            // We haven't built a node for this Fn yet, do so
            toAdd = buildChildren(fn);
          }
          ret->children.insert(toAdd);
          ret->transitive.insert(toAdd);
        }
      }

      return ret;
    }

    void buildTransitive(Node* head) {
      head->visited = true;
      if (head->transitive.empty()) return;

      for_set(Node, n, head->children) {
        if (!n->visited) {
          buildTransitive(n);
        }
        head->transitive.insert(n->transitive.begin(), n->transitive.end());
      }
    }

    void computeOnDepth(Node* head) {
      for_set(Node, n, head->children) {
        depthHelper(n, 0);
      }
    }

    void depthHelper(Node* head, int onLevel) {
      if (!head) return;
      if (head->onDepth == -1) return;
      bool recursive = false, hasCycle = false;
      if (head->children.find(head) != head->children.end()) {
        DEBUG_PRINTF("%s may be recursive\n", head->fn->cname);
        recursive = true;
      }
      if (head->transitive.find(head) != head->transitive.end()) {
        DEBUG_PRINTF("%s may eventually call itself\n", head->fn->cname);
        hasCycle = true;
      }

      if (recursive || hasCycle) {
        if (head->hasOn) {
          DEBUG_PRINTF("\tThis generates an on function, so we may have infinite ons\n");
          for_set(Node, n, head->transitive) {
            n->onDepth = -1;
          }
        }
        return;
      }

      // TODO: I think we'll need to set onLevel if the current depth is greater
      if (head->onDepth >= 0 && head->onDepth < onLevel)
        head->onDepth = onLevel;

      if (head->fn) {
        bool startingOn = head->fn->hasFlag(FLAG_ON);
        if (startingOn) onLevel++;
      }

      for_set(Node, n, head->children) {
        depthHelper(n, onLevel);
      }
    }
};

//
// Type aliases for maps
//
typedef Map<SymExpr*,Type*> SymExprTypeMap;
typedef MapElem<SymExpr*,Type*> SymExprTypeMapElem;
typedef Map<Symbol*,WideInfo*> WideInfoMap;
typedef MapElem<Symbol*,WideInfo*> WideInfoMapElem;

//
// The map from symbols to wideInfo objects.
//
static WideInfoMap* wideInfoMap;
static std::vector<CallExpr*> setMemberLocalFields;
static std::vector<SymExpr*>  getMemberLocalFields;

//
// Static function declarations
//
static void populateWideInfoMap();
static void narrowVarsArgsFieldsInMap(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                      Map<Symbol*,Vec<SymExpr*>*>& useMap,
                                      CallGraph* graph);
static void resolveWideness();
static void resolveHelper(WideInfo* wi);
static void doNarrowing(SymExprTypeMap&);
static void handleLocalFields(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                              Map<Symbol*,Vec<SymExpr*>*>& useMap);
static void pruneNarrowedActuals(SymExprTypeMap&);
static void printNarrowEffectSummary();
static void insertWideReferenceTemps(SymExprTypeMap&);
static void moveAddressSourcesToTemp();
static bool usedInOn(Symbol* sym, Map<Symbol*, Vec<SymExpr*>*> &useMap, CallGraph* graph);

//
// Print routines for debugging and development
//
void
printNode(BaseAST* ast) {
  Symbol* sym = toSymbol(ast);
  WideInfo* wi = wideInfoMap->get(sym);
  printf("%s[%d]\n", sym->cname, sym->id);
  printf("%s\n", (wi->mustBeWide) ? "WIDE" : "NARROW");
  printf("IN: ");
  forv_Vec(Symbol, inSym, wi->inVec)
    printf("%d ", inSym->id);
  printf("\n");
  printf("OUT: ");
  forv_Vec(Symbol, outSym, wi->outVec)
    printf("%d ", outSym->id);
  printf("\n");
}

void
printNode(int id) {
  printNode(aid(id));
}

//
// Add narrowing dependence from one symbol to another to mark that
// the symbol 'to' cannot be narrowed if the symbol 'from' cannot be
// narrowed.
//
static void
addNarrowDep(Symbol* from, Symbol* to) {
  wideInfoMap->get(to)->inVec.add(from);
  wideInfoMap->get(from)->outVec.add(to);
}


//
// Analyze fields to mark them with the "fail to narrow" flag and/or
// add dependences from other symbols to this field.
//
static void
narrowField(Symbol* field, WideInfo* wi) {
  TypeSymbol* ts = toTypeSymbol(field->defPoint->parentSymbol);

  // TODO: What about other bundled args?
  if (!strcmp(ts->name, "_class_localson_fn")) {
    DEBUG_PRINTF("Field %s(%d) in 'on' bundled args must be wide\n", field->cname, field->id);
    wi->mustBeWide = true;
    return;
  }

  if (!strcmp(ts->name, "_class_localscoforall_fn")) {
    if (isRef(field)) {
      wi->mustBeWide = true;
    }
    return;
  }

  if (ts->hasFlag(FLAG_WIDE_REF) ||
      ts->hasFlag(FLAG_WIDE_CLASS)) {
    DEBUG_PRINTF("Field %s(%d) in %s is a ref or class, must be wide\n", field->cname, field->id, ts->cname);
    wi->mustBeWide = true;
    return;
  }

  //
  // The following code is a work in progress to try to narrow fields. (Dietz)
  //

//   for_uses(use, useMap, field) {
//     if (CallExpr* call = toCallExpr(use->parentExpr)) {
//       if (call->isPrimitive(PRIM_SET_MEMBER) && call->get(2) == use) {
//         SymExpr* base = toSymExpr(call->get(1));
//         SymExpr* rhs = toSymExpr(call->get(3));
//         if (base->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
//             base->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
//           addNarrowDep(base->var, field);
//         if (rhs->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
//             rhs->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
//           addNarrowDep(rhs->var, field);
//         continue;
//       }
//       if (call->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
//         continue;
//       }
//     }
    DEBUG_PRINTF("All fields are wide: %s(%d)\n", field->cname, field->id);
    wi->mustBeWide = true;
    return;
//   }
}

static void setWide(WideInfo* wi, SymExpr* se, const char* msg) {
#ifdef PRINT_NARROW_ANALYSIS
  DEBUG_PRINTF("%d %s %s in %s", wi->sym->id, wi->sym->cname, msg, se->getModule()->cname);
  print_view(se->getStmtExpr());
#endif
  wi->mustBeWide = true;
}

//
// Analyze symbols (variables and arguments) to mark them with the
// "fail to narrow" flag and/or add dependences from other symbols to
// this symbol.
//
static void
narrowSym(Symbol* sym, WideInfo* wi,
          Map<Symbol*,Vec<SymExpr*>*>& defMap,
          Map<Symbol*,Vec<SymExpr*>*>& useMap)
{
  bool isWideObj = sym->type->symbol->hasFlag(FLAG_WIDE_CLASS);
  bool isWideRef = sym->type->symbol->hasFlag(FLAG_WIDE_REF);

  // This scans the definitions of the given symbol and weeds out calls that can
  // be narrowed.  If any such call fails to get weeded out, the symbol is
  // marked as "mustBeWide" and this routine returns.
  // Otherwise, all defs can be narrowed, and control drops down to the next loop.
  DEBUG_PRINTF("narrowSym %s (%d)\n", sym->cname, sym->id);
  for_defs(def, defMap, sym) {
    if (CallExpr* call = toCallExpr(def->parentExpr)) {
      if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
        if (CallExpr* rhs = toCallExpr(call->get(2))) {
          if (rhs->isPrimitive(PRIM_GET_PRIV_CLASS)) {
            continue;
          }
          if (rhs->isPrimitive(PRIM_GET_MEMBER)) {
            INT_ASSERT(isWideRef);

            // If the base is wide, the lhs needs to be wide
            SymExpr* base = toSymExpr(rhs->get(1));
            if (base->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
                base->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(base->var, sym);

            // TODO: can we narrow the ref's _val when the member
            // is a local field?
            SymExpr* member = toSymExpr(rhs->get(2));
            if (WideInfo* nw = wideInfoMap->get(member->var->defPoint->sym)) {
              if (member->var->hasFlag(FLAG_LOCAL_FIELD)) {
                getMemberLocalFields.push_back(def);
              }
              nw->refsToNarrow.add(sym);
            }
            continue;
          }
          if (rhs->isPrimitive(PRIM_GET_MEMBER_VALUE)) {
            SymExpr* base = toSymExpr(rhs->get(1));
            SymExpr* member = toSymExpr(rhs->get(2));
            if (base->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
                base->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
              addNarrowDep(base->var, sym);
            }

            if (member->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
                member->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS)) {
              if (fIgnoreLocalClasses || !member->var->hasFlag(FLAG_LOCAL_FIELD)) {
                addNarrowDep(member->var, sym);
              } else {
                wi->getMemberValueFields.add(def);
              }
            }
            continue;
          }
          if (rhs->isPrimitive(PRIM_STRING_COPY)) {
            SymExpr* se = toSymExpr(rhs->get(1));
            if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
                se->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(se->var, sym);
            continue;
          }
          if (rhs->isPrimitive(PRIM_CAST) ||
              rhs->isPrimitive(PRIM_DYNAMIC_CAST)) {
            SymExpr* se = toSymExpr(rhs->get(2));
            if (se->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
                se->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS))
              addNarrowDep(se->var, sym);
            continue;
          }
          if (rhs->isPrimitive(PRIM_ARRAY_GET) || rhs->isPrimitive(PRIM_ARRAY_GET_VALUE)) {
            // ddatas with class elements have their elements automatically widened.
            // The LHS of moves should be a wide class.

            SymExpr* se = toSymExpr(rhs->get(1));

            addNarrowDep(se->var, sym);

            // Get the narrow data class type
            Type* rtype = se->var->type;
            if (se->var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
              rtype = rtype->getField("addr")->type;
            }

            // TODO: Fix once we have narrow elements
            //
            // Get the element type and check its wideness
            INT_ASSERT(rtype->symbol->hasFlag(FLAG_DATA_CLASS));
            if (TypeSymbol* ts = getDataClassType(rtype->symbol)) {
              if (ts->hasFlag(FLAG_WIDE_CLASS)) {
                if (isRef(sym)) {
                  wi->valIsWide = true;
                } else if (sym->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
                  wi->mustBeWide = true;
                }
              }
            }
            continue;
          }
          if (rhs->isPrimitive(PRIM_DEREF)) {
            // If the ref stores a narrow _val, the lhs of this move can also
            // be narrow.
            SymExpr* se = toSymExpr(rhs->get(1));
            if (WideInfo* nw = wideInfoMap->get(se->var)) {
              nw->destsToNarrow.add(def);
              addNarrowDep(se->var, sym);
            }
            continue;
          }
          if (rhs->isPrimitive(PRIM_ADDR_OF)) {
            // If the src type is narrow, the _val of this ref can be narrow
            if (SymExpr* src = toSymExpr(rhs->get(1))) {
              if (WideInfo* nw = wideInfoMap->get(src->var)) {
                nw->refsToNarrow.add(sym);
                wi->destsToNarrow.add(src);
              }
            }
            continue;
          }
          if (rhs->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) ||
              rhs->isPrimitive(PRIM_GET_SVEC_MEMBER)) {
            Symbol* member = getSvecSymbol(rhs);
            if (member == NULL) {
              Type* ty = rhs->get(1)->getValType();

              // For star tuples we'll use the first field
              if (ty->symbol->hasFlag(FLAG_STAR_TUPLE)) {
                AggregateType* tuple = toAggregateType(ty);
                member = tuple->getField(1);
              }
            }

            if (member && isWideType(member)) {
              addNarrowDep(member, sym);
            }

            if (SymExpr* se = toSymExpr(rhs->get(1))) {
              if (isWideType(se->var)) {
                addNarrowDep(toSymExpr(rhs->get(1))->var, sym);
              }
            }
            continue;
          }
          if (rhs->isPrimitive(PRIM_STRING_FROM_C_STRING)) {
            // The destination string need not be wide if we're initializing from
            // a c_string.
            continue;
          }
          if (rhs->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
            // For now, we assume everything returned from a virtual method is wide.
            wi->mustBeWide = true;
            return;
          }
          if (FnSymbol* fn = rhs->isResolved()) {
            if (fn->hasFlag(FLAG_LOCALE_MODEL_ALLOC))
              continue;
            bool retIsWide = isWideType(fn->retType->symbol);
            if (retIsWide && (isWideRef || isWideObj)) {
                addNarrowDep(fn->getReturnSymbol(), sym);
            }
            if (isRef(sym)) {
              if (WideInfo* next = wideInfoMap->get(fn->getReturnSymbol())) {
                DEBUG_PRINTF("refsToNarrow: %d -> %d\n", fn->getReturnSymbol()->id, sym->id);
                next->refsToNarrow.add(sym);
              }
            }
            continue;
          }
        }
        if (SymExpr* rhs = toSymExpr(call->get(2))) {
            bool isObj = isWideObj || isClass(sym->type);
            bool rhsIsWide = isWideType(rhs->var);
            if (rhsIsWide && (isRef(sym) || isObj)) {
              addNarrowDep(rhs->var, sym);
            }
            if (isRef(sym)) {
              if (rhs->var->type->symbol->hasFlag(FLAG_WIDE_CLASS) ||
                  isRef(rhs->var)) {
                wideInfoMap->get(rhs->var)->refsToNarrow.add(sym);
                if (isRef(rhs->var)) {
                  wi->refsToNarrow.add(rhs->var);
                }
              }
            }
          continue;
        } else {
          setWide(wi, def, "def fail to narrow");
          return;
        }
      } else if(isOpEqualPrim(call)) {
        continue;
      } else if (call->isResolved() &&
                 !strcmp(call->isResolved()->name, "=")) // A flag should be
                                                         // used to distinguish
                                                         // assignment
                                                         // operators.
      {
        continue;
      }
    }
  }

  for_uses(use, useMap, sym) {
    if (CallExpr* call = toCallExpr(use->parentExpr)) {
      if ((call->isPrimitive(PRIM_MOVE)) ||
          (call->isPrimitive(PRIM_ASSIGN)) ||
          (call->isPrimitive(PRIM_SET_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_MEMBER_VALUE) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_SET_SVEC_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_SVEC_MEMBER) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_GET_SVEC_MEMBER_VALUE) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_CAST) && call->get(2) == use) ||
          (call->isPrimitive(PRIM_DYNAMIC_CAST)) ||
          (call->isPrimitive(PRIM_CAST_TO_VOID_STAR) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_ARRAY_GET) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_ARRAY_GET_VALUE)) ||
          (call->isPrimitive(PRIM_ARRAY_ALLOC)) ||
          (call->isPrimitive(PRIM_GETCID) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_TESTCID) && call->get(1) == use) ||
          (call->isPrimitive(PRIM_DEREF)) ||
          (call->isPrimitive(PRIM_ADDR_OF)) ||
          (call->isPrimitive(PRIM_SYNC_INIT)) ||
          (call->isPrimitive(PRIM_SYNC_LOCK)) ||
          (call->isPrimitive(PRIM_SYNC_DESTROY)) ||
          (call->isPrimitive(PRIM_NEW_PRIV_CLASS)) ||
          (call->isPrimitive(PRIM_PROCESS_TASK_LIST)) ||
          (call->isPrimitive(PRIM_STRING_COPY)) ||
          (call->isPrimitive(PRIM_SETCID)) ||
          (call->isPrimitive(PRIM_SIZEOF)) ||
          (call->isPrimitive(PRIM_RT_ERROR)) ||
          (call->isPrimitive(PRIM_WIDE_GET_NODE)) ||
          (call->isPrimitive(PRIM_WIDE_GET_LOCALE)) ||
          (call->isPrimitive(PRIM_PRIVATE_BROADCAST)) ||
          (call->isPrimitive(PRIM_C_STRING_FROM_STRING)) ||
          (call->isPrimitive(PRIM_ARRAY_FREE)) ||
          (call->isResolved() &&
           (call->isResolved()->hasFlag(FLAG_LOCALE_MODEL_ALLOC) ||
            call->isResolved()->hasFlag(FLAG_LOCALE_MODEL_FREE)) &&
           call->get(1)==use) ||
          (isOpEqualPrim(call)) )
        continue;
      if (call->isResolved()) {
        // pruneNarrowedActuals will clean up
        DEBUG_PRINTF("call: widening expr %d\n", sym->id);
        wi->exprsToWiden.push_back(use);
        continue;
      }
      if (call->isPrimitive(PRIM_HEAP_REGISTER_GLOBAL_VAR)) {
        DEBUG_PRINTF("Registered as global heap var, must be wide\n");
        wi->mustBeWide = true;
        return;
      }
      if (call->isPrimitive(PRIM_SET_MEMBER) && call->get(3) == use) {
        SymExpr* member = toSymExpr(call->get(2));
        SymExpr* base = toSymExpr(call->get(1));
        if (isRef(sym)) {
          addNarrowDep(member->var, sym);
        } else {
          // coforall bundled args are assigned to once, so we can easily
          // reason about their wideness.
          if (!strcmp(base->var->type->symbol->cname, "_class_localscoforall_fn")) {
            DEBUG_PRINTF("trying to narrow coforall arg: %d -> %d\n", sym->id, member->var->id);
            addNarrowDep(sym, member->var);
          } else {
            // For all other types, we assume the fields are wide.
            DEBUG_PRINTF("set: widening expr %d\n", sym->id);
            wi->exprsToWiden.push_back(use);
            if (member->var->hasFlag(FLAG_LOCAL_FIELD)) {
              setMemberLocalFields.push_back(call);
            }
          }
        }
        continue;
      }
      if (call->isPrimitive(PRIM_SET_SVEC_MEMBER) && call->get(3) == use) {
        if (isRef(sym)) {
          Symbol* member = getSvecSymbol(call);
          if (member == NULL) {
            Type* ty = call->get(1)->getValType();

            // Use the first field for star tuples
            if (ty->symbol->hasFlag(FLAG_STAR_TUPLE)) {
              AggregateType* tuple = toAggregateType(ty);
              member = tuple->getField(1);
            }
          }
          if (member && isWideType(member)) {
            addNarrowDep(member, sym);
          }
        } else {
          DEBUG_PRINTF("widening expr %d\n", sym->id);
          wi->exprsToWiden.push_back(use);
        }
        continue;
      }
      if (call->isPrimitive(PRIM_CHPL_COMM_GET)) {
        DEBUG_PRINTF("Used in comm_get, must be wide\n");
        // TODO: would exprsToWiden be better here?
        wi->mustBeWide = true;
        continue;
      }
      if (call->isPrimitive(PRIM_LOCAL_CHECK)) {
        wi->callsToRemove.add(call);
        continue;
      }
      if (call->isPrimitive(PRIM_ARRAY_SET_FIRST)) {
        if (call->get(3) == use) {
          wi->exprsToWiden.push_back(use);
        }
        continue;
      }
      if (call->isPrimitive(PRIM_RETURN)) {
        FnSymbol* fn = toFnSymbol(call->parentSymbol);
        if (!isWideType(fn->retType->symbol))
          // already narrow, so skip it.
          continue;
        wi->fnToNarrow = fn;
        INT_ASSERT(fn);
        // The return type of virtual functions currently needs to be wide in
        // order for codegen to stamp out the correct cast:
        //    (chpl____wide_BaseDom(*))vmlTable[...]
        forv_Vec(CallExpr*, call, *fn->calledBy) {
          if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
            wi->mustBeWide = true;
            return;
          }
        }
        continue;
      }
      if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
        DEBUG_PRINTF("expr to widen: %s (%d / %d)\n", sym->cname, use->id, sym->id);
        wi->exprsToWiden.push_back(use);
        continue;
      }
      if (call->isPrimitive(PRIM_PTR_EQUAL) || call->isPrimitive(PRIM_PTR_NOTEQUAL)) {
        SymExpr* left = toSymExpr(call->get(1));
        SymExpr* right = toSymExpr(call->get(2));
        if (left->typeInfo() != dtNil && right->typeInfo() != dtNil) {
          // TODO: prune
          wi->exprsToWiden.push_back(use);
        }
        continue;
      }
      if (call->isPrimitive(PRIM_ARRAY_SHIFT_BASE_POINTER)) {
        if (use == call->get(1)) {
          addNarrowDep(toSymExpr(call->get(2))->var, sym);
        }
        continue;
      }
    }
    //
    // It's not clear to me (bradcray) why falling through to this
    // point has traditionally meant "this case must be wide"; this
    // has the effect of causing things to be widened unnecessarily.
    // See, for example, test/users/ferguson/ddata/cToChapelArray.chpl
    // under r22899: the 'array' argument to chapelProc() is passed
    // in narrow, but then gets widened unnecessarily.
    //
    // Once formal temps were eliminated in r22900, this 'mustBeWide'
    // case became incorrect because it caused a formal that must be
    // narrow (due to being an exported function) to be incorrectly
    // widened.  This suggests that there are other cases that could
    // also be narrowed by adding additional checks here.  More to
    // the point, it raises questions for me about why the default
    // behavior for 'narrowSym()' is "it must be wide."
    //
    if (ArgSymbol* arg = toArgSymbol(sym)) {
      if (arg->defPoint->parentSymbol->hasFlag(FLAG_LOCAL_ARGS)) {
        // this arg can't be wide, so don't set mustBeWide
        return;
      }
    }
    setWide(wi, use, "use fail to narrow");
    return;
  }
}


//
// Analyze arguments to mark them with the "fail to narrow" flag
// and/or add dependences from other symbols to this argument.
// Piggyback on the analysis of variables.
//
static void
narrowArg(ArgSymbol* arg, WideInfo* wi,
          Map<Symbol*,Vec<SymExpr*>*>& defMap,
          Map<Symbol*,Vec<SymExpr*>*>& useMap)
{
  FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);
  INT_ASSERT(fn);
  forv_Vec(FnSymbol, indirectlyCalledFn, ftableVec) {
    if (fn == indirectlyCalledFn) {
      DEBUG_PRINTF("%s called from ftableVec\n", arg->cname);
      wi->mustBeWide = true;
      return;
    }
  }
  forv_Vec(CallExpr, call, *fn->calledBy) {
    if (call->isPrimitive(PRIM_VIRTUAL_METHOD_CALL)) {
      // All related virtual methods need to have the same function definition.
      // For now, let's simply assert that they all should have wide args.
      //
      // Some later optimization should try to synchronize them onto narrow
      // args if possible.
      wi->mustBeWide = true;
    } else {
      SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
      INT_ASSERT(actual);
      if (isWideType(actual->var)) {
        DEBUG_PRINTF("\targdep %d->%d\n", actual->var->id, arg->id);
        addNarrowDep(actual->var, arg);
        
        // sync _val wideness
        wi->refsToNarrow.add(actual->var);
        wideInfoMap->get(actual->var)->refsToNarrow.add(arg);
      }
    }

  }
  narrowSym(arg, wi, defMap, useMap);
}

//
// Returns true if the symbol is used within a function that might be remote
//
static bool usedInOn(Symbol* sym, Map<Symbol*, Vec<SymExpr*>*> &useMap, CallGraph* graph) {
  for_uses(use, useMap, sym) {
    if (FnSymbol* fn = toFnSymbol(use->parentSymbol)) {
      if (CallGraph::Node* n = graph->vertices[fn]) {
        if (n->onDepth != 0 ||
            fn->hasFlag(FLAG_ON_BLOCK) ||
            fn->hasFlag(FLAG_ON)) {
              DEBUG_PRINTF("ON: %s used in %s (%d): %d\n", sym->cname, fn->cname, fn->id, n->onDepth);
              return true;
        }
      } else return true;
    }
  }
  return false;
}


void
narrowWideReferences() {
  if (!requireWideReferences()) return;

  Map<Symbol*,Vec<SymExpr*>*> defMap;
  Map<Symbol*,Vec<SymExpr*>*> useMap;

  buildDefUseMaps(defMap, useMap);

  wideInfoMap = new WideInfoMap();

  CallGraph* graph = new CallGraph();

//
// Set of uses that may need to be widened after narrowing.  This
// happens, for example, if we widen a reference that has to be passed
// to a function that expects a wide reference and other calls to that
// function pass it a wide reference.  Another choice would be to
// clone the function.
//
  SymExprTypeMap* widenMap = new SymExprTypeMap();

  compute_call_sites();

  populateWideInfoMap();

  narrowVarsArgsFieldsInMap(defMap, useMap, graph);

  resolveWideness();

  doNarrowing(*widenMap);

  pruneNarrowedActuals(*widenMap);

  printNarrowEffectSummary();

  insertWideReferenceTemps(*widenMap);

  if (!fIgnoreLocalClasses) {
    handleLocalFields(defMap, useMap);
  }

  //
  // Free WideInfo class instances and def and use maps.
  //
  form_Map(WideInfoMapElem, e, *wideInfoMap) {
    delete e->value;
  }
  delete wideInfoMap; wideInfoMap = 0;
  delete widenMap; widenMap = 0;

  freeDefUseMaps(defMap, useMap);

  // TODO: Test if this step is really necessary.  If it is, document why.
  moveAddressSourcesToTemp();
}


static void populateWideInfoMap()
{
  // Insert all wide variables and arguments into wideInfoMap.
  // Also insert local refs and local data classes.

  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->type->symbol->hasFlag(FLAG_WIDE_REF) ||
        var->type->symbol->hasFlag(FLAG_REF) ||
        var->type->symbol->hasFlag(FLAG_DATA_CLASS) ||
        var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideInfoMap->put(var, new WideInfo(var));
    }
  }
  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (arg->type->symbol->hasFlag(FLAG_WIDE_REF) ||
        arg->type->symbol->hasFlag(FLAG_REF) ||
        arg->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideInfoMap->put(arg, new WideInfo(arg));
    }
  }
}


static void narrowVarsArgsFieldsInMap(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                                      Map<Symbol*,Vec<SymExpr*>*>& useMap,
                                      CallGraph* graph)
{
  form_Map(WideInfoMapElem, e, *wideInfoMap) {
    WideInfo* wi = e->value;

    if (ArgSymbol* arg = toArgSymbol(wi->sym)) {
      narrowArg(arg, wi, defMap, useMap);
    }

    if (VarSymbol* var = toVarSymbol(wi->sym)) {
      if (isTypeSymbol(var->defPoint->parentSymbol)) {
        INT_ASSERT(defMap.get(var) == NULL);
        narrowField(var, wi);
      }
      else {
        // TODO: Can FLAG_PRIVATE vars be narrow?

        narrowSym(var, wi, defMap, useMap);

        if (!wi->mustBeWide && wi->sym->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {

          Symbol* defParent = var->defPoint->parentSymbol;

          if (isModuleSymbol(defParent)) {
            wi->mustBeWide = usedInOn(wi->sym, useMap, graph);
            if (!wi->mustBeWide)
              DEBUG_PRINTF("Able to narrow %s (%d)\n", wi->sym->cname, wi->sym->id);
            else
              DEBUG_PRINTF("Unable to narrow %s (%d)\n", wi->sym->cname, wi->sym->id);
          }
        } else {
          if (wi->mustBeWide)
            DEBUG_PRINTF("narrowSym set %d to wide\n", wi->sym->id);
        }
      }
    }
  }
}

static void resolveHelper(WideInfo* wi) {
  if (wi->mustBeWide) {
    if (wi->outVec.length() > 0) {
      DEBUG_PRINTF("%s (%d) in %s is wide, so these should be:\n", wi->sym->cname, wi->sym->id, wi->sym->getModule()->cname);
    }

    forv_Vec(Symbol, outSym, wi->outVec) {
      WideInfo* nwi = wideInfoMap->get(outSym);
      DEBUG_PRINTF("%s (%d)", nwi->sym->cname, nwi->sym->id);
      if (!nwi->mustBeWide && isWideType(nwi->sym)) {
        DEBUG_PRINTF(" (%d)\n", wi->sym->id);
        nwi->mustBeWide = true;
        resolveHelper(nwi);
      } else DEBUG_PRINTF("\n");
    }

    //
    // If this variable is wide, then the value wrapped by these references
    // also need to be wide. Since the value in those references is wide,
    // the destination of a dereference also needs to be wide.
    //
    if (wi->refsToNarrow.length() > 0) {
      DEBUG_PRINTF("%s (%d) in %s is wide, these refs must be:\n", wi->sym->cname, wi->sym->id, wi->sym->getModule()->cname);
    }
    forv_Vec(Symbol, se, wi->refsToNarrow) {
      WideInfo* nwi = wideInfoMap->get(se);
      DEBUG_PRINTF("REF %s (%d)", nwi->sym->cname, nwi->sym->id);
      if (!nwi->valIsWide) {
        DEBUG_PRINTF(" (%d)\n", wi->sym->id);
        nwi->valIsWide = true;
        resolveHelper(nwi);
      } else DEBUG_PRINTF("\n");
    }

    if (isRef(wi->sym)) {
      if (wi->destsToNarrow.length() > 0) {
        DEBUG_PRINTF("Ref %s (%d) in %s is wide, these dests must be:\n", wi->sym->cname, wi->sym->id, wi->sym->getModule()->cname); 
      }
      forv_Vec(SymExpr, se, wi->destsToNarrow) {
        WideInfo* nwi = wideInfoMap->get(se->var);
        DEBUG_PRINTF("DEST %s (%d)", nwi->sym->cname, nwi->sym->id);
        if (!nwi->mustBeWide) {
          DEBUG_PRINTF(" (%d)\n", wi->sym->id);
          nwi->mustBeWide = true;
          resolveHelper(nwi);
        } else DEBUG_PRINTF("\n");
      }
    }
  } else {
    if (wi->refsToNarrow.length() > 0) {
      DEBUG_PRINTF("Propagating ref %d in %s\n", wi->sym->id, wi->sym->getModule()->cname);
    }

    // ref that could be narrow
    forv_Vec(Symbol, se, wi->refsToNarrow) {
      WideInfo* nwi = wideInfoMap->get(se);
      DEBUG_PRINTF("REF %s (%d): val is ", nwi->sym->cname, nwi->sym->id);
      if (wi->valIsWide) DEBUG_PRINTF("wide");
      else DEBUG_PRINTF("not wide");

      if (wi->valIsWide && !nwi->valIsWide) {
        DEBUG_PRINTF(" (%d)\n", wi->sym->id);
        nwi->valIsWide = wi->valIsWide;
        resolveHelper(nwi);
      }
      else DEBUG_PRINTF("\n");
    }

    if (wi->valIsWide) {
      if (wi->destsToNarrow.length() > 0) {
        DEBUG_PRINTF("Widening dests of %d\n", wi->sym->id);
      }

      forv_Vec(SymExpr, se, wi->destsToNarrow) {
        WideInfo* nwi = wideInfoMap->get(se->var);
        DEBUG_PRINTF("DEST: %d", se->var->id);
        if (!nwi->mustBeWide) {
          DEBUG_PRINTF(" (%d)\n", wi->sym->id);
          nwi->mustBeWide = true;
          resolveHelper(nwi);
        } else DEBUG_PRINTF("\n");
      }
    }
  }
}

static void resolveWideness() {
  form_Map(WideInfoMapElem, e, *wideInfoMap) {
    WideInfo* wi = e->value;
    resolveHelper(wi);
  }
}

static void doNarrowing(SymExprTypeMap& widenMap)
{
  // Now, traverse the wideInfoMap, and perform the narrowings it calls for.
  // Uses of the variable or argument requiring widening are added to widenMap.
  form_Map(WideInfoMapElem, e, *wideInfoMap) {
    WideInfo* wi = e->value;
    if (!wi->mustBeWide) {

      //
      // add exprs to widen to the widen map
      //
      for_vector(SymExpr, se, wi->exprsToWiden) {
        widenMap.put(se, se->var->type);
      }

      //
      // remove calls to remove
      //
      forv_Vec(CallExpr, call, wi->callsToRemove) {
        call->remove();
      }

      if (wi->fnToNarrow) {
        wi->fnToNarrow->retType = wi->fnToNarrow->retType->getField("addr")->type;
        EFFECT_PRINTF("%d %s function narrowed\n", wi->fnToNarrow->id, wi->fnToNarrow->cname);
      }

#ifdef PRINT_NARROW_EFFECT_SUMMARY
      narrowCount++;
#endif
      EFFECT_PRINTF("%d %s narrowed of from %s\n", wi->sym->id, wi->sym->cname, wi->sym->type->symbol->cname);
      if (isWideType(wi->sym)) {
        wi->sym->type = wi->sym->type->getField("addr")->type;
      }
      EFFECT_PRINTF("\t-> %s\n", wi->sym->type->symbol->cname);

      if (wi->sym->type->symbol->hasFlag(FLAG_REF)) {
        if (wi->sym->type->getField("_val")->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
          if (!wi->valIsWide) {
            EFFECT_PRINTF("\tNarrowed value of ref %d: ", wi->sym->id);
            EFFECT_PRINTF("%s -> %s\n", wi->sym->type->symbol->cname, narrowRefWideMap[wi->sym->type]->symbol->cname);
            wi->sym->type = narrowRefWideMap[wi->sym->type];
          }
        }
      }
    } else {
      if(!isWideType(wi->sym)) {
        if (!isTypeSymbol(wi->sym->defPoint->parentSymbol)) {
          DEBUG_PRINTF("incorrectly wide: %s(%d): %s\n",wi->sym->cname, wi->sym->id, wi->sym->type->symbol->cname);
        }
      }
    }
  }
}


// TODO: It looks like we're narrowing args that will be in a virtual method call, 
// which is not resolved and can't be used here...?
static void pruneNarrowedActuals(SymExprTypeMap& widenMap)
{
  //
  // Prune the map of expressions to widen because of arguments that
  // have been narrowed.
  //
  form_Map(WideInfoMapElem, e, *wideInfoMap) {
    WideInfo* wi = e->value;
    if (!wi->mustBeWide) {
      if (ArgSymbol* arg = toArgSymbol(wi->sym)) {
        DEBUG_PRINTF("Pruning arg %d\n", arg->id);
        FnSymbol* fn = toFnSymbol(arg->defPoint->parentSymbol);
        forv_Vec(CallExpr, call, *fn->calledBy) {
          SymExpr* actual = toSymExpr(formal_to_actual(call, arg));
          widenMap.put(actual, NULL);
          DEBUG_PRINTF("PRUNE: %d\n", actual->var->id);
        }
      }
    }
  }
}


static void printNarrowEffectSummary()
{
#ifdef PRINT_NARROW_EFFECT_SUMMARY
  forv_Vec(VarSymbol, var, gVarSymbols) {
    if (var->type->symbol->hasFlag(FLAG_WIDE_REF) ||
        var->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideCount++;
    }
  }

  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    if (arg->type->symbol->hasFlag(FLAG_WIDE_REF) ||
        arg->type->symbol->hasFlag(FLAG_WIDE_CLASS)) {
      wideCount++;
    }
  }
  
  printf("Narrow count: %d\nWide count: %d\n", narrowCount, wideCount);
#endif
}


static void insertWideReferenceTemps(SymExprTypeMap& widenMap)
{
  //
  // Insert a wide reference temporary if we narrowed a wide reference
  // that is passed to a function that expects a wide reference.
  //
  form_Map(SymExprTypeMapElem, e, widenMap) {
    SymExpr* key = e->key;
    Type* value = e->value;
    if (value && key->var->type != value) { // can this be an assert?
      Expr* stmt = key->getStmtExpr();
      SET_LINENO(stmt);
      Symbol* tmp = newTemp(value);
      DEBUG_PRINTF("Made temp %d for %d\n", tmp->id, key->var->id);
      stmt->insertBefore(new DefExpr(tmp));
      key->replace(new SymExpr(tmp));
      stmt->insertBefore(new CallExpr(PRIM_MOVE, tmp, key));
    }
  }
}


// In every move:
//   if the LHS type has the WIDE or REF flag
//   and its value type is a wide class
//   and the RHS type is the same as the contents of the wide pointer:
//     Create a temp copy of the RHS, and
//     replace the RHS of the move with the temp.
static void moveAddressSourcesToTemp()
{
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
      if ((call->get(1)->typeInfo()->symbol->hasFlag(FLAG_WIDE_REF) ||
           call->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF)) &&
          call->get(1)->getValType()->symbol->hasFlag(FLAG_WIDE_CLASS) &&
          call->get(2)->typeInfo() == call->get(1)->getValType()->getField("addr")->type) {
        //
        // widen rhs class
        //
        SET_LINENO(call);
        VarSymbol* tmp = newTemp(call->get(1)->getValType());
        call->insertBefore(new DefExpr(tmp));
        call->insertBefore(new CallExpr(PRIM_MOVE, tmp, call->get(2)->remove()));
        call->insertAtTail(tmp);
        DEBUG_PRINTF("TEMP: %d\n", tmp->id);
      }
    }
  }
}

//
// insert an check that will halt if the nodes are not identical
// for these SymExprs.
//
static void insertNodeComparison(Expr* stmt, SymExpr* lhs, SymExpr* rhs) {
  stmt->insertBefore(new CondStmt(new CallExpr(PRIM_NOTEQUAL,
    new CallExpr(PRIM_WIDE_GET_NODE, lhs->copy()),
    new CallExpr(PRIM_WIDE_GET_NODE, rhs->copy())),
    new CallExpr(PRIM_RT_ERROR, new_StringSymbol("Attempted to assign to local class field with remote class"))));
}

static void handleLocalFields(Map<Symbol*,Vec<SymExpr*>*>& defMap,
                              Map<Symbol*,Vec<SymExpr*>*>& useMap) {
  // TODO:
  //   - errors/warnings for "local field" pragma usage
  form_Map(WideInfoMapElem, e, *wideInfoMap) {
    WideInfo* wi = e->value;
    if (!wi->mustBeWide) {
      forv_Vec(SymExpr, se, wi->getMemberValueFields) {
        // TODO: try to skip inside local fn?
        SET_LINENO(se);
        VarSymbol* var = newTemp(astr("wide_", se->var->name), wi->wideType);
        Expr* stmt = toExpr(se)->getStmtExpr();
        INT_ASSERT(se && stmt);

        stmt->insertBefore(new DefExpr(var));
        stmt->insertAfter(new CallExpr(PRIM_MOVE, se->copy(), var));
        if (!fNoLocalChecks) {
          stmt->insertAfter(new CallExpr(PRIM_LOCAL_CHECK, new SymExpr(var)));
        }

        se->replace(new SymExpr(var));
      }
    }
  }
  if (!fNoLocalChecks) {
    for_vector(SymExpr, se, getMemberLocalFields) {
      for_defs(def, defMap, se->var) {
        if (def == se) continue;
        if (CallExpr* call = toCallExpr(def->parentExpr)) {
          if (call->isPrimitive(PRIM_MOVE) || call->isPrimitive(PRIM_ASSIGN)) {
            if (def == call->get(1)) {
              // using a ref to initialize a class member
              // assert that the locales match
              DEBUG_PRINTF("inserting assignment locality check\n");
              SET_LINENO(def);
              Expr* stmt = toExpr(def)->getStmtExpr();
              VarSymbol* tmp = newTemp(def->var->getValType());
              stmt->insertBefore(new DefExpr(tmp));
              SymExpr* other = toSymExpr(call->get(2));
              stmt->insertBefore(new CallExpr(PRIM_MOVE, new SymExpr(tmp), new CallExpr(PRIM_DEREF, new SymExpr(se->var))));
              insertNodeComparison(stmt, new SymExpr(tmp), other);
            }
          }
        }
      }
    }
    for_vector(CallExpr, call, setMemberLocalFields) {
      INT_ASSERT(call->isPrimitive(PRIM_SET_MEMBER));
      SymExpr* rhs = toSymExpr(call->get(3));
      Expr* stmt = rhs->getStmtExpr();
      SET_LINENO(stmt);

      SymExpr* base = toSymExpr(call->get(1));
      //SymExpr* member = toSymExpr(call->get(2));

      if (isWideType(base->var)) {
        // base.node == rhs.node
        insertNodeComparison(stmt, base->copy(), rhs->copy());
      } else {
        // rhs.node is local
        stmt->insertBefore(new CallExpr(PRIM_LOCAL_CHECK, rhs->copy()));
      }
    }
  }
}
