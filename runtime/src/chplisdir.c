/*
 * Copyright 2004-2014 Cray Inc.
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

#include "sys_basic.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "chplisdir.h"
#include "chplrt.h"
#include "error.h"

int chpl_rt_isDir(const char* pathname, chpl_bool followLinks) {
  struct stat fileinfo;
  if (followLinks) {
    int status = stat(pathname, &fileinfo);
    if (status) {
      chpl_internal_error("Fatal error in stat()");
    }
    return S_ISDIR(fileinfo.st_mode);
    /*  return fileinfo.st_mode & S_IFDIR;*/
  } else {
    int status = lstat(pathname, &fileinfo);
    if (status) {
      chpl_internal_error("Fatal error in lstat()");
    }
    if (fileinfo.st_mode & S_IFLNK) {
      return false;
    } else {
      return S_ISDIR(fileinfo.st_mode);
    }
  }
}
