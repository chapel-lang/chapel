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
  // In the multilocale conversion call, we would take ownership of the buffer
  // instead?
  //
  proc chpl__exportConv(val: chpl_bytes, type rt: bytes): rt {
    use ByteBufferHelpers;
    var data = val.data:ByteBufferHelpers.bufferType;
    // TODO: Use safe casts with overflow check?
    var length = val.size:int(64);
    var buflen = (val.size + 1):int(64);
    var result = createBytesWithNewBuffer(data, length=length, size=buflen);
    return result;
  }

} // End module "ExternalString".

