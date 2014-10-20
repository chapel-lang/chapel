/*
 * Copyright 2004-2014 Cray Inc.
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

#ifndef _LEXYACC_H_
#define _LEXYACC_H_

#include "build.h"
#include "countTokens.h"
#include "driver.h"
#include "expr.h"
#include "ForLoop.h"
#include "parser.h"
#include "processTokens.h"
#include "stmt.h"
#include "stringutil.h"
#include "symbol.h"
#include "type.h"
#include "WhileDoStmt.h"
#include "DoWhileStmt.h"
#include "yy.h"

// Make sure exit is undefined:
#ifdef exit
#undef exit
#endif
// And redefine it to call our exit routine:
#define exit(x) clean_exit(x)

extern int captureTokens;
extern char captureString[1024];

#endif
