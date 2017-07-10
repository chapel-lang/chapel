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

/*
  This module [eventually] contains several iterators that can be used to drive
  a `forall` loop by distributing iterations for a range, domain, or array.

  Part of a 2017 Cray summer intern project by Sean I. Geronimo Anderson
  (sgeronimo@cray.com) as mentored by Ben Harshbarger (bharshbarg@cray.com).
*/
module DistributedIters
{

// Toggle debugging output.
config param debugDistributedIters:bool=false;

// Guided Distributed Iterator.
/*
  :arg c: The range to iterate over. The length of the range must be greater
          than zero.
  :type c: `range(?)`

  :arg numTasks: The number of tasks to use. Must be >= zero. If this argument
                 has the value 0, the iterator will use the value indicated by
                 ``dataParTasksPerLocale``.
  :type numTasks: `int`

  :arg minChunkSize: The smallest allowable chunk size. Must be >= zero. If
                     this argument has the value 0, the iterator will use the
                     input range length divided by ``numLocales``.
  :type minChunkSize: `int`

  :arg coordinated: Have locale 0 coordinate task distribution only; disallow
                    it from receiving work. (If true and multi-locale.)
  :type coordinated: `bool`

  :yields: Indices in the range ``c``.

  This iterator is equivalent to a distributed version of the guided policy of
  OpenMP: Given an input range ``c``, each locale (except the calling locale)
  receives chunks of approximately exponentially decreasing size, until the
  remaining iterations reaches a minimum value, ``minChunkSize``, or there are
  no remaining iterations in ``c``. The chunk size is the number of unassigned
  iterations divided by the number of locales. Each locale then distributes
  sub-chunks as tasks, where each sub-chunk size is the number of unassigned
  local iterations divided by the number of tasks, ``numTasks``, and decreases
  approximately exponentially to 1. The splitting strategy is therefore
  adaptive.

  This iterator is available for serial and zippered contexts.
*/
// Serial version.
iter guidedDistributed(c:range(?),
                       numTasks:int=0,
                       coordinated:bool=false)
{
  if debugDistributedIters
  then writeln("Serial guided iterator, working with range ", c);

  for i in c do yield i;
}

// Zippered version.
pragma "no doc"
iter guidedDistributed(param tag:iterKind,
                       c:range(?),
                       numTasks:int=0,
                       coordinated:bool=false)
where tag == iterKind.leader
{
  const iterCount=c.length;

  if iterCount == 0 then halt("The range is empty");

  type cType=c.type;
  const denseRange:cType=densify(c,c);

  if iterCount == 1 || numTasks == 1 && numLocales == 1
  then
  {
    if debugDistributedIters
    then writeln("Distributed guided iterator: serial execution due to ",
                 "insufficient work or compute resources");
    yield (denseRange,);
  }
  else
  {
    const denseRangeHigh:int = denseRange.high;
    const masterLocale = here.locale;
    const nLocales = if coordinated && numLocales > 1
                     then numLocales-1
                     else numLocales;
    var meitneriumIndex:atomic int;

    if debugDistributedIters
    then writeln("iterCount = ", iterCount,
                 ", nLocales = ", nLocales);

    coforall L in Locales
    with (ref meitneriumIndex) do
    on L do
    {
      if numLocales == 1
         || !coordinated
         || L != masterLocale // coordinated == true
      then
      {
        var localeStage:int = meitneriumIndex.fetchAdd(1);
        var localeRange:cType = guidedSubrange(denseRange,
                                               nLocales,
                                               localeStage);
        while localeRange.high <= denseRangeHigh do
        {
          const localeRangeHigh:int = localeRange.high;
          const nTasks = min(localeRange.length, defaultNumTasks(numTasks));
          var plutoniumIndex:atomic int;

          coforall tid in 0..#nTasks
          with (ref plutoniumIndex) do
          {
            var taskStage:int = plutoniumIndex.fetchAdd(1);
            var taskRange:cType = guidedSubrange(localeRange,
                                                 nTasks,
                                                 taskStage);
            while taskRange.high <= localeRangeHigh do
            {
              if debugDistributedIters
              then writeln("Distributed guided iterator (leader): ",
                           here.locale, ", tid ", tid, ": yielding ",
                           unDensify(taskRange,c),
                           " (", taskRange.length,
                           "/", localeRange.length,
                           " locale-owned of ", iterCount, " total)",
                           " as ", taskRange);
              yield (taskRange,);

              taskStage = plutoniumIndex.fetchAdd(1);
              taskRange = guidedSubrange(localeRange, nTasks, taskStage);
            }
          }
          localeStage = meitneriumIndex.fetchAdd(1);
          localeRange = guidedSubrange(denseRange, nLocales, localeStage);
        }
      }
    }
  }
}
pragma "no doc"
iter guidedDistributed(param tag:iterKind,
                       c:range(?),
                       numTasks:int,
                       coordinated:bool=false,
                       followThis)
where tag == iterKind.follower
{
  const current:c.type=unDensify(followThis(1),c);

  if debugDistributedIters
  then writeln("Distributed guided iterator (follower): ", here.locale, ": ",
               "received range ", followThis,
               " (", current.length, "/", c.length, ")",
               "; shifting to ", current);

  for i in current do yield i;
}

// Helpers.

private proc defaultNumTasks(nTasks:int)
{
  var dnTasks=nTasks;
  if nTasks == 0
  then
  {
    if dataParTasksPerLocale == 0
    then dnTasks=here.maxTaskPar;
    else dnTasks=dataParTasksPerLocale;
  }
  else if nTasks < 0 then halt("'numTasks' is negative");
  return dnTasks;
}

private proc guidedSubrange(c:range(?), workerCount:int, stage:int)
/*
  :arg c: The range from which to retrieve a guided subrange.
  :type c: `range(?)`

  :arg workerCount: The number of workers (locales, tasks) to assume are
                    working on ``c``. This (along with stage) determines the
                    subrange length.
  :type workerCount: `int`

  :arg stage: The number of guided subranges to skip before returning a guided
              subrange.
  :type stage: `int`

  :returns: A subrange of ``c``.

  This function takes a range, a worker count, and a stage, and simulates
  performing OpenMP's guided schedule on the range with the given worker count
  until reaching the given stage. It then returns the subrange that the guided
  schedule would have produced at that point.
*/
{
  assert(workerCount > 0, "'workerCount' must be positive");
  const cLength = c.length;
  var low:int = c.low;
  var chunkSize:int = cLength / workerCount;
  var remainder:int = cLength - chunkSize;
  for unused in 1..#stage do
  {
    low += chunkSize;
    chunkSize = remainder / workerCount;
    chunkSize = if chunkSize >= 1
                then chunkSize
                else 1;
    remainder -= chunkSize;
  }
  const subrange:c.type = low..#chunkSize;
  return subrange;
}

} // End of module.
