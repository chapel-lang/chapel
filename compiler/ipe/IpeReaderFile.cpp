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

#include "IpeReaderFile.h"

#include "astutil.h"
#include "files.h"
#include "stmt.h"
#include "stringutil.h"

IpeReaderFile::IpeReaderFile()
{
  mPath =  NULL;
  mFP   =  NULL;
}


IpeReaderFile::~IpeReaderFile()
{
  if (mFP != NULL)
    close();
}

bool IpeReaderFile::open(const char* fileName)
{
  if ((mFP = fopen(fileName, "r")) != 0)
  {
    mPath      = strdup(fileName);
    yyfilename = fileName;

    yyset_in(mFP, mContext.scanner);
  }

  return (mFP != NULL) ? true : false;
}

void IpeReaderFile::close()
{
  if (mFP != NULL)
  {
    fclose(mFP);

    free(mPath);

    mPath      = NULL;
    mFP        = NULL;
    yyfilename = NULL;
  }
}

// Collect all of the module definitions in a file
//
// A Chapel module file consists of zero or more chapel statements
//    IF      there are no statements
//      signal an error
//
//    ELSE IF every statement is a module-declaration-statement
//      return a vector of the modules
//
//    ELSE
//       create a module whose body is all of the statements

std::vector<DefExpr*> IpeReaderFile::readModules(const char* path, ModTag modType)
{
  IpeReaderFile         factory;
  std::vector<DefExpr*> retval;

  if (factory.open(path) == true)
  {
    std::vector<Expr*> exprList;

    // Collect all of the top level statements
    while (Expr* expr = factory.readStmt())
      exprList.push_back(expr);

    factory.close();

    if (exprList.size() == 0)
    {
      printf("IpeReaderFile::readModules: File %s is empty\n", path);
    }

    else if (onlyModuleDefs(exprList) == true)
    {
      for (size_t i = 0; i < exprList.size(); i++)
        retval.push_back(toDefExpr(exprList[i]));
    }

    else if (path == NULL)
    {
      printf("IpeReaderFile::readModules Need to synthesize a module but source is not a file\n");
      INT_ASSERT(false);
    }

    else
    {
      const char*   modName = filenameToModulename(path);
      ModuleSymbol* modSym  = new ModuleSymbol(modName, modType, yyblock);

      modSym->filename = astr(path);

      retval.push_back(new DefExpr(modSym, NULL, NULL));
    }
  }

  return retval;
}

// Return true if every expression is a module defintion
bool IpeReaderFile::onlyModuleDefs(std::vector<Expr*> exprList)
{
  bool retval = true;

  for (size_t i = 0; i < exprList.size() && retval == true; i++)
  {
    DefExpr* expr = toDefExpr(exprList[i]);

    retval = (expr != NULL && isModuleSymbol(expr->sym) == true) ? true : false;
  }

  return retval;
}
