/************************************ | *************************************
*                                                                           *
* Function resolution requires a scan through a chain of DefScopes to find  *
* all visible functions with a given name followed by a complex algorithm   *
* that attempts to select a preferred function based on matching the list   *
* of formals to the list of actuals.                                        *
*                                                                           *
* This data structure is used to record an instance of a match. The         *
* resolution algorithm is depdendent on the distance, in terms of lexical   *
* scope, between the caller and the definition and so we need both the      *
* value and the scope in which the value was found.                         *
*                                                                           *
************************************* | ************************************/

#ifndef _VISIBLE_SYMBOL_H_
#define _VISIBLE_SYMBOL_H_

class DefScope;
class Symbol;

class VisibleSymbol
{
public:
                               VisibleSymbol(Symbol*         sym,
                                             const DefScope* scope);
                              ~VisibleSymbol();

  Symbol*                      symbol()                           const;

  const DefScope*              scope()                            const;
  int                          depth()                            const;

private:
                               VisibleSymbol();

  Symbol*                      mSymbol;
  const DefScope*              mScope;
};

#endif
