/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#include "flags.h"

#include "expr.h"
#include "stringutil.h"


typedef Map<const char*, int> FlagMap;
typedef MapElem<const char*, int> FlagMapElem;
// see also flags_list.h for description
static FlagMap flagMap;
static const char* flagNames[NUM_FLAGS];
static const char* flagShortNames[NUM_FLAGS];
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
    flagShortNames[NAME] = astr(MAPNAME); \
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
  for (int flagNum = FLAG_FIRST; flagNum <= FLAG_LAST; flagNum++) {
    if (sym->flags[flagNum]) {
      fprintf(fp, " \"%s\"", flagShortNames[flagNum]);
    }
  }
}

// these affect what viewFlags() prints
bool viewFlagsShort   = true;
bool viewFlagsPragma  = false;
bool viewFlagsName    = false;
bool viewFlagsComment = false;
bool viewFlagsExtras  = true;

static void viewSymbolFlags(Symbol* sym) {
    for (int flagNum = FLAG_FIRST; flagNum <= FLAG_LAST; flagNum++) {
      if (sym->flags[flagNum]) {
        if (viewFlagsName) {
          printf("%s ", flagNames[flagNum]);
        }

        if (viewFlagsPragma) {
          printf("%s", flagPragma[flagNum] ? "ypr " : "npr ");
        }

        if (viewFlagsShort) {
          printf("\"%s\" ", flagShortNames[flagNum]);
        }

        if (viewFlagsComment) {
          printf("// %s",
                 *flagComments[flagNum] ? flagComments[flagNum] : "ncm");
        }

        printf("\n");
      }
    }

    if (viewFlagsExtras) {
      if (VarSymbol* vs = toVarSymbol(sym)) {
        if (vs->immediate) {
          printf("immediate ");
          fprint_imm(stdout, *toVarSymbol(sym)->immediate, true);
          printf("\n");
        }
        if (ShadowVarSymbol* svar = toShadowVarSymbol(vs)) {
          printf("%s shadow var  ", svar->intentDescrString());
        }
        printf("qual %s\n", qualifierToStr(vs->qual));

      } else if (ArgSymbol* as = toArgSymbol(sym)) {
        printf("%s arg  qual %s\n",
               as->intentDescrString(), qualifierToStr(as->qual));

      } else if (toTypeSymbol(sym)) {
        printf("a TypeSymbol\n");

      } else if (FnSymbol* fs = toFnSymbol(sym)) {
        printf("isGeneric %s\n", fs->isGenericIsValid() ?
               (fs->isGeneric() ? "yes" : "no") : "unset");
        bool isMethod = fs->_this != NULL;
        bool isTypeMethod = isMethod && fs->_this->hasFlag(FLAG_TYPE_VARIABLE);
        printf("fn %s%s%s(%d args) %s\n",
               isMethod ? intentDescrString(fs->thisTag) : "",
               isTypeMethod ? ", type method" : "",
               isMethod ? " " : "",
               fs->numFormals(),
               retTagDescrString(fs->retTag));

      } else if (toEnumSymbol(sym)) {
        printf("an EnumSymbol\n");

      } else if (ModuleSymbol* ms = toModuleSymbol(sym)) {
        printf("module %s\n", ModuleSymbol::modTagToString(ms->modTag));

      } else if (toLabelSymbol(sym)) {
        printf("a LabelSymbol\n");

      } else {
        printf("unknown symbol kind\n");
      }
    }
}

static void viewFlagsHelper(BaseAST* ast, Symbol* sym, const char* msg) {
  printf("%d: %s %d\n", ast->id, msg, sym->id);
  viewSymbolFlags(sym);
}
  
void viewFlags(BaseAST* ast) {
  if (!viewFlagsShort && !viewFlagsComment)
    viewFlagsName = true;
  if (Symbol* sym = toSymbol(ast)) {
    viewSymbolFlags(sym);
  } else if (Type* type = toType(ast)) {
    viewFlagsHelper(ast, type->symbol, "is a type, showing its Symbol");
  } else if (SymExpr* se = toSymExpr(ast)) {
    viewFlagsHelper(ast, se->symbol(), "is a SymExpr, showing its Symbol");
  } else if (DefExpr* def = toDefExpr(ast)) {
    viewFlagsHelper(ast, def->sym, "is a DefExpr, showing its Symbol");
  } else {
    // more cases
    bool handled = false;
    if (CallExpr* call = toCallExpr(ast)) {
      if (SymExpr* se = toSymExpr(call->baseExpr)) {
        viewFlagsHelper(ast, se->symbol(),
                        "is a CallExpr, showing its baseExpr Symbol");
        handled = true;
      }
    }
    if (!handled)
      printf("%d: is a %s (does not support Flags)\n",
             ast->id, ast->astTagAsString());
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
