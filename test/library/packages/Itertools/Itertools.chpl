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
  iterators, and is currently being worked upon.
*/

module Itertools {

  use RangeChunk;

  /*
    Returns an object over and over again, a specified
    number of times.


    :arg arg: The object to be returned
    :type arg: `?`

    :arg times: The number of times to return
    :type times: `int`

    :yields: Object in the range ``1..times``

    :throws: ``IllegalArgumentError`` on parallel infinite iteration (see below)


    If the argument ``times`` has the value 0, it will return
    the object an infinite number of times.

    This iterator can be called in serial and parallel zippered contexts.

    .. note::
      This iterator is not suitable for parallel infinite iteration i.e.
      avoid using zippered, ``forall``, or ``coforall`` loops with the
      ``times`` argument set to 0.
  */

  // Serial iterator

  iter repeat (arg, times = 0) {
    if times == 0 then
      for 1.. do yield arg;
    else
      for 1..#times do yield arg;
  }

  // Standalone parallel iterator

  pragma "no doc"
  iter repeat (param tag: iterKind, arg, times = 0) throws
      where tag == iterKind.standalone {
    if times == 0 then
      throw new owned IllegalArgumentError(
          "Infinite iteration not supported for parallel loops.");
    else
      forall 1..#times do yield arg;
  }

  // Parallel iterator - Leader

  pragma "no doc"
  iter repeat (param tag: iterKind, arg, times = 0) throws
      where tag == iterKind.leader {

    var numTasks = if dataParTasksPerLocale > 0 then dataParTasksPerLocale
                                                  else here.maxTaskPar;

    if numTasks > times then numTasks = times;

    if times == 0 then
      throw new owned IllegalArgumentError(
          "Infinite iteration not supported for parallel loops.");
    else
      coforall tid in 0..#numTasks {
        const workingIters = chunk(0..#times, numTasks, tid);
        yield(workingIters,);
      }
  }

  // Parallel iterator - Follower

  pragma "no doc"
  iter repeat (param tag: iterKind, arg, times = 0, followThis)
      where tag == iterKind.follower && followThis.size == 1 {
    const workingIters = followThis(1);

    for workingIters do yield arg;
  }



  /*
    Returns elements from an iterable over and over again, a specified
    number of times.


    :arg arg: The iterable whose elements are to be returned
    :type arg: `?`

    :arg times: The number of times to iterate through the iterable
    (i.e. number of times each element is to be returned)
    :type times: `int`

    :yields: Elements of the iterable ``times`` times

    :throws: ``IllegalArgumentError`` on parallel infinite iteration (see below)


    If the argument ``times`` has the value 0, it will return each element of
    the iterable an infinite number of times.

    This iterator can be called in serial and parallel zippered contexts.

    .. note::
      This iterator is not suitable for parallel infinite iteration i.e.
      avoid using zippered, ``forall``, or ``coforall`` loops with the
      ``times`` argument set to 0.
  */

  // Serial iterator

  iter cycle(arg, times = 0) {
    if times == 0 then
      for 0.. do
        for element in arg do
          yield element;
    else
      for 0..#times do
        for element in arg do
          yield element;
  }

  // Parallel iterator - Leader

  pragma "no doc"
  iter cycle(param tag: iterKind, arg, times = 0) throws
      where tag == iterKind.leader {

    var numTasks = if dataParTasksPerLocale > 0 then dataParTasksPerLocale
                                                else here.maxTaskPar;

    if numTasks > times then numTasks = times;

    if times == 0 then
      throw new owned IllegalArgumentError(
          "infinite iteration not supported for parallel loops");
    else
      coforall tid in 0..#numTasks {
        const workingIters = chunk(0..#times, numTasks, tid);
        workingIters.translate(-workingIters.low);
        yield(0..#(workingIters.high * arg.size),);
      }
  }

  // Parallel iterator - Follower

  pragma "no doc"
  iter cycle(param tag: iterKind, arg, times = 0, followThis)
      where tag == iterKind.follower && followThis.size == 1 {

    const workingIters = followThis(1);

    if isString(arg) || isArray(arg) || isTuple(arg) then
      for idx in workingIters do
        yield arg[(idx % arg.size) + 1];
    else {
      var tempObject: [1..#arg.size] arg.low.type;

      for (idx, element) in zip(1..#arg.size, arg) do
        tempObject[idx] = element;

      for idx in workingIters do
        yield tempObject[(idx % arg.size) + 1];
    }
  }

} // end module
