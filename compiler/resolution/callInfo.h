/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
 * Copyright 2004-2019 Cray Inc.
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

#ifndef _CALL_INFO_H_
#define _CALL_INFO_H_

#include "vec.h"

class BlockStmt;
class CallExpr;
class Symbol;

class CallInfo {
public:
                   CallInfo();

  bool             isWellFormed(CallExpr* call);

  void             haltNotWellFormed()                                   const;

  const char*      toString();

  CallExpr*        call;        // call expression
  BlockStmt*       scope;       // module scope as in M.call

  const char*      name;        // function name

  Vec<Symbol*>     actuals;     // actual symbols
  Vec<const char*> actualNames; // named arguments
};

#endif
