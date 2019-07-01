/*
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

module ExternalString {

  // For the `c_ptr` type definition.
  use CPtr;

  //
  // Use this as a convenient way to get a handle to "char*" during resolution.
  //
  type chpl__exportTypeCharPtr = c_ptr(c_char);

  //
  // Copy is not the most efficient, because it uses `c_calloc` instead of
  // `c_malloc`. We do this because there is no way to index bytes of a
  // `c_string` directly.
  //
  private proc chpl__exportCopyStringBuffer(s: string): c_ptr(c_char) {
    const bytes = s.length;
    const src = s.c_str():c_void_ptr;
    var dst = c_calloc(c_char, bytes + 1);

    c_memcpy(dst, src, bytes);

    var result = dst:c_ptr(c_char);

    return result;
  }

  //
  // For now, always copy the internal buffer.
  //
  proc chpl__exportConvertToCharPtr(s: string, copy=true): c_ptr(c_char) {
    return chpl__exportCopyStringBuffer(s);
  }

  proc chpl__exportConvertToConstCharPtr(s: string, copy=true): c_string {
    return chpl__exportCopyStringBuffer(s):c_string;
  }

  //
  // If `copy` is false, then the buffer that is being absorbed by the string
  // must have been allocated on the Chapel heap.
  //
  proc chpl__exportConvertToString(c: c_string, copy=true): string {
    return new string(cs=c, isowned=true, needToCopy=copy);
  }

} // End module "ExternalString".

