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

// ChapelTaskDataHelp.chpl
//
module ChapelTaskDataHelp {

  private use ChapelStandard;

  extern type chpl_task_infoChapel_t;
  pragma "fn synchronization free"
  extern proc chpl_task_getInfoChapel(): c_ptr(chpl_task_infoChapel_t);
  pragma "fn synchronization free"
  extern proc chpl_task_getInfoChapelInBundle(args: chpl_task_bundle_p):c_ptr(chpl_task_infoChapel_t);

  // This function is called to set up the Chapel-managed portion
  // of task-local storage in the argument bundle when creating
  // a new task.
  proc chpl_task_data_setup(args: chpl_task_bundle_p,
                            infoChapel:c_ptr(chpl_task_infoChapel_t)) {
    c_memcpy(chpl_task_getInfoChapelInBundle(args), infoChapel,
             c_sizeof(chpl_task_infoChapel_t));
  }

  // Propagate an error from a task to its caller / sync point.
  pragma "task complete impl fn"
  proc chpl_save_task_error(e: _EndCountBase, err: unmanaged Error?) {
    if err != nil {
      e.errors.append(err!);
    }
  }
  pragma "task complete impl fn"
  proc chpl_save_task_error_owned(e: _EndCountBase, in err: owned Error?) {
    if err != nil {
      e.errors.append(err.release()!);
    }
  }
}
