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

#ifndef CHPL_FLAGS_H_
#define CHPL_FLAGS_H_


//
// HOW TO ADD A FLAG:
// - add an entry (sorted alphabetically) to the list in flags_list.h
// - to add the flag to a symbol, use Symbol::addFlag(Flag)
// - to check whether a symbol has a flag, use Symbol::hasFlag(Flag)
//
// Flags can be added to Chapel code using the pragma production as in
//
//   pragma "my pragma" def FunctionName
//   pragma "my pragma" var VariableName
//   pragma "my pragma" class ClassName
//
// HOW TO VIEW A FLAG IN GDB: Use the command "flags ast" where ast is
// a Symbol.  If you just know the id of the ast, use the command
// "flags aid(id)" where id is the integral symbol id.
//


#include "chpl.h"


class BaseAST;
class Symbol;
class TypeSymbol;


enum Flag {
  FLAG_UNKNOWN = 0,
# define symbolFlag(NAME,PRAGMA,MAPNAME,COMMENT) NAME,
# include "flags_list.h"
# undef symbolFlag
  NUM_FLAGS,
  FLAG_FIRST = FLAG_UNKNOWN + 1, // index of the first flag
  FLAG_LAST  = NUM_FLAGS - 1     // index of the last flag
};

// only meaningful flags are allowed
#define CHECK_FLAG(FLAG) \
  INT_ASSERT(FLAG_FIRST <= (FLAG) && (FLAG) <= FLAG_LAST)


Flag pragma2flag(const char* str);
void initFlags();
void viewFlags(BaseAST* sym);
void writeFlags(FILE* fp, Symbol* sym);
TypeSymbol* getDataClassType(TypeSymbol* ts);
void setDataClassType(TypeSymbol* ts, TypeSymbol* ets);

// For the purposes of gdb support
bool hasFlag(BaseAST* ast, int flag);
void addFlag(BaseAST* ast, int flag);
void removeFlag(BaseAST* ast, int flag);

bool hasFlag(BaseAST* ast, Flag flag);
void addFlag(BaseAST* ast, Flag flag);
void removeFlag(BaseAST* ast, Flag flag);
// End gdb support


#endif
