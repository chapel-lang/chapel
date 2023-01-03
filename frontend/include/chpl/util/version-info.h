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

#ifndef CHPL_UTIL_VERSION_INFO_H
#define CHPL_UTIL_VERSION_INFO_H

namespace chpl
{

/*
  Returns the gitSHA value from the most recent source modification that was
  committed.
*/
const char* gitSHA();

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
  get the chpl version string. Developer flag helps decide if a git-sha is
  included or not
*/
std::string getVersion(bool developer);

/*
  get the official release flag that was set through the top-level CMakeLists.txt
  file.
*/
bool getIsOfficialRelease();

/*
  get the build version value. This may be an integer or a git-sha depending on
  the value of official_release and developer.
*/
const char* getBuildVersion(bool developer);

} // namespace chpl


#endif // CHPL_UTIL_VERSION_INFO_H
