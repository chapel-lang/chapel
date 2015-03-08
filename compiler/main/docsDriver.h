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

#include "arg.h"
#include "misc.h"

// FIXME: This global should no longer be needed. Instead, use argv[0] ==
//        "chpldoc". Alt. set it based on that comparison.
//        (thomasvandoren, 2015-03-08)
extern bool fDocs;

extern bool fDocsAlphabetize;
extern char fDocsCommentLabel[256];
extern char fDocsFolder[256];
extern bool fDocsTextOnly;
extern char fDocsSphinxDir[256];

// FIXME: This is always true, so propagate that to the one spot it is used
//        (thomasvandoren, 2015-03-08)
extern bool fDocsIncludeExterns;


void docsArgSetCommentLabel(const ArgumentState* arg_state, const char* label);


extern ArgumentDescription docs_arg_desc[];
