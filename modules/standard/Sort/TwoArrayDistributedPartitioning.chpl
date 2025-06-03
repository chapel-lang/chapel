/*
 * Copyright 2020-2025 Hewlett Packard Enterprise Development LP
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



@chpldoc.nodoc
module TwoArrayDistributedPartitioning {
  public use super.TwoArrayPartitioning;
  private use BlockDist;
  private use Math;
  public use List only list;
  import Sort.{ShellSort, MSBRadixSort, QuickSort};
  import Sort.{RadixSortHelp, ShallowCopy, keyComparator};
  use MSBRadixSort;

  private param debugDist = false;

  record TwoArrayDistSortPerBucketTaskStartComparator: keyComparator {
    proc key(elt: TwoArrayDistSortPerBucketTask) {
      return elt.start;
    }
  }

  record TwoArrayDistSortPerBucketTask {
    var start: int;
    var size: int;
    var startbit: int; // could be moved to TwoArrayDistSortTask

    var firstLocaleId: int;
    var lastLocaleId: int;
    var useSecondState: bool;

    proc isEmpty() {
      return size == 0;
    }

    // create an easy-to-identify empty task
    proc init() {
      this.start = -1;
      this.size = 0;
      this.startbit = max(int);
      this.firstLocaleId = max(int);
      this.lastLocaleId = -1;
      this.useSecondState = false;
    }
    // To make sure all fields are specified
    proc init(start:int, size:int, startbit:int,
              firstLocaleId:int, lastLocaleId:int,
              useSecondState:bool) {
      this.start = start;
      this.size = size;
      this.startbit = startbit;
      this.firstLocaleId = firstLocaleId;
      this.lastLocaleId = lastLocaleId;
      this.useSecondState = useSecondState;
    }
    proc nLocales() {
      return lastLocaleId - firstLocaleId + 1;
    }
    // yields tuples of (loc, tid) for the locales involved with this bucket
    iter localeAndIds(A) {
      const ref tgtLocs = A.targetLocales();
      foreach tid in firstLocaleId..lastLocaleId {
        const loc = tgtLocs[tid];
        yield (loc, tid);
      }
    }
    // yield the other ids but do so in an order that depends on myId
    //  myId + 1 will be the first id.
    iter otherIds(myId) {
      const nIds = lastLocaleId-firstLocaleId+1;
      foreach i in 1..#nIds {
        yield firstLocaleId + ((myId + i) % nIds);
      }
    }
    iter otherIds(param tag: iterKind, myId) where tag == iterKind.standalone {
      const nIds = lastLocaleId-firstLocaleId+1;
      forall i in 1..#nIds {
        yield firstLocaleId + ((myId + i) % nIds);
      }
    }
  }

  record TwoArrayDistSortTask : writeSerializable {
    var tasks: list(TwoArrayDistSortPerBucketTask);

    // Create an empty one
    proc init() { }
    // Create one with just 1 bucket
    proc init(start:int, size:int, startbit:int,
              firstLocaleId:int, lastLocaleId:int) {
      var t = new TwoArrayDistSortPerBucketTask(start, size, startbit,
                                                firstLocaleId, lastLocaleId,
                                                false);
      assert(!t.isEmpty());
      init this;
      tasks.pushBack(t);
    }
    proc serialize(writer, ref serializer) throws {
      writer.write("TwoArrayDistSortTask");
      for t in tasks {
        writer.write(" ");
        writer.write(t);
      }
    }

    proc isEmpty() {
      return tasks.isEmpty();
    }
    // yield (loc, locId, task) for each non-empty bucket
    // loc is the locale "owning" the bucket.
    iter localesAndTasks(A) {
      foreach t in tasks {
        const locId = t.firstLocaleId;
        const loc = A.targetLocales()[locId];
        yield (loc, locId, t);
      }
    }
  }

  record TwoArrayDistributedBucketizerStatePerLocale {
    type bucketizerType;

    var compat:TwoArrayBucketizerSharedState(bucketizerType);

    var countsSize:int = numLocales*maxBuckets;
    // globalCounts stores counts like this:
    //   count for bin 0, locale 0
    //   count for bin 0, locale 1
    //   ...
    //   count for bin 1, locale 0
    //   count for bin 1, locale 1
    // i.e. bin*nTasks + localeId
    var globalCounts:[0..#countsSize] int;
    var globalEnds:[0..#countsSize] int;
  }

  record TwoArrayDistributedBucketizerSharedState {
    type bucketizerType;

    var numLocales:int;
    var perLocale = blockDist.createArray(0..#numLocales,
        TwoArrayDistributedBucketizerStatePerLocale(bucketizerType));

    const baseCaseSize:int;
    const distributedBaseCaseSize:int;
    const endbit:int = max(int);

    const countsSize:int = numLocales*maxBuckets;

    // globalCounts stores counts like this:
    //   count for bin 0, locale 0
    //   count for bin 0, locale 1
    //   ...
    //   count for bin 1, locale 0
    //   count for bin 1, locale 1
    // i.e. bin*nTasks + localeId
    var globalCounts:[0..#countsSize] int;
    var globalEnds:[0..#countsSize] int;

    proc ref postinit() {
      // Copy some vars to the compat
      for p in perLocale {
        p.compat.baseCaseSize = baseCaseSize;
        p.compat.endbit = endbit;
        assert(p.compat.nTasks > 0);
        assert(p.compat.countsSize > 0);
      }
    }
  }

  private proc distributedPartitioningSortWithScratchSpaceBaseCase(
          start_n:int, end_n:int, ref A:[], ref Scratch:[],
          ref compat: TwoArrayBucketizerSharedState,
          criterion, startbit:int):void {

    if startbit > compat.endbit then
      return;

    const n = end_n - start_n + 1;

    const localSubdomain = A.localSubdomain();
    const curDomain = {start_n..end_n};
    const intersect = curDomain[localSubdomain];
    if curDomain == intersect {
      local {
        if n > compat.baseCaseSize {
          compat.bigTasks.clear();
          compat.smallTasks.clear();
          partitioningSortWithScratchSpace(start_n, end_n,
                       A.localSlice(curDomain), Scratch.localSlice(curDomain),
                       compat, criterion,
                       startbit);
        } else {
          ShellSort.shellSortMoveEltsIntIdx(A.localSlice(curDomain), criterion, start=start_n, end=end_n);
        }
      }
    } else {
      const size = end_n-start_n+1;
      // Copy it to one locale
      var LocalA:[start_n..end_n] A.eltType;
      var LocalScratch:[start_n..end_n] A.eltType;
      ShallowCopy.shallowCopy(LocalA, start_n, A, start_n, size);
      // Sort it
      if n > compat.baseCaseSize {
        compat.bigTasks.clear();
        compat.smallTasks.clear();
        partitioningSortWithScratchSpace(start_n, end_n,
                     LocalA, LocalScratch,
                     compat, criterion,
                     startbit);
      } else {
        ShellSort.shellSortMoveEltsIntIdx(LocalA, criterion, start=start_n, end=end_n);
      }
      // Copy it back
      ShallowCopy.shallowCopy(A, start_n, LocalA, start_n, size);
    }

    if debugDist {
      RadixSortHelp.checkSorted(start_n, end_n, A, criterion);
    }
    return;
  }


  private proc distributedPartitioningSortWithScratchSpaceHandleSampling(
          start_n:int, end_n:int, ref A:[], ref Scratch:[],
          ref state: TwoArrayDistributedBucketizerSharedState,
          criterion, startbit:int):void {
    // If we are doing a sample sort, we need to gather a fresh sample.
    // (Otherwise we'll never be able to solve recursive subproblems,
    //  as if in quicksort we never chose a new pivot).
    if state.perLocale[0].compat.bucketizer.isSampleSort {
      var n = 1 + end_n - start_n;
      var logNumBuckets = computeLogBucketSize(n);
      var numBuckets = 1 << logNumBuckets;
      var sampleStep = chooseSampleStep(n, logNumBuckets);
      var sampleSize = sampleStep * numBuckets - 1;

      if sampleSize >= n {
        if debugDist then
          writeln("Reducing sample size because it was too big");
        sampleSize = max(1, n/2);
      }

      // select the sample
      putRandomSampleAtArrayStart(start_n, end_n, A, sampleSize);

      if debugDist then
        writeln("recursing to sort the sample");

      // sort the sample

      // TODO: make it adjustable in the settings
      if sampleSize <= 1024*1024 {
        // base case sort, parallel OK
        distributedPartitioningSortWithScratchSpaceBaseCase(start_n, start_n + sampleSize - 1,
                                                            A, Scratch,
                                                            state, criterion,
                                                            startbit);
      } else {
        distributedPartitioningSortWithScratchSpace(start_n, start_n + sampleSize - 1,
                                                    A, Scratch,
                                                    state, criterion, startbit);
      }
      if debugDist {
        RadixSortHelp.checkSorted(start_n, start_n + sampleSize - 1, A, criterion, startbit);
      }

      createSplittersFromSample(A,
                                state.perLocale[0].compat.bucketizer, criterion,
                                start_n, sampleSize, sampleStep, numBuckets);
      if debugDist {
        writeln("sample bucketizer ", state.perLocale[0].compat.bucketizer);
        writef("A %i %i A=%?\n", start_n, end_n, A[start_n..end_n]);
      }

      // Now copy the bucketizer sample to all other locales
      for lid in 1..state.numLocales {
        state.perLocale[lid].bucketizer = state.perLocale[0].bucketizer;
      }
    }
  }

  proc distributedPartitioningSortWithScratchSpace(
          start_n:int, end_n:int, ref A:[], ref Scratch:[],
          ref state1: TwoArrayDistributedBucketizerSharedState,
          ref state2: TwoArrayDistributedBucketizerSharedState,
          criterion, startbit:int): void {

    if !A.hasSingleLocalSubdomain() {
      compilerError("distributedPartitioningSortWithScratchSpace needs single local subdomain");
    }
    // TODO: assert that src and dst have the same distribution?

    if startbit > state1.endbit then
      return;

    // If it's really small, just sort it on Locale 0.
    if end_n - start_n < state1.distributedBaseCaseSize {
      ref compat = state1.perLocale[0].compat;
      distributedPartitioningSortWithScratchSpaceBaseCase(start_n, end_n,
                                                          A, Scratch,
                                                          compat, criterion,
                                                          startbit);
      return;
    }


    if debugDist {
      writeln("in distributed radix sort ", start_n, "..", end_n,
              " startbit ", startbit, " endbit ", state1.endbit);
    }

    // TODO: use something more like distributed bag for these
    var distTask: TwoArrayDistSortTask =
      new TwoArrayDistSortTask(start_n, end_n - start_n + 1,
                               startbit,
                               0, state1.numLocales-1);
    var nextDistTaskElts: list(TwoArrayDistSortPerBucketTask, parSafe=true);
    var smallTasksPerLocale = blockDist.createArray(0..#numLocales,
                                          list(TwoArraySortTask, parSafe=true));

    assert(!distTask.isEmpty());

    const nBuckets = state1.perLocale[0].compat.bucketizer.getNumBuckets();
    const nLocalesTotal = state1.numLocales;

    // Part A: Handle the "big" subproblems
    while true {
      if distTask.isEmpty() then break;

      if debugDist then
        writeln("Doing big task ", distTask);

      if debugDist {
        var usedLocales1:[0..#nLocalesTotal] bool;
        var usedLocales2:[0..#nLocalesTotal] bool;
        // check: non-overlapping locales are used by each bucket in distTask
        for t in distTask.tasks {
          if !t.isEmpty() {
            if t.useSecondState {
              for (_, tid) in t.localeAndIds(A) {
                assert(!usedLocales2[tid]); // means race condition would occur
                usedLocales2[tid] = true;
              }

            } else {
              for (_, tid) in t.localeAndIds(A) {
                assert(!usedLocales1[tid]); // means race condition would occur
                usedLocales1[tid] = true;
              }
            }
          }
        }
      }

      // TODO: put this call back in somewhere to support sample sort
      // ... or just rewrite sample sort.
      // Distributed sample sort should have sample size ~= numLocales,
      // but that would interfere with maxBuckets being param.
      // distributedPartitioningSortWithScratchSpaceHandleSampling(
      //       task.start, taskEnd, A, Scratch,
      //       state, criterion, task.startbit);

      coforall (bktLoc, bktLocId, bktTask) in distTask.localesAndTasks(A)
      with (ref state1, ref state2, ref nextDistTaskElts, ref smallTasksPerLocale) do
      on bktLoc {
        // Each bucket can run in parallel - this allows each
        // bucket to use nested coforalls to barrier.
        assert(!bktTask.isEmpty());
        const nLocalesBucket = bktTask.nLocales();
        ref state = if bktTask.useSecondState then state2 else state1;

        if debugDist then
          writeln(bktLocId, " doing big task component ", bktTask);

        coforall (loc,tid) in bktTask.localeAndIds(A) with (ref state) do
        on loc {
          const task = bktTask;

          // Step 1: Each locale sorts local portions into buckets
          // and shares those counts in globalCounts.
          // This uses perLocale[tid].compat.
          const taskStart = task.start;
          const taskEnd = task.start + task.size - 1;
          var taskStartBit = task.startbit;

          const localDomain = A.localSubdomain()[task.start..taskEnd];
          ref localSrc = A.localSlice(localDomain);
          ref localDst = Scratch.localSlice(localDomain);

          if debugDist then
            writeln(tid, " bucketizing local portion ", localDomain);

          bucketize(localDomain.low,
                    localDomain.high,
                    localDst, localSrc,
                    state.perLocale[tid].compat, criterion, taskStartBit);

          ref localCounts = state.perLocale[tid].compat.counts;

          if debugDist {
            var total = 0;
            for bin in 0..#nBuckets {
              if localCounts[bin] > 0 {
                total += localCounts[bin];
              }
            }
            assert(total == localDomain.size);
          }

          // Do an all-to-all to send the counts to everybody
          // To make this communication more efficient, temporarily store
          // in the order
          //     count for locale 0, bin 0
          //     count for locale 0, bin 1
          //     ...
          // (i.e. the transpose of the order needed for scan)
          const toIdx = maxBuckets * tid;
          ref perLocale = state.perLocale;
          forall dstTid in task.otherIds(tid) with (ref perLocale) {
            // perLocale[dstTid].globalCounts[toIdx..#maxBuckets] = localCounts;
            ShallowCopy.shallowCopyPutGet(
                perLocale[dstTid].globalCounts, toIdx,
                localCounts, 0, maxBuckets);
          }
        }
        // Now the data is in Scratch

        if debugDist {
          writef("%i after bucketize local portions, Scratch is %?\n",
              bktLocId,
              Scratch[bktTask.start..#bktTask.size]);
        }

        // ending coforall and recreating it is just a barrier

        coforall (loc,tid) in bktTask.localeAndIds(A) with (ref state) do
        on loc {
          const task = bktTask;
          const taskStart = task.start;
          const taskEnd = task.start + task.size - 1;

          const bktFirstLocale = bktTask.firstLocaleId;
          const bktLastLocale = bktTask.lastLocaleId;

          // Step 2: scan
          // Note that the globalCounts arrays are stored in transpose
          // order up until now to optimize communication, so we first
          // rearrange them.

          // Temporarily use the globalEnds array to reorder
          {
            ref globalCounts = state.perLocale[tid].globalCounts;
            ref globalEnds = state.perLocale[tid].globalEnds;

            // Compute the transpose
            forall (tid,bkt) in {0..#nLocalesTotal, 0..#maxBuckets} with (ref globalEnds) {
              var count = 0;
              if bktFirstLocale <= tid && tid <= bktLastLocale then
                count = globalCounts[tid*maxBuckets+bkt];

              globalEnds[bkt*nLocalesTotal+tid] = count;
            }
            globalCounts = globalEnds;

            // Do the scan itself
            globalEnds = (+ scan globalCounts) + task.start;
          }

          if debugDist && tid == bktLocId {
            var total = 0;
            for i in 0..#state.countsSize {
              if state.perLocale[bktLocId].globalCounts[i] != 0 {
                total += state.perLocale[bktLocId].globalCounts[i];
                writeln(tid, " state.globalCounts[", i, "]=",
                    state.perLocale[bktLocId].globalCounts[i]);
                writeln(tid, " state.globalEnds[", i, "]=",
                    state.perLocale[bktLocId].globalEnds[i]);
              }
            }
            assert(total == task.size);
          }

          // Step 3: distribute the keys to the src array according
          // to the globalEnds.
          // In particular, bin i from task j should be stored into
          // globalEnds[i*ntasks+j-1] .. globalEnds[i*ntasks+j] - 1
          // (because the scan is inclusive)

          const ref globalCounts = state.perLocale[tid].globalCounts;
          const ref globalEnds = state.perLocale[tid].globalEnds;
          const localSubdomain = A.localSubdomain()[task.start..taskEnd];
          // Compute localOffsets array based on the counts here
          // This could be written as a scan expression...
          ref localOffsets = state.perLocale[tid].compat.counts;
          {
            var offset = localSubdomain.low;
            for bin in 0..#nBuckets {
              localOffsets[bin] = offset;
              offset += globalCounts[bin*nLocalesTotal + tid];
            }
          }

          forall bin in 0..#nBuckets with (ref A) {
            var size = globalCounts[bin*nLocalesTotal + tid];
            if size > 0 {
              var localStart = localOffsets[bin];
              var localEnd = localStart + size - 1;
              var globalStart = if bin*nLocalesTotal+tid > 0
                                then globalEnds[bin*nLocalesTotal+tid-1]
                                else taskStart;
              var globalEnd = globalEnds[bin*nLocalesTotal+tid] - 1;
              if debugDist {
                writeln("bin ", bin, " tid ", tid, " range ", taskStart..taskEnd,
                        " A[", globalStart, "..", globalEnd, "] = Scratch[",
                        localStart, "..", localEnd, "]");
                assert(globalCounts[bin*nLocalesTotal+tid] ==
                    state.perLocale[bktLocId].globalCounts[bin*nLocalesTotal+tid]);
                assert(globalEnds[bin*nLocalesTotal+tid] ==
                    state.perLocale[bktLocId].globalEnds[bin*nLocalesTotal+tid]);
              }
              ShallowCopy.shallowCopy(A, globalStart, Scratch, localStart, size);
            }
          }
        }

        // aka barrier

        // now the data is all in A

        // Step 4: Add sub-tasks depending on if the bin is local or distributed
        // still.
        ref bktOwnerState = state.perLocale[bktLocId];

        for bin in bktOwnerState.compat.bucketizer.getBinsToRecursivelySort() {
          const binStart = if bin*nLocalesTotal > 0
                            then bktOwnerState.globalEnds[bin*nLocalesTotal-1]
                            else bktTask.start;
          const binEnd = bktOwnerState.globalEnds[bin*nLocalesTotal+nLocalesTotal-1] - 1;
          const binSize = binEnd - binStart + 1;
          const binStartBit = bktOwnerState.compat.bucketizer.getNextStartBit(bktTask.startbit);
          if binSize > 1 {
            var small = false;
            var theLocaleId = -1;

            // Compute the regions on the same locale as the first, last
            // elements in the bin.
            const firstLoc = A.domain.distribution.idxToLocale(binStart);
            const lastLoc = A.domain.distribution.idxToLocale(binEnd);
            const onFirstLoc = A.localSubdomain(firstLoc)[binStart..binEnd];
            const onLastLoc = A.localSubdomain(lastLoc)[binStart..binEnd];
            var theLocale = firstLoc;
            if onFirstLoc.size == binSize {
              // case 1: all elements are on firstLoc
              small = true;
            } else if binSize <= state.distributedBaseCaseSize {
              // case 2: size is small enough to do on 1 locale
              small = true;
            }
            theLocaleId = theLocale.id;
            assert(A.targetLocales()[theLocaleId] == theLocale);

            if debugDist then
              writeln(bktLocId,
                      " Recursive bin ", bin,
                      " start = ", binStart,
                      " size = ", binSize,
                      " startbit = ", binStartBit,
                      " small = ", small);

            if small {
              var small = new TwoArraySortTask(binStart, binSize, binStartBit,
                                               true, true);

              if debugDist then
                writeln(bktLocId, " Adding small task ", small);

              smallTasksPerLocale[theLocaleId].pushBack(small);
            } else {
              // Create a distributed sorting task
              var firstLocId = firstLoc.id;
              var lastLocId = lastLoc.id;
              assert(A.targetLocales()[firstLocId] == firstLoc);
              assert(A.targetLocales()[lastLocId] == lastLoc);
              if debugDist {
                // the above assumes something block-like.
                // check that the bin only involves firstLoc..lastLoc
                for (loc,tid) in zip(A.targetLocales(),0..) {
                  if !(firstLocId..lastLocId).contains(tid) {
                    assert(A.localSubdomain(loc)[binStart..binEnd].size == 0);
                  }
                }
              }

              var t = new TwoArrayDistSortPerBucketTask(
                                     binStart, binSize, binStartBit,
                                     firstLocId, lastLocId,
                                     useSecondState=false);

              if debugDist then
                writeln(bktLocId, " Adding big subtask", t);

              nextDistTaskElts.pushBack(t);
            }
          }
        }
      }

      // Update distTasks based on nextDistTaskElts
      sort(nextDistTaskElts,
           comparator=new TwoArrayDistSortPerBucketTaskStartComparator());

      // For each of those tasks, decide if they should use
      // counts1 or counts2
      var lastLocaleIdIn1 = -1;
      var lastLocaleIdIn2 = -1;
      for t in nextDistTaskElts {
        if lastLocaleIdIn1 < t.firstLocaleId {
          t.useSecondState = false;
          lastLocaleIdIn1 = t.lastLocaleId;
        } else if lastLocaleIdIn2 < t.firstLocaleId {
          t.useSecondState = true;
          lastLocaleIdIn2 = t.lastLocaleId;
        } else {
          halt("Algorithm Problem!");
        }
      }

      distTask.tasks = nextDistTaskElts;
      nextDistTaskElts.clear();
    }

    // Part B: Handle the "small" subproblems

    if debugDist then
      writef("After big tasks, A is: %?\n", A);

    // Always use state 1 for small subproblems...
    ref state = state1;
    coforall (loc,tid) in zip(A.targetLocales(),0..) with (ref state, ref smallTasksPerLocale) do
    on loc {
      // Get the tasks to sort here

      while true {
        if smallTasksPerLocale[tid].isEmpty() then break;
        const task = smallTasksPerLocale[tid].popBack();

        if debugDist then
          writeln(tid, " Doing small task ", task);

        // Run the local task
        ref compat = state.perLocale[tid].compat;
        const taskEnd = task.start + task.size - 1;

        distributedPartitioningSortWithScratchSpaceBaseCase(
            task.start, taskEnd,
            A, Scratch,
            compat, criterion, task.startbit);

        if debugDist {
          checkSorted(task.start, taskEnd, A, criterion);
          writef("%i after small sort, dst is %?\n", tid, A[task.start..taskEnd]);
        }
      }
    }

    if debugDist {
      writef("After small tasks, A is: %?\n", A);
      checkSorted(start_n, end_n, A, criterion);
    }
  }
}


