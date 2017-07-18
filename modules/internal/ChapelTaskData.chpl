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

  // Chapel task-local data format:
  // 16 bytes of wide pointer for _remoteEndCountType
  // 1 byte of bool for serial_state
  private const chpl_offset_endCount = 0:size_t;
  private const  chpl_offset_serial = 16:size_t;
  private const     chpl_offset_end = chpl_offset_serial+sizeof(bool);

  // workaround to make sure some things are wide
  private var neverExecuted = false;

  // These functions get/set parts of the Chapel managed
  // task local storage starting from a pointer to the tls region.
  proc chpl_task_data_setDynamicEndCount(tls:c_ptr(chpl_task_ChapelData_t),in end: _remoteEndCountType) {
    var prv = tls:c_ptr(c_uchar);
    var i = chpl_offset_endCount;
    // Using memcpy to avoid pointer type punning
    // using 16 to work around problems with sizeof(wideptrtype)
    c_memcpy(c_ptrTo(prv[i]), c_ptrTo(end), 16);

    // workaround to make end always wide
    if neverExecuted {
      on Locales[numLocales-1] do
        end = _endCountAlloc(false);
    }
  }
  proc chpl_task_data_getDynamicEndCount(tls:c_ptr(chpl_task_ChapelData_t)) {
    var ret:_remoteEndCountType = nil;
    var prv = tls:c_ptr(c_uchar);
    var i = chpl_offset_endCount;
    // Using memcpy to avoid pointer type punning
    c_memcpy(c_ptrTo(ret), c_ptrTo(prv[i]), 16);
    
    // workaround to make ret always wide
    if neverExecuted {
      on Locales[numLocales-1] do
        ret = _endCountAlloc(false);
    }

    return ret;
  }

  proc chpl_task_data_setSerial(tls:c_ptr(chpl_task_ChapelData_t), in makeSerial: bool) : void {
    var prv = tls:c_ptr(c_uchar);
    var i = chpl_offset_serial;
    // Using memcpy to avoid pointer type punning
    c_memcpy(c_ptrTo(prv[i]), c_ptrTo(makeSerial), sizeof(bool));
  }
  proc chpl_task_data_getSerial(tls:c_ptr(chpl_task_ChapelData_t)) : bool {
    var ret:bool = false;
    var prv = tls:c_ptr(c_uchar);
    var i = chpl_offset_serial;
    // Using memcpy to avoid pointer type punning
    c_memcpy(c_ptrTo(ret), c_ptrTo(prv[i]), sizeof(bool));
    return ret;
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
