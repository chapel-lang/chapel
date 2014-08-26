/*
 * Copyright 2004-2014 Cray Inc.
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

#include "flags.h"

#include "baseAST.h"
#include "stringutil.h"
#include "symbol.h"


typedef Map<const char*, int> FlagMap;
typedef MapElem<const char*, int> FlagMapElem;
// see also flags_list.h for description
static FlagMap flagMap;
static const char* flagNames[NUM_FLAGS];
static const char* flagComments[NUM_FLAGS];
static bool flagPragma[NUM_FLAGS];


Flag
pragma2flag(const char* str) {
  Flag lookup = (Flag)flagMap.get(astr(str));
  if (lookup == FLAG_UNKNOWN || !flagPragma[lookup])
    return FLAG_UNKNOWN;
  else
    return lookup;
}


void
initFlags() {
# define symbolFlag(NAME,PRAGMA,MAPNAME,COMMENT) \
    flagMap.put(astr(MAPNAME), NAME); \
    flagNames[NAME] = #NAME; \
    flagComments[NAME] = COMMENT; \
    flagPragma[NAME] = PRAGMA;
# include "flags_list.h"
# undef symbolFlag

  // FLAG_UNKNOWN - limited representation; NUM_FLAGS - no representation
  flagNames[FLAG_UNKNOWN] = "FLAG_UNKNOWN";
  flagComments[FLAG_UNKNOWN] = "";
  flagPragma[FLAG_UNKNOWN] = false;
  flagNames[FLAG_UNKNOWN] = "unknown flag (0)";

}

void writeFlags(FILE* fp, Symbol* sym) {
  form_Map(FlagMapElem, e, flagMap) {
    if (sym->flags[e->value]) {
      fprintf(fp, " \"%s\"", e->key);
    }
  }
}


// these affect what viewFlags() prints
bool viewFlagsShort = true;
bool viewFlagsPragma = false;
bool viewFlagsName  = false;
bool viewFlagsComment = false;

void
viewFlags(BaseAST* ast) {
  if (!viewFlagsShort && !viewFlagsName && !viewFlagsComment)
    viewFlagsName = true;
  if (Symbol* sym = toSymbol(ast)) {
    form_Map(FlagMapElem, e, flagMap) {
      if (sym->flags[e->value]) {
        if (viewFlagsName)
          printf("%s ", flagNames[e->value]);
        if (viewFlagsPragma)
          printf("%s", flagPragma[e->value] ? "ypr " : "npr ");
        if (viewFlagsShort)
          printf("\"%s\" ", e->key);
        if (viewFlagsComment)
          printf("// %s",
                 *flagComments[e->value] ? flagComments[e->value] : "ncm");
        printf("\n");
      }
    }
  } else {
    printf("[%d]: not a Symbol, has no flags\n", ast->id);
  }
}

// for gdb (which does not let me call setFlag, addFlag, removeFlag)

static Symbol* symflagOK(const char* msg, BaseAST* ast, int flag) {
  Symbol* sym = toSymbol(ast);
  if (!sym) {
    printf("%s: [%d] is a %s (does not support Flags)\n", msg, ast->id,
           ast->astTagAsString());
    return NULL;
  } else if (flag <= 0) {
    printf("%s: flag is non-positive: %d\n", msg, flag);
    return NULL;
  } else if (flag >= NUM_FLAGS) {
    printf("%s: flag is too big (NUM_FLAGS is %d): %d\n", msg, NUM_FLAGS, flag);
    return NULL;
  } else {
    printf("%s(%s=%d)\n", msg, flagNames[flag], flag);
    return sym;
  }
}

bool hasFlag(BaseAST* ast, int flag) {
  Symbol* sym = symflagOK("hasFlag", ast, flag);
  return sym ? sym->hasFlag((Flag)flag): false;
}

void addFlag(BaseAST* ast, int flag) {
  Symbol* sym = symflagOK("addFlag", ast, flag);
  if (sym) sym->addFlag((Flag)flag);
}

void removeFlag(BaseAST* ast, int flag) {
  Symbol* sym = symflagOK("removeFlag", ast, flag);
  if (sym) sym->removeFlag((Flag)flag);
}

bool hasFlag(BaseAST* ast, Flag flag)   { return hasFlag(ast, (int)flag); }
void addFlag(BaseAST* ast, Flag flag)    { addFlag(ast, (int)flag); }
void removeFlag(BaseAST* ast, Flag flag) { removeFlag(ast, (int)flag); }

// end gdb support


TypeSymbol*
getDataClassType(TypeSymbol* ts) {
  form_Map(SymbolMapElem, e, ts->type->substitutions) {
    if (TypeSymbol* ets = toTypeSymbol(e->value))
      return ets;
  }
  return NULL;
}


void
setDataClassType(TypeSymbol* ts, TypeSymbol* ets) {
  form_Map(SymbolMapElem, e, ts->type->substitutions) {
    if (isTypeSymbol(e->value))
      e->value = ets;
  }
}
