/*
 * Copyright 2004-2015 Cray Inc.
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

#define _FILE_UTILS_C

#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include "chpl-file-utils.h"

#include <stdio.h>
#include <errno.h>
#include <sys/param.h> // MAXPATHLEN
#include <sys/stat.h>


qioerr chpl_fs_chdir(const char* name) {
  qioerr err = 0;
  int exitStatus = chdir(name);
  if (exitStatus)
    err = qio_mkerror_errno();
  return err;
}

qioerr chpl_fs_chmod(const char* name, int mode) {
  qioerr err = 0;
  int exitStatus = chmod(name, mode);
  if (exitStatus)
    err = qio_mkerror_errno();
  return err;
}

qioerr chpl_fs_chown(const char* name, int uid, int gid) {
  qioerr err = 0;
  int exitStatus = chown(name, uid, gid);
  if (exitStatus)
    err = qio_mkerror_errno();
  return err;
}

// This routine returns a malloc'd string (through the working_dir pointer)
// that must be deallocated by the caller.
qioerr chpl_fs_cwd(const char** working_dir) {
  qioerr err = 0;
  size_t bufsize = MAXPATHLEN*sizeof(char);
  char* bufptr;
  char* pathbuf = (char *)qio_malloc(bufsize);
  bufptr = getcwd(pathbuf, bufsize);
  if (bufptr == NULL)
    err = qio_mkerror_errno();
  else
    *working_dir = pathbuf;
  return err;
}

qioerr chpl_fs_exists(int* ret, const char* name) {
  qioerr err = 0;
  struct stat buf;
  // Stat will attempt to follow all symbolic links.  If the link is broken,
  // this means we will detect it and return false.
  int exitStatus = stat(name, &buf);
  if (exitStatus == -1 && errno == ENOENT) {
    // File or directory does not exist, return false
    *ret = 0;
  } else if (exitStatus) {
    // Another error occurred.  Return it.
    err = qio_mkerror_errno();
  } else {
    // The file or directory exists, return true
    *ret = 1;
  }
  return err;
}

qioerr chpl_fs_get_uid(int* ret, const char* name) {
  struct stat buf;
  int exitStatus = stat(name, &buf);
  if (exitStatus)
    return qio_mkerror_errno();
  *ret = buf.st_uid;
  return 0;
}

qioerr chpl_fs_get_gid(int* ret, const char* name) {
  struct stat buf;
  int exitStatus = stat(name, &buf);
  if (exitStatus)
    return qio_mkerror_errno();
  *ret = buf.st_gid;
  return 0;
}

qioerr _chpl_fs_check_mode(int* ret, const char* name, int mode_flag) {
  struct stat buf;
  int exitStatus = stat(name, &buf);
  if (exitStatus)
    return qio_mkerror_errno();
  *ret = (buf.st_mode&mode_flag) != 0;
  return 0;
}

qioerr chpl_fs_is_dir(int* ret, const char* name) {
  return _chpl_fs_check_mode(ret, name, S_IFDIR);
}

qioerr chpl_fs_is_file(int* ret, const char* name) {
  return _chpl_fs_check_mode(ret, name, S_IFREG);
}

qioerr chpl_fs_is_link(int* ret, const char* name) {
  // Note: Cannot use _chpl_fs_check_mode in this case because stat follows
  // symbolic links instead of evaluating the link itself.  The similar
  // comparison is also not valid when an unlinked file is provided.
  struct stat buf;
  int exitStatus = lstat(name, &buf);
  if (exitStatus)
    return qio_mkerror_errno();
  *ret = S_ISLNK(buf.st_mode);
  return 0;
}

/* Creates a directory with the given name and settings if possible,
   returning a qioerr if not. If parents != 0, then the callee wishes
   to create all interim directories necessary as well. */
qioerr chpl_fs_mkdir(const char* name, int mode, int parents) {
  qioerr err = 0;
  int exitStatus;
  if (!parents) {
    // Simple, easy.  Callee didn't specify recursive creation, so
    // if something fails, they get to deal with it.
    exitStatus = mkdir(name, mode);
  } else {
    int len = strlen(name);
    char tmp[len+1];
    int index;
    struct stat statRes;
    // We don't actually care about the full result of the stat calls, merely
    // the existance and state of the directory being accessed.
    while (name[len-1] == '/') {
      // In case the caller, in their infinite wisdom, decides to send
      // a directory name of the form "foo///////".
      len--;
      // Note: not being able to mix declarations and code means that
      // tmp must be created larger than might be necessary.
    }
    // Copy each step of the directory path into a temporary string,
    // creating the parent directories as needed. In the case of name
    // being "foo/bar/baz", this means that tmp will be "foo/" and then
    // "foo/bar" for each inner call of mkdir.
    for (index = 0; name[index] != '\0' && index < len; index++) {
      tmp[index] = name[index];
      if(tmp[index] == '/') {
        tmp[index+1] = '\0';
        exitStatus = stat(tmp, &statRes);
        if (exitStatus == -1 && errno == ENOENT) {
          // This error means we could not find the parent directory, so need
          // to create it.
          exitStatus = mkdir(tmp, mode);
        }
        // EEXIST could occur from the mkdir call above if the directory came
        // into existence between when we checked and when we tried to create
        // it.  There's really nothing to be done about it, so skip it and
        // continue on.
        if (exitStatus && errno != EEXIST) {
          // We encountered an error making a parent directory or during the
          // stat call to determine if we need to make a directory.  We will
          // encounter errors for every step after this, so return this one
          // as it will be more informative.
          err = qio_mkerror_errno();
          return err;
        }
      }
    }
    tmp[len] = '\0';
    exitStatus = mkdir(tmp, mode);
    if (exitStatus && errno == EEXIST) {
      // If we encounted EEXIST when creating the last directory, ignore it.
      // This behavior is consistent with the command line mkdir -p behavior.
      exitStatus = 0;
    }
  }
  if (exitStatus) {
    err = qio_mkerror_errno();
  }
  return err;
}

/* Renames the file from oldname to newname, returning a qioerr if one
   occurred. */
qioerr chpl_fs_rename(const char* oldname, const char* newname) {
  qioerr err = 0;
  int exitStatus = rename(oldname, newname);
  // utilizes the C library function rename.
  if (exitStatus)
    err = qio_mkerror_errno();
  return err;
}

/* Removes the file specified, returning a qioerr if one occurred. */
qioerr chpl_fs_remove(const char* name) {
  qioerr err = 0;
  int exitStatus = remove(name);
  if (exitStatus)
    err = qio_mkerror_errno();
  return err;
}

qioerr chpl_fs_samefile(int* ret, qio_file_t* file1, qio_file_t* file2) {
  qioerr err = 0;
  struct stat f1;
  struct stat f2;

  int exitStatus = fstat(file1->fd, &f1);
  if (exitStatus) {
    // An error occurred.  Return it.
    err = qio_mkerror_errno();
    return err;
  }
  exitStatus = fstat(file2->fd, &f2);
  if (exitStatus) {
    // An error occurred.  Return it.
    err = qio_mkerror_errno();
  } else {
    if (f1.st_dev == f2.st_dev && f1.st_ino == f2.st_ino) {
      // The files had the same device and inode numbers.  Return true
      *ret = 1;
    } else {
      // At least one of these was different.  Return false;
      *ret = 0;
    }
  }

  return err;
}

qioerr chpl_fs_samefile_string(int* ret, const char* file1, const char* file2) {
  qioerr err = 0;
  struct stat f1;
  struct stat f2;

  int exitStatus = stat(file1, &f1);
  if (exitStatus) {
    // An error occurred.  Return it.
    err = qio_mkerror_errno();
    return err;
  }
  exitStatus = stat(file2, &f2);
  if (exitStatus) {
    // An error occurred.  Return it.
    err = qio_mkerror_errno();
  } else {
    if (f1.st_dev == f2.st_dev && f1.st_ino == f2.st_ino) {
      // The files had the same device and inode numbers.  Return true
      *ret = 1;
    } else {
      // At least one of these was different.  Return false;
      *ret = 0;
    }
  }
  return err;
}

/* creates a symlink named linkName to the file orig */
qioerr chpl_fs_symlink(const char* orig, const char* linkName) {
  qioerr err = 0;
  int exitStatus = symlink(orig, linkName);
  if (exitStatus)
    err = qio_mkerror_errno();
  return err;

}

/* Returns the current permissions on a file specified by name */
qioerr chpl_fs_viewmode(int* ret, const char* name) {
  struct stat buf;
  int exitStatus = stat(name, &buf);
  if (exitStatus)
    return qio_mkerror_errno();
  *ret = (int)(buf.st_mode&(S_IRWXU | S_IRWXG | S_IRWXO | S_ISUID | S_ISGID | S_ISVTX));
  // Stylistic decision: while we have the capacity to make sure all we're
  // getting are the permissions bits in module code, sending that extra
  // information strikes me as unnecessary, since we don't intend to use it at
  // the module level in other circumstances.
  return 0;
}
