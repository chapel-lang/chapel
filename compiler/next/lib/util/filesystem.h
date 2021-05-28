/*
 * Copyright 2021 Hewlett Packard Enterprise Development LP
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

#ifndef CHPL_UTIL_FILESYSTEM_H
#define CHPL_UTIL_FILESYSTEM_H

#include "chpl/queries/ErrorMessage.h"

#include <cstdio>
#include <string>

namespace chpl {

/**
  Open a file. If the open failed, return nullptr and set errorOut.
 */
FILE* openfile(const char* path, const char* mode, ErrorMessage& errorOut);

/**
  Close a file. If the close failed, return false and set errorOut.
 */
bool closefile(FILE* fp, const char* path, ErrorMessage& errorOut);

/**
  Reads the contents of a file into a string.
  If something failed, returns false and sets errorOut.
 */
bool readfile(const char* path, std::string& strOut, ErrorMessage& errorOut);

} // end namespace chpl

#endif
