/*
 * Copyright 2004-2020 Hewlett Packard Enterprise Development LP
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
   example, this module contains :ref:`file-manip` and functions for determining
   the :ref:`file-status`.  Also included are operations relating to the current
   process's file system state, which are performed on a specified locale
   (:ref:`locale-state`).  The module also contains iterators for traversing the
   file system (:ref:`filerator`).

   .. _file-manip:

   File/Directory Manipulations
   ----------------------------
   :proc:`copy`
   :proc:`copyFile`
   :proc:`copyTree`
   :proc:`mkdir`
   :proc:`moveDir`
   :proc:`remove`
   :proc:`rmTree`
   :proc:`symlink`
   :proc:`chmod`
   :proc:`chown`
   :proc:`copyMode`
   :proc:`rename`

   .. _file-status:

   File/Directory Properties
   -------------------------
   :proc:`getGID`
   :proc:`getMode`
   :proc:`getFileSize`
   :proc:`getUID`
   :proc:`exists`
   :proc:`isDir`
   :proc:`isFile`
   :proc:`isLink`
   :proc:`isMount`
   :proc:`sameFile`

   .. _locale-state:

   Locale State Functionality
   --------------------------
   :proc:`locale.chdir`
   :proc:`locale.cwd`
   :proc:`locale.umask`

   .. _filerator:

   File System Traversal Iterators
   -------------------------------
   :iter:`glob`
   :iter:`listdir`
   :iter:`walkdirs`
   :iter:`findfiles`

   Constant and Function Definitions
   ---------------------------------
 */
module FileSystem {

  use SysError;
  private use Path;
  private use HaltWrappers;
  private use SysCTypes;
  use IO;

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

/*
   Localizes and unescapes string to create a bytes to be used for obtaining a
   c_string to pass to extern file system operations.
*/
private inline proc unescape(str: string) {
  return str.encode(errors=encodePolicy.unescape);
}

/* Change the current working directory of the locale in question to the
   specified path `name`.

   .. warning::

      This is not safe within a parallel context.  A chdir call in one task
      will affect the current working directory of all tasks for that locale.

   :arg name: The intended current working directory
   :type name: `string`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc locale.chdir(name: string) throws {
  extern proc chpl_fs_chdir(name: c_string):syserr;

  var err: syserr = ENOERR;
  on this {
    err = chpl_fs_chdir(unescape(name).c_str());
  }
  if err then try ioerror(err, "in chdir", name);
}

pragma "no doc"
proc locale.chdir(out error: syserr, name: string) {
  compilerWarning("This version of locale.chdir() is deprecated; " +
                  "please switch to a throwing version");
  try {
    chdir(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

// CHPLDOC TODO: really want to make a section for S_IRUSR and friends.

/* Set the permissions of the file or directory specified by the argument
   `name` to that indicated by the argument `mode`.

   :arg name: The name of the file or directory whose permissions should be
              altered.
   :type name: `string`
   :arg mode: The permissions desired for the file or directory in question.
              See description of :const:`S_IRUSR`, for instance, for potential
              values.
   :type mode: `int`

   :throws FileNotFoundError: Thrown when the name specified does not correspond
                              to a file or directory that exists.
   :throws PermissionError: Thrown when the current user does not have
                            permission to change the permissions
*/
proc chmod(name: string, mode: int) throws {
  extern proc chpl_fs_chmod(name: c_string, mode: int): syserr;

  var err = chpl_fs_chmod(unescape(name).c_str(), mode);
  if err then try ioerror(err, "in chmod", name);
}

pragma "no doc"
proc chmod(out error: syserr, name: string, mode: int) {
  compilerWarning("This version of chmod() is deprecated; " +
                  "please switch to a throwing version");
  try {
    chmod(name, mode);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

/* Change one or both of the owner and group id of the named file or directory
   to the specified values.  If `uid` or `gid` are -1, the value in question
   will remain unchanged.

   :arg name: The name of the file to be changed.
   :type name: `string`
   :arg uid: The intended new owner(user) id, or -1 if it should remain the
             same.
   :type uid: `int`
   :arg gid: The intended new group owner(id), or -1 if it should remain the
             same.
   :type gid: `int`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc chown(name: string, uid: int, gid: int) throws {
  extern proc chpl_fs_chown(name: c_string, uid: c_int, gid: c_int):syserr;

  var err = chpl_fs_chown(unescape(name).c_str(), uid:c_int, gid:c_int);
  if err then try ioerror(err, "in chown", name);
}

pragma "no doc"
proc chown(out error: syserr, name: string, uid: int, gid: int) {
  compilerWarning("This version of chown() is deprecated; " +
                  "please switch to a throwing version");
  try {
    chown(name, uid, gid);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

// FUTURE WORK:
// When basename and joinPath are supported, enable dest to be a directory.

/* Copies the contents and permissions of the file indicated by `src` into
   the file or directory `dest`. If `metadata` is set to `true`, will also
   copy the metadata (uid, gid, time of last access and time of modification)
   of the file to be copied. A partially copied file or directory may be
   present in `dest` if there is an error in copying.

   .. note::

      In the future, when the :mod:`Path` module has been expanded further,
      this function will be able to support directories for the dest argument.

   :arg src: The source file whose contents and permissions are to be copied
   :type src: `string`
   :arg dest: The name of the destination file for the contents and permissions.
              May or may not exist previously, but will be overwritten if it did
              exist
   :type dest: `string`
   :arg metadata: This argument indicates whether to copy metadata associated
                  with the source file.  It is set to `false` by default.
   :type metadata: `bool`

   :throws IsADirectoryError: when `dest` is directory.
   :throws SystemError: thrown to describe another error if it occurs.
*/
proc copy(src: string, dest: string, metadata: bool = false) throws {
  var destFile = dest;
  try {
    if (isDir(destFile)) {
      // destFile = joinPath(destFile, basename(src));
      ioerror(EISDIR:syserr, "in copy(" + src + ", " + dest + ")");

      // Supporting a destination directory requires getting the basename from
      // the src (because we're using the same name) and joining it with the
      // provided destination directory.  Both of those operations are part of
      // the string portion, so we aren't supporting it just yet.
    }
  } catch e: FileNotFoundError {
    // Destination didn't exist before, and we're overwriting it anyways.
  }

  try copyFile(src, destFile);
  try copyMode(src, destFile);

  if (metadata) {
    extern proc chpl_fs_copy_metadata(source: c_string, dest: c_string): syserr;

    // Copies the access time, and time of last modification.
    // Does not copy uid, gid, or mode
    var err = chpl_fs_copy_metadata(unescape(src).c_str(),
                                    unescape(dest).c_str());
    if err then try ioerror(err, "in copy(" + src + ", " + dest + ")");

    // Get uid and gid from src
    var uid = try getUID(src);
    var gid = try getGID(src);

    // Change uid and gid to that of the src
    try chown(destFile, uid, gid);
  }
}

pragma "no doc"
proc copy(out error: syserr, src: string, dest: string, metadata: bool = false) {
  compilerWarning("This version of copy() is deprecated; " +
                  "please switch to a throwing version");
  try {
    copy(src, dest, metadata);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

/* Copies the contents of the file indicated by `src` into the file indicated
   by `dest`, replacing `dest`'s contents if it already exists (and is a
   different file than `src`, i.e. not a symbolic link to `src`).
   Does not copy metadata.

   :arg src: The source file whose contents are to be copied.
   :type src: `string`
   :arg dest: The intended destination of the contents.
   :type dest: `string`

   :throws FileNotFoundError: when `src` does not exist.
   :throws IsADirectoryError: when `src` or `dest` is a directory.
   :throws SystemError: when `src` and `dest` refer to the same file,
                        when `dest` is not writable,
                        or to describe another error if it occurs.
*/
proc copyFile(src: string, dest: string) throws {
  // This implementation is based off of the python implementation for copyfile,
  // with some slight differences.  That implementation was found at:
  // https://bitbucket.org/mirror/cpython/src/c8ce5bca0fcda4307f7ac5d69103ce128a562705/Lib/shutil.py?at=default
  // I did not look at the other functions in that file, except for copyfileobj
  // (which copyfile called).

  if !(try exists(src)) then
    // Source didn't exist, we can't copy it.
    try ioerror(ENOENT:syserr, "in copyFile(" + src + ", " + dest + ")");

  try {
    if (isDir(src) || isDir(dest)) {
      // If the source is a directory, the user has made a mistake, so return
      // an error.  The same is true if the destination is a directory.
      ioerror(EISDIR:syserr, "in copyFile(" + src + ", " + dest + ")");
    }

    if (sameFile(src, dest)) {
      // Check if the files are the same, error if yes
      try ioerror(EINVAL:syserr, "in copyFile(" + src + ", " + dest + ")");
    }
  } catch e: FileNotFoundError {
    // We don't care if dest did not exist before, we'll create or overwrite
    // it anyways.  We already know src exists.
  }

  // Open src for reading, open dest for writing
  var srcFile = try open(src, iomode.r);
  defer {
    try {
      srcFile.close();
    } catch { /* ignore errors */ }
  }

  var destFile = try open(dest, iomode.cw);
  defer {
    try {
      destFile.close();
    } catch { /* ignore errors */ }
  }

  var srcChnl = try srcFile.reader(kind=ionative, locking=false);
  defer {
    try {
      srcChnl.close();
    } catch { /* ignore errors */ }
  }

  var destChnl = try destFile.writer(kind=ionative, locking=false);
  defer {
    try {
      destChnl.close();
    } catch { /* ignore errors */ }
  }

  // read in, write out.
  var buf: bytes;
  var numRead: int = 0;
  // If increasing the read size, make sure there's a test in
  // test/library/standard/FileSystem that copies a file larger than one buffer.
  while (try srcChnl.readbytes(buf, len=4096)) {
    try destChnl.write(buf);
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
  }

  // throws close errors if reading is successful
  try destChnl.close();
  try srcChnl.close();
  try destFile.close();
  try srcFile.close();
}

pragma "no doc"
proc copyFile(out error: syserr, src: string, dest: string) {
  compilerWarning("This version of copyFile() is deprecated; " +
                  "please switch to a throwing version");
  try {
    copyFile(src, dest);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

/* Copies the permissions of the file indicated by `src` to the file indicated
   by `dest`, leaving contents, owner and group unaffected.

   :arg src: The source file whose permissions are to be copied.
   :type src: `string`
   :arg dest: The intended destination of the permissions.
   :type dest: `string`

   :throws FileNotFoundError: Thrown when the name specified does not correspond
                              to a file or directory that exists.
   :throws PermissionError: Thrown when the current user does not have
                            permission to change the permissions
*/
proc copyMode(src: string, dest: string) throws {
  try {
    // Gets the mode from the source file.
    var srcMode = getMode(src);
    // Sets the mode of the destination to the source's mode.
    chmod(dest, srcMode);
  } catch e: SystemError {
    // Hide implementation details.
    try ioerror(e.err, "in copyMode " + src, dest);
  }
}

pragma "no doc"
proc copyMode(out error: syserr, src: string, dest: string) {
  var err: syserr = ENOERR;
  try {
    copyMode(src, dest);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

/* Will recursively copy the tree which lives under `src` into `dst`,
   including all contents, permissions, and metadata.  `dst` must not
   previously exist, this function assumes it can create it and any missing
   parent directories. If `copySymbolically` is `true`, symlinks will be
   copied as symlinks, otherwise their contents and metadata will be copied
   instead.

   :arg src: The root of the source tree to be copied.
   :type src: `string`
   :arg dest: The root of the destination directory under which the contents of
              `src` are to be copied (must not exist prior to this function
              call).
   :type dest: `string`
   :arg copySymbolically: This argument is used to indicate how to handle
                          symlinks in the source directory.  It is set to
                          `false` by default
   :type copySymbolically: `bool`

   :throws FileExistsError: when the `dest` already exists.
   :throws NotADirectoryError: when `src` is not a directory.
   :throws SystemError: thrown to describe another error if it occurs.
*/
proc copyTree(src: string, dest: string, copySymbolically: bool=false) throws {
  var expectedErrorCases = try exists(dest);
  if (expectedErrorCases) then
    // dest exists.  That's not ideal.
    try ioerror(EEXIST:syserr, "in copyTree(" + src + ", " + dest + ")");

  expectedErrorCases = !(try isDir(src));
  if (expectedErrorCases) then
    try ioerror(ENOTDIR:syserr, "in copyTree(" + src + ", " + dest + ")");

  var srcPath = try realPath(src);
  try copyTreeHelper(srcPath, dest, copySymbolically);
}

private proc copyTreeHelper(src: string, dest: string, copySymbolically: bool=false) throws {
  // Create dest
  var oldMode = try getMode(src);
  try mkdir(dest, mode=oldMode, parents=true);

  for filename in listdir(path=src, dirs=false, files=true, listlinks=true) {
    // Take care of files in src
    var fileDestName = dest + "/" + filename;
    var fileSrcName = src + "/" + filename;
    if (try isLink(fileSrcName) && copySymbolically) {
      // Copy symbolically means symlinks should be copied as symlinks
      var realp = try realPath(fileSrcName);
      try symlink(realp, fileDestName);
    } else {
      // Either we didn't find a link, or copy symbolically is false, which
      // means we want the contents of the linked file, not a link itself.
      try copy(fileSrcName, fileDestName, metadata=true);
    }
  }

  for dirname in listdir(path=src, dirs=true, files=false, listlinks=true) {
    var dirDestName = dest+"/"+dirname;
    var dirSrcName = src+"/"+dirname;
    if (try isLink(dirSrcName) && copySymbolically) {
      // Copy symbolically means symlinks should be copied as symlinks
      var realp = try realPath(dirSrcName);
      try symlink(realp, dirDestName);
    } else {
      // Either we didn't find a link, or copy symbolically is false, which
      // means we want the contents of the linked directory, not a link itself.
      try copyTreeHelper(dirSrcName, dirDestName, copySymbolically);
    }
  }
}

pragma "no doc"
proc copyTree(out error: syserr, src: string, dest: string, copySymbolically: bool=false) {
  compilerWarning("This version of copyTree() is deprecated; " +
                  "please switch to a throwing version");
  try {
    copyTree(src, dest, copySymbolically);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

/* Obtains and returns the current working directory for this locale.

   .. warning::

      Another task on this locale can change the current working directory from
      underneath this task, so use caution when making use of this method in
      a parallel environment.

   :return: The current working directory for the locale in question.
   :rtype: `string`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc locale.cwd(): string throws {
  extern proc chpl_fs_cwd(ref working_dir:c_string):syserr;

  var ret:string;
  var err: syserr = ENOERR;
  on this {
    var tmp:c_string;
    // c_strings can't cross on statements.
    err = chpl_fs_cwd(tmp);
    try! {
      ret = createStringWithNewBuffer(tmp, errors=decodePolicy.escape);
    }
  }
  if err != ENOERR then try ioerror(err, "in cwd");
  return ret;
}

pragma "no doc"
proc locale.cwd(out error: syserr): string {
  compilerWarning("This version of locale.cwd() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return cwd();
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return "";
}

/* Determines if the file or directory indicated by `name` exists and returns
   the result of this check.

   :arg name: The file or directory whose existence is in question.
   :type name: `string`

   :return: `true` if the provided argument corresponds to an existing file or
            directory, `false` otherwise.  Also returns `false` for broken
            symbolic links.
   :rtype: `bool`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc exists(name: string): bool throws {
  extern proc chpl_fs_exists(ref result:c_int, name: c_string): syserr;

  if (name.isEmpty()) {
    // chpl_fs_exists uses stat to determine if a file exists, which throws an
    // error when "" is passed to it.  Check it here early and return false
    // like Python does
    return false;
  }

  var ret:c_int;
  var err = chpl_fs_exists(ret, unescape(name).c_str());
  if err then try ioerror(err, "in exists");
  return ret != 0;
}

pragma "no doc"
proc exists(out error: syserr, name: string): bool {
  compilerWarning("This version of exists() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return exists(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return false;
}


/* Finds files from a given start directory and yields their names,
   similar to simple invocations of the command-line `find` utility.
   May be invoked in serial or non-zippered parallel contexts.

   :arg startdir: The root directory from which to start the search
                  (defaults to ``"."``)
   :type startdir: `string`

   :arg recursive: Indicates whether or not to descend recursively into
                   subdirectories (defaults to `false`)
   :type recursive: `bool`

   :arg hidden: Indicates whether or not to descend into hidden subdirectories and yield hidden files (defaults to `false`)
   :type hidden: `bool`

   :yield:  The paths to any files found, relative to `startdir`, as strings
*/

iter findfiles(startdir: string = ".", recursive: bool = false,
               hidden: bool = false): string {
  if (recursive) then
    for subdir in walkdirs(startdir, hidden=hidden) do
      for file in listdir(subdir, hidden=hidden, dirs=false, files=true, listlinks=true) do
        yield subdir+"/"+file;
  else
    for file in listdir(startdir, hidden=hidden, dirs=false, files=true, listlinks=false) do
      yield startdir+"/"+file;
}

pragma "no doc"
iter findfiles(startdir: string = ".", recursive: bool = false,
               hidden: bool = false, param tag: iterKind): string
       where tag == iterKind.standalone {
  if (recursive) then
    // Why "with (ref hidden)"?  A: the compiler currently allows only
    // [const] ref intents in forall loops over recursive parallel iterators
    // such as walkdirs().
    forall subdir in walkdirs(startdir, hidden=hidden) with (ref hidden) do
      for file in listdir(subdir, hidden=hidden, dirs=false, files=true, listlinks=true) do
        yield subdir+"/"+file;
  else
    for file in listdir(startdir, hidden=hidden, dirs=false, files=true, listlinks=false) do
      yield startdir+"/"+file;
}

/* Obtains and returns the group id associated with the file or directory
   specified by `name`.

   :arg name: The file or directory whose group id is desired
   :type name: `string`

   :return: The group id of the file or directory in question
   :rtype: `int`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc getGID(name: string): int throws {
  extern proc chpl_fs_get_gid(ref result: c_int, filename: c_string): syserr;

  var result: c_int;
  var err = chpl_fs_get_gid(result, unescape(name).c_str());
  if err then try ioerror(err, "in getGID");
  return result;
}

pragma "no doc"
proc getGID(out error: syserr, name: string): int {
  compilerWarning("This version of getGID() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return getGID(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return 0;
}

/* Obtains and returns the current permissions of the file or directory
   specified by `name`.

   :arg name: The file or directory whose permissions are desired.
   :type name: `string`

   :return: The permissions of the specified file or directory
            See description of :const:`S_IRUSR`, for instance, for potential
            values.
   :rtype: `int`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc getMode(name: string): int throws {
  extern proc chpl_fs_viewmode(ref result:c_int, name: c_string): syserr;

  var ret:c_int;
  var err = chpl_fs_viewmode(ret, unescape(name).c_str());
  if err then try ioerror(err, "in getMode", name);
  return ret;
}

pragma "no doc"
proc getMode(out error: syserr, name: string): int {
  compilerWarning("This version of getMode() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return getMode(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return 0;
}

/* Obtains and returns the size (in bytes) of the file specified by `name`.

   :arg name: The file whose size is desired
   :type name: `string`

   :return: The size in bytes of the file in question
   :rtype: `int`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc getFileSize(name: string): int throws {
  extern proc chpl_fs_get_size(ref result: int, filename: c_string):syserr;

  var result: int;
  var err = chpl_fs_get_size(result, unescape(name).c_str());
  if err then try ioerror(err, "in getFileSize", name);
  return result;
}

pragma "no doc"
proc getFileSize(out error: syserr, name: string): int {
  compilerWarning("This version of getFileSize() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return getFileSize(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return 0;
}

/* Obtains and returns the user id associated with the file or directory
   specified by `name`.

   :arg name: The file or directory whose user id is desired
   :type name: `string`

   :return: The user id of the specified file or directory
   :rtype: `int`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc getUID(name: string): int throws {
  extern proc chpl_fs_get_uid(ref result: c_int, filename: c_string): syserr;

  var result: c_int;
  var err = chpl_fs_get_uid(result, unescape(name).c_str());
  if err then try ioerror(err, "in getUID");
  return result;
}

pragma "no doc"
proc getUID(out error: syserr, name: string): int {
  compilerWarning("This version of getUID() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return getUID(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return false;
}

//
// This is a helper module used by the various glob() overloads.
// It provides wrappers over the C-level routines that take care
// of casting and error checking.
//
private module GlobWrappers {
  extern type glob_t;

  private extern const GLOB_NOMATCH: c_int;
  private extern const GLOB_NOSPACE: c_int;

  // glob wrapper that takes care of casting and error checking
  inline proc glob_w(pattern: string, ref ret_glob:glob_t): void {
    extern proc chpl_glob(pattern: c_string, flags: c_int,
                          ref ret_glob: glob_t): c_int;

    const GLOB_NOFLAGS = 0: c_int;
    const err = chpl_glob(unescape(pattern).c_str(), GLOB_NOFLAGS, ret_glob);

    // When no flags are being passed, glob can only fail with GLOB_NOMATCH or
    // GLOB_NOSPACE. GLOB_NOMATCH is fine and will just result in a 0 length
    // glob_num. GLOB_NOSPACE occurs if the `malloc()` in glob failed, so just
    // convert that into an out of memory error.
    assert (err == 0 || err == GLOB_NOMATCH || err == GLOB_NOSPACE);
    if err == GLOB_NOSPACE then
      HaltWrappers.outOfMemoryHalt("glob()");
  }

  // glob_num wrapper that takes care of casting
  inline proc glob_num_w(glb: glob_t): int {
    extern proc chpl_glob_num(glb: glob_t): size_t;
    return chpl_glob_num(glb).safeCast(int);
  }

  // glob_index wrapper that takes care of casting
  inline proc glob_index_w(glb: glob_t, idx: int): string {
    extern proc chpl_glob_index(glb: glob_t, idx: size_t): c_string;
    try! {
      return createStringWithNewBuffer(chpl_glob_index(glb,
                                                       idx.safeCast(size_t)),
                                       errors=decodePolicy.escape);
    }
  }

  // globfree wrapper that exists only for symmetry in the routine names
  inline proc globfree_w(ref glb: glob_t): void {
    extern proc globfree(ref glb: glob_t): void;
    globfree(glb);
  }
}


/* Yields filenames that match a given `glob` pattern.  May be invoked
   in serial or parallel contexts (zippered or non-).

   :arg pattern: The glob pattern to match against (defaults to ``"*"``)
   :type pattern: `string`

   :yield: The matching filenames as strings
*/
iter glob(pattern: string = "*"): string {
  use GlobWrappers;
  var glb : glob_t;

  glob_w(pattern, glb);
  const num = glob_num_w(glb);

  for i in 0..num-1 do
    yield glob_index_w(glb, i);

  globfree_w(glb);
}


pragma "no doc"
iter glob(pattern: string = "*", param tag: iterKind): string
       where tag == iterKind.standalone {
  use GlobWrappers;
  var glb : glob_t;

  glob_w(pattern, glb);
  const num = glob_num_w(glb);

  forall i in 0..num-1 do
    yield glob_index_w(glb, i);

  globfree_w(glb);
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
iter glob(pattern: string = "*", param tag: iterKind)
       where tag == iterKind.leader {
  use GlobWrappers;
  var glb : glob_t;

  glob_w(pattern, glb);
  const num = glob_num_w(glb);
  globfree_w(glb);

  //
  // Forward to the range type's leader
  //
  for followThis in (0..num-1).these(tag) do
    yield followThis;
}

pragma "no doc"
iter glob(pattern: string = "*", followThis, param tag: iterKind): string
       where tag == iterKind.follower {
  use GlobWrappers;
  var glb : glob_t;
  if (followThis.size != 1) then
    compilerError("glob() iterator can only be zipped with 1D iterators");
  var r = followThis(1);

  glob_w(pattern, glb);
  const num = glob_num_w(glb);
  if (r.high >= num) then
    HaltWrappers.zipLengthHalt("glob() is being zipped with something too big; it only has " + num:string + " matches");

  for i in r do
    yield glob_index_w(glb, i);

  globfree_w(glb);
}

/* Determine if the provided path `name` corresponds to a directory and return
   the result

   :arg name: A path that could refer to a directory.
   :type name: `string`

   :return: `true` if the path is a directory, `false` if it is not
   :rtype: `bool`

   :throws SystemError: Thrown to describe an error if one occurs,
                        including the case where the path does not refer
                        to a valid file or directory.
*/
proc isDir(name:string):bool throws {
  extern proc chpl_fs_is_dir(ref result:c_int, name: c_string):syserr;

  var ret:c_int;
  var doesExist = try exists(name);
  if !doesExist then return false;

  var err = chpl_fs_is_dir(ret, unescape(name).c_str());
  if err then try ioerror(err, "in isDir", name);
  return ret != 0;
}

pragma "no doc"
proc isDir(out error:syserr, name:string):bool {
  compilerWarning("This version of isDir() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return isDir(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return false;
}

/* Determine if the provided path `name` corresponds to a file and return
   the result

   :arg name: A path that could refer to a file.
   :type name: `string`

   :return: `true` if the path is a file, `false` if it is not
   :rtype: `bool`

   :throws SystemError: Thrown to describe an error if one occurs,
                        including the case where the path does not refer
                        to a valid file or directory.
*/
proc isFile(name:string):bool throws {
  extern proc chpl_fs_is_file(ref result:c_int, name: c_string):syserr;

  var ret:c_int;
  var doesExist = try exists(name);
  if !doesExist then return false;

  var err = chpl_fs_is_file(ret, unescape(name).c_str());
  if err then try ioerror(err, "in isFile", name);
  return ret != 0;
}

pragma "no doc"
proc isFile(out error:syserr, name:string):bool {
  compilerWarning("This version of isFile() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return isFile(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return false;
}

/* Determine if the provided path `name` corresponds to a link and return the
   result.  If symbolic links are not supported, will return `false`.

   :arg name: A path that could refer to a symbolic link.
   :type name: `string`

   :return: `true` if the path is a symbolic link, `false` if it is not or
            if symbolic links are not supported.
   :rtype: `bool`

   :throws SystemError: Thrown to describe an error if one occurs,
                        including the case where the path does not refer
                        to a valid file or directory.
*/
proc isLink(name: string): bool throws {
  extern proc chpl_fs_is_link(ref result:c_int, name: c_string): syserr;

  if (name.isEmpty()) {
    // chpl_fs_is_link uses lstat to determine if a path is a link, which throws
    // an error when "" is passed to it.  Check it here early and return false
    // like Python does
    return false;
  }

  var ret:c_int;
  var err = chpl_fs_is_link(ret, unescape(name).c_str());
  if err then try ioerror(err, "in isLink", name);
  return ret != 0;
}

pragma "no doc"
proc isLink(out error:syserr, name: string): bool {
  compilerWarning("This version of isLink() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return isLink(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return false;
}

/* Determine if the provided path `name` corresponds to a mount point and
   return the result.

   :arg name: A path that could refer to a mount point.
   :type name: `string`

   :return: `true` if the path is a mount point, `false` if it is not.
   :rtype: `bool`

   :throws SystemError: Thrown to describe an error if one occurs,
                        including the case where the path does not refer
                        to a valid file or directory.
*/
proc isMount(name: string): bool throws {

  extern proc chpl_fs_is_mount(ref result:c_int, name: c_string): syserr;

  var doesExist = try exists(name);
  if !doesExist then return false;

  // Files aren't mount points.  That would be silly.
  var doesExistFile = try isFile(name);
  if doesExistFile then return false;

  var ret:c_int;
  var err = chpl_fs_is_mount(ret, unescape(name).c_str());
  if err then try ioerror(err, "in isMount", name);
  return ret != 0;
}

pragma "no doc"
proc isMount(out error:syserr, name: string): bool {
  compilerWarning("This version of isMount() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return isMount(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return false;
}

/* Lists the contents of a directory.  May be invoked in serial
   contexts only.

   :arg path: The directory whose contents should be listed
              (defaults to ``"."``)
   :type path: `string`

   :arg hidden: Indicates whether hidden files/directory should be listed
                (defaults to `false`)
   :type hidden: `bool`

   :arg dirs: Indicates whether directories should be listed
              (defaults to `true`)
   :type dirs: `bool`

   :arg files: Indicates whether files should be listed (defaults to `true`)
   :type files: `bool`

   :arg listlinks: Indicates whether symbolic links should be listed
                   (defaults to `true`)
   :type listlinks: `bool`

   :yield: The names of the specified directory's contents, as strings
*/
iter listdir(path: string = ".", hidden: bool = false, dirs: bool = true,
              files: bool = true, listlinks: bool = true): string {
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
  var err:syserr = ENOERR;
  dir = opendir(unescape(path).c_str());
  if (!is_c_nil(dir)) {
    ent = readdir(dir);
    while (!is_c_nil(ent)) {
      var filename: string;
      try! {
        filename = createStringWithNewBuffer(ent.d_name(),
                                             errors=decodePolicy.escape);
      }
      if (hidden || filename[1] != '.') {
        if (filename != "." && filename != "..") {
          const fullpath = path + "/" + filename;

          // TODO: revisit error handling for this method
          try {
            if (listlinks || !isLink(fullpath)) {
              if (dirs && isDir(fullpath)) then
                yield filename;
              else if (files && isFile(fullpath)) then
                yield filename;
            }
          } catch e: SystemError {
            writeln("error in listdir(): ", errorToString(e.err));
            break;
          } catch {
            writeln("unknown error in listdir()");
            break;
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

/* Attempt to create a directory with the given path, `name`.  If `parents`
   is `true`, will attempt to create any directory in the path that did not
   previously exist.

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
   :type name: `string`
   :arg mode: The permissions desired for the directory to create.  Takes the
              current :proc:`~locale.umask` into account.  See description of
              :const:`S_IRUSR`, for instance, for potential values.
   :type mode: `int`
   :arg parents: Indicates whether parent directories should be created.  If
                 set to `false`, any nonexistent parent will cause an error
                 to occur.
   :type parents: `bool`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc mkdir(name: string, mode: int = 0o777, parents: bool=false) throws {
  extern proc chpl_fs_mkdir(name: c_string, mode: int, parents: bool):syserr;

  if name.isEmpty() then
    try ioerror(ENOENT:syserr, "mkdir called with illegal path: '" + name + "'");

  var err = chpl_fs_mkdir(unescape(name).c_str(), mode, parents);
  if err then try ioerror(err, "in mkdir", name);
}

pragma "no doc"
proc mkdir(out error: syserr, name: string, mode: int = 0o777,
           parents: bool=false) {
  compilerWarning("This version of isDir() is deprecated; " +
                  "please switch to a throwing version");
  try {
    mkdir(name, mode, parents);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

/* Recursively moves the directory indicated by `src` and its contents to the
   destination denoted by `dest`.  If `dest` is a directory, `src` is moved
   inside of it.

   .. note::

      We do not currently support the case where the dest argument already
      exists and is a directory.  When the :mod:`Path` module has been
      expanded further, this support can be enabled.

   :arg src: the location of the directory to be moved
   :type src: `string`
   :arg dest: the location to move it to.
   :type dest: `string`

   :throws IsADirectoryError: when `dest` exists and is a directory.
   :throws NotADirectoryError: when `dest` exists and is not a directory.
   :throws IllegalArgumentError: when `src` and `dest` is the same directory.
   :throws SystemError: thrown to describe another error if it occurs.
*/
proc moveDir(src: string, dest: string) throws {
  var destExists = try exists(dest);
  if (destExists) {
    // dest already existed
    if (try isFile(dest)) {
      // dest is a file, we can't move src within it!
      // Note: Python gives EEXIST in this case, but I think ENOTDIR is
      // clearer.
      try ioerror(ENOTDIR:syserr, "in moveDir(" + src + ", " + dest + ")");
    } else if (try isDir(dest)) {
      if (try sameFile(src, dest)) {
        // Python's behavior when calling move over the same directory for
        // source and destination is to fail with a helpful error message.
        // Since this error code shouldn't occur otherwise, it signals to
        // the wrapper function what has happened.
        throw new owned IllegalArgumentError("src", "Cannot move a directory \'" + src + "\' into itself \'" + dest + "\'.");
      } else {
        // dest is a directory, we'll copy src inside it
        // NOT YET SUPPORTED.  Requires basename and joinPath
        try ioerror(EISDIR:syserr, "unsupported operation in moveDir(" + src + ", " + dest + ")");
      }
    } else {
      // What we've been provided is both not a file and not a directory.  Given
      // the expected behavior of isFile and isDir when it comes to symlinks,
      // I'm not sure how this case would arise.
      try ioerror(ENOTDIR:syserr, "unsupported operation in moveDir(" + src + ", " + dest + ")");
    }
  } else {
    try copyTree(src, dest, true);
    try rmTree(src);
  }
}

pragma "no doc"
proc moveDir(out error: syserr, src: string, dest: string) {
  compilerWarning("This version of moveDir() is deprecated; " +
                  "please switch to a throwing version");
  try {
    moveDir(src, dest);
  } catch e: SystemError {
    error = e.err;
  } catch e: IllegalArgumentError {
    error = EEXIST;
  } catch {
    error = EINVAL;
  }
}

/* Renames the file specified by `oldname` to `newname`.  The file is not
   opened during this operation.

   :arg oldname: Current name of the file
   :type oldname: `string`
   :arg newname: Name which should be used to refer to the file in the future.
   :type newname: `string`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc rename(oldname: string, newname: string) throws {
  extern proc chpl_fs_rename(oldname: c_string, newname: c_string):syserr;

  var err = chpl_fs_rename(unescape(oldname).c_str(),
                           unescape(newname).c_str());
  if err then try ioerror(err, "in rename", oldname);
}

pragma "no doc"
proc rename(out error: syserr, oldname, newname: string) {
  compilerWarning("This version of rename() is deprecated; " +
                  "please switch to a throwing version");
  try {
    rename(oldname, newname);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return false;
}

/* Removes the file or directory specified by `name`

   :arg name: The file/directory to remove
   :type name: `string`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc remove(name: string) throws {
  extern proc chpl_fs_remove(name: c_string):syserr;

  var err = chpl_fs_remove(unescape(name).c_str());
  if err then try ioerror(err, "in remove", name);
}

pragma "no doc"
proc remove(out error: syserr, name: string) {
  try {
    remove(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

/* Delete the entire directory tree specified by root.

   :arg root: path name representing a directory that should be deleted along
              with its entire contents.
   :type root: `string`

   :throws FileNotFoundError: when `root` does not exist.
   :throws NotADirectoryError: when `root` is not a directory.
   :throws SystemError: thrown to describe another error if it occurs.
*/
proc rmTree(root: string) throws {
  // root doesn't exist.  We can't remove something that isn't there
  var rootExists = try exists(root);
  if !rootExists then try ioerror(ENOENT:syserr, "in rmTree(" + root + ")");

  // We need it to be a directory!
  var rootIsDir = try isDir(root);
  if !rootIsDir then try ioerror(ENOTDIR:syserr, "in rmTree(" + root + ")");

  var rootPath = try realPath(root);
  try rmTreeHelper(rootPath);
}

private proc rmTreeHelper(root: string) throws {
  // Go through all the files in this current directory and remove them
  for filename in listdir(path=root, dirs=false, files=true, listlinks=true, hidden=true) {
    var name = root + "/" + filename;
    try remove(name);
  }
  // Then traverse all the directories within this current directory and have
  // them handle cleaning up their contents and themselves
  for dirname in listdir(path=root, dirs=true, files=false, listlinks=true, hidden=true) {
    var fullpath = root + "/" + dirname;
    var dirIsLink = try isLink(fullpath);
    if (dirIsLink) {
      try remove(fullpath);
    } else {
      try rmTreeHelper(fullpath);
    }
  }
  // Once everything else has been removed, remove ourself.
  try remove(root);
}

pragma "no doc"
proc rmTree(out error: syserr, root: string) {
  compilerWarning("This version of rmTree() is deprecated; " +
                  "please switch to a throwing version");
  try {
    rmTree(root);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

/* Determines if both pathnames refer to the same file or directory (utilizing
   operating system operations rather than string ones, due to the possibility
   of symbolic links, :data:`~Path.curDir`, or :data:`~Path.parentDir` appearing
   in the path) and returns the result of that check

   :arg file1: The first path to be compared.
   :type file1: `string`
   :arg file2: The second path to be compared.
   :type file2: `string`

   :return: `true` if the two paths refer to the same file or directory,
            `false` otherwise.
   :rtype: `bool`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc sameFile(file1: string, file2: string): bool throws {
  extern proc chpl_fs_samefile_string(ref ret: c_int,
                                      file1: c_string, file2: c_string): syserr;

  var ret:c_int;
  var err = chpl_fs_samefile_string(ret, unescape(file1).c_str(),
                                         unescape(file2).c_str());
  if err then try ioerror(err, "in sameFile(" + file1 + ", " + file2 + ")");
  return ret != 0;
}

pragma "no doc"
proc sameFile(out error: syserr, file1: string, file2: string): bool {
  compilerWarning("This version of sameFile() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return sameFile(file1, file2);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return false;
}

/* Determines if both :type:`~IO.file` records refer to the same file
   (utilizing operating system operations rather than string ones, due to the
   possibility of symbolic links, :data:`~Path.curDir`, or
   :data:`~Path.parentDir` appearing in the path) and returns the result of that
   check

   :arg file1: The first file to be compared.
   :type file1: `file`
   :arg file2: The second file to be compared.
   :type file2: `file`

   :return: `true` if the two records refer to the same file, `false`
            otherwise.
   :rtype: `bool`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc sameFile(file1: file, file2: file): bool throws {
  extern proc chpl_fs_samefile(ref ret: c_int, file1: qio_file_ptr_t,
                               file2: qio_file_ptr_t): syserr;

  // If one of the files references a null file, propagate to avoid a segfault.
  try {
    file1.check();
    file2.check();
  }

  var ret:c_int;
  var err = chpl_fs_samefile(ret, file1._file_internal, file2._file_internal);
  if err then try ioerror(err, "in sameFile " + file1.tryGetPath(),
                          file2.tryGetPath());
  return ret != 0;
}

pragma "no doc"
proc sameFile(out error: syserr, file1: file, file2: file): bool {
  compilerWarning("This version of someFile() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return sameFile(file1, file2);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return false;
}

/* Create a symbolic link pointing to `oldName` with the path `newName`.

   :arg oldName: The source file to be linked
   :type oldName: `string`
   :arg newName: The location where the symbolic link should live
   :type newName: `string`

   :throws SystemError: Thrown to describe an error if one occurs.
*/
proc symlink(oldName: string, newName: string) throws {
  extern proc chpl_fs_symlink(orig: c_string, linkName: c_string): syserr;

  var err = chpl_fs_symlink(unescape(oldName).c_str(),
                            unescape(newName).c_str());
  if err then try ioerror(err, "in symlink " + oldName, newName);
}

pragma "no doc"
proc symlink(out error: syserr, oldName: string, newName: string) {
  var err:syserr = ENOERR;
  compilerWarning("This version of symlink() is deprecated; " +
                  "please switch to a throwing version");
  try {
    symlink(err, oldName, newName);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
}

/* Sets the file creation mask of the current locale to `mask`, and returns
   the previous value of the file creation mask for that locale.  See
   description of :const:`S_IRUSR`, for instance, for potential values.

   .. warning::

      This is not safe within a parallel context.  A umask call in one task
      will affect the umask of all tasks for that locale.


   :arg mask: The file creation mask to use now.
   :type mask: `int`

   :return: The previous file creation mask
   :rtype: `int`
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


/* Recursively walk a directory structure, yielding directory names.
   May be invoked in serial or non-zippered parallel contexts.

   .. note::
            The current parallel version is not very adaptive/dynamic
            in its application of parallelism to the list of
            subdirectories at any given level of the traversal, and
            could be improved in this regard.

   :arg path: The directory from which to start the walk (defaults to ``"."``)
   :type path: `string`

   :arg topdown: Indicates whether to yield a directory before or after descending into its children (defaults to `true`)
   :type topdown: `bool`

   :arg depth: Indicates the maximum recursion depth to use (defaults to `max(int)`)
   :type depth: `int`

   :arg hidden: Indicates whether to descend into hidden directories (defaults to `false`)
   :type hidden: `bool`

   :arg followlinks: Indicates whether to follow symbolic links (defaults to `false`)
   :type followlinks: `bool`

   :arg sort: Indicates whether or not to consider subdirectories in sorted order (defaults to `false`).  Note that requesting sorting has no effect in parallel invocations.
   :type sort: `bool`

   :yield: The directory names encountered, relative to `path`, as strings
*/
iter walkdirs(path: string = ".", topdown: bool = true, depth: int = max(int),
              hidden: bool = false, followlinks: bool = false,
              sort: bool = false): string {

  if (topdown) then
    yield path;

  if (depth) {
    var subdirs = listdir(path, hidden=hidden, files=false, listlinks=followlinks);
    if (sort) {
      use Sort /* only sort */;
      sort(subdirs);
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
iter walkdirs(path: string = ".", topdown: bool = true, depth: int =max(int),
              hidden: bool = false, followlinks: bool = false,
              sort: bool = false, param tag: iterKind): string
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
