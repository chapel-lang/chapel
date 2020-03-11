/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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

#ifndef _docs_driver_H_
#define _docs_driver_H_

#include "arg.h"
#include "misc.h"

extern bool fDocs;
extern char fDocsAuthor[256];
extern bool fDocsAlphabetize;
extern char fDocsCommentLabel[256];
extern char fDocsFolder[256];
extern bool fDocsTextOnly;
extern char fDocsSphinxDir[256];
extern bool fDocsHTML;
extern bool fDocsProcessUsedModules;
extern char fDocsProjectVersion[256];

// TODO: Whether or not to support this flag is an open discussion. Currently,
//       it is not supported, so the flag is always true.
//       (thomasvandoren, 2015-03-08)
extern bool fDocsIncludeExterns;


void docsArgSetCommentLabel(const ArgumentDescription* desc, const char* label);


extern ArgumentDescription docs_arg_desc[];

#endif
