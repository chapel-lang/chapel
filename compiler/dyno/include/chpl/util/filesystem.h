/*
 * Copyright 2021-2022 Hewlett Packard Enterprise Development LP
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

#include "chpl/framework/ErrorMessage.h"

#include <string>

namespace chpl {


/**
  Reads the contents of a file into a string.
  If something failed, returns false and sets errorOut.
 */
bool readfile(const char* path, std::string& strOut, ErrorMessage& errorOut);

/**
  Checks to see if a file exists at path. Returns 'true' if it does.
 */
bool fileExists(const char* path);

/**
 * create the directory specified by argument dirname
 *
 * dirname the path of the directory to create
 * returns std::error_code
 */
std::error_code ensureDirExists(std::string dirname);


/**
 * creates a directory in the temp location for the system
 * with the pattern "dirPrefix-username.deleteme-XXXXXX/"
 *
 * dirPrefix a prefix to place in the directory name
 * tmpDirPathOut reference to an empty string that will hold the path of the created directory
 * returns std::error_code
 */
std::error_code makeTempDir(std::string dirPrefix, std::string& tmpDirPathOut);

/**
 * forwards to llvm::sys::fs::remove_directories
 *
 * dirname the path of the directory to remove
 * returns std::error_code
 */
std::error_code deleteDir(std::string dirname);

/*
 * Returns the current working directory. Does not report failures. Use
 * currentWorkingDir() if you need error reports.
 */
std::string getCwd();

/*
 * Gets the current working directory
 * (uses LLVM sys::fs::current_path internally).
 */
std::error_code currentWorkingDir(std::string& path_out);

/**
 * makes the directory and all intermediate directories in dirpath
 * if they don't exist. Directory permissions are set to llvm::all-all.
 * which should be equivalent to  S_IRWXU | S_IRWXG | S_IRWXO
 *
 * dirpath - the path of the directory to create
 * returns - std::error_code
 */
std::error_code makeDir(std::string dirpath);

} // end namespace chpl

#endif
