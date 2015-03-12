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

/* A file utilities library

   The FileSystem module focuses on file and directory properties and
   operations.  It does not cover every interaction involving a file -
   for instance, path specific operations live in the :mod:`Path` module, while
   opening, writing to, or reading from a file appropriately live in the
   :mod:`IO` module.  Rather, it covers cases where the user would prefer a file
   or directory handled wholesale and/or with minimal interaction.  The
   :proc:`copy` functionality belongs here, as does :proc:`chmod`,
   :proc:`mkdir`, and :proc:`remove`, for instance.  Also included are
   operations relating to the current process's state, such as :proc:`umask` or
   :proc:`~locale.chdir` (an operation which is performed on a specified
   locale).

 */
module FileSystem {

use Error;


pragma "no doc"
proc locale.chdir(out error: syserr, name: string) {
  extern proc chpl_fs_chdir(name: c_string):syserr;

  on this {
    error = chpl_fs_chdir(name.c_str());
  }
}

/* Change the current working directory of the locale in question to the
   specified name.

   Will halt with an error message if one is detected.

   .. warning::

      This is not safe within a parallel context.  A chdir call in one task
      will affect the current working directory of all tasks for that locale.

   :arg name: The intended current working directory
   :type name: string
*/
proc locale.chdir(name: string) {
  var err: syserr = ENOERR;
  chdir(err, name);
  if err != ENOERR then ioerror(err, "in chdir", name);
}

pragma "no doc"
proc chmod(out error: syserr, name: string, mode: int) {
  extern proc chpl_fs_chmod(name: c_string, mode: int): syserr;

  error = chpl_fs_chmod(name.c_str(), mode);
}

// CHPLDOC TODO: really want to make a section for S_IRUSR and friends.

/* Set the permissions of the file or directory specified by the argument
   ``name`` to that indicated by the argument ``mode``.

   Will halt with an error message if one is detected

   :arg name: The name of the file or directory whose permissions should be
              alterred.
   :type name: string
   :arg mode: The permissions desired for the file or direcory in question.
              See description of :const:`S_IRUSR`, for instance, for potential
              values.
   :type mode: int
*/
proc chmod(name: string, mode: int) {
  var err: syserr = ENOERR;
  chmod(err, name, mode);
  if err != ENOERR then ioerror(err, "in chmod", name);
}


pragma "no doc"
proc chown(out error: syserr, name: string, uid: int, gid: int) {
  extern proc chpl_fs_chown(name: c_string, uid: c_int, gid: c_int):syserr;

  error = chpl_fs_chown(name.c_str(), uid:c_int, gid:c_int);
}

/* Change one or both of the owner and group id of the named file or directory
   to the specified values.  If ``uid`` or ``gid`` are -1, the value in question
   will remain unchanged.

   Halts with an error message if one occurred.

   :arg name: The name of the file to be changed.
   :type name: string
   :arg uid: The intended new owner(user) id, or -1 if it should remain the
             same.
   :type uid: int
   :arg gid: The intended new group owner(id), or -1 if it should remain the
             same.
   :type gid: int
*/
proc chown(name: string, uid: int, gid: int) {
  var err: syserr = ENOERR;
  chown(err, name, uid, gid);
  if err != ENOERR then ioerror(err, "in chown", name);
}

// FUTURE WORK:
// When basename and joinPath are supported, enable dest to be a directory.
pragma "no doc"
proc copy(out error: syserr, src: string, dest: string, metadata: bool = false) {
  var destFile = dest;
  if (isDir(error, destFile)) {
    // destFile = joinPath(destFile, basename(src));
    error = EISDIR;
    // Supporting a destination directory requires getting the basename from
    // the src (because we're using the same name) and joining it with the
    // provided destination directory.  Both of those operations are part of
    // the string portion, so we aren't supporting it just yet.
    return;
  } else {
    if (error == ENOENT) {
      // Destination didn't exist before.  We'd be overwriting it anyways, so
      // we don't care.
      error = ENOERR;
    }
    if error != ENOERR then return;
  }
  copyFile(error, src, destFile);
  if error != ENOERR then return;
  copyMode(error, src, destFile);
  if error != ENOERR then return;

  if (metadata) {
    extern proc chpl_fs_copy_metadata(source: c_string, dest: c_string): syserr;

    // Copies the access time, and time of last modification.
    // Does not copy uid, gid, or mode
    error = chpl_fs_copy_metadata(src.c_str(), dest.c_str());

    // Get uid and gid from src
    var uid = getUID(error, src);
    if error != ENOERR then return;
    var gid = getGID(error, src);
    if error != ENOERR then return;
    // Change uid and gid to that of the src
    chown(error, destFile, uid, gid);
  }
}

/* Copies the contents and permissions of the file indicated by ``src`` into
   the file or directory ``dest``.  If ``dest`` is a directory, will halt with
   an error message.  If ``metadata`` is set to ``true``, will also copy the
   metadata (uid, gid, time of last access and time of modification) of the
   file to be copied.

   May halt with other error messages.

   :arg src: The source file whose contents and permissions are to be copied
   :type src: string
   :arg dest: The name of the destination file for the contents and permissions.
              May or may not exist previously, but will be overwritten if it did
              exist
   :type dest: string
   :arg metadata: This argument indicates whether to copy metadata associated
                  with the source file.  It is set to ``false`` by default.
   :type metadata: bool
*/
proc copy(src: string, dest: string, metadata: bool = false) {
  var err: syserr = ENOERR;
  copy(err, src, dest, metadata);
  if err != ENOERR then ioerror(err, "in copy(" + src + ", " + dest + ")");
}

pragma "no doc"
proc copyFile(out error: syserr, src: string, dest: string) {
  // This implementation is based off of the python implementation for copyfile,
  // with some slight differences.  That implementation was found at:
  // https://bitbucket.org/mirror/cpython/src/c8ce5bca0fcda4307f7ac5d69103ce128a562705/Lib/shutil.py?at=default
  // I did not look at the other functions in that file, except for copyfileobj
  // (which copyfile called).
  if (!exists(src)) {
    error = ENOENT;
    // Source didn't exist, we can't copy it.
    return;
  }
  if (isDir(error, src) || isDir(error, dest)) {
    // If the source is a directory, the user has made a mistake, so return an
    // error.  The same is true if the destination is a directory.
    error = EISDIR;
    return;
  }

  if (error == ENOENT) {
    error = ENOERR;
    // We don't care if dest did not exist before, we'll create or overwrite it
    // anyways.  We already know src exists.
  } else if (sameFile(error, src, dest)) {
    // Check if the files are the same, error if yes

    // Don't need to check if they're the same file when we know dest didn't
    // exist.
    error = EINVAL;
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
    while (srcChnl.readline(line, numRead=numRead, error=error)) {
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
    if error == EEOF then error = ENOERR;
    destChnl.flush();

    srcFile.close();
    destFile.close();
  }
}

/* Copies the contents of the file indicated by ``src`` into the file indicated
   by ``dest``, replacing ``dest``'s contents if it already exists (and is a
   different file than ``src``, i.e. not a symbolic link to ``src``).

   If ``dest`` is not writable, or ``src`` and ``dest`` refer to the same file,
   this function will halt with an error message.  Does not copy metadata.  May
   halt with other error messages.

   :arg src: The source file whose contents are to be copied.
   :type src: string
   :arg dest: The intended destination of the contents.
   :type dest: string
*/
proc copyFile(src: string, dest: string) {
  var err: syserr = ENOERR;
  copyFile(err, src, dest);
  if err != ENOERR then ioerror(err, "in copyFile(" + src + ", " + dest + ")");
}

pragma "no doc"
proc copyMode(out error: syserr, src: string, dest: string) {
  // Gets the mode from the source file.
  var srcMode = getMode(error, src);
  // If any error occurred, we want to be the one reporting it, so as not
  // to bleed implementation details.  If we found one when viewing the
  // source's mode, we should return immediately.
  if error != ENOERR then return;
  // Sets the mode of the destination to the source's mode.
  chmod(error, dest, srcMode);
}

/* Copies the permissions of the file indicated by ``src`` to the file indicated
   by ``dest``, leaving contents, owner and group unaffected.

   Will halt with an error message if one is detected.

   :arg src: The source file whose permissions are to be copied.
   :type src: string
   :arg dest: The intended destination of the permissions.
   :type dest: string
*/
proc copyMode(src: string, dest: string) {
  var err: syserr = ENOERR;
  copyMode(err, src, dest);
  if err != ENOERR then ioerror(err, "in copyMode " + src, dest);
}

use Filerator;
use Path;

pragma "no doc"
proc copyTreeHelper(out error: syserr, src: string, dest: string, copySymbolically: bool=false) {
  var oldMode = getMode(src);
  mkdir(error, dest, mode=oldMode, parents=true);
  if error != ENOERR then return;
  // Create dest

  for filename in listdir(path=src, dirs=false, files=true, listlinks=copySymbolically) {
    // Take care of files in src
    var fileDestName = dest + "/" + filename;
    copy(error, src + "/" + filename, fileDestName, metadata=true);
    if (error != ENOERR) then return;
  }

  for dirname in listdir(path=src, dirs=true, files=false, listlinks=copySymbolically) {
    copyTreeHelper(error, src+"/"+dirname, dest+"/"+dirname, copySymbolically);
  }
}

pragma "no doc"
proc copyTree(out error: syserr, src: string, dest: string, copySymbolically: bool=false) {
  var expectedErrorCases = exists(error, dest);
  if (error != ENOERR) then return; // Some error occurred in checking the existence of dest.
  else if (expectedErrorCases) {
    // dest exists.  That's not ideal.
    error = EEXIST;
    return;
  }
  expectedErrorCases = !isDir(error, src);
  if (error != ENOERR) then return; // Some error occurred in checking src was a directory.
  else if (expectedErrorCases) {
    error = ENOTDIR;
    return;
  }

  var srcPath = realPath(src);

  copyTreeHelper(error, srcPath, dest, copySymbolically);
}

/* Will recursively copy the tree which lives under ``src`` into ``dst``,
   including all contents, permissions, and metadata.  ``dst`` must not
   previously exist, this function assumes it can create it and any missing
   parent directories. If ``copySymbolically`` is ``true``, symlinks will be
   copied as symlinks, otherwise their contents and metadata will be copied
   instead.

   Will halt with an error message if one is detected.

   :arg src: The root of the source tree to be copied.
   :type src: string
   :arg dest: The root of the destination directory under which the contents of
              ``src`` are to be copied (must not exist prior to this function
              call).
   :type dest: string
   :arg copySymbolically: This argument is used to indicate how to handle
                          symlinks in the source directory.  It is set to
                          ``false`` by default
   :type copySymbolically: bool
*/
proc copyTree(src: string, dest: string, copySymbolically: bool=false) {
  var err: syserr = ENOERR;
  copyTree(err, src, dest, copySymbolically);
  if err != ENOERR then ioerror(err, "in copyTree " + src, dest);
}

pragma "no doc"
proc locale.cwd(out error: syserr): string {
  extern proc chpl_fs_cwd(ref working_dir:c_string_copy):syserr;

  var ret:string;
  on this {
    var tmp:c_string_copy;
    // c_strings and c_string_copy's can't cross on statements.
    error = chpl_fs_cwd(tmp);
    if (error != ENOERR) {
      ret = "";
    } else {
      // This version of toString steals its operand.  No need to free.
      ret = toString(tmp);
    }
  }
  return ret;
}

/* Obtains and returns the current working directory for this locale.

   Will halt with an error message if one was detected.

   .. warning::

      Another task on this locale can change the current working directory from
      underneath this task, so use caution when making use of this function in
      a parallel environment.

   :return: The current working directory for the locale in question.
   :rtype: string
*/
proc locale.cwd(): string {
  var err: syserr = ENOERR;
  var ret = cwd(err);
  if err != ENOERR then ioerror(err, "in cwd");
  return ret;
}

pragma "no doc"
proc exists(out error: syserr, name: string): bool {
  extern proc chpl_fs_exists(ref result:c_int, name: c_string): syserr;

  var ret:c_int;
  error = chpl_fs_exists(ret, name.c_str());
  return ret != 0;
}

/* Determines if the file or directory indicated by ``name`` exists and returns
   the result of this check.

   Will halt with an error message if one was detected.

   :arg name: The file or directory whose existence is in question.
   :type name: string

   :return: ``true`` if the provided argument corresponds to an existing file or
            directory, ``false`` otherwise.  Also returns ``false`` for broken
            symbolic links.
   :rtype: bool
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
pragma "no doc"
proc getGID(out error: syserr, name: string): int {
  extern proc chpl_fs_get_gid(ref result: c_int, filename: c_string): syserr;

  var result: c_int;
  error = chpl_fs_get_gid(result, name.c_str());
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

/* Returns an integer representing the current permissions of the file specified
   by name.  May generate an error message.

   err: a syserr used to indicate if an error occurred during this function
   name: the name of the file that you want to know the permissions of.
*/
pragma "no doc"
proc getMode(out error: syserr, name: string): int {
  extern proc chpl_fs_viewmode(ref result:c_int, name: c_string): syserr;

  var ret:c_int;
  error = chpl_fs_viewmode(ret, name.c_str());
  return ret;
}

/* Returns an integer representing the current permissions of the file specified
   by name.  May generate an error message.

   name: the name of the file that you want to know the permissions of.
*/
proc getMode(name: string): int {
  var err:syserr = ENOERR;
  var result = getMode(err, name);
  if err != ENOERR then ioerror(err, "in getMode", name);
  return result;
}

/* Returns the user id associated with the file or directory specified by
   name.  Returns any errors that occurred via an out parameter.

   err: a syserr used to indicate if an error occurred
   name: a string used to indicate the file in question
*/
pragma "no doc"
proc getUID(out error: syserr, name: string): int {
  extern proc chpl_fs_get_uid(ref result: c_int, filename: c_string): syserr;

  var result: c_int;
  error = chpl_fs_get_uid(result, name.c_str());
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
pragma "no doc"
proc isDir(out error:syserr, name:string):bool {
  extern proc chpl_fs_is_dir(ref result:c_int, name: c_string):syserr;

  var ret:c_int;
  error = chpl_fs_is_dir(ret, name.c_str());
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
pragma "no doc"
proc isFile(out error:syserr, name:string):bool {
  extern proc chpl_fs_is_file(ref result:c_int, name: c_string):syserr;

  var ret:c_int;
  error = chpl_fs_is_file(ret, name.c_str());
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
pragma "no doc"
proc isLink(out error:syserr, name: string): bool {
  extern proc chpl_fs_is_link(ref result:c_int, name: c_string): syserr;

  var ret:c_int;
  error = chpl_fs_is_link(ret, name.c_str());
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

/* Returns true if the name corresponds to a mount point, false otherwise.

   err: a syserr used to indicate if an error occurred
   name: a string that could be the name of a mount point.
*/
pragma "no doc"
proc isMount(out error:syserr, name: string): bool {
  extern proc chpl_fs_is_mount(ref result:c_int, name: c_string): syserr;

  if (isFile(name)) {
    // Files aren't mount points.  That would be silly.
    error = ENOERR;
    return false;
  }
  var ret:c_int;
  error = chpl_fs_is_mount(ret, name.c_str());
  return ret != 0;
}

/* Returns true if the name corresponds to a mount point, false otherwise.

   name: a string that could be the name of a mount point.
*/
proc isMount(name: string): bool {
  var err:syserr;
  var ret = isMount(err, name);
  if err != ENOERR then ioerror(err, "in isMount", name);
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

   Important note: In the case where parents is true, there is a potential
   security vulnerability.  Checking whether parent directories exist and
   creating them if not are separate events. So even if parents==true and a
   parent directory didn't exist before this function is called but does exist
   afterward, it's still not necessarily true that this function created that
   parent. Some other concurrent operation could have done so.

   :arg err: a syserr used to indicate if an error occurred
   :arg name: the name of the directory to be created, fully specified.
   :arg mode: an integer representing the permissions desired for the file in
   question. See description of the provided constants for
   potential values.
   :arg parents: a boolean indicating if parent directories should be created.
   If set to false, any nonexistent parent will cause an error to
   occur.
*/
pragma "no doc"
proc mkdir(out error: syserr, name: string, mode: int = 0o777,
           parents: bool=false) {
  extern proc chpl_fs_mkdir(name: c_string, mode: int, parents: bool):syserr;

  error = chpl_fs_mkdir(name.c_str(), mode, parents);
}

/* Attempt to create a directory with the given path.  If parents is true,
   will attempt to create any directory in the path that did not previously
   exist.  Generates an error message if one occurred.

   Important note: In the case where parents is true, there is a potential
   security vulnerability.  Checking whether parent directories exist and
   creating them if not are separate events. So even if parents==true and a
   parent directory didn't exist before this function is called but does exist
   afterward, it's still not necessarily true that this function created that
   parent. Some other concurrent operation could have done so.

   :arg name: the name of the directory to be created, fully specified.
   :arg mode: an integer representing the permissions desired for the file in question. See description of the provided constants for potential values.
   :arg parents: a boolean indicating if parent directories should be created.  If set to false, any nonexistent parent will cause an error to occur.
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
pragma "no doc"
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
pragma "no doc"
proc remove(out error: syserr, name: string) {
  extern proc chpl_fs_remove(name: c_string):syserr;

  error = chpl_fs_remove(name.c_str());
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

   error: a syserr used to indicate if an error occurred during comparison
   file1, file2: string representations of paths to be compared.
*/
pragma "no doc"
proc sameFile(out error: syserr, file1: string, file2: string): bool {
  extern proc chpl_fs_samefile_string(ref ret: c_int, file1: c_string, file2: c_string): syserr;

  var ret:c_int;
  error = chpl_fs_samefile_string(ret, file1.c_str(), file2.c_str());
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
  if err != ENOERR then ioerror(err, "in sameFile(" + file1 + ", " + file2 + ")");
  return result;
}

/* Same as the above function, but taking file records instead of string
   pathnames as arguments.
*/
pragma "no doc"
proc sameFile(out error: syserr, file1: file, file2: file): bool {
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
    error = EBADF;
    return false; // This part isn't as important as the error.
  }


  error = chpl_fs_samefile(ret, file1._file_internal, file2._file_internal);
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
pragma "no doc"
proc symlink(out error: syserr, oldName: string, newName: string) {
  extern proc chpl_fs_symlink(orig: c_string, linkName: c_string): syserr;

  error = chpl_fs_symlink(oldName.c_str(), newName.c_str());
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

/* Sets the file creation mask of the current process to mask, and returns
   the previous value of the file creation mask.

   mask: the file creation mask to use now.
*/
proc umask(mask: int): int {
  extern proc chpl_fs_umask(mask: mode_t): mode_t;

  return chpl_fs_umask(mask.safeCast(mode_t));
}

}
