#include <map>
#include <string>
#include "symtab.h"

using namespace std;

static map<string, Symbol*> table;

void Symboltable::define(Symbol* sym) {
  table[sym->name] = sym;
}

Symbol* Symboltable::lookup(char* name) {
  if (table.find(name) == table.end()) {
    return NULL;
  } else {
    return table[name];
  }
}
