/*
 * Copyright 2020-2024 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UTIL_VERSION_INFO_H
#define CHPL_UTIL_VERSION_INFO_H

namespace chpl
{

/*
  Returns the gitSHA value from the most recent source modification that was
  committed.
*/
const char* getCommitHash();

/*
  The chpl major version value
*/
int getMajorVersion();

/*
  The chpl minor version value
*/
int getMinorVersion();

/*
  The chpl update version value
*/
int getUpdateVersion();

/*
  The configured installation prefix for a chapel installation. Typically set
  by running ./configure from the chapel source directory.
*/
const char* getConfiguredPrefix();

/*
  Get the major.minor version of chpl. Typically used for writing paths used
  during installation
*/
std::string getMajorMinorVersion();

/*
  Get the chpl version string. Release versions will have format Major.Minor.Patch or
  Major.Minor.Patch.Build. Pre-release versions will be Major.Minor.Patch pre-release (git-sha)
*/
std::string getVersion();

/*
  Get the official release flag that was set through the top-level CMakeLists.txt
  file, defined there as CHPL_OFFICIAL_RELEASE.
*/
bool getIsOfficialRelease();


} // namespace chpl


#endif // CHPL_UTIL_VERSION_INFO_H
