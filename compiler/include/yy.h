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

#ifndef _YY_H_
#define _YY_H_

#include "stmt.h"
#include "symbol.h"

extern BlockStmt* yyblock;
extern int yystartlineno;

extern FILE *yyin;

extern char* yytext;
extern int chplLineno;
extern bool chplParseString;
extern const char *chplParseStringMsg;
extern const char* yyfilename;
  
int yyparse(void);
void yyerror(const char* str);
int getNextYYChar();
BlockStmt* parseString(const char* string, const char* filename, 
                       const char* msg);

int yylex(void);
extern int yydebug;

enum ProcIter {
  ProcIter_PROC,
  ProcIter_ITER
};

#endif
