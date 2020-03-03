/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#include "passes.h"

#include "astutil.h"
#include "driver.h"
#include "expr.h"
#include "ForallStmt.h"
#include "ForLoop.h"
#include "iterator.h"
#include "lateConstCheck.h"
#include "loopDetails.h"
#include "postFold.h"
#include "resolution.h"
#include "stlUtil.h"
#include "stmt.h"
#include "symbol.h"
#include "view.h"

/* This pass implements a backwards (uses to defs) analysis
   to determine if certain reference Symbols are constant.

   The are two language features that require this backwards
   data flow (and they are related to each other).
   The first is the default argument intent for arrays - which is
   `const ref` if the array is not modified in the body of
   the function and `ref` if it is. The second is the `ref-pair`
   feature which was created for sparse arrays.

   This pass proceeds for the most part by collecting
   Symbols needing special attention and determining which
   of them can be marked as constant. (For a ContextCallExpr,
   the relevant Symbol is the temporary that stores the result
   of the ContextCallExpr). Next, it determines which of these
   Symbols can be `const ref` and which must continue to be
   mutable `ref`. The general strategy there is to keep track
   of symbols of unknown const-ness.

   Then, it performs appropriate transformations,
   which amount to updating ContextCallExprs to use
   one call or the other (the ref-return version or
   the value/const-ref-return version).
 */


// Used for debugging this pass.
static const int breakOnId1 = 0;
static const int breakOnId2 = 0;
static const int breakOnId3 = 0;

#define DEBUG_SYMBOL(sym) \
  if (sym->id == breakOnId1 || sym->id == breakOnId2 || sym->id == breakOnId3) { \
    gdbShouldBreakHere(); \
  }

static const int trace_all = 0;
static const int trace_usr = 0;

static bool shouldTrace(Symbol* sym)
{
  if (trace_all ||
      (trace_usr && sym->defPoint->getModule()->modTag == MOD_USER))
    return true;
  else
    return false;
}

//static bool symbolIsSetLocal(Symbol* sym);

typedef enum {
  USE_REF = 1,
  USE_CONST_REF,
  USE_VALUE,
} choose_type_t;

static bool symExprIsSet(SymExpr* sym);
static bool symbolIsUsedAsConstRef(Symbol* sym);
static void lowerContextCall(ContextCallExpr* cc, choose_type_t which);
static void lowerContextCallPreferRefConstRef(ContextCallExpr* cc);
static void lowerContextCallPreferConstRefValue(ContextCallExpr* cc);
static void lowerContextCallComputeConstRef(ContextCallExpr* cc, bool notConst, Symbol* lhsSymbol);
static bool firstPassLowerContextCall(ContextCallExpr* cc);

static bool
symExprIsSetByDef(SymExpr* def) {
  // We're only looking for things that set the value.
  // We don't care about PRIM_MOVEs b/c they only set the reference.
  // We do care about PRIM_ASSIGN or if the argument is passed
  // to a function (typically = ) as ref, inout, or out argument.
  if (def->parentExpr) {
    if (CallExpr* parentCall = toCallExpr(def->parentExpr)) {
      if (parentCall->isPrimitive(PRIM_MOVE) &&
          parentCall->get(1)->typeInfo()->symbol->hasFlag(FLAG_REF) &&
          parentCall->get(2)->typeInfo()->symbol->hasFlag(FLAG_REF)) {
        // Ignore this def
        // We don't care about a PRIM_MOVE because it's setting
        // a reference
      } else {
        return true;
      }
    }
  }

  return false;
}

static bool
symExprIsSetByUse(SymExpr* use) {
  if (CallExpr* call = toCallExpr(use->parentExpr)) {
    if (FnSymbol* fn = call->resolvedFunction()) {
      ArgSymbol* formal = actual_to_formal(use);

      if (formal->intent == INTENT_INOUT || formal->intent == INTENT_OUT) {
        // Shouldn't this be a Def, not a Use, then?
        INT_ASSERT(0);
        return true;
      }

      if (formal->type->symbol->hasFlag(FLAG_REF) &&
          (fn->hasFlag(FLAG_ALLOW_REF) ||
           formal->hasFlag(FLAG_WRAP_WRITTEN_FORMAL))) {
        // This case has to do with wrapper functions (promotion?)
        return true;
      }

    } else if (call->isPrimitive(PRIM_SET_MEMBER)) {
      // PRIM_SET_MEMBER to set the pointer inside of a reference
      // counts as "setter"
      // the below conditional would better be isRefType()
      if (!call->get(2)->typeInfo()->refType) {
        return true;
      }

    } else if (call->isPrimitive(PRIM_RETURN) ||
               call->isPrimitive(PRIM_YIELD)) {
      FnSymbol* inFn = toFnSymbol(call->parentSymbol);

      // It is not necessary to use the 'ref' version
      // if the function result is returned by 'const ref'.
      if (inFn->retTag == RET_CONST_REF) return false;
      // MPF: it seems to cause problems to return false
      // here when inFn->retTag is RET_VALUE.
      // TODO: can we add
      //if (inFn->retTag == RET_VALUE) return false;
      return true;
    }
  }

  return false;
}

static
bool symExprIsUsedAsConstRef(SymExpr* use) {
  if (CallExpr* call = toCallExpr(use->parentExpr)) {
    if (FnSymbol* calledFn = call->resolvedFunction()) {
      ArgSymbol* formal = actual_to_formal(use);

      // generally, use const-ref-return if passing to const ref formal
      if (formal->intent == INTENT_CONST_REF) {
        // but make an exception for initCopy calls
        if (calledFn->hasFlag(FLAG_INIT_COPY_FN))
          return false;

        // TODO: tuples of types with blank intent
        // being 'in' should perhaps use the value version.
        return true;
      }

    } else if (call->isPrimitive(PRIM_RETURN) ||
               call->isPrimitive(PRIM_YIELD)) {
      FnSymbol* inFn = toFnSymbol(call->parentSymbol);

      // use const-ref-return if returning by const ref intent
      if (inFn->retTag == RET_CONST_REF)
        return true;

    } else if (call->isPrimitive(PRIM_WIDE_GET_LOCALE) ||
               call->isPrimitive(PRIM_WIDE_GET_NODE)) {
      // If we are extracting a field from the wide pointer,
      // we need to keep it as a pointer.

      // use const-ref-return if querying locale
      return true;

    } else {
      // Check for the case that sym is moved to a compiler-introduced
      // variable, possibly with PRIM_MOVE tmp, PRIM_ADDR_OF sym
      if (call->isPrimitive(PRIM_ADDR_OF) ||
          call->isPrimitive(PRIM_SET_REFERENCE) ||
          call->isPrimitive(PRIM_GET_MEMBER) ||
          call->isPrimitive(PRIM_GET_SVEC_MEMBER))
        call = toCallExpr(call->parentExpr);

      if (call->isPrimitive(PRIM_MOVE)) {
        SymExpr* lhs = toSymExpr(call->get(1));
        Symbol* lhsSymbol = lhs->symbol();

        if (lhsSymbol->hasFlag(FLAG_REF_VAR)) {
          // intended to handle 'const ref'
          // it would be an error to reach this point if it is not const
          INT_ASSERT(lhsSymbol->hasFlag(FLAG_CONST));
          return true;
        }

        if (lhs != use &&
            lhsSymbol->isRef() &&
            symbolIsUsedAsConstRef(lhsSymbol))
          return true;
      }
    }
  }
  return false;
}

static
bool symbolIsUsedAsConstRef(Symbol* sym) {
  for_SymbolSymExprs(se, sym) {
    if (symExprIsUsedAsConstRef(se)) {
      return true;
    }
  }
  return false;
}

static
bool symExprIsSet(SymExpr* se)
{
  // The ref is necessary if it is for an explicit ref var
  if (se->symbol()->hasFlag(FLAG_REF_VAR)) {
    return true;
  }

  // a ref is not necessary if the LHS is a value
  // (this can come up in recursive handling of a PRIM_MOVE).
  if (!se->symbol()->isRef())
    return false;

  // A ref is not necessary if we've already determined
  // that the ref symbol is `const` for some reason.
  if (se->symbol()->qualType().isConst())
    return false;

  int defOrUse = isDefAndOrUse(se);
  if (defOrUse & 1) { // def
    if (symExprIsSetByDef(se))
      return true;
  }
  if (defOrUse & 2) { // use
    if (symExprIsSetByUse(se))
      return true;
  }

  return false;
}

static
bool callSetsSymbol(Symbol* sym, CallExpr* call)
{
  for_alist(expr, call->argList) {
    if (SymExpr* se = toSymExpr(expr)) {
      if (se->symbol() == sym) {
        if (symExprIsSet(se)) {
          return true;
        }
      }
    }
  }
  return false;
}

// If 'expr' is the outer var of a shadow var SV and
// SV has the ref intent, return SV.
static ShadowVarSymbol* refShadowVarForOuterVarExpr(Expr* expr) {
  if (isOuterVarOfShadowVar(expr))
    if (ShadowVarSymbol* svar = toShadowVarSymbol(expr->parentSymbol))
      if (svar->intent == TFI_REF)
        return svar;
  return NULL;
}

// Would choosing the refCall or valueCall change our determination
// about sym? If so, this function returns true.
static
bool contextCallItDepends(Symbol* sym, ContextCallExpr* cc) {
  CallExpr* refCall = NULL;
  CallExpr* valueCall = NULL;
  CallExpr* constRefCall = NULL;

  cc->getCalls(refCall, valueCall, constRefCall);

  bool ref = refCall?callSetsSymbol(sym, refCall):false;
  bool val = valueCall?callSetsSymbol(sym, valueCall):false;
  bool cref = constRefCall?callSetsSymbol(sym, constRefCall):false;

  if (ref != val || ref != cref) {
    return true;
  }

  return false;
}

// TODO
// tuples
//   Symbol* (ArgSymbol or VarSymbol)
//   Vec<int> indexes
//
// index 0 means the entire tuple / svec?
// Or linearize tuple?

// index (1)(2) -> linearize into size1*1 + 2
// index 0 means whole tuple
//
//  Symbol*
//    -> std::vector<bool> of linearized indices
//    -> tracks if that tuple element is const

//typedef std::map<Symbol*, std::vector<bool> > tupleElementConst_t;
struct GraphNode {
  Symbol* variable; // a VarSymbol or ArgSymbol;
  int     fieldIndex; // 0 for "any field" / not a tuple
};

static
GraphNode makeNode(Symbol* variable, int fieldIndex)
{
  GraphNode ret;
  ret.variable = variable;
  if (variable->getValType()->symbol->hasFlag(FLAG_TUPLE))
    ret.fieldIndex = fieldIndex;
  else
    ret.fieldIndex = 0;
  return ret;
}

static
bool isConst(GraphNode node)
{
  if (node.variable->fieldQualifiers) {
    Qualifier fieldQualifier = node.variable->fieldQualifiers[node.fieldIndex];
    return QualifiedType::qualifierIsConst(fieldQualifier);
  }

  if (node.fieldIndex == 0)
    return node.variable->qualType().isConst();

  return false;
}

static
bool operator<(const GraphNode & a, const GraphNode & b)
{
  return (a.variable->id < b.variable->id) ||
         (a.variable->id == b.variable->id && a.fieldIndex < b.fieldIndex);
}

// Now use element_s instead of Symbol* below
typedef std::map<GraphNode, std::vector<GraphNode> > revisitGraph_t;
typedef std::set<GraphNode> revisitUnknowns_t;

static
void addDependency(revisitGraph_t & graph, GraphNode from, GraphNode to)
{
  DEBUG_SYMBOL(from.variable);
  if (shouldTrace(from.variable)) {
    printf("Adding a dependency from %i,%i to %i,%i\n",
           from.variable->id, from.fieldIndex,
           to.variable->id, to.fieldIndex);
  }


  graph[from].push_back(to);
}

static
void createFieldQualifiersIfNeeded(Symbol* sym)
{
  AggregateType* at = toAggregateType(sym->getValType());
  if (at && !sym->fieldQualifiers) {
    int numFields = at->numFields();
    sym->fieldQualifiers = new Qualifier[numFields+1]; // +1 for 1-base
    Qualifier totalQ = QUAL_UNKNOWN;
    if (sym->isRef()) {
      if (sym->isConstant())
        totalQ = QUAL_CONST_REF;
      else
        totalQ = QUAL_REF;
    }
    sym->fieldQualifiers[0] = totalQ;
    int i = 1;
    for_fields(field, at) {
      Qualifier q = QUAL_UNKNOWN;
      if (field->isRef()) {
        if (field->isConstant())
          q = QUAL_CONST_REF;
        else
          q = QUAL_REF;
      }
      sym->fieldQualifiers[i] = q;
      i++;
    }
  }
}

static
bool containsReferenceFields(AggregateType* at)
{
  // This is an optimization for now, since only
  // tuple types can contain reference fields.
  if (! at->symbol->hasFlag(FLAG_TUPLE))
    return false;

  // Check if any fields are reference fields.
  for_fields(field, at) {
    if (field->isRef())
      return true;
  }

  return false;
}

static
void markSymbolConst(Symbol* sym)
{
  ArgSymbol* arg = toArgSymbol(sym);
  // it is const, mark it so
  sym->qual = QualifiedType::qualifierToConst(sym->qual);
  if (arg && arg->intent == INTENT_REF_MAYBE_CONST)
    arg->intent = INTENT_CONST_REF;
}
static
void markSymbolNotConst(Symbol* sym)
{
  ArgSymbol* arg = toArgSymbol(sym);

  // This could set sym->qual to e.g. QUAL_REF
  // but currently QUAL_REF is used both for
  // ref-with-unknown-constness and ref-not-const,
  // so we can just leave it alone.
  INT_ASSERT(!sym->qualType().isConst());
  if (arg && arg->intent == INTENT_REF_MAYBE_CONST)
    arg->intent = INTENT_REF;
}

static
void markConst(GraphNode node)
{
  if (shouldTrace(node.variable)) {
    printf(" const %i,%i\n", node.variable->id, node.fieldIndex);
    DEBUG_SYMBOL(node.variable);
  }

  Symbol* sym = node.variable;
  int fieldIndex = node.fieldIndex;
  AggregateType* at = toAggregateType(node.variable->getValType());

  if (at && containsReferenceFields(at)) {
    createFieldQualifiersIfNeeded(sym);

    if (fieldIndex == 0) {
      // mark all fields
      int nFields = at->numFields();
      for (int i = 0; i <= nFields; i++) {
        if (sym->fieldQualifiers[i] == QUAL_REF)
          sym->fieldQualifiers[i] = QUAL_CONST_REF;
      }
      markSymbolConst(sym);
    } else {
      // mark only fieldIndex
      int i = node.fieldIndex;
      if (sym->fieldQualifiers[i] == QUAL_REF)
        sym->fieldQualifiers[i] = QUAL_CONST_REF;
    }
  } else {
    markSymbolConst(sym);
  }
}
static
void markNotConst(GraphNode node)
{
  if (shouldTrace(node.variable)) {
    printf(" not const %i,%i\n", node.variable->id, node.fieldIndex);
    DEBUG_SYMBOL(node.variable);
  }

  Symbol* sym = node.variable;
  int fieldIndex = node.fieldIndex;
  AggregateType* at = toAggregateType(node.variable->getValType());

  if (at && containsReferenceFields(at)) {
    createFieldQualifiersIfNeeded(sym);

    if (fieldIndex == 0) {
      // mark all fields
      int nFields = at->numFields();
      for (int i = 0; i <= nFields; i++) {
        INT_ASSERT(sym->fieldQualifiers[i] != QUAL_CONST_REF);
      }
    } else {
      // mark only fieldIndex
      int i = node.fieldIndex;
      INT_ASSERT(sym->fieldQualifiers[i] != QUAL_CONST_REF);
    }
  } else {
    markSymbolNotConst(sym);
  }
}


/* Given a sym that we just decided should be considered non-const,
   propagate that information through the graph of dependent
   symbols.
 */
static
void transitivelyMarkNotConst(GraphNode node, /* sym, index */
                              revisitGraph_t & graph,
                              revisitUnknowns_t & unknownConstSyms,
                              std::map<BaseAST*, BaseAST*> & reasonNotConst)
{
  std::vector<GraphNode> & edges = graph[node];

  for( size_t i = 0; i < edges.size(); i++ ) {
    GraphNode otherNode = edges[i];
    if (unknownConstSyms.count(otherNode) != 0) {
      // otherNode still has unknown const-ness
      // mark it as not-const
      markNotConst(otherNode);
      reasonNotConst[otherNode.variable] = node.variable;
      unknownConstSyms.erase(otherNode);
      transitivelyMarkNotConst(otherNode, graph, unknownConstSyms, reasonNotConst);
    }
  }
}

static bool considerAllowRefCall(CallExpr* move, FnSymbol* calledFn) {
  if (! calledFn->hasFlag(FLAG_ALLOW_REF) )
    return true;

  // Also allow _build_tuple_always_allow_ref into user index vars, ex.
  //   forall tup in zip(A,B) ...
  //   functions/ferguson/ref-pair/iterating-over-arrays.chpl
  Symbol* lhs = toSymExpr(move->get(1))->symbol();
  if (lhs->hasFlag(FLAG_INDEX_VAR) && !lhs->hasFlag(FLAG_TEMP))
    return true;

  // workaround for compiler-introduced
  // _build_tuple_always_allow_ref calls
  return false;
}

static
bool isRefOrTupleWithRef(Symbol* index, int tupleElement)
{
  if (index->isRef()) return true;

  if (tupleElement > 0 &&
      index->type->symbol->hasFlag(FLAG_TUPLE)) {
    AggregateType* at = toAggregateType(index->type);
    Symbol* field = at->getField(tupleElement);
    if (field->isRef())
      return true;
  }

  return false;
}

//
// This function adjusts calls to functions that have both
// a ref and non-ref version. These calls are represented with
// a ContextCallExpr. Whether to use the ref or value/const-ref
// version is determined based on how the result of the call is used.
//
// This pass studies all of these call-sites and determines if the
// "by-reference" version is "necessary".  If it is not, then the
// compiler attempts to switch in the "by-value" version. This requires
// the insertion of a temporary value near the call site.
//
// The compiler must also determine whether the "by value" implementation
// will have inserted an autoCopy.   If so then the compiler attaches
// the AUTO_COPY/AUTO_DESTROY flags as necessary to enable the
// callDestructors pass to operate correctly.
//
// This pass also concerns itself with lowering ArgSymbols with
// INTENT_REF_MAYBE_CONST to INTENT_REF or INTENT_CONST_REF, depending
// on whether the formal was "set" in the body of the function.
//
void cullOverReferences() {

  /* A note about the structure of this pass:

     While it's interprocedural, it shouldn't present a big problem for
     separate compilation. The reason is that functions compiled into
     a library should know whether or not they are setting their arguments.
     Combining that fact with the plan to make the compiler produce a
     "header file" in addition to "implementation files" as part of a library,
     there is no problem.

     (However, any interface describing a callable function / call-back
      function will need to have some other default -- possibly blank
      argument intent leads to compilation error for ref-if-modified types
      like arrays).

     Second, this pass works in a uses-to-symbols manner and is an
     interprocedural analysis. However, that does not mean that it can't
     fit into a Pass Manager concept. In particular, it could be
     what LLVM calls a CallGraphSCCPass.
  */


  // Determine const-ness of two types of reference symbols:
  //  1) the symbol storing the ref/const ref result of a ref-pair invocation
  //     (aka the result of a ContextCallExpr).
  //  2) ArgSymbols with intent INTENT_REF_MAYBE_CONST

  // Also, adjust ContextCallExprs including the
  // case when the result is not saved.

  // First, collect Symbols that we need to determine the const-ness of
  std::vector<GraphNode> collectedSymbols;

  revisitUnknowns_t unknownConstSyms;

  std::map<BaseAST*, BaseAST*> reasonNotConst;

  revisitGraph_t revisitGraph;

  // forward-flow constness for FLAG_REF_TO_CONST_WHEN_CONST_THIS
  forv_Vec(FnSymbol, fn, gFnSymbols) {
    if (fn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS)) {
      for_SymbolSymExprs(se, fn) {
        if (CallExpr* call = toCallExpr(se->parentExpr)) {
          if (fn == call->resolvedFunction()) {
            SymExpr* thisActual   = toSymExpr(call->get(1));
            Symbol*  actualSymbol = thisActual->symbol();

            if (CallExpr* parentCall = toCallExpr(call->parentExpr)) {
              if (parentCall->isPrimitive(PRIM_MOVE)) {
                SymExpr* lhs    = toSymExpr(parentCall->get(1));
                Symbol*  lhsSym = lhs->symbol();

                if (actualSymbol->qualType().isConst()) {
                  markSymbolConst(lhsSym);
                }
              }
            }
          }
        }
      }
    }
  }
  // forward-flow constness for getting refs to tuples
  // this would be handled by a FLAG_REF_TO_CONST_WHEN_CONST_THIS accessor
  // but for whatever reason the compiler doesn't always use that for tuples.
  forv_Vec(CallExpr, call, gCallExprs) {
    if (call->isPrimitive(PRIM_GET_MEMBER)) {
      CallExpr* move = toCallExpr(call->parentExpr);
      if (move->isPrimitive(PRIM_MOVE)) {
        SymExpr* aggregateSe = toSymExpr(call->get(1));
        SymExpr* lhsSe = toSymExpr(move->get(1));
        AggregateType* at = toAggregateType(aggregateSe->getValType());
        // note, at might be NULL for unmanaged SomeClass
        if (at && !at->isClass() && aggregateSe->symbol()->qualType().isConst())
            markSymbolConst(lhsSe->symbol());
      }
    }
  }


  // Determine const-ness of ArgSymbols with INTENT_REF_MAYBE_CONST
  forv_Vec(ArgSymbol, arg, gArgSymbols) {
    DEBUG_SYMBOL(arg);

    // Don't try to delve into _build_tuple
    if (arg->defPoint->parentSymbol->hasFlag(FLAG_BUILD_TUPLE)) {
      continue;
    }

    if (arg->defPoint->parentSymbol->hasFlag(FLAG_TUPLE_CAST_FN)) {
      continue;
    }

    AggregateType* argAt = toAggregateType(arg->getValType());
    if (argAt && argAt->symbol->hasFlag(FLAG_TUPLE) &&
        containsReferenceFields(argAt)) {
      AggregateType* tupleType  = toAggregateType(arg->type);
      int            fieldIndex = 1;

      for_fields(field, tupleType) {
        if (field->isRef() ||
            field->type->symbol->hasFlag(FLAG_TUPLE)) {
          collectedSymbols.push_back(makeNode(arg,fieldIndex));
        }

        fieldIndex++;
      }
    } else if (arg->intent == INTENT_REF_MAYBE_CONST) {
      collectedSymbols.push_back(makeNode(arg,0));
    }
  }

  // Determine const-ness of the results of a ContextCallExpr
  forv_Vec(ContextCallExpr, cc, gContextCallExprs) {
    CallExpr* move = toCallExpr(cc->parentExpr);
    INT_ASSERT(move && move->isPrimitive(PRIM_MOVE));

    SymExpr* lhs = toSymExpr(move->get(1));

    INT_ASSERT(lhs);

    // Lower certain degenerate cases
    if (firstPassLowerContextCall(cc)) {
      // ContextCall removed, no further action required on it
    } else {
      // ContextCall needs more analysis, do this in 2nd step
      collectedSymbols.push_back(makeNode(lhs->symbol(), 0));
    }
  }

  // Next, determine the const-ness of the collectedSymbols.
  // Do this by looking at SymExprs referring to that symbol.

  // There are three cases for a given SymExpr:
  //  1) it's a write (ie def)
  //  2) it's a read (ie use)
  //  3) we can't tell yet because it depends on another Symbol's constness.
  //     This could be due to recursion with INTENT_REF_MAYBE_CONST ArgSymbols
  //     or due to nested calls with ContextCallExpr.

  // We will set Qualifier or Intent to mark symbols const,
  // except for tuple elements, we'll update constTupleElements.
  //
  // Note: code in this loop can append to collectedSymbols.
  // TODO: it could visit the same Symbol more than once
  //       i.e. the same Symbol could be added to collectedSymbols multiple
  //       times
  // TODO: I think it would be nicer to structure this pass explicitly
  //       as function-by-function followed by an interprocedural portion.
  //       That might make the interactions clearer, but it would probably
  //       be more complex code. But faster.
  //       E.g. we could process build_tuple calls and consider the
  //       arguments of interest at once, instead of looking through
  //       build_tuple calls for each argument of interest as below.
  //
  for(size_t i = 0; i < collectedSymbols.size(); i++) {

    GraphNode node = collectedSymbols[i];
    Symbol* sym = node.variable;

    DEBUG_SYMBOL(sym);

    AggregateType* symAt = toAggregateType(sym->getValType());
    bool symHasRefFields = symAt && containsReferenceFields(symAt);

    // If we already determined that a symbol is const, no need to
    // do additional work here.
    if (sym->qualType().isConst() && !symHasRefFields)
      continue;

    // If it's a tuple, create the field qualifiers if needed
    if (symHasRefFields) {
      createFieldQualifiersIfNeeded(sym);

      // If it has ref fields, and all of them are const, do no more here
      bool allConst = (sym->fieldQualifiers[0] != QUAL_REF);
      int i = 1;
      for_fields(field, symAt) {
        if (field->isRef()) {
          if (sym->fieldQualifiers[i] == QUAL_REF)
            allConst = false;
        }
        i++;
      }
      if (allConst)
        continue;
    }

    bool setter = false;
    bool revisit = false;

    for_SymbolSymExprs(se, sym) {

      // Check several cases that might require other information
      // to resolve. These can be added to the revisitGraph.
      if (CallExpr* call = toCallExpr(se->parentExpr)) {

        // Case: loop.
        bool foundLoop = false;
        bool isForall = false;
        IteratorDetails leaderDetails;
        ForLoop* followerForLoop = NULL;
        std::vector<IteratorDetails> detailsVector;

        if (ForallStmt* pfs = isForallIterExpr(call)) {

          gatherLoopDetails(pfs, isForall, leaderDetails,
                            followerForLoop, detailsVector);
          foundLoop = true;
        }

        // Otherwise, look for a ForLoop / old style forall

        // Check if sym is iterated over. In that case, what's the
        // index variable?
        //
        // It's important that this case run before the check
        // for build_tuple.
        else {
          // Find enclosing PRIM_MOVE
          CallExpr* move = toCallExpr(se->parentExpr->getStmtExpr());
          if (!move->isPrimitive(PRIM_MOVE))
            move = NULL;

          if (move != NULL) {
            // Now, LHS of PRIM_MOVE is iterator variable
            SymExpr* lhs = toSymExpr(move->get(1));
            Symbol* iterator = lhs->symbol();
            ForLoop* forLoop = NULL;
            ForallStmt*   fs = NULL;
            // Todo expand isChplIterOrLoopIterator to watch for ForallStmt?

            // marked with chpl__iter or with type iterator class?
            if (isChplIterOrLoopIterator(iterator, forLoop)) {

              // Scroll through exprs until we find ForLoop

              if (!forLoop) {
                Expr* e = move;
                while (e) {
                  if ( (forLoop = toForLoop(e)) )
                    break;
                  if ( (fs = toForallStmt(e)) )
                    break;
                  e = e->next;
                }
              }

              if (forLoop) {
                // Gather the loop details to understand the
                // correspondence between what was iterated over
                // and the index variables.

                /*
                printf("print working on node %i %i\n",
                       node.variable->id, node.fieldIndex);

                printf("for iterator %i\n", iterator->id);
                */

                gatherLoopDetails(forLoop, isForall, leaderDetails,
                                  followerForLoop, detailsVector);
                foundLoop = true;
              }
              else if (fs) {
                // Ditto if it is a ForallStmt.

                gatherLoopDetails(fs, isForall, leaderDetails,
                                  followerForLoop, detailsVector);
                foundLoop = true;
              }
            }
          }
        }

        if (foundLoop) {
                bool handled = false;

                for (size_t i = 0; i < detailsVector.size(); i++) {
                  bool iteratorYieldsConstWhenConstThis = false;

                  Expr* iterable = detailsVector[i].iterable;
                  int iterableTupleElement = detailsVector[i].iterableTupleElement;
                  Symbol* index = detailsVector[i].index;
                  int indexTupleElement = detailsVector[i].indexTupleElement;
                  FnSymbol* iteratorFn  = detailsVector[i].iterator;
                  SymExpr* iterableSe = toSymExpr(iterable);

                  // Also check if we are iterating using these() method
                  // ex. functions/ferguson/ref-pair/const-error-iterated*
                  if (!iterableSe)
                    if (CallExpr* iterableCall = toCallExpr(iterable))
                      if (iterableCall->isNamed("these"))
                        iterableSe = toSymExpr(iterableCall->get(1));

                  /*
                  printf("  i %i\n", (int) i);
                  printf("  iterable %i %i\n", iterableSe->symbol()->id, iterableTupleElement);
                  printf("  index %i %i\n", index->id, indexTupleElement);
                   */


                  // In the future this could be based upon ref-pair
                  // iterators.
                  // For now, the compiler makes this adjustment for
                  // any iterator methods on array implementation classes.
                  // The goal here is that iterating over an array
                  // and modifying the index variable should make us
                  // consider the array to be "set".
                  if (iteratorFn->isMethod() &&
                      (isArrayClass(iteratorFn->getFormal(1)->type) ||
                       iteratorFn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS)))
                      iteratorYieldsConstWhenConstThis = true;

                  // Note, if we wanted to use the return intent
                  // of the iterator, it is overwritten in protoIteratorClass.

                  // This flag should be set for array iteration
                  if (iterableSe &&
                      iterableSe->symbol() == sym &&
                      (iterableTupleElement == 0 || iterableTupleElement ==
                        node.fieldIndex) &&
                      iteratorYieldsConstWhenConstThis &&
                      index &&
                      isRefOrTupleWithRef(index, indexTupleElement)) {
                    // Now the const-ness of the array depends
                    // on whether or not the yielded value is set

                    GraphNode srcNode = makeNode(index, indexTupleElement);
                    collectedSymbols.push_back(srcNode);
                    revisit = true;
                    addDependency(revisitGraph, srcNode, node);
                    handled = true;
                  }
                }

                if (handled)
                  continue; // continue outer loop
        }

        // Case: 'call' invokes build_tuple().
        if (FnSymbol* calledFn = call->resolvedFunction()) {
          if (calledFn->hasFlag(FLAG_BUILD_TUPLE)) {
            if (CallExpr* move = toCallExpr(call->parentExpr)) {
              if (move->isPrimitive(PRIM_MOVE) &&
                  considerAllowRefCall(move, calledFn)) {
                SymExpr* lhs       = toSymExpr(move->get(1));
                Symbol*  lhsSymbol = lhs->symbol();
                int      j         = 1;

                for_actuals(actual, call) {
                  if (se == actual) {
                    break;
                  }

                  j++;
                }

                INT_ASSERT(1 <= j && j <= call->numActuals());

                // What is the field we are interested in?
                Symbol*        tupleField = NULL;
                AggregateType* tupleType  = toAggregateType(lhsSymbol->type);
                int            k          = 1;

                for_fields(field, tupleType) {
                  if (j == k) {
                    tupleField = field;
                  }

                  k++;
                }

                // Does the tuple store the field by reference?
                if (tupleField->isRef()) {
                  GraphNode srcNode = makeNode(lhsSymbol, j);

                  collectedSymbols.push_back(srcNode);

                  revisit = true;

                  addDependency(revisitGraph, srcNode, node);

                  //DEBUG_SYMBOL(lhsSymbol);
                  continue;
                }
              }
            }
          }
        }

        // Case: ContextCallExpr.
        // check for the case that sym is passed a ContextCall
        // and the determination depends on which branch is chosen.
        if (ContextCallExpr* cc = toContextCallExpr(call->parentExpr)) {
          if (contextCallItDepends(sym, cc/*, ignoredDefs*/)) {
            // since lhs->symbol() is the result of a move from
            // a ContextCallExpr, it will already be in the list
            // of collectedSymbols.
            //
            // TODO: This isn't quite right, since the called ref-pair
            // could still use a ref-if-modified argument by ref in
            // both branches... or the ref-return version could
            // be const ref on the actual, while the const-ref-return
            // version is ref on the actual.
            CallExpr* move      = toCallExpr(cc->parentExpr);
            SymExpr*  lhs       = toSymExpr(move->get(1));
            Symbol*   lhsSymbol = lhs->symbol();

            revisit = true;

            // Make a note that determining how lhs->symbol()
            // is used (is it const or not?) will allow us to
            // resolve this ContextCallExpr.
            GraphNode srcNode = makeNode(lhsSymbol, node.fieldIndex);

            addDependency(revisitGraph, srcNode, node);

            continue; // move on to the next SymExpr
          }
        }

        // Case: tuple cast fn or accessor-like fn or const?-ref formal.
        if (FnSymbol* calledFn = call->resolvedFunction()) {
          ArgSymbol* formal = actual_to_formal(se);

          // Check for the case that sym is in a call
          // to a tuple cast function. We consider it
          // const if the result of the tuple cast is const.
          if (calledFn->hasFlag(FLAG_TUPLE_CAST_FN)) {
            CallExpr* move = toCallExpr(call->parentExpr);

            if (move && move->isPrimitive(PRIM_MOVE)) {
              SymExpr*  lhs       = toSymExpr(move->get(1));
              Symbol*   lhsSymbol = lhs->symbol();
              GraphNode srcNode   = makeNode(lhsSymbol, node.fieldIndex);

              collectedSymbols.push_back(srcNode);

              revisit = true;

              addDependency(revisitGraph, srcNode, node);

              continue; // move on to the next SymExpr
            }
          }

          // Check for the case that sym is the _this
          // actual for a function marked with the flag
          // FLAG_REF_TO_CONST_WHEN_CONST_THIS
          // (which is used for field accessors among other things).
          // In that event, it depends on how the returned
          // value is used.
          if (calledFn->hasFlag(FLAG_REF_TO_CONST_WHEN_CONST_THIS) &&
              formal->hasFlag(FLAG_ARG_THIS)) {
            CallExpr* move = toCallExpr(call->parentExpr);

            if (move && move->isPrimitive(PRIM_MOVE)) {
              SymExpr* lhs       = toSymExpr(move->get(1));
              Symbol*  lhsSymbol = lhs->symbol();

              if (lhsSymbol->isRef() && lhsSymbol != sym) {
                GraphNode srcNode = makeNode(lhsSymbol, node.fieldIndex);

                collectedSymbols.push_back(srcNode);

                revisit = true;

                addDependency(revisitGraph, srcNode, node);

                continue; // move on to the next SymExpr
              }
            }
          }


          // Check for the case that sym is passed to an
          // array formal with blank intent. In that case,
          // it depends on the determination of the called function.
          if (formal->intent == INTENT_REF_MAYBE_CONST &&
              !calledFn->hasFlag(FLAG_BUILD_TUPLE)) {
            // since it has INTENT_REF_MAYBE_CONST, it will
            // already be in the list of collectedSymbols.
            revisit = true;

            // Make a note that determining how formal
            // is used (const or not?) will allow us to resolve
            // this Symbol's const-ness.
            GraphNode srcNode = makeNode(formal, node.fieldIndex);

            addDependency(revisitGraph, srcNode, node);

            continue; // move on to the next SymExpr
          }

        }

        // Case: extracting a reference field.
        // from a tuple into another Symbol
        if (call->isPrimitive(PRIM_GET_MEMBER_VALUE))
          if (CallExpr* parentCall = toCallExpr(call->parentExpr))
            if (parentCall->isPrimitive(PRIM_MOVE)) {
              SymExpr* lhs       = toSymExpr(parentCall->get(1));
              Symbol*  lhsSymbol = lhs->symbol();

              if (lhsSymbol->isRef()) {
                SymExpr*       fieldSe    = toSymExpr(call->get(2));
                Symbol*        field      = fieldSe->symbol();
                AggregateType* tupleType  =
                  toAggregateType(call->get(1)->getValType());
                int            fieldIndex = 1;

                for_fields(curField, tupleType) {
                  if (curField == field) {
                    break;
                  }

                  fieldIndex++;
                }

                INT_ASSERT(fieldIndex <= tupleType->numFields());

                // ignore if the field set isn't
                // the current field.
                if (node.fieldIndex == 0 || node.fieldIndex == fieldIndex) {
                  // add a dependency in the graph. Knowing
                  // if lhsSymbol is set will tell us if
                  // sym tuple element i is set.
                  GraphNode srcNode = makeNode(lhsSymbol, 0);

                  collectedSymbols.push_back(srcNode);

                  revisit = true;

                  addDependency(revisitGraph,
                                srcNode,
                                makeNode(sym, fieldIndex));

                  continue;
                }
              }
            }

        // Case: creating a temporary from one tuple to another tuple.
        if (call->isPrimitive(PRIM_SET_MEMBER)) {
          SymExpr* base       = toSymExpr(call->get(1));
          Symbol*  baseSymbol = base->symbol();

          SymExpr* rhs        = toSymExpr(call->get(3));
          Symbol*  rhsSymbol  = rhs->symbol();

          if (rhsSymbol == sym) {
            SymExpr*       fieldSe    = toSymExpr(call->get(2));
            Symbol*        field      = fieldSe->symbol();
            AggregateType* tupleType  = toAggregateType(base->getValType());
            int            fieldIndex = 1;

            for_fields(curField, tupleType) {
              if (curField == field) {
                break;
              }

              fieldIndex++;
            }

            INT_ASSERT(fieldIndex <= tupleType->numFields());

            // ignore if the field set isn't
            // the current field.
            if (node.fieldIndex == 0 || node.fieldIndex == fieldIndex) {
              // add a dependency in the graph. Knowing if the
              // tuple field is set will tell us if the
              // RHS is set.
              GraphNode srcNode = makeNode(baseSymbol, fieldIndex);

              collectedSymbols.push_back(srcNode);

              revisit = true;

              addDependency(revisitGraph,
                            srcNode,
                            makeNode(rhsSymbol, 0));

              continue;
            }
          }
        }

        // Check for the case of extracting a star tuple field?

        // Case: sym is moved to a compiler temp, ex.
        //   PRIM_MOVE tmp, PRIM_ADDR_OF sym
        CallExpr* call2 = call;
        if (call->isPrimitive(PRIM_ADDR_OF) ||
            call->isPrimitive(PRIM_SET_REFERENCE) ||
            call->isPrimitive(PRIM_GET_MEMBER) ||
            call->isPrimitive(PRIM_GET_SVEC_MEMBER))
          call2 = toCallExpr(call->parentExpr);

        if (call2->isPrimitive(PRIM_MOVE)) {
          SymExpr* lhs       = toSymExpr(call2->get(1));
          Symbol*  lhsSymbol = lhs->symbol();

          if (lhsSymbol != sym &&
              isRefOrTupleWithRef(lhsSymbol, node.fieldIndex)) {
            GraphNode srcNode = makeNode(lhsSymbol, node.fieldIndex);

            collectedSymbols.push_back(srcNode);

            revisit = true;

            addDependency(revisitGraph, srcNode, node);

            continue; // move on to the next SymExpr
          }
        }
      }  // if (CallExpr* call = toCallExpr(se->parentExpr))

      // Case: sym is the outer variable in a forall intent.
      if (ShadowVarSymbol* svar = refShadowVarForOuterVarExpr(se)) {
        GraphNode srcNode = makeNode(svar, node.fieldIndex);
        collectedSymbols.push_back(srcNode);
        addDependency(revisitGraph, srcNode, node);
        revisit = true;
        continue; // move on to the next SymExpr
      }

      // Determine if se represents a "setting" or a "getting" mention of sym
      if (!setter && symExprIsSet(se)) {
        setter              = true;
        reasonNotConst[sym] = se;

        if (CallExpr* call = toCallExpr(se->parentExpr)) {
          if (call->isResolved()) {
            ArgSymbol* formal = actual_to_formal(se);

            reasonNotConst[se] = formal;
          }
        }
      }
    }  // for_SymbolSymExprs(se, sym)

    if (revisit) {
      if (setter) {
        // We decided to revisit this Symbol, but separate uses
        // determined it to be not const. So don't revisit it.
        revisit = false;
      } else {
        // We might still decide to use setter.
        unknownConstSyms.insert(node);
        if (shouldTrace(node.variable)) {
          printf("Adding to unknownConstSyms %i,%i\n",
                 node.variable->id, node.fieldIndex);
        }
      }
    }

    if (!revisit) {
      if (setter) {
        // it's not CONST & it shouldn't be CONST
        markNotConst(node);
      } else {
        // it is const, mark it so
        markConst(node);
      }
    }
  }  // for(size_t i = 0; i < collectedSymbols.size(); i++)

  // Handle the graph of revisits
  // Note this could be a cyclic graph when there are recursive
  // functions.

  // This algorithm could be naturally represented in terms of
  // Strongly Connected Components analysis, but for now it's
  // just doing it manually.
  {
    // First, propagate non-const-ness through the graph
    for (revisitGraph_t::iterator it = revisitGraph.begin();
         it != revisitGraph.end();
         ++it) {

      GraphNode node = it->first;
      Symbol* sym = node.variable;

      DEBUG_SYMBOL(sym);

      if (unknownConstSyms.count(node) == 0) {
        if (!isConst(node)) {
          // If sym has known const-ness, and it's a setter,
          // propagate that information in the graph.
          transitivelyMarkNotConst(node, revisitGraph, unknownConstSyms,
              reasonNotConst);
        }
      }
    }

    // Next, mark any remaining symbols with unknown const-ness
    // as const, since they are never set
    // (this accounts for cycles possibly due to recursive functions
    //  with blank-intent array formals)
    for (revisitUnknowns_t::iterator it = unknownConstSyms.begin();
         it != unknownConstSyms.end();
         ++it) {

      GraphNode node = *it;
      Symbol* sym = node.variable;

      // Anything we didn't remove from unknownConstSyms must be
      // in a cycle of const-ness

      DEBUG_SYMBOL(sym);

      markConst(node);
    }
  }

  // Now, lower ContextCalls
  forv_Vec(ContextCallExpr, cc, gContextCallExprs) {
    // Some ContextCallExprs have already been removed above
    if (!cc->inTree())
      continue;

    CallExpr* move = toCallExpr(cc->parentExpr);

    bool notConst = false;
    Symbol* lhsSymbol = NULL;

    if (move) {
      SymExpr* lhs = toSymExpr(move->get(1));
      lhsSymbol = lhs->symbol();
      Qualifier qual = lhsSymbol->qualType().getQual();

      if (qual == QUAL_REF)
        notConst = true;
    }

    lowerContextCallComputeConstRef(cc, notConst, lhsSymbol);
  }

  // We already changed INTENT_REF_MAYBE_CONST in
  // markConst / markNotConst so there is nothing else to do
  // here for ArgSymbols.

  lateConstCheck(&reasonNotConst);
}

// Handle certain degenerate cases, such as when a
// ContextCallExpr is not in a PRIM_MOVE.
static
bool firstPassLowerContextCall(ContextCallExpr* cc)
{
  CallExpr* refCall = NULL;
  CallExpr* valueCall = NULL;
  CallExpr* constRefCall = NULL;

  cc->getCalls(refCall, valueCall, constRefCall);

  INT_ASSERT(refCall || valueCall || constRefCall);

  CallExpr* someCall = refCall;
  if (someCall == NULL) someCall = constRefCall;
  if (someCall == NULL) someCall = valueCall;

  FnSymbol* fn = someCall->resolvedFunction();
  INT_ASSERT(fn);

  CallExpr* move = NULL; // set if the call is in a PRIM_MOVE
  SymExpr* lhs = NULL; // lhs if call is in a PRIM_MOVE

  // Decide whether to use the value call or the ref call.
  // Always leave the ref call for iterators.
  // (It would be an improvement to choose the appropriate one
  //  based upon how the iterator is used, but such a feature
  //  would require specific support for iterators since yielding
  //  is not the same as returning.)
  move = toCallExpr(cc->parentExpr);
  if (fn->isIterator()) {
    lowerContextCallPreferRefConstRef(cc);
    return true;
  } else if (move) {
    INT_ASSERT(move->isPrimitive(PRIM_MOVE));
    lhs = toSymExpr(move->get(1));
    INT_ASSERT(lhs);
    // This case is complex and handled later.
    return false;
  } else {
    // e.g. array access in own statement like this:
    //   A(i)
    // should use 'getter'
    // MPF - note 2016-01: this code does not seem to be triggered
    // in the present compiler.
    lowerContextCallPreferConstRefValue(cc);
    return true;
  }
}

static
void lowerContextCallPreferRefConstRef(ContextCallExpr* cc)
{
  CallExpr* refCall = NULL;
  CallExpr* valueCall = NULL;
  CallExpr* constRefCall = NULL;
  choose_type_t which;

  cc->getCalls(refCall, valueCall, constRefCall);

  if (refCall) {
    which = USE_REF;
  } else if(constRefCall) {
    which = USE_CONST_REF;
  } else {
    which = USE_VALUE;
    INT_FATAL("lowering context call with only 1 option");
  }

  lowerContextCall(cc, which);
}

static
void lowerContextCallPreferConstRefValue(ContextCallExpr* cc)
{
  CallExpr* refCall = NULL;
  CallExpr* valueCall = NULL;
  CallExpr* constRefCall = NULL;
  choose_type_t which;

  cc->getCalls(refCall, valueCall, constRefCall);

  if(constRefCall) {
    which = USE_CONST_REF;
  } else if(valueCall) {
    which = USE_VALUE;
  } else {
    which = USE_REF;
    INT_FATAL("lowering context call with only 1 option");
  }

  lowerContextCall(cc, which);
}

static
void lowerContextCallComputeConstRef(ContextCallExpr* cc, bool notConst, Symbol* lhsSymbol)
{
  CallExpr* refCall = NULL;
  CallExpr* valueCall = NULL;
  CallExpr* constRefCall = NULL;
  choose_type_t which = USE_CONST_REF;

  cc->getCalls(refCall, valueCall, constRefCall);

  if (notConst) {
    which = USE_REF;
    // it would be a program error if the ref version didn't exist
  } else {
    // Check: should we use the const-ref or value version?
    // Use value version if it's never passed/returned as const ref
    if (valueCall != NULL && constRefCall != NULL) {
      if (lhsSymbol == NULL || symbolIsUsedAsConstRef(lhsSymbol))
        which = USE_CONST_REF;
      else
        which = USE_VALUE;
    } else {
      // Use whichever value version we have.
      if (constRefCall != NULL)
        which = USE_CONST_REF;
      else
        which = USE_VALUE;
    }
  }

  lowerContextCall(cc, which);
}

static
void lowerContextCall(ContextCallExpr* cc, choose_type_t which)
{
  CallExpr* refCall = NULL;
  CallExpr* valueCall = NULL;
  CallExpr* constRefCall = NULL;

  cc->getCalls(refCall, valueCall, constRefCall);

  // Check that whatever was selected is available.
  if (which == USE_REF)
    INT_ASSERT(refCall != NULL);
  if (which == USE_CONST_REF)
    INT_ASSERT(constRefCall != NULL);
  if (which == USE_VALUE)
    INT_ASSERT(valueCall != NULL);

  CallExpr* someCall = refCall;
  if (someCall == NULL) someCall = constRefCall;
  if (someCall == NULL) someCall = valueCall;

  FnSymbol* fn = someCall->resolvedFunction();
  INT_ASSERT(fn);

  // TODO tidy up below based upon the above assumptions.

  bool useValueCall = (which == USE_VALUE || which == USE_CONST_REF);
  CallExpr* move = NULL; // set if the call is in a PRIM_MOVE
  SymExpr* lhs = NULL; // lhs if call is in a PRIM_MOVE

  // Decide whether to use the value call or the ref call.
  // Always leave the ref call for iterators.
  // (It would be an improvement to choose the appropriate one
  //  based upon how the iterator is used, but such a feature
  //  would require specific support for iterators since yielding
  //  is not the same as returning.)
  move = toCallExpr(cc->parentExpr);
  if (fn->isIterator()) {
    INT_ASSERT(which == USE_REF);
    useValueCall = false;
  } else if (move) {
    lhs = toSymExpr(move->get(1));
    // useValueCall set from useSetter argument to this function
  } else {
    // e.g. array access in own statement like this:
    //   A(i)
    // should use 'getter'
    // MPF - note 2016-01: this code does not seem to be triggered
    // in the present compiler.
    useValueCall = true;
    INT_ASSERT(which == USE_CONST_REF || which == USE_VALUE);
  }

  if (refCall) refCall->remove();
  if (valueCall) valueCall->remove();
  if (constRefCall) constRefCall->remove();

  if (useValueCall) {
    // Use available value or const-ref return version if only
    // one is available
    CallExpr* useCall = valueCall?valueCall:constRefCall;

    // If value version is selected and available, use that
    if (which == USE_VALUE && valueCall)
      useCall = valueCall;
    // If const ref version is selected and available, use that
    if (which == USE_CONST_REF && constRefCall)
      useCall = constRefCall;

    FnSymbol* useFn = useCall->resolvedFunction();
    INT_ASSERT(useFn);

    // Replace the ContextCallExpr with the value/const ref call
    cc->replace(useCall);

    // Adjust the AST around the value call to include
    // a temporary to receive the value.

    // Adjust code to use value return version.
    // The other option is that retTag is RET_CONST_REF,
    // in which case no further adjustment is necessary.
    if (move && useFn->retTag == RET_VALUE) {
      SET_LINENO(move);
      // Generate a value temp to receive the value
      VarSymbol* tmp  = newTemp(useFn->retType);
      move->insertBefore(new DefExpr(tmp));

      if (requiresImplicitDestroy(useCall)) {
        if (typeNeedsCopyInitDeinit(useFn->retType) == false) {
          tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
        } else {
          tmp->addFlag(FLAG_INSERT_AUTO_DESTROY);
        }
      }

      if (lhs && lhs->symbol()->isUsed()) {
        // If the LHS was used, set it to the address of the
        // new temporary (which is the function return value)

        FnSymbol* moveInFn = toFnSymbol(move->parentSymbol);
        INT_ASSERT(moveInFn);
        Symbol* retSymbol = moveInFn->getReturnSymbol();
        // Check: are we adding a return of a local variable ?
        for_SymbolUses(use, lhs->symbol()) {
          if (CallExpr* useCall = toCallExpr(use->parentExpr))
            if (useCall->isPrimitive(PRIM_MOVE))
              if (SymExpr* useCallLHS = toSymExpr(useCall->get(1)))
                if (useCallLHS->symbol() == retSymbol) {
                  USR_FATAL_CONT(move, "illegal expression to return by ref");
                  USR_PRINT(useFn, "called function returns a value not a reference");
                }
        }

        move->insertAfter(new CallExpr(PRIM_MOVE,
                                       lhs->symbol(),
                                       new CallExpr(PRIM_ADDR_OF, tmp)));
      } else {
        // If the LHS was not used,
        // remove the old definition point since we have
        // provided a new one above.
        lhs->symbol()->defPoint->remove();
      }

      // Replace the LHS with our new temporary
      lhs->setSymbol(tmp);
    }

  } else {
    // Replace the ContextCallExpr with the ref call
    if (refCall) cc->replace(refCall);
    else if(constRefCall) cc->replace(constRefCall);
  }
}
