/*
 * Copyright 2020 Hewlett Packard Enterprise Development LP
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

pragma "no doc"
module ExportWrappers {
  use ChapelStandard;
  use CPtr;
  use SysCTypes;

  // Actual definition is in "runtime/include/chpl-export-wrappers.h".
  pragma "export wrapper"
  extern record chpl_byte_buffer {
    var isOwned: int(8);
    var data: c_ptr(c_char);
    var size: uint(64);
  }

  extern proc chpl_byte_buffer_free(cb: chpl_byte_buffer);

  //
  // TODO: Using type aliases to resolve a type shouldn't be necessary. The
  // compiler should be able to figure this out on its own.
  //
  type chpl__exportTypeChplByteBuffer = chpl_byte_buffer;

  // Generic, but both string and bytes have the same implementation.
  proc chpl__exportRetStringOrBytes(ref val): chpl_byte_buffer {
    var result: chpl_byte_buffer;
    result.isOwned = val.isOwned:int(8);
    result.data = val.buff:c_ptr(c_char);
    // Get the length of the string/bytes record in bytes!
    result.size = val.numBytes:uint(64);
    // Assume ownership of the string/bytes record's internal buffer.
    val.isOwned = false;
    return result;
  }

  proc chpl__exportRet(ref val: string, type rt: chpl_byte_buffer): rt {
    return chpl__exportRetStringOrBytes(val); 
  }

  proc chpl__exportRet(ref val: bytes, type rt: chpl_byte_buffer): rt {
    return chpl__exportRetStringOrBytes(val);
  }

  proc chpl__exportArg(cp: bool, val: chpl_byte_buffer, type rt: string): rt {
    var data = val.data:c_string;
    var size = val.size.safeCast(int);
    try! {
      if cp then return createStringWithNewBuffer(data, size);
      return createStringWithBorrowedBuffer(data, size);
    }
  }

  proc chpl__exportArg(cp: bool, val: chpl_byte_buffer, type rt: bytes): rt {
    var data = val.data:c_string;
    var size = val.size.safeCast(int);
    try! {
      if cp then return createBytesWithNewBuffer(data, size);
      return createBytesWithBorrowedBuffer(data, size);
    }
  }

} // End module "ExportWrappers".

