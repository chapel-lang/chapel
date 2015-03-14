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

#ifndef _common_flags_H_
#define _common_flags_H_

#include "arg.h"

// Shared flags.
extern bool fPrintCopyright;
extern bool fPrintEnvHelp;
extern bool fPrintHelp;
extern bool fPrintLicense;
extern bool fPrintSettingsHelp;
extern bool fPrintVersion;

// Shared setter functions.
void driverSetHelpTrue(const ArgumentState* state, const char* unused);


#define DRIVER_ARG_COPYRIGHT \
  {"copyright", ' ', NULL, "Show copyright", "F", &fPrintCopyright, NULL, NULL}

#define DRIVER_ARG_HELP \
  {"help", 'h', NULL, "Help (show this list)", "F", &fPrintHelp, NULL, NULL}

#define DRIVER_ARG_HELP_ENV \
  {"help-env", ' ', NULL, "Environment variable help", "F", &fPrintEnvHelp, "", driverSetHelpTrue}

#define DRIVER_ARG_HELP_SETTINGS \
  {"help-settings", ' ', NULL, "Current flag settings", "F", &fPrintSettingsHelp, "", driverSetHelpTrue}

#define DRIVER_ARG_LICENSE \
  {"license", ' ', NULL, "Show license", "F", &fPrintLicense, NULL, NULL}

#define DRIVER_ARG_VERSION \
  {"version", ' ', NULL, "Show version", "F", &fPrintVersion, NULL, NULL}

#define DRIVER_ARG_LAST \
  {0}

#endif
