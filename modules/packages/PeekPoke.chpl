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
   .. warning::
     This module is unstable and the API is likely to change over time.

   This module provides `peek()` and `poke()` operations on atomics. `peek()`
   and `poke()` are non-atomic read and write operations. They can provide a
   performance improvement when local reads/writes are performed and atomicity
   is not required. For example they can be used to initialize an array of
   atomics or perform a reduction when no concurrent updates are occurring.

   .. code-block:: chapel

     use BlockDist, PeekPoke;

     const space = {1..1000};
     const D = space dmapped Block(space);
     var A: [D] atomic int;

     forall i in D do
       A[i].poke(i);

     const sum = + reduce A.peek();
     writeln(sum); // 500500
 */
module PeekPoke {
  /*
     Non-atomically reads the stored value.
  */
  inline proc const AtomicBool.peek(): bool {
    return this.read(order=memoryOrder.relaxed);
  }
  pragma "no doc"
  inline proc const RAtomicBool.peek(): bool {
    return _v:bool;
  }

  /*
     Non-atomically writes `value`.
  */
  inline proc AtomicBool.poke(value:bool): void {
    this.write(value, order=memoryOrder.relaxed);
  }
  pragma "no doc"
  inline proc RAtomicBool.poke(value:bool): void {
    _v = value:int(64);
  }


  /*
     Non-atomically reads the stored value.
  */
  inline proc const AtomicT.peek(): T {
    return this.read(order=memoryOrder.relaxed);
  }
  pragma "no doc"
  inline proc const RAtomicT.peek(): T {
    return _v;
  }


  /*
     Non-atomically writes `value`.
  */
  inline proc AtomicT.poke(value:T): void {
    this.write(value, order=memoryOrder.relaxed);
  }
  pragma "no doc"
  inline proc RAtomicT.poke(value:T): void {
    _v = value;
  }
}
