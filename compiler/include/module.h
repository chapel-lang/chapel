#ifndef _MODULE_H_
#define _MODULE_H_

#include <stdio.h>
#include "stmt.h"

class Module : public ILink {
 public:
  FILE* codefile;
  FILE* extheadfile;
  FILE* intheadfile;

  Stmt* stmts;

  Module(char*);

  void codegen(void);

  static Module* createModules(int numFilenames, char* filename[]);
};

extern Module* currentModule;

#endif
