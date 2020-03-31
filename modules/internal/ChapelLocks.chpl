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

/*
 * Collection of mutexes/locks.
 */
module ChapelLocks {
  private use Atomics, ChapelBase;
  private use MemConsistency;
  /*
   * Local processor atomic spinlock. Intended for situations with minimal
   * contention or very short critical sections.
   */
  pragma "default intent is ref"
  record chpl_LocalSpinlock {
    var l: chpl__processorAtomicType(bool);

    inline proc lock() {
      on this do
        while l.read() || l.testAndSet(memoryOrder.acquire) do
          chpl_task_yield();
    }

    inline proc unlock() {
      l.clear(memoryOrder.release);
    }
  }
}
