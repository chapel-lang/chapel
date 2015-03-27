//# AliasVectorMap.h -*- C++ -*-
//########################################################################
//#
//# A utility class derived from std::map<Symbol*, SymbolVector*>.
//# An element in this map can contain dynamically-allocated memory through its
//# SymbolVector (second) field.  So the main point of the class is to provide
//# a destructor that will clean up an instance properly.
//# The insert and merge utilities manipulate the data structure while
//# maintaining its main invariants:
//#  1. Each member maps a symbol to a vector of symbols.
//#  2. The vector of symbols contains at least the same symbol and also other
//#     symbols with which it is an alias.
//#  3. The set of mutual aliases forms a clique.  Since this set is common to
//#     all its members, the symbol vector representing a clique is shared among all
//#     members belonging to that clique.
//#
//########################################################################

#ifdef ALIASVECTORMAP_H
#error Multiple inclusion.
#else
#define ALIASVECTORMAP_H

#include "symbol.h"
#include "stlUtil.h"

class AliasVectorMap : public std::map<Symbol*, SymbolVector*>
{
 public:
  typedef value_type AliasMapElement;

 private:
  // Copying an alias vector map involves copying the symbol vector -- which we
  // don't want to do, so we just disallow copying instead.
  AliasVectorMap(const AliasVectorMap& rhs);
  void operator=(const AliasVectorMap& rhs);

 public:
  ~AliasVectorMap();
  AliasVectorMap() : std::map<Symbol*, SymbolVector*>() {}

  void insert(Symbol* sym);
  SymbolVector* at(Symbol* sym) const;
  void merge(Symbol* orig, Symbol* alias);
};

// To destroy all the lists in the map, we find the list through the first
// member of the clique, then set the map element for each member in the list
// to NULL.  Then we delete the list.
inline
AliasVectorMap::~AliasVectorMap()
{
  for (iterator it = begin();
       it != end(); ++it)
  {
    if (it->second == NULL)
      // This entry is part of a clique that was already deleted.
      continue;

    SymbolVector* aliasList = it->second;
    for_vector(Symbol, sym, *aliasList)
      operator[](sym) = NULL;
      
    delete aliasList;
  }
}

// Insert a symbol into the alias vector map.
// When a symbol is first inserted it is associated with an alias vector that
// contains only itself.
inline void
AliasVectorMap::insert(Symbol* sym)
{
  SymbolVector* aliasList = new SymbolVector();
  aliasList->push_back(sym);
  operator[](sym) = aliasList;
}

// Retrieves the SymbolVector* store in the selected element if it exists;
// returns null otherwise.
inline SymbolVector*
AliasVectorMap::at(Symbol* sym) const
{
  const_iterator e = find(sym);
  if (e == end())
    return NULL;
  return e->second;
}

// Merge the alias lists of two symbols that have become aliases.
inline void
AliasVectorMap::merge(Symbol* orig, Symbol* alias)
{
  SymbolVector* origList = operator[](orig);
  SymbolVector* aliasList = operator[](alias);

  if (aliasList == origList)
  {
    // These two symbols are already aliases.
    return;
  }

  // Copy elements from the alias's list into the original symbol's list and
  // set each entry in the map corresponding to those elements to point to the
  // updated original list.
  for (size_t i = 0; i < aliasList->size(); ++i)
  {
    Symbol* sym = aliasList->at(i);
    origList->push_back(sym);
    operator[](sym) = origList;
  }

  // After that, no elements point to this aliasList, so it is deleted.
  delete aliasList;
}
//#
//######################### End of AliasVectorMap #########################


#endif
