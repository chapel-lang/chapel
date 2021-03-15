/*
 * Copyright 2020-2021 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#include "AstToText.h"

#include "DecoratedClassType.h"
#include "driver.h"
#include "expr.h"
#include "stmt.h"
#include "symbol.h"
#include "IfExpr.h"
#include "LoopExpr.h"

const std::string& AstToText::text() const
{
  return mText;
}

void AstToText::appendNameAndFormals(FnSymbol* fn)
{
  if (fn->instantiatedFrom != nullptr && developer == false)
  {
    appendNameAndFormals(fn->instantiatedFrom);
  }
  else
  {
    appendName(fn);
    appendFormals(fn);
  }
}

/************************************ | *************************************
*                                                                           *
* Append a normalized version of the function's name to the buffer.         *
*                                                                           *
************************************* | ************************************/

void AstToText::appendName(FnSymbol* fn)
{
  if (fn->hasFlag(FLAG_MODULE_INIT))
  {
    INT_ASSERT(strncmp(fn->name, "chpl__init_",      11) == 0);

    mText += "top-level module statements for ";
    mText += (fn->name + 11);
  }

  else if (fn->isMethod() == true)
  {
    appendThisIntent(fn);

    if (!fn->isPrimaryMethod()) {
      appendClassName(fn);
      mText += '.';
    }

    const char* fnName = fn->name;

    if (fn->hasFlag(FLAG_DESTRUCTOR))
      fnName = "deinit";

    mText += fnName;
  }

  else
  {
    mText += fn->name;
  }
}

void AstToText::appendThisIntent(FnSymbol* fn)
{
  if (fn->thisTag == INTENT_REF)
    mText += "ref ";

  else if (fn->thisTag == INTENT_PARAM)
    mText += "param ";

  else if (fn->thisTag == INTENT_TYPE)
    mText += "type ";
}

void AstToText::appendClassName(FnSymbol* fn)
{
  int index = indexForThis(fn);

  if (index > 0)
  {
    DefExpr*   formal = toDefExpr(fn->formals.get(index));
    ArgSymbol* argSym = toArgSymbol(formal->sym);

    if (argSym->typeExpr)
    {
      BlockStmt* bs = argSym->typeExpr;

      if (bs->body.length == 1)
      {
        Expr* expr = bs->body.only();

        if (UnresolvedSymExpr* sel = toUnresolvedSymExpr(expr))
        {
          mText += sel->unresolved;
        }

        else if (SymExpr* sel = toSymExpr(expr))
        {
          if (TypeSymbol* typeSym = toTypeSymbol(sel->symbol()))
          {
            appendExpr(typeSym->name);
          }
          else
          {
            INT_ASSERT(false);
          }
        }

        else
        {
          INT_ASSERT(false);
        }
      }

      else
      {
        INT_ASSERT(false);
      }
    }

    else if (PrimitiveType* type = toPrimitiveType(argSym->type))
      appendExpr(type->symbol->name);

    else if (AggregateType* type = toAggregateType(argSym->type))
      appendExpr(type->symbol->name);

    else
      INT_ASSERT(false);
  }
  else
  {
    INT_ASSERT(false);
  }
}

/************************************ | *************************************
*                                                                           *
* Append a normalized version of the formals list to the buffer.            *
*                                                                           *
************************************* | ************************************/

void AstToText::appendFormals(FnSymbol* fn)
{
  int  count = numFormals(fn);
  bool skip  = skipParens(fn);
  bool first = true;

  if (skip == false)
    mText += '(';

  for (int index = 1; index <= count; index++)
  {
    if (first == true)
    {
      if (skip == true)
        mText += " ";

      first = false;
    }
    else
      mText += ", ";

    appendFormal(fn, index);
  }

  if (skip == false)
    mText += ')';
}

bool AstToText::skipParens(FnSymbol* fn) const
{
  bool retval = false;

  if (fn->hasFlag(FLAG_NO_PARENS))
    retval = true;

  else if (fn->hasFlag(FLAG_MODULE_INIT)      && developer        == false)
    retval = true;

  else
    retval = false;

  return retval;
}

/************************************ | *************************************
*                                                                           *
* Append a normalized version of a formal to the buffer.                    *
*                                                                           *
************************************* | ************************************/

// Excludes "_mt" (method token) and "this" when they are present
int AstToText::numFormals(FnSymbol* fn) const
{
  int retval = fn->formals.length;

  if (fn->isMethod() == true)
    retval = retval - indexForThis(fn);

  return retval;
}

void AstToText::appendFormal(FnSymbol* fn, int oneBasedIndex)
{
  ArgSymbol* arg = formalGet(fn, oneBasedIndex);

  appendFormalIntent(arg);

  appendFormalName(arg);

  appendFormalType(arg);

  appendFormalVariableExpr(arg);

  appendFormalDefault(arg);
}

void AstToText::appendFormalIntent(ArgSymbol* arg)
{
  switch (arg->intent)
  {
    case INTENT_IN:
      mText += "in ";
      break;

    case INTENT_OUT:
      mText += "out ";
      break;

    case INTENT_INOUT:
      mText += "inout ";
      break;

    case INTENT_CONST:
      mText += "const ";
      break;

    case INTENT_CONST_IN:
      mText += "const in ";
      break;

    case INTENT_REF:
      mText += "ref ";
      break;

    case INTENT_CONST_REF:
      mText += "const ref ";
      break;

    case INTENT_REF_MAYBE_CONST:
      mText += "const? ref ";
      break;

    case INTENT_PARAM:
      mText += "param ";
      break;

    case INTENT_TYPE:
      break;

    case INTENT_BLANK:
      break;
  }

  if (arg->hasFlag(FLAG_TYPE_VARIABLE))
    mText += "type ";
}

void AstToText::appendFormalName(ArgSymbol* arg)
{
  mText += arg->name;
}

void AstToText::appendFormalType(ArgSymbol* arg)
{
  if (arg->typeExpr != 0)
  {
    BlockStmt* blockStmt = arg->typeExpr;

    if (blockStmt->length() == 1)
    {
      // Do not print a synthesized typeExpr
      if (typeExprCopiedFromDefaultExpr(arg) == false)
      {
        mText += ": ";
        appendExpr(arg->typeExpr->body.get(1), true);
      }
    }

    else
    {
      if (handleNormalizedTypeOf(blockStmt) == false)
      {
        // NOAKES 2015/02/05  Debugging support.
        // Might become ASSERT in the future or perhaps AST will be
        // altered so that typeExpr is actually an expression
        mText += " AppendType.00";
      }
    }
  }

  else if (arg->type == dtUnknown)
  {

  }

  else if (arg->type == dtAny)
  {

  }

  else if (PrimitiveType* type = toPrimitiveType(arg->type))
  {
    mText += ": ";
    appendExpr(type->symbol->name);
  }

  else if (AggregateType* type = toAggregateType(arg->type))
  {
    mText += ": ";
    appendExpr(type->symbol->name);
  }

  else if (EnumType*      type = toEnumType(arg->type))
  {
    mText += ": ";
    appendExpr(type->symbol->name);
  }

  else
  {
    // NOAKES 2015/02/05  Debugging support.
    // Might become ASSERT in the future
    mText += " AppendType.02";
  }
}

//
// Attempt to determine, heuristically, if normalize.hack_resolve_types()
// has copied defaultExpr to typeExpr. We want to avoid printing this
// synthesizedtypeExpr expression.
//
// The current minimum conditions for this are
//
//     a) typeExpr was nullptr
//     b) the defaultExpr is a blockStmt with 1 stmt
//
// Then it gets tricky. We fall back on a tree-recursion that tries to
// determine if two expression are structurally "equal" in a manner that
// appears to handle the current use cases.
//
//
bool AstToText::typeExprCopiedFromDefaultExpr(ArgSymbol* arg) const
{
  BlockStmt* typeBlock    = arg->typeExpr;
  BlockStmt* defaultBlock = arg->defaultExpr;
  bool       retval       = false;

  if (typeBlock != nullptr && defaultBlock != nullptr)
  {
    if (typeBlock->body.length == 1 && defaultBlock->body.length == 1)
    {
      Expr* typeExpr    = typeBlock->body.only();
      Expr* defaultExpr = defaultBlock->body.only();

      retval = exprTypeHackEqual(typeExpr, defaultExpr);
    }
  }

  return retval;
}

// Compares two expressions for signs of the typeHack copy
bool AstToText::exprTypeHackEqual(Expr* expr0, Expr* expr1) const
{
  bool retval = true;

  if (expr0 == nullptr && expr1 == nullptr)
  {
    retval = true;
  }

  else if (isUnresolvedSymExpr(expr0) && isUnresolvedSymExpr(expr1))
  {
    UnresolvedSymExpr* sym0 = toUnresolvedSymExpr(expr0);
    UnresolvedSymExpr* sym1 = toUnresolvedSymExpr(expr1);

    retval = (sym0->unresolved == sym1->unresolved);
  }

  else if (isSymExpr(expr0) && isSymExpr(expr1))
  {
    SymExpr* sym0 = toSymExpr(expr0);
    SymExpr* sym1 = toSymExpr(expr1);

    retval = (sym0->symbol() == sym1->symbol());
  }

  else if (isCallExpr(expr0) && isCallExpr(expr1))
  {
    CallExpr* call0 = toCallExpr(expr0);
    CallExpr* call1 = toCallExpr(expr1);

    if (call0->primitive != call1->primitive)
      retval = false;

    else if (call0->numActuals() != call1->numActuals())
      retval = false;

    else if (exprTypeHackEqual(call0->baseExpr, call1->baseExpr) == false)
      retval = false;

    else
    {
      for (int i = 1; i <= call0->numActuals() && retval == true; i++)
        retval = exprTypeHackEqual(call0->get(i), call1->get(i));
    }
  }

  // For proc of the form "proc foo(x = bar())",
  //   expr0 wraps a FnSymbol
  //   expr1 is the original function name
  else if (isSymExpr(expr0) && isUnresolvedSymExpr(expr1))
  {
    SymExpr*           sym0 = toSymExpr(expr0);
    FnSymbol*          fn   = toFnSymbol(sym0->symbol());

    UnresolvedSymExpr* sym1 = toUnresolvedSymExpr(expr1);

    retval = (fn != 0 && strcmp(fn->name, sym1->unresolved) == 0);
  }

  else
  {
    retval = false;
  }

  return retval;
}

//
// Before normalize, the AST for the typeOf part of a signature like
//
//   +(x: _tuple, y: x(1).type)
//
// is roughly
//
//   #<BlockStmt #<CallExpr "typeof" #<CallExpr x(1)>> >
//
// After normalize, this becomes (roughly)
//
//   #<BlockStmt #<DefExpr  call_tmp>
//               #<CallExpr "move"(call_tmp, #<CallExpr x(1)>)>
//               #<CallExpr "typeof"(call_tmp)>
//
// Attempt to detect this pattern and then generate the desired output.
//

bool AstToText::handleNormalizedTypeOf(BlockStmt* bs)
{
  bool retval = false;

  if (bs->body.length == 3)
  {
    DefExpr*  callTmp = toDefExpr (bs->body.get(1));
    CallExpr* moveExp = toCallExpr(bs->body.get(2));
    CallExpr* typeExp = toCallExpr(bs->body.get(3));

    if (callTmp != nullptr && moveExp != nullptr && typeExp != nullptr)
    {
      if (moveExp->isPrimitive(PRIM_MOVE)   == true &&
          typeExp->isPrimitive(PRIM_TYPEOF) == true)
      {
        if (CallExpr* moveSrc = toCallExpr(moveExp->get(2)))
        {
          if (moveSrc->numActuals() == 1)
          {
            mText  += ": ";
            appendExpr(moveSrc, true);
            mText  += ".type";

            retval =  true;
          }
        }
      }
    }
  }

  return retval;
}

void AstToText::appendFormalVariableExpr(ArgSymbol* arg)
{
  if (arg->variableExpr != 0)
  {
    mText += " ...";

    if (BlockStmt* blockStmt = toBlockStmt(arg->variableExpr))
    {
      Expr* expr = blockStmt->body.get(1);

      if (blockStmt->length() == 1)
      {
        if (DefExpr* sel = toDefExpr(expr))
        {
          if (VarSymbol* sym = toVarSymbol(sel->sym))
          {
            mText += "?";
            mText += sym->name;
          }
          else
          {
            // NOAKES 2015/02/05  Debugging support.
            // Might become ASSERT in the future
            mText += " appendFormalVariableExpr.00";
          }
        }

        else
        {
          SymExpr* se = toSymExpr(expr);
          bool unnamed = se && se->symbol() == gUninstantiated;

          if (!unnamed) {
            appendExpr(expr, false);
          }
        }
      }
      else
      {
        // NOAKES 2015/02/05  Debugging support. Might become ASSERT in
        // the future or variableExpr might be converted to an expression
        mText += " appendFormalVariableExpr.01";
      }
    }
    else
    {
      // NOAKES 2015/02/05  Debugging support.
      // Might become ASSERT in the future
      mText += " appendFormalVariableExpr.02";
    }
  }
}

void AstToText::appendFormalDefault(ArgSymbol* arg)
{
  if (arg->defaultExpr  != nullptr)
  {
    BlockStmt* bs = arg->defaultExpr;

    if (bs->body.length == 1)
    {
      Expr* expr = bs->body.get(1);

      if (isTypeDefault(expr) == false)
      {
        mText += " = ";

        if (SymExpr* sym = toSymExpr(expr))
          appendExpr(sym, false, true);
        else
          appendExpr(expr, false);
      }
    }

    else
    {
      // NOAKES 2015/02/05  Debugging support. Might become ASSERT in the future
      // or AST might be updated so that defaultExpr is simply an expression.
      mText += " = AppendFormalDefault.00";
    }
  }
}

bool AstToText::isTypeDefault(Expr* expr) const
{
  bool retval = false;

  if (SymExpr* symExpr = toSymExpr(expr))
  {
    if (VarSymbol* var = toVarSymbol(symExpr->symbol()))
      retval = (strcmp(var->name, "_typeDefaultT") == 0) ? true : false;
  }

  return retval;
}

//
//  Helper function for appending domain calls with multiple arguments
//
void AstToText::appendDomain(CallExpr* expr, bool printingType)
{
  mText += "domain(";

  for(int i=2; i<=expr->numActuals(); i++)
  {
    if (i != 2)
      mText += ", ";

    if (UnresolvedSymExpr* symi = toUnresolvedSymExpr(expr->get(i)))
    {
      appendExpr(symi, printingType);
    }

    else if (SymExpr* symi = toSymExpr(expr->get(i)))
    {
      appendExpr(symi, printingType);
    }

    else if (CallExpr* symi = toCallExpr(expr->get(i)))
    {
      appendExpr(symi, printingType);
    }

    else if (NamedExpr* symi = toNamedExpr(expr->get(i)))
    {
      appendExpr(symi, printingType);
    }

    else
    {
      mText += "AppendExpr.Call10";
    }
  }

  mText += ")";
}

/************************************ | *************************************
*                                                                           *
* Helper functions for handling the "hidden formals" for methods.           *
*                                                                           *
* If a procedure is a method then                                           *
*                                                                           *
*   In earlier passes                                                       *
*      formals[1] has name _mt (the method token)                           *
*      formals[2] is flagged with FLAG_ARG_THIS                             *
*                                                                           *
*    In later passes                                                        *
*       formals[1] is flagged with FLAG_ARG_THIS                            *
*                                                                           *
************************************* | ************************************/

// The index for "this" -> [0 .. 2]
int AstToText::indexForThis(FnSymbol* fn) const
{
  int        numFormals = fn->formals.length;
  ArgSymbol* arg1       = nullptr;
  ArgSymbol* arg2       = nullptr;
  int        retval     = 0;

  //
  // Attempt to get the ArgSymbol for the first two formals (if present)
  //
  if (numFormals >= 1)
  {
    DefExpr* formal = toDefExpr(fn->formals.get(1));

    arg1 = toArgSymbol(formal->sym);
  }

  if (numFormals >= 2)
  {
    DefExpr* formal = toDefExpr(fn->formals.get(2));

    arg2 = toArgSymbol(formal->sym);
  }

  //
  // Determine if either of the first two formals is "this"
  //
  if      (arg1 != nullptr && strcmp(arg1->name, "_mt")    == 0 &&
           arg2 != nullptr && arg2->hasFlag(FLAG_ARG_THIS) == true)
    retval = 2;

  else if (arg1 != nullptr && arg1->hasFlag(FLAG_ARG_THIS) == true)
    retval = 1;

  else
    retval = 0;

  return retval;
}

// The one-based index of the first user-facing formal -> [1 .. 3]
int AstToText::indexOfFirstFormal(FnSymbol* fn) const
{
  return indexForThis(fn) + 1;
}

ArgSymbol* AstToText::formalGet(FnSymbol* fn, int oneBasedIndex) const
{
  int      effIndex = indexForThis(fn) + oneBasedIndex;
  DefExpr* expr     = toDefExpr(fn->formals.get(effIndex));

  return toArgSymbol(expr->sym);
}

/************************************ | *************************************
*                                                                           *
* Normalized version for the primitive expressions found in formals.        *
*                                                                           *
************************************* | ************************************/

void AstToText::appendExpr(Expr* expr, bool printingType, const char *outer,
                           bool unary, bool postfix, bool isRHS)
{
  if      (UnresolvedSymExpr* sel = toUnresolvedSymExpr(expr))
    appendExpr(sel);

  else if (SymExpr*           sel = toSymExpr(expr))
    appendExpr(sel, printingType, false);

  else if (CallExpr*          sel = toCallExpr(expr))
    appendExpr(sel, printingType, outer, unary, postfix, isRHS);

  else if (DefExpr*           sel = toDefExpr(expr))
    appendExpr(sel, printingType);

  else if (NamedExpr*         sel = toNamedExpr(expr))
    appendExpr(sel, printingType);

  else if (IfExpr*         sel = toIfExpr(expr))
    appendExpr(sel, printingType);

  else if (LoopExpr*         sel = toLoopExpr(expr))
    appendExpr(sel, printingType);

  else
  {
    // NOAKES 2015/02/05  Debugging support.
    // Might become ASSERT in the future
    mText += "AppendExpr.01";
  }
}

void AstToText::appendExpr(UnresolvedSymExpr* expr)
{
  if (strcmp(expr->unresolved, "_shared") == 0) {
    mText += "shared";
  } else if (strcmp(expr->unresolved, "_owned") == 0) {
    mText += "owned";
  } else if (strcmp(expr->unresolved, "_borrowed") == 0) {
    mText += "borrowed";
  } else if (strcmp(expr->unresolved, "_unmanaged") == 0) {
    mText += "unmanaged";
  } else {
    appendExpr(expr->unresolved);
  }
}

void AstToText::appendExpr(SymExpr* expr, bool printingType, bool quoteStrings)
{
  if (VarSymbol* var = toVarSymbol(expr->symbol()))
  {
    if (var->immediate != 0)
    {
      const size_t bufSize = 128;
      char         imm[bufSize];

      if (var->type == dtBool)
      {
        if (var->immediate->v_bool == 0)
          sprintf(imm, "false");
        else
          sprintf(imm, "true");
      }

      else if (var->immediate->const_kind == CONST_KIND_STRING)
      {
        if (strcmp(var->immediate->v_string, "_dom") == 0)
          strcpy(imm, "domain");

        else if (quoteStrings == true)
        {
          char* ptr = imm;

          if (var->immediate->string_kind == STRING_KIND_C_STRING)
            *ptr++ = 'c';
          else if (var->immediate->string_kind == STRING_KIND_BYTES)
            *ptr++ = 'b';
          *ptr++ = '"';
          strcpy(ptr, var->immediate->v_string);
          ptr = strchr(ptr, '\0');
          *ptr++ = '"';
          *ptr++ = '\0';
        }

        else
          strcpy(imm, var->immediate->v_string);
      }

      else
      {
        snprint_imm(imm, bufSize, *var->immediate);

        if (var->type != 0 && is_imag_type(var->type) == true)
        {
          char* tail = strchr(imm, '\0');

          *tail++ = 'i';
          *tail   = '\0';
        }
      }

      mText += imm;
    }
    else
    {
      /*
       * For an expression like
       *   var o: object? = nil;
       * we arrive here at the "nil" with printingType == false.
       *
       * For an expression like
       *   x: [something] int
       * we arrive here inside the [] with printingType == true.  When
       * var->name is "nil" we want to produce "x: [] int".
       */
      if (!printingType || strcmp(var->name, "nil") != 0)
        mText += var->name;
    }
  }

  else if (ArgSymbol*  sym = toArgSymbol(expr->symbol()))
  {
    appendExpr(sym->name);
  }

  else if (TypeSymbol* sym = toTypeSymbol(expr->symbol()))
  {
    appendExpr(sym->name);
  }

  else if (EnumSymbol* sym = toEnumSymbol(expr->symbol()))
  {
    if (EnumType* type = toEnumType(sym->type))
    {
      mText += type->symbol->name;
      mText += '.';
      mText += sym->name;
    }
    else
    {
      mText += " appendExpr.SymExpr.00";
    }
  }

  else
  {
    // NOAKES 2015/02/05  Debugging support.
    // Might become ASSERT in the future
    mText += " appendExpr.SymExpr.01";
  }
}

static bool looksLikeInfixOperator(const char *fnName)
{
  // It looks like an operator if it doesn't look like an identifier.
  bool looksLikeIdentifier = isalpha(fnName[0]) || fnName[0] == '_';

  return !looksLikeIdentifier;
}

/*
 * Operator precedence according to the table in the spec,
 * expressions.rst
 *
 * unary flag is needed because unary - (and +) have higher precedence
 * than binary - (and +).
 *
 * postfix flag is needed because postfix ! has higher precedence than
 * prefix !.
 *
 * Returns precedence: higher is tighter-binding.
 * Returns -1 for unhandled operator -- caller should respond conservatively.
 */
static int opToPrecedence(const char *op, bool unary, bool postfix) {
  // new is precedence 19, but doesn't come through this path.
  if (postfix && (strcmp(op, "?") == 0 || strcmp(op, "!") == 0))
    return 18;
  else if (strcmp(op, ":") == 0 || op == astr_cast)
    return 17;
  else if (strcmp(op, "**") == 0)
    return 16;
  // reduce/scan/dmapped are precedence 15, but don't come through this path.
  else if (strcmp(op, "!") == 0 || strcmp(op, "~") == 0)
    return 14;
  else if (strcmp(op, "*") == 0 ||
           strcmp(op, "/") == 0 || strcmp(op, "%") == 0)
    return 13;
  else if (unary &&
           (strcmp(op, "+") == 0 || strcmp(op, "-") == 0))
    return 12;
  else if (strcmp(op, "<<") == 0 || strcmp(op, ">>") == 0)
    return 11;
  else if (strcmp(op, "&") == 0)
    return 10;
  else if (strcmp(op, "^") == 0)
    return 9;
  else if (strcmp(op, "|") == 0)
    return 8;
  else if (strcmp(op, "+") == 0 || strcmp(op, "-") == 0)
    return 7;
  // .. and ..< are precedence 6, but don't come through this path.
  else if (op == astrSlt || op == astrSlte ||
           op == astrSgt || op == astrSgte)
    return 5;
  else if (op == astrSeq || op == astrSne)
    return 4;
  else if (strcmp(op, "&&") == 0)
    return 3;
  else if (strcmp(op, "||") == 0)
    return 2;
  // by and align are precedence 1 too, but don't come through this path.
  else if (strcmp(op, "#") == 0)
    return 1;

  return -1;
}

/*
 * needParens(outer, inner, ...) is called to evaluate whether we need
 * to add parens around the inner op.  We're here when
 * appendExpr(outer) is calling appendExpr(expr->get(1)) or
 * expr->get(2), which has become appendExpr(inner).
 *
 * Given an AST node outer, with one child inner, this function
 * returns true if we need to emit parens around the child expression.
 * That is, if emitting the expression without parenthesis would
 * change the semantics from what the AST represents.
 *
 * If the inner (child) operator has higher precedence than the outer
 * (parent), then we don't need parens, as emitting
 * "a outer_op b inner_op c" is equivalent to
 * "a outer_op (b inner_op c)".
 *
 * If the child operator has equal precedence to the outer, then we
 * generally don't need parenthesis, except for a few exceptions.  If
 * the desired expression is a-(b-c) or a-(b+c), then the parentheses
 * are needed.  Of course they're not needed for a+(b-c) or a+(b+c).
 * If the desired expression is a/(b/c) or a/(b*c) or a%(b/c), they're
 * needed, but not for a*(b*c) or a*(b/c).
 * (TODO: a*(b/c) might need the parens for overflow reasons.)
 *
 * Unary - (and +) have higher precedence than binary - (and +), so we
 * need the unary flag to know which case we're in.
 *
 * The postfix flag is needed because postfix ! has higher precedence
 * than prefix !.
 *
 * The innerIsRHS flag is needed because we need parens to express
 * a-(b-c) where inner- is the RHS of the outer-.  But we can emit
 * a-b-c instead of (a-b)-c -- when inner- is the LHS of outer-.
 * Also, to distinguish (-1)**2 (parens needed) from 1**(-2) (not needed).
 */
static bool needParens(const char *outer, const char *inner,
                       bool outerUnary, bool outerPostfix,
                       bool innerUnary, bool innerPostfix,
                       bool innerIsRHS) {
  bool ret = false;
  int outerprec, innerprec;

  if (!outer)
    return false;

  outerprec = opToPrecedence(outer, outerUnary, outerPostfix);
  innerprec = opToPrecedence(inner, innerUnary, innerPostfix);

  // -1 means opToPrecedence wasn't expecting one of these operators.
  // Conservatively wrap parentheses around the representation of this
  // AST node.
  if (outerprec == -1 || innerprec == -1)
    return true;

  // We never need parens around the unary expression on the RHS:
  // 1**-2 vs 1**(-2)
  if (innerUnary && innerIsRHS)
    return false;

  if (outerprec > innerprec)
    ret = true;

  // If inner and outer have the same precedence, and inner is the
  // rhs, it needs parens if a op1 (b op2 c) isn't equivalent to
  // a op1 b op2 c.  (Note op1 and op2 may be the same op, a - (b - c))
  if (innerIsRHS &&
      (strcmp(outer, "-") == 0 ||
       strcmp(outer, "/") == 0 || strcmp(outer, "%") == 0 ||
       strcmp(outer, "<<") == 0 || strcmp(outer, ">>") == 0 ||
       // (a==b)==true vs. a==(b==true)
       strcmp(outer, "==") == 0 || strcmp(outer, "!=") == 0)
      && outerprec == innerprec)
    ret = true;

  // ** is right-associative, and a**(b**c) != (a**b)**c.
  if (!innerIsRHS && strcmp(outer, "**") == 0 && outerprec == innerprec)
    ret = true;

  return ret;
}

/*
 * Args needed just for needsParens() call above, described in more
 * detail there:
 *
 * outer: The operator in this node's parent node.
 *        The remaining args are ignored if this is false.
 * unary: The parent node is a unary operator.
 * postfix: The parent node is a postfix unary operator.
 * isRHS: This node is the right-hand-side of the parent binary operator.
 */
void AstToText::appendExpr(CallExpr* expr, bool printingType, const char *outer,
                           bool unary, bool postfix, bool isRHS)
{
  if (expr->primitive == 0)
  {
    if (UnresolvedSymExpr* sel = toUnresolvedSymExpr(expr->baseExpr))
    {
      const char* fnName = sel->unresolved;

      // UnaryOp not
      if     (strcmp(fnName, "!")                            == 0)
      {
        bool needsParens = needParens(outer, fnName, unary, postfix,
                                      true, false, isRHS);

        if (needsParens)
          mText += "(";
        mText += "!";
        appendExpr(expr->get(1), printingType, fnName, true, false, false);
        if (needsParens)
          mText += ")";
      }

      // postfix!
      else if (fnName == astrPostfixBang                     &&
               expr->numActuals()                            == 1)
      {
        bool needsParens = needParens(outer, fnName, unary, postfix,
                                      true, true, isRHS);

        if (needsParens)
          mText += "(";
        appendExpr(expr->get(1), printingType, fnName, true, true, false);
        mText += "!";
        if (needsParens)
          mText += ")";
      }

      // UnaryOp bitwise negate
      else if (strcmp(fnName, "~")                           == 0 &&
               expr->numActuals()                            == 1)
      {
        bool needsParens = needParens(outer, fnName, unary, postfix,
                                      true, false, isRHS);

        if (needsParens)
          mText += "(";
        mText += "~";
        appendExpr(expr->get(1), printingType, fnName, true, false, false);
        if (needsParens)
          mText += ")";
      }

      // UnaryOp negate
      else if (strcmp(fnName, "-")                           == 0 &&
               expr->numActuals()                            == 1)
      {
        bool needsParens = needParens(outer, fnName, unary, postfix,
                                      true, false, isRHS);
        if (needsParens)
          mText += "(";
        mText += "-";
        appendExpr(expr->get(1), printingType, fnName, true, false, false);
        if (needsParens)
          mText += ")";
      }

      // UnaryOp plus
      else if (strcmp(fnName, "+")                           == 0 &&
               expr->numActuals()                            == 1)
      {
        bool needsParens = needParens(outer, fnName, unary, postfix,
                                      true, false, isRHS);
        if (needsParens)
          mText += "(";
        mText += "+";
        appendExpr(expr->get(1), printingType, fnName, true, false, false);
        if (needsParens)
          mText += ")";
      }

      else if (expr->isCast())
      {
        bool needsParens = needParens(outer, fnName, unary, postfix,
                                      false, false, isRHS);
        if (needsParens)
          mText += "(";
        appendExpr(expr->castFrom(), printingType, ":", false, false, false);
        if (needsParens)
          mText += ")";
        mText += ": ";
        appendExpr(expr->castTo(), printingType);
      }

      else if (strcmp(fnName, "chpl_by")                      == 0)
      {
        appendExpr(expr->get(1), printingType);
        mText += " by ";
        appendExpr(expr->get(2), printingType);
      }

      else if (strcmp(fnName, "chpl_align")                   == 0)
      {
        appendExpr(expr->get(1), printingType);
        mText += " align ";
        appendExpr(expr->get(2), printingType);
      }

      else if (strcmp(fnName, "chpl__atomicType")             == 0)
      {
        mText += "atomic ";
        appendExpr(expr->get(1), printingType);
      }

      else if (strcmp(fnName, "chpl__distributed")             == 0)
      {
        appendExpr(expr->get(2), printingType);
        mText += " dmapped ";
        appendExpr(expr->get(1), printingType);
      }

      else if (strcmp(fnName, "chpl__buildDomainExpr")        == 0)
      {
        mText += "{";
        appendExpr(expr->get(1), printingType);

        // last argument to chpl__buildDomainExpr is definedConst
        for (int index = 2; index <= expr->numActuals()-1; index++)
        {
          mText += ", ";
          appendExpr(expr->get(index), printingType);
        }
        mText += "}";
      }

      else if (strcmp(fnName, "chpl__ensureDomainExpr")       == 0)
      {
        appendExpr(expr->get(1), printingType);
        for (int index = 2; index <= expr->numActuals(); index++)
        {
          mText += ", ";
          appendExpr(expr->get(index), printingType);
        }
      }

      else if (strcmp(fnName, "chpl__buildDomainRuntimeType") == 0)
      {
        if (expr->numActuals() > 1)
        {

          if (isUnresolvedSymExpr(expr->get(1)))
          {
            UnresolvedSymExpr* sym = toUnresolvedSymExpr(expr->get(1));

            if (strcmp(sym->unresolved, "defaultDist") == 0)
            {
              appendDomain(expr, printingType);
            }

            else
            {
              // NOAKES 2015/02/05  Debugging support.
              // Might become ASSERT in the future
              mText += "AppendExpr.Call00";
            }
          }

          else if (isSymExpr(expr->get(1)))
          {
            SymExpr*   sym1 = toSymExpr(expr->get(1));

            VarSymbol* arg1 = toVarSymbol(sym1->symbol());

            if (arg1 != 0 && strcmp(arg1->name, "defaultDist") == 0)
            {
              appendDomain(expr, printingType);
            }

            else
            {
              // NOAKES 2015/02/05  Debugging support.
              // Might become ASSERT in the future
              mText += "AppendExpr.Call01";
            }
          }

          else
          {
            // NOAKES 2015/02/05  Debugging support.
            // Might become ASSERT in the future
            mText += "AppendExpr.Call02";
          }

        }

        else
        {
          // NOAKES 2015/02/05  Debugging support.
          // Might become ASSERT in the future
          mText += "AppendExpr.Call03";
        }
      }

      else if (strcmp(fnName, "chpl__buildArrayRuntimeType") == 0)
      {
        mText += "[";
        appendExpr(expr->get(1), printingType);
        mText += "]";

        if (expr->numActuals() == 2)
        {
          mText += " ";
          appendExpr(expr->get(2), printingType);
        }
      }

      else if (strcmp(fnName, "_build_tuple")                == 0)
        appendExpr(expr, "", printingType);

      else if (strcmp(fnName, "chpl__buildIndexType")        == 0)
        appendExpr(expr, "index", printingType);

      else if (strcmp(fnName, "range")                       == 0)
        appendExpr(expr, "range", printingType);

      else if (strcmp(fnName, "chpl_build_bounded_range") == 0)
      {
        appendExpr(expr->get(1), printingType);
        mText += "..";
        appendExpr(expr->get(2), printingType);
      }

      else if (strcmp(fnName, "chpl_build_low_bounded_range") == 0)
      {
        appendExpr(expr->get(1), printingType);
        mText += "..";
      }

      else if (strcmp(fnName, "chpl_build_high_bounded_range") == 0)
      {
        mText += "..";
        appendExpr(expr->get(1), printingType);
      }

      else if (strcmp(fnName, "chpl_build_unbounded_range") == 0)
      {
        mText += "..";
      }

      else if (strcmp(fnName, "_owned") == 0)
      {
        mText += "owned ";
        appendExpr(expr->get(1), printingType);
      }

      else if (strcmp(fnName, "_shared") == 0)
      {
        mText += "shared ";
        appendExpr(expr->get(1), printingType);
      }

      else if ((fnName != astrSdot)                          == 0)
      {
        SymExpr* symExpr1 = toSymExpr(expr->get(1));
        SymExpr* symExpr2 = toSymExpr(expr->get(2));

        if (symExpr1 != 0 && symExpr2 != 0)
        {
          if (isArgSymbol(symExpr1->symbol()) && isVarSymbol(symExpr2->symbol()))
          {
            ArgSymbol* sym1 = toArgSymbol(symExpr1->symbol());

            if (sym1->name == astrThis)
            {
              appendExpr(symExpr2, printingType);
            }
            else
            {
              appendExpr(symExpr1, printingType);
              mText += '.';
              appendExpr(symExpr2, printingType);
            }
          }

          else if (isVarSymbol(symExpr1->symbol()) && isVarSymbol(symExpr2->symbol()))
          {
            VarSymbol* sym1 = toVarSymbol(symExpr1->symbol());

            if (sym1->name == astrThis)
            {
              appendExpr(symExpr2, printingType);
            }
            else
            {
              appendExpr(symExpr1, printingType);
              mText += '.';
              appendExpr(symExpr2, printingType);
            }
          }

          else
          {
            // NOAKES 2015/02/05  Debugging support.
            // Might become ASSERT in the future
            mText += "AppendExpr.Call04";
          }

        }
        else
        {
          appendExpr(expr->get(1), printingType);
          mText += '.';
          appendExpr(expr->get(2), printingType);
        }
      }

      // Convert <name>(_mt, this) to <name>
      else if (isMtThis(expr) == true)
      {
        UnresolvedSymExpr* name = toUnresolvedSymExpr(expr->baseExpr);

        mText += name->unresolved;
      }

      // Convert <name>(_mt, <class>) to <class>.<name>
      else if (isMtOther(expr) == true)
      {
        UnresolvedSymExpr* name     = toUnresolvedSymExpr(expr->baseExpr);
        SymExpr*           symClass = toSymExpr(expr->get(2));

        mText += symClass->symbol()->name;
        mText += '.';
        mText += name->unresolved;
      }

      // Format binary operators in infix notation
      else if (expr->numActuals() == 2 && looksLikeInfixOperator(fnName))
      {
        // ... except homogeneous tuple inner workings.
        if (printingType && strcmp(fnName, "*") == 0)
        {
          // This is not a multiply, it's the symbol for a homogeneous tuple.

          // I found that some multiplies would match this (even though they
          // really should be PRIM_MULT), so we must rely on context to
          // differentiate between the two cases: if we're in a type expression,
          // either something has gone terribly wrong or a homogeneous tuple is
          // intended.  If we're in another expression, it's more likely to be
          // a multiply.
          appendExpr(expr->get(1), printingType);
          mText += "*(";
          appendExpr(expr->get(2), printingType);
          mText += ")";
        }

        else
        {
          // Binary operator, infix notation
          bool needsParens = needParens(outer, fnName, unary, postfix,
                                        false, false, isRHS);
          if (needsParens)
            mText += "(";
          appendExpr(expr->get(1), printingType, fnName, false, false, false);
          appendExpr(expr->baseExpr, printingType);
          appendExpr(expr->get(2), printingType, fnName, false, false, true);
          if (needsParens)
            mText += ")";
        }

      }

      else /* function/type */
        appendExpr(expr, fnName, printingType);
    }

    else if (isSymExpr(expr->baseExpr))
    {
      if (expr->numActuals() == 0)
      {
        // NOAKES 2015/02/05  Debugging support.
        // Might become ASSERT in the future
        mText += "AppendExpr.Call06";
      }

      else
      {
        appendExpr(expr->baseExpr, printingType);
        mText += '(';

        for (int i = 1; i <= expr->numActuals(); i++)
        {
          if (i > 1)
            mText += ", ";

          appendExpr(expr->get(i), printingType);
        }

        mText += ')';
      }
    }

    else if (isCallExpr(expr->baseExpr))
    {
      CallExpr* subCall = toCallExpr(expr->baseExpr);
      appendExpr(subCall->get(1), printingType);
      mText += '.';
      appendExpr(subCall->get(2), printingType);
      mText += '(';

      for (int i = 1; i <= expr->numActuals(); i++)
      {
        if (i > 1)
          mText += ", ";

        appendExpr(expr->get(i), printingType);
      }

      mText += ')';
    }

    else
    {
      // NOAKES 2015/02/05  Debugging support.
      // Might become ASSERT in the future
      mText += "AppendExpr.Call08";
    }
  }

  else
  {
    if (expr->isPrimitive(PRIM_TYPEOF))
    {
      appendExpr(expr->get(1), printingType);
      mText += ".type";
    }
    else if (expr->isPrimitive(PRIM_TRY_EXPR))
    {
      mText += "try ";
      appendExpr(expr->get(1), printingType);
    }
    else if (expr->isPrimitive(PRIM_TRYBANG_EXPR))
    {
      mText += "try! ";
      appendExpr(expr->get(1), printingType);
    }
    else if (expr->isPrimitive(PRIM_NEW))
    {
      mText += "new ";

      bool addQ = false;
      Expr* inner = expr->get(1);
      // skip management decorator if present
      if (NamedExpr* ne = toNamedExpr(inner)) {
        if (ne->name == astr_chpl_manager) {
          inner = expr->get(2);
          Type* t = ne->actual->typeInfo();
          const char* n = "";
          if (t == dtBorrowed)
            n = "borrowed ";
          if (t == dtUnmanaged)
            n = "unmanaged ";
          if (t == dtShared)
            n = "shared ";
          if (t == dtOwned)
            n = "owned ";
          if (isNilableClassType(t))
            addQ = true;

          mText += n;
        }
      }
      // skip to-nilable if present
      while (inner) {
        CallExpr* call = toCallExpr(inner);
        if (call == nullptr)
          break;
        if (call->isPrimitive(PRIM_TO_NILABLE_CLASS) == false &&
            call->isPrimitive(PRIM_TO_NILABLE_CLASS_CHECKED) == false)
          break;
        // it's a PRIM_TO_NILABLE_CLASS etc
        inner = call->get(1);
        addQ = true;
      }

      appendExpr(inner, printingType);

      if (addQ)
        mText += "?";
    }
    else if (expr->isPrimitive(PRIM_TO_UNMANAGED_CLASS) ||
             expr->isPrimitive(PRIM_TO_UNMANAGED_CLASS_CHECKED))
    {
      mText += "unmanaged ";
      appendExpr(expr->get(1), printingType);
    }
    else if (expr->isPrimitive(PRIM_TO_BORROWED_CLASS) ||
             expr->isPrimitive(PRIM_TO_BORROWED_CLASS_CHECKED))
    {
      mText += "borrowed ";
      appendExpr(expr->get(1), printingType);
    }
    else if (expr->isPrimitive(PRIM_TO_NILABLE_CLASS) ||
             expr->isPrimitive(PRIM_TO_NILABLE_CLASS_CHECKED))
    {
      mText += "nilable ";
      appendExpr(expr->get(1), printingType);
    }
    else if (expr->isPrimitive(PRIM_TO_NON_NILABLE_CLASS))
    {
      mText += "nonnilable ";
      appendExpr(expr->get(1), printingType);
    }
    else if (!expr->isPrimitive(PRIM_UNKNOWN))
    {
      mText += "__primitive(\"";
      mText += expr->primitive->name;
      mText += "\"";
      for (int index = 1; index <= expr->numActuals(); index++)
        {
          mText += ", ";
          if (!isSymExpr(expr->get(index)))
          {
            appendExpr(expr->get(index), printingType);
          }
          else
          {
            mText += "\"";
            appendExpr(expr->get(index), printingType);
            mText += "\"";
          }
        }
      mText += ")";
    }
    else
    {
      // NOAKES 2015/02/05  Debugging support.
      // Might become ASSERT in the future
      mText += "AppendExpr.Call09";
    }
  }
}

void AstToText::appendExpr(DefExpr* expr, bool printingType)
{
  if (printingType)
    {

    mText += '?';

    // This section initially was ensuring the sym field referred to
    // a VarSymbol in this case.  However, since we were only accessing the
    // name field - which is present for all Symbols - this check was not
    // necessary.  Should something go wrong with this section, perhaps
    // first check if expr->sym is a VarSymbol as was initially expected?
    const char* name = expr->sym->name;
    if (strncmp(name, "chpl__query", 11) != 0)
      mText += name;

    }
  else
    {
      mText += expr->sym->name;
      if (expr->exprType) {
        mText += ": ";
        appendExpr(expr->exprType, true);
      }
      if (expr->init) {
        mText += " = ";
        if (SymExpr* sym = toSymExpr(expr->init))
          appendExpr(sym, false, true);
        else
          appendExpr(expr->init, false);
      }
    }
}

void AstToText::appendExpr(NamedExpr* expr, bool printingType)
{
  mText += expr->name;
  mText += " = ";
  appendExpr(expr->actual, printingType);
}

void AstToText::appendExpr(IfExpr* expr, bool printingType)
{
  mText += "if ";
  if (Expr* sel = toExpr(expr->getCondition()))
  {
    appendExpr(sel, printingType);

    if (BlockStmt* thenBlockStmt = toBlockStmt(expr->getThenStmt()))
    {
      mText += " then ";
      if (thenBlockStmt->body.length == 1)
      {
        Expr* exp = thenBlockStmt->body.get(1);
        appendExpr(exp, printingType);

        if (BlockStmt* elseBlockStmt = toBlockStmt(expr->getElseStmt()))
        {
          mText += " else ";
          if (elseBlockStmt->body.length == 1)
          {
            Expr* exp = elseBlockStmt->body.get(1);
            appendExpr(exp, printingType);
          }

          else
          {
            mText += "AppendExpr.If00";
          }
        }

        else
        {
          mText += "AppendExpr.If01";
        }
      }

      else
      {
        mText += "AppendExpr.If02";
      }
    }

    else
    {
      mText += "AppendExpr.If03";
    }
  }

  else
  {
    mText += "AppendExpr.If04";
  }
}

void AstToText::appendExpr(LoopExpr* expr, bool printingType)
{
  std::string start,end;
  if (expr->forall)
  {
    if (expr->maybeArrayType)
    {
      start = "[";
      end = "]";
    }

    else
    {
      start = "forall ";
      end = " do";
    }
  }

  else
  {
    start = "for ";
    end = " do";
  }
  mText += start;

  if (expr->indices)
  {
    appendExpr(expr->indices, printingType);
    mText += " in ";
  }

  if (expr->iteratorExpr)
  {
    appendExpr(expr->iteratorExpr, printingType);
    mText += end;

    if (BlockStmt* bs = toBlockStmt(expr->loopBody))
    {
      mText += ' ';
      appendExpr(bs->body.get(1), printingType);
    }

    else
    {
      mText += " AppendExpr.Loop01";
    }

  }

  else
  {
    mText += " AppendExpr.Loop02";
  }
}

void AstToText::appendExpr(CallExpr* expr, const char* fnName, bool printingType)
{
  appendExpr(fnName);
  mText += '(';

  for (int i = 1; i <= expr->numActuals(); i++)
  {
    if (i > 1)
      mText += ", ";

    appendExpr(expr->get(i), printingType);
  }

  // 1-tuples get a trailing "," inside the parens.
  // Only tuples reach here with fnName == "".
  if (strlen(fnName) == 0 && expr->numActuals() == 1)
    mText += ",";

  mText += ')';
}

// Normalize names
void AstToText::appendExpr(const char* name)
{
  //
  // NOAKES 2015/02/05
  //
  // The compiler maps sync -> _syncvar, single -> _singlevar, etc.
  //
  // Undo this mapping.
  // There are a few uses of these names in internal modules and those
  // will be altered but this is acceptable for this incremental upgrade.

  if      (strncmp(name, "_syncvar",          8) == 0)
    appendSpecialExpr(name, "_syncvar", "sync");

  else if (strncmp(name, "_singlevar",        8) == 0)
    appendSpecialExpr(name, "_singlevar",  "single");

  else if (strncmp(name, "_domain",           8) == 0)
    appendSpecialExpr(name, "_domain",  "domain");

  // Unwrap _ref(foo())
  else if (strncmp(name, "_ref(",             5) == 0)
  {
    std::string buffer = removeOuterParens(name + 4);

    appendExpr(buffer.c_str());
  }

  // Convert to array syntax
  else if (strcmp(name, "_array")                == 0)
    mText += "[] ";

  // Normalize type names for 64-bit default
  else if (strcmp(name, "int(64)")               == 0)
    mText += "int";

  else if (strcmp(name, "uint(64)")              == 0)
    mText += "uint";

  else if (strcmp(name, "real(64)")              == 0)
    mText += "real";

  else if (strcmp(name, "imag(64)")              == 0)
    mText += "imag";

  else if (strcmp(name, "complex(128)")          == 0)
    mText += "complex";

  else
    mText += name;
}

// Handle cases like
//   _syncvar -> sync
//   _syncvar(int(64)) -> sync int
//
//   root will be "_syncvar"
//   repl will be "sync"
//
void AstToText::appendSpecialExpr(const char* name,
                                  const char* root,
                                  const char* repl)
{
  int len = strlen(root);

  // Write out the replacement string
  mText += repl;

  // Now check for a parenthesized string
  if (name[len] == '(')
  {
    std::string buffer = removeOuterParens(name);

    mText += ' ';
    appendExpr(buffer.c_str());
  }
}

// Remove outer parens i.e. "(foo(a))" -> "foo(a)"
std::string AstToText::removeOuterParens(const char* parenExpr) const
{
  // Step over the initial open paren
  const char* fptr       = parenExpr + 1;
  int         parenCount = 1;

  std::string retval     = "";

  while ((*fptr != '\0' && *fptr != ')') || parenCount > 1)
  {
    if (*fptr == '(')
      parenCount += 1;
    else if (*fptr == ')')
      parenCount -= 1;

    retval += *fptr++;
  }

  return retval;
}

//
// Before normalize, the AST for the signatures of methods like
//
//    _singlevar.writeEF(val: base_type)
//    _singlevar.writeEF(val: sv.base_type)
//
// is roughly
//
//    #<ArgSymbol typeExpr: #<Call "." ("this", "base_type")>>
//    #<ArgSymbol typeExpr: #<Call "." ("sv",   "base_type")>>
//
// respectively.  This is easy to detect and reformat.
//
// After normalize these signatures become (roughly)
//
//    #<ArgSymbol typeExpr: #<Call "base_type" ("_mt", "this")
//    #<ArgSymbol typeExpr: #<Call "base_type" ("_mt", "sv")
//
// These predicates check for this pattern.
//

bool AstToText::isMtThis(CallExpr* expr) const
{
  return isMtArg(expr, true);
}

bool AstToText::isMtOther(CallExpr* expr) const
{
  return isMtArg(expr, false);
}

bool AstToText::isMtArg(CallExpr* expr, bool expectThis) const
{
  bool retval = false;

  if (isUnresolvedSymExpr(expr->baseExpr))
  {
    if (CallExpr* call = toCallExpr(expr))
    {
      if (call->numActuals() == 2)
      {
        SymExpr* symMt     = toSymExpr(call->get(1));
        SymExpr* symTarget = toSymExpr(call->get(2));

        if (symMt != nullptr && symTarget != 0)
        {
          VarSymbol* varMt     = toVarSymbol(symMt->symbol());
          ArgSymbol* argTarget = toArgSymbol(symTarget->symbol());

          if (varMt                             != nullptr       &&
              argTarget                         != nullptr       &&
              varMt->type                       == dtMethodToken &&
              argTarget->hasFlag(FLAG_ARG_THIS) == expectThis)
          {
            retval = true;
          }
        }
      }
    }
  }

  return retval;
}

void AstToText::appendEnumDecl(EnumType* et) {
  mText += et->symbol->name;

  appendEnumConstants(et);
}

void AstToText::appendEnumConstants(EnumType* et) {
  if (et->constants.length > 0) {
    bool first = true;
    mText += " { ";

    for_alist(constant, et->constants) {
      DefExpr* de = toDefExpr(constant);
      // We expect all members in constants alist to be DefExprs.
      INT_ASSERT(de);
      if (!first) {
        mText += ", ";
      } else {
        first = false;
      }
      appendExpr(de, false);
    }
    mText += " }";
  } else {
    mText += "";
  }
}

void AstToText::appendVarDef(VarSymbol* var) {
  appendExpr(var->defPoint, false);
}
