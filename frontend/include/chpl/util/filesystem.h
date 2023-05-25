/*
 * Copyright 2021-2023 Hewlett Packard Enterprise Development LP
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

#include "llvm/Config/llvm-config.h"

#include "llvm/Support/ErrorOr.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/Twine.h"

#include <array>
#include <string>
#include <system_error>

namespace chpl {


/**
  Reads the contents of a file into a string.
  If something failed, returns false and sets errorOut.
 */
bool readfile(const char* path, std::string& strOut, std::string& errorOut);

/**
  Writes a string to a file, replacing its contents.
  Returns a std::error_code of 0 if everything worked OK.
 */
std::error_code writeFile(const char* path, const std::string& data);

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
std::error_code ensureDirExists(const llvm::Twine& dirname);


/**
 * creates a directory in the temp location for the system
 * with the pattern "<TMP>/<dirPrefix>-<username>.deleteme-XXXXXX/"
 * (where <TMP> is typically /tmp)
 *
 * prefix a prefix to put at the start of the directory name
 * tmpDirPathOut ref to a string that be set to the path of  the created dir
 * returns std::error_code
 */
std::error_code makeTempDir(llvm::StringRef prefix, std::string& tmpDirPathOut);

/**
 * forwards to llvm::sys::fs::remove_directories
 *
 * dirname the path of the directory to remove
 * returns std::error_code
 */
std::error_code deleteDir(const llvm::Twine& dirname);

/*
 * Gets the current working directory
 * (uses LLVM sys::fs::current_path internally).
 */
std::error_code currentWorkingDir(std::string& path_out);

/**
 * makes the directory in dirpath. If makeParents is false, this will fail
 * if a directory in the path doesn't already exist.
 * Directory permissions are set to llvm::all-all.
 * which should be equivalent to  S_IRWXU | S_IRWXG | S_IRWXO
 *
 * dirpath - the path of the directory to create
 * makeParents - if true, creates parent directories if they don't exist
 * returns - std::error_code
 */
std::error_code makeDir(const llvm::Twine& dirpath, bool makeParents=false);


/*
  Try to get the path of the executable. We rely on llvm implementation,
  which states it _may_ return an empty path if it fails.
  https://llvm.org/doxygen/namespacellvm_1_1sys_1_1fs.html#a057a733b2dfa2f0531ceb335cf3b1d03
*/
std::string getExecutablePath(const char* argv0, void* MainExecAddr);


/**
  Compare two paths to see if they point to the same filesystem object.
  Utilizes llvm::sys::fs:equivalent to do the comparison.
*/
bool isSameFile(const llvm::Twine& path1, const llvm::Twine& path2);


#if LLVM_VERSION_MAJOR >= 13
/**
 Non-error result type for hashFile.
 */
using HashFileResult = std::array<uint8_t, 32>;
#else
// LLVM 12 and older don't have SHA256, so use the SHA1 size.
using HashFileResult = std::array<uint8_t, 20>;
#endif

/**
  Convert a HashFileResult to a string storing its hex value
  */
std::string fileHashToHex(const HashFileResult& hash);

/**
  Returns a cryptographic hash of the file contents at the passed path.
  Currently uses SHA256 but exactly which hash is used should be
  considered an implementation detail.
 */
llvm::ErrorOr<HashFileResult> hashFile(const llvm::Twine& path);

/**
  Sets the modification and access time of one file to the
  modification of another file.
 */
std::error_code copyModificationTime(const llvm::Twine& srcPath,
                                     const llvm::Twine& dstPath);


} // end namespace chpl


#endif
