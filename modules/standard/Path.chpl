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

/* Returns the canonical path specified, eliminating symbolic links.  If an
   error occurred, it will be returned via the out parameter.
   err: a syserr used to indicate if an error occurred during this operation.
   name: a string representing a valid path.
*/
proc realPath(out error: syserr, name: string): string {
  extern proc chpl_fs_realpath(path: c_string, ref shortened: c_string_copy): syserr;

  var res: c_string_copy;
  error = chpl_fs_realpath(name.c_str(), res);
  return toString(res);
}

/* Returns the canonical path specified, eliminating symbolic links.  May
   halt with an error message.
   name: a string representing a valid path.
*/
proc realPath(name: string): string {
  var err: syserr = ENOERR;
  var ret = realPath(err, name);
  if err != ENOERR then ioerror(err, "in realPath of", name);
  return ret;
}

/* Returns the canonical path referred to by the file record, eliminating
   symbolic links.  If an error occurred, it will be returned via the out
   parameter.
   err: a syserr used to indicate if an error occurred during this operation.
*/
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
  return toString(res);
}

/* Returns the canonical path referred to by the file record, eliminating
   symbolic links.
*/
proc file.realPath(): string {
  var err: syserr = ENOERR;
  var ret = realPath(err);
  if err != ENOERR then ioerror(err, "in file.realPath");
  return ret;
}
