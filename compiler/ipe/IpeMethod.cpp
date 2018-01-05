/*
 * Copyright 2004-2018 Cray Inc.
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

#include "IpeMethod.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "IpeBlockStmt.h"
#include "IpeCallExpr.h"
#include "IpeEnv.h"
#include "IpeModule.h"
#include "IpeProcedure.h"
#include "IpeScope.h"
#include "IpeScopeBlock.h"
#include "IpeScopeMethod.h"
#include "IpeValue.h"

#include "ipeDriver.h"
#include "ipeResolve.h"
#include "ipeEvaluate.h"

#include "stmt.h"
#include "symbol.h"
#include "WhileDoStmt.h"

#include <cstdio>


extern IpeEnv* sFooEnv;


IpeMethod::IpeMethod(FnSymbol* sym, IpeEnv* parentEnv)
{
  mState       = kUnresolved;
  mFnDecl      = sym;

  mEnvParent   = parentEnv;
  mScope       = NULL;

  mBody        = NULL;

  mFrameSize   = 0;

  mInvokeCount = 0;
}

IpeMethod::~IpeMethod()
{

}

const char* IpeMethod::name() const
{
  return mFnDecl->name;
}

FnSymbol* IpeMethod::fnSymbol() const
{
  return mFnDecl;
}

Type* IpeMethod::typeGet() const
{
  Type* retval = NULL;

  switch (mState)
  {
    case kUnresolved:
    case kResolvedFormals:
      break;

    case kResolvedReturnType:
    case kResolved:
      retval = mFnDecl->retType;
      break;

    case kUnresolvable:
      break;
  }

  INT_ASSERT(retval);

  return retval;
}

const char* IpeMethod::stateAsString() const
{
  const char* retval = NULL;

  switch (mState)
  {
    case kUnresolved:
      retval = "Unresolved";
      break;

    case kResolvedFormals:
      retval = "Formals Resolved";
      break;

    case kResolvedReturnType:
      retval = "Formals/ReturnType Resolved";
      break;

    case kResolved:
      retval = "Resolved";
      break;

    case kUnresolvable:
      retval = "Unresolvable";
      break;
  }

  return retval;
}

ArgSymbol* IpeMethod::formalGet(int index) const
{
  ArgSymbol* retval = NULL;

  if (index >= 0 && index < mFnDecl->formals.length)
  {
    DefExpr* formal = toDefExpr(mFnDecl->formals.get(index + 1));

    retval = toArgSymbol(formal->sym);
  }

  return retval;
}


/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

bool IpeMethod::resolveFormals()
{
  if (mState == kUnresolved)
  {
    // 2015/03/27 Do we need to be more careful about the scope to resolve the types?
    for_alist(formalExpr, mFnDecl->formals)
    {
      DefExpr*           defExpr   = toDefExpr(formalExpr);
      ArgSymbol*         formal    = toArgSymbol(defExpr->sym);
      BlockStmt*         blockStmt = formal->typeExpr;
      UnresolvedSymExpr* unres     = toUnresolvedSymExpr(blockStmt->body.get(1));
      LcnSymbol*         typeVar   = mEnvParent->findVariable(unres->unresolved);

      INT_ASSERT(formal->type           == dtUnknown);
      INT_ASSERT(blockStmt              != NULL);
      INT_ASSERT(blockStmt->body.length == 1);

      if (typeVar != 0)
        formal->type = (Type*) mEnvParent->fetchPtr(typeVar);
      else
        mState       = kUnresolvable;
    }

    if (mState == kUnresolved)
      mState = kResolvedFormals;
  }

  return (mState != kUnresolvable) ? true : false;
}

bool IpeMethod::resolveReturnType()
{
  // 2015/03/27 Do we need to be more careful about the scope to resolve the type?
  if (mState == kResolvedFormals)
  {
    BlockStmt*         blockStmt = mFnDecl->retExprType;
    UnresolvedSymExpr* unres     = toUnresolvedSymExpr(blockStmt->body.get(1));
    LcnSymbol*         typeVar   = mEnvParent->findVariable(unres->unresolved);

    INT_ASSERT(mFnDecl->retType       == dtUnknown);
    INT_ASSERT(blockStmt              != NULL);
    INT_ASSERT(blockStmt->body.length == 1);

    mFnDecl->retType = (typeVar != 0) ? (Type*) mEnvParent->fetchPtr(typeVar) : NULL;
    mState           = (typeVar != 0) ? kResolvedReturnType                   : kUnresolvable;
  }

  return (mState != kResolvedReturnType) ? true : false;
}

bool IpeMethod::isExactMatch(std::vector<Expr*>& actuals) const
{
  bool retval = true;

  INT_ASSERT(mState != kUnresolved);

  if (mFnDecl->formals.length == (int) actuals.size())
  {
    for (int i = 0; i < mFnDecl->formals.length && retval == true; i++)
    {
      DefExpr*   defExpr = toDefExpr(mFnDecl->formals.get(i + 1));
      ArgSymbol* formal  = toArgSymbol(defExpr->sym);
      Type*      type    = NULL;

      if (CallExpr* sel = toCallExpr(actuals[i]))
      {
        IpeCallExpr* callExpr = (IpeCallExpr*) sel;

        type = callExpr->typeGet();
      }
      else
      {
        type = actuals[i]->typeInfo();
      }

      retval = (formal->type == type) ? true : false;
    }
  }
  else
    retval = false;

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

IpeValue IpeMethod::apply(IpeCallExpr* callingExpr, IpeEnv* callingEnv)
{
  IpeValue retval;

#if 0
  static int sCount = 0;
  int        count  = sCount;

  sCount = sCount + 1;

  printf("\n\n\n\n\n");
  printf("   IpeMethod::apply          00100 %5d\n", count);

  printf("   Method:\n");
  describe(3, true);
  printf("\n");

  printf("   callingExpr:\n");
  callingExpr->describe(3);
  printf("\n");

  printf("   callingEnv:\n");
  callingEnv->describe(3);
  printf("\n");
  printf("\n\n\n");
#endif

  if (resolveBody() == true)
  {
    void*  frame = (mFrameSize > 0) ? malloc(mFrameSize) : NULL;
    IpeEnv newEnv(mEnvParent, mScope, mFrameSize, frame);

    if (frame)
      memset(frame, 0, mFrameSize);

    // Evaluate the actuals in the calling environment
    for (int i = 1; i <= callingExpr->numActuals(); i++)
    {
      Expr*      actual     = callingExpr->get(i);
      IpeValue   value      = evaluateExpr(actual, callingEnv);

      Expr*      formalExpr = mFnDecl->formals.get(i);
      DefExpr*   formalDef  = toDefExpr(formalExpr);
      ArgSymbol* formal     = toArgSymbol(formalDef->sym);

      newEnv.store(formal, value);
    }

    if (isExternFunction(mFnDecl) == true)
    {
      retval = externFunctionInvoke(&newEnv);
    }

    else
    {
#if 0
      printf("   IpeMethod::apply          00700 %5d\n", count);

      printf("      Resolved Body\n");
      mBody->describe(6);
      printf("\n");

      printf("      New Env\n");
      newEnv.describe(6);
      printf("\n");
#endif

      retval = evaluateExpr(mBody, &newEnv);

#if 0
      printf("   IpeMethod::apply          00750 %5d\n", count);
#endif
    }

    mInvokeCount = mInvokeCount + 1;

    if (frame != NULL)
      free(frame);
  }

  else
  {
    // NOAKES 2015/04/15 Generate a thoughtful error message
    INT_ASSERT(false);
  }

  return retval;
}

bool IpeMethod::resolveBody()
{
  if (mState == kResolvedReturnType)
  {
    int             offset = 0;
    int             delta  = 8;           // NOAKES  Size of every type is currently 8
    IpeScopeMethod* scope  = new IpeScopeMethod(this, mEnvParent->scopeGet());
    IpeEnv          env(mEnvParent, scope);

    for_alist(formalExpr, mFnDecl->formals)
    {
      DefExpr*   defExpr = toDefExpr(formalExpr);
      ArgSymbol* formal  = toArgSymbol(defExpr->sym);

      formal->locationSet(env.depth(), offset);
      offset = offset + delta;

      env.argAdd(formal);
    }

    mScope     = scope;
    mBody      = blockResolve(mFnDecl->body, &env);

    INT_ASSERT(mScope);
    INT_ASSERT(mBody);

    if (bodyIsSimple(mBody) == false)
    {
      printf("   About to fail\n");
      mBody->describe(3);
      printf("\n\n");

      INT_ASSERT(false);
    }

    else
    {
      for (int i = 1; i <= mBody->body.length; i++)
      {
        Expr* expr = mBody->body.get(i);

        if      (DefExpr* defExpr = toDefExpr(expr))
        {
          VarSymbol* var = toVarSymbol(defExpr->sym);

          INT_ASSERT(var);

          var->locationSet(env.depth(), offset);

          offset = offset + delta;
        }
      }

      mFrameSize = offset;
    }

    mState = kResolved;
  }

  return (mState == kResolved) ? true : false;
}

bool IpeMethod::bodyIsSimple(IpeSequence* stmt) const
{
  bool retval = true;

  for (int i = 1; i <= stmt->body.length && retval == true; i++)
  {
    Expr* expr = stmt->body.get(i);

    if      (DefExpr* defExpr = toDefExpr(expr))
    {
      retval = isVarSymbol(defExpr->sym);
    }

    else if (isUseStmt(expr)   == true)
    {
      retval = false;
    }

    else if (WhileDoStmt* whileDoStmt = toWhileDoStmt(expr))
    {
      for (int i = 1; i <= whileDoStmt->body.length && retval == true; i++)
      {
        Expr* expr = whileDoStmt->body.get(i);

        retval = (isDefExpr(expr) == true) ? false : true;
      }
    }

    else if (isBlockStmt(expr) == true)
    {
      IpeSequence* seq = (IpeSequence*) expr;

      retval = seq->isScopeless();
    }
  }

  return retval;
}

#if 0
void IpeMethod::resolveBody()
{
  IpeScopeMethod* scope = new IpeScopeMethod(this, mEnvParent->scopeGet());
  IpeEnv          env(mEnvParent, scope);

  for_alist(formalExpr, mFnDecl->formals)
  {
    DefExpr*   defExpr = toDefExpr(formalExpr);
    ArgSymbol* formal  = toArgSymbol(defExpr->sym);

    env.argAdd(formal);
  }

  mScope = scope;
  mBody  = resolveExpr(mFnDecl->body, &env);
}
#endif

#if 0
int IpeMethod::assignLocations() const
{
#if 0
  int    delta = 8;           // NOAKES  Size of every type is currently 8
  IpeEnv env(mScope);

  for_alist(formalExpr, mFnDecl->formals)
  {
    DefExpr*   defExpr = toDefExpr(formalExpr);
    ArgSymbol* formal  = toArgSymbol(defExpr->sym);

    env.locationSet(formal);
  }

  return delta * mFnDecl->formals.length + locationExpr(mBody, &env);
#else

  INT_ASSERT(false);

  return 0;

#endif
}
#endif

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

bool IpeMethod::isExternFunction(FnSymbol* fn) const
{
  return fn->hasFlag(FLAG_EXTERN);
}

IpeValue IpeMethod::externFunctionInvoke(IpeEnv* env) const
{
  IpeValue retval;

  if      (isQuit()    == true)
  {
    clean_exit(0);
  }

  else if (isWriteln() == true)
  {
    if (mFnDecl->formals.length == 1)
    {
      DefExpr*   defExpr1 = toDefExpr(mFnDecl->formals.get(1));
      ArgSymbol* formal1  = toArgSymbol(defExpr1->sym);

      writeln1(env->fetch(formal1), formal1->type);
    }

    else
    {
      DefExpr*   defExpr1 = toDefExpr(mFnDecl->formals.get(1));
      ArgSymbol* formal1  = toArgSymbol(defExpr1->sym);

      DefExpr*   defExpr2 = toDefExpr(mFnDecl->formals.get(2));
      ArgSymbol* formal2  = toArgSymbol(defExpr2->sym);

      writeln2(env->fetch(formal1), env->fetch(formal2), formal2->type);
    }
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   IpeMethod::externFunctionInvoke(IpeEnv*)  unsupported\n");
    printf("   ");
    mFnDecl->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

bool IpeMethod::isQuit() const
{
  bool retval = false;

  if (strcmp(mFnDecl->name, "quit") == 0)
  {
    if (ModuleSymbol* mod = mFnDecl->getModule())
      retval = (strcmp(mod->name, "ChapelBase") == 0) ? true : false;
  }

  return retval;
}

bool IpeMethod::isWriteln() const
{
  bool retval = false;

  if (strcmp(mFnDecl->name, "writeln") == 0)
  {
    if (ModuleSymbol* mod = mFnDecl->getModule())
      retval = (strcmp(mod->name, "ChapelBase") == 0) ? true : false;
  }

  return retval;
}

void IpeMethod::writeln1(IpeValue value, Type* type) const
{
  printf("     ");

  if      (type == dtBool)
    printf("%s\n", (value.boolGet() == true) ? " true" : "false");

  else if (type == dtInt[INT_SIZE_64])
    printf("%5ld\n", value.integerGet());

  else if (type == dtReal[FLOAT_SIZE_64])
    printf("%6.2f\n", value.realGet());

  else if (type == dtStringC)
    printf("%s\n", value.cstringGet());

  else
    printf("???\n");
}

void IpeMethod::writeln2(IpeValue msg, IpeValue value, Type* type) const
{
  printf("     ");
  fputs(msg.cstringGet(), stdout);

  if      (type == dtBool)
    printf("%s\n", (value.boolGet() == true) ? " true" : "false");

  else if (type == dtInt[INT_SIZE_64])
    printf("%5ld\n", value.integerGet());

  else if (type == dtReal[FLOAT_SIZE_64])
    printf("%6.2f\n", value.realGet());

  else if (type == dtStringC)
    printf("%s\n", value.cstringGet());

  else
    printf("???\n");
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void IpeMethod::describe(int offset, bool fullP) const
{
  char pad[32] = { '\0' };

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  if (fullP == false)
  {
    printf("%s#<IpeMethod %-20s %-20s>\n", pad, name(), stateAsString());
  }
  else
  {
    AstDumpToNode logger(stdout, offset + 5);

    printf("%s#<IpeMethod %s\n", pad, name());
    printf("%s  State:   %s\n",  pad, stateAsString());
    printf("%s  Frame:   %4d bytes\n",  pad, mFrameSize);

    if (mInvokeCount == 1)
      printf("%s  Invoked: %4d time\n",  pad, mInvokeCount);
    else
      printf("%s  Invoked: %4d times\n", pad, mInvokeCount);

    if (mEnvParent)
    {
      printf("\n");
      printf("%s  EnvParent:\n", pad);
      mEnvParent->describe(offset + 5);
    }
    else
    {
      printf("%s  ParentScope is EMPTY\n", pad);
    }

    printf("\n");
    printf("%s  Decl:\n", pad);
    printf("%s     ", pad);
    mFnDecl->accept(&logger);
    printf("\n");

    if (mBody != NULL)
    {
      printf("\n");
      printf("%s  Resolved Body:\n", pad);

      if      (isWhileStmt(mBody) == true)
      {
        printf("%s     ", pad);
        mBody->accept(&logger);
        printf("\n");
      }

      else if (isBlockStmt(mBody) == true)
      {
        IpeSequence* seq = (IpeSequence*) mBody;

        seq->describe(offset + 5);
      }

      else
      {
        printf("%s     ", pad);
        mBody->accept(&logger);
        printf("\n");
      }
    }

    printf("%s>\n", pad);
  }
}
