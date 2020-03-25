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

#ifndef _COUNTTOKENS_H_
#define _COUNTTOKENS_H_

/* BLC: This file contains routines that help keep track of statistics
   on numbers of tokens and to print out the code in a tokenized
   form */

#include "bison-chapel.h"

extern bool countTokens;
extern bool printTokens;

void startCountingFileTokens(const char* filename);
void stopCountingFileTokens(yyscan_t scanner);
void finishCountingTokens();

void countToken(const char* toktext1,
                const char* toktext2 = NULL,
                const char* toktext3 = NULL);
void countNewline();
void countCommentLine();
void countSingleLineComment(const char* comment);
void countMultiLineComment(const char* comment);

#endif
