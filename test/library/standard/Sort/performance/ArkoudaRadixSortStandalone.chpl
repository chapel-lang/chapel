/* This is a version of Arkouda's RadixSortLSD
   that has been modified to be a self-contained test.
   It is based upon Arkouda commit ca6b25672913ef689c3c734d9e4d83be5bff821a. */
module ArkoudaRadixSortStandalone
{
    /*
    Bit width of digits for the LSD radix sort and related ops
     */ 
    config param bitsPerDigit = 16;
    private param numBuckets = 1 << bitsPerDigit;
    private param maskDigit = numBuckets-1;

    // these need to be const for comms/performance reasons

    // tasks per locale based on locale0
    config const numTasks = here.maxTaskPar;
    const Tasks = {0..#numTasks};

    
    use BlockDist;
    use CopyAggregation;
    use Random;
    use RangeChunk;
    use Time;
    use Sort;

    record KeysComparator: keyComparator {
      inline proc key(k) { return k; }
    }

    record KeysRanksComparator: keyComparator {
      inline proc key(kr) { const (k, _) = kr; return k; }
    }

    inline proc getDigit(key: uint, rshift: int, last: bool, negs: bool): int {
      return ((key >> rshift) & (maskDigit:uint)):int;
    }

    // calculate sub-domain for task
    inline proc calcBlock(task: int, low: int, high: int) {
        var totalsize = high - low + 1;
        var div = totalsize / numTasks;
        var rem = totalsize % numTasks;
        var rlow: int;
        var rhigh: int;
        if (task < rem) {
            rlow = task * (div+1) + low;
            rhigh = rlow + div;
        }
        else {
            rlow = task * div + rem + low;
            rhigh = rlow + div - 1;
        }
        return {rlow .. rhigh};
    }

    // calc global transposed index
    // (bucket,loc,task) = (bucket * numLocales * numTasks) + (loc * numTasks) + task;
    inline proc calcGlobalIndex(bucket: int, loc: int, task: int): int {
        return ((bucket * numLocales * numTasks) + (loc * numTasks) + task);
    }

    /* Radix Sort Least Significant Digit
       In-place radix sort a block distributed array
       comparator is used to extract the key from array elements
     */
    proc radixSortLSDCore(ref a:[?aD] ?t, nBits, negs, comparator) {
        var temp = blockDist.createArray(aD, a.eltType);
        temp = a;

        // create a global count array to scan
        var globalCounts = blockDist.createArray(0..<(numLocales*numTasks*numBuckets), int);

        // loop over digits
        for rshift in {0..#nBits by bitsPerDigit} {
            const last = (rshift + bitsPerDigit) >= nBits;
            // count digits
            coforall loc in Locales with (ref globalCounts) {
                on loc {
                    // allocate counts
                    var tasksBucketCounts: [Tasks] [0..#numBuckets] int;
                    coforall task in Tasks with (ref tasksBucketCounts) {
                        ref taskBucketCounts = tasksBucketCounts[task];
                        // get local domain's indices
                        var lD = temp.localSubdomain();
                        // calc task's indices from local domain's indices
                        var tD = calcBlock(task, lD.low, lD.high);
                        // count digits in this task's part of the array
                        for i in tD {
                            const key = comparator.key(temp.localAccess[i]);
                            var bucket = getDigit(key, rshift, last, negs); // calc bucket from key
                            taskBucketCounts[bucket] += 1;
                        }
                    }//coforall task
                    // write counts in to global counts in transposed order
                    coforall tid in Tasks with (ref tasksBucketCounts, ref globalCounts) {
                        var aggregator = new DstAggregator(int);
                        for task in Tasks {
                            ref taskBucketCounts = tasksBucketCounts[task];
                            for bucket in chunk(0..#numBuckets, numTasks, tid) {
                                aggregator.copy(globalCounts[calcGlobalIndex(bucket, loc.id, task)],
                                                             taskBucketCounts[bucket]);
                            }
                        }
                        aggregator.flush();
                    }//coforall task
                }//on loc
            }//coforall loc
            
            // scan globalCounts to get bucket ends on each locale/task
            var globalStarts = + scan globalCounts;
            globalStarts -= globalCounts;
            
            // calc new positions and permute
            coforall loc in Locales with (ref a) {
                on loc {
                    // allocate counts
                    var tasksBucketPos: [Tasks] [0..#numBuckets] int;
                    // read start pos in to globalStarts back from transposed order
                    coforall tid in Tasks with (ref tasksBucketPos) {
                        var aggregator = new SrcAggregator(int);
                        for task in Tasks {
                            ref taskBucketPos = tasksBucketPos[task];
                            for bucket in chunk(0..#numBuckets, numTasks, tid) {
                              aggregator.copy(taskBucketPos[bucket],
                                         globalStarts[calcGlobalIndex(bucket, loc.id, task)]);
                            }
                        }
                        aggregator.flush();
                    }//coforall task
                    coforall task in Tasks with (ref tasksBucketPos, ref a) {
                        ref taskBucketPos = tasksBucketPos[task];
                        // get local domain's indices
                        var lD = temp.localSubdomain();
                        // calc task's indices from local domain's indices
                        var tD = calcBlock(task, lD.low, lD.high);
                        // calc new position and put data there in temp
                        {
                            var aggregator = new DstAggregator(t);
                            for i in tD {
                                const ref tempi = temp.localAccess[i];
                                const key = comparator.key(tempi);
                                var bucket = getDigit(key, rshift, last, negs); // calc bucket from key
                                var pos = taskBucketPos[bucket];
                                taskBucketPos[bucket] += 1;
                                aggregator.copy(a[pos], tempi);
                            }
                            aggregator.flush();
                        }
                    }//coforall task 
                }//on loc
            }//coforall loc

            // copy back to temp for next iteration
            // Only do this if there are more digits left
            if !last {
              temp <=> a;
            }
        } // for rshift
    }//proc radixSortLSDCore


}
