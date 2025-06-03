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



// the algorithm here is also called "American Flag Sort"
// it is not stable and not fully parallel
@chpldoc.nodoc
module MSBRadixSort {
  import Sort.{defaultComparator, ShellSort};
  private use super.RadixSortHelp;
  private use OS.POSIX;

  // This structure tracks configuration for the radix sorter.
  record MSBRadixSortSettings {
    param DISTRIBUTE_BUFFER = 5; // Number of temps during shuffle step
    const sortSwitch = 256; // when sorting <= this # elements, use other sort
    const minForTask = 256; // when sorting >= this many elements, go parallel
    param CHECK_SORTS = false; // do costly extra checks that data is sorted
    param progress = false; // print progress
    const alwaysSerial = false; // never create tasks
    const maxTasks = here.maxTaskPar;//;here.numPUs(logical=true); // maximum number of tasks to make
  }

  proc msbRadixSort(ref Data:[], comparator:?rec = new defaultComparator(),
                    region:range(?)=Data.domain.dim(0)) {

    var endbit:int;
    endbit = msbRadixSortParamLastStartBit(Data, comparator);
    if endbit < 0 then
      endbit = max(int);

    msbRadixSort(Data, start_n=region.low, end_n=region.high,
                 comparator,
                 startbit=0, endbit=endbit,
                 settings=new MSBRadixSortSettings());
  }

  // forall with intents used in tuple expansion causes compilation errors,
  // for now, explicitly thwart kernel generation here, as detecting intents is
  // not easy that late in compilation
  pragma "no gpu codegen"
  // startbit counts from 0 and is a multiple of RADIX_BITS
  proc msbRadixSort(ref A:[], start_n:A.idxType, end_n:A.idxType, criterion,
                    startbit:int, endbit:int,
                    settings /* MSBRadixSortSettings */)
  {
    type idxType = A.idxType;
    if startbit > endbit then
      return;

    if( end_n - start_n < settings.sortSwitch ) {
      // Shell sort here works reasonably well for
      // ordered and random input sequences.
      // Insertion sort would improve performance for random sequences
      // but causes performance problems with ordered sequences.
      // Using quicksort here has a similar, but less extreme impact.
      ShellSort.shellSortMoveElts(A, criterion,
                                  start=start_n, end=end_n);
      if settings.CHECK_SORTS then checkSorted(start_n, end_n, A, criterion);
      return;
    }

    if settings.progress then writeln("radix sort start=", start_n, " end=", end_n, " startbit=", startbit, " endbit=", endbit);

    const radixbits = RADIX_BITS;
    const radix = (1 << radixbits) + 1;

    // 0th bin is for records where we've consumed all the key.
    var offsets:[0..radix] idxType;
    var end_offsets:[0..radix] idxType;
    type ubitsType = binForRecord(A[start_n], criterion, startbit)(1).type;
    var min_ubits: ubitsType = max(ubitsType);
    var max_ubits: ubitsType = 0;
    var min_bin = radix+1;
    var max_bin = 0;
    var any_ending = false;

    // Step 1: count.
    if settings.alwaysSerial == false {
      forall i in start_n..end_n
        with (+ reduce offsets,
              min reduce min_ubits,
              max reduce max_ubits,
              || reduce any_ending) {
        const (bin, ubits) = binForRecord(A[i], criterion, startbit);
        if ubits < min_ubits then
          min_ubits = ubits;
        if ubits > max_ubits then
          max_ubits = ubits;
        if bin == 0 || bin == radix then
          any_ending = true;
        offsets[bin] += 1;
      }
    } else {
      // The serial version
      for i in start_n..end_n {
        const (bin, ubits) = binForRecord(A[i], criterion, startbit);
        if ubits < min_ubits then
          min_ubits = ubits;
        if ubits > max_ubits then
          max_ubits = ubits;
        if bin == 0 || bin == radix then
          any_ending = true;
        offsets[bin] += 1;
      }
    }

    // TODO: any_ending should be replaced by checking
    // offsets[0] and offsets[radix]

    // If the data parts we gathered all have the same leading bits,
    // we might be able to skip ahead immediately to the next count step.
    if any_ending == false {
      var dataStartBit = findDataStartBit(startbit, min_ubits, max_ubits);
      if dataStartBit > startbit {
        // Re-start count again immediately at the new start position.
        msbRadixSort(A, start_n, end_n, criterion,
                     dataStartBit, endbit, settings);
        return;
      }
    }

    if settings.progress then writeln("accumulate");

    // Step 2: accumulate
    var sum = 0:idxType;
    for (off,end) in zip(offsets,end_offsets) {
      var binstart = sum;
      sum += off;
      var binend = sum;
      off = start_n + binstart;
      end = start_n + binend;
    }

    var curbin = 0;

    if settings.progress then writeln("shuffle");

    // Step 3: shuffle
    while true {
      // Find the next bin that isn't totally in place.
      while curbin <= radix && offsets[curbin] == end_offsets[curbin] {
        curbin += 1;
      }
      if curbin > radix {
        break;
      }

      // TODO: I think it might be possible to make this sort stable
      // by populating buf from the start of the data instead of from the end.
      // buf would need to be populated with the first M elements that aren't
      // already in the correct bin.

      // TODO: I think it's possible to make this shuffle parallel
      // by imagining each task has a max_buf and have them update
      // atomic offsets.
      param max_buf = settings.DISTRIBUTE_BUFFER;
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
        for param j in 0..max_buf-1 {
          const (bin, _) = binForRecord(buf[j], criterion, startbit);
          // prefetch(A[offsets[bin]]) could be here but doesn't help

          // Swap buf[j] into its appropriate bin.
          // Leave buf[j] with the next unsorted item.
          A[offsets[bin]] <=> buf[j];
          offsets[bin] += 1;
        }
      }
      // Now, handle elements in bufstart...end_offsets[cur_bin]
      while offsets[curbin] < end {
        // Put buf[j] into its right home
        var j = 0;
        while used_buf >= 0 && j < used_buf {
          const (bin, _) = binForRecord(buf[j], criterion, startbit);
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

    if settings.progress then writeln("sort sub-problems");

    // Step 4: sort sub-problems.
    // Note that shuffle changed the offsets to be == end_offset..
    // put offsets back.
    offsets[0] = start_n;
    for i in 1..radix {
      offsets[i] = end_offsets[i-1];
    }

    // This is a parallel version
    if settings.alwaysSerial == false {
      const subbits = startbit + radixbits;
      var nbigsubs = 0;
      var bigsubs:[0..radix] (idxType,idxType);
      const runningNow = here.runningTasks();

      // Never recursively sort the first or last bins
      // (these store the end)

      for bin in 1..radix-1 {
        // Does the bin contain more than one record?
        const bin_start = offsets[bin];
        const bin_end = if bin+1<=radix then offsets[bin+1]-1 else end_n;
        const num = 1 + bin_end - bin_start;
        if num <= 1 || startbit >= endbit {
          // do nothing
        } else if num < settings.minForTask || runningNow >= settings.maxTasks {
          // sort it in this thread
          msbRadixSort(A, bin_start, bin_end, criterion,
                       subbits, endbit, settings);
        } else {
          // Add it to the list of things to do in parallel
          bigsubs[nbigsubs] = (bin_start, bin_end);
          nbigsubs += 1;
        }
      }

      forall (_,(bin_start,bin_end)) in zip(0..#nbigsubs,bigsubs) with (ref A) {
        msbRadixSort(A, bin_start, bin_end, criterion, subbits, endbit, settings);
      }
    } else {
      // The serial version
      for bin in 1..radix-1 {
        // Does the bin contain more than one record?
        const bin_start = offsets[bin];
        const bin_end = if bin+1<=radix then offsets[bin+1]-1 else end_n;
        const num = 1 + bin_end - bin_start;
        if num <= 1 || startbit >= endbit {
          // do nothing
        } else {
          // sort it in this thread
          msbRadixSort(A, bin_start, bin_end, criterion,
                       startbit + radixbits, endbit, settings);
        }
      }
    }

    if settings.CHECK_SORTS then checkSorted(start_n, end_n, A, criterion);
  }
}
