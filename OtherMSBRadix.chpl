module OtherMSBRadix {
  
  use Time;
  use Reflection;

  /* Sorting criterion has a method:
       (key:uint,done:bool) = criterion.keyPart(record, startPart=1)

       X(key:uint,done:bool) = criterion.keyBits(record, startbit)
       X(cmp=-1,0,1,nsamebits) = criterion.prefixCompare(recordA, recordB)
       cmp=-1,0,1 = criterion.compare(recordA, recordB)

       returns 64-bits of key starting at startbit
         and done is set if startbit is beyond the number
         of bits in this record.
   */

  param RADIX_BITS = 8;
  param DISTRIBUTE_BUFFER = 5;
  config const sortSwitch = 256;
  config const minForTask = 256;
  config param CHECK_SORTS = false;
  config param progress = false;
  config const alwaysSerial = false;
  config const maxTasks = here.numPUs(logical=true);

  // startbit is starting from 0
  // bin 0 is for the end was reached
  // bins 1..256 are for data with next part 0..255.
  inline
  proc binForRecord(a, criterion, startbit:int):int
  {
    if canResolveMethod(criterion, "key", a) {
      // We have keyPart(element):int/uint
      var key = criterion.key(a);
      param bitsPerPart = numBits(key.type);
      param bitsPerPartModRadixBits = bitsPerPart % RADIX_BITS;
      if bitsPerPartModRadixBits != 0 then
        compilerError("key size must be a multiple of radix bits");
        // or else the implementation below would have to handle crossing parts

      // startbit must be a multiple of RADIX_BITS because the radix
      // sort operates RADIX_BITS at a time.

      // startbit might be partway through a part (e.g. 16 bits into a uint(64))
      const whichpart = startbit / bitsPerPart;
      const bitsinpart = startbit % bitsPerPart;

      // If the number is signed, invert the top bit, so that
      // the negative numbers sort below the positive numbers
      var ubits = key:uint(bitsPerPart);
      if isInt(key) {
        const one:ubits.type = 1;
        ubits = ubits ^ (one << (bitsPerPart - 1));
      }
      param mask:uint = (1 << RADIX_BITS) - 1;
      var ubin = (ubits >> (bitsPerPart - bitsinpart - RADIX_BITS)) & mask;
      //writef("startbit=%i bin=%xu key=%xu whichpart=%i bitsinpart=%i, shift=%i\n",
       //      startbit, ubin, ubits, whichpart, bitsinpart, (bitsPerPart - bitsinpart));
      return if whichpart != 0 then 0
             else 1 + ubin:int;
    } else {
      // We have keyPart(element, start):(section:int(8), part:int/uint)
      var testRet: criterion.keyPart(a, 1).type;
      var testPart = testRet(2);
      param bitsPerPart = numBits(testPart.type);
      param bitsPerPartModRadixBits = bitsPerPart % RADIX_BITS;
      if bitsPerPartModRadixBits != 0 then
        compilerError("part size must be a multiple of radix bits");
        // or else the implementation below would have to handle crossing parts

      // startbit must be a multiple of RADIX_BITS because the radix
      // sort operates RADIX_BITS at a time.

      // startbit might be partway through a part (e.g. 16 bits into a uint(64))
      const whichpart = startbit / bitsPerPart;
      const bitsinpart = startbit % bitsPerPart;

      var (section, part) = criterion.keyPart(a, 1+whichpart);
      var ubits = part:uint(bitsPerPart);
      // If the number is signed, invert the top bit, so that
      // the negative numbers sort below the positive numbers
      if isInt(part) {
        const one:ubits.type = 1;
        ubits = ubits ^ (one << (bitsPerPart - 1));
      }
      param mask:uint = (1 << RADIX_BITS) - 1;
      var ubin = (ubits >> (bitsPerPart - bitsinpart - RADIX_BITS)) & mask;

      //writef("startbit=%i bin=%xu key=%xu whichpart=%i bitsinpart=%i, shift=%i section=%t\n",
       //      startbit, ubin, ubits, whichpart, bitsinpart, (bitsPerPart - bitsinpart), section);
      
      if section == 0 then
        return ubin:int + 1;
      else if section < 0 then
        return 0;
      else
        return (1 << RADIX_BITS) + 1;
    }
    //var (bits,done) = criterion.keyBits(a,startbit);
    //var bin = if done then 0 else 1 + (bits >> (64 - radixbits));
    //return bin:int;
  }

  proc msbRadixSort(start_n:int, end_n:int, A:[], criterion, startbit = 0)
  {
    if( end_n - start_n < sortSwitch ) {
      shellSort(A, criterion, start=start_n, end=end_n);
      if CHECK_SORTS then checkSorted(start_n, end_n, A, criterion);
      return;
    }
    
    if progress then writeln("radix sort start=", start_n, " end=", end_n, " startbit=", startbit);

    const radixbits = RADIX_BITS;
    const radix = (1 << radixbits) + 1;

    // 0th bin is for records where we've consumed all the key.
    var offsets:[0..radix] int;
    var end_offsets:[0..radix] int;

    // Step 1: count.
    // Could be parallel... but that's hard to write unless we
    // can do a custom reduction? Or have thread-private variables?
    for i in start_n..end_n {
      var bin = binForRecord(A[i], criterion, startbit);
      offsets[bin] += 1;
    }

    if progress then writeln("accumulate");

    //writeln("COUNTS: ", offsets);
    // Step 2: accumulate
    end_offsets = + scan offsets;
    for (off,end) in zip(offsets,end_offsets) {
      off = start_n + end - off;
      end = start_n + end;
    }
    //writeln("OFFSETS: ", offsets);

    var curbin = 0;

    if progress then writeln("shuffle");

    // Step 3: shuffle
    while true {
      // Find the next bin that isn't totally in place.
      while curbin <= radix && offsets[curbin] == end_offsets[curbin] {
        curbin += 1;
      }
      if curbin > radix {
        break;
      }

      param max_buf = DISTRIBUTE_BUFFER;
      var buf: max_buf*A.eltType;
      var used_buf = 0;
      var end = end_offsets[curbin];
      var endfast = max(offsets[curbin], end_offsets[curbin]-2*max_buf);
      var bufstart = max(offsets[curbin], end_offsets[curbin]-max_buf);
      var i = bufstart;

      // Fill buf with up to max_buf records from the end of this bin.
      while i < end {
        buf[used_buf+1] = A[i];
        used_buf += 1;
        i += 1;
      }

      while offsets[curbin] < endfast {
        // Now go through the records in buf
        // putting them in their right home.
        for param j in 1..max_buf {
          var bin = binForRecord(buf[j], criterion, startbit);
//          prefetch(A[offsets[bin]]); it does not help
          // Swap buf[j] into its appropriate bin.
          // Leave buf[j] with the next unsorted item.
          A[offsets[bin]] <=> buf[j];
          offsets[bin] += 1;
        }
      }
      // Now, handle elements in bufstart...end_offsets[cur_bin]
      while offsets[curbin] < end {
        // Put buf[j] into its right home
        var j = 1;
        while used_buf > 0 && j <= used_buf {
          var bin = binForRecord(buf[j], criterion, startbit);
          // Swap buf[j] into its appropriate bin.
          // Leave buf[j] with the next unsorted item.
          // But offsets[bin] might in the region we already read.
          if bin == curbin && offsets[curbin] >= bufstart {
            A[offsets[bin]] = buf[j];
            buf[j] = buf[used_buf];
            used_buf -= 1;
          } else {
            A[offsets[bin]] <=> buf[j];
          }
          offsets[bin] += 1;
          j += 1;
        }
      }
    }

    if progress then writeln("sort sub-problems");

    // Step 4: sort sub-problems.
    // Note that shuffle changed the offsets to be == end_offset..
    // put offsets back.
    offsets[0] = start_n;
    for i in 1..radix {
      offsets[i] = end_offsets[i-1];
    }

    // This is a parallel version
    if alwaysSerial == false {
      const subbits = startbit + radixbits;
      var nbigsubs = 0;
      var bigsubs:[0..radix] (int,int);
      const runningNow = here.runningTasks();

      // Never recursively sort the first or last bins
      // (these store the end)
      // (TODO: we could if we wanted to break ties in a comparator)

      for bin in 1..radix-1 {
        // Does the bin contain more than one record?
        const bin_start = offsets[bin];
        const bin_end = if bin+1<=radix then offsets[bin+1]-1 else end_n;
        const num = 1 + bin_end - bin_start;
        if num <= 1 {
          // do nothing 
        } else if num < minForTask || runningNow >= maxTasks {
          // sort it in this thread
          msbRadixSort(bin_start, bin_end, A, criterion, subbits);
        } else {
          // Add it to the list of things to do in parallel
          bigsubs[nbigsubs] = (bin_start, bin_end);
          nbigsubs += 1;
        }
      }

      forall (bin,(bin_start,bin_end)) in zip(0..#nbigsubs,bigsubs) {
        msbRadixSort(bin_start, bin_end, A, criterion, subbits);
      }
    } else {
      // The serial version
      for bin in 1..radix-1 {
        // Does the bin contain more than one record?
        const bin_start = offsets[bin];
        const bin_end = if bin+1<=radix then offsets[bin+1]-1 else end_n;
        const num = 1 + bin_end - bin_start;
        if num > 1 {
          // sort it in this thread
          msbRadixSort(bin_start, bin_end, A, criterion, startbit + radixbits);
        }
      }
    }
   
    if CHECK_SORTS then checkSorted(start_n, end_n, A, criterion);
  }

  proc checkSorted(start_n:int, end_n:int, A:[], criterion, startbit = 0)
  {
    for i in start_n+1..end_n {
      var cmp = chpl_compare(A[i-1], A[i], criterion);
      if cmp > 0 {
        writeln("Error: not sorted properly at i=", i, " A[i-1]=", A[i-1], " A[i]=", A[i], " in start=", start_n, " end=", end_n);
        writeln(A);
        assert(false);
      }
    }
  }
}
