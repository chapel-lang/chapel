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

#include "IpeModule.h"
#include "IpeProcedure.h"
#include "IpeScope.h"
#include "IpeValue.h"

#include "ipeDriver.h"

#include "AstDumpToNode.h"
#include "expr.h"
#include "misc.h"
#include "symbol.h"

#include <cstdio>

//
// Module level variables are allocted in a shared dynamic store
//
char* IpeEnv::sStoreData = NULL;
int   IpeEnv::sStoreTail =    0;
int   IpeEnv::sStoreSize =    0;

// Create an environment that implicitly references the ModuleStore
IpeEnv::IpeEnv(IpeEnv* parent, IpeScope* scope)
{
  mParent    = parent;
  mDepth     = (parent == NULL || scope->isScopeModule() == true) ? 0 : parent->mDepth + 1;
  mScope     = scope;

  mFrameData =  NULL;
  mFrameSize =    -1;
  mFrameTail =    -1;
}

// Create a dynamic environment with a specified initial size
IpeEnv::IpeEnv(IpeEnv* parent, IpeScope* scope, int frameSize)
{
  mParent    = parent;
  mDepth     = (parent == NULL) ? 0 : parent->mDepth + 1;
  mScope     = scope;

  mFrameData = (frameSize > 0) ? (char*) malloc(frameSize) : NULL;
  mFrameSize = frameSize;
  mFrameTail = 0;
}

// Create a static environment with a specified frame
IpeEnv::IpeEnv(IpeEnv* parent, IpeScope* scope, int frameSize, void* frameData)
{
  mParent    = parent;
  mDepth     = (parent == NULL) ? 0 : parent->mDepth + 1;
  mScope     = scope;

  mFrameData = (char*) frameData;
  mFrameSize = frameSize;
  mFrameTail = -1;
}

IpeEnv::~IpeEnv()
{

}

IpeScope* IpeEnv::scopeGet() const
{
  return mScope;
}

bool IpeEnv::isEnvForModule() const
{
  return mScope->isScopeModule();
}

bool IpeEnv::isEnvForBlockStmt() const
{
  return mScope->isScopeBlockStmt();
}

int IpeEnv::frameSize() const
{
  return mScope->frameSize();
}

int IpeEnv::depth() const
{
  return mDepth;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

bool IpeEnv::globalsAllocate(size_t initialSize)
{
  sStoreData = (char*) malloc(initialSize);
  sStoreTail = 0;
  sStoreSize = initialSize;

  INT_ASSERT((((long) sStoreData) % 8) == 0);

  return true;
}

void IpeEnv::globalsDeallocate()
{
  free(sStoreData);

  sStoreData = NULL;
  sStoreTail =    0;
  sStoreSize =    0;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void IpeEnv::varAdd(VarSymbol* var) const
{
  INT_ASSERT(mScope);

  mScope->varAdd(var);
}

int IpeEnv::varCount() const
{
  INT_ASSERT(mScope);

  return mScope->varCount();
}

LcnSymbol* IpeEnv::varGet(int index) const
{
  INT_ASSERT(mScope);

  return mScope->varGet(index);
}

void IpeEnv::argAdd(ArgSymbol* arg) const
{
  INT_ASSERT(mScope);

  mScope->varAdd(arg);
}

void IpeEnv::useAdd(IpeModule* use)
{
  INT_ASSERT(mScope);

  mScope->useAdd(use);
}

int IpeEnv::useCount() const
{
  INT_ASSERT(mScope);

  return mScope->useCount();
}

IpeModule* IpeEnv::useGet(int index) const
{
  INT_ASSERT(mScope);

  return mScope->useGet(index);
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

LcnSymbol* IpeEnv::findLocal(const char* name) const
{
  return findVariable(name, true);
}

LcnSymbol* IpeEnv::findVariable(const char* name) const
{
  return findVariable(name, false);
}

LcnSymbol* IpeEnv::findVariable(const char* identifier, bool localOnly) const
{
  int        varCount = mScope->varCount();
  LcnSymbol* retval   = NULL;

  for (int i = 0; i < varCount && retval == NULL; i++)
  {
    LcnSymbol* lcn = mScope->varGet(i);

    retval = (lcn->name == identifier) ? lcn : NULL;
  }

  // Possibly search over the used Modules
  if (retval == NULL && localOnly == false)
  {
    std::vector<LcnSymbol*> usedMatches;
    int                     useCount = mScope->useCount();

    for (int i = 0; i < useCount; i++)
    {
      IpeModule* mod = mScope->useGet(i);
      IpeEnv*    env = mod->environment();

      if (LcnSymbol* var = env->findVariable(identifier))
        usedMatches.push_back(var);
    }

    if (usedMatches.size() > 0)
      retval = usedMatches[0];

    for (size_t i = 1; i < usedMatches.size(); i++)
    {
      if (retval != usedMatches[i])
        printf("IpeScope::findVariable %s   multiple definitions\n", identifier);
    }
  }

  // Possibly recurse to the parent
  if (retval == NULL && localOnly == false && mParent != NULL)
    retval = mParent->findVariable(identifier, false);

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

bool IpeEnv::fetchBool(LcnSymbol* sym) const
{
  long value = fetchInteger(sym);

  return (value == 1) ? true : false;
}

long IpeEnv::fetchInteger(LcnSymbol* sym) const
{
  int  depth  = sym->depth();
  long retval = 0;

  if      (depth <  mDepth)
  {
    retval = mParent->fetchInteger(sym);
  }

  else if (depth == mDepth)
  {

    int   offset = sym->offset();
    long* ptr    = NULL;

    // The global environment
    if      (mFrameSize == -1 && mFrameTail == -1)
    {
      INT_ASSERT(offset >= 0 && offset < sStoreTail);

      ptr = (long*) (sStoreData + offset);
    }

    // A dynamic environment
    else if (mFrameSize >=  0 && mFrameTail >=  0)
    {
      INT_ASSERT(offset >= 0 && offset < mFrameTail);

      ptr = (long*) (mFrameData + offset);
    }

    // A static  environment
    else if (mFrameSize >=  0 && mFrameTail == -1)
    {
      INT_ASSERT(offset >= 0 && offset < mFrameSize);

      ptr = (long*) (mFrameData + offset);
    }

    // Invalid
    else
    {
      INT_ASSERT(false);
    }

    retval = *ptr;
  }

  else
  {
    INT_ASSERT(false);
  }

  return retval;
}

double IpeEnv::fetchReal(LcnSymbol* sym) const
{
  int    depth  = sym->depth();
  double retval = 0.0;

  if      (depth <  mDepth)
  {
    retval = mParent->fetchReal(sym);
  }

  else if (depth == mDepth)
  {
    int     offset = sym->offset();
    double* ptr    = NULL;

    // The global environment
    if      (mFrameSize == -1 && mFrameTail == -1)
    {
      INT_ASSERT(offset >= 0 && offset < sStoreTail);

      ptr = (double*) (sStoreData + offset);
    }

    // A dynamic environment
    else if (mFrameSize >=  0 && mFrameTail >=  0)
    {
      INT_ASSERT(offset >= 0 && offset < mFrameTail);

      ptr = (double*) (mFrameData + offset);
    }

    // A static  environment
    else if (mFrameSize >=  0 && mFrameTail == -1)
    {
      INT_ASSERT(offset >= 0 && offset < mFrameSize);

      ptr = (double*) (mFrameData + offset);
    }

    // Invalid
    else
    {
      INT_ASSERT(false);
    }

    retval = *ptr;
  }

  else
  {
    INT_ASSERT(false);
  }

  return retval;
}

void* IpeEnv::fetchPtr(LcnSymbol* sym) const
{
  int    depth  = sym->depth();
  void*  retval = NULL;

  if      (depth <  mDepth)
  {
    retval = mParent->fetchPtr(sym);
  }

  else if (depth == mDepth)
  {
    int    offset = sym->offset();
    void** ptr    = NULL;

    // The global environment
    if      (mFrameSize == -1 && mFrameTail == -1)
    {
      INT_ASSERT(depth == 0);
      INT_ASSERT(offset >= 0 && offset < sStoreTail);

      ptr = (void**) (sStoreData + sym->offset());
    }

    // A dynamic environment
    else if (mFrameSize >=  0 && mFrameTail >=  0)
    {
      INT_ASSERT(depth  >  0);
      INT_ASSERT(offset >= 0 && offset < mFrameTail);

      ptr = (void**) (mFrameData + sym->offset());
    }

    // A static  environment
    else if (mFrameSize >=  0 && mFrameTail == -1)
    {
      INT_ASSERT(depth  >  0);
      INT_ASSERT(offset >= 0 && offset < mFrameSize);

      ptr = (void**) (mFrameData + sym->offset());
    }

    // Invalid
    else
    {
      INT_ASSERT(false);
    }

    retval =  *ptr;
  }

  else
  {
    INT_ASSERT(false);
  }

  return retval;
}

IpeValue IpeEnv::fetch(LcnSymbol* sym) const
{
  IpeValue  addr = addrOf(sym);
  IpeValue* ptr  = addr.refGet();
  IpeValue  retval;

  if (sym->type == dtBools[BOOL_SIZE_SYS])
  {
    long* src = (long*) ptr;

    retval = IpeValue((*src == 1) ? true : false);
  }

  else if (sym->type == dtInt[INT_SIZE_64])
  {
    long* src = (long*) ptr;

    retval = IpeValue(*src);
  }

  else if (sym->type == dtReal[FLOAT_SIZE_64])
  {
    double* src = (double*) ptr;

    retval = IpeValue(*src);
  }

  else if (sym->type == dtStringC)
  {
    const char** src = (const char**) ptr;

    retval = IpeValue(*src);
  }

  else if (sym->type == gIpeTypeType)
  {
    Type** src = (Type**) ptr;

    retval = IpeValue(*src);
  }

  else if (sym->type == gIpeTypeProcedure)
  {
    IpeProcedure** src = (IpeProcedure**) ptr;

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

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void IpeEnv::store(LcnSymbol* sym, IpeValue value) const
{
  IpeValue* addr = addrOf(sym).refGet();

  if (sym->type == dtBools[BOOL_SIZE_SYS])
  {
    long* ptr = (long*) addr;

    *ptr = (value.boolGet() == true) ? 1 : 0;
  }

  else if (sym->type == dtInt[INT_SIZE_64])
  {
    long* ptr = (long*) addr;

    *ptr = value.integerGet();
  }

  else if (sym->type == dtReal[FLOAT_SIZE_64])
  {
    double* ptr = (double*) addr;

    *ptr = value.realGet();
  }

  else if (sym->type == dtStringC)
  {
    const char** ptr = (const char**) addr;

    *ptr = value.cstringGet();
  }

  else if (sym->type == gIpeTypeProcedure)
  {
    IpeProcedure** ptr = (IpeProcedure**) addr;

    *ptr = value.procedureGet();
  }

  else
  {
    AstDumpToNode logger(stdout, 3);

    printf("   IpeModule::store(LcnSymbol*, IpeValue value)  unsupported\n");
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

IpeValue IpeEnv::addrOf(LcnSymbol* sym) const
{
  int      depth  = sym->depth();
  int      offset = sym->offset();
  IpeValue retval;

  INT_ASSERT(depth  >= 0);
  INT_ASSERT(offset >= 0);

  if      (depth == 0)
  {
    IpeValue* ptr = (IpeValue*) (sStoreData + sym->offset());

    INT_ASSERT(offset >= 0 && offset < sStoreTail);

    retval = IpeValue(ptr);
  }

  else if (depth <  mDepth)
  {
    retval = mParent->addrOf(sym);
  }

  else if (depth == mDepth)
  {
    // The global environment
    if      (mFrameSize == -1 && mFrameTail == -1)
    {
      INT_ASSERT(false);
    }

    else if (mFrameData == NULL)
    {
      retval = IpeValue((IpeValue*) NULL);
    }

    // A dynamic environment
    else if (mFrameSize >=  0 && mFrameTail >=  0)
    {
      IpeValue* ptr = (IpeValue*) (mFrameData + sym->offset());

      INT_ASSERT(depth  >  0);
      INT_ASSERT(offset >= 0 && offset < mFrameTail);

      retval = IpeValue(ptr);
    }

    // A static  environment
    else if (mFrameSize >=  0 && mFrameTail == -1)
    {
      IpeValue* ptr = (IpeValue*) (mFrameData + sym->offset());

      INT_ASSERT(depth  >  0);
      INT_ASSERT(offset >= 0 && offset < mFrameSize);

      retval = IpeValue(ptr);
    }

    // Invalid
    else
    {
      INT_ASSERT(false);
    }
  }

  return retval;
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

int IpeEnv::allocateValue(void* value)
{
  void** ptr    = NULL;
  int    delta  =    8;      // NOAKES: Everything is 8 bytes
  int    retval =   -1;

  // The global environment
  if      (mFrameSize == -1 && mFrameTail == -1)
  {
    INT_ASSERT(sStoreTail + delta <= sStoreSize);

    ptr        = (void**) (sStoreData + sStoreTail);
    retval     = (int)    sStoreTail;
    sStoreTail = sStoreTail + delta;
  }

  // A dynamic environment
  else if (mFrameSize >=  0 && mFrameTail >=  0)
  {
    if (mFrameTail + delta > mFrameSize)
    {
      printf("About to overflow for allocateValue\n");
      describe(3);
      printf("\n\n");
    }

    INT_ASSERT(mFrameTail + delta <= mFrameSize);

    ptr        = (void**) (mFrameData + mFrameTail);
    retval     = (int)    mFrameTail;
    mFrameTail = mFrameTail + delta;
  }

  // A static  environment
  else if (mFrameSize >=  0 && mFrameTail == -1)
  {
    INT_ASSERT(false);
  }

  // Invalid
  else
  {
    INT_ASSERT(false);
  }

  *ptr = value;

  return retval;
}

void IpeEnv::extend(LcnSymbol* sym, IpeValue value)
{
  mScope->varAdd(sym);
  allocate(sym, value);
}

void IpeEnv::allocate(LcnSymbol* sym, IpeValue value)
{
  int delta  = 8;      // NOAKES: Everything is 8 bytes
  int offset = 0;

  // The global environment
  if      (mFrameSize == -1 && mFrameTail == -1)
  {
    INT_ASSERT(sStoreTail + delta <= sStoreSize);

    offset     = sStoreTail;
    sStoreTail = sStoreTail + delta;
  }

  // A dynamic environment
  else if (mFrameSize >=  0 && mFrameTail >=  0)
  {
    INT_ASSERT(mFrameTail + delta <= mFrameSize);

    offset     = mFrameTail;
    mFrameTail = mFrameTail + delta;
  }

  // A static  environment
  else if (mFrameSize >=  0 && mFrameTail == -1)
  {
    INT_ASSERT(false);
  }

  // Invalid
  else
  {
    INT_ASSERT(false);
  }

  sym->locationSet(mDepth, offset);
  store(sym, value);
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/

void IpeEnv::initializeUsedModules() const
{
  int useCount = mScope->useCount();

  for (int i = 0; i < useCount; i++)
    mScope->useGet(i)->initialize();
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

  if (!(mDepth == 0 && mFrameTail == -1 && mFrameSize == -1 && mFrameData == 0))
  {
    printf("%s  Depth:  %3d\n", pad, mDepth);

    printf("%s  Data:   %3d / %3d    0x%012lX\n",
           pad,
           mFrameTail,
           mFrameSize,
           (long) mFrameData);
  }

  if (mScope->hasParent() == true)
    mScope->parentSummarize(pad);

  for (int i = 0; i < useCount; i++)
  {
    if (i == 0)
      printf("%s  Uses:   %s\n", pad, mScope->useGet(i)->name());
    else
      printf("%s          %s\n", pad, mScope->useGet(i)->name());
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

  if      (argSym                      != NULL)
  {
    if (argSym->intent & INTENT_REF)
      printf("ref   %-12s", typeName);
    else
      printf("arg   %-12s", typeName);
  }

  else if (varSym->immediate           != NULL)
    printf("const %-12s", typeName);

  else if (varSym->hasFlag(FLAG_CONST) == true)
    printf("const %-12s", typeName);

  else if (varSym->hasFlag(FLAG_PARAM) == true)
    printf("param %-12s", typeName);

  else
    printf("var   %-12s", typeName);

  if (var->depth() >= 0 && var->offset() >= 0)
    printf("%4d %4d ", var->depth(), var->offset());
  else
    printf("          ");

  if      (argSym != NULL && (argSym->intent & INTENT_REF) != 0)
  {
    if (mFrameData != NULL)
    {
      int       offset = argSym->offset();
      IpeValue* ref    = *((IpeValue**) (((char*) mFrameData) + offset));

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
    if (mDepth == 0 || mFrameData != NULL)
      printf("%s", (fetchBool(var) == true) ?  "true" : "false");
  }

  else if (var->type     == dtInt[INT_SIZE_64])
  {
    if (mDepth == 0 || mFrameData != NULL)
      printf("%8ld", fetchInteger(var));
  }

  else if (var->type     == dtReal[FLOAT_SIZE_64])
  {
    if (mDepth == 0 || mFrameData != NULL)
      printf("   %8.2f", fetchReal(var));
  }

  else if (var->type     == gIpeTypeModule)
  {
    if (mDepth == 0 || mFrameData != NULL)
    {
      IpeModule* value = (IpeModule*) fetchPtr(var);

      printf("#<IpeModule    %-20s 0x%012lX>", value->name(), (long) value);
    }
  }

  else if (var->type     == gIpeTypeProcedure)
  {
    if (mDepth == 0 || mFrameData != NULL)
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
  }

  else
    printf("Really??  Really??");

  printf("\n");
}

/************************************ | *************************************
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
*                                                                           *
************************************* | ************************************/



#if 0
int IpeEnv::locationSet(ArgSymbol* arg) const
{
#if 0
  return mScope->locationSet(arg);
#else
  INT_ASSERT(false);

  return 0;
#endif
}

int IpeEnv::locationSet(VarSymbol* var) const
{
#if 0
  return mScope->locationSet(var);
#else
  INT_ASSERT(false);

  return 0;
#endif
}
#endif

#if 0
int IpeEnv::reserveSpace(VarSymbol* var)
{
#if 0
  long* ptr    = (long*) (sStoreData + sStoreTail);
  int   delta  = 8;                   // NOAKES: Everything is 8 bytes
  int   retval = sStoreTail;

  INT_ASSERT(sStoreTail + delta <= sStoreSize);

  *ptr        = 0;
  sStoreTail = sStoreTail + delta;

  return retval;
#endif

  INT_ASSERT(false);

  return 0;
}
#endif

#if 0
int IpeEnv::allocateValue(bool value)
{
#if 0
  long iValue = (value == true) ? 1 : 0;

  return allocateValue(iValue);
#endif

  INT_ASSERT(false);

  return 0;
}
#endif

#if 0
int IpeEnv::allocateValue(long value)
{
#if 0
  long*  ptr    = (long*)  (sStoreData + sStoreTail);
  int    retval = (int)    sStoreTail;

  INT_ASSERT(sStoreTail + sizeof(value) <= sStoreSize);

  *ptr        = value;
  sStoreTail = sStoreTail + sizeof(value);

  return retval;
#endif

  INT_ASSERT(false);

  return 0;
}
#endif


#if 0
IpeValue IpeEnv::addrOfFoo(VarSymbol* sym) const
{
#if 0
  return IpeValue((IpeValue*) (sStoreData + sym->offset()));
#endif

  INT_ASSERT(false);

  IpeValue retval;

  return retval;
}
#endif

#if 0
IpeValue IpeEnv::fetchFoo(LcnSymbol* sym) const
{
#if 0
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
#endif

  INT_ASSERT(false);

  IpeValue retval;

  return retval;
}
#endif

#if 0
void IpeEnv::storeFoo(LcnSymbol* sym, IpeValue value)
{
#if 0
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
#endif

  INT_ASSERT(false);
}
#endif

#if 0
void IpeEnv::storeBool(LcnSymbol* sym, IpeValue value)
{
#if 0
  INT_ASSERT(sym->offset() >= 0);

  *((long*) (sStoreData + sym->offset())) = (value.boolGet() == true) ? 1L : 0L;
#endif

  INT_ASSERT(false);
}
#endif

#if 0
void IpeEnv::storeInteger(LcnSymbol* sym, IpeValue value)
{
#if 0
  INT_ASSERT(sym->offset() >= 0);

  *((long*) (sStoreData + sym->offset())) = value.integerGet();
#endif

#if 0
  long* ptr = NULL;

  // The global environment
  if (mFrameSize == -1 && mFrameTail == -1)
  {
    INT_ASSERT(sym->offset() >= 0);

    ptr = (long*) (sStoreData + sym->offset());
  }

  // A dynamic envrionment

  // A static  environment

  INT_ASSERT(ptr != NULL);

  *ptr = value.integerGet();
#endif

  INT_ASSERT(false);
}
#endif

#if 0
void IpeEnv::storeReal(LcnSymbol* sym, IpeValue value)
{
#if 0
  INT_ASSERT(sym->offset() >= 0);

  *((double*) (sStoreData + sym->offset())) = value.realGet();
#endif

  INT_ASSERT(false);
}
#endif


#if 0
void IpeEnv::allocateSpace(VarSymbol* var) const
{
#if 0
  int delta  = 8;      // NOAKES: Everything is 8 bytes

  INT_ASSERT(sStoreTail + delta <= sStoreSize);

  var->locationSet(0, sStoreTail);
  varAdd(var);

  sStoreTail = sStoreTail + delta;
#else
  INT_ASSERT(false);
#endif
}
#endif

#if 0
void IpeEnv::assign(VarSymbol* sym, IpeValue value)
{
#if 0
  assign((LcnSymbol*) sym, value);
#else
  INT_ASSERT(false);
#endif
}
#endif

#if 0
IpeValue IpeEnv::valueFetch(LcnSymbol* sym) const
{
#if 0
  char*    base = (sym->depth() == 0) ? sStoreData : (char*) mFrameData;
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
#else
  INT_ASSERT(false);

  IpeValue retval;

  return retval;
#endif
}
#endif


#if 0
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
#endif











#if 0
  IpeValue retval;

  if      (sym->depth() == 0)
  {
    retval = IpeEnv::addrOfFoo(sym);
  }

  else if (sym->depth() == 1 && mFrameData != 0)
  {
    if (sym->type == dtBools[BOOL_SIZE_SYS])
    {
      long* src = (long*) (((char*) mFrameData) + sym->offset());

      retval = IpeValue((IpeValue*) src);
    }

    else if (sym->type == dtInt[INT_SIZE_64])
    {
      long* src = (long*) (((char*) mFrameData) + sym->offset());

      retval = IpeValue((IpeValue*) src);
    }

    else if (sym->type == dtReal[FLOAT_SIZE_64])
    {
      double* src = (double*) (((char*) mFrameData) + sym->offset());

      retval = IpeValue((IpeValue*) src);
    }

    else if (sym->type == dtStringC)
    {
      const char** src = (const char**) (((char*) mFrameData) + sym->offset());

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
#endif

#if 0
void IpeEnv::assign(ArgSymbol* sym, IpeValue value)
{
#if 0
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

    else if (sym->depth() == 1 && mFrameData != 0)
    {
      store((LcnSymbol*) sym, value);
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
    store((LcnSymbol*) sym, value);
  }
#else
  INT_ASSERT(false);
#endif
}
#endif

#if 0
void IpeEnv::store(LcnSymbol* sym, IpeValue value)
{
  if      (sym->depth() == 0)
  {
    IpeEnv::storeFoo(sym, value);
  }

  else if (sym->depth() == 1 && mFrameData != 0)
  {
    if (sym->type == dtBools[BOOL_SIZE_SYS])
    {
      long* dst = (long*) (((char*) mFrameData) + sym->offset());

      *dst = (value.boolGet() == true) ? 1L : 0L;
    }

    else if (sym->type == dtInt[INT_SIZE_64])
    {
      long* dst = (long*) (((char*) mFrameData) + sym->offset());

      *dst = value.integerGet();
    }

    else if (sym->type == dtReal[FLOAT_SIZE_64])
    {
      double* dst = (double*) (((char*) mFrameData) + sym->offset());

      *dst = value.realGet();
    }

    else if (sym->type == dtStringC)
    {
      const char** dst = (const char**) (((char*) mFrameData) + sym->offset());

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
#endif



#if 0
IpeValue IpeEnv::fetch(LcnSymbol* sym) const
{
  int      depth  = sym->depth();
  IpeValue retval;

  if      (depth <  mDepth)
  {
    retval = mParent->fetch(sym);
  }

  else if (depth == mDepth)
  {
    IpeValue  addr = addrOf(sym);
    IpeValue* ptr  = addr.refGet();

    if (sym->type == dtBools[BOOL_SIZE_SYS])
    {
      long* src = (long*) ptr;

      retval = IpeValue((*src == 1) ? true : false);
    }

    else if (sym->type == dtInt[INT_SIZE_64])
    {
      long* src = (long*) ptr;

      retval = IpeValue(*src);
    }

    else if (sym->type == dtReal[FLOAT_SIZE_64])
    {
      double* src = (double*) ptr;

      retval = IpeValue(*src);
    }

    else if (sym->type == dtStringC)
    {
      const char** src = (const char**) ptr;

      retval = IpeValue(*src);
    }

    else if (sym->type == gIpeTypeType)
    {
      Type** src = (Type**) ptr;

      retval = IpeValue(*src);
    }

    else if (sym->type == gIpeTypeProcedure)
    {
      IpeProcedure** src = (IpeProcedure**) ptr;

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
    INT_ASSERT(false);
  }

  return retval;
}
#endif


#if 0
IpeValue IpeEnv::fetch(LcnSymbol* sym) const
{
  int      depth  = sym->depth();
  IpeValue retval;

  if      (depth <  mDepth)
  {
    retval = mParent->fetch(sym);
  }

  else if (depth == mDepth)
  {
    IpeValue  addr = addrOf(sym);
    IpeValue* ptr  = addr.refGet();

    if (sym->type == dtBools[BOOL_SIZE_SYS])
    {
      long* src = (long*) ptr;

      retval = IpeValue((*src == 1) ? true : false);
    }

    else if (sym->type == dtInt[INT_SIZE_64])
    {
      long* src = (long*) ptr;

      retval = IpeValue(*src);
    }

    else if (sym->type == dtReal[FLOAT_SIZE_64])
    {
      double* src = (double*) ptr;

      retval = IpeValue(*src);
    }

    else if (sym->type == dtStringC)
    {
      const char** src = (const char**) ptr;

      retval = IpeValue(*src);
    }

    else if (sym->type == gIpeTypeType)
    {
      Type** src = (Type**) ptr;

      retval = IpeValue(*src);
    }

    else if (sym->type == gIpeTypeProcedure)
    {
      IpeProcedure** src = (IpeProcedure**) ptr;

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
    INT_ASSERT(false);
  }

  return retval;
}
#endif
