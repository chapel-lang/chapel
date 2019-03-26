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

/*
  This module serves as a toolkit of common serial and parallel
  iterators. This is currently being worked upon, and contains
  only the ``repeat`` iterator tool as of now.
*/

module Itertools {

  /*
    Returns an object over and over again, a specified
    number of times.


    :arg arg: The object to be returned
    :type arg: `?`

    :arg times: The number of times to return
    :type times: `int`

    :yields: Object in the range ``1..times``


    If the argument ``times`` has the value 0, it will return
    the object an infinite number of times.

    This iterator can be called in serial and zippered contexts.

    .. note::
      This iterator is not suitable for parallel infinite iteration i.e.
      avoid using zippered, ``forall``, or ``coforall`` loops with the
      ``times`` argument set to 0.
  */

  // Serial iterator

  iter repeat (arg, times = 0) {
    if times == 0 then
      for count in 1.. do yield arg;
    else
      for count in 1..#times do yield arg;     
  }

  // Standalone parallel iterator

  pragma "no doc"
  iter repeat (param tag: iterKind, arg, times = 0)
      where tag == iterKind.standalone {
    if times == 0 then
      halt("Infinite iteration not supported for parallel loops.");
    else
      forall count in 1..#times do yield arg;
  }

  use RangeChunk;

  // Parallel iterator - Leader

  pragma "no doc"
  iter repeat (param tag: iterKind, arg, times = 0)
      where tag == iterKind.leader {

    const numTasks = if dataParTasksPerLocale > 0 then dataParTasksPerLocale
                                                  else here.maxTaskPar;

    if times == 0 then
      halt("Infinite iteration not supported for parallel loops.");
    else
      coforall tid in 0..#numTasks {
        const working_iters = chunk(0..#times, numTasks, tid);
        yield(working_iters,);
      }
  }

  // Parallel iterator - Follower
  
  pragma "no doc"
  iter repeat (param tag: iterKind, arg, times = 0, followThis)
      where tag == iterKind.follower && followThis.size == 1 {
    const working_iters = followThis(1);

    for idx in working_iters do yield arg;
  }
} // end module
