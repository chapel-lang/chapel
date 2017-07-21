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

// ChapelTaskData.chpl
//
module ChapelTaskData {

  use ChapelStandard;

  // use ChapelError
  // use CPtr

  private extern proc sizeof(type t):size_t;
  private extern proc memcpy(dst:c_ptr, src:c_ptr, n:size_t);

  // Chapel task-local data format:
  // up to 16 bytes of wide pointer for _remoteEndCountType
  // 1 byte for serial_state
  private const chpl_offset_endCount = 0:size_t;
  private const  chpl_offset_serial = sizeof_endcount_ptr();
  private const     chpl_offset_end = chpl_offset_serial+1;

  // workaround to make sure some things are wide
  private var neverExecuted = false;

  // What is the size of a wide _EndCount pointer?
  private
  proc sizeof_endcount_ptr() {
    if _local {
      return sizeof(c_void_ptr);
    } else {
      extern type wide_ptr_t;
      return sizeof(wide_ptr_t);
    }
  }

  // Copy a single wide _EndCount pointer from src to dst
  private
  proc memcpy_endcount_ptr(dst, src)
  {
    if _local {
      memcpy(dst, src, sizeof(c_void_ptr));
    } else {
      extern type wide_ptr_t;
      memcpy(dst, src, sizeof(wide_ptr_t));
    }
  }

  // These functions get/set parts of the Chapel managed
  // task local storage starting from a pointer to the tls region.
  proc chpl_task_data_setDynamicEndCount(tls:c_ptr(chpl_task_ChapelData_t), end: _remoteEndCountType) {
    var prv = tls:c_ptr(c_uchar);
    var i = chpl_offset_endCount;

    var tmpEnd = end;
    // workaround to make tmpEnd always wide
    if neverExecuted {
      on Locales[numLocales-1] do
        tmpEnd = _endCountAlloc(false);
    }

    // Using memcpy to avoid pointer type punning
    memcpy_endcount_ptr(c_ptrTo(prv[i]), c_ptrTo(tmpEnd));
  }
  proc chpl_task_data_getDynamicEndCount(tls:c_ptr(chpl_task_ChapelData_t)) {
    var prv = tls:c_ptr(c_uchar);
    var i = chpl_offset_endCount;

    var ret:_remoteEndCountType = nil;
    // workaround to make ret always wide
    if neverExecuted {
      on Locales[numLocales-1] do
        ret = _endCountAlloc(false);
    }

    // Using memcpy to avoid pointer type punning
    memcpy_endcount_ptr(c_ptrTo(ret), c_ptrTo(prv[i]));

    return ret;
  }

  proc chpl_task_data_setSerial(tls:c_ptr(chpl_task_ChapelData_t), makeSerial: bool) : void {
    var prv = tls:c_ptr(c_uchar);
    var i = chpl_offset_serial;
    var v:uint(8) = 0;
    if makeSerial then
      v = 1;
    // Using memcpy to avoid pointer type punning
    memcpy(c_ptrTo(prv[i]), c_ptrTo(v), sizeof(uint(8)));
  }
  proc chpl_task_data_getSerial(tls:c_ptr(chpl_task_ChapelData_t)) : bool {
    var ret:bool = false;
    var prv = tls:c_ptr(c_uchar);
    var i = chpl_offset_serial;
    var v:uint(8) = 0;
    // Using memcpy to avoid pointer type punning
    memcpy(c_ptrTo(v), c_ptrTo(prv[i]), sizeof(uint(8)));
    // check we got 1 or 0 if bounds checking is on
    // (to detect runtime implementation errors where this part of
    //  the argument bundle is stack trash)
    if boundsChecking then
      assert(v == 0 || v == 1);
    return v == 1;
  }


  // These functions are like the above but first get the pointer
  // to the task local storage region for the currently executing task.
  // Note, these can be called before module initialization is complete.

  proc chpl_task_setDynamicEndCount(end: _remoteEndCountType) {
    chpl_task_data_setDynamicEndCount(chpl_task_getChapelData(), end);
  }
  proc chpl_task_getDynamicEndCount() : _remoteEndCountType {
    return chpl_task_data_getDynamicEndCount(chpl_task_getChapelData());
  }

  export proc chpl_task_setSerial(makeSerial: bool) : void {
    chpl_task_data_setSerial(chpl_task_getChapelData(), makeSerial);
  }
  export proc chpl_task_getSerial() : bool {
    return chpl_task_data_getSerial(chpl_task_getChapelData());
  }


  // module init function - check sizes
  {
    assert(chpl_offset_end <= sizeof(chpl_task_ChapelData_t));
  }
}
