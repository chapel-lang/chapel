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

  // Actual definition is in "runtime/include/chpl-export-wrappers.h".
  extern record chpl_bytes {
    var isOwned: c_int;
    var data: c_ptr(c_char);
    var size: size_t;
  }

  // May need to call this in one of the conversion routines.
  extern proc chpl_bytes_free(cb: chpl_bytes);

  //
  // TODO: Using type aliases to resolve a type shouldn't be necessary. The
  // compiler should be able to figure this out on its own.
  //
  type chpl__exportTypeCharPtr = c_ptr(c_char);
  type chpl__exportTypeChplBytes = chpl_bytes;

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
  // TODO: Seperate set of conversion calls used in multilocale libraries.
  //
  proc chpl__exportConv(ref val: bytes, type rt: chpl_bytes): rt {
    var result: chpl_bytes;
    result.isOwned = val.isowned:c_int;
    result.data = val.buff:c_ptr(c_char);
    // Assume ownership of the bytes buffer.
    val.isowned = false;
    result.size = val.size:size_t;
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
  proc chpl__exportConv(val: chpl_bytes, type rt: bytes): rt {
    use ByteBufferHelpers;
    var data = val.data:ByteBufferHelpers.bufferType;
    // TODO: Are these casts safe casts?
    var length = val.size:int(64);
    var buflen = (val.size + 1):int(64);
    var result = createBytesWithNewBuffer(data, length=length, size=buflen);
    return result;
  }

} // End module "ExportWrappers".

