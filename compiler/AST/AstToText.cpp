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

#include "AstToText.h"

#include "expr.h"
#include "stmt.h"
#include "symbol.h"

AstToText::AstToText()
{

}

AstToText::~AstToText()
{

}

const std::string& AstToText::text() const
{
  return mText;
}

void AstToText::appendNameAndFormals(FnSymbol* fn)
{
  if (fn->instantiatedFrom != NULL && developer == false)
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
  if (developer == true)
    mText += fn->name;

  else if (fn->hasFlag(FLAG_MODULE_INIT))
  {
    INT_ASSERT(strncmp(fn->name, "chpl__init_",      11) == 0);

    mText += "top-level module statements for ";
    mText += (fn->name + 11);
  }

  else if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR))
  {
    INT_ASSERT(strncmp(fn->name, "_type_construct_", 16) == 0);

    mText += (fn->name + 16);
  }

  else if (fn->hasFlag(FLAG_CONSTRUCTOR))
  {
    INT_ASSERT(strncmp(fn->name, "_construct_",      11) == 0);

    mText += (fn->name + 11);
  }

  else if (fn->hasFlag(FLAG_DESTRUCTOR) == true)
  {
    appendClassName(fn);
    mText += ".~";
    appendClassName(fn);
  }

  else if (fn->hasFlag(FLAG_METHOD))
  {
    appendThisIntent(fn);

    if (strcmp(fn->name, "this") == 0)
    {
      appendClassName(fn);
    }

    else
    {
      appendClassName(fn);
      mText += '.';
      mText += fn->name;
    }
  }

  else
    mText += fn->name;
}

void AstToText::appendThisIntent(FnSymbol* fn)
{
  int index = indexForThis(fn);

  if (index > 0)
  {
    DefExpr*   formal = toDefExpr(fn->formals.get(index));
    ArgSymbol* argSym = toArgSymbol(formal->sym);

    if (argSym->intent == INTENT_REF)
      mText += "ref ";

    else if (argSym->intent == INTENT_PARAM)
      mText += "param ";
  }
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
          if (TypeSymbol* typeSym = toTypeSymbol(sel->var))
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
    ArgSymbol* arg = formalGet(fn, index);

    if (arg->hasFlag(FLAG_IS_MEME) == false)
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
  }

  if (skip == false)
    mText += ')';
}

bool AstToText::skipParens(FnSymbol* fn) const
{
  bool retval = false;

  if (fn->hasFlag(FLAG_NO_PARENS))
    retval = true;

  else if (fn->hasFlag(FLAG_TYPE_CONSTRUCTOR) && fn->numFormals() == 0)
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
        appendExpr(arg->typeExpr->body.get(1));
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

bool AstToText::typeExprCopiedFromDefaultExpr(ArgSymbol* arg) const
{
  // Incomplete
  return false;
}

bool AstToText::handleNormalizedTypeOf(BlockStmt* bs)
{
  // Incomplete
  return false;
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
            if (strncmp(sym->name, "chpl__query", 11) != 0)
            {
              mText += "?";
              mText += sym->name;
            }
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
          appendExpr(expr);
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
  if (arg->defaultExpr  != NULL)
  {
    BlockStmt* bs = arg->defaultExpr;

    if (bs->body.length == 1)
    {
      Expr* expr = bs->body.get(1);

      if (isTypeDefault(expr) == false)
      {
        mText += " = ";

        if (SymExpr* sym = toSymExpr(expr))
          appendExpr(sym, true);
        else
          appendExpr(expr);
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
    if (VarSymbol* var = toVarSymbol(symExpr->var))
      retval = (strcmp(var->name, "_typeDefaultT") == 0) ? true : false;
  }

  return retval;
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
  ArgSymbol* arg1       = NULL;
  ArgSymbol* arg2       = NULL;
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
  if      (arg1 != NULL && strcmp(arg1->name, "_mt")    == 0 &&
           arg2 != NULL && arg2->hasFlag(FLAG_ARG_THIS) == true)
    retval = 2;

  else if (arg1 != NULL && arg1->hasFlag(FLAG_ARG_THIS) == true)
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

void AstToText::appendExpr(Expr* expr)
{
  // Incomplete
}

void AstToText::appendExpr(SymExpr* expr, bool quoteStrings)
{
  // Incomplete
}

void AstToText::appendExpr(const char* name)
{
  // Incomplete
}

