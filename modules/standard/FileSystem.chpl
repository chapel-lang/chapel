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
use Error;


/* Change the current working directory of the current locale to the specified
   name. Returns any errors that occurred via an out parameter.
   err: a syserr used to indicate if an error occurred
   name: a string indicating a new directory

   Note: this is not safe within a parallel context.  A chdir call in one task
   will affect the current working directory of all tasks for that locale.
*/
proc locale.chdir(out err: syserr, name: string) {
  extern proc chpl_fs_chdir(name: c_string):syserr;

  on this {
    err = chpl_fs_chdir(name.c_str());
  }
}

/* Change the current working directory of the current locale to the specified
   name. Generates an error message if one occurred.
   name: a string indicating a new directory

   Note: this is not safe within a parallel context.  A chdir call in one task
   will affect the current working directory of all tasks for that locale.
*/
proc locale.chdir(name: string) {
  var err: syserr = ENOERR;
  chdir(err, name);
  if err != ENOERR then ioerror(err, "in chdir", name);
}

/* Set the permissions of the file or directory specified by name to that
   indicated by settings.  Returns any errors that occurred via an out
   parameter.

   :arg err: a syserr used to indicate if an error occurred
   :arg name: the name of the file/directory which should have its permissions
              alterred.
   :arg mode: an integer representing the permissions desired for the file in
              question.  See description of the provided constants for
              potential values.
*/
proc chmod(out err: syserr, name: string, mode: int) {
  extern proc chpl_fs_chmod(name: c_string, mode: int): syserr;

  err = chpl_fs_chmod(name.c_str(), mode);
}


/* Set the permissions of the file or directory specified by name to that
   indicated by settings, and may generate an error message

   :arg name: the name of the file/directory which should have its permissions
              alterred.
   :arg mode: an integer representing the permissions desired for the file
              in question.  See description of the provided constants for potential
              values.
*/
proc chmod(name: string, mode: int){
  var err: syserr = ENOERR;
  chmod(err, name, mode);
  if err != ENOERR then ioerror(err, "in chmod", name);
}


/* Changes one or both of the owner and group id of the named file to the
   specified values.  If uid or gid are -1, the value in question will remain
   unchanged.
   err: a syserr used to indicate if an error occurred
   name: the name of the file to be changed.
   uid: user id to use as new owner, or -1 if it should remain the same.
   gid: group id to use as the new group owner, or -1 if it should remain the same.
*/
proc chown(out err: syserr, name: string, uid: int, gid: int) {
  extern proc chpl_fs_chown(name: c_string, uid: c_int, gid: c_int):syserr;

  err = chpl_fs_chown(name.c_str(), uid:c_int, gid:c_int);
}

/* Changes one or both of the owner and group id of the named file to the
   specified values.  If uid or gid are -1, the value in question will remain
   unchanged. Generates an error message if one occurred.
   name: the name of the file to be changed.
   uid: user id to use as new owner, or -1 if it should remain the same.
   gid: group id to use as the new group owner, or -1 if it should remain the same.
*/
proc chown(name: string, uid: int, gid: int) {
  var err: syserr = ENOERR;
  chown(err, name, uid, gid);
  if err != ENOERR then ioerror(err, "in chown", name);
}

// FUTURE WORK:
// When basename and joinPath are supported, enable dest to be a directory.

/* Copies the contents and permissions of the file indicated by src into
   the file or directory dest.  If dest is a directory, will return an error via
   the out parameter.  If metadata is set to true, will also copy the metadata
   of the file to be copied.  Other errors may also be returned via the out
   parameter.
   err: a syserr used to indicate if an error occurred
   src: the source file whose contents and permissions are to be copied
   dest: the destination file for the contents and permissions.
   metadata: a boolean indicating whether to copy metadata (uid, gid, time of
             last access and time of modification) associated with the source
             file.
*/
proc copy(out err: syserr, src: string, dest: string, metadata: bool = false) {
  var destFile = dest;
  if (isDir(err, destFile)) {
    // destFile = joinPath(destFile, basename(src));
    err = EISDIR;
    // Supporting a destination directory requires getting the basename from
    // the src (because we're using the same name) and joining it with the
    // provided destination directory.  Both of those operations are part of
    // the string portion, so we aren't supporting it just yet.
    return;
  } else {
    if (err == ENOENT) {
      // Destination didn't exist before.  We'd be overwriting it anyways, so
      // we don't care.
      err = ENOERR;
    }
    if err != ENOERR then return;
  }
  copyFile(err, src, destFile);
  if err != ENOERR then return;
  copyMode(err, src, destFile);
  if err != ENOERR then return;

  if (metadata) {
    extern proc chpl_fs_copy_metadata(source: c_string, dest: c_string): syserr;

    // Copies the access time, and time of last modification.
    // Does not copy uid, gid, or mode
    err = chpl_fs_copy_metadata(src.c_str(), dest.c_str());

    // Get uid and gid from src
    var uid = getUID(err, src);
    if err != ENOERR then return;
    var gid = getGID(err, src);
    if err != ENOERR then return;
    // Change uid and gid to that of the src
    chown(err, destFile, uid, gid);
  }
}

/* Copies the contents and permissions of the file indicated by src into
   the file or directory dest.  If dest is a directory, will halt with an error
   message.  If metadata is set to true, will also copy the metadata of the
   file to be copied.  May halt with other error messages.
   src: the source file whose contents and permissions are to be copied
   dest: the destination file for the contents and permissions.
   metadata: a boolean indicating whether to copy metadata (uid, gid, time of
             last access and time of modification) associated with the source
             file.
*/
proc copy(src: string, dest: string, metadata: bool = false) {
  var err: syserr = ENOERR;
  copy(err, src, dest, metadata);
  if err != ENOERR then ioerror(err, "in copy(" + src + ", " + dest + ")");
}

/* Copies the contents of the file indicated by src into the file indicated
   by dest, replacing dest if it already exists (and is different than src).
   If the dest is not writable or src and dest are the same file, generates
   an error.  Does not copy metadata.
   err: a syserr used to indicate if an error occurred
   src: the source file whose contents are to be copied.
   dest: the destination of the contents.
*/
proc copyFile(out err: syserr, src: string, dest: string) {
  // This implementation is based off of the python implementation for copyfile,
  // with some slight differences.  That implementation was found at:
  // https://bitbucket.org/mirror/cpython/src/c8ce5bca0fcda4307f7ac5d69103ce128a562705/Lib/shutil.py?at=default
  // I did not look at the other functions in that file, except for copyfileobj
  // (which copyfile called).
  if (!exists(src)) {
    err = ENOENT;
    // Source didn't exist, we can't copy it.
    return;
  }
  if (isDir(err, src) || isDir(err, dest)) {
    // If the source is a directory, the user has made a mistake, so return an
    // error.  The same is true if the destination is a directory.
    err = EISDIR;
    return;
  }

  if (err == ENOENT) {
    err = ENOERR;
    // We don't care if dest did not exist before, we'll create or overwrite it
    // anyways.  We already know src exists.
  } else if (sameFile(err, src, dest)) {
    // Check if the files are the same, error if yes

    // Don't need to check if they're the same file when we know dest didn't
    // exist.
    err = EINVAL;
    return;
    // The second argument is invalid if the two arguments are the same.
  }

  // Lydia note (03/04/2014): These enclosing curly braces are to avoid the bug
  // found in test/io/lydia/outArgEarlyExit*.future.  When those futures are
  // resolved, the braces should be removed.
  {
    // Open src for reading, open dest for writing
    var srcFile = open(src, iomode.r);
    var destFile = open(dest, iomode.cw);
    var srcChnl = srcFile.reader(kind=ionative, locking=false);
    var destChnl = destFile.writer(kind=ionative, locking=false);
    // read in, write out.
    var line: [0..1023] uint(8);
    var numRead: int = 0;
    while (srcChnl.readline(line, numRead=numRead, error=err)) {
      // From mppf:
      // If you want it to be faster, we can make it only buffer once (sharing
      // the bytes read into memory between the two channels). To do that you'd
      // do something like this (in a loop):

      // srcReader.mark
      // srcReader.advance( buffer size )
      // srcReader.beginPeekBuffer to get the current buffer (qio_channel_begin_peek_buffer)
      // dstWriter.putBuffer with the buffer we got (qio_channel_put_buffer)
      // srcReader.endPeekBuffer

      // Some of these routines don't exist with Chapel wrappers now

      destChnl.write(line[0..#numRead]);
    }
    if err == EEOF then err = ENOERR;
    destChnl.flush();

    srcFile.close();
    destFile.close();
  }
}

/* Copies the contents of the file indicated by src into the file indicated
   by dest, replacing dest if it already exists (and is different than src).
   If the dest is not writable or src and dest are the same file, halts with
   an error.  Does not copy metadata.  May halt with an error message.
   src: the source file whose contents are to be copied.
   dest: the destination of the contents.
*/
proc copyFile(src: string, dest: string) {
  var err: syserr = ENOERR;
  copyFile(err, src, dest);
  if err != ENOERR then ioerror(err, "in copyFile(" + src + ", " + dest + ")");
}

/* Copies the permissions of the file indicated by src to the file indicated
   by dest, leaving contents, owner and group unaffected.
   err: a syserr used to indicate if an error occurred
   src: the source file whose permissions are to be copied.
   dest: the destination of the permissions.
*/
proc copyMode(out err: syserr, src: string, dest: string) {
  // Gets the mode from the source file.
  var srcMode = viewMode(err, src);
  // If any error occurred, we want to be the one reporting it, so as not
  // to bleed implementation details.  If we found one when viewing the
  // source's mode, we should return immediately.
  if err != ENOERR then return;
  // Sets the mode of the destination to the source's mode.
  chmod(err, dest, srcMode);
}

/* Copies the permissions of the file indicated by src to the file indicated
   by dest, leaving contents, owner and group unaffected.  May halt with an
   error message.
   src: the source file whose permissions are to be copied.
   dest: the destination of the permissions.
*/
proc copyMode(src: string, dest: string) {
  var err: syserr = ENOERR;
  copyMode(err, src, dest);
  if err != ENOERR then ioerror(err, "in copyMode " + src, dest);
}

/* Returns the current working directory for the current locale.
   err: a syserr used to indicate if an error occurred

   Note: another task on this locale can change the current working
   directory from underneath this task, so use caution when making use
   of this function in a parallel environment.
*/
proc locale.cwd(out err: syserr): string {
  extern proc chpl_fs_cwd(ref working_dir:c_string_copy):syserr;

  var ret:string;
  on this {
    var tmp:c_string_copy;
    // c_strings and c_string_copy's can't cross on statements.
    err = chpl_fs_cwd(tmp);
    if (err != ENOERR) {
      ret = "";
    } else {
      // This version of toString steals its operand.  No need to free.
      ret = toString(tmp);
    }
  }
  return ret;
}

/* Returns the current working directory for the current locale. Generates an
   error message if one occurred.

   Note: another task on this locale can change the current working
   directory from underneath this task, so use caution when making use
   of this function in a parallel environment.
*/
proc locale.cwd(): string {
  var err: syserr = ENOERR;
  var ret = cwd(err);
  if err != ENOERR then ioerror(err, "in cwd");
  return ret;
}

/* Returns true if the provided filename corresponds to an existing file, false
   otherwise.  Returns false for broken symbolic links.  Returns any errors that
   occurred via an out parameter
   name: a string used to attempt to find the file specified.
*/
proc exists(out err: syserr, name: string): bool {
  extern proc chpl_fs_exists(ref result:c_int, name: c_string): syserr;

  var ret:c_int;
  err = chpl_fs_exists(ret, name.c_str());
  return ret != 0;
}

/* Returns true if the provided filename corresponds to an existing file, false
   otherwise.  Returns false for broken symbolic links.  Generates an error
   message if one occurred.
   name: a string used to attempt to find the file specified.
*/
proc exists(name: string): bool {
  var err: syserr = ENOERR;
  var ret = exists(err, name);
  if err != ENOERR then ioerror(err, "in exists");
  return ret;
}

/* Returns the group id associated with the file or directory specified by
   name.  Returns any errors that occurred via an out parameter.
   err: a syserr used to indicate if an error occurred
   name: a string used to indicate the file in question
*/
proc getGID(out err: syserr, name: string): int {
  extern proc chpl_fs_get_gid(ref result: c_int, filename: c_string): syserr;

  var result: c_int;
  err = chpl_fs_get_gid(result, name.c_str());
  return result;
}

/* Returns the group id associated with the file or directory specified by
   name.  May generate an error message.
   name: a string used to indicate the file in question
*/
proc getGID(name: string): int {
  var err: syserr = ENOERR;
  var ret = getGID(err, name);
  if err != ENOERR then ioerror(err, "in getGID");
  return ret;
}

/* Returns the user id associated with the file or directory specified by
   name.  Returns any errors that occurred via an out parameter.
   err: a syserr used to indicate if an error occurred
   name: a string used to indicate the file in question
*/
proc getUID(out err: syserr, name: string): int {
  extern proc chpl_fs_get_uid(ref result: c_int, filename: c_string): syserr;

  var result: c_int;
  err = chpl_fs_get_uid(result, name.c_str());
  return result;
}

/* Returns the user id associated with the file or directory specified by
   name.  May generate an error message.
   name: a string used to indicate the file in question
*/
proc getUID(name: string): int {
  var err: syserr = ENOERR;
  var ret = getUID(err, name);
  if err != ENOERR then ioerror(err, "in getUID");
  return ret;
}

/* Returns true if the name corresponds to a directory, false otherwise.
   err: a syserr used to indicate if an error occurred
   name: a string that could be the name of a directory.
*/
proc isDir(out err:syserr, name:string):bool {
  extern proc chpl_fs_is_dir(ref result:c_int, name: c_string):syserr;

  var ret:c_int;
  err = chpl_fs_is_dir(ret, name.c_str());
  return ret != 0;
}

/* Returns true if the name corresponds to a directory, false otherwise.
   Generates an error message if one occurs.
   name: a string that could be the name of a directory.
*/
proc isDir(name:string):bool {
  var err:syserr;
  var ret = isDir(err, name);
  if err != ENOERR then ioerror(err, "in isDir", name);
  return ret;
}

/* Returns true if the name corresponds to a file, false otherwise.
   err: a syserr used to indicate if an error occurred
   name: a string that could be the name of a file.
*/
proc isFile(out err:syserr, name:string):bool {
  extern proc chpl_fs_is_file(ref result:c_int, name: c_string):syserr;

  var ret:c_int;
  err = chpl_fs_is_file(ret, name.c_str());
  return ret != 0;
}

/* Returns true if the name corresponds to a file, false otherwise.
   Generates an error message if one occurs.
   name: a string that could be the name of a file.
*/
proc isFile(name:string):bool {
  var err:syserr;
  var ret = isFile(err, name);
  if err != ENOERR then ioerror(err, "in isFile", name);
  return ret;
}

/* Returns true if the name corresponds to a symbolic link, false if not
   or if symbolic links are not supported.
   name: a string that could be the name of a symbolic link.
*/
proc isLink(out err:syserr, name: string): bool {
  extern proc chpl_fs_is_link(ref result:c_int, name: c_string): syserr;

  var ret:c_int;
  err = chpl_fs_is_link(ret, name.c_str());
  return ret != 0;
}

/* Returns true if the name corresponds to a symbolic link, false if not
   or if symbolic links are not supported.  Generates an error message if one
   occurs
   name: a string that could be the name of a symbolic link.
*/
proc isLink(name: string): bool {
  var err:syserr;
  var ret = isLink(err, name);
  if err != ENOERR then ioerror(err, "in isLink", name);
  return ret;
}

/* These are constant values of the form S_I[R | W | X][USR | GRP | OTH],
   S_IRWX[U | G | O], S_ISUID, S_ISGID, or S_ISVTX, where R corresponds to
   readable, W corresponds to writable, X corresponds to executable, USR and
   U correspond to user, GRP and G correspond to group, OTH and O correspond
   to other, directly tied to the C idea of these constants.  They are intended
   for use with functions that alter the permissions of files or directories.
*/
extern const S_IRUSR: int;
extern const S_IWUSR: int;
extern const S_IXUSR: int;
extern const S_IRWXU: int;

extern const S_IRGRP: int;
extern const S_IWGRP: int;
extern const S_IXGRP: int;
extern const S_IRWXG: int;

extern const S_IROTH: int;
extern const S_IWOTH: int;
extern const S_IXOTH: int;
extern const S_IRWXO: int;

extern const S_ISUID: int;
extern const S_ISGID: int;
extern const S_ISVTX: int;

/* Attempt to create a directory with the given path.  If parents is true,
   will attempt to create any directory in the path that did not previously
   exist.  Returns any errors that occurred via an out parameter

   :arg err: a syserr used to indicate if an error occurred
   :arg name: the name of the directory to be created, fully specified.
   :arg mode: an integer representing the permissions desired for the file in
              question. See description of the provided constants for
              potential values.
   :arg parents: a boolean indicating if parent directories should be created.
                 If set to false, any nonexistent parent will cause an error to
                 occur.

   Important note: In the case where parents is true, there is a potential
   security vulnerability.  Checking whether parent directories exist and
   creating them if not are separate events. So even if parents==true and a
   parent directory didn't exist before this function is called but does exist
   afterward, it's still not necessarily true that this function created that
   parent. Some other concurrent operation could have done so.
*/
proc mkdir(out err: syserr, name: string, mode: int = 0o777,
           parents: bool=false) {
  extern proc chpl_fs_mkdir(name: c_string, mode: int, parents: bool):syserr;

  err = chpl_fs_mkdir(name.c_str(), mode, parents);
}

/* Attempt to create a directory with the given path.  If parents is true,
   will attempt to create any directory in the path that did not previously
   exist.  Generates an error message if one occurred.

   :arg name: the name of the directory to be created, fully specified.
   :arg mode: an integer representing the permissions desired for the file in
              question. See description of the provided constants for
              potential values.
   :arg parents: a boolean indicating if parent directories should be created.
                 If set to false, any nonexistent parent will cause an error to
                 occur.

   Important note: In the case where parents is true, there is a potential
   security vulnerability.  Checking whether parent directories exist and
   creating them if not are separate events. So even if parents==true and a
   parent directory didn't exist before this function is called but does exist
   afterward, it's still not necessarily true that this function created that
   parent. Some other concurrent operation could have done so.
*/
proc mkdir(name: string, mode: int = 0o777, parents: bool=false) {
  var err: syserr = ENOERR;
  mkdir(err, name, mode, parents);
  if err != ENOERR then ioerror(err, "in mkdir", name);
}

/* Renames the file specified by oldname to newname, returning an error
   if one occurred.  The file is not opened during this operation.
   error: a syserr used to indicate if an error occurred during renaming.
   oldname: current name of the file
   newname: name which should refer to the file in the future.*/
proc rename(out error: syserr, oldname, newname: string) {
  extern proc chpl_fs_rename(oldname: c_string, newname: c_string):syserr;

  error = chpl_fs_rename(oldname.c_str(), newname.c_str());
}

/* Renames the file specified by oldname to newname, generating an error message
   if one occurred.  The file is not opened during this operation.
   oldname: current name of the file
   newname: name which should refer to the file in the future.*/
proc rename(oldname, newname: string) {
  var err:syserr = ENOERR;
  rename(err, oldname, newname);
  if err != ENOERR then ioerror(err, "in rename", oldname);
}

/* Removes the file or directory specified by name, returning an error
   if one occurred via an out parameter.
   err: a syserr used to indicate if an error occurred during removal
   name: the name of the file/directory to remove */
proc remove(out err: syserr, name: string) {
  extern proc chpl_fs_remove(name: c_string):syserr;

  err = chpl_fs_remove(name.c_str());
}

/* Removes the file or directory specified by name, generating an error message
   if one occurred.
   name: the name of the file/directory to remove */
proc remove(name: string) {
  var err:syserr = ENOERR;
  remove(err, name);
  if err != ENOERR then ioerror(err, "in remove", name);
}

/* Returns true if both pathnames refer to the same file or directory
   (utilizing operating system operations rather than string ones), returns
   false otherwise.
   err: a syserr used to indicate if an error occurred during comparison
   file1, file2: string representations of paths to be compared.
*/
proc sameFile(out err: syserr, file1: string, file2: string): bool {
  extern proc chpl_fs_samefile_string(ref ret: c_int, file1: c_string, file2: c_string): syserr;

  var ret:c_int;
  err = chpl_fs_samefile_string(ret, file1.c_str(), file2.c_str());
  return ret != 0;
}

/* Returns true if both pathnames refer to the same file or directory
   (utilizing operating system operations rather than string ones), returns
   false otherwise.  May generate an error message.
   file1, file2: string representations of paths to be compared.
*/
proc sameFile(file1: string, file2: string): bool {
  var err:syserr = ENOERR;
  var result = sameFile(err, file1, file2);
  if err != ENOERR then ioerror(err, "in sameFile " + file1, file2);
  return result;
}

/* Same as the above function, but taking file records instead of string
   pathnames as arguments.
*/
proc sameFile(out err: syserr, file1: file, file2: file): bool {
  extern proc chpl_fs_samefile(ref ret: c_int, file1: qio_file_ptr_t,
                               file2: qio_file_ptr_t): syserr;

  var ret:c_int;
  if (is_c_nil(file1._file_internal) || is_c_nil(file2._file_internal)) {
    // Implementation note on program design tradeoffs:
    // I could use file.check() here.  That would not rely on the understanding
    // of file's internals.  However, it would cause this method to either
    // return error messages or halt, depending on the error encountered.
    // This check could be moved to the version w/o the err argument, but if
    // someone called this function w/o going through that, we'd lose the
    // check.  Also, we already must make use of the record internals to do the
    // inner comparison (since the record is a chapel construct), so there's no
    // additional harm.

    // The file is referencing a null file.  We'll get a segfault if we
    // continue.
    err = EBADF;
    return false; // This part isn't as important as the error.
  }


  err = chpl_fs_samefile(ret, file1._file_internal, file2._file_internal);
  return ret != 0;
}

/* Same as the above function, but taking file records instead of string
   pathnames as arguments.  May generate an error message.
*/
proc sameFile(file1: file, file2: file): bool {
  var err:syserr = ENOERR;
  var result = sameFile(err, file1, file2);
  if err != ENOERR then ioerror(err, "in sameFile " + file1.path, file2.path);
  return result;
}

/* Create a symbolic link pointing to oldName named newName.  May generate an
   error message.
   err: a syserr used to indicate if an error occurred during creation
   oldName: the source file to be linked
   newName: the location the symbolic link should live
*/
proc symlink(out err: syserr, oldName: string, newName: string) {
  extern proc chpl_fs_symlink(orig: c_string, linkName: c_string): syserr;

  err = chpl_fs_symlink(oldName.c_str(), newName.c_str());
}

/* Create a symbolic link pointing to oldName named sym.  May generate an error
   message.
   oldName: the source file to be linked
   newName: the location the symbolic link should live
*/
proc symlink(oldName: string, newName: string) {
  var err:syserr = ENOERR;
  symlink(err, oldName, newName);
  if err != ENOERR then ioerror(err, "in symlink " + oldName, newName);
}

/* Returns an integer representing the current permissions of the file specified
   by name.  May generate an error message.
   err: a syserr used to indicate if an error occurred during this function
   name: the name of the file that you want to know the permissions of.
*/
proc viewMode(out err: syserr, name: string): int {
  extern proc chpl_fs_viewmode(ref result:c_int, name: c_string): syserr;

  var ret:c_int;
  err = chpl_fs_viewmode(ret, name.c_str());
  return ret;
}

/* Returns an integer representing the current permissions of the file specified
   by name.  May generate an error message.
   name: the name of the file that you want to know the permissions of.
*/
proc viewMode(name: string): int {
  var err:syserr = ENOERR;
  var result = viewMode(err, name);
  if err != ENOERR then ioerror(err, "in viewMode", name);
  return result;
}
