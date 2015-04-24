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

#include "IpeEnv.h"

#include "AstDumpToNode.h"

#include "IpeModule.h"
#include "IpeProcedure.h"
#include "IpeScope.h"
#include "IpeValue.h"

#include "ipeDriver.h"
#include "misc.h"
#include "symbol.h"

#include <cstdio>

char*  IpeEnv::gMemoryData = NULL;
size_t IpeEnv::gMemoryTail =    0;
size_t IpeEnv::gMemorySize =    0;

IpeEnv::IpeEnv(IpeScope* scope)
{
  mScope = scope;
  mData  =     0;
}

IpeEnv::IpeEnv(IpeScope* scope, void* data)
{
  mScope = scope;
  mData  =  data;
}

IpeEnv::~IpeEnv()
{

}

IpeScope* IpeEnv::scopeGet() const
{
  return mScope;
}

bool IpeEnv::isEnvForBlockStmt() const
{
  return mScope->isScopeBlockStmt();
}

bool IpeEnv::isEnvForModule() const
{
  return mScope->isScopeModule();
}

int IpeEnv::frameSize() const
{
  return mScope->frameSize();
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

bool IpeEnv::globalsAllocate(size_t initialSize)
{
  gMemoryData = (char*) malloc(initialSize);
  gMemoryTail = 0;
  gMemorySize = initialSize;

  INT_ASSERT((((long) gMemoryData) % 8) == 0);

  return true;
}

void IpeEnv::globalsDeallocate()
{
  free(gMemoryData);

  gMemoryData = NULL;
  gMemoryTail =    0;
  gMemorySize =    0;
}

int IpeEnv::reserveSpace(VarSymbol* var)
{
  long* ptr    = (long*) (gMemoryData + gMemoryTail);
  int   delta  = 8;             // Currently, everything is 8 bytes
  int   retval = gMemoryTail;

  INT_ASSERT(gMemoryTail + delta <= gMemorySize);

  *ptr        = 0;
  gMemoryTail = gMemoryTail + delta;

  return retval;
}

int IpeEnv::allocateValue(bool value)
{
  long iValue = (value == true) ? 1 : 0;

  return allocateValue(iValue);
}

int IpeEnv::allocateValue(long value)
{
  long*  ptr    = (long*)  (gMemoryData + gMemoryTail);
  int    retval = (int)    gMemoryTail;

  INT_ASSERT(gMemoryTail + sizeof(value) <= gMemorySize);

  *ptr        = value;
  gMemoryTail = gMemoryTail + sizeof(value);

  return retval;
}

int IpeEnv::allocateValue(void* value)
{
  void** ptr    = (void**) (gMemoryData + gMemoryTail);
  int    retval = (int)    gMemoryTail;

  INT_ASSERT(gMemoryTail + sizeof(value) <= gMemorySize);

  *ptr        = value;
  gMemoryTail = gMemoryTail + sizeof(value);

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

IpeValue IpeEnv::addrOfFoo(VarSymbol* sym)
{
  return IpeValue((IpeValue*) (gMemoryData + sym->offset()));
}

IpeValue IpeEnv::fetchFoo(LcnSymbol* sym)
{
  IpeValue retval;

  if      (sym->type == dtBools[BOOL_SIZE_SYS])
    retval = IpeValue((fetchBool(sym) == true) ? 1L : 0L);

  else if (sym->type == dtInt[INT_SIZE_64])
    retval = IpeValue(fetchInteger(sym));

  else if (sym->type == dtReal[FLOAT_SIZE_64])
    retval = IpeValue(fetchReal(sym));

  else if (sym->type == gIpeTypeType)
    retval = IpeValue((Type*) fetchPtr(sym));

  else if (sym->type == gIpeTypeProcedure)
    retval = IpeValue((IpeProcedure*) fetchPtr(sym));

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n");
    printf("   IpeEnv::fetch(LcnSymbol*) Unsupported\n");

    printf("   ");
    sym->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);

  }

  return retval;
}

bool IpeEnv::fetchBool(LcnSymbol* sym)
{
  long value = fetchInteger(sym);

  return (value == 1) ? true : false;
}

long IpeEnv::fetchInteger(LcnSymbol* sym)
{
  INT_ASSERT(sym->offset() >= 0);

  return *((long*) (gMemoryData + sym->offset()));
}

double IpeEnv::fetchReal(LcnSymbol* sym)
{
  INT_ASSERT(sym->offset() >= 0);

  return *((double*) (gMemoryData + sym->offset()));
}

void* IpeEnv::fetchPtr(LcnSymbol* sym)
{
  INT_ASSERT(sym->offset() >= 0);

  return *((void**) (gMemoryData + sym->offset()));
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void IpeEnv::storeFoo(LcnSymbol* sym, IpeValue value)
{
  if      (sym->type == dtBools[BOOL_SIZE_SYS])
  {
    storeBool(sym, value.boolGet());
  }

  else if (sym->type == dtInt[INT_SIZE_64])
  {
    storeInteger(sym, value.integerGet());
  }

  else if (sym->type == dtReal[FLOAT_SIZE_64])
  {
    storeReal(sym, value.realGet());
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n");
    printf("   IpeEnv::store(LcnSymbol*) Unsupported\n");

    printf("   ");
    sym->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);

  }
}

void IpeEnv::storeBool(LcnSymbol* sym, IpeValue value)
{
  INT_ASSERT(sym->offset() >= 0);

  *((long*) (gMemoryData + sym->offset())) = (value.boolGet() == true) ? 1L : 0L;
}

void IpeEnv::storeInteger(LcnSymbol* sym, IpeValue value)
{
  INT_ASSERT(sym->offset() >= 0);

  *((long*) (gMemoryData + sym->offset())) = value.integerGet();
}

void IpeEnv::storeReal(LcnSymbol* sym, IpeValue value)
{
  INT_ASSERT(sym->offset() >= 0);

  *((double*) (gMemoryData + sym->offset())) = value.realGet();
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

bool IpeEnv::isDefinedLocally(const char* name) const
{
  return mScope->isDefinedLocally(name);
}

void IpeEnv::varAdd(VarSymbol* var) const
{
  INT_ASSERT(mScope);

  mScope->varAdd(var);
}

void IpeEnv::argAdd(ArgSymbol* arg) const
{
  INT_ASSERT(mScope);

  mScope->varAdd(arg);
}

LcnSymbol* IpeEnv::findLocal(const char* name) const
{
  return findVariable(name, true);
}

LcnSymbol* IpeEnv::findVariable(UnresolvedSymExpr* expr, bool localOnly) const
{
  return mScope->findVariable(expr, localOnly);
}

LcnSymbol* IpeEnv::findVariable(const char* name, bool localOnly) const
{
  return mScope->findVariable(name, localOnly);
}

void IpeEnv::useAdd(IpeModule* use)
{
  mScope->useAdd(use);
}

void IpeEnv::initializeUsedModules() const
{
  int useCount = mScope->useCount();

  for (int i = 0; i < useCount; i++)
    mScope->useGet(i)->initialize();
}

void IpeEnv::allocateSpace(VarSymbol* var) const
{
  // Currently, everything is 8 bytes
  int delta  = 8;

  INT_ASSERT(gMemoryTail + delta <= gMemorySize);

  var->locationSet(0, gMemoryTail);
  varAdd(var);

  gMemoryTail = gMemoryTail + delta;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

IpeValue IpeEnv::addrOf(VarSymbol* sym) const
{
  IpeValue retval;

  if      (sym->depth() == 0)
  {
    retval = IpeEnv::addrOfFoo(sym);
  }

  else if (sym->depth() == 1 && mData != 0)
  {
    if (sym->type == dtBools[BOOL_SIZE_SYS])
    {
      long* src = (long*) (((char*) mData) + sym->offset());

      retval = IpeValue((IpeValue*) src);
    }

    else if (sym->type == dtInt[INT_SIZE_64])
    {
      long* src = (long*) (((char*) mData) + sym->offset());

      retval = IpeValue((IpeValue*) src);
    }

    else if (sym->type == dtReal[FLOAT_SIZE_64])
    {
      double* src = (double*) (((char*) mData) + sym->offset());

      retval = IpeValue((IpeValue*) src);
    }

    else if (sym->type == dtStringC)
    {
      const char** src = (const char**) (((char*) mData) + sym->offset());

      retval = IpeValue((IpeValue*) src);
    }

    else
    {
      AstDumpToNode logger(stdout, 3);

      printf("IpeEnv::addrOf(VarSymbol*) Unsupported\n");

      printf("   ");
      sym->accept(&logger);
      printf("\n\n");

      INT_ASSERT(false);
    }
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("IpeEnv::addrOf(ArgSymbol*) Unsupported\n");

    printf("   ");
    sym->accept(&logger);
    printf("\n\n");
    printf("   in\n\n");
    describe(3);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

IpeValue IpeEnv::fetch(LcnSymbol* sym) const
{
  IpeValue retval;

  if      (sym->depth() == 0)
  {
    retval = IpeEnv::fetchFoo(sym);
  }

  else if (sym->depth() == 1 && mData != 0)
  {
    if (sym->type == dtBools[BOOL_SIZE_SYS])
    {
      long* src = (long*) (((char*) mData) + sym->offset());

      retval = IpeValue((*src == 1) ? true : false);
    }

    else if (sym->type == dtInt[INT_SIZE_64])
    {
      long* src = (long*) (((char*) mData) + sym->offset());

      retval = IpeValue(*src);
    }

    else if (sym->type == dtReal[FLOAT_SIZE_64])
    {
      double* src = (double*) (((char*) mData) + sym->offset());

      retval = IpeValue(*src);
    }

    else if (sym->type == dtStringC)
    {
      const char** src = (const char**) (((char*) mData) + sym->offset());

      retval = IpeValue(*src);
    }

    else
    {
      AstDumpToNode logger(stdout, 3);

      printf("IpeEnv::fetch(LcnSymbol*) Unsupported\n");

      printf("   ");
      sym->accept(&logger);
      printf("\n\n");

      INT_ASSERT(false);
    }
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("IpeEnv::fetch(LcnSymbol*) Unsupported\n");

    printf("   ");
    sym->accept(&logger);
    printf("\n\n");
    printf("   in\n\n");
    describe(3);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void IpeEnv::assign(ArgSymbol* sym, IpeValue value)
{
  if ((sym->intent & INTENT_REF) != 0)
  {
    if      (sym->depth() == 0)
    {
      AstDumpToNode logger(stdout, 3);

      printf("IpeEnv::assign(ArgSymbol*, IpeValue) Unsupported\n");

      printf("   ");
      sym->accept(&logger);
      printf("\n\n");

      INT_ASSERT(false);
    }

    else if (sym->depth() == 1 && mData != 0)
    {
      assign((LcnSymbol*) sym, value);
    }

    else
    {
      AstDumpToNode logger(stdout, 3);

      printf("IpeEnv::assign(ArgSymbol*, IpeValue) Unsupported\n");

      printf("   ");
      sym->accept(&logger);
      printf("\n\n");

      INT_ASSERT(false);
    }
  }

  else
  {
    assign((LcnSymbol*) sym, value);
  }
}

void IpeEnv::assign(VarSymbol* sym, IpeValue value)
{
  assign((LcnSymbol*) sym, value);
}

void IpeEnv::assign(LcnSymbol* sym, IpeValue value)
{
  if      (sym->depth() == 0)
  {
    IpeEnv::storeFoo(sym, value);
  }

  else if (sym->depth() == 1 && mData != 0)
  {
    if (sym->type == dtBools[BOOL_SIZE_SYS])
    {
      long* dst = (long*) (((char*) mData) + sym->offset());

      *dst = (value.boolGet() == true) ? 1L : 0L;
    }

    else if (sym->type == dtInt[INT_SIZE_64])
    {
      long* dst = (long*) (((char*) mData) + sym->offset());

      *dst = value.integerGet();
    }

    else if (sym->type == dtReal[FLOAT_SIZE_64])
    {
      double* dst = (double*) (((char*) mData) + sym->offset());

      *dst = value.realGet();
    }

    else if (sym->type == dtStringC)
    {
      const char** dst = (const char**) (((char*) mData) + sym->offset());

      *dst = value.cstringGet();
    }

    else
    {
      AstDumpToNode logger(stdout, 3);

      printf("\n\n");
      printf("   IpeEnv::assign(LcnSymbol*, IpeValue) Unsupported\n");

      printf("   ");
      sym->accept(&logger);
      printf("\n\n");

      INT_ASSERT(false);
    }
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("\n\n");
    printf("   IpeEnv::assign(LcnSymbol*, IpeValue) Unsupported\n");

    printf("   ");
    sym->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }
}

IpeValue IpeEnv::valueForVariable(LcnSymbol* lcn) const
{
  IpeValue retval;

  if (lcn->isImmediate() == true)
  {
    VarSymbol* var  = toVarSymbol(lcn);
    Immediate* imm  = var->immediate;
    Type*      type = var->type;

    INT_ASSERT(type);
    INT_ASSERT(type->symbol);
    INT_ASSERT(type->symbol->name);

    if      (strcmp(type->symbol->name, "bool")     == 0)
      retval.boolSet(imm->v_bool);

    else if (strcmp(type->symbol->name, "int")      == 0)
      retval.integerSet(imm->v_int64);

    else if (strcmp(type->symbol->name, "real")     == 0)
      retval.realSet(imm->v_float64);

    else if (strcmp(type->symbol->name, "c_string") == 0)
      retval.cstringSet(imm->v_string);

    else
      INT_ASSERT(false);
  }

  else
    retval = valueFetch(lcn);

  return retval;
}

IpeValue IpeEnv::valueFetch(LcnSymbol* sym) const
{
  char*    base = (sym->depth() == 0) ? gMemoryData : (char*) mData;
  IpeValue retval;

  if      (base == NULL)
    INT_ASSERT(false);

  else if (sym->offset() < 0)
    INT_ASSERT(false);

  else if (sym->type == dtUnknown)
    INT_ASSERT(false);

  else if (sym->type == gIpeTypeType)
    retval = IpeValue((Type*) fetchPtr(sym));

  else if (sym->type == dtBools[BOOL_SIZE_SYS])
  {
    long* ptr = (long*) (base + sym->offset());

    retval = IpeValue((*ptr == 1) ? true : false);
  }

  else if (sym->type == dtInt[INT_SIZE_64])
  {
    long* ptr = (long*) (base + sym->offset());

    retval = IpeValue(*ptr);
  }

  else if (sym->type == dtReal[FLOAT_SIZE_64])
  {
    double* ptr = (double*) (base + sym->offset());

    retval = IpeValue(*ptr);
  }

  else if (sym->type == gIpeTypeProcedure)
  {
    IpeProcedure** ptr = (IpeProcedure**) (base + sym->offset());

    retval = IpeValue(*ptr);
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   IpeModule::valueFetch(LcnSymbol*)  unsupported\n");
    printf("   ");
    sym->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }

  return retval;
}

void IpeEnv::valueStore(LcnSymbol* sym, IpeValue value) const
{
  char*    base = (sym->depth() == 0) ? gMemoryData : (char*) mData;

  INT_ASSERT(sym->offset() >= 0);

  if      (base == NULL)
    INT_ASSERT(false);

  else if (sym->offset() < 0)
    INT_ASSERT(false);

  else if (sym->type == dtUnknown)
    INT_ASSERT(false);

  else if (sym->type == dtBools[BOOL_SIZE_SYS])
  {
    long* ptr = (long*) (base + sym->offset());

    *ptr = (value.boolGet() == true) ? 1 : 0;
  }

  else if (sym->type == dtInt[INT_SIZE_64])
  {
    long* ptr = (long*) (base + sym->offset());

    *ptr = value.integerGet();
  }

  else if (sym->type == dtReal[FLOAT_SIZE_64])
  {
    double* ptr = (double*) (base + sym->offset());

    *ptr = value.realGet();
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   IpeModule::valueStore(LcnSymbol*, IpeValue value)  unsupported\n");
    printf("   ");
    sym->accept(&logger);
    printf("\n\n");

    INT_ASSERT(false);
  }
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void IpeEnv::describe(int offset) const
{
  int  useCount = mScope->useCount();
  int  varCount = mScope->varCount();
  char pad[32]  = { '\0' };

  if (offset < 32)
  {
    char* tptr = pad;

    for (int i = 0; i < offset; i++)
      *tptr++ = ' ';

    *tptr = '\0';
  }

  printf("%s#<IpeEnv 0x%012lX  %-9s %s\n", pad, (long) this, mScope->type(), mScope->name());

  if (mData)
    printf("%s   data: 0x%012lX\n", pad, (long) mData);

  if (mScope->hasParent() == true)
    mScope->parentSummarize(pad);

  for (int i = 0; i < useCount; i++)
  {
    if (i == 0)
      printf("%s  uses: %s\n", pad, mScope->useGet(i)->name());
    else
      printf("%s        %s\n", pad, mScope->useGet(i)->name());
  }

  for (int i = 0; i < varCount; i++)
    describe(pad, i, mScope->varGet(i));

  printf("%s>\n", pad);
}

void IpeEnv::describe(const char* pad, int index, LcnSymbol* var) const
{
  const char* symName  = var->name;
  const char* typeName = "";
  ArgSymbol*  argSym   = toArgSymbol(var);
  VarSymbol*  varSym   = toVarSymbol(var);

  if (var->type)
    typeName = var->type->symbol->name;

  printf("%s %5d: %-30s", pad, index, symName);

  if (argSym != NULL)
  {
    if (argSym->intent & INTENT_REF)
      printf("ref   %-12s", typeName);
    else
      printf("arg   %-12s", typeName);
  }

  else if (varSym->immediate != NULL)
    printf("const %-12s", typeName);

  else if (varSym->hasFlag(FLAG_CONST))
    printf("const %-12s", typeName);

  else if (varSym->hasFlag(FLAG_PARAM))
    printf("param %-12s", typeName);

  else
    printf("var   %-12s", typeName);

  if (var->depth() >= 0 && var->offset() >= 0)
    printf("%4d %4d ", var->depth(), var->offset());
  else
    printf("          ");

  if (argSym != 0 && (argSym->intent & INTENT_REF) != 0)
  {
    if (mData != NULL)
    {
      int       offset = argSym->offset();
      IpeValue* ref    = *((IpeValue**) (((char*) mData) + offset));

      printf("0x%012lX", (long) ref);
    }
  }

  else if (var->offset() <    0)
    ;

  else if (var->type     == NULL)
    ;

  else if (var->type     == gIpeTypeType)
    printf("%s", symName);

  else if (var->type     == dtBool)
  {
    if (var->depth() == 0)
    {
      printf("%s", (IpeEnv::fetchBool(var) == true) ?  "true" : "false");
    }
    else
    {
      if (mData != NULL)
      {
        long value = *((long*) (((char*) mData) + var->offset()));

        printf("%-8s", (value == 1) ? "true" : "false");
      }
      else
        printf("Waaaa bool");
    }
  }

  else if (var->type     == dtInt[INT_SIZE_64])
  {
    if (var->depth() == 0)
      printf("%8ld", IpeEnv::fetchInteger(var));

    else if (argSym != 0)
    {
      if (mData != NULL)
      {
        long value = *((long*) (((char*) mData) + argSym->offset()));

        printf("%8ld", value);
      }
    }

    else if (varSym != 0)
    {
      if (mData != NULL)
      {
        long value = *((long*) (((char*) mData) + varSym->offset()));

        printf("%8ld", value);
      }
    }

    else
    {
      printf("Waaaa 02");
    }
  }

  else if (var->type     == dtReal[FLOAT_SIZE_64])
  {
    if (var->depth() == 0)
      printf("   %8.2f", fetchReal(var));
    else
      printf("Waaaa");
  }

  else if (var->type     == gIpeTypeModule)
  {
    if (var->depth() == 0)
    {
      IpeModule* value = (IpeModule*) fetchPtr(var);

      printf("#<IpeModule    %-20s 0x%012lX>", value->name(), (long) value);

    }
    else
    {
      printf("Waaaa");
    }
  }

  else if (var->type     == gIpeTypeProcedure)
  {
    if (var->depth() == 0)
    {
      IpeProcedure* value = (IpeProcedure*) fetchPtr(var);

      if (value->methodCount() == 1)
        printf("#<IpeProcedure %-20s with %3d method  0x%012lX>",
               value->name(),
               value->methodCount(),
               (long) value);
      else
        printf("#<IpeProcedure %-20s with %3d methods 0x%012lX>",
               value->name(),
               value->methodCount(),
               (long) value);
    }
    else
      printf("Waaaa");
  }

  else
    printf("Really??  Really??");

  printf("\n");
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

int IpeEnv::locationSet(ArgSymbol* arg) const
{
  return mScope->locationSet(arg);
}

int IpeEnv::locationSet(VarSymbol* var) const
{
  return mScope->locationSet(var);
}
