/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

#define _FILE_UTILS_C

#ifndef CHPL_RT_UNIT_TEST
#include "chplrt.h"
#endif

#include "chpl-file-utils.h"

#include <stdio.h>
#include <errno.h>
#include <sys/param.h> // MAXPATHLEN
#include <sys/stat.h>
#include <utime.h> // Defines utimbuf and utime()


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

qioerr chpl_fs_copy_metadata(const char* source, const char* dest) {
  qioerr err = 0;
  struct stat oldTimes;
  struct utimbuf times;
  int exitStatus = stat(source, &oldTimes);
  if (exitStatus == -1) {
    // Hopefully an error will not occur (as we have checked that the
    // file exists when we perform the other operations on it).  But just in
    // case, check it here.
    err = qio_mkerror_errno();
    return err;
  }
  times.actime = oldTimes.st_atime;  // The access time
  times.modtime = oldTimes.st_mtime; // The modification time
  exitStatus = utime(dest, &times);  // Set the times for dest.
  if (exitStatus == -1) {
    err = qio_mkerror_errno();
  }
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
  if (bufptr == NULL) {
    err = qio_mkerror_errno();
    qio_free(pathbuf);
  } else {
    *working_dir = pathbuf;
  }
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

qioerr chpl_fs_get_size(int64_t* ret, const char* name) {
  struct stat buf;
  int exitStatus = stat(name, &buf);
  if (exitStatus)
    return qio_mkerror_errno();
  *ret = buf.st_size;
  return 0;
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
  if (exitStatus == -1 && errno == ENOENT) {
    // The link examined does not exist, return false
    *ret = 0;
    return 0;
  } else if (exitStatus) {
    return qio_mkerror_errno();
  }
  *ret = S_ISLNK(buf.st_mode);
  return 0;
}

qioerr chpl_fs_is_mount(int* ret, const char* name) {
  qioerr err = 0;
  struct stat nBuf, parentBuf;
  int exitStatus = 0;
  size_t nameLen = strlen(name);
  char* parent = (char* ) chpl_mem_allocMany(nameLen + 4, sizeof(char), CHPL_RT_MD_OS_LAYER_TMP_DATA, 0, 0);
  char* safeNameCopy = (char* ) chpl_mem_allocMany(nameLen + 1, sizeof(char), CHPL_RT_MD_OS_LAYER_TMP_DATA, 0, 0);
  strcpy(safeNameCopy, name);
  // Need to copy name so that we can use it in the case of links

  err = chpl_fs_is_link(&exitStatus, name);
  if (err) {
    // The stat call in is_link returned an error, which we would encounter too,
    // so return immediately.
    chpl_mem_free(parent, 0, 0);
    chpl_mem_free(safeNameCopy, 0, 0);
    return err;
  } else if (exitStatus) {
    // We are dealing with a link.  Using /.. will refer to the parent of the
    // linked location, rather than the parent of the link itself.  We need to
    // perform some string token action.

    // Lydia note (03/17/2015): when the Path library is more fleshed out, this
    // operation could be done in module code and this function would instead
    // take the name of the parent and child instead of creating the parent name
    // itself.

    char* curTok = strtok(safeNameCopy, "/");
    char* nextTok = strtok(NULL, "/");
    // We need the next token to determine if the path is longer than a single
    // link name.
    assert(curTok != NULL);
    // curTok should never be null.  The only string which would return null is
    // "/", but that directory is not a link, so won't be here in the first
    // place.

    if (nextTok != NULL) {
      // name includes a path longer than just the current symlink.
      // Thus, we should copy up to (but not including) the basename of the
      // path.
      strcpy(parent, curTok);
      curTok = nextTok;
      nextTok = strtok(NULL, "/");
      while (nextTok != NULL) {
        // While we haven't found the end of the path (in nextTok)
        strcat(parent, "/");
        // Restore the lost path separator.
        strcat(parent, curTok);
        // Add the current token to the parent list
        curTok = nextTok;
        // And prepare to check if the next token is the last in the path
        nextTok = strtok(NULL, "/");
      }
    } else {
      // name was merely the current symlink rather than a longer path.
      // That means its parent is "." or the current directory.
      strcpy(parent, ".");
    }
  } else {
    // We are not referring to a link, so concatenating "/.." is fine.
    strcpy(parent, name);
    strcat(parent, "/..");
    // TODO: Using "/" is not necessarily portable, look into this
  }

  exitStatus = lstat(name, &nBuf);
  if (exitStatus) {
    err = qio_mkerror_errno();
    chpl_mem_free(parent, 0, 0);
    chpl_mem_free(safeNameCopy, 0, 0);
    return err;
  }
  exitStatus = lstat(parent, &parentBuf);
  if (exitStatus) {
    err = qio_mkerror_errno();
  } else {
    if (nBuf.st_dev != parentBuf.st_dev) {
      *ret = 1;
    // Check if the st_dev matches that of its parent directory.
    // If they don't match, it is a mount point.
    } else {
      err = chpl_fs_samefile_string(ret, name, parent);
      // If the parent directory is the same as the current directory, we've
      // reached the root.  If they don't, we know it isn't a mount point
      // because we already know their st_dev matches.
    }
  }
  chpl_mem_free(parent, 0, 0);
  chpl_mem_free(safeNameCopy, 0, 0);
  return err;
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
    // the existence and state of the directory being accessed.
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
      // If we encountered EEXIST when creating the last directory, ignore it.
      // This behavior is consistent with the command line mkdir -p behavior.
      exitStatus = 0;
    }
  }
  if (exitStatus) {
    err = qio_mkerror_errno();
  }
  return err;
}

qioerr chpl_fs_realpath(const char* path, const char **shortened) {
  qioerr err = 0;
  size_t bufsize = MAXPATHLEN*sizeof(char);
  char* bufptr;
  char* pathbuf = (char *)qio_malloc(bufsize);

  bufptr = realpath(path, pathbuf);
  if (bufptr == NULL) {
    // If an error occurred, bufptr will be NULL.  Otherwise, it will
    // point to pathbuf anyways
    err = qio_mkerror_errno();
    qio_free(pathbuf);
  } else {
    *shortened = pathbuf;
  }
  return err;
}

qioerr chpl_fs_realpath_file(qio_file_t* path, const char **shortened) {
  char *unshortened = NULL;
  qioerr err = 0;
  err = qio_file_path(path, (const char **)&unshortened);
  // qio already had a way to get the path from the qio_file_t, so use it.

  // check the error status here.
  if (err) {
    return err;
  }
  err = chpl_fs_realpath(unshortened, shortened);
  qio_free(unshortened);
  // Since what is returned from qio_file_path is not necessarily the realpath,
  // call realpath on it before returning.
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

mode_t chpl_fs_umask(mode_t mask) {
  return umask(mask);
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
