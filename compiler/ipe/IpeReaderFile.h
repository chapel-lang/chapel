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

#ifndef _IPE_READER_FILE_H_
#define _IPE_READER_FILE_H_

#include "IpeReader.h"

#include "symbol.h"

class Expr;
class DefExpr;

#include <vector>

class IpeReaderFile : public IpeReader
{
  //
  // Class interface
  //
public:
  static std::vector<DefExpr*>    readModules(const char* path, ModTag moduleType);

private:
  static bool                     onlyModuleDefs(std::vector<Expr*> exprList);


  //
  // Instance interface
  //
public:
                                  IpeReaderFile();
  virtual                        ~IpeReaderFile();

  bool                            open(const char* fileName);
  void                            close();

private:
  char*                           mPath;
  FILE*                           mFP;
};

#endif
