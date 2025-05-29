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
module TwoArrayPartitioning {
  private use Math;
  public use List only list;
  import Sort.{ShellSort, MSBRadixSort};
  import Sort.{RadixSortHelp, ShallowCopy};
  import Sort;
  use MSBRadixSort;

  private param debug = false;
  param maxBuckets = 512;

  record TwoArraySortTask {
    var start: int;
    var size: int;
    var startbit: int;
    var inA: bool;
    var doSort: bool;

    proc init() { }
    // To make sure all fields are specified
    proc init(start:int, size:int, startbit:int, inA:bool, doSort:bool) {
      this.start = start;
      this.size = size;
      this.startbit = startbit;
      this.inA = inA;
      this.doSort = doSort;
    }
  }

  record TwoArrayBucketizerPerTaskState {
    var localCounts: [0..#maxBuckets] int;
  }

  proc defaultNumTasks() {
    if __primitive("task_get_serial") {
      return 1;
    }

    const tasksPerLocale = dataParTasksPerLocale;
    const ignoreRunning = dataParIgnoreRunningTasks;
    var nTasks = if tasksPerLocale > 0 then tasksPerLocale else here.maxTaskPar;
    if !ignoreRunning {
      const otherTasks = here.runningTasks() - 1; // don't include self
      nTasks = if otherTasks < nTasks then (nTasks-otherTasks):int else 1;
    }

    return nTasks;
  }

  record TwoArrayBucketizerSharedState {
    var nTasks:int = defaultNumTasks();
    var countsSize:int = nTasks*maxBuckets;

    type bucketizerType;
    var bucketizer: bucketizerType; // contains e.g. sample

    // globalCounts stores counts like this:
    //   count for bin 0, task 0
    //   count for bin 0, task 1
    //   ...
    //   count for bin 1, task 0
    //   count for bin 1, task 1
    // i.e. bin*nTasks + taskId
    var globalCounts:[0..#countsSize] int;
    var globalEnds:[0..#countsSize] int;

    var localState:[0..#nTasks] TwoArrayBucketizerPerTaskState;

    // Set by bucketizer, used by sorter
    var counts:[0..#maxBuckets] int;

    // Used by sorter
    var ends:[0..#maxBuckets] int;
    var outputInA:[0..#maxBuckets] bool;

    // start, end, isInA
    // were [0..-1]
    var bigTasks: list(TwoArraySortTask);
    var smallTasks: list(TwoArraySortTask);

    var baseCaseSize:int = 16;
    var sequentialSizePerTask:int = 4096;
    var endbit:int = max(int);

    proc init(type bucketizerType) {
      this.bucketizerType = bucketizerType;
    }

    proc init(in bucketizer,
              baseCaseSize: int = 16,
              sequentialSizePerTask: int = 4096,
              endbit: int = max(int)) {
      this.bucketizerType = bucketizer.type;
      this.bucketizer = bucketizer;
      this.baseCaseSize = baseCaseSize;
      this.sequentialSizePerTask = sequentialSizePerTask;
      this.endbit = endbit;
    }
  }

  // Stores the data in dst in buckets according to the bucketizer.
  // (e.g. sorted by the next digit in radix sort)
  // Counts per bin are stored in state.counts. Other data in
  // state is used locally by this routine or used elsewhere
  // If allowSkipahead=true, and the count determines everything is
  // in just one bin, startbit can be modified and the bucketize
  // will reflect a later startbit.
  proc bucketize(start_n: int, end_n: int, ref dst:[], src:[],
                 ref state: TwoArrayBucketizerSharedState,
                 criterion, inout startbit:int,
                 allowSkipahead=false) {

    if debug then
      writeln("bucketize ", start_n..end_n, " startbit=", startbit);

    type idxType = dst.idxType;
    const nBuckets = state.bucketizer.getNumBuckets();
    const n = end_n - start_n + 1;
    const nTasks = if n >= state.nTasks then state.nTasks else 1;
    assert(nTasks > 0);

    if n == 0 {
      state.counts = 0;
      return;
    }

    // Divide the input into nTasks chunks.
    const countsSize = nTasks * nBuckets;
    const blockSize = divCeil(n, nTasks);
    const nBlocks = divCeil(n, blockSize);

    // Count
    coforall tid in 0..#nTasks with (ref state) {
      var start = start_n + tid * blockSize;
      var end = start + blockSize - 1;
      if end > end_n {
        end = end_n;
      }

      if debug then
        writeln("tid ", tid, " considering ", start..end);

      ref counts = state.localState[tid].localCounts;
      for bin in 0..#nBuckets {
        counts[bin] = 0;
      }
      for (_,bin) in state.bucketizer.classify(src, start, end,
                                               criterion, startbit) {
        counts[bin] += 1;
      }
      // Now store the counts into the global counts array
      foreach bin in 0..#nBuckets with (ref state) {
        state.globalCounts[bin*nTasks + tid] = counts[bin];
      }
    }

    // Compute the total counts for the next check and for use
    // after this function returns.
    ref counts = state.counts;
    forall bin in 0..#nBuckets with (ref counts) {
      var total = 0;
      for tid in 0..#nTasks {
        total += state.globalCounts[bin*nTasks + tid];
      }
      counts[bin] = total;
    }

    if !state.bucketizer.isSampleSort && allowSkipahead {
      // If the data parts we gathered all have the same leading bits,
      // we can skip ahead immediately to the next count step.
      //
      // Check: was there actually only one bin with data?
      var onlyBin: int = -1;
      for bin in 0..#nBuckets {
        var total = counts[bin];
        if total == 0 {
          // ok, continue
        } else if total == n {
          // everything is in one bin, so we can stop
          onlyBin = bin;
          break;
        } else {
          // a bin contained not 0 and not n,
          // so this check is done
          break;
        }
      }

      if onlyBin >= 0 {
        // TODO: would it help performance to compute min and max
        // here and reset startbit according to these?
        // Or, would it be better to compute min and max word
        // in the above loop? (similar to 'ubits' in msbRadixSort)

        // compute the next start bit since there was no need to sort
        // at this start bit
        startbit = state.bucketizer.getNextStartBit(startbit);
        // stop if the next startbit is too far
        if startbit > state.endbit then
          return;

        // stop if it's a bin that doesn't need sorting
        // (bin for end-of-string indicator)
        if !state.bucketizer.getBinsToRecursivelySort().contains(onlyBin) then
          return;

        // start over with the new start bit
        bucketize(start_n, end_n, dst, src, state, criterion,
                  startbit, allowSkipahead=true);
        return; // note: startbit is inout so will change at call site
      }
    }

    // Step 2: scan
    state.globalEnds = (+ scan state.globalCounts) + start_n;

    if debug {
      for bin in 0..#nBuckets {
        for tid in 0..#nTasks {
          var gb = bin*nTasks+tid;
          if state.globalCounts[gb] != 0 {
            writeln("tid ", tid, " count[", bin, "] = ", state.globalCounts[gb],
                    " end = ", state.globalEnds[gb] - 1);
          }
        }
      }
    }

    // Step 3: distribute
    coforall tid in 0..#nTasks with (ref state) {
      var start = start_n + tid * blockSize;
      var end = start + blockSize - 1;
      if end > end_n {
        end = end_n;
      }

      ref nextOffsets = state.localState[tid].localCounts;
      // initialize nextOffsets
      for bin in 0..#nBuckets {
        var globalBin = bin*nTasks+tid;
        nextOffsets[bin] = if globalBin > 0
                           then state.globalEnds[globalBin-1]
                           else start_n;
        if debug {
          if state.globalCounts[globalBin] != 0 {
            writeln("tid ", tid, " nextOffsets[", bin, "] = ", nextOffsets[bin]);
          }
        }
      }

      for (i,bin) in state.bucketizer.classify(src, start, end,
                                               criterion, startbit) {
        // Store it in the right bin
        ref next = nextOffsets[bin];
        if debug {
          writeln("tid ", tid, " dst[", next, "] = src[", i, "] bin ", bin);
        }
        ShallowCopy.shallowCopy(dst, next, src, i:idxType, 1:idxType);
        next += 1;
      }
    }
  }
  proc testBucketize(start_n: int, end_n: int, ref dst:[], src:[],
                     bucketizer, criterion, startbit:int) {

    var state = new TwoArrayBucketizerSharedState(bucketizer=bucketizer);

    var myStartBit = startbit;
    bucketize(start_n, end_n, dst, src, state, criterion, myStartBit);

    return state.counts;
  }


 proc partitioningSortWithScratchSpaceHandleSampling(
          start_n:int, end_n:int, ref A:[], ref Scratch:[],
          ref state: TwoArrayBucketizerSharedState,
          criterion, startbit:int):void {
    // If we are doing a sample sort, we need to gather a fresh sample.
    // (Otherwise we'll never be able to solve recursive subproblems,
    //  as if in quicksort we never chose a new pivot).
    if state.bucketizer.isSampleSort {
      var n = 1 + end_n - start_n;
      var logNumBuckets = computeLogBucketSize(n);
      var numBuckets = 1 << logNumBuckets;
      var sampleStep = chooseSampleStep(n, logNumBuckets);
      var sampleSize = sampleStep * numBuckets - 1;

      if sampleSize >= n {
        if debug then
          writeln("Reducing sample size because it was too big");
        sampleSize = max(1, n/2);
      }

      // select the sample
      putRandomSampleAtArrayStart(start_n, end_n, A, sampleSize);

      if debug then
        writeln("recursing to sort the sample");

      // sort the sample using the usual sorting algorithm
      Sort.sort(A[start_n..#sampleSize], comparator=criterion);

      if debug {
        RadixSortHelp.checkSorted(start_n, start_n + sampleSize - 1, A, criterion, startbit);
      }

      createSplittersFromSample(A,
                                state.bucketizer, criterion,
                                start_n, sampleSize, sampleStep, numBuckets);
      if debug {
        writeln("sample bucketizer ", state.bucketizer);
        writef("A %i %i A=%?\n", start_n, end_n, A[start_n..end_n]);
      }
    }
  }

  // Sorts the data in A.
  proc partitioningSortWithScratchSpace(
          start_n:int, end_n:int, ref A:[], ref Scratch:[],
          ref state: TwoArrayBucketizerSharedState,
          criterion, startbit:int):void {

    if startbit > state.endbit then
      return;

    if end_n - start_n < state.baseCaseSize {
      ShellSort.shellSortMoveEltsIntIdx(A, criterion, start=start_n, end=end_n);
      return;
    }

    if debug {
      writeln("partitioningSortWithScratchSpace(", start_n, ",", end_n, ")");
      writef("A %i %i A=%?\n", start_n, end_n, A[start_n..end_n]);
    }


    const n = (end_n - start_n + 1);
    const maxSequentialSize = max(n / state.nTasks,
                                  state.nTasks*state.sequentialSizePerTask);

    state.bigTasks.pushBack(new TwoArraySortTask(start_n, n, startbit, inA=true, doSort=true));
    assert(state.bigTasks.size == 1);
    assert(state.smallTasks.size == 0);

    while !state.bigTasks.isEmpty() {
      const task = state.bigTasks.popBack();
      const taskEnd = task.start + task.size - 1;
      var taskStartBit = task.startbit;

      assert(task.doSort);

      if debug then {
        writeln("doing big task ", task.start..taskEnd);
      }

      if task.inA {
        partitioningSortWithScratchSpaceHandleSampling(
              task.start, taskEnd, A, Scratch,
              state, criterion, taskStartBit);

        // Count and partition
        bucketize(task.start, taskEnd, Scratch, A, state,
                  criterion, taskStartBit, allowSkipahead=true);
        // bucketized data now in Scratch
        if debug {
          writef("pb %i %i Scratch=%?\n", task.start, taskEnd, Scratch[task.start..taskEnd]);
        }
      } else {
        partitioningSortWithScratchSpaceHandleSampling(
              task.start, taskEnd, Scratch, A,
              state, criterion, taskStartBit);

        // Count and partition
        bucketize(task.start, taskEnd, A, Scratch, state,
                  criterion, taskStartBit, allowSkipahead=true);
        // bucketized data now in A
        if debug {
          writef("pb %i %i A=%?\n", task.start, taskEnd, A[task.start..taskEnd]);
        }
      }
      const nowInA = !task.inA;

      // Compute the bucket ends
      state.ends = (+ scan state.counts) + task.start;

      // enqueue any sorting tasks not yet completed
      const nBuckets = state.bucketizer.getNumBuckets();
      for bin in 0..#nBuckets {
        const binSize = state.counts[bin];
        const binStart = state.ends[bin] - binSize;
        const binEnd = binStart + binSize - 1;
        const binStartBit = state.bucketizer.getNextStartBit(taskStartBit);

        const sortit =
          binSize > 1 && // have 2 or more elements to sort
          binStartBit <= state.endbit && // have bits to sort
          state.bucketizer.getBinsToRecursivelySort().contains(bin);

        if binSize == 0 {
          // Do nothing
        } else if nowInA && !sortit {
          // no need to copy it or to sort it
        } else if !nowInA && !sortit {
          // Enqueue a small task to do the copy but not sort it
          // TODO: handle large copies in big tasks,
          // or enqueue several tasks here
          state.smallTasks.pushBack(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));
        } else if //binStartBit > state.endbit ||
                  //binStart >= binEnd ||
                  binSize <= maxSequentialSize {
          if debug && binSize > 0 {
            writeln("handling bin ", bin, " ", binStart..binEnd, " as small");
          }

          // Enqueue a small subproblem to sort and possibly copy.
          state.smallTasks.pushBack(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));

        } else {
          if debug && binSize > 0 {
            writeln("handling bin ", bin, " ", binStart..binEnd, " as big");
          }

          // Enqueue a big subproblem to sort
          state.bigTasks.pushBack(
            new TwoArraySortTask(binStart, binSize, binStartBit, nowInA, sortit));
        }
      }
    }

    // Now handle any small tasks.

    // TODO: sort small tasks by size

    forall task in state.smallTasks with (ref A) {
      const size = task.size;
      const taskEnd = task.start + size - 1;
      if debug {
        writef("doing small task %i %i A=%?\n", task.start, taskEnd, A[task.start..taskEnd]);
      }

      if !task.inA {
        ShallowCopy.shallowCopy(A, task.start, Scratch, task.start, size);
      }

      if task.doSort {
        // Sort it serially.
        // Note that the subproblems here are on the order of 500,000 elements
        // because the two-array method will create small subproblems as soon
        // as it does not seem useful to use parallelism to sort.
        // Because of this, it matters to use a radix sort here.
        // It also seems to matter to use an in-place algorithm here,
        // but I am not completely confident of that.
        msbRadixSort(A, task.start:A.idxType, taskEnd:A.idxType,
                     criterion,
                     task.startbit, state.endbit,
                     settings=new MSBRadixSortSettings(alwaysSerial=true));
      }
    }

    if debug {
      writef("ps %i %i A=%?\n", start_n, end_n, A[start_n..end_n]);
      writef("ps %i %i Scratch=%?\n", start_n, end_n, Scratch[start_n..end_n]);
      RadixSortHelp.checkSorted(start_n, end_n, A, criterion, startbit);
    }
  }
}
