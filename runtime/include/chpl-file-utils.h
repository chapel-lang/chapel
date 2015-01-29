#ifndef _FILE_UTILS_H_
#define _FILE_UTILS_H_

#include "qio_error.h"
#include "sys_basic.h"


qioerr chpl_fs_chdir(const char* name);

// Change the permissions of a given file, returning a qioerr if errors
// occurred.
qioerr chpl_fs_chmod(const char* name, int mode);

qioerr chpl_fs_chown(const char* name, int uid, int gid);

qioerr chpl_fs_cwd(const char** working_dir);

qioerr chpl_fs_exists(int* ret, const char* name);

qioerr _chpl_fs_check_mode(int* ret, const char* name, int mode_flag);

qioerr chpl_fs_is_dir(int* ret, const char* name);
qioerr chpl_fs_is_file(int* ret, const char* name);
qioerr chpl_fs_is_link(int* ret, const char* name);

// Creates a directory with the given name and settings if possible,
// returning a qioerr if not.
qioerr chpl_fs_mkdir(const char* name, int mode, int parents);

// Renames the file from oldname to newname, returning a qioerr if one
// occured.
qioerr chpl_fs_rename(const char* oldname, const char* newname);
// Removes the file specified, returning a qioerr if one occurred
qioerr chpl_fs_remove(const char* name);


#endif
