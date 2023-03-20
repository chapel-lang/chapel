/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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
void setHome(const ArgumentDescription* desc, const char* arg);

#define DRIVER_ARG_COPYRIGHT \

#define DRIVER_ARG_BREAKFLAGS_COMMON \

#define DRIVER_ARG_DEBUGGERS                                            \

#define DRIVER_ARG_DEVELOPER \

#define DRIVER_ARG_HELP \

#define DRIVER_ARG_HELP_ENV \

#define DRIVER_ARG_HELP_SETTINGS \

#define DRIVER_ARG_LICENSE \

#define DRIVER_ARG_PRINT_CHPL_HOME \

#define DRIVER_ARG_VERSION \

#define DRIVER_ARG_LAST \
  {0}

#endif
