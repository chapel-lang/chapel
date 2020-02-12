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

#include "docsDriver.h"

#include "commonFlags.h"
#include "driver.h"
#include "mysystem.h"

bool fDocs = false;
char fDocsAuthor[256] = "";
bool fDocsAlphabetize = false;
char fDocsCommentLabel[256] = "";
char fDocsFolder[256] = "";
bool fDocsTextOnly = false;
char fDocsSphinxDir[256] = "";
bool fDocsHTML = true;
char fDocsProjectVersion[256] = "0.0.1";

// TODO: Whether or not to support this flag is an open discussion. Currently,
//       it is not supported, so the flag is always true.
//       (thomasvandoren, 2015-03-08)
bool fDocsIncludeExterns = true;
bool fDocsProcessUsedModules = false;


void docsArgSetCommentLabel(const ArgumentDescription* desc, const char* label) {
  assert(label != NULL);
  size_t len = strlen(label);
  if (len != 0) {
    if (len > sizeof(fDocsCommentLabel)) {
      USR_FATAL("the label is too large!");
    }else if (label[0] != '/' || label[1] != '*') {
      USR_FATAL("comment label should start with /*");
    } else {
      strcpy(fDocsCommentLabel, label);
    }
  }
}

/*
Flag types:

  I = int
  P = path
  S = string
  D = double
  f = set to false
  F = set to true
  + = increment
  T = toggle
  L = int64 (long)
  N = --no-... flag, --no version sets to false
  n = --no-... flag, --no version sets to true

Record components:
 {"long option" (or "" for separators), 'short option', "description of option argument(s), if any", "option description", "option type", &affectedVariable, "environment variable name", setter_function},
*/

ArgumentDescription docs_arg_desc[] = {
 {"", ' ', NULL, "Documentation Options", NULL, NULL, NULL, NULL},

 // TODO: This option is disabled for now (since source based ordering was
 //       introduced). The code to support it is still around, and the plan is
 //       to bring it back someday soon. (thomasvandoren, 2015-03-11)
 //
 // {"alphabetical", ' ', NULL, "Alphabetizes the documentation", "N", &fDocsAlphabetize, NULL, NULL},

 {"output-dir", 'o', "<dirname>", "Sets the documentation directory to <dirname>", "S256", fDocsFolder, NULL, NULL},
 {"author", ' ', "<author>", "Documentation author string.", "S256", fDocsAuthor, "CHPLDOC_AUTHOR", NULL},
 {"comment-style", ' ', "<indicator>", "Only includes comments that start with <indicator>", "S256", fDocsCommentLabel, NULL, docsArgSetCommentLabel},
 {"process-used-modules", ' ', NULL, "Also parse and document 'use'd modules", "F", &fDocsProcessUsedModules, NULL, NULL},
 {"save-sphinx",  ' ', "<directory>", "Save generated Sphinx project in directory", "S256", fDocsSphinxDir, NULL, NULL},
 {"text-only", ' ', NULL, "Generate text documentation only", "F", &fDocsTextOnly, NULL, NULL},
 {"html", ' ', NULL, "[Don't] generate html documentation (on by default)", "N", &fDocsHTML, NULL, NULL},
 {"project-version", ' ', "<projectversion>", "Sets the documentation version to <projectversion>", "S256", fDocsProjectVersion, "CHPLDOC_PROJECT_VERSION", NULL},


 // TODO: Whether or not to support this flag is an open discussion. Currently,
 //       it is not supported, so the flag is always true.
 //       (thomasvandoren, 2015-03-08)
 //{"externs", ' ', NULL, "Include externs", "n", &fDocsIncludeExterns, NULL, NULL},
 {"print-commands", ' ', NULL, "[Don't] print system commands", "N", &printSystemCommands, "CHPL_PRINT_COMMANDS", NULL},

 {"", ' ', NULL, "Information Options", NULL, NULL, NULL, NULL},
 DRIVER_ARG_HELP,
 DRIVER_ARG_HELP_ENV,
 DRIVER_ARG_HELP_SETTINGS,
 DRIVER_ARG_VERSION,
 DRIVER_ARG_COPYRIGHT,
 DRIVER_ARG_LICENSE,

 {"", ' ', NULL, "Developer Flags", NULL, NULL, NULL, NULL},
 DRIVER_ARG_DEVELOPER,
 DRIVER_ARG_BREAKFLAGS_COMMON,
 DRIVER_ARG_DEBUGGERS,
 DRIVER_ARG_PRINT_CHPL_HOME,

 DRIVER_ARG_LAST
};
