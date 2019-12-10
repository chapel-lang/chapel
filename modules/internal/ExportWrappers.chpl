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
  type chpl__exportTypeChplBytesWrapper = chpl_byte_buffer;

  // Generic, but both string and bytes have the same implementation.
  proc chpl__exportRetStringOrBytes(ref val): chpl_byte_buffer {
    var result: chpl_byte_buffer;
    result.isOwned = val.isowned:int(8);
    result.data = val.buff:c_ptr(c_char);
    result.size = val.size:uint(64);
    // Assume ownership of the string/bytes record's internal buffer.
    val.isowned = false;
    result.size = val.size:uint(64);
    return result;
  }

  proc chpl__exportRet(ref val: string, type rt: chpl_byte_buffer): rt {
    return chpl__exportRetStringOrBytes(val); 
  }

  proc chpl__exportRet(ref val: bytes, type rt: chpl_byte_buffer): rt {
    return chpl__exportRetStringOrBytes(val);
  }

  // TODO: Identify where it is appropriate to make shallow copies.
  proc chpl__exportArg(val: chpl_byte_buffer, type rt: string): rt {
    var data = val.data:c_string;
    var size = val.size.safeCast(int);
    return createStringWithNewBuffer(data, size);
  }

  // TODO: Identify where it is appropriate to make shallow copies.
  proc chpl__exportArg(val: chpl_byte_buffer, type rt: bytes): rt {
    var data = val.data:c_string;
    var size = val.size.safeCast(int);
    return createBytesWithNewBuffer(data, size);
  }

} // End module "ExportWrappers".

