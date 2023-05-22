/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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

/* A file utilities library focusing on path operations.

   The Path module focuses on manipulation of the path to a file or directory.
   Also provided are constant values representing common idioms that may vary
   across operating systems (though rarely in the modern era), such as general
   references to a parent directory or the current directory.

   .. note::

      This module is currently missing the implementation for `expandUser
      <https://github.com/chapel-lang/chapel/issues/6008>`_, `normCase
      <https://github.com/chapel-lang/chapel/issues/6013>`_, Once those are
      implemented, it will be considered complete.

   Operations which occur on the files or directories referred to by these paths
   may be found in :mod:`FileSystem` (for operations *on* the file) or :mod:`IO`
   (for operations *within* the file).

   Path Computations
   -----------------
   :proc:`commonPath`
   :proc:`normPath`
   :proc:`realPath`
   :proc:`relPath`

   Path Manipulations
   ------------------
   :proc:`absPath`
   :proc:`expandVars`
   :proc:`joinPath`
   :proc:`replaceBasename`
   :proc:`replaceDirname`
   :proc:`replaceExt`
   :proc:`splitExt`
   :proc:`splitPath`

   Path Properties
   ---------------
   :proc:`basename`
   :proc:`dirname`
   :proc:`isAbsPath`

   Constant and Function Definitions
   ---------------------------------
*/
module Path {

use List;
use IO;
use OS, OS.POSIX;
use CTypes;

/*
   Represents generally the current directory. This starts as the directory
   where the program is being executed from. On all the platforms that Chapel
   supports this parameter is set to ".".
*/
param curDir;

/*
   Represents generally the parent directory. On all the platforms that Chapel
   supports this parameter is set to "..".
*/
param parentDir;

/*
   Denotes the separator between a directory and its child.  On all the
   platforms that Chapel supports this parameter is set to "/"
*/
param pathSep;

// these can be set within a param `if`, in case we start to support platforms
// where these are not the right values (e.g. Windows)
curDir = ".";
parentDir = "..";
pathSep = "/";

/*
   Localizes and unescapes string to create a bytes to be used for obtaining a
   c_string to pass to extern file system operations.
*/
private inline proc unescape(str: string) {
  return str.encode(policy=encodePolicy.unescape);
}

/*
  Creates a normalized absolutized version of a path. On most platforms, when
  given a non-absolute path this function is equivalent to the following code:

  .. code-block:: Chapel

    normPath(joinPath(here.cwd(), path))

  See :proc:`normPath()`, :proc:`joinPath()`, :proc:`~FileSystem.locale.cwd()`
  for details.

  .. warning::

    This function is unsafe for use in a parallel environment due to its
    reliance on :proc:`~FileSystem.locale.cwd()`. Another task on the current
    locale may change the current working directory at any time.

  :arg path: The path whose absolute path is desired.
  :type path: `string`

  :return: A normalized, absolutized version of the path specified.
  :rtype: `string`

  :throws SystemError: Upon failure to get the current working directory.
*/
proc absPath(path: string): string throws {
  use FileSystem;

  if !isAbsPath(path) then
    return normPath(joinPath(try here.cwd(), path));
  return normPath(path);
}

/*
  Creates a normalized absolutized version of the path of a
  :type:`~IO.file`. On most platforms, when given a non-absolute path this
  function is equivalent to the following code:

  .. code-block:: Chapel

      normPath(joinPath(here.cwd(), f.path))

  See :proc:`normPath()`, :proc:`joinPath()`, :proc:`~FileSystem.locale.cwd()`,
  :proc:`~IO.file.path` for details.

  .. warning::

    This method is unsafe for use in a parallel environment due to its
    reliance on :proc:`~FileSystem.locale.cwd()`. Another task on the current
    locale may change the current working directory at any time.

  :arg f: The file whose absolute path is desired.
  :type f: :type:`~IO.file`

  :return: A normalized, absolutized version of the path for the file argument.
  :rtype: `string`

  :throws SystemError: Upon failure to get the current working directory.
*/
proc absPath(f: file): string throws {
  // switch this to "return try f.path" after relative path deprecation is removed
  return try f._abspath;
}

/* Returns the file name portion of the path provided.  For instance:

   .. code-block:: Chapel

      writeln(basename("/foo/bar/baz")); // Prints "baz"
      writeln(basename("/foo/bar/")); // Prints "", because of the empty string

   Note that this is different from the Unix ``basename`` function.

   :arg path: A string file name.  Note that this string does not have to be
              a valid file name, as the file itself will not be affected.
   :type path: `string`
*/
proc basename(path: string): string {
   return splitPath(path)[1];
}

/* Determines and returns the longest common path prefix of
   all the string pathnames provided.

   :arg paths: Any number of paths.
   :type paths: `string`

   :return: The longest common path prefix.
   :rtype: `string`
*/
proc commonPath(paths: string ...?n): string {
  var result: string = "";    // result string
  var inputLength = n;   // size of input array
  var firstPath = paths(0);
  var flag: int = 0;

  // if input is empty, return empty string.
  // if input is just one string, return that string as longest common prefix
  // path.

  if inputLength == 0 then {
    return result;
  } else if inputLength == 1 then{
    return firstPath;
  }

  var prefixList = new list(string);
  for x in firstPath.split(pathSep, -1, false) do
    prefixList.pushBack(x);

  var pos = prefixList.size;   // rightmost index of common prefix
  var minPathLength = prefixList.size;

  for i in 1..n-1 do {

    var tempList = new list(string);
    for x in paths(i).split(pathSep, -1, false) do
      tempList.pushBack(x);

    var minimum = min(prefixList.size, tempList.size);

    if minimum < minPathLength then {
      minPathLength = minimum;
    }

    for itr in 0..#minimum do {
      if (tempList[itr]!=prefixList[itr] && itr<pos) {
        pos = itr;
        flag=1;   // indicating that pos was changed
        break;
      }
    }
  }

  if (flag == 1) {
    for i in pos..prefixList.size-1 by -1 do
      try! prefixList.pop(i);
  } else {
    for i in minPathLength..prefixList.size-1 by -1 do
      try! prefixList.pop(i);
    // in case all paths are subsets of the longest path thus pos was never
    // updated
  }

  result = pathSep.join(prefixList.these());
  return result;
}

/* Determines and returns the longest common path prefix of
   all the string pathnames provided.

   :arg paths: Any number of paths as an array.
   :type paths: `array`

   :return: The longest common path prefix.
   :rtype: `string`
*/

proc commonPath(paths: []): string {
  var result: string = "";    // result string
  var inputLength = paths.size;   // size of input array
  if inputLength == 0 then {     // if input is empty, return empty string.
    return result;
  }

  var start: int = paths.domain.first;
  var end: int = paths.domain.last;
  var firstPath = paths[start];
  var delimiter: string;
  var flag: int = 0;

  // if input is just one string, return that string as longest common prefix
  // path.

  if inputLength == 1 then{
    return firstPath;
  }

  // finding delimiter to split the paths.

  if firstPath.find("\\") == -1 then {
    delimiter = "/";
  } else {
    delimiter = "\\";
  }

  var prefixList = new list(string);
  for x in firstPath.split(delimiter, -1, false) do
    prefixList.pushBack(x);
  // array of resultant prefix string

  var pos = prefixList.size;   // rightmost index of common prefix
  var minPathLength = prefixList.size;

  for i in (start+1)..end do {

    var tempList = new list(string);
    for x in paths[i].split(delimiter, -1, false) do
      tempList.pushBack(x);
    // temporary array storing the current path under consideration

    var minimum = min(prefixList.size, tempList.size);

    if minimum < minPathLength then {
      minPathLength = minimum;
    }

    for itr in 0..#minimum do {
      if (tempList[itr]!=prefixList[itr] && itr<=pos) {
        pos = itr;
        flag = 1;   // indicating that pos was changed
        break;
      }
    }
  }

  if (flag == 1) {
    for i in pos..prefixList.size-1 by -1 do
      try! prefixList.pop(i);
  } else {
    for i in minPathLength..prefixList.size-1 by -1 do
      try! prefixList.pop(i);
    // in case all paths are subsets of the longest path thus pos was never
    // updated
  }

  result = delimiter.join(prefixList.these());
  return result;
}

/* Returns the parent directory portion of the path provided.  For instance:

   .. code-block:: Chapel

      writeln(dirname("/foo/bar/baz")); // Prints "/foo/bar"
      writeln(dirname("/foo/bar/")); // Also prints "/foo/bar"

   Note that this is different from the Unix ``dirname`` function.

   :arg path: A string file name.  Note that this string does not have to be
              a valid file name, as the file itself will not be affected.
   :type path: `string`
*/
proc dirname(path: string): string {
  return splitPath(path)[0];
}

/* Expands any environment variables in the path of the form ``$<name>`` or
   ``${<name>}`` into their values.  If ``<name>`` does not exist, they are left
   in place. Returns the path which includes these expansions.

   :arg path: A string representation of a path, which may or may not include
              ``$<name>`` or ``${<name>}``.
   :type path: `string`

   :return: `path`, having replaced all references to environment variables with
            their values.
   :rtype: `string`
*/
 proc expandVars(path: string): string {
  extern proc sys_getenv(name:c_string, ref string_out:c_string):c_int;

   var path_p: string = path;
   var varChars: string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_";
   var res: string = "";
   var ind: int = 0;
   var pathlen: int = path_p.size;
   while (ind < pathlen) {
     var c: string = path_p(ind);
     if (c == "$" && ind + 1 < pathlen) {
       if (path_p(ind+1) == "$") {
         res = res + c;
         ind += 1;
       } else if (path_p(ind+1) == "{") {
         path_p = path_p((ind+2)..);
         pathlen = path_p.numBytes;
         ind = path_p.find("}"):int;
         if (ind == -1) {
           res += "${" +path_p;
           ind = pathlen-1;
         } else {
           var env_var: string = path_p(..(ind-1));
           var value: string;
           var value_c: c_string;
           // buffer received from sys_getenv, shouldn't be freed
           var h: int = sys_getenv(unescape(env_var).c_str(), value_c);
           if (h != 1) {
             value = "${" + env_var + "}";
           } else {
             try! {
               value = string.createCopyingBuffer(value_c,
                                                 policy=decodePolicy.escape);
             }
           }
           res += value;
         }
       } else {
         var env_var: string = "";
         ind += 1;
         while (ind < path_p.size && varChars.find(path_p(ind)) != -1) {
           env_var += path_p(ind);
           ind += 1;
         }
         var value: string;
         var value_c: c_string;
         // buffer received from sys_getenv, shouldn't be freed
         var h: int = sys_getenv(unescape(env_var).c_str(), value_c);
         if (h != 1) {
           value = "$" + env_var;
         } else {
           try! {
             value = string.createCopyingBuffer(value_c,
                                               policy=decodePolicy.escape);
           }
         }
         res += value;
         if (ind < path_p.numBytes) {
           ind -= 1;
         }
       }
     } else {
       res += c;
     }
     ind +=1;
   }
   return res;
 }

/* Determines whether the path specified is an absolute path.

   .. note::

      This is currently only implemented in a Unix environment.  It will not
      behave correctly in a non-Unix environment.

   :arg path: The path to be checked.
   :type path: `string`

   :return: `true` if `name` is an absolute path, `false` otherwise.
   :rtype: `bool`
*/

proc isAbsPath(path: string): bool {
  if path.isEmpty() {
    return false;
  }
  var str: string = path[0];
  if (str == '/') {
    return true;
  } else {
    return false;
  }
}

/* Build up path components as described in joinPath(). */
private proc joinPathComponent(comp: string, ref result: string) {
  if comp.startsWith('/') || result == "" then
    result = comp;
  else if result.endsWith('/') then
    result += comp;
  else
    result += '/' + comp;
}

/* Join and return one or more paths, putting precedent on the last absolute
   path seen.  Return value is the concatenation of the paths with one
   directory separator following each non-empty argument except the last.
   Examples:

   .. code-block:: chapel

      writeln(joinPath("/foo/bar", "/baz")); // Prints "/baz"
      writeln(joinPath("/foo", "./baz")); // Prints "/foo/./baz"
      writeln(joinPath("/foo/", "", "./baz")); // Prints "/foo/./baz"

   :arg paths: Any number of paths.
   :type paths: `string`

   :return: The concatenation of the last absolute path with everything
            following it, or all the paths provided if no absolute path is
            present.
   :rtype: `string`
*/
proc joinPath(paths: string ...?n): string {
  var result: string;

  for path in paths do
    joinPathComponent(path, result);

  return result;
}

/* This overload is private for now, needed for relPath. */
private proc joinPath(paths: [] string): string {
  if paths.isEmpty() then
    return "";

  var result: string;

  for path in paths do
    joinPathComponent(path, result);

  return result;
}

// Normalize leading slash count to a value between 0 and 2.
private proc normalizeLeadingSlashCount(path: string): int {
  var result = if path.startsWith(pathSep) then 1 else 0;

  // Two leading slashes has a special meaning in POSIX.
  if path.startsWith(pathSep * 2) && !path.startsWith(pathSep * 3) then
    result = 2;

  return result;
}

/*
  Normalize a path by eliminating redundant separators and up-level references.
  The paths ``foo//bar``, ``foo/bar/``, ``foo/./bar``, and ``foo/baz/../bar``
  would all be changed to ``foo/bar``.

  .. warning::

    May alter the meaning of paths containing symbolic links.

  .. note::

    Unlike its Python counterpart, this function does not (currently) change
    slashes to backslashes on Windows.

  :arg path: A potential path to collapse, possibly destroying the meaning of
             the path if symbolic links were included.
  :type path: `string`

  :return: The collapsed version of `path`.
  :rtype: `string`
*/
proc normPath(path: string): string {

  // Python 3.7 implementation:
  // https://github.com/python/cpython/blob/3.7/Lib/posixpath.py

  if path == "" then
    return curDir;

  const leadingSlashes = normalizeLeadingSlashCount(path);

  var comps = path.split(pathSep);
  var outComps = new list(string);

  for comp in comps {
    if comp == "" || comp == curDir then
      continue;

    // Second case exists because we cannot go up past the top level.
    // Third case continues a chain of leading up-levels.
    if comp != parentDir || (leadingSlashes == 0 && outComps.isEmpty()) ||
        (!outComps.isEmpty() && outComps[outComps.size-1] == parentDir) then
      outComps.pushBack(comp);
    else if !outComps.isEmpty() then
      try! outComps.popBack();
  }

  var result = pathSep * leadingSlashes + pathSep.join(outComps.these());

  if result == "" then
    return curDir;

  return result;
}

/* Given a path ``path``, attempts to determine the canonical path referenced.
   This resolves and removes any :data:`curDir` and :data:`parentDir` uses
   present, as well as any symbolic links.  Returns the result.

   :arg path: A path to resolve.  If the path does not refer to a valid file
              or directory, an error will occur.
   :type path: `string`

   :return: A canonical version of the argument.
   :rtype: `string`
   :throws SystemError: If one occurs.
*/
proc realPath(path: string): string throws {
  import OS.errorCode;
  extern proc chpl_fs_realpath(path: c_string, ref shortened: c_string): errorCode;

  var res: c_string;
  var err = chpl_fs_realpath(unescape(path).c_str(), res);
  if err then try ioerror(err, "realPath", path);
  const ret = string.createCopyingBuffer(res, policy=decodePolicy.escape);
  // res was qio_malloc'd by chpl_fs_realpath, so free it here
  chpl_free_c_string(res);
  return ret;
}

/* Determines the canonical path referenced by a given :type:`~IO.file` record.
   This resolves and removes any :data:`curDir` and :data:`parentDir` uses
   present, as well as any symbolic links. Returns the result.

   :arg f: A file whose path should be resolved.
   :type f: :type:`~IO.file`

   :return: A canonical path to the file referenced by the given :type:`~IO.file`
            record.  If the :type:`~IO.file` record is not valid, an error will
            occur.
   :rtype: `string`
   :throws SystemError: If one occurs.
*/
proc realPath(f: file): string throws {
  import OS.errorCode;
  extern proc chpl_fs_realpath_file(path: qio_file_ptr_t, ref shortened: c_string): errorCode;

  if (is_c_nil(f._file_internal)) then
    try ioerror(EBADF:errorCode, "in realPath with a file argument");

  var res: c_string;
  var err = chpl_fs_realpath_file(f._file_internal, res);
  if err then try ioerror(err, "in realPath with a file argument");
  return string.createAdoptingBuffer(res);
}

/* Compute the common prefix length between two lists of path components. */
private
proc commonPrefixLength(const a1: [] string, const a2: [] string): int {
  const ref a;
  const ref b;
  if a1.size < a2.size {
    a = a1;
    b = a2;
  } else {
    a = a2;
    b = a1;
  }
  var result = 0;

  for i in 0..<a.size do
    if a[i] != b[i] then
      return result;
    else
      result += 1;

  return result;
}

/*
  Returns a relative filepath to `name` either from the current directory or an
  optional `start` directory. The filesystem is not accessed to verify the
  existence of the named path or the specified starting location.

  .. warning::

    This method is unsafe for use in a parallel environment due to its
    reliance on :proc:`~FileSystem.locale.cwd()`. Another task on the current
    locale may change the current working directory at any time.

  :arg path: A path which the caller would like to access.
  :type path: `string`

  :arg start: The location from which access to path is desired. If no value
    is provided, defaults to :const:`curDir`.
  :type start: `string`

  :return: The relative path to `path` from the current directory.
  :rtype: `string`

  :throws SystemError: Upon failure to get the current working directory.
*/
proc relPath(path: string, start:string=curDir): string throws {
  const realstart = if start == "" then curDir else start;

  // NOTE: Reliance on locale.cwd() can't be avoided.
  const startComps = absPath(realstart).split(pathSep, -1, true);
  const pathComps = absPath(path).split(pathSep, -1, true);

  const prefixLen = commonPrefixLength(startComps, pathComps);

  // Append up-levels until we reach the point where the paths diverge.
  var outComps = new list(string);
  for i in 1..(startComps.size - prefixLen) do
    outComps.pushBack(parentDir);

  // Append the portion of path following the common prefix.
  if !pathComps.isEmpty() then
    for x in pathComps[prefixLen..<pathComps.size] do
      outComps.pushBack(x);

  if outComps.isEmpty() then
    return curDir;

  return joinPath(outComps.toArray());
}

/*
  Returns a relative filepath to the path in a given :type:`~IO.file` either
  from the current directory or an optional `start` directory. The filesystem is
  not accessed to verify the existence of the named path or the specified
  starting location.

  .. warning::

    This method is unsafe for use in a parallel environment due to its
    reliance on :proc:`~FileSystem.locale.cwd()`. Another task on the current
    locale may change the current working directory at any time.

  :arg f: The file
  :type f: :type:`~IO.file`

  :arg start: The location from which access to the path in this
    :type:`~IO.file` is desired. If no value is provided, defaults to
    :const:`curDir`.
  :type start: `string`

  :return: The relative filepath to the path in the given :type:`~IO.file`.
  :rtype: `string`

  :throws SystemError: Upon failure to get the current working directory.
*/
proc relPath(f: file, start:string=curDir): string throws {
  return relPath(f.path, start);
}

/*
  Returns a new path with basename in `path` replaced with `newBasename`.
  If `path` had no basename then `newBasename` is added to the path or if
  the `newBasename` is an empty string then basename is removed from `path`.

  :arg path: A path which the caller would like to access.
  :type path: `string`

  :arg newBasename: A basename to replace the current one
  :type newBasename: `string`

  :returns: a new path after replacing the basename.
  :rtype: `string`
*/
proc replaceBasename(path: string, newBasename: string): string {
    const (dirname, basename) = splitPath(path);
    return joinPath(dirname, newBasename);
}

/*
  Returns a new path with the dirname in `path` replaced with `newDirname`.
  If path had no dirname `newDirname` is added to the `path` or if the
  `newDirname` is an empty string then dirname is removed from the `path`.

  :arg path: A path which the caller would like to access.
  :type path: `string`

  :arg newDirname: dirname to replace the current one
  :type newDirname: `string`

  :returns: The new path after replacing dirname.
  :rtype: `string`
*/
proc replaceDirname(path: string, newDirname: string): string {
    const (dirname, basename) = splitPath(path);
    return joinPath(newDirname, basename);
}

/*
  Returns a new path with extension in `path` replaced with `newExt`.
  If `path` had no extension `newExt` is added to the path or if
  `newExt` is an empty string then extension is removed from the `path`.
  Extension has to be of form `.name`,`name` or it can be an empty
  string and shouldn't contain spaces.

  :arg path: A path which the caller would like to access.
  :type path: `string`

  :arg newExt: extension to replace the current one
  :type newExt: `string`

  :returns: The new path after replacing extension if a valid `newExt`
            is provided.
  :rtype: `string`

  :throws IllegalArgumentError: Upon failure to provide a valid `newExt`
                                or if the `path` had no basename.
*/
proc replaceExt(path: string, newExt: string): string throws {
    const (extLessPath, ext) = splitExt(path);
    const (dirname, basename) = splitPath(extLessPath);

    // Check for empty basename as extension can't be appended
    if  basename.isEmpty() {
      throw new owned IllegalArgumentError(path, "has an empty basename");
    }
    // check if extension contains separator.
    else if newExt.find(pathSep) != -1 {
      throw new owned IllegalArgumentError(newExt, "extension can't contain path separators");
    }
    // if extension is not blank then check it shouldn't end with ''.' and isn't just '.'
    else if newExt == "." || newExt.endsWith(".") {
      throw new owned IllegalArgumentError(newExt, "extension can't end with '.'");
    }
    // remove leading '.' if any for uniform support to both
    const strippedExt = newExt.strip(".", leading=true);
    // check for presence of spaces in strippedExt
    for c in strippedExt {
      if c.isSpace() {
        throw new owned IllegalArgumentError(newExt, "extension can't contain spaces");
      }
    }
    var updatedExt = strippedExt;
    if !strippedExt.isEmpty(){
      updatedExt = "."+strippedExt;
    }
    return replaceBasename(path, basename + updatedExt);
}

/*
  Splits the given path into its root and extension.
  Leading periods in the path are ignored.

  :arg path: A string file name, not necessarily valid.
  :type path: `string`

  :returns: A tuple of the form ``(root, ext)``.
  :rtype: `(string, string)`

*/

proc splitExt(path:string): (string, string) {
  var lastIdx = path.rfind(".");
  var lastSep = path.rfind("/");
  if(lastIdx == -1 || lastSep > lastIdx) {
    return (path, "");
  }
  var idx = lastIdx - 1;
  while(true) {
    if(idx < 0 || path[idx] == "/") {
      return (path, "");
    }
    else if(path[idx] == ".") {
      idx -= 1;
    }
    else break;
  }
  return try! (path[..(lastIdx - 1)], path[lastIdx..]);
}

/* Split path into a tuple that is equivalent to (:proc:`dirname`,
   :proc:`basename`).  The second part of the tuple will never contain a slash.
   Examples:

   .. code-block:: Chapel

      writeln(splitPath("foo/bar")); // Prints "(foo, bar)"
      writeln(splitPath("bar")); // Prints "(, bar)"
      writeln(splitPath("foo/")); // Prints "(foo, )"
      writeln(splitPath("")); // Prints "(, )"
      writeln(splitPath("/")); // Prints "(/, )"

   With the exception of a path of the empty string or just "/", the original
   path can be recreated from this function's returned parts by joining them
   with the path separator character, either explicitly or by calling
   :proc:`joinPath`:

   .. code-block:: Chapel

      var res = splitPath("foo/bar");
      var dirnameVar = res(0);
      var basenameVar = res(1);
      writeln(dirnameVar + "/" + basenameVar); // Prints "foo/bar"
      writeln(joinPath(dirnameVar, basenameVar)); // Prints "foo/bar"

   :arg path: Path to be split.
   :type name: `string`
*/
proc splitPath(path: string): (string, string) {
 var rLoc, lLoc, prev: byteIndex = path.rfind(pathSep);
 if (prev != -1) {
   do {
     prev = lLoc;
     lLoc = path.rfind(pathSep, 0:byteIndex..prev-1);
   } while (lLoc + 1 == prev && lLoc > 0);

   // slice operations here are safe as long as `pathSep` is a 1-byte string.
   try! {
     if (prev == 0) {
       // This happens when the only instance of pathSep in the string is
       // the first character
       return (path[prev..rLoc], path[rLoc+1..]);
     } else if (lLoc == 0 && prev == 1) {
       // This happens when there is a line of pathSep instances at the
       // start of the string
       return (path[..rLoc], path[rLoc+1..]);
     } else if (prev != rLoc) {
       // If prev wasn't the first character, then we want to skip all those
       // duplicate pathSeps
       return (path[..prev-1], path[rLoc+1..]);
     } else {
       // The last instance of pathSep in the string was on its own, so just
       // snip it out.
       return (path[..rLoc-1], path[rLoc+1..]);
     }
   }
 } else {
   return ("", path);
 }
}
} // end module Path
