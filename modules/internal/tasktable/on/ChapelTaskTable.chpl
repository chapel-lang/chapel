/*
 * Copyright 2004-2015 Cray Inc.
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

// ChapelTaskTable.chpl
//
module ChapelTaskTable {
  
  use ChapelBase;
  use ChapelIO;
  use ChapelArray;
  use DefaultRectangular;
  use DefaultAssociative;
  use ChapelTaskID;
  
  /* 
     This code keeps track of tasks, their state, and where they
     spawned from.
  
     The task information is used to generate a task report (optionally
     displayed when the user Ctrl+C's out of the program).
  
     The type chpl_taskID_t is a primitive type defined in the include
     files for each tasking layer.
  
  */
  
  enum taskState { pending, active, suspended };
  
  //
  // This represents a currently running task.  The state, lineno, and
  // filename members should be obvious; tl_info is information belonging
  // to the tasking layer.
  //
  // If we included the parent in this, the print method could topologically
  // sort the tasks and indent to show the parent/child relationships, which
  // might be very nice.
  //
  record chpldev_Task {
    var state     : taskState;
    var lineno    : uint(32);
    var filename  : c_string;
    var tl_info   : uint(64);
  }
  
  class chpldev_taskTable_t {
    var dom : domain(chpl_taskID_t, parSafe=false);
    var map : [dom] chpldev_Task;
  }
  
  pragma "private"
  var chpldev_taskTable : chpldev_taskTable_t;
  
  //----------------------------------------------------------------------{
  //- Code to initialize the task table on each locale.
  //-
  proc chpldev_taskTable_init() {
    // Doing this in parallel should be safe as long as this module is
    // initialized late (after DefaultRectangular and most other
    // internal modules are already initialized)
    coforall loc in Locales with (ref chpldev_taskTable) do on loc {
      chpldev_taskTable = new chpldev_taskTable_t();
    }
  }

  extern var taskreport: int(32);
  if taskreport then chpldev_taskTable_init();

  //-
  //----------------------------------------------------------------------}
  
  
  ////////////////////////////////////////////////////////////////////////{
  //
  // Exported task table code.
  //
  // In general, tasks may have been created before the task table was
  // initialized.
  // In that case, operations may be attempted on the task table using
  // taskIDs that are unknown to it.  That is why we check
  // for membership on all operations.
  //
  ////////////////////////////////////////////////////////////////////////}
  
  export proc chpldev_taskTable_add(taskID   : chpl_taskID_t,
                                    lineno   : uint(32),
                                    filename : c_string,
                                    tl_info  : uint(64))
  {
    if (chpldev_taskTable == nil) then return;
  
    if (!chpldev_taskTable.dom.member(taskID)) then
      // This must be serial, because if add() results in parallelism
      // (due to _resize), it may lead to deadlock (due to reentry
      // into the runtime tasking layer for task table operations).
      serial true do
        chpldev_taskTable.dom.add(taskID);
  
    chpldev_taskTable.map[taskID] = new chpldev_Task(taskState.pending,
                                                 lineno, filename, tl_info);
  }
  
  export proc chpldev_taskTable_remove(taskID : chpl_taskID_t)
  {
    if (chpldev_taskTable == nil ||
        !chpldev_taskTable.dom.member(taskID)) then return;
  
    // This must be serial, because if remove() results in parallelism
    // (due to _resize), it may lead to deadlock (due to reentry into
    // the runtime tasking layer for task table operations).
    serial true do
      chpldev_taskTable.dom.remove(taskID);
  }
  
  export proc chpldev_taskTable_set_active(taskID : chpl_taskID_t)
  {
    if (chpldev_taskTable == nil ||
        !chpldev_taskTable.dom.member(taskID)) then return;
  
    chpldev_taskTable.map[taskID].state = taskState.active;
  }
  
  export proc chpldev_taskTable_set_suspended(taskID : chpl_taskID_t)
  {
    if (chpldev_taskTable == nil ||
        !chpldev_taskTable.dom.member(taskID)) then return;
  
    chpldev_taskTable.map[taskID].state = taskState.suspended;
  }
  
  export proc chpldev_taskTable_get_tl_info(taskID : chpl_taskID_t)
  {
    if (chpldev_taskTable == nil ||
        !chpldev_taskTable.dom.member(taskID)) then return 0:uint(64);
  
    return chpldev_taskTable.map[taskID].tl_info;
  }
  
  export proc chpldev_taskTable_print() 
  {
    if (chpldev_taskTable == nil) then return;
  
    for taskID in chpldev_taskTable.dom {
      stderr.writeln("- ", chpldev_taskTable.map[taskID].filename,
                     ":",  chpldev_taskTable.map[taskID].lineno,
                     " is ", chpldev_taskTable.map[taskID].state);
    }
  }
  
}
