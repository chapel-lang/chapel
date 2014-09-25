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

#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include "qio_error.h"
#include "sys_basic.h"


qioerr chpl_file_chdir(const char* name);

qioerr chpl_file_chown(const char* name, int uid, int gid);

qioerr chpl_file_cwd(const char** working_dir);

qioerr _chpl_file_check_mode(int* ret, const char* name, int mode_flag);

qioerr chpl_file_is_dir(int* ret, const char* name);
qioerr chpl_file_is_file(int* ret, const char* name);

// Creates a directory with the given name and settings if possible,
// returning a qioerr if not.
qioerr chpl_file_mkdir(const char* name, int mode, int parents);

// Renames the file from oldname to newname, returning a qioerr if one
// occured.
qioerr chpl_file_rename(const char* oldname, const char* newname);
// Removes the file specified, returning a qioerr if one occurred
qioerr chpl_file_remove(const char* name);


#endif
