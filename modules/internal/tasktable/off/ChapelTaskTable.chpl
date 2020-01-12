/*
 * Copyright 2004-2020 Cray Inc.
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

  ////////////////////////////////////////////////////////////////////////{
  //
  // Exported task table code.
  //
  // In the --no-task-table mode, these are just no-op stubs and a halt()
  // if someone tries to print the task table.
  //
  ////////////////////////////////////////////////////////////////////////}

  export proc chpldev_taskTable_add(taskID   : chpl_taskID_t,
                                    lineno   : uint(32),
                                    filename : int(32),
                                    tl_info  : uint(64))
  {
  }

  export proc chpldev_taskTable_remove(taskID : chpl_taskID_t)
  {
  }

  export proc chpldev_taskTable_set_active(taskID : chpl_taskID_t)
  {
  }

  export proc chpldev_taskTable_set_suspended(taskID : chpl_taskID_t)
  {
  }

  export proc chpldev_taskTable_get_tl_info(taskID : chpl_taskID_t)
  {
  }

  export proc chpldev_taskTable_print()
  {
    use ChapelIO;
    halt("To use task tracking, you must recompile with --task-tracking");
  }
}
