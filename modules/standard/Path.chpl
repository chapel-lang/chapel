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

/* A file utilities library, specifically related to path operations

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
   :proc:`file.realPath`
   :proc:`relPath`
   :proc:`file.relPath`

   Path Manipulations
   ------------------
   :proc:`absPath`
   :proc:`file.absPath`
   :proc:`expandVars`
   :proc:`joinPath`
   :proc:`splitPath`

   Path Properties
   ---------------
   :proc:`basename`
   :proc:`dirname`
   :proc:`file.getParentName`
   :proc:`isAbsPath`

   Constant and Function Definitions
   ---------------------------------
*/
module Path {

private use List;
use SysError, IO;
private use Sys;

/* Represents generally the current directory. This starts as the directory
   where the program is being executed from.
*/
const curDir = ".";
/* Represents generally the parent directory. */
const parentDir = "..";
/* Denotes the separator between a directory and its child. */
const pathSep = "/";

/*
  Creates a normalized absolutized version of a path. On most platforms, when
  given a non-absolute path this function is equivalent to the following code:

  .. code-block:: Chapel
  
    normPath(joinPath(here.cwd(), name))
  
  See :proc:`normPath()`, :proc:`joinPath()`, :proc:`~FileSystem.locale.cwd()`
  for details.
    
  .. warning::

    This function is unsafe for use in a parallel environment due to its
    reliance on :proc:`~FileSystem.locale.cwd()`. Another task on the current
    locale may change the current working directory at any time.

  :arg name: The path whose absolute path is desired.
  :type name: `string`

  :return: A normalized, absolutized version of the path specified.
  :rtype: `string`

  :throws SystemError: Upon failure to get the current working directory.
*/
proc absPath(name: string): string throws {
  use FileSystem;

  if !isAbsPath(name) then
    return normPath(joinPath(try here.cwd(), name));
  return normPath(name);
}

/*
  Creates a normalized absolutized version of the path in this
  :type:`~IO.file`. On most platforms, when given a non-absolute path this
  function is equivalent to the following code:
  
  .. code-block:: Chapel
  
      normPath(joinPath(here.cwd(), file.path))
      
  See :proc:`normPath()`, :proc:`joinPath()`, :proc:`~FileSystem.locale.cwd()`,
  :proc:`~IO.file.path` for details.

  .. warning::

    This method is unsafe for use in a parallel environment due to its
    reliance on :proc:`~FileSystem.locale.cwd()`. Another task on the current
    locale may change the current working directory at any time.

  :return: A normalized, absolutized version of the path for this file.
  :rtype: `string`

  :throws SystemError: Upon failure to get the current working directory.
*/
proc file.absPath(): string throws {
  // If we don't use the namespace we get a funky compiler type error.
  return try Path.absPath(this.path);
}

/* Returns the basename of the file name provided.  For instance:

   .. code-block:: Chapel

      writeln(basename("/foo/bar/baz")); // Prints "baz"
      writeln(basename("/foo/bar/")); // Prints "", because of the empty string

   Note that this is different from the Unix basename function.

   :arg name: A string file name.  Note that this string does not have to be
              a valid file name, as the file itself will not be affected.
   :type name: `string`
*/
proc basename(name: string): string {
   return splitPath(name)[2];
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
  var firstPath = paths(1);
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
    prefixList.append(x);

  var pos = prefixList.size;   // rightmost index of common prefix
  var minPathLength = prefixList.size;

  for i in 2..n do {

    var tempList = new list(string);
    for x in paths(i).split(pathSep, -1, false) do
      tempList.append(x);

    var minimum = min(prefixList.size, tempList.size);

    if minimum < minPathLength then {
      minPathLength = minimum;
    }

    for itr in 1..minimum do {
      if (tempList[itr]!=prefixList[itr] && itr<=pos) {
        pos = itr;
        flag=1;   // indicating that pos was changed
        break;
      }
    }
  }

  if (flag == 1) {
    for i in pos..prefixList.size by -1 do
      try! prefixList.pop(i);
  } else {
    for i in (minPathLength + 1)..prefixList.size by -1 do
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

  if firstPath.find("\\") == 0 then {
    delimiter = "/";
  } else {
    delimiter = "\\";
  }

  var prefixList = new list(string);
  for x in firstPath.split(delimiter, -1, false) do
    prefixList.append(x);
  // array of resultant prefix string

  var pos = prefixList.size;   // rightmost index of common prefix
  var minPathLength = prefixList.size;

  for i in (start+1)..end do {

    var tempList = new list(string);
    for x in paths[i].split(delimiter, -1, false) do
      tempList.append(x);
    // temporary array storing the current path under consideration

    var minimum = min(prefixList.size, tempList.size);

    if minimum < minPathLength then {
      minPathLength = minimum;
    }

    for itr in 1..minimum do {
      if (tempList[itr]!=prefixList[itr] && itr<=pos) {
        pos = itr;
        flag = 1;   // indicating that pos was changed
        break;
      }
    }
  }

  if (flag == 1) {
    for i in pos..prefixList.size by -1 do
      try! prefixList.pop(i);
  } else {
    for i in (minPathLength + 1)..prefixList.size by -1 do
      try! prefixList.pop(i);
    // in case all paths are subsets of the longest path thus pos was never
    // updated
  }

  result = delimiter.join(prefixList.these());
  return result;
}

/* Returns the parent directory of the file name provided.  For instance:

   .. code-block:: Chapel

      writeln(dirname("/foo/bar/baz")); // Prints "/foo/bar"
      writeln(dirname("/foo/bar/")); // Also prints "/foo/bar"

   :arg name: A string file name.  Note that this string does not have to be
              a valid file name, as the file itself will not be affected.
   :type name: `string`
*/
proc dirname(name: string): string {
  return splitPath(name)[1];
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
   var path_p: string = path;
   var varChars: string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_";
   var res: string = "";
   var ind: byteIndex = 1;
   var pathlen: int = path_p.numBytes;
   while (ind <= pathlen) {
     var c: string = path_p(ind);
     if (c == "$" && ind + 1 <= pathlen) {
       if (path_p(ind+1) == "$") {
         res = res + c;
         ind += 1;
       } else if (path_p(ind+1) == "{") {
         path_p = path_p((ind+2)..);
         pathlen = path_p.numBytes;
         ind = path_p.find("}");
         if (ind == 0) {
           res += "${" +path_p;
           ind = pathlen;
         } else {
           var env_var: string = path_p(..(ind-1));
           var value: string;
           var value_c: c_string;
           var h: int = sys_getenv(env_var.c_str(), value_c);
           if (h != 1) {
             value = "${" + env_var + "}";
           } else {
             try! {
               value = createStringWithNewBuffer(value_c);
             }
           }
           res += value;
         }
       } else {
         var env_var: string = "";
         ind += 1;
         while (ind <= path_p.numBytes && varChars.find(path_p(ind)) != 0) {
           env_var += path_p(ind);
           ind += 1;
         }
         var value: string;
         var value_c: c_string;
         var h: int = sys_getenv(env_var.c_str(), value_c);
         if (h != 1) {
           value = "$" + env_var;
         } else {
           try! {
             value = createStringWithNewBuffer(value_c);
           }
         }
         res += value;
         if (ind <= path_p.numBytes) {
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

/*
  Returns the parent directory of the :type:`~IO.file` record.  For instance:

  .. code-block:: Chapel

     var myFile = open("/foo/bar/baz.txt", iomode.r);
     writeln(myFile.getParentName()); // Prints "/foo/bar"

  Will throw a SystemError if one occurs.

  :return: The parent directory of the file.
  :rtype: `string`
*/
proc file.getParentName(): string throws {
  try check();

  try {
    return dirname(createStringWithNewBuffer(this.realPath()));
  } catch {
    return "unknown";
  }
}

/* Determines whether the path specified is an absolute path.

   .. note::

      This is currently only implemented in a Unix environment.  It will not
      behave correctly in a non-Unix environment.

   :arg name: The path to be checked.
   :type name: `string`

   :return: `true` if `name` is an absolute path, `false` otherwise.
   :rtype: `bool`
*/

proc isAbsPath(name: string): bool {
  if name.isEmpty() {
    return false;
  }
  var str: string = name[1];
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
private proc normalizeLeadingSlashCount(name: string): int {
  var result = if name.startsWith(pathSep) then 1 else 0;

  // Two leading slashes has a special meaning in POSIX.
  if name.startsWith(pathSep * 2) && !name.startsWith(pathSep * 3) then
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

  :arg name: A potential path to collapse, possibly destroying the meaning of
             the path if symbolic links were included.
  :type name: `string`

  :return: The collapsed version of `name`.
  :rtype: `string`
*/
proc normPath(name: string): string {
  
  // Python 3.7 implementation:
  // https://github.com/python/cpython/blob/3.7/Lib/posixpath.py

  if name == "" then
    return curDir;

  const leadingSlashes = normalizeLeadingSlashCount(name);

  var comps = name.split(pathSep);
  var outComps = new list(string);

  for comp in comps {
    if comp == "" || comp == curDir then
      continue;

    // Second case exists because we cannot go up past the top level.
    // Third case continues a chain of leading up-levels.
    if comp != parentDir || (leadingSlashes == 0 && outComps.isEmpty()) ||
        (!outComps.isEmpty() && outComps[outComps.size] == parentDir) then
      outComps.append(comp);
    else if !outComps.isEmpty() then
      try! outComps.pop();
  }

  var result = pathSep * leadingSlashes + pathSep.join(outComps.these());

  if result == "" then
    return curDir;

  return result;
}

/* Given a path ``name``, attempts to determine the canonical path referenced.
   This resolves and removes any :data:`curDir` and :data:`parentDir` uses
   present, as well as any symbolic links.  Returns the result.

   Will throw a SystemError if one occurs.

   :arg name: A path to resolve.  If the path does not refer to a valid file
              or directory, an error will occur.
   :type name: `string`

   :return: A canonical version of the argument.
   :rtype: `string`
*/
proc realPath(name: string): string throws {
  extern proc chpl_fs_realpath(path: c_string, ref shortened: c_string): syserr;

  var res: c_string;
  var err = chpl_fs_realpath(name.localize().c_str(), res);
  if err then try ioerror(err, "realPath", name);
  return createStringWithOwnedBuffer(res);
}

pragma "no doc"
proc realPath(out error: syserr, name: string): string {
  compilerWarning("This version of realPath() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return realPath(name);
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return "";
}

/* Determines the canonical path referenced by the :type:`~IO.file` record
   performing this operation.  This resolves and removes any :data:`curDir` and
   :data:`parentDir` uses present, as well as any symbolic links.  Returns the
   result.

   Will throw a SystemError if one occurs.

   :return: A canonical path to the file referenced by this :type:`~IO.file`
            record.  If the :type:`~IO.file` record is not valid, an error will
            occur.
   :rtype: `string`
*/
proc file.realPath(): string throws {
  extern proc chpl_fs_realpath_file(path: qio_file_ptr_t, ref shortened: c_string): syserr;

  if (is_c_nil(_file_internal)) then
    try ioerror(EBADF:syserr, "in file.realPath");

  var res: c_string;
  var err = chpl_fs_realpath_file(_file_internal, res);
  if err then try ioerror(err, "in file.realPath");
  return createStringWithOwnedBuffer(res);
}

pragma "no doc"
proc file.realPath(out error: syserr): string {
  compilerWarning("This version of realPath() is deprecated; " +
                  "please switch to a throwing version");
  try {
    return realPath();
  } catch e: SystemError {
    error = e.err;
  } catch {
    error = EINVAL;
  }
  return "";
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

  for i in 1..a.size do
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

  :arg name: A path which the caller would like to access.
  :type name: `string`

  :arg start: The location from which access to name is desired. If no value
    is provided, defaults to :const:`curDir`.
  :type start: `string`

  :return: The relative path to `name` from the current directory.
  :rtype: `string`

  :throws SystemError: Upon failure to get the current working directory.
*/
proc relPath(name: string, start:string=curDir): string throws {
  const realstart = if start == "" then curDir else start;

  // NOTE: Reliance on locale.cwd() can't be avoided.
  const startComps = absPath(realstart).split(pathSep, -1, true);
  const nameComps = absPath(name).split(pathSep, -1, true);

  const prefixLen = commonPrefixLength(startComps, nameComps);

  // Append up-levels until we reach the point where the paths diverge.
  var outComps = new list(string);
  for i in 1..(startComps.size - prefixLen) do
    outComps.append(parentDir);

  // Append the portion of name following the common prefix.
  if !nameComps.isEmpty() then
    for x in nameComps[(prefixLen + 1)..nameComps.size] do
      outComps.append(x);

  if outComps.isEmpty() then
    return curDir;

  return joinPath(outComps.toArray());
}

/*
  Returns a relative filepath to the path in this :type:`~IO.file` either from
  the current directory or an optional `start` directory. The filesystem is not
  accessed to verify the existence of the named path or the specified starting
  location.

  .. warning::

    This method is unsafe for use in a parallel environment due to its
    reliance on :proc:`~FileSystem.locale.cwd()`. Another task on the current
    locale may change the current working directory at any time.

  :arg start: The location from which access to the path in this
    :type:`~IO.file` is desired. If no value is provided, defaults to
    :const:`curDir`.
  :type start: `string`

  :return: The relative filepath to the path in this :type:`~IO.file`.
  :rtype: `string`

  :throws SystemError: Upon failure to get the current working directory.
*/
proc file.relPath(start:string=curDir): string throws {
  use Path only;
  // Have to prefix module name to avoid muddying name resolution.
  return Path.relPath(this.path, start);
}

/* Split name into a tuple that is equivalent to (:proc:`dirname`,
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
      var dirnameVar = res(1);
      var basenameVar = res(2);
      writeln(dirnameVar + "/" + basenameVar); // Prints "foo/bar"
      writeln(joinPath(dirnameVar, basenameVar)); // Prints "foo/bar"

   :arg name: Path to be split.
   :type name: `string`
*/
 proc splitPath(name: string): (string, string) {
   var rLoc, lLoc, prev: byteIndex = name.rfind(pathSep);
   if (prev != 0) {
     do {
       prev = lLoc;
       lLoc = name.rfind(pathSep, 1:byteIndex..prev-1);
     } while (lLoc + 1 == prev && lLoc > 1);

     if (prev == 1) {
       // This happens when the only instance of pathSep in the string is
       // the first character
       return (name[prev..rLoc], name[rLoc+1..]);
     } else if (lLoc == 1 && prev == 2) {
       // This happens when there is a line of pathSep instances at the
       // start of the string
       return (name[..rLoc], name[rLoc+1..]);
     } else if (prev != rLoc) {
       // If prev wasn't the first character, then we want to skip all those
       // duplicate pathSeps
       return (name[..prev-1], name[rLoc+1..]);
     } else {
       // The last instance of pathSep in the string was on its own, so just
       // snip it out.
       return (name[..rLoc-1], name[rLoc+1..]);
     }
   } else {
     return ("", name);
   }
 }
}
