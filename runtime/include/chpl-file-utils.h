/*
 * Copyright 2004-2020 Cray Inc.
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
#include "qio.h"
#include "sys_basic.h"

#include "sys/stat.h"

qioerr chpl_fs_chdir(const char* name);

// Change the permissions of a given file, returning a qioerr if errors
// occurred.
qioerr chpl_fs_chmod(const char* name, int mode);

qioerr chpl_fs_chown(const char* name, int uid, int gid);

qioerr chpl_fs_copy_metadata(const char* source, const char* dest);

qioerr chpl_fs_cwd(const char** working_dir);

qioerr chpl_fs_exists(int* ret, const char* name);

qioerr chpl_fs_get_size(int64_t* ret, const char* name);
qioerr chpl_fs_get_uid(int* ret, const char* name);
qioerr chpl_fs_get_gid(int* ret, const char* name);

qioerr _chpl_fs_check_mode(int* ret, const char* name, int mode_flag);

qioerr chpl_fs_is_dir(int* ret, const char* name);
qioerr chpl_fs_is_file(int* ret, const char* name);
qioerr chpl_fs_is_link(int* ret, const char* name);
qioerr chpl_fs_is_mount(int* ret, const char* name);

// Creates a directory with the given name and settings if possible,
// returning a qioerr if not.
qioerr chpl_fs_mkdir(const char* name, int mode, int parents);

// Takes a path string and resolves and removes all "/../", "/./", and
// symbolic links, returning the result in shortened
qioerr chpl_fs_realpath(const char* path, const char **shortened);
// Takes a qio_file_t and returns the realpath it refers to.
qioerr chpl_fs_realpath_file(qio_file_t* path, const char **shortened);

// Renames the file from oldname to newname, returning a qioerr if one
// occurred.
qioerr chpl_fs_rename(const char* oldname, const char* newname);
// Removes the file specified, returning a qioerr if one occurred
qioerr chpl_fs_remove(const char* name);

qioerr chpl_fs_samefile(int* ret, qio_file_t* file1, qio_file_t* file2);
qioerr chpl_fs_samefile_string(int* ret, const char* file1, const char* file2);

qioerr chpl_fs_symlink(const char* orig, const char* linkName);

mode_t chpl_fs_umask(mode_t mask);

qioerr chpl_fs_viewmode(int* ret, const char* name);

#endif
