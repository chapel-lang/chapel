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

#ifndef _IPE_SYMBOL_H_
#define _IPE_SYMBOL_H_

#include "symbol.h"

/******************************** | *********************************
*                                                                   *
* An IpeSymbol is used to reference an IPE runtime variable         *
*                                                                   *
********************************* | ********************************/

class AstVisitor;
class BaseAST;
class Type;

class IpeSymbol : public Symbol
{
public:
                     IpeSymbol(const char* name,
                               Type*       type,
                               int         depth,
                               int         offset);
                    ~IpeSymbol();

  virtual void       verify();
  virtual void       accept(AstVisitor* visitor);

  DECLARE_SYMBOL_COPY(IpeSymbol);

  virtual void       replaceChild(BaseAST* oldAst, BaseAST* newAst);

  virtual bool       isConstant()                             const;
  virtual bool       isConstValWillNotChange()                const;
  virtual bool       isParameter()                            const;

  int                depth()                                  const;
  int                offset()                                 const;

private:
  int                mDepth;
  int                mOffset;
};

#endif
