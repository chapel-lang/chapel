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

#ifndef _common_flags_H_
#define _common_flags_H_

#include "arg.h"

// Shared flags.
extern bool ccwarnings;
extern bool fPrintChplHome;
extern bool fPrintCopyright;
extern bool fPrintEnvHelp;
extern bool fPrintHelp;
extern bool fPrintLicense;
extern bool fPrintSettingsHelp;
extern bool fPrintVersion;
extern bool fRungdb;
extern bool fRunlldb;

// Shared setter functions.
void driverSetHelpTrue(const ArgumentDescription* desc, const char* unused);
void driverSetDevelSettings(const ArgumentDescription* desc, const char* arg_unused);

#define DRIVER_ARG_COPYRIGHT \
  {"copyright", ' ', NULL, "Show copyright", "F", &fPrintCopyright, NULL, NULL}

#define DRIVER_ARG_BREAKFLAGS_COMMON \
  {"break-on-id", ' ', NULL, "Break when AST id is created", "I", &breakOnID, "CHPL_BREAK_ON_ID", NULL}, \
  {"break-on-remove-id", ' ', NULL, "Break when AST id is removed from the tree", "I", &breakOnRemoveID, "CHPL_BREAK_ON_REMOVE_ID", NULL}

#define DRIVER_ARG_DEBUGGERS                                            \
  {"gdb", ' ', NULL, "Run compiler in gdb", "F", &fRungdb, NULL, NULL}, \
  {"lldb", ' ', NULL, "Run compiler in lldb", "F", &fRunlldb, NULL, NULL}

#define DRIVER_ARG_DEVELOPER \
  {"devel", ' ', NULL, "Compile as a developer [user]", "N", &developer, "CHPL_DEVELOPER", driverSetDevelSettings}

#define DRIVER_ARG_HELP \
  {"help", 'h', NULL, "Help (show this list)", "F", &fPrintHelp, NULL, NULL}

#define DRIVER_ARG_HELP_ENV \
  {"help-env", ' ', NULL, "Environment variable help", "F", &fPrintEnvHelp, "", driverSetHelpTrue}

#define DRIVER_ARG_HELP_SETTINGS \
  {"help-settings", ' ', NULL, "Current flag settings", "F", &fPrintSettingsHelp, "", driverSetHelpTrue}

#define DRIVER_ARG_LICENSE \
  {"license", ' ', NULL, "Show license", "F", &fPrintLicense, NULL, NULL}

#define DRIVER_ARG_PRINT_CHPL_HOME \
  {"print-chpl-home", ' ', NULL, "Print CHPL_HOME and path to this executable and exit", "F", &fPrintChplHome, NULL,NULL}

#define DRIVER_ARG_VERSION \
  {"version", ' ', NULL, "Show version", "F", &fPrintVersion, NULL, NULL}

#define DRIVER_ARG_LAST \
  {0}

#endif
