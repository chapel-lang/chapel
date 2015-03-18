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
   operations.  It does not cover every interaction involving a file--- for
   instance, path-specific operations live in the :mod:`Path` module, while
   routines for opening, writing to, or reading from a file live in the
   :mod:`IO` module.  Rather, it covers cases where the user would prefer a file
   or directory to be handled wholesale and/or with minimal interaction.  For
   example, this module contains a file :proc:`copy` routine, as well as
   operations like :proc:`chmod`, :proc:`mkdir`, and :proc:`remove`.  Also
   included are operations relating to the current process's file system state,
   such as :proc:`~locale.umask` or :proc:`~locale.chdir`, which are performed
   on a specified locale.  The module also contains iterators for traversing the
   file system, such as :iter:`glob`, :iter:`listdir`, :iter:`walkdirs`, and
   :iter:`findfiles`.

 */
module FileSystem {

use Error, Path;

/* S_IRUSR and the following constants are values of the form
   S_I[R | W | X][USR | GRP | OTH], S_IRWX[U | G | O], S_ISUID, S_ISGID, or
   S_ISVTX, where R corresponds to readable, W corresponds to writable, X
   corresponds to executable, USR and U correspond to user, GRP and G
   correspond to group, OTH and O correspond to other, directly tied to the C
   idea of these constants as visible at `GNU Permissions
   <http://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html>`_.
   They are intended for use when dealing with the permissions of files or
   directories, such as with :proc:`chmod`, :proc:`getMode`, :proc:`mkdir`,
   or :proc:`~locale.umask`

   S_IRUSR refers to the user's read permission
*/
extern const S_IRUSR: int;
/* Refers to the user's write permission, see :data:`S_IRUSR` */
extern const S_IWUSR: int;
/* Refers to the user's executable permission, see :data:`S_IRUSR` */
extern const S_IXUSR: int;
/* Refers to all the user's permissions - read, write, and execute.  See
   :data:`S_IRUSR`
*/
extern const S_IRWXU: int;

/* Refers to the group's read permission, see :data:`S_IRUSR` */
extern const S_IRGRP: int;
/* Refers to the group's write permission, see :data:`S_IRUSR` */
extern const S_IWGRP: int;
/* Refers to the group's executable permission, see :data:`S_IRUSR` */
extern const S_IXGRP: int;
/* Refers to all the permissions - read, write, and execute - of those in this
   group, see :data:`S_IRUSR`
*/
extern const S_IRWXG: int;

/* Refers to everyone else's read permission, see :data:`S_IRUSR` */
extern const S_IROTH: int;
/* Refers to everyone else's write permission, see :data:`S_IRUSR` */
extern const S_IWOTH: int;
/* Refers to everyone else's executable permission, see :data:`S_IRUSR` */
extern const S_IXOTH: int;
/* Refers to all the permissions - read, write, and execute - of everyone else,
   see :data:`S_IRUSR`
*/
extern const S_IRWXO: int;

/* Use of this would cause the user id to be set on execution.  See
   `GNU Permissions
   <http://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html>`_
   for what that generally means, and :data:`S_IRUSR` for intended uses, etc.
*/
extern const S_ISUID: int;
/* Use of this would cause the group id to be set on execution.  See
   `GNU Permissions
   <http://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html>`_
   for what that generally means, and :data:`S_IRUSR` for intended uses, etc.
*/
extern const S_ISGID: int;
/* This is the sticky bit. See `GNU Permissions
   <http://www.gnu.org/software/libc/manual/html_node/Permission-Bits.html>`_
   for what that generally means, and :data:`S_IRUSR` for intended uses, etc.
*/
extern const S_ISVTX: int;

pragma "no doc"
proc locale.chdir(out error: syserr, name: string) {
  extern proc chpl_fs_chdir(name: c_string):syserr;

  on this {
    error = chpl_fs_chdir(name.c_str());
  }
}

/* Change the current working directory of the locale in question to the
   specified path `name`.

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
   `name` to that indicated by the argument `mode`.

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
   to the specified values.  If `uid` or `gid` are -1, the value in question
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

/* Copies the contents and permissions of the file indicated by `src` into
   the file or directory `dest`.  If `dest` is a directory, will halt with
   an error message.  If `metadata` is set to `true`, will also copy the
   metadata (uid, gid, time of last access and time of modification) of the
   file to be copied.

   May halt with other error messages.

   .. note::

      In the future, when the :mod:`Path` module has been expanded further,
      this function will be able to support directories for the dest argument.

   :arg src: The source file whose contents and permissions are to be copied
   :type src: string
   :arg dest: The name of the destination file for the contents and permissions.
              May or may not exist previously, but will be overwritten if it did
              exist
   :type dest: string
   :arg metadata: This argument indicates whether to copy metadata associated
                  with the source file.  It is set to `false` by default.
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

/* Copies the contents of the file indicated by `src` into the file indicated
   by `dest`, replacing `dest`'s contents if it already exists (and is a
   different file than `src`, i.e. not a symbolic link to `src`).

   If `dest` is not writable, or `src` and `dest` refer to the same file,
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

/* Copies the permissions of the file indicated by `src` to the file indicated
   by `dest`, leaving contents, owner and group unaffected.

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

/* Will recursively copy the tree which lives under `src` into `dst`,
   including all contents, permissions, and metadata.  `dst` must not
   previously exist, this function assumes it can create it and any missing
   parent directories. If `copySymbolically` is `true`, symlinks will be
   copied as symlinks, otherwise their contents and metadata will be copied
   instead.

   Will halt with an error message if one is detected.

   :arg src: The root of the source tree to be copied.
   :type src: string
   :arg dest: The root of the destination directory under which the contents of
              `src` are to be copied (must not exist prior to this function
              call).
   :type dest: string
   :arg copySymbolically: This argument is used to indicate how to handle
                          symlinks in the source directory.  It is set to
                          `false` by default
   :type copySymbolically: bool
*/
proc copyTree(src: string, dest: string, copySymbolically: bool=false) {
  var err: syserr = ENOERR;
  copyTree(err, src, dest, copySymbolically);
  if err != ENOERR then ioerror(err, "in copyTree(" + src + ", " + dest + ")");
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
      underneath this task, so use caution when making use of this method in
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

/* Determines if the file or directory indicated by `name` exists and returns
   the result of this check.

   Will halt with an error message if one was detected.

   :arg name: The file or directory whose existence is in question.
   :type name: string

   :return: `true` if the provided argument corresponds to an existing file or
            directory, `false` otherwise.  Also returns `false` for broken
            symbolic links.
   :rtype: bool
*/
proc exists(name: string): bool {
  var err: syserr = ENOERR;
  var ret = exists(err, name);
  if err != ENOERR then ioerror(err, "in exists");
  return ret;
}


/* iter findfiles(startdir = ".", recursive=false, hidden=false)

   findfiles() is a simple find-like utility implemented using the
   above routines

     * startdir: where to start when looking for files
     * recursive: tells whether or not to descend recursively
     * hidden: tells whether or not to yield hidden
*/

iter findfiles(startdir = ".", recursive=false, hidden=false) {
  if (recursive) then
    for subdir in walkdirs(startdir, hidden=hidden) do
      for file in listdir(subdir, hidden=hidden, dirs=false, files=true, listlinks=true) do
        yield subdir+"/"+file;
  else
    for file in listdir(startdir, hidden=hidden, dirs=false, files=true, listlinks=false) do
      yield startdir+"/"+file;
}

pragma "no doc"
iter findfiles(startdir = ".", recursive=false, hidden=false, param tag: iterKind) where tag == iterKind.standalone {
  if (recursive) then
    forall subdir in walkdirs(startdir, hidden=hidden) do
      for file in listdir(subdir, hidden=hidden, dirs=false, files=true, listlinks=true) do
        yield subdir+"/"+file;
  else
    for file in listdir(startdir, hidden=hidden, dirs=false, files=true, listlinks=false) do
      yield startdir+"/"+file;
}


pragma "no doc"
proc getGID(out error: syserr, name: string): int {
  extern proc chpl_fs_get_gid(ref result: c_int, filename: c_string): syserr;

  var result: c_int;
  error = chpl_fs_get_gid(result, name.c_str());
  return result;
}

/* Obtains and returns the group id associated with the file or directory
   specified by `name`.

   Will halt with an error message if one is detected

   :arg name: The file or directory whose group id is desired
   :type name: string

   :return: The group id of the file or directory in question
   :rtype: int
*/
proc getGID(name: string): int {
  var err: syserr = ENOERR;
  var ret = getGID(err, name);
  if err != ENOERR then ioerror(err, "in getGID");
  return ret;
}

pragma "no doc"
proc getMode(out error: syserr, name: string): int {
  extern proc chpl_fs_viewmode(ref result:c_int, name: c_string): syserr;

  var ret:c_int;
  error = chpl_fs_viewmode(ret, name.c_str());
  return ret;
}

/* Obtains and returns the current permissions of the file or directory
   specified by `name`.

   Will halt with an error message if one is detected

   :arg name: The file or directory whose permissions are desired.
   :type name: string

   :return: The permissions of the specified file or directory
            See description of :const:`S_IRUSR`, for instance, for potential
            values.
   :rtype: int
*/
proc getMode(name: string): int {
  var err:syserr = ENOERR;
  var result = getMode(err, name);
  if err != ENOERR then ioerror(err, "in getMode", name);
  return result;
}

pragma "no doc"
proc getUID(out error: syserr, name: string): int {
  extern proc chpl_fs_get_uid(ref result: c_int, filename: c_string): syserr;

  var result: c_int;
  error = chpl_fs_get_uid(result, name.c_str());
  return result;
}

/* Obtains and returns the user id associated with the file or directory
   specified by `name`.

   Will halt with an error message if one is detected.

   :arg name: The file or directory whose user id is desired
   :type name: string

   :return: The user id of the specified file or directory
   :rtype: int
*/
proc getUID(name: string): int {
  var err: syserr = ENOERR;
  var ret = getUID(err, name);
  if err != ENOERR then ioerror(err, "in getUID");
  return ret;
}


//
// This is a helper module used by the various glob() overloads
// to access the C-level routines, types, and values
//
pragma "no doc"
module chpl_glob_c_interface {
  extern type glob_t;

  extern const GLOB_NOMATCH: c_int;

  extern proc chpl_glob(pattern:c_string, flags: c_int, 
                        ref ret_glob:glob_t):c_int;
  extern proc chpl_glob_num(x:glob_t): size_t;
  extern proc chpl_glob_index(x:glob_t, idx:size_t): c_string;
  extern proc globfree(ref glb:glob_t);
}


/* iter glob(pattern="*")

   glob() gives glob() capabilities and is implemented using C's glob()

     * pattern: the glob pattern to match against

   By default, it will list all files/directories in the current directory
*/
iter glob(pattern="*") {
  var glb : chpl_glob_c_interface.glob_t;

  const err = chpl_glob_c_interface.chpl_glob(pattern:c_string, 0, glb);
  // TODO: Handle error cases better
  if (err != 0 && err != chpl_glob_c_interface.GLOB_NOMATCH) then
    __primitive("chpl_error", "unhandled error in glob()");
  //
  // Use safeCast here, and then back again, in order to avoid conditional
  // in iterator in order to get better generated code, and to support
  // 'num-1' without risk of overflow
  //
  const num = chpl_glob_c_interface.chpl_glob_num(glb).safeCast(int);
  for i in 0..num-1 do
    yield chpl_glob_c_interface.chpl_glob_index(glb, i.safeCast(size_t)): string;

  chpl_glob_c_interface.globfree(glb);
}


pragma "no doc"
iter glob(pattern:string="*", param tag: iterKind) 
       where tag == iterKind.standalone {
  var glb : chpl_glob_c_interface.glob_t;

  const err = chpl_glob_c_interface.chpl_glob(pattern:c_string, 0, glb);
  // TODO: Handle error cases better
  if (err != 0 && err != chpl_glob_c_interface.GLOB_NOMATCH) then
    __primitive("chpl_error", "unhandled error in glob()");
  const num = chpl_glob_c_interface.chpl_glob_num(glb).safeCast(int);
  forall i in 0..num-1 do
    yield chpl_glob_c_interface.chpl_glob_index(glb, i.safeCast(size_t)): string;

  chpl_glob_c_interface.globfree(glb);
}

//
// TODO: The following leader/follower iterator assumes that
// the file system state won't change between calls because
// it calls the glob routines twice and assumes the same
// result will come back.  In leader-follower 2.0, we would
// like to store such state between calls in which case this
// should be rewritten to do so (and would require freeing
// the state at the end of the call).
//
pragma "no doc"
iter glob(pattern:string="*", param tag: iterKind) 
       where tag == iterKind.leader {
  var glb : chpl_glob_c_interface.glob_t;

  const err = chpl_glob_c_interface.chpl_glob(pattern:c_string, 0, glb);
  // TODO: Handle error cases better
  if (err != 0 && err != chpl_glob_c_interface.GLOB_NOMATCH) then
    __primitive("chpl_error", "unhandled error in glob()");
  //
  // cast is used here to ensure we create an int-based leader
  //
  const num = chpl_glob_c_interface.chpl_glob_num(glb).safeCast(int);
  chpl_glob_c_interface.globfree(glb);

  //
  // Forward to the range type's leader
  //
  for followThis in (0..num-1).these(tag) do
    yield followThis;
}

pragma "no doc"
iter glob(pattern:string="*", followThis, param tag: iterKind) 
       where tag == iterKind.follower {
  var glb : chpl_glob_c_interface.glob_t;
  if (followThis.size != 1) then
    compilerError("glob() iterator can only be zipped with 1D iterators");
  var r = followThis(1);

  const err = chpl_glob_c_interface.chpl_glob(pattern:c_string, 0, glb);
  // TODO: Handle error cases better
  if (err != 0 && err != chpl_glob_c_interface.GLOB_NOMATCH) then
    __primitive("chpl_error", "unhandled error in glob()");
  const num = chpl_glob_c_interface.chpl_glob_num(glb);
  if (r.high > num.safeCast(int)) then
    halt("glob() iterator zipped with something too big");
  for i in r do
    //
    // safe cast is used here to turn an int into a size_t
    //
    yield chpl_glob_c_interface.chpl_glob_index(glb, i.safeCast(size_t)): string;

  chpl_glob_c_interface.globfree(glb);
}


pragma "no doc"
proc isDir(out error:syserr, name:string):bool {
  extern proc chpl_fs_is_dir(ref result:c_int, name: c_string):syserr;

  var ret:c_int;
  error = chpl_fs_is_dir(ret, name.c_str());
  return ret != 0;
}

/* Determine if the provided path `name` corresponds to a directory and return
   the result

   Will halt with an error message if one is detected, including if the path
   does not refer to a valid file or directory

   :arg name: A path that could refer to a directory.
   :type name: string

   :return: `true` if the path is a directory, `false` if it is not
   :rtype: bool
*/
proc isDir(name:string):bool {
  var err:syserr;
  var ret = isDir(err, name);
  if err != ENOERR then ioerror(err, "in isDir", name);
  return ret;
}

pragma "no doc"
proc isFile(out error:syserr, name:string):bool {
  extern proc chpl_fs_is_file(ref result:c_int, name: c_string):syserr;

  var ret:c_int;
  error = chpl_fs_is_file(ret, name.c_str());
  return ret != 0;
}

/* Determine if the provided path `name` corresponds to a file and return
   the result

   Will halt with an error message if one is detected, including if the path
   does not refer to a valid file or directory

   :arg name: A path that could refer to a file.
   :type name: string

   :return: `true` if the path is a file, `false` if it is not
   :rtype: bool
*/
proc isFile(name:string):bool {
  var err:syserr;
  var ret = isFile(err, name);
  if err != ENOERR then ioerror(err, "in isFile", name);
  return ret;
}

pragma "no doc"
proc isLink(out error:syserr, name: string): bool {
  extern proc chpl_fs_is_link(ref result:c_int, name: c_string): syserr;

  var ret:c_int;
  error = chpl_fs_is_link(ret, name.c_str());
  return ret != 0;
}

/* Determine if the provided path `name` corresponds to a link and return the
   result.  If symbolic links are not supported, will return `false`.

   Will halt with an error message if one is detected, including if the path
   does not refer to a valid file or directory

   :arg name: A path that could refer to a symbolic link.
   :type name: string

   :return: `true` if the path is a symbolic link, `false` if it is not or
            if symbolic links are not supported.
   :rtype: bool
*/
proc isLink(name: string): bool {
  var err:syserr;
  var ret = isLink(err, name);
  if err != ENOERR then ioerror(err, "in isLink", name);
  return ret;
}

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

/* Determine if the provided path `name` corresponds to a mount point and
   return the result.

   Will halt with an error message if one is detected, including if the path
   does not refer to a valid file or directory.

   :arg name: A path that could refer to a mount point.
   :type name: string

   :return: `true` if the path is a mount point, `false` if it is not.
   :rtype: bool
*/
proc isMount(name: string): bool {
  var err:syserr;
  var ret = isMount(err, name);
  if err != ENOERR then ioerror(err, "in isMount", name);
  return ret;
}


/* iter listdir(path: string, hidden=false, dirs=true, files=true, 
                listlinks=true): string
  
    listdir() lists the contents of a directory, similar to 'ls'
      * path: the directory whose contents should be listed
      * hidden: should hidden files/directories be listed?
      * dirs: should dirs be listed?
      * files: should files be listed?
      * listlinks: should symbolic links be listed?
  
   By default this routine lists all files and directories in the
   current directory, including symbolic links, as long as they don't
   start with '.'
*/

iter listdir(path: string, hidden=false, dirs=true, files=true, 
             listlinks=true): string {
  extern type DIRptr;
  extern type direntptr;
  extern proc opendir(name: c_string): DIRptr;
  extern proc readdir(dirp: DIRptr): direntptr;
  extern proc closedir(dirp: DIRptr): c_int;

  proc direntptr.d_name(): c_string {
    extern proc chpl_rt_direntptr_getname(d: direntptr): c_string;

    return chpl_rt_direntptr_getname(this);
  }

  var dir: DIRptr;
  var ent: direntptr;
  dir = opendir(path:c_string);
  if (!is_c_nil(dir)) {
    ent = readdir(dir);
    while (!is_c_nil(ent)) {
      const filename = ent.d_name();
      if (hidden || filename.substring(1) != '.') {
        if (filename != "." && filename != "..") {
          const fullpath = path + "/" + filename;

          if (listlinks || !isLink(fullpath)) {
            if (dirs && isDir(fullpath)) then
              yield filename;
            else if (files && isFile(fullpath)) then
              yield filename;
          }
        }
      }
      ent = readdir(dir);
    }
    closedir(dir);
  } else {
    extern proc perror(s: c_string);
    perror("error in listdir(): ");
  }
}


pragma "no doc"
proc mkdir(out error: syserr, name: string, mode: int = 0o777,
           parents: bool=false) {
  extern proc chpl_fs_mkdir(name: c_string, mode: int, parents: bool):syserr;

  error = chpl_fs_mkdir(name.c_str(), mode, parents);
}

/* Attempt to create a directory with the given path, `name`.  If `parents`
   is `true`, will attempt to create any directory in the path that did not
   previously exist.

   Will halt with an error message if one is detected

   .. warning::

      In the case where `parents` is `true`, there is a potential security
      vulnerability.  Checking whether parent directories exist and creating
      them are separate events.  This is called a Time of Check, Time of Use
      vulnerability (TOCTOU), and in the case of files or directories that did
      not previously exist, there is no known guard against it.  So even if
      `parents == true` and a parent directory didn't exist before this
      function was called but does exist afterward, it's not necessarily true
      that this function created that parent. Some other concurrent operation
      could have done so, either intentionally or unintentionally, maliciously
      or ignorantly.  This option is provided as a convenience only, and any
      attempt to perform the same functionality will run a similar risk.

   :arg name: The name of the directory to be created, fully specified.
   :arg mode: The permissions desired for the directory to create.  Takes the
              current :proc:`~locale.umask` into account.  See description of
              :const:`S_IRUSR`, for instance, for potential values.
   :arg parents: Indicates whether parent directories should be created.  If
                 set to `false`, any nonexistent parent will cause an error
                 to occur.
*/
proc mkdir(name: string, mode: int = 0o777, parents: bool=false) {
  var err: syserr = ENOERR;
  mkdir(err, name, mode, parents);
  if err != ENOERR then ioerror(err, "in mkdir", name);
}

pragma "no doc"
proc rename(out error: syserr, oldname, newname: string) {
  extern proc chpl_fs_rename(oldname: c_string, newname: c_string):syserr;

  error = chpl_fs_rename(oldname.c_str(), newname.c_str());
}

/* Renames the file specified by `oldname` to `newname`.  The file is not
   opened during this operation.

   Will halt with an error message if one is detected

   :arg oldname: Current name of the file
   :type oldname: string
   :arg newname: Name which should be used to refer to the file in the future.
   :type newname: string
*/
 proc rename(oldname: string, newname: string) {
  var err:syserr = ENOERR;
  rename(err, oldname, newname);
  if err != ENOERR then ioerror(err, "in rename", oldname);
}

pragma "no doc"
proc remove(out error: syserr, name: string) {
  extern proc chpl_fs_remove(name: c_string):syserr;

  error = chpl_fs_remove(name.c_str());
}

/* Removes the file or directory specified by `name`

   Will halt with an error message if one is detected

   :arg name: The file/directory to remove
   :type name: string
*/
proc remove(name: string) {
  var err:syserr = ENOERR;
  remove(err, name);
  if err != ENOERR then ioerror(err, "in remove", name);
}

pragma "no doc"
proc sameFile(out error: syserr, file1: string, file2: string): bool {
  extern proc chpl_fs_samefile_string(ref ret: c_int, file1: c_string, file2: c_string): syserr;

  var ret:c_int;
  error = chpl_fs_samefile_string(ret, file1.c_str(), file2.c_str());
  return ret != 0;
}

/* Determines if both pathnames refer to the same file or directory (utilizing
   operating system operations rather than string ones, due to the possibility
   of symbolic links, :data:`~Path.curDir`, or :data:`~Path.parentDir` appearing
   in the path) and returns the result of that check

   Will halt with an error message if one is detected

   :arg file1: The first path to be compared.
   :type file1: string
   :arg file2: The second path to be compared.
   :type file2: string

   :return: `true` if the two paths refer to the same file or directory,
            `false` otherwise.
   :rtype: bool
*/
proc sameFile(file1: string, file2: string): bool {
  var err:syserr = ENOERR;
  var result = sameFile(err, file1, file2);
  if err != ENOERR then ioerror(err, "in sameFile(" + file1 + ", " + file2 + ")");
  return result;
}

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

/* Determines if both :type:`~IO.file` records refer to the same file
   (utilizing operating system operations rather than string ones, due to the
   possibility of symbolic links, :data:`~Path.curDir`, or
   :data:`~Path.parentDir` appearing in the path) and returns the result of that
   check

   Will halt with an error message if one is detected

   :arg file1: The first file to be compared.
   :type file1: file
   :arg file2: The second file to be compared.
   :type file2: file

   :return: `true` if the two records refer to the same file, `false`
            otherwise.
   :rtype: bool
*/
proc sameFile(file1: file, file2: file): bool {
  var err:syserr = ENOERR;
  var result = sameFile(err, file1, file2);
  if err != ENOERR then ioerror(err, "in sameFile " + file1.path, file2.path);
  return result;
}

pragma "no doc"
proc symlink(out error: syserr, oldName: string, newName: string) {
  extern proc chpl_fs_symlink(orig: c_string, linkName: c_string): syserr;

  error = chpl_fs_symlink(oldName.c_str(), newName.c_str());
}

/* Create a symbolic link pointing to `oldName` with the path `newName`.

   Will halt with an error message if one is detected

   :arg oldName: The source file to be linked
   :type oldName: string
   :arg newName: The location where the symbolic link should live
   :type newName: string
*/
proc symlink(oldName: string, newName: string) {
  var err:syserr = ENOERR;
  symlink(err, oldName, newName);
  if err != ENOERR then ioerror(err, "in symlink " + oldName, newName);
}

/* Sets the file creation mask of the current locale to `mask`, and returns
   the previous value of the file creation mask for that locale.  See
   description of :const:`S_IRUSR`, for instance, for potential values.

   .. warning::

      This is not safe within a parallel context.  A umask call in one task
      will affect the umask of all tasks for that locale.


   :arg mask: The file creation mask to use now.
   :type mask: int

   :return: The previous file creation mask
   :rtype: int
*/
proc locale.umask(mask: int): int {
  extern proc chpl_fs_umask(mask: mode_t): mode_t;

  var result: int;
  on this {
    var callRes = chpl_fs_umask(mask.safeCast(mode_t));
    result = callRes.safeCast(int);
  }
  return result;
}


/* iter walkdirs(path: string=".", topdown=true, depth=max(int), 
                 hidden=false, followlinks=false, sort=false): string
  
   walkdirs() recursively walks a directory structure, yielding
   directory names.  The strings that are generated will be rooted
   from 'path'.

     * path: the directory to start from
     * topdown: indicates whether to yield the directories using a
       preorder (vs. postorder) traversal
     * depth: indicates the maximal depth of recursion to use
     * hidden: indicates whether to enter hidden directories
     * followlinks: indicates whether to follow symbolic links or not
     * sort: indicates whether to consider subdirectories in sorted
       order or not
  
   by default, walkdirs() will start in the current directory, process
   directories in preorder; recursively traverse subdirectories; and
   neither follow dotfile directories nor symbolic links.  It will not
   sort the directories by default.
*/

iter walkdirs(path: string=".", topdown=true, depth=max(int), hidden=false, 
              followlinks=false, sort=false): string {

  if (topdown) then
    yield path;

  if (depth) {
    var subdirs = listdir(path, hidden=hidden, files=false, listlinks=followlinks);
    if (sort) {
      use Sort;
      QuickSort(subdirs);
    }

    for subdir in subdirs {
      const fullpath = path + "/" + subdir;
      for subdir in walkdirs(fullpath, topdown, depth-1, hidden, 
                             followlinks, sort) do
        yield subdir;
    }
  }

  if (!topdown) then
    yield path;
}


//
// Here's a parallel version
//
pragma "no doc"
iter walkdirs(path: string=".", topdown=true, depth=max(int), hidden=false, 
              followlinks=false, sort=false, param tag: iterKind): string 
       where tag == iterKind.standalone {

  if (sort) then
    warning("sorting has no effect for parallel invocations of walkdirs()");

  if (topdown) then
    yield path;

  if (depth) {
    var subdirs = listdir(path, hidden=hidden, files=false, listlinks=followlinks);
    forall subdir in subdirs {
      const fullpath = path + "/" + subdir;
      //
      // Call standalone walkdirs() iterator recursively; set sort=false since it is
      // not useful and we've already printed the warning
      //
      for subdir in walkdirs(fullpath, topdown, depth-1, hidden, followlinks, sort=false, iterKind.standalone) do
        yield subdir;
    }
  }

  if (!topdown) then
    yield path;
}



}
