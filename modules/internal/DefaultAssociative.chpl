/*
 * Copyright 2004-2016 Cray Inc.
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

// DefaultAssociative.chpl
//
module DefaultAssociative {

  use Operations;

  use DSIUtil;
  config param debugDefaultAssoc = false;
  config param debugAssocDataPar = false;
  config param defaultAssocAggregation = CHPL_CACHE_REMOTE;
  
  // TODO: make the domain parameterized by this?
  type chpl_table_index_type = int;


  /* These declarations could/should both be nested within
     DefaultAssociativeDom? */
  enum chpl__hash_status { empty, full, deleted };

  record chpl_TableEntry {
    type idxType;
    var status: chpl__hash_status = chpl__hash_status.empty;
    var idx: idxType;
  }

  proc chpl__primes return
  (23, 53, 89, 191, 383, 761, 1531, 3067, 6143, 12281, 24571, 49139, 98299,
   196597, 393209, 786431, 1572853, 3145721, 6291449, 12582893, 25165813,
   50331599, 100663291, 201326557, 402653171, 805306357, 1610612711, 3221225461,
   6442450939, 12884901877, 25769803751, 51539607551, 103079215087,
   206158430183, 412316860387, 824633720831, 1649267441651, 3298534883309,
   6597069766631, 13194139533299, 26388279066623, 52776558133177,
   105553116266489, 211106232532969, 422212465065953, 844424930131963,
   1688849860263901, 3377699720527861, 6755399441055731, 13510798882111483,
   27021597764222939, 54043195528445869, 108086391056891903, 216172782113783773,
   432345564227567561, 864691128455135207);

  class DefaultAssociativeDomOperationsHandler : OperationsHandler {
    type idxType;
    param parSafe: bool;
    var dom: DefaultAssociativeDom(idxType, parSafe);
    proc processOperations( size:int, buf:c_void_ptr ) {
      extern proc sizeof(type t):size_t;

      //extern proc printf(fmt:c_string, sz:c_int);

      //extern proc printf(fmt:c_string, id:c_int, sz:c_int);
      //printf("%i IN  PROCESS size=%i\n", here.id:c_int, size:c_int);

// Why do I seem to need to use this primitive?
      var ptr:c_ptr(idxType) = __primitive("cast", c_ptr(idxType), buf);
      const eltSize = sizeof(idxType):int;
      var i = 0;
      var offset = 0;

      var newkeys = size/eltSize;
      // resize if needed
      // - flush rdata cache if we will resize
      // - take lock either way

      // This helps performance for cache+bulk add but I can't understand why
      var localLock = size < 880*2;
      //var localLock = true;
      if localLock {
        dom._resizeIfNeeded(newkeys, takeLock=parSafe);
      } else {
        // This doesn't help
        //chpl_rmem_consist_release();
        //dom._resizeIfNeeded(newkeys, takeLock=parSafe);

        // this does
        dom.lockTable();
        dom._resizeIfNeeded(newkeys, takeLock=false);
      }

 
// Why can't I use request capacity?
// It seems to change the result? Losing a small number of elements?
//      dom.dsiRequestCapacity(dom.dsiNumIndices + size/eltSize, haveLock=true);
      while offset < size {
        //writeln("Adding ", ptr[i], " size is ", dom.dsiNumIndices);
//        dom.dsiAddInternal(ptr[i], haveLock=true);
        dom._doadd(ptr[i]);
        //printf("%i\n", ptr[i]:c_int);
        offset += eltSize;
        i += 1;
      }
      if parSafe {
        // See above.
        // dom.unlockTable();
        dom.unlockTable(onlyLocalFence=true);
      }

      //printf("%i END PROCESS size=%i\n", here.id:c_int, size:c_int);
    }
    proc enqueueAdd(idx: idxType) {

      //extern proc printf(fmt:c_string, sz:c_int);
      //printf("IN ENQUEUE v=%i\n", idx:c_int);


      extern proc sizeof(type t):size_t;
      var idx_copy = idx;
      var idx_copy_ptr = c_ptrTo(idx_copy);
      enqueue_operation(this:OperationsHandler, 0, 0,
                        sizeof(idxType):int, idx_copy_ptr:c_void_ptr);
    }
    proc enqueueAdds(arr: [?Dom] idxType, subrange) where Dom.rank == 1 {

      //extern proc printf(fmt:c_string, sz:c_int);
      //printf("IN ENQUEUE v=%i\n", idx:c_int);

      // Does this help performance? No.
      //chpl_rmem_consist_fence(order=memory_order_seq_cst);


      extern proc sizeof(type t):size_t;
      var idx_copy_ptr = c_ptrTo(arr[subrange.low]);
      var n = subrange.size;
      enqueue_operation(this:OperationsHandler, 0, 0,
                        n*sizeof(idxType):int, idx_copy_ptr:c_void_ptr);
    }

 
    // This flush call isn't actually necessary as
    // long as we lock the table
    proc flush() {
      //extern proc printf(fmt:c_string, sz:c_int);
      //printf("IN FLUSH %i\n", 0:c_int);


      flush_operations(this:OperationsHandler, 0, 0);
    }
  }

  class DefaultAssociativeDom: BaseAssociativeDom {
    type idxType;
    param parSafe: bool;

    var dist: DefaultDist;
  
    // The guts of the associative domain
  
    // We explicitly use processor atomics here since this is not
    // by design a distributed data structure
    var numEntries: atomic_int64;
    var tableLock: atomicbool; // do not access directly, use function below
    var tableSizeNum = 1;
    var tableSize = chpl__primes(tableSizeNum);
    var tableDom = {0..tableSize-1};
    var table: [tableDom] chpl_TableEntry(idxType);
  
    var opHandler:DefaultAssociativeDomOperationsHandler(idxType, parSafe);

    inline proc lockTable(param onlyLocalFence=false) {
      extern proc chpl_cache_startall();

      if onlyLocalFence {
        // TODO: This is a hack to turn off cache flush
        // Right solution is to make tableLock only a local
        // lock (not flush the cache) and add such scope arguments
        // to atomic operations (See LLVM atomic for inspiration).
        // Don't know if table lock can always be this way or
        // it should just do it in this routine.

        // This one shouldn't flush the remote data cache
        while tableLock.testAndSet(order=memory_order_relaxed) do
          chpl_task_yield();

        // The below has intermediate performance:

        /*
        // If there is contention, start operations
        // because remote nodes might make progress while
        // we are contended.
        if !tableLock.testAndSet(order=memory_order_relaxed) then
          return;

        // Otherwise, try starting remote operations 
        while tableLock.testAndSet(order=memory_order_relaxed) {
          chpl_task_yield();
          //chpl_cache_startall(); doesn't help
          chpl_rmem_consist_release();
        }
        */

      } else {
        // This one needs to flush the remote data cache
        while tableLock.testAndSet() do
          chpl_task_yield();
      }
    }

    inline proc unlockTable(param onlyLocalFence=false) {
      if onlyLocalFence {
        // Hack - see lockTable
        tableLock.clear(order=memory_order_relaxed);
      } else {
        tableLock.clear();
      }
    }
  
    // TODO: An ugly [0..-1] domain appears several times in the code --
    //       replace with a named constant/param?
    var postponeResize = false;
  
    proc linksDistribution() param return false;
    proc dsiLinksDistribution()     return false;
  
    proc DefaultAssociativeDom(type idxType,
                               param parSafe: bool,
                               dist: DefaultDist) {
      if !chpl__validDefaultAssocDomIdxType(idxType) then
        compilerError("Default Associative domains with idxType=",
                      idxType:string, " are not allowed", 2);
      this.dist = dist;
      if defaultAssocAggregation then
        this.opHandler = new DefaultAssociativeDomOperationsHandler(idxType, parSafe, this);
    }
  
    proc _flushAggregatedOps() {
      if defaultAssocAggregation then
        this.opHandler.flush();
    }

    //
    // Standard Internal Domain Interface
    //
    proc dsiBuildArray(type eltType) {
      _flushAggregatedOps();
      return new DefaultAssociativeArr(eltType=eltType, idxType=idxType,
                                       parSafeDom=parSafe, dom=this);
    }
  
    proc dsiSerialReadWrite(f /*: Reader or Writer*/) {
      _flushAggregatedOps();
      var first = true;
      f <~> new ioLiteral("{");
      for idx in this {
        if first then 
          first = false; 
        else 
          f <~> new ioLiteral(", ");
        f <~> idx;
      }
      f <~> new ioLiteral("}");
    }
    proc dsiSerialWrite(f) { this.dsiSerialReadWrite(f); }
    proc dsiSerialRead(f) { this.dsiSerialReadWrite(f); }
  
    //
    // Standard user domain interface
    //
  
    inline proc dsiNumIndices {
      _flushAggregatedOps();
      return numEntries.read();
    }
  
    iter dsiIndsIterSafeForRemoving() {
      _flushAggregatedOps();
      postponeResize = true;
      for i in this.these() do
        yield i;
      on this {
        postponeResize = false;
        if (numEntries.read()*8 < tableSize && tableSizeNum > 1) {
          if parSafe then lockTable();
          if (numEntries.read()*8 < tableSize && tableSizeNum > 1) {
            _resize(grow=false);
          }
          if parSafe then unlockTable();
        }
      }
    }
  
    iter these() {
      _flushAggregatedOps();
      if !isEnumType(idxType) {
        for slot in _fullSlots() {
          yield table[slot].idx;
        }
      } else {
        for val in chpl_enumerate(idxType) {
          var (match, slot) = _findFilledSlot(val);
          if match then
            yield table[slot].idx;
        }
      }
    }
 
    iter these(param tag: iterKind) where tag == iterKind.standalone {
      _flushAggregatedOps();
      if debugDefaultAssoc {
        writeln("*** In associative domain standalone iterator");
      }
      // We are simply slicing up the table here.  Trying to do something
      //  more intelligent (like evenly dividing up the full slots, led
      //  to poor speed ups.
      const numIndices = tableSize;
      const numChunks = _computeNumChunks(numIndices);

      if debugAssocDataPar {
        writeln("### numChunks=", numChunks, ", numIndices=", numIndices);
      }

      if numChunks == 1 {
        for slot in 0..numIndices-1 {
          if table[slot].status == chpl__hash_status.full {
            yield table[slot].idx;
          }
        }
      } else {
        coforall chunk in 0..#numChunks {
          const (lo, hi) = _computeBlock(numIndices, numChunks,
                                         chunk, numIndices-1);
          if debugAssocDataPar then
            writeln("*** chunk: ", chunk, " owns ", lo..hi);
          for slot in lo..hi {
            if table[slot].status == chpl__hash_status.full {
              yield table[slot].idx;
            }
          }
        }
      }
    }
 
    iter these(param tag: iterKind) where tag == iterKind.leader {
      _flushAggregatedOps();
      if debugDefaultAssoc then
        writeln("*** In domain leader code:");
      const numTasks = if dataParTasksPerLocale==0 then here.maxTaskPar
                       else dataParTasksPerLocale;
      const ignoreRunning = dataParIgnoreRunningTasks;
      const minIndicesPerTask = dataParMinGranularity;
      // We are simply slicing up the table here.  Trying to do something
      //  more intelligent (like evenly dividing up the full slots, led
      //  to poor speed ups.
      // This requires that the zipppered domains match.
      const numIndices = tableSize;
  
      if debugAssocDataPar {
        writeln("### numTasks = ", numTasks);
        writeln("### ignoreRunning = ", ignoreRunning);
        writeln("### minIndicesPerTask = ", minIndicesPerTask);
      }
  
      if debugDefaultAssoc then
        writeln("    numTasks=", numTasks, " (", ignoreRunning,
                "), minIndicesPerTask=", minIndicesPerTask);
  
      var numChunks = _computeNumChunks(numTasks, ignoreRunning,
                                        minIndicesPerTask,
                                        numIndices);
      if debugDefaultAssoc then
        writeln("    numChunks=", numChunks, "length=", numIndices);
  
      if debugAssocDataPar then writeln("### numChunks=", numChunks);
  
      if numChunks == 1 {
        yield (0..numIndices-1, this);
      } else {
        coforall chunk in 0..#numChunks {
          const (lo, hi) = _computeBlock(numIndices, numChunks,
                                         chunk, numIndices-1);
          if debugDefaultAssoc then
            writeln("*** DI[", chunk, "]: tuple = ", (lo..hi,));
          yield (lo..hi, this);
        }
      }
    }
  
    iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
      _flushAggregatedOps();
      var (chunk, followThisDom) = followThis;

      if debugDefaultAssoc then
        writeln("In domain follower code: Following ", chunk);

      const sameDom = followThisDom == this;
      
      if !sameDom then
        if followThisDom.dsiNumIndices != this.dsiNumIndices then
          halt("zippered associative domains do not match");

      var otherTable = followThisDom.table;
      for slot in chunk.low..chunk.high {
        var entry = otherTable[slot];
        if entry.status == chpl__hash_status.full {
          var idx = slot;
          if !sameDom {
            const (match, loc) = _findFilledSlot(entry.idx, haveLock=true);
            if !match then halt("zippered associative domains do not match");
            idx = loc;
          }
          yield table[idx].idx;
        }
      }
    }
  
    //
    // Associative Domain Interface
    //
    proc dsiMyDist() : BaseDist {
      return dist;
    }

    proc dsiClear() {
      _flushAggregatedOps();
      on this {
        if parSafe then lockTable();
        for slot in tableDom {
          table[slot].status = chpl__hash_status.empty;
        }
        numEntries.write(0);
        if parSafe then unlockTable();
      }
    }
  
    proc dsiMember(idx: idxType): bool {
      _flushAggregatedOps();
      return _findFilledSlot(idx)(1);
    }
  
    proc _addWrapper(idx: idxType, in slotNum : index(tableDom) = -1, 
        haveLock = !parSafe) {

      const inSlot = slotNum;
      var retVal = 0;
      on this {
        const shouldLock = !haveLock && parSafe;
        if shouldLock then lockTable();
        var findAgain = shouldLock;
        if ((numEntries.read()+1)*2 > tableSize) {
          _resize(grow=true);
          findAgain = true;
        }
        if findAgain then
          (slotNum, retVal) = _add(idx, -1);
        else
          (_, retVal) = _add(idx, inSlot);
        if shouldLock then unlockTable();
      }
      return (slotNum, retVal);
    }

   proc dsiAdd(idx: idxType) {
     if defaultAssocAggregation then
       this.opHandler.enqueueAdd(idx);
     else {
       // add helpers will return a tuple like (slotNum, numIndicesAdded);

       // these two seemingly redundant lines were necessary to work around a
       // compiler bug. I was unable to create a smaller case that has the same
       // issue.
       // More: `return _addWrapper(idx)[2]` Call to _addWrapper seems to
       // have no effect when `idx` is a range and the line is promoted. My
       // understanding of promotion makes me believe that things might go haywire
       // since return type of the method becomes an array(?). However, it seemed
       // that _addWrapper is never called when the return statement is promoted.
       // I checked the C code and couldn't see any call to _addWrapper.
       // I tried to replicate the issue with generic classes but it always
       // worked smoothly.
       const numInds = _addWrapper(idx)[2];
       return numInds;
     }
   }
   proc dsiAdd(arr: [?Dom] idxType, subrange) where Dom.rank == 1 {
     if defaultAssocAggregation then
       this.opHandler.enqueueAdds(arr, subrange);
     else {

      on this {
       // TODO -- try other versions, like relying
       // on cache to get locArr data.
       var locArr = arr[subrange];
       //writeln("On ", here.id, " adding ", locArr.size);
       const shouldLock = parSafe;
       _resizeIfNeeded(subrange.size, takeLock=shouldLock);
       //if shouldLock then lockTable();
       //_resizeIfNeeded(subrange.size, takeLock=false);

        //writeln("before resize ", numEntries.read(), " / ", tableSize, " taken");
        //writeln("after resize ", numEntries.read(), " / ", tableSize, " taken");
       for idx in subrange {
         _add(locArr[idx], -1);
//         _add(arr[idx], -1);
       }
       if shouldLock then unlockTable();
      }
     }
   }

    proc _doadd(idx: idxType) {
      _add(idx, -1);
    }
    // This routine adds new indices without checking the table size and
    //  is thus appropriate for use by routines like _resize().
    //
    // NOTE: Calls to this routine assume that the tableLock has been acquired.
    //
    proc _add(idx: idxType, in slotNum : index(tableDom) = -1) {
      var foundSlot : bool = (slotNum != -1);
      if !foundSlot then
        (foundSlot, slotNum) = _findEmptySlot(idx);
      if foundSlot {
        table[slotNum].status = chpl__hash_status.full;
        table[slotNum].idx = idx;
        numEntries.add(1, order=memory_order_relaxed);
        //writeln("Adding 1 to num entries");
      } else {
        if (slotNum < 0) {
          halt("couldn't add ", idx, " -- ",
              numEntries.read(order=memory_order_relaxed),
              " / ", tableSize, " taken");
          return (-1, 0);
        }
        // otherwise, re-adding an index that's already in there
        return (slotNum, 0);
      }
      return (slotNum, 1);
    }
  
    proc dsiRemove(idx: idxType) {
      _flushAggregatedOps();
      var retval = 1;
      on this {
        if parSafe then lockTable();
        const (foundSlot, slotNum) = _findFilledSlot(idx, haveLock=parSafe);
        if (foundSlot) {
          for a in _arrs do
            a.clearEntry(idx, true);
          table[slotNum].status = chpl__hash_status.deleted;
          numEntries.sub(1);
        } else {
          retval = 0;
        }
        if (numEntries.read()*8 < tableSize && tableSizeNum > 1) {
          _resize(grow=false);
        }
        if parSafe then unlockTable();
      }
      return retval;
    }
  
    proc dsiRequestCapacity(numKeys:int, haveLock = !parSafe) {
      _flushAggregatedOps();
      const shouldLock = !haveLock && parSafe;
      var entries = numEntries.read();

      // Shouldn't this be in an 'on' statement?

      if entries < numKeys {

        //Find the first suitable prime
        var threshold = (numKeys + 1) * 2;
        var prime = 0;
        var primeLoc = 0;
        for i in 1..chpl__primes.size {
            if chpl__primes(i) > threshold {
              prime = chpl__primes(i);
              primeLoc = i;
              break;
            }
        }

        //No suitable prime found
        if prime == 0 {
          halt("Requested capacity (", numKeys, ") exceeds maximum size");
        }

        //Changing underlying structure, time for locking
        if shouldLock then lockTable();
        if entries > 0 {
          if tableSize==prime {
            if shouldLock then lockTable();

            return;
          }

          // Slow path: back up required
          _backupArrays();

          // copy the table (TODO: could use swap between two versions)
          var copyDom = tableDom;
          var copyTable: [copyDom] chpl_TableEntry(idxType) = table;

          // Do not preserve entries
          tableDom = {0..-1};

          tableSizeNum = primeLoc;
          tableSize = prime;
          tableDom = {0..tableSize-1};

          //numEntries will be reconstructed as keys are readded
          numEntries.write(0);

          // insert old data into newly resized table
          for slot in _fullSlots(copyTable) {
            const (newslot, _) = _add(copyTable[slot].idx);
            _preserveArrayElements(oldslot=slot, newslot=newslot);
          }
            
          _removeArrayBackups();
        } else {
          //Fast path, nothing to backup
          tableSizeNum=primeLoc;
          tableSize=prime;
          tableDom = {0..tableSize-1};
        }

        //Unlock the table
        if shouldLock then unlockTable();
      } else if entries > numKeys {
        warning("Requested capacity (" + numKeys + ") " +
                "is less than current size (" + entries + ")");
      }
    }
  
    iter dsiSorted(comparator:?t) {
      use Sort;
      _flushAggregatedOps();
      var tableCopy: [0..#numEntries.read()] idxType;
  
      for (tmp, slot) in zip(tableCopy.domain, _fullSlots()) do
        tableCopy(tmp) = table[slot].idx;
  
      sort(tableCopy, comparator=comparator);
  
      for ind in tableCopy do
        yield ind;
    }
  
    //
    // Internal interface (private)
    //
    // NOTE: Calls to these _resize routines assume that the tableLock
    // has been acquired.
    //
    proc _resizeIfNeeded(newKeys:int, param takeLock=false) {
      if takeLock then
        lockTable(onlyLocalFence=true);

      var numKeys = numEntries.read(order=memory_order_relaxed) + newKeys;
      var threshold = (numKeys + 1) * 2;
      if threshold < tableSize {
        return;
      }

      if takeLock {
        unlockTable(onlyLocalFence=true);
        lockTable(onlyLocalFence=false);
        numKeys = numEntries.read(order=memory_order_relaxed) + newKeys;
        threshold = (numKeys + 1) * 2;
      }

      // Find the new prime to use.
      var prime = 0;
      var primeLoc = 0;
      for i in 1..chpl__primes.size {
        if chpl__primes(i) > threshold {
          prime = chpl__primes(i);
          primeLoc = i;
          break;
        }
      }
      _resize(primeIndex=primeLoc);
    }

    proc _resize(grow:bool) {
      //writeln("in resize grow=", grow, " sz=", tableSizeNum);
      if postponeResize then return;
      var primeIndex = tableSizeNum;
      primeIndex += if grow then 1 else -1;
      _resize(primeIndex=primeIndex);
    }

    proc _resize(primeIndex:int) {
      //writeln("in resize index=", primeIndex, " v=", chpl__primes(primeIndex));
      // back up the arrays
      _backupArrays();
  
      // copy the table (TODO: could use swap between two versions)
      var copyDom = tableDom;
      var copyTable: [copyDom] chpl_TableEntry(idxType) = table;
 
      // grow original table
      tableDom = {0..(-1:chpl_table_index_type)}; // non-preserving resize
      numEntries.write(0); // reset, because the adds below will re-set this
      tableSizeNum = primeIndex;
      if tableSizeNum > chpl__primes.size then halt("associative array exceeds maximum size");
      if tableSizeNum <= 0 then halt("invalid prime index");
      tableSize = chpl__primes(tableSizeNum);
      tableDom = {0..tableSize-1};
  
      // insert old data into newly resized table
      for slot in _fullSlots(copyTable) {
        const (newslot, _) = _add(copyTable[slot].idx);
        _preserveArrayElements(oldslot=slot, newslot=newslot);
      }
      
      _removeArrayBackups();
    }

    // Searches for 'idx' in a filled slot.
    //
    // Returns true if found, along with the first open slot that may be
    // re-used for faster addition to the domain
    proc _findFilledSlot(idx: idxType, param haveLock = false) : (bool, index(tableDom)) {
      if parSafe && !haveLock then lockTable();
      var firstOpen = -1;
      for slotNum in _lookForSlots(idx, table.domain.high+1) {
        const slotStatus = table[slotNum].status;
        // if we encounter a slot that's empty, our element could not
        // be found past this point.
        if (slotStatus == chpl__hash_status.empty) {
          if firstOpen == -1 then firstOpen = slotNum;
          if parSafe && !haveLock then unlockTable();
          return (false, firstOpen);
        } else if (slotStatus == chpl__hash_status.full) {
          if (table[slotNum].idx == idx) {
            if parSafe && !haveLock then unlockTable();
            return (true, slotNum);
          }
        } else { // this entry was removed, but is the first slot we could use
          if firstOpen == -1 then firstOpen = slotNum;
        }
      }
      if parSafe && !haveLock then unlockTable();
      return (false, -1);
    }

    //
    // NOTE: Calls to this routine assume that the tableLock has been acquired.
    //
    proc _findEmptySlot(idx: idxType, haveLock = false): (bool, index(tableDom)) {
      for slotNum in _lookForSlots(idx) {
        const slotStatus = table[slotNum].status;
        if (slotStatus == chpl__hash_status.empty ||
            slotStatus == chpl__hash_status.deleted) {
          return (true, slotNum);
        } else if (table[slotNum].idx == idx) {
          return (false, slotNum);
        }
      }
      return (false, -1);
    }
      
    //
    // NOTE: Calls to this routine assume that the tableLock has been acquired.
    //
    // NOTE: A copy of this routine is tested in
    //    test/associative/ferguson/check-look-for-slots.chpl
    // So, when updating this routine, either refactor so the test
    // can use the below code - or update the test in a corresponding manner.
    iter _lookForSlots(idx: idxType, numSlots = tableSize) {
      const baseSlot = chpl__defaultHashWrapper(idx):uint;
      for probe in 0..numSlots/2 {
        var uprobe = probe:uint;
        var n = numSlots:uint;
        yield ((baseSlot + uprobe**2)%n):int;
      }
    }
  
    iter _fullSlots(tab = table) {
      for slot in tab.domain {
        if tab[slot].status == chpl__hash_status.full then
          yield slot;
      }
    }
  }
  
  class DefaultAssociativeArr: BaseArr {
    type eltType;
    type idxType;
    param parSafeDom: bool;
    var dom : DefaultAssociativeDom(idxType, parSafe=parSafeDom);
  
    var data : [dom.tableDom] eltType;
  
    var tmpDom = {0..(-1:chpl_table_index_type)};
    var tmpTable: [tmpDom] eltType;
  
    //
    // Standard internal array interface
    // 
  
    proc dsiGetBaseDom() return dom;
  
    proc clearEntry(idx: idxType, haveLock = false) {
      const initval: eltType;
      dsiAccess(idx, haveLock) = initval;
    }

    // ref version
    proc dsiAccess(idx : idxType, haveLock = false) ref {
      const shouldLock = dom.parSafe && !haveLock;
      if shouldLock then dom.lockTable();
      var (found, slotNum) = dom._findFilledSlot(idx, haveLock=true);
      if found {
        if shouldLock then dom.unlockTable();
        return data(slotNum);
      } else if slotNum != -1 { // do an insert using the slot we found
        if dom._arrs.length != 1 {
          halt("cannot implicitly add to an array's domain when the domain is used by more than one array: ", dom._arrs.length);
          return data(0);
        } else {
          const (newSlot, _) = dom._addWrapper(idx, slotNum, haveLock=true);
          if shouldLock then dom.unlockTable();
          return data(newSlot);
        }
      } else {
        halt("array index out of bounds: ", idx);
        return data(0);
      }
    }

    // value version for POD types
    proc dsiAccess(idx : idxType, haveLock = false)
    where !shouldReturnRvalueByConstRef(eltType) {
      const shouldLock = dom.parSafe && !haveLock;
      if shouldLock then dom.lockTable();
      var (found, slotNum) = dom._findFilledSlot(idx, haveLock=true);
      if found {
        if shouldLock then dom.unlockTable();
        return data(slotNum);
      } else {
        halt("array index out of bounds: ", idx);
        return data(0);
      }
    }
    // const ref version for strings, records with copy ctor
    proc dsiAccess(idx : idxType, haveLock = false) const ref
    where shouldReturnRvalueByConstRef(eltType) {
      const shouldLock = dom.parSafe && !haveLock;
      if shouldLock then dom.lockTable();
      var (found, slotNum) = dom._findFilledSlot(idx, haveLock=true);
      if found {
        if shouldLock then dom.unlockTable();
        return data(slotNum);
      } else {
        halt("array index out of bounds: ", idx);
        return data(0);
      }
    }
  
    iter these() ref {
      for slot in dom {
        yield dsiAccess(slot);
      }
    }

    iter these(param tag: iterKind) ref where tag == iterKind.standalone {
      if debugDefaultAssoc {
        writeln("*** In associative array standalone iterator");
      }
      const numIndices = dom.tableSize;
      const numChunks = _computeNumChunks(numIndices);
      if numChunks == 1 {
        for slot in 0..#numIndices {
          if dom.table[slot].status == chpl__hash_status.full {
            yield data[slot];
          }
        }
      } else {
        coforall chunk in 0..#numChunks {
          const (lo, hi) = _computeBlock(numIndices, numChunks,
                                         chunk, numIndices-1);
          if debugAssocDataPar {
            writeln("In associative array standalone iterator: chunk = ", chunk);
          }
          var table = dom.table;
          for slot in lo..hi {
            if dom.table[slot].status == chpl__hash_status.full {
              yield data[slot];
            }
          }
        }
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      for followThis in dom.these(tag) do
        yield followThis;
    }
  
    iter these(param tag: iterKind, followThis) ref where tag == iterKind.follower {
      var (chunk, followThisDom) = followThis;

      if debugDefaultAssoc then
        writeln("In array follower code: Following ", chunk);

      const sameDom = followThisDom == this.dom;

      if !sameDom then
        if followThisDom.dsiNumIndices != this.dom.dsiNumIndices then
          halt("zippered associative array does not match the iterated domain");

      var otherTable = followThisDom.table;
      for slot in chunk.low..chunk.high {
        var entry = otherTable[slot];
        if entry.status == chpl__hash_status.full {
          var idx = slot;
          if !sameDom {
            const (match, loc) = dom._findFilledSlot(entry.idx, haveLock=true);
            if !match then halt("zippered associative array does not match the iterated domain");
            idx = loc;
          }
          yield data[idx];
        }
      }
    }
  
    proc dsiSerialReadWrite(f /*: Reader or Writer*/) {
      var first = true;
      for val in this {
        if (first) then
          first = false;
        else
          f <~> new ioLiteral(" ");
        f <~> val;
      }
    }
    proc dsiSerialWrite(f) { this.dsiSerialReadWrite(f); }
    proc dsiSerialRead(f) { this.dsiSerialReadWrite(f); }
  
  
    //
    // Associative array interface
    //
  
    iter dsiSorted() {
      use Sort;
      var tableCopy: [0..dom.dsiNumIndices-1] eltType;
      for (copy, slot) in zip(tableCopy.domain, dom._fullSlots()) do
        tableCopy(copy) = data(slot);
  
      sort(tableCopy);
  
      for elem in tableCopy do
        yield elem;
    }
  
  
    //
    // Internal associative array interface
    //
  
    proc _backupArray() {
      tmpDom = dom.tableDom;
      tmpTable = data;
    }
  
    proc _removeArrayBackup() {
      tmpDom = {0..(-1:chpl_table_index_type)};
    }
  
    proc _preserveArrayElement(oldslot, newslot) {
      data(newslot) = tmpTable[oldslot];
    }

    proc dsiTargetLocales() {
      compilerError("targetLocales is unsupported by associative domains");
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain() {
      return _newDomain(dom);
    }
  }
  
  
  proc chpl__defaultHashWrapper(x): chpl_table_index_type {
    const hash = chpl__defaultHash(x); 
    return (hash & max(chpl_table_index_type)): chpl_table_index_type;
  }
  
  
  // Thomas Wang's 64b mix function from http://www.concentric.net/~Ttwang/tech/inthash.htm
  proc _gen_key(i: int(64)): int(64) {
    var key = i;
    key += ~(key << 32);
    key = key ^ (key >> 22);
    key += ~(key << 13);
    key = key ^ (key >> 8);
    key += (key << 3);
    key = key ^ (key >> 15);
    key += ~(key << 27);
    key = key ^ (key >> 31);
    return (key & max(int(64))): int(64);  // YAH, make non-negative
  }
  
  inline proc chpl__defaultHash(b: bool): int(64) {
    if (b) then
      return 0;
    else
      return 1;
  }
  
  inline proc chpl__defaultHash(i: int(64)): int(64) {
    return _gen_key(i);
  }
  
  inline proc chpl__defaultHash(u: uint(64)): int(64) {
    return _gen_key(u:int(64));
  }
  
  inline proc chpl__defaultHash(f: real): int(64) {
    return _gen_key(__primitive( "real2int", f));
  }
  
  inline proc chpl__defaultHash(c: complex): int(64) {
    return _gen_key(__primitive("real2int", c.re) ^ __primitive("real2int", c.im)); 
  }
  
  inline proc chpl__defaultHash(a: imag): int(64) {
    return _gen_key(__primitive( "real2int", _i2r(a)));
  }
  
  inline proc chpl__defaultHash(u: chpl_taskID_t): int(64) {
    return _gen_key(u:int(64));
  }
  
  // TODO: maybe move this into Strings.chpl
  // Use djb2 (Dan Bernstein in comp.lang.c)
  inline proc chpl__defaultHash(x : string): int(64) {
    var hash: int(64) = 0;
    for c in 0..#(x.length) {
      hash = ((hash << 5) + hash) ^ x.buff[c];
    }
    return _gen_key(hash);
  }

  inline proc chpl__defaultHash(l : []) {
      var hash : int(64) = 0;
      for obj in l {
          hash = (31 * hash) + chpl__defaultHash(obj);
      }
  
      return _gen_key(hash);
  }
  
  inline proc chpl__defaultHash(o: object): int(64) {
    return _gen_key(__primitive( "object2int", o));
  }
  
  // Is 'idxType' legal to create a default associative domain with?
  // Currently based on the availability of chpl__defaultHash().
  // Enumerated, opaque, and sparse domains are handled separately.
  // Tuples and records also work, somehow.
  proc chpl__validDefaultAssocDomIdxType(type idxType) param return false;
  
  proc chpl__validDefaultAssocDomIdxType(type idxType) param where
      // one check per an implementation of chpl__defaultHash() above
      isBoolType(idxType)     ||
      isIntType(idxType)      ||
      isUintType(idxType)    ||
      isRealType(idxType)        ||
      isImagType(idxType)        ||
      isComplexType(idxType)     ||
      idxType == chpl_taskID_t    ||
      idxType == string           ||
      idxType == c_string         ||
      isClassType(idxType)        ||
      // these are handled differently
      isEnumType(idxType)  ||
      isTupleType(idxType)        ||
      isRecordType(idxType)
  {
    return true;
  }
  
}
