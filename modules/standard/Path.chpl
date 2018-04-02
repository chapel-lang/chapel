/*
 * Copyright 2004-2018 Cray Inc.
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

   Path Computations
   -----------------
   :proc:`commonPath`
   :proc:`realPath`
   :proc:`file.realPath`

   Path Manipulations
   ------------------
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

use SysError;
use Sys;

/* Represents generally the current directory */
const curDir = ".";

/* Represents generally the parent directory */
const parentDir = "..";
/* Denotes the separator between a directory and its child. */
const pathSep = "/";

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

 /* Determines and returns the longest common path prefix of
    all the string pathnames provided.

    :arg paths: Any number of paths
    :type paths: `string`

    :return: The longest common path prefix
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

   var prefixArray = firstPath.split(pathSep, -1, false);
   // array of resultant prefix string

   var pos = prefixArray.size;   // rightmost index of common prefix
   var minPathLength = prefixArray.size;

   for i in 2..n do {

     var tempArray = paths(i).split(pathSep, -1, false);
     // temporary array storing the current path under consideration

     var minimum = min(prefixArray.size, tempArray.size);

     if minimum < minPathLength then {
       minPathLength = minimum;
     }

     for itr in 1..minimum do {
       if (tempArray[itr]!=prefixArray[itr] && itr<=pos) {
         pos = itr;
         flag=1;   // indicating that pos was changed
         break;
       }
     }
   }

   if (flag == 1) {
     prefixArray.remove(pos..prefixArray.size);
   } else {
     prefixArray.remove(minPathLength+1..prefixArray.size);
     // in case all paths are subsets of the longest path thus pos was never
     // updated
   }

   result = pathSep.join(prefixArray);

   return result;
 }

 /* Determines and returns the longest common path prefix of
    all the string pathnames provided.

    :arg paths: Any number of paths as an array
    :type paths: `array`

    :return: The longest common path prefix
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

   if firstPath.find("\\", 1..firstPath.length) == 0 then {
     delimiter = "/";
   } else {
     delimiter = "\\";
   }

   var prefixArray = firstPath.split(delimiter, -1, false);
   // array of resultant prefix string

   var pos = prefixArray.size;   // rightmost index of common prefix
   var minPathLength = prefixArray.size;

   for i in (start+1)..end do {

     var tempArray = paths[i].split(delimiter, -1, false);
     // temporary array storing the current path under consideration

     var minimum = min(prefixArray.size, tempArray.size);

     if minimum < minPathLength then {
       minPathLength = minimum;
     }

     for itr in 1..minimum do {
       if (tempArray[itr]!=prefixArray[itr] && itr<=pos) {
         pos = itr;
         flag = 1;   // indicating that pos was changed
         break;
       }
     }
   }

   if (flag == 1) {
     prefixArray.remove(pos..prefixArray.size);
   } else {
     prefixArray.remove(minPathLength+1..prefixArray.size);
     // in case all paths are subsets of the longest path thus pos was never
     // updated
   }

   result = delimiter.join(prefixArray);

   return result;
 }

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

 /*

   Returns the parent directory of the :type:`~IO.file` record.  For instance,
   a file with path `/foo/bar/baz` would return `/foo/bar`

   :return: The parent directory of the file
   :rtype: `string`

 */
 pragma "no doc"
 proc file.getParentName(out error:syserr): string {
   check(error);

   var ret: string = "unknown";
   if !error {
     var tmp: string;
     tmp = this.realPath(error);
     if !error then
       ret = dirname(new string(tmp));
   }
   return ret;
 }

 /*
   Returns the parent directory of the :type:`~IO.file` record.  For instance,
   a file with path `/foo/bar/baz` would return `/foo/bar`

   Will throw an error if one occurs.

   :return: The parent directory of the file
   :rtype: `string`
 */
 proc file.getParentName(): string throws {
   var err: syserr = ENOERR;
   var ret = getParentName(err);
   if err != ENOERR then try ioerror(err, "in file.getParentName");
   return ret;
 }

/* Determines whether the path specified is an absolute path.

   .. note::

      This is currently only implemented in a Unix environment.  It will not
      behave correctly in a non-Unix environment.

   :arg name: the path to be checked.
   :type name: `string`

   :return: `true` if `name` is an absolute path, `false` otherwise
   :rtype: `bool`
*/

  proc isAbsPath(name: string): bool {
    if name.isEmptyString() {
      return false;
    }
    const len: int = name.length;
    var str: string = name[1];
    if (str == '/') {
      return true;
    } else {
      return false;
    }
  }

/* Join and return one or more paths, putting precedent on the last absolute
   path seen.  Return value is the concatenation of the paths with one
   directory separator following each non-empty argument except the last.
   Examples:

   `joinPath("/foo/bar", "/baz")` will yield `"/baz"`

   `joinPath("/foo", "./baz")` will yield `"/foo/./baz"`

   `joinPath("/foo/", "", "./baz")` will also yield `"/foo/./baz"`

   :arg paths: Any number of paths
   :type paths: `string`

   :return: The concatenation of the last absolute path with everything following
            it, or all the paths provided if no absolute path is present
   :rtype: `string`
*/
  proc joinPath(paths: string ...?n): string {
    var result : string = paths(1); // result variable stores final answer
    // loop to iterate over all the paths
    for i in 2..n {
      var temp : string = paths(i);
      if temp.startsWith('/') {
        result = temp;
      }
      else if result.endsWith('/') {
        result = result + temp;
      }
      else {
        result = result + "/" + temp;
      }
    }
   return result;
 }

pragma "no doc"
proc realPath(out error: syserr, name: string): string {
  extern proc chpl_fs_realpath(path: c_string, ref shortened: c_string): syserr;

  var res: c_string;
  error = chpl_fs_realpath(name.localize().c_str(), res);
  return new string(res, needToCopy=false);
}

/* Given a path `name`, attempts to determine the canonical path referenced.
   This resolves and removes any :data:`curDir` and :data:`parentDir` uses
   present, as well as any symbolic links.  Returns the result

   Will throw an error if one occurs.

   :arg name: A path to resolve.  If the path does not refer to a valid file
              or directory, an error will occur.
   :type name: `string`

   :return: A canonical version of the argument.
   :rtype: `string`
*/
proc realPath(name: string): string throws {
  var err: syserr = ENOERR;
  var ret = realPath(err, name);
  if err != ENOERR then try ioerror(err, "realPath", name);
  return ret;
}

pragma "no doc"
proc file.realPath(out error: syserr): string {
  extern proc chpl_fs_realpath_file(path: qio_file_ptr_t, ref shortened: c_string): syserr;

  var res: c_string;

  if (is_c_nil(_file_internal)) {
    // This file is referencing a null file.  We'll get a segfault if we
    // continue.
    error = EBADF;
    return "";
  }
  error = chpl_fs_realpath_file(_file_internal, res);
  return new string(res, needToCopy=false);
}

/* Determines the canonical path referenced by the :type:`~IO.file` record
   performing this operation.  This resolves and removes any :data:`curDir` and
   :data:`parentDir` uses present, as well as any symbolic links.  Returns the
   result

   Will throw an error if one occurs.

   :return: A canonical path to the file referenced by this :type:`~IO.file`
            record.  If the :type:`~IO.file` record is not valid, an error will
            occur
   :rtype: `string`
*/
proc file.realPath(): string throws {
  var err: syserr = ENOERR;
  var ret = realPath(err);
  if err != ENOERR then try ioerror(err, "in file.realPath");
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
   with the path separator character, either explicitly:

   `dirname` + "/" + `basename`

   or by calling :proc:`joinPath`:

   `joinPath(dirname, basename)`

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


/* Expands any environment variables in the path of the form `$<name>` or
   `${<name>}` into their values.  If <name> does not exist, they are left
   in place. Returns the path which includes these expansions.

   :arg path: a string representation of a path, which may or may not include
                   `$<name>` or `${<name>}`.
   :type path: `string`

   :return: `path`, having replaced all references to environment variables with
                their values
   :rtype: `string`
*/
  proc expandVars(path: string): string{
    var path_p:string = path;
    var varChars:string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890_";
    var res:string = "";
    var ind:int = 1;
    var pathlen:int = path_p.length;
    while(ind <= pathlen){
        var c:string = path_p(ind);
        if(c == "$" && ind+1<=pathlen){
            if(path_p(ind+1) == "$"){
                res = res + c;
                ind += 1;
            }
            else if(path_p(ind+1) == "{"){
                path_p = path_p((ind+2)..);
                pathlen = path_p.length;
                ind = path_p.find("}");
                if(ind == 0){
                    res += "${" +path_p;
                    ind = pathlen;
                }
                else{
                    var env_var:string = path_p(..(ind-1));
                    var value:string;
                    var value_c:c_string;
                    var h:int = sys_getenv(env_var.c_str(), value_c);
                    if(h != 1){
                        value = "${" + env_var + "}";
                    }
                    else{
                        value = value_c:string;
                    }
                    res += value;
                }
            }
            else{
                var env_var:string = "";
                ind += 1;
                while(ind <= path_p.length && varChars.find(path_p(ind))!= 0){
                    env_var += path_p(ind);
                    ind += 1;
                }
                var value:string;
                var value_c:c_string;
                var h:int = sys_getenv(env_var.c_str(), value_c);
                if(h != 1){
                    value = "$" + env_var;
                }
                else{
                    value = value_c:string;
                }
                res += value;
                if(ind <= path_p.length){
                    ind -= 1;
                }
            }
        }
        else{
            res += c;
        }
        ind +=1;
    }
    return res;
  }
}

