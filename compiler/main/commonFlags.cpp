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

#include "commonFlags.h"

bool ccwarnings = false;
bool developer = false;
bool fPrintChplHome = false;
bool fPrintCopyright = false;
bool fPrintEnvHelp = false;
bool fPrintHelp = false;
bool fPrintLicense = false;
bool fPrintSettingsHelp = false;
bool fPrintVersion = false;
bool fRungdb = false;
bool fRunlldb = false;

void driverSetHelpTrue(const ArgumentDescription* desc, const char* unused) {
  fPrintHelp = true;
}

void driverSetDevelSettings(const ArgumentDescription* desc, const char* arg_unused) {
  // have to handle both cases since this will be called with --devel
  // and --no-devel
  if (developer) {
    ccwarnings = true;
  } else {
    ccwarnings = false;
  }
}
