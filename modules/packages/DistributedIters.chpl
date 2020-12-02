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
  This module contains iterators that can be used to distribute a `forall`
  loop for a range or domain by dynamically splitting iterations between
  locales.

  ..
    Part of a 2017 Cray summer intern project by Sean I. Geronimo Anderson
    (github.com/s-geronimoanderson) as mentored by Ben Harshbarger
    (github.com/benharsh).
*/
module DistributedIters
{
use DynamicIters,
    Time,
    DSIUtil;

/*
  Toggle debugging output.
*/
config param debugDistributedIters:bool = false;

/*
  Toggle per-locale performance timing and output.
*/
config param timeDistributedIters:bool = false;

/*
  Toggle invocation information output.
*/
config const infoDistributedIters:bool = false;

// Distributed Dynamic Iterator.
// Serial version.
/*
  :arg c: The range (or domain) to iterate over. The range (domain) size must
    be positive.
  :type c: `range(?)` or `domain`

  :arg chunkSize: The chunk size to yield to each task. Must be positive.
    Defaults to 1.
  :type chunkSize: `int`

  :arg numTasks: The number of tasks to use. Must be nonnegative. If this
    argument has value 0, the iterator will use the value indicated by
    ``dataParTasksPerLocale``.
  :type numTasks: int

  :arg parDim: If ``c`` is a domain, then this specifies the dimension index
    to parallelize across. Must be non-negative and less than the rank of
    the domain ``c``. Defaults to 0.
  :type parDim: int

  :arg localeChunkSize: Chunk size to yield to each locale. Must be
    nonnegative. If this argument has value 0, the iterator will use
    an undefined heuristic in an attempt to choose a value that will
    perform well.
  :type localeChunkSize: `int`

  :arg coordinated: If true (and multi-locale), then have the locale invoking
    the iterator coordinate task distribution only; that is, disallow it from
    receiving work.
  :type coordinated: bool

  :arg workerLocales: An array of locales over which to distribute the work.
    Defaults to ``Locales`` (all available locales).
  :type workerLocales: [] locale

  :yields: Indices in the range ``c``.

  This iterator is equivalent to a distributed version of the dynamic policy of
  OpenMP.

  Given an input range (or domain) ``c``, each locale (except the calling
  locale, if coordinated is true) receives chunks of size ``localeChunkSize``
  from ``c`` (or the remaining iterations if there are fewer than
  ``localeChunkSize``). Each locale then distributes sub-chunks of size
  ``chunkSize`` as tasks, using the ``dynamic`` iterator from the
  ``DynamicIters`` module.

  Available for serial and zippered contexts.
*/
iter distributedDynamic(c,
                        chunkSize:int=1,
                        numTasks:int=0,
                        parDim:int=0,
                        localeChunkSize:int=0,
                        coordinated:bool=false,
                        workerLocales=Locales)
{
  compilerAssert(isDomain(c) || isRange(c),
                 ("DistributedIters: Dynamic iterator (serial): must use a "
                  + "valid domain or range"),
                 1);
  if debugDistributedIters
  then writeln("DistributedIters: Dynamic iterator (serial): working with ",
               (if isDomain(c) then "domain " else "range "), c);
  for i in c do yield i;
}

// Zippered leader.
pragma "no doc"
iter distributedDynamic(param tag:iterKind,
                        c,
                        chunkSize:int=1,
                        numTasks:int=0,
                        parDim:int=0,
                        localeChunkSize:int=0,
                        coordinated:bool=false,
                        workerLocales=Locales)
where tag == iterKind.leader
{
  compilerAssert(isDomain(c) || isRange(c),
                 ("DistributedIters: Dynamic iterator (leader): must use a "
                  + "valid domain or range"),
                 1);
  assert(chunkSize > 0,
         ("DistributedIters: Dynamic iterator (leader): "
          + "chunkSize must be a positive integer"));
  assert(localeChunkSize >= 0,
         ("DistributedIters: Dynamic iterator (leader): "
          + "localeChunkSize must be a nonnegative integer"));

  type cType = c.type;

  if isDomain(c) then
  {
    assert(c.rank > 0, ("DistributedIters: Dynamic iterator (leader): "
                        + "Must use a valid domain"));
    assert(parDim >= 0, ("DistributedIters: Dynamic iterator (leader): "
                        + "parDim must be a non-negative integer"));
    assert(parDim < c.rank, ("DistributedIters: Dynamic iterator (leader): "
                              + "parDim must be a dimension of the domain"));
    var parDimDim = c.dim(parDim);
    for t in distributedDynamic(tag=iterKind.leader,
                                c=parDimDim,
                                chunkSize=chunkSize,
                                numTasks=numTasks,
                                parDim=0,
                                localeChunkSize=localeChunkSize,
                                coordinated=coordinated,
                                workerLocales=workerLocales)
    {
      // Set the new range based on the tuple the dynamic 1-D iterator yields.
      var newRange = t(0);

      // Does the same thing as densify, but densify makes a stridable domain,
      // which mismatches here if c (and thus cType) is non-stridable.
      var tempDom : cType = computeZeroBasedDomain(c);

      // Rank-change slice the domain along parDim
      var tempTup = tempDom.dims();
      // Change the value of the parDim elem of the tuple to the new range
      tempTup(parDim) = newRange;

      yield tempTup;
    }
  }
  else // c is a range.
  {
    const iterCount = c.size;

    if iterCount == 0 then halt("DistributedIters: Dynamic iterator (leader):",
                                " the range is empty");

    const denseRange:cType = densify(c,c);

    if iterCount == 1
       || numTasks == 1 && numLocales == 1
    then
    {
      if debugDistributedIters
      then writeln("DistributedIters: Dynamic iterator (leader): serial ",
                   "execution due to insufficient work or compute resources");
      yield (denseRange,);
    }
    else
    {
      const numWorkerLocales = workerLocales.size;
      const denseRangeHigh:int = denseRange.high;
      const masterLocale = here.locale;

      const actualWorkerLocales =
        [L in workerLocales] if numLocales == 1
                                || !coordinated
                                || L != masterLocale
                             then L;

      if infoDistributedIters then
      {
        const actualWorkerLocaleIds = [L in actualWorkerLocales] L.id:string;
        const actualWorkerLocaleIdsSorted = actualWorkerLocaleIds.sorted();
        writeln("DistributedIters: distributedDynamic:");
        writeln("  coordinated = ", coordinated);
        writeln("  numLocales = ", numLocales);
        writeln("  numWorkerLocales = ", numWorkerLocales);
        writeln("  actualWorkerLocales.size = ", actualWorkerLocales.size);
        writeln("  masterLocale.id = ", masterLocale.id);
        writeln("  actualWorkerLocaleIds = [ ",
                ", ".join(actualWorkerLocaleIdsSorted),
                " ]");
      }

      var localeTimes:[0..#numLocales]real;
      var totalTime:Timer;
      if timeDistributedIters then totalTime.start();

      // The dynamic iterator stage (determines next subrange index and size).
      var dynamicStageCount:atomic int;

      coforall L in actualWorkerLocales
      with (ref dynamicStageCount, ref localeTimes)
      do on L
      {
        // TODO: Find a better heuristic backed by experimentation
        const computedSize = if localeChunkSize == 0
                             then denseRange.size / actualWorkerLocales.size / 10
                             else localeChunkSize;
        // localeChunkSize should not be less than chunkSize
        const maxSize = max(computedSize, chunkSize);
        const actualLocaleChunkSize = min(maxSize, denseRange.size);
        var localeTime:Timer;
        if timeDistributedIters then localeTime.start();

        var localeStage:int = dynamicStageCount.fetchAdd(1);
        var localeRange:cType = dynamicSubrange(denseRange,
                                                actualLocaleChunkSize,
                                                localeStage);
        while localeRange.low <= denseRangeHigh
        {
          const denseLocaleRange:cType = densify(localeRange, localeRange);
          for denseTaskRangeTuple in DynamicIters.dynamic(tag=iterKind.leader,
                                                          localeRange,
                                                          chunkSize,
                                                          numTasks)
          {
            const taskRange:cType = unDensify(denseTaskRangeTuple(0),
                                              localeRange);
            if debugDistributedIters
            then writeln("DistributedIters: Dynamic iterator (leader): ",
                         here.locale, ": yielding ", unDensify(taskRange,c),
                         " (", taskRange.size,
                         "/", localeRange.size,
                         " locale-owned of ", iterCount,
                         " total) as ", taskRange);
            yield (taskRange,);
          }

          localeStage = dynamicStageCount.fetchAdd(1);
          localeRange = dynamicSubrange(denseRange,
                                        actualLocaleChunkSize,
                                        localeStage);
        }

        if timeDistributedIters then
        {
          localeTime.stop();
          localeTimes[here.id] = localeTime.elapsed();
        }
      }

      if timeDistributedIters then
      {
        totalTime.stop();
        writeTimeStatistics(totalTime.elapsed(), localeTimes, coordinated);
      }
    }
  }
}

// Zippered follower.
pragma "no doc"
iter distributedDynamic(param tag:iterKind,
                        c,
                        chunkSize:int,
                        numTasks:int,
                        parDim:int,
                        localeChunkSize:int,
                        coordinated:bool,
                        workerLocales=Locales,
                        followThis)
where tag == iterKind.follower
{
  compilerAssert(isDomain(c) || isRange(c),
                 ("DistributedIters: Dynamic iterator (follower): must use a "
                  + "valid domain or range"),
                 1);
  const current = if isDomain(c)
                  then c.these(tag=iterKind.follower, followThis=followThis)
                  else unDensify(followThis(0), c);

  if debugDistributedIters
  then writeln("DistributedIters: Dynamic iterator (follower): ", here.locale,
               ": received ",
               if isDomain(c) then "domain " else "range ",
               followThis, " (", current.size,
               "/", c.size, "); shifting to ", current);

  for i in current do yield i;
}

// Distributed Guided Iterator.
// Serial version.
/*
  :arg c: The range (or domain) to iterate over. The range (domain) size must
    be positive.
  :type c: `range(?)` or `domain`

  :arg numTasks: The number of tasks to use. Must be nonnegative. If this
    argument has value 0, the iterator will use the value indicated by
    ``dataParTasksPerLocale``.
  :type numTasks: int

  :arg parDim: If ``c`` is a domain, then this specifies the dimension index
    to parallelize across. Must be non-negative and less than the rank of
    the domain ``c``. Defaults to 0.
  :type parDim: int

  :arg minChunkSize: The smallest allowable chunk size. Must be positive.
    Defaults to 1.
  :type minChunkSize: int

  :arg coordinated: If true (and multi-locale), then have the locale invoking
    the iterator coordinate task distribution only; that is, disallow it from
    receiving work.
  :type coordinated: bool

  :arg workerLocales: An array of locales over which to distribute the work.
    Defaults to ``Locales`` (all available locales).
  :type workerLocales: [] locale

  :yields: Indices in the range ``c``.

  This iterator is equivalent to a distributed version of the guided policy of
  OpenMP.

  Given an input range (or domain) ``c``, each locale (except the calling
  locale, if coordinated is true) receives chunks of approximately
  exponentially decreasing size, until the remaining iterations reaches a
  minimum value, ``minChunkSize``, or there are no remaining iterations in
  ``c``. The chunk size is the number of unassigned iterations divided by the
  number of locales. Each locale then distributes sub-chunks as tasks, where
  each sub-chunk size is the number of unassigned local iterations divided by
  the number of tasks, ``numTasks``, and decreases approximately exponentially
  to 1. The splitting strategy is therefore adaptive.

  Available for serial and zippered contexts.
*/
iter distributedGuided(c,
                       numTasks:int=0,
                       parDim:int=0,
                       minChunkSize:int=1,
                       coordinated:bool=false,
                       workerLocales=Locales)
{
  compilerAssert(isDomain(c) || isRange(c),
                 ("DistributedIters: Guided iterator (serial): must use a "
                  + "valid domain or range"),
                 1);
  if debugDistributedIters
  then writeln("DistributedIters: Guided iterator (serial): working with ",
               (if isDomain(c) then "domain " else "range "), c);
  for i in c do yield i;
}

// Zippered leader.
pragma "no doc"
iter distributedGuided(param tag:iterKind,
                       c,
                       numTasks:int=0,
                       parDim:int=0,
                       minChunkSize:int=1,
                       coordinated:bool=false,
                       workerLocales=Locales)
where tag == iterKind.leader
{
  compilerAssert(isDomain(c) || isRange(c),
                 ("DistributedIters: Guided iterator (leader): must use a "
                  + "valid domain or range"),
                 1);
  if isDomain(c) then
  {
    assert(c.rank > 0, ("DistributedIters: Guided iterator (leader): Must "
                        + "use a valid domain"));
    assert(parDim >= 0, ("DistributedIters: Guided iterator (leader): parDim "
                        + "must be a non-negative integer"));
    assert(parDim < c.rank, ("DistributedIters: Guided iterator (leader): "
                              + "parDim must be a dimension of the domain"));

    var parDimDim = c.dim(parDim);

    for t in distributedGuided(tag=iterKind.leader,
                               c=parDimDim,
                               numTasks=numTasks,
                               parDim=0,
                               minChunkSize=minChunkSize,
                               coordinated=coordinated,
                               workerLocales=workerLocales)
    {
      // Set the new range based on the tuple the guided 1-D iterator yields.
      var newRange = t(0);

      type cType = c.type;
      // Does the same thing as densify, but densify makes a stridable domain,
      // which mismatches here if c (and thus cType) is non-stridable.
      var tempDom : cType = computeZeroBasedDomain(c);

      // Rank-change slice the domain along parDim
      var tempTup = tempDom.dims();
      // Change the value of the parDim elem of the tuple to the new range
      tempTup(parDim) = newRange;

      yield tempTup;
    }
  }
  else // c is a range.
  {
    const iterCount = c.size;

    if iterCount == 0 then halt("DistributedIters: Guided iterator (leader): ",
                                "the range is empty");

    type cType = c.type;
    const denseRange:cType = densify(c,c);

    if iterCount == 1
       || numTasks == 1 && numLocales == 1
    then
    {
      if debugDistributedIters
      then writeln("DistributedIters: Guided iterator (leader): serial ",
                   "execution due to insufficient work or compute resources");
      yield (denseRange,);
    }
    else
    {
      const numWorkerLocales = workerLocales.size;
      const denseRangeHigh:int = denseRange.high;
      const masterLocale = here.locale;

      const actualWorkerLocales =
        [L in workerLocales] if numLocales == 1
                                || !coordinated
                                || L != masterLocale
                             then L;
      const numActualWorkerLocales = actualWorkerLocales.size;

      // The guided iterator stage (determines next subrange index and size).
      var guidedStageCount:atomic int;

      if infoDistributedIters then
      {
        const actualWorkerLocaleIds = [L in actualWorkerLocales] L.id:string;
        const actualWorkerLocaleIdsSorted = actualWorkerLocaleIds.sorted();
        writeln("DistributedIters: distributedGuided:");
        writeln("  coordinated = ", coordinated);
        writeln("  numLocales = ", numLocales);
        writeln("  numWorkerLocales = ", numWorkerLocales);
        writeln("  numActualWorkerLocales = ", numActualWorkerLocales);
        writeln("  masterLocale.id = ", masterLocale.id);
        writeln("  actualWorkerLocaleIds = [ ",
                ", ".join(actualWorkerLocaleIdsSorted),
                " ]");
      }

      var localeTimes:[0..#numLocales]real;
      var totalTime:Timer;
      if timeDistributedIters then totalTime.start();

      coforall L in actualWorkerLocales
      with (ref guidedStageCount, ref localeTimes)
      do on L
      {
        var localeTime:Timer;
        if timeDistributedIters then localeTime.start();

        var localeStage:int = guidedStageCount.fetchAdd(1);
        var localeRange:cType = guidedSubrange(denseRange,
                                               numActualWorkerLocales,
                                               localeStage);
        while localeRange.high <= denseRangeHigh
        {
          const denseLocaleRange:cType = densify(localeRange, localeRange);
          for denseTaskRangeTuple in DynamicIters.guided(tag=iterKind.leader,
                                                         localeRange,
                                                         numTasks)
          {
            const taskRange:cType = unDensify(denseTaskRangeTuple(0),
                                              localeRange);
            if debugDistributedIters
            then writeln("DistributedIters: Guided iterator (leader): ",
                         here.locale, ": yielding ", unDensify(taskRange,c),
                         " (", taskRange.size,
                         "/", localeRange.size,
                         " locale-owned of ", iterCount,
                         " total) as ", taskRange);
            yield (taskRange,);
          }

          localeStage = guidedStageCount.fetchAdd(1);
          localeRange = guidedSubrange(denseRange,
                                       numActualWorkerLocales,
                                       localeStage);
        }

        if timeDistributedIters then
        {
          localeTime.stop();
          localeTimes[here.id] = localeTime.elapsed();
        }
      }

      if timeDistributedIters then
      {
        totalTime.stop();
        writeTimeStatistics(totalTime.elapsed(), localeTimes, coordinated);
      }
    }
  }
}

// Zippered follower.
pragma "no doc"
iter distributedGuided(param tag:iterKind,
                       c,
                       numTasks:int,
                       parDim:int=0,
                       minChunkSize:int,
                       coordinated:bool,
                       workerLocales=Locales,
                       followThis)
where tag == iterKind.follower
{
  compilerAssert(isDomain(c) || isRange(c),
                 ("DistributedIters: Guided iterator (follower): must use a "
                  + "valid domain or range"),
                 1);
  const current = if isDomain(c)
                  then c.these(tag=iterKind.follower, followThis=followThis)
                  else unDensify(followThis(0), c);

  if debugDistributedIters
  then writeln("DistributedIters: Guided iterator (follower): ", here.locale,
               ": received ",
               if isDomain(c) then "domain " else "range ",
               followThis, " (", current.size,
               "/", c.size, "); shifting to ", current);

  for i in current do yield i;
}

/*
  Helpers.
*/

// Dynamic subrange calculation.
/*
  :arg c: The range from which to retrieve a dynamic subrange.
  :type c: `range(?)`

  :arg chunkSize: The chunk size. Must be positive.
  :type chunkSize: `int`

  :arg stage: The number of dynamic subranges to skip before returning a
    dynamic subrange.
  :type stage: `int`

  :returns: A subrange of ``c``.

  This function takes a range, a chunk size, and a stage, and simulates
  performing OpenMP's dynamic schedule on the range with the given chunk
  size until reaching the given stage. It then returns the subrange that the
  dynamic schedule would have produced at that point.
*/
private proc dynamicSubrange(c:range(?),
                             chunkSize:int,
                             stage:int)
{
  const low:int = (c.low + (stage * chunkSize));
  const potentialHigh:int = (low + (chunkSize - 1));
  const high:int = min(potentialHigh, c.high);
  const subrange:c.type = (low..high);
  return subrange;
}

// Guided subrange calculation.
/*
  :arg c: The range from which to retrieve a guided subrange.
  :type c: `range(?)`

  :arg workerCount: The number of workers (locales, tasks) to assume are
    working on ``c``. This (along with stage) determines the subrange length.
  :type workerCount: `int`

  :arg stage: The number of guided subranges to skip before returning a
    guided subrange.
  :type stage: `int`

  :arg minChunkSize: The smallest allowable chunk size. Must be positive.
    Defaults to 1.
  :type minChunkSize: `int`

  :returns: A subrange of ``c``.

  This function takes a range, a worker count, and a stage, and simulates
  performing OpenMP's guided schedule on the range with the given worker
  count until reaching the given stage. It then returns the subrange that the
  guided schedule would have produced at that point. The simulation overhead
  is insignificant.
*/
private proc guidedSubrange(c:range(?),
                            workerCount:int,
                            stage:int,
                            minChunkSize:int=1)
{
  assert(workerCount > 0, ("DistributedIters: guidedSubrange: "
                           + "'workerCount' must be positive"));
  const cLength = c.size;
  var low:int = c.low;
  var chunkSize:int = (cLength / workerCount);
  var remainder:int = (cLength - chunkSize);
  for unused in (1..#stage)
  {
    low += chunkSize;
    chunkSize = (remainder / workerCount);
    chunkSize = if (chunkSize >= minChunkSize)
                then chunkSize
                else minChunkSize;
    remainder -= chunkSize;
  }
  const subrange:c.type = (low..#chunkSize);
  return subrange;
}

// Per-locale time statistics.
/*
  :arg wallTime: The wall time statistic.
  :type wallTime: `real`

  :arg localeTimes: Time statistic per locale.
  :type localeTimes: `[]real`

  :arg coordinated: Whether the time statistics are from coordinated mode.
  :type coordinated: `bool`

  This function takes a real wall time (only for writing out), a per-locale
  real time array, and a coordinated boolean, and writes out a formatted
  performance timing summary: Total time for each locale, and locale time
*/
private proc writeTimeStatistics(wallTime:real,
                                 localeTimes:[]real,
                                 coordinated:bool)
{
  const low:int = if coordinated && (numLocales > 1)
                  then 1
                  else 0;
  const nLocales:int = if coordinated && (numLocales > 1)
                       then (numLocales - 1)
                       else numLocales;
  var localeMeanTime,localeStdDev,localeTotalTime:real;
  var localeTimesFormatted:string;

  const localeRange:range = low..#nLocales;
  for i in localeRange
  {
    const localeTime = localeTimes[i];
    localeTotalTime += localeTime;
    localeTimesFormatted += (i:string + ": " + localeTime:string);
    localeTimesFormatted += if i == localeRange.high
                            then ""
                            else ", ";
  }
  localeMeanTime = (localeTotalTime / nLocales);

  for i in localeRange
  do localeStdDev += ((localeTimes[i] - localeMeanTime) ** 2);
  localeStdDev = ((localeStdDev / nLocales) ** (1.0 / 2.0));

  writeln("DistributedIters: total time by locale: ", localeTimesFormatted);
  writeln("DistributedIters: wall time: ", wallTime);
  writeln("DistributedIters: locale time (total, mean, stddev): (",
          localeTotalTime, ", ",
          localeMeanTime, ", ",
          localeStdDev, ").");
}

} // End of module.
