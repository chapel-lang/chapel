#ifndef _CREATE_CONFIG_VAR_TABLE_H_
#define _CREATE_CONFIG_VAR_TABLE_H_

#include "files.h"
#include "traversal.h"


class CreateConfigVarTable : public Traversal {
 public:
  fileinfo outfileinfo;
  fileinfo extheadfileinfo;
  fileinfo intheadfileinfo;

  CreateConfigVarTable(void);

  void preProcessStmt(Stmt* stmt);

  void closeCFile(void);
};

#endif
