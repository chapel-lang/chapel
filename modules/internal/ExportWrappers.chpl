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
  use ChapelStandard;
  use CPtr;
  private use SysCTypes;

  // Actual definition is in "runtime/include/chpl-export-wrappers.h".
  extern record chpl_bytes_wrapper {
    var isOwned: int(8);
    var data: c_ptr(c_char);
    var size: uint(64);
  }

  // May need to call this in one of the conversion routines.
  extern proc chpl_bytes_wrapper_free(cb: chpl_bytes_wrapper);

  //
  // TODO: Using type aliases to resolve a type shouldn't be necessary. The
  // compiler should be able to figure this out on its own.
  //
  type chpl__exportTypeCharPtr = c_ptr(c_char);
  type chpl__exportTypeChplBytesWrapper = chpl_bytes_wrapper;

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
    try! {
      return createStringWithBorrowedBuffer(val);
    }
  }

  //
  // TODO: For multilocale, we have to make sure to free the wrapper buffer
  // ourselves after we are done beaming it out over the wire.
  //
  // TODO: Seperate set of conversion calls used in multilocale libraries.
  //
  proc chpl__exportConv(ref val: bytes, type rt: chpl_bytes_wrapper): rt {
    var result: chpl_bytes_wrapper;
    result.isOwned = val.isowned:int(8);
    result.data = val.buff:c_ptr(c_char);
    // Assume ownership of the bytes buffer.
    val.isowned = false;
    result.size = val.size:uint(64);
    return result;
  }

  //
  // For now, all copies passed from Python to the C bridge code are SHALLOW,
  // and then Chapel makes a deep copy from the shallow buffer.
  //
  // TODO: In the multilocale conversion call, we would take ownership of the
  // buffer instead? We would have to allocated a piece of tracked memory on
  // the Chapel heap.
  //
  proc chpl__exportConv(val: chpl_bytes_wrapper, type rt: bytes): rt {
    return createBytesWithNewBuffer(val.data:c_string, val.size.safeCast(int));
  }

} // End module "ExportWrappers".

