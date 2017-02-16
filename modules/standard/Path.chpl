/*
 * Copyright 2004-2017 Cray Inc.
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

      This module is currently under development and will expand significantly
      in upcoming releases.  Stay tuned!

   Operations which occur on the files or directories referred to by these paths
   may be found in :mod:`FileSystem` (for operations *on* the file) or :mod:`IO`
   (for operations *within* the file).
*/
module Path {

use SysError;

/* Returns the basename of the file name provided.  For instance, in the
   name `/foo/bar/baz`, this function would return `baz`, while `/foo/bar/`
   would yield the empty string.  Note that this is different from the Unix
   basename function.

   :arg name: a string file name.  Note that this string does not have to be
              a valid file name, as the file itself will not be affected.
   :type name: `string`
*/
 proc basename(name: string): string {
   return splitPath(name)[2];
 }

/* Represents generally the current directory */
const curDir = ".";

/* Returns the parent directory of the file name provided.  For instance,
   in the name `/foo/bar/baz`, this function would return `/foo/bar`, as
   would a call with `/foo/bar/` as the argument.

   :arg name: a string file name.  Note that this string does not have to be
              a valid file name, as the file itself will not be affected.
   :type name: `string`
*/
 proc dirname(name: string): string{
   return splitPath(name)[1];
 }

/* Represents generally the parent directory */
const parentDir = "..";
/* Denotes the separator between a directory and its child. */
const pathSep = "/";

pragma "no doc"
proc realPath(out error: syserr, name: string): string {
  extern proc chpl_fs_realpath(path: c_string, ref shortened: c_string_copy): syserr;

  var res: c_string_copy;
  error = chpl_fs_realpath(name.localize().c_str(), res);
  var len = res.length;
  return new string(res:c_ptr(uint(8)), len, len+1, owned=true, needToCopy=false);
}

/* Given a path `name`, attempts to determine the canonical path referenced.
   This resolves and removes any :data:`curDir` and :data:`parentDir` uses
   present, as well as any symbolic links.  Returns the result

   Will halt with an error message if one is detected.

   :arg name: A path to resolve.  If the path does not refer to a valid file
              or directory, an error will occur.
   :type name: `string`

   :return: A canonical version of the argument.
   :rtype: `string`
*/
proc realPath(name: string): string {
  var err: syserr = ENOERR;
  var ret = realPath(err, name);
  if err != ENOERR then ioerror(err, "in realPath of", name);
  return ret;
}

pragma "no doc"
proc file.realPath(out error: syserr): string {
  extern proc chpl_fs_realpath_file(path: qio_file_ptr_t, ref shortened: c_string_copy): syserr;

  var res: c_string_copy;

  if (is_c_nil(_file_internal)) {
    // This file is referencing a null file.  We'll get a segfault if we
    // continue.
    error = EBADF;
    return "";
  }
  error = chpl_fs_realpath_file(_file_internal, res);
  var len = res.length;
  return new string(res:c_ptr(uint(8)), len, len+1, owned=true, needToCopy=false);
}

/* Determines the canonical path referenced by the :type:`~IO.file` record
   performing this operation.  This resolves and removes any :data:`curDir` and
   :data:`parentDir` uses present, as well as any symbolic links.  Returns the
   result

   Will halt with an error message if one is detected.

   :return: A canonical path to the file referenced by this :type:`~IO.file`
            record.  If the :type:`~IO.file` record is not valid, an error will
            occur
   :rtype: `string`
*/
proc file.realPath(): string {
  var err: syserr = ENOERR;
  var ret = realPath(err);
  if err != ENOERR then ioerror(err, "in file.realPath");
  return ret;
}

/* Split name into a tuple that is equivalent to (:proc:`dirname`,
   :proc:`basename`).  The second part of the tuple will never contain a slash.
   Examples:

   `splitPath("foo/bar")` will yield `("foo", "bar")`

   `splitPath("bar")` will yield `("", "bar")`

   `splitPath("foo/")` will yield `("foo", "")`

   `splitPath("")` will yield `("", "")`

   `splitPath("/")` will yield `("/", "")`

   With the exception of a path of the empty string or just "/", the original
   path can be recreated from this function's returned parts by joining them
   with the path separator character:

   `dirname` + "/" + `basename`

   :arg name: path to be split
   :type name: `string`
*/
 proc splitPath(name: string): (string, string) {
   var rLoc, lLoc, prev: int = name.rfind(pathSep);
   if (prev != 0) {
     do {
       prev = lLoc;
       lLoc = name.rfind(pathSep, 1..prev-1);
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
