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

#include "filesystem_help.h"

#include "./my_strerror_r.h"

#include "chpl/framework/ErrorMessage.h"
#include "chpl/framework/Location.h"

#include "llvm/Support/FileSystem.h"

#include <cerrno>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>

namespace chpl {


static void removeSpacesBackslashesFromString(char* str)
{
 char* src = str;
 char* dst = str;
 while (*src != '\0')
 {
   *dst = *src++;
   if (*dst != ' ' && *dst != '\\')
       dst++;
 }
 *dst = '\0';
}

static std::string my_strerror(int errno_) {
  char errbuf[256];
  int rc;
  errbuf[0] = '\0';
  rc = my_strerror_r(errno_, errbuf, sizeof(errbuf));
  if (rc != 0)
    strncpy(errbuf, "<unknown error>", sizeof(errbuf));
  return std::string(errbuf);
}

/*
 * Find the default tmp directory. Try getting the tmp dir from the ISO/IEC
 * 9945 env var options first, then P_tmpdir, then "/tmp".
 */
static std::string getTempDir() {
  std::vector<std::string> possibleDirsInEnv = {"TMPDIR", "TMP", "TEMP", "TEMPDIR"};
  for (unsigned int i = 0; i < possibleDirsInEnv.size(); i++) {
    auto curDir = getenv(possibleDirsInEnv[i].c_str());
    if (curDir != NULL) {
      return curDir;
    }
  }
#ifdef P_tmpdir
  return P_tmpdir;
#else
  return "/tmp";
#endif
}

FILE* openfile(const char* path, const char* mode, std::string& errorOut) {
  FILE* fp = fopen(path, mode);
  if (fp == nullptr) {
    // set errorOut. NULL will be returned.
    errorOut = my_strerror(errno) + ".";
  }

  return fp;
}

bool closefile(FILE* fp, const char* path, std::string& errorOut) {
  int rc = fclose(fp);
  if (rc != 0) {
    errorOut = my_strerror(errno) + ".";
    return false;
  }
  return true;
}

bool readfile(const char* path, std::string& strOut, std::string& errorOut) {
  FILE* fp = openfile(path, "r", errorOut);
  if (!fp) {
    return false;
  }

  // Try to get the file length in order to optimize string allocation
  long len = 0;
  fseek(fp, 0, SEEK_END);
  len = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  if (len > 0) strOut.reserve(len);

  char buf[256];
  while (true) {
    size_t got = fread(buf, 1, sizeof(buf), fp);
    if (got > 0) {
      strOut.append(buf, got);
    } else {
      if (ferror(fp)) {
        std::string ignoredErrorOut;
        closefile(fp, path, ignoredErrorOut);
        return false;
      }
      // otherwise, end of file reached
      break;
    }
  }

  return closefile(fp, path, errorOut);
}

bool fileExists(const char* path) {
  struct stat s;
  int err = stat(path, &s);
  return err == 0;
}


std::error_code deleteDir(std::string dirname) {
  // LLVM 5 added remove_directories
  return llvm::sys::fs::remove_directories(dirname, false);
}

std::error_code makeTempDir(std::string dirPrefix, std::string& tmpDirPathOut) {
  std::string tmpdirprefix = std::string(getTempDir()) + "/" + dirPrefix;
  std::string tmpdirsuffix = ".deleteme-XXXXXX";

  struct passwd* passwdinfo = getpwuid(geteuid());
  const char* userid;
  if (passwdinfo == NULL) {
    userid = "anon";
  } else {
    userid = passwdinfo->pw_name;
  }
  char* myuserid = strdup(userid);
  removeSpacesBackslashesFromString(myuserid);
  std::string tmpDir = tmpdirprefix + std::string(myuserid) + tmpdirsuffix;
  char* tmpDirMut = strdup(tmpDir.c_str());
  // TODO: we could use llvm::sys::fs::createUniqueDirectory instead of mkdtemp
  // see the comment in LLVM source
  // https://llvm.org/doxygen/Path_8cpp_source.html#l00883
  auto dirRes = std::string(mkdtemp(tmpDirMut));

  // get the posix error code if mkdtemp failed.
  int err = errno;

  if (dirRes.empty()) {
    std::error_code ret = make_error_code(static_cast<std::errc>(err));
    return ret;
  }

  free(myuserid); myuserid = NULL;
  free(tmpDirMut); tmpDirMut = NULL;

  tmpDirPathOut = dirRes;
  return std::error_code();
}

std::error_code ensureDirExists(std::string dirname) {
  return llvm::sys::fs::create_directories(dirname);
}

// Functionality also exists in runtime/src/qio/sys.c
// returns empty std::error_code on success.
std::error_code currentWorkingDir(std::string& path_out) {
  llvm::SmallString<128> buf;
  if (std::error_code err = llvm::sys::fs::current_path(buf)) {
    return err;
  } else {
    // buf.str() returns an LLVM::StringRef,
    // so we call .str() on that to get a std::string.
    path_out = buf.str().str();
    return std::error_code();
  }
}

std::error_code makeDir(std::string dirpath, bool makeParents) {
  using namespace llvm::sys::fs;
  if (makeParents) {
    return create_directories(dirpath, true, perms::all_all);
  } else {
    return create_directory(dirpath, true, perms::all_all);
  }
}

std::string getExecutablePath(const char* argv0, void* MainExecAddr) {
  using namespace llvm::sys::fs;
  return getMainExecutable(argv0, MainExecAddr);
}


} // namespace chpl
