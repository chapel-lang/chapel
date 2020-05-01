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
  This module contains several iterators that can be used to drive a `forall`
  loop by performing dynamic and adaptive splitting of a range's iterations.

  For more information, see *User-Defined Parallel Zippered Iterators
  in Chapel*. Bradford L. Chamberlain, Sung-Eun Choi, Steven J. Deitz,
  Angeles Navarro. *PGAS 2011: Fifth Conference on Partitioned Global
  Address Space Programming Models*, October 2011.
*/
module DynamicIters {

  private use ChapelLocks;

/*
   Toggle debugging output.
*/
config param debugDynamicIters:bool=false;

//************************* Dynamic iterator

/*

  :arg c: The range to iterate over. The length of the range must be greater
          than zero.
  :type c: `range(?)`

  :arg chunkSize: The size of chunks to be yielded to each thread. Must be
                  greater than zero.
  :type chunkSize: `int`

  :arg numTasks: The number of tasks to use. Must be >= zero. If this argument
                 has the value 0, it will use the value indicated by
                 ``dataParTasksPerLocale``.
  :type numTasks: `int`

  :yields: Indices in the range ``c``.


  This iterator is equivalent to the dynamic scheduling approach of OpenMP.

  Given an input range ``c``, each task is assigned chunks of size
  ``chunkSize`` from ``c`` (or the remaining iterations if there are fewer
  than ``chunkSize``). This continues until there are no remaining iterations
  in ``c``.

  This iterator can be called in serial and zippered contexts.

*/
iter dynamic(c:range(?), chunkSize:int=1, numTasks:int=0) {

  if debugDynamicIters then
    writeln("Serial dynamic Iterator. Working with range ", c);

  for i in c do yield i;
}

// Parallel iterator
pragma "no doc"
iter dynamic(param tag:iterKind, c:range(?), chunkSize:int=1, numTasks:int=0)
where tag == iterKind.leader
{
  assert(chunkSize > 0); // caller's responsibility

  // # of tasks the range can fill. (fast) ceil so all work is represented
  const chunkTasks = divceilpos(c.size, chunkSize): int;

  // Check if the number of tasks is 0, in that case it returns a default value
  const nTasks = min(chunkTasks, defaultNumTasks(numTasks));

  type rType=c.type;

  // We're going to have to densify at some point, might as well
  // do it early and make range slicing easier.
  const remain:rType=densify(c,c);

  // If the number of tasks is insufficient, yield in serial
  if nTasks == 1 then {
    if debugDynamicIters then
      writeln("Dynamic Iterator: serial execution because there is not enough work");
    yield (remain,);
  } else {
    var moreWork : atomic bool;
    moreWork.write(true);
    var curIndex : atomic remain.low.type;
    curIndex.write(remain.low);

    coforall tid in 0..#nTasks with (const in remain) {
      while moreWork.read() {
        // There is local work in remain
        const low = curIndex.fetchAdd(chunkSize);
        var high = low + chunkSize-1;

        if low > remain.high {
          break;
        } else if high > remain.high {
          high = remain.high;
          moreWork.write(false);
        }

        const current:rType = remain(low .. high);

        if high >= low then {
          if debugDynamicIters then
            writeln("Parallel dynamic Iterator. Working at tid ", tid, " with range ", unDensify(current,c), " yielded as ", current);
          yield (current,);
        }
      }
    }
  }
}

// Follower
pragma "no doc"
iter dynamic(param tag:iterKind, c:range(?), chunkSize:int=1, numTasks:int, followThis)
where tag == iterKind.follower
{
  type rType=c.type;
  const current:rType=unDensify(followThis(0),c);
  if debugDynamicIters then
    writeln("Follower received range ", followThis, " ; shifting to ", current);
  for i in current do {
    yield i;
  }
}

//************************* Dynamic domain iterator
//This is the serial version of this iterator
/*

  :arg c: The domain to iterate over. The rank of the domain must be greater
          than zero.
  :type c: `domain`

  :arg chunkSize: The size of chunks to be yielded to each thread. Must be
                  greater than zero
  :type chunkSize: `int`

  :arg numTasks: The number of tasks to use. Must be >= zero. If this argument
                 has the value 0, it will use the value indicated by
                 ``dataParTasksPerLocale``.
  :type numTasks: `int`

  :arg parDim: The index of the dimension to parallelize across. Must be >= 0.
                Must be < the rank of the domain ``c``. Defaults to 0.
  :type parDim: `int`

  :yields: Indices of the domain ``c``

  Given an input domain ``c``, each task is assigned slices of ``c``. The
  chunks each have ``chunkSize`` slices in them (or the remaining iterations
  if there are fewer than ``chunkSize``). This continues until there are no
  remaining iterations in the dimension of ``c`` indicated by ``parDim``.

  This iterator can be called in serial and zippered contexts.
*/

iter dynamic(c:domain, chunkSize:int=1, numTasks:int=0, parDim:int=0)
{
  if debugDynamicIters then
    writeln("Serial Dynamic Domain Iterator, working with domain: ", c);

  for yieldTuple in c do yield yieldTuple;
}

//Leader
pragma "no doc"
iter dynamic(param tag:iterKind, c:domain, chunkSize:int=1, numTasks:int=0, parDim : int = 0)
  where tag == iterKind.leader
  {
    //caller's responsibility to use a valid chunk size
    assert(chunkSize > 0, "Chunk size must be greater than zero");

    //caller's responsibility to use a valid domain
    assert(c.rank > 0, "Must use a valid domain");

    //caller's responsibility to use a valid parDim
    assert(parDim < c.rank, "parDim must be a dimension of the domain");
    assert(parDim >= 0, "parDim must be a non-negative integer");

    var parDimDim = c.dim(parDim);
    var parDimOffset = c.dim(parDim).low;

    for i in dynamic(tag=iterKind.leader, parDimDim, chunkSize, numTasks) {
      //Set the new range based on the tuple the dynamic 1d iterator yields
      var newRange = i(0);

      type dType = c.type;
      //does the same thing as densify, but densify makes a stridable domain,
      // which mismatches here if c (and thus dType) is non-stridable
      var tempDom : dType = computeZeroBasedDomain(c);

      //rank-change slice the domain along parDim
      var tempTup = tempDom.dims();
      //change the value of the parDim elem of the tuple to the new range
      tempTup(parDim) = newRange;

      yield tempTup;
    }
  }

//Follower
pragma "no doc"
iter dynamic(param tag:iterKind, c:domain, chunkSize:int=1, numTasks:int, parDim:int, followThis)
where tag == iterKind.follower
{
  //Invoke the default rectangular domain follower iterator
  for i in c.these(tag=iterKind.follower, followThis=followThis) do
    yield i;
}

//************************* Guided iterator

/*

  :arg c: The range to iterate over. Must have a length greater than zero.
  :type c: `range(?)`

  :arg numTasks: The number of tasks to use. Must be >= zero. If this argument
                 has the value 0, it will use the value indicated by
                 ``dataParTasksPerLocale``.
  :type numTasks: `int`

  :yields: Indices in the range ``c``.

  This iterator is equivalent to the guided policy of OpenMP: Given an input
  range ``c``, each task is assigned chunks of variable size, until there are
  no remaining iterations in ``c``. The size of each chunk is the number of
  unassigned iterations divided by the number of tasks, ``numTasks``. The size
  decreases approximately exponentially to 1. The splitting strategy is
  therefore adaptive.

  This iterator can be called in serial and zippered contexts.

*/
iter guided(c:range(?), numTasks:int=0) {

  if debugDynamicIters then
    writeln("Serial guided Iterator. Working with range ", c);

  for i in c do yield i;
}

pragma "no doc"
iter guided(param tag:iterKind, c:range(?), numTasks:int=0)
where tag == iterKind.leader
{
  // Check if the number of tasks is 0, in that case it returns a default value
  const nTasks=min(c.size, defaultNumTasks(numTasks));
  type rType=c.type;
  var remain:rType = densify(c,c);
  // If the number of tasks is insufficient, yield in serial
  if nTasks == 1 then {
    if debugDynamicIters then
      writeln("Guided Iterator: serial execution because there is not enough work");
    yield (remain,);
  }

  else {
    var undone: atomic bool = true;
    const factor=nTasks;
    var lock: chpl_LocalSpinlock;

    coforall tid in 0..#nTasks with (ref remain) do {
      while undone.read() do {
        // There is local work in remain(tid)
        const current:rType=adaptSplit(remain, factor, undone, lock);
        if current.size !=0 then {
          if debugDynamicIters then
            writeln("Parallel guided Iterator. Working at tid ", tid, " with range ", unDensify(current,c), " yielded as ", current);
          yield (current,);
        }
      }
    }
  }
}

// Follower
pragma "no doc"
iter guided(param tag:iterKind, c:range(?), numTasks:int, followThis)
where tag == iterKind.follower

{
  type rType=c.type;
  const current:rType=unDensify(followThis(0),c);
  if debugDynamicIters then
    writeln("Follower received range ", followThis, " ; shifting to ", current);
  for i in current do {
    yield i;
  }
}

//************************* Guided domain iterator
// Here is the serial version of this iterator.
/*

  :arg c: The domain to iterate over. The rank of the domain must be greater
          than zero.
  :type c: `domain`

  :arg numTasks: The number of tasks to use. Must be >= zero. If this argument
                 has the value 0, it will use the value indicated by
                 ``dataParTasksPerLocale``.
  :type numTasks: `int`

  :arg parDim: The index of the dimension to parallelize across. Must be >= 0.
               Must be < the rank of the domain ``c``. Defaults to 0.
  :type parDim: `int`

  :yields: Indices in the domain ``c``.

  This iterator is equivalent to the guided policy of OpenMP.

  Given an input domain ``c``, each task is assigned slices of variable size,
  until there are no remaining iterations in the dimension of ``c`` indicated
  by ``parDim``. The size of each chunk is the number of unassigned iterations
  divided by the number of tasks, ``numTasks``. The size decreases
  approximately exponentially to 1. The splitting strategy is therefore
  adaptive.

  This iterator can be called in serial and zippered contexts.

*/
iter guided(c:domain, numTasks:int=0, parDim:int=0)
{
  if debugDynamicIters then
    writeln("Serial guided domain iterator, working with domain ", c);

  for yieldTuple in c do yield yieldTuple;
}

// Leader.
pragma "no doc"
iter guided(param tag:iterKind, c:domain, numTasks:int=0, parDim:int=0)
where tag == iterKind.leader
{
  // Caller's responsibility to use a valid domain.
  assert(c.rank > 0, "Must use a valid domain");

  // Caller's responsibility to use a valid parDim.
  assert(parDim < c.rank, "parDim must be a dimension of the domain");
  assert(parDim >= 0, "parDim must be a non-negative integer");

  var parDimDim = c.dim(parDim);

  for i in guided(tag=iterKind.leader, parDimDim, numTasks) {
    // Set the new range based on the tuple the guided 1-D iterator yields.
    var newRange = i(0);

    type dType = c.type;
    // Does the same thing as densify, but densify makes a stridable domain,
    // which mismatches here if c (and thus dType) is non-stridable.
    var tempDom : dType = computeZeroBasedDomain(c);

    // Rank-change slice the domain along parDim
    var tempTup = tempDom.dims();
    // Change the value of the parDim elem of the tuple to the new range
    tempTup(parDim) = newRange;

    yield tempTup;
  }
}

// Follower.
pragma "no doc"
iter guided(param tag:iterKind, c:domain, numTasks:int, parDim:int, followThis)
where tag == iterKind.follower
{
  // Invoke the default rectangular domain follower iterator.
  for i in c.these(tag=iterKind.follower, followThis=followThis) do {
    yield i;
  }
}

//************************* Adaptive work-stealing iterator
/*

  :arg c: The range to iterate over. Must have a length greater than zero.
  :type c: `range(?)`

  :arg numTasks: The number of tasks to use. Must be >= zero. If this argument
                 has the value 0, it will use the value indicated by
                 ``dataParTasksPerLocale``.
  :type numTasks: `int`

  :yields: Indices in the range ``c``.

  This iterator implements a naive adaptive binary splitting work-stealing
  strategy: Initially the leader iterator distributes the range to split, ``c``,
  evenly among the ``numTasks`` tasks.

  Then, each task performs adaptive splitting on its local sub-range's iterations.
  When a task exhausts its local iterations, it steals and splits from the
  range of another task (the victim). The splitting method on the local range
  and on the victim range is binary: i.e. the size of each chunk is computed as
  the number of unassigned iterations divided by 2. There are three stealing
  strategies that can be selected at compile time using the config param
  :param:`methodStealing`.

  This iterator can be called in serial and zippered contexts.
*/
iter adaptive(c:range(?), numTasks:int=0) {

  if debugDynamicIters then
    writeln("Serial adaptive work-stealing Iterator. Working with range ", c);

  for i in c do yield i;

}


// Parallel iterator
/*
The enum used to represent adaptive methods.

- ``Whole``
  Each task without work tries to steal from its neighbor range
  until it exhausts that range. Then the task continues with the next
  neighbor range, and so on until there is no more work. This is the default
  policy.

- ``RoundRobin``
  Each task without work tries to steal once from its neighbor range, next
  from the following neighbor range and so on in a round-robin way until
  there is no more work.

- ``WholeTail``
  Similar to the ``Whole`` method, but now the splitting in the victim
  range is performed from its tail.
*/
enum Method {
  Whole = 0,
  RoundRobin = 1,
  WholeTail = 2
};

/*
  Used to select the adaptive stealing method. Defaults to ``Whole``.
  See :data:`Method` for more information.
*/
config param methodStealing = Method.Whole;

pragma "no doc"
iter adaptive(param tag:iterKind, c:range(?), numTasks:int=0)
where tag == iterKind.leader

{
  /*if (methodStealing != Method.Whole || methodStealing != Method.WholeTail) then
    compilerError("methodStealing value must be between 0 and 2");*/

  // Check if the number of tasks is 0, in that case it returns a default value
  const nTasks=min(c.size, defaultNumTasks(numTasks));
  type rType=c.type;

  // If the number of tasks is insufficient, yield in serial
  if nTasks == 1 then {
    if debugDynamicIters then
      writeln("Adaptive work-stealing Iterator: serial execution because there is not enough work");
    yield (densify(c,c),);

  }
  else {
    const r:rType=densify(c,c);
    const SpaceThreads:domain(1)=0..#nTasks;
    var localWork:[SpaceThreads] rType; // The remaining range to split on each Thread
    var moreLocalWork:[SpaceThreads] bool=true; // bool var to signal there is still work on each local range
    var locks: [SpaceThreads] chpl_LocalSpinlock;
    const factor:int=2;

    const factorSteal:int=2;
    var moreWork : atomic bool; // A global var to control the termination
    moreWork.write(true);

    // Variables to put a barrier to ensure the initial range is computed on each Thread
    var barrier : atomic int;

    // Start the parallel work
    coforall tid in 0..#nTasks with (const in r) {

      // Step 1: Initial range per Thread/Task

      // Initial Local range in localWork[tid]
      const chunkSize = c.size/nTasks;
      localWork[tid]=
      if tid==nTasks-1 then
        r#(chunkSize*(nTasks-1)-r.size)
      else
        (r+tid*chunkSize)#chunkSize;
      barrier.add(1);

      if debugDynamicIters then
        writeln("Parallel adaptive work-stealing Iterator. Working at tid ", tid, " with initial range ", localWork[tid]);

      // A barrier waiting for each thread to finish the initial assignment
      barrier.waitFor(nTasks);

      // Step 2: While there is work at tid, do splitting

      while moreLocalWork[tid] do {
        // There is local work
        // The current range we get after splitting locally
        const zeroBasedIters:rType=adaptSplit(localWork[tid], factorSteal, moreLocalWork[tid], locks[tid]);
        if zeroBasedIters.size !=0 then {
          if debugDynamicIters then
            writeln("Parallel adaptive Iterator. Working locally at tid ", tid, " with range yielded as ", zeroBasedIters);
          yield (zeroBasedIters,);
        }
      }

      // Step3: Task tid finished its work, so it will try to steal from a neighbor

      var nVisitedVictims:int=0;
      var victim=(tid+1) % nTasks;
      var stealFailed:bool=false;

      while moreWork.read() do {
        if debugDynamicIters then
          writeln("Entering at Stealing phase in tid ", tid," with victim ", victim, " using method of Stealing ", methodStealing);

        // Perform the splitting at the victim remaining range

        if methodStealing == Method.RoundRobin {
          if moreLocalWork[victim] then {
            // There is work in victim
            const zeroBasedIters2:rType=adaptSplit(localWork[victim], factorSteal, moreLocalWork[victim], locks[victim]);
            if zeroBasedIters2.size !=0 then {
              if debugDynamicIters then
                writeln("Range stolen at victim ", victim," yielded as ", zeroBasedIters2," by tid ", tid);
              yield (zeroBasedIters2,);
            }
          } else {
            stealFailed=true;
          }
        } else {
          while moreLocalWork[victim] do {
            // There is work in victim
            const zeroBasedIters2:rType=adaptSplit(localWork[victim], factorSteal, moreLocalWork[victim], locks[victim], methodStealing==Method.WholeTail);
                                          //after splitting from a victim range
            if zeroBasedIters2.size !=0 then {
              if debugDynamicIters then
                writeln("Range stolen at victim ", victim," yielded as ", zeroBasedIters2," by tid ", tid);
              yield (zeroBasedIters2,);
            }
          }
        }

        // If here, then it can have failed the stealing intent at the victim (method 1),
        // or we have exhausted the victim range (methods 0, 2)

        if (methodStealing==Method.Whole || methodStealing==Method.WholeTail || (methodStealing==Method.RoundRobin && stealFailed)) then {
          nVisitedVictims += 1; // Signal that there is no more work in victim
          stealFailed=false;
          if debugDynamicIters then
            writeln("Failed Stealing intent at tid ", tid," with victim ", victim, " and total no. of visited victims ", nVisitedVictims);
        }
        // Check if there is no more work
        if nVisitedVictims >= nTasks-1 then
          moreWork.write(false); // Signal that there is no more work in any victim
        else {  // There can be still work in other victim
          victim=(victim+1) % nTasks; // New victim to steal
          if methodStealing==Method.RoundRobin && victim==tid then
            victim=(victim+1) % nTasks;
        }
      }
    }
  }
}

// Follower
pragma "no doc"
iter adaptive(param tag:iterKind, c:range(?), numTasks:int, followThis)
where tag == iterKind.follower
{
  type rType=c.type;
  var current:rType=unDensify(followThis(0),c);
  if debugDynamicIters then
    writeln("Follower received range ", followThis, " ; shifting to ", current);
  for i in current do {
    yield i;
  }
}

//************************* Adaptive work-stealing domain iterator
// Here is the serial version of this iterator.
/*

  :arg c: The domain to iterate over. Must have a length greater than zero.
  :type c: `domain`

  :arg numTasks: The number of tasks to use. Must be >= zero. If this argument
                 has the value 0, it will use the value indicated by
                 ``dataParTasksPerLocale``.
  :type numTasks: `int`

  :arg parDim: The index of the dimension to parallelize across. Must be >= 0.
               Must be < the rank of the domain ``c``. Defaults to 0.
  :type parDim: `int`

  :yields: Indices in the domain ``c``.

  This iterator implements a naive adaptive binary splitting work-stealing
  strategy: Initially, the leader iterator distributes the domain to split,
  ``c``, evenly among the ``numTasks`` tasks.

  Then, each task performs adaptive splitting on its local sub-domain's
  iterations. When a task exhausts its local iterations, it steals and splits
  from the domain of another task (the victim). The splitting method on the
  local domain and on the victim domain is binary: i.e. the size of each chunk
  is computed as the number of unassigned iterations divided by 2. There are
  three stealing strategies that can be selected at compile time using the
  config param :param:`methodStealing` (see range version of iterator above).

  This iterator can be called in serial and zippered contexts.
*/
iter adaptive(c:domain, numTasks:int=0, parDim:int=0)
{
  if debugDynamicIters then
    writeln("Serial adaptive domain iterator, working with domain ", c);

  for yieldTuple in c do yield yieldTuple;
}

// Leader.
pragma "no doc"
iter adaptive(param tag:iterKind, c:domain, numTasks:int=0, parDim:int=0)
where tag == iterKind.leader
{
  // Caller's responsibility to use a valid domain.
  assert(c.rank > 0, "Must use a valid domain");

  // Caller's responsibility to use a valid parDim.
  assert(parDim < c.rank, "parDim must be a dimension of the domain");
  assert(parDim >= 0, "parDim must be a non-negative integer");

  var parDimDim = c.dim(parDim);

  for i in adaptive(tag=iterKind.leader, parDimDim, numTasks) {
    // Set the new range based on the tuple the guided 1-D iterator yields.
    var newRange = i(0);

    type dType = c.type;
    // Does the same thing as densify, but densify makes a stridable domain,
    // which mismatches here if c (and thus dType) is non-stridable.
    var tempDom : dType = computeZeroBasedDomain(c);

    // Rank-change slice the domain along parDim
    var tempTup = tempDom.dims();
    // Change the value of the parDim elem of the tuple to the new range
    tempTup(parDim) = newRange;

    yield tempTup;
  }
}

// Follower.
pragma "no doc"
iter adaptive(param tag:iterKind, c:domain, numTasks:int, parDim:int, followThis)
where tag == iterKind.follower
{
  // Invoke the default rectangular domain follower iterator.
  for i in c.these(tag=iterKind.follower, followThis=followThis) do {
    yield i;
  }
}

//************************* Helper functions
private proc defaultNumTasks(nTasks:int)
{
  var dnTasks = nTasks;
  if nTasks <= 0  {
    if dataParTasksPerLocale == 0 then
      dnTasks = here.maxTaskPar;
    else
      dnTasks = dataParTasksPerLocale;

    if nTasks < 0 then
      warning("'numTasks' < 0, defaulting to numTasks=", dnTasks);
  }
  return dnTasks;
}

private proc adaptSplit(ref rangeToSplit:range(?), splitFactor:int, ref itLeft, lock:chpl_LocalSpinlock, splitTail:bool=false)
{
  type rType=rangeToSplit.type;
  type lenType=rangeToSplit.size.type;
  var totLen, size:lenType;
  const profThreshold=1;

  lock.lock();
  totLen=rangeToSplit.size;
  if totLen > profThreshold then
    size=max(totLen/splitFactor, profThreshold);
  else {
    size=totLen;
    if isAtomic(itLeft) then itLeft.write(false); else itLeft=false;
  }
  const direction = if splitTail then -1 else 1;
  const firstRange:rType=rangeToSplit#(direction*size);
  rangeToSplit=rangeToSplit#(direction*(size-totLen));
  lock.unlock();
  return firstRange;
}

}
