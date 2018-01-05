/*
 * Copyright 2004-2018 Cray Inc.
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

#ifndef _IPE_DEF_EXPR_H_
#define _IPE_DEF_EXPR_H_

#include "expr.h"

class LcnSymbol;
class FnSymbol;
class ModuleSymbol;

class IpeDefExpr : public DefExpr
{
public:
                           IpeDefExpr(LcnSymbol*    sym,
                                      Expr*         initExpr,
                                      Expr*         typeExpr);

                           IpeDefExpr(LcnSymbol*    sym,
                                      FnSymbol*     fnSymbol);

                           IpeDefExpr(LcnSymbol*    sym,
                                      ModuleSymbol* moduleSymbol);

  virtual                 ~IpeDefExpr();

  FnSymbol*                fnSymbolGet()                            const;
  ModuleSymbol*            moduleSymbolGet()                        const;

  void                     describe(int offset);

private:
                           IpeDefExpr();

  FnSymbol*                mFnSymbol;
  ModuleSymbol*            mModuleSymbol;
};

#endif
