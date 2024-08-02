module SequentialInPlacePartitioning {
  private use Math;
  private param DISTRIBUTE_BUFFER = 5; // Number of temps during shuffle step

  // Returns the count for each bucket
  // Stores the data in dst in buckets according to the bucketizer.
  // (e.g. sorted by the next digit in radix sort)
  proc bucketize(start_n: int, end_n: int, ref A:[],
                 bucketizer,
                 criterion, startbit:int) {

    var nBuckets = bucketizer.getNumBuckets();
    const nTasks = if dataParTasksPerLocale > 0
               then dataParTasksPerLocale
               else here.maxTaskPar;

    // Divide the input into nTasks chunks.
    const countsSize = nTasks * nBuckets;
    const n = end_n - start_n + 1;
    const blockSize = divCeil(n, nTasks);
    const nBlocks = divCeil(n, blockSize);

    var counts: [0..#nBuckets] int;

    // Step 1: count
    coforall tid in 0..#nTasks with (+ reduce counts) {
      var start = start_n + tid * blockSize;
      var end = start + blockSize - 1;
      if end > end_n {
        end = end_n;
      }

      for (_,bin) in bucketizer.classify(A, start, end, criterion, startbit) {
        counts[bin] += 1;
      }
    }

    // Step 2: scan
    var end_offsets = (+ scan counts) + start_n;
    var offsets = end_offsets - counts;

    // Step 3: distribute/shuffle
    var curbin = 0;
    while true {
      // Find the next bin that isn't totally in place.
      while curbin < nBuckets && offsets[curbin] == end_offsets[curbin] {
        curbin += 1;
      }
      if curbin >= nBuckets {
        break;
      }

      // TODO: I think it might be possible to make this sort stable
      // by populating buf from the start of the data instead of from the end.
      // buf would need to be populated with the first M elements that aren't
      // already in the correct bin.

      // TODO: I think it's possible to make this shuffle parallel
      // by imagining each task has a max_buf and have them update
      // atomic offsets.
      param max_buf = DISTRIBUTE_BUFFER;
      var buf: max_buf*A.eltType;
      var used_buf = 0;
      var end = end_offsets[curbin];
      var endfast = max(offsets[curbin], end_offsets[curbin]-2*max_buf);
      var bufstart = max(offsets[curbin], end_offsets[curbin]-max_buf);
      var i = bufstart;

      // Fill buf with up to max_buf records from the end of this bin.
      while i < end {
        buf[used_buf] <=> A[i];
        used_buf += 1;
        i += 1;
      }

      while offsets[curbin] < endfast {
        // Now go through the records in buf
        // putting them in their right home.
        for (idx, bin) in bucketizer.classify(buf, 0, max_buf-1,
                                              criterion, startbit) {
          // Swap buf[j] into its appropriate bin.
          // Leave buf[j] with the next unsorted item.
          A[offsets[bin]] <=> buf[idx];
          offsets[bin] += 1;
        }
      }
      // Now, handle elements in bufstart...end_offsets[cur_bin]
      while offsets[curbin] < end {
        // Put buf[j] into its right home
        var j = 0;
        while used_buf >= 0 && j < used_buf {
          const bin = bucketizer.bucketForRecord(buf[j], criterion, startbit);
          // Swap buf[j] into its appropriate bin.
          var offset = offsets[bin];
          A[offset] <=> buf[j];
          offsets[bin] += 1;
          // Leave buf[j] with the next unsorted item.
          // But offsets[bin] might be in the region we already read.
          if bin == curbin && offset >= bufstart {
            used_buf -= 1;
            buf[j] <=> buf[used_buf];
          }
          j += 1;
        }
      }
    }

    return counts;
  }
}
