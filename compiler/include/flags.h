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


enum Flag {
  FLAG_UNKNOWN = 0,
# define symbolFlag(NAME,PRAGMA,MAPNAME,COMMENT) NAME,
# include "flags_list.h"
# undef symbolFlag
  NUM_FLAGS
};

// only meaningful flags are allowed
#define CHECK_FLAG(FLAG) \
  INT_ASSERT(FLAG_UNKNOWN < (FLAG) && (FLAG) < NUM_FLAGS)


Flag pragma2flag(const char* str);
void initFlags();
void viewFlags(BaseAST* sym);
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
