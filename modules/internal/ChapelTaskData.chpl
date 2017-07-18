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

  extern type chpl_task_ChapelData_t;
  extern proc chpl_task_getChapelData(): c_ptr(chpl_task_ChapelData_t);

  // Chapel task-local data format:
  // 16 bytes of wide pointer for _remoteEndCountType
  // 1 byte of bool for serial_state
  private const chpl_offset_endCount = 0;
  private const  chpl_offset_serial = 16;
  private const     chpl_offset_end = chpl_offset_serial+c_sizeof(bool);

  // workaround to make sure some things are wide
  private var neverExecuted = false;

  proc chpl_task_setDynamicEndCount(in end: _remoteEndCountType) {
    var prv = chpl_task_getChapelData():c_ptr(c_uchar);
    var i = chpl_offset_endCount;
    // Using memcpy to avoid pointer type punning
    // using 16 to work around problems with sizeof(wideptrtype)
    c_memcpy(c_ptrTo(prv[i]), c_ptrTo(end), 16);

    if neverExecuted {
      on Locales[numLocales-1] do
        end = _endCountAlloc(false);
    }
  }
  proc chpl_task_getDynamicEndCount() : _remoteEndCountType {
    var ret:_remoteEndCountType = nil;
    var prv = chpl_task_getChapelData():c_ptr(c_uchar);
    var i = chpl_offset_endCount;
    // Using memcpy to avoid pointer type punning
    c_memcpy(c_ptrTo(ret), c_ptrTo(prv[i]), 16);
    
    if neverExecuted {
      on Locales[numLocales-1] do
        ret = _endCountAlloc(false);
    }

    return ret;
  }
 
  // Note, these can be called before module initialization is complete.
  export proc chpl_task_setSerial(in makeSerial: bool) : void {
    var prv = chpl_task_getChapelData():c_ptr(c_uchar);
    var i = chpl_offset_serial;
    // Using memcpy to avoid pointer type punning
    c_memcpy(c_ptrTo(prv[i]), c_ptrTo(makeSerial), c_sizeof(bool));
  }

  export proc chpl_task_getSerial() : bool {
    var ret:bool = false;
    var prv = chpl_task_getChapelData():c_ptr(c_uchar);
    var i = chpl_offset_serial;
    // Using memcpy to avoid pointer type punning
    c_memcpy(c_ptrTo(ret), c_ptrTo(prv[i]), c_sizeof(bool));
    return ret;
  }

  // module init function - check sizes
  {
    assert(chpl_offset_end <= c_sizeof(chpl_task_ChapelData_t));
  }
}
