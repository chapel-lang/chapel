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

module ExportWrappers {
  use CPtr;

  // Actual definition is in "runtime/include/chpl-export-wrappers.h".
  extern record chpl_bytes {
    var isOwned: c_int;
    var data: c_ptr(c_char);
    var len: size_t;
  }

  export proc chpl_bytes_free(cb: chpl_bytes) {
    if cb.isOwned:bool && cb.data != nil then
      chpl_here_free(cb.data);
    return;
  }

  //
  // TODO: Using type aliases to resolve a type shouldn't be necessary. The
  // compiler should be able to figure this out on its own.
  //
  type chpl__exportTypeCharPtr = c_ptr(c_char);
  type chpl__exportTypeChplBytes = chpl_bytes;

  //
  // TODO: Currently, we return strings in C as a `char*`. Since there is no
  // way to encode any additional information about the buffer, we are
  // forced to always allocate a new string every time. Presently, this takes
  // the form of always copying the string.
  // If the string owns its buffer, then we can assume control and set the
  // string `isowned` to false - however before we do that we have to verify
  // that `chpl_free` plays nice with buffers allocated by `chpl_here_alloc`.
  //
  private proc chpl__exportCopyStringBuffer(s: string): c_ptr(c_char) {
    const nBytes = s.numBytes;
    const src = s.c_str():c_void_ptr;
    var result = c_malloc(c_char, nBytes + 1):c_ptr(c_char);
    c_memcpy(result, src, nBytes);
    result[nBytes] = 0;
    return result;
  }

  proc chpl__exportConv(val: string, type rt: c_ptr(c_char)): rt {
    return chpl__exportCopyStringBuffer(val);
  }

  proc chpl__exportConv(val: c_string, type rt: string): rt {
    return createStringWithBorrowedBuffer(val);
  }

  //
  // TODO: For multilocale, we have to make sure to free the `chpl_bytes`
  // buffer ourselves after we are done beaming it out over the wire.
  //
  proc chpl__exportConv(ref val: bytes, type rt: chpl_bytes): rt {
    var result: chpl_bytes;
    result.isOwned = val.isowned:c_int;
    result.data = val.buff:c_ptr(c_char);
    // Assume ownership of the bytes buffer.
    val.isowned = false;
    result.len = val.size:size_t;
    return result;
  }

  proc chpl__exportConv(val: chpl_bytes, type rt: bytes): rt {
    halt("Routine not implemented yet!");
  }

  //
  // This is meant to be called on Chapel strings that are being returned from
  // an exported Chapel function. It makes no assumptions about the current
  // state of the returned string (whether it owns its buffer or not), and
  // does a deep copy of the buffer.
  // TODO: This could only conditionally copy the buffer if the input string
  // has `isowned=false`, and cannibalize the string formal's buffer
  // otherwise, to reduce copying.
  // TODO: Adjust multi-locale interop code to account for this copy, and
  // make sure to deallocate buffers for `c_string` formals that originally
  // mapped to Chapel strings.
  //
  proc chpl__exportStringToConstCharPtr(s: string): c_string {
    return chpl__exportCopyStringBuffer(s):c_string;
  }

  //
  // Ditto the above.
  //
  proc chpl__exportStringToCharPtr(s: string): c_ptr(c_char) {
    return chpl__exportCopyStringBuffer(s);
  }

  //
  // This is meant to be called on Chapel string formals in exported Chapel
  // functions. Passed in `c_string` are considered to be _read only_, and
  // thus the created string has `isowned=false` and `needToCopy=false`.
  // TODO: The string documentation warns to avoid any in-place operations
  // when "needToCopy=false". This makes me think that there are inplace
  // operations that can occur on a string without copying?
  // In the worst case, we will have to document that strings constructed
  // this way are read only, _or_ perform a full copy for formals as well.
  //
  proc chpl__exportConstCharPtrToString(c: c_string): string {
    return createStringWithBorrowedBuffer(c);
  }

  //
  // Ditto the above.
  //
  proc chpl__exportCharPtrToString(c: c_ptr(c_char)): string {
    return createStringWithBorrowedBuffer(c);
  }

} // End module "ExternalString".

