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

// DefaultAssociative.chpl
//
pragma "unsafe" // workaround for trying to default-initialize nil objects
module DefaultAssociative {

  use DSIUtil;
  private use ChapelDistribution, ChapelRange, SysBasic, ChapelArray;
  private use ChapelBase, ChapelLocks, IO;
  config param debugDefaultAssoc = false;
  config param debugAssocDataPar = false;

  // TODO: make the domain parameterized by this?
  type chpl_table_index_type = int;


  /* These declarations could/should both be nested within
     DefaultAssociativeDom? */
  // empty needs to be 0 so memset 0 sets it
  enum chpl__hash_status { empty=0, full, deleted };

  record chpl_TableEntry {
    type idxType;
    var status: chpl__hash_status = chpl__hash_status.empty;
    var idx: idxType;
  }

  // TODO: Would we save compilation time by making this an array?
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

  class DefaultAssociativeDom: BaseAssociativeDom {
    type idxType;
    param parSafe: bool;

    var dist: unmanaged DefaultDist;

    // The guts of the associative domain

    // We explicitly use processor atomics here since this is not
    // by design a distributed data structure
    var numEntries: chpl__processorAtomicType(int);
    var tableLock: if parSafe then chpl_LocalSpinlock else nothing;
    var tableSizeNum = 0;
    var tableSize : int;
    var table: _ddata(chpl_TableEntry(idxType)); // 0..<tableSize

    inline proc lockTable() {
      if parSafe then tableLock.lock();
    }

    inline proc unlockTable() {
      if parSafe then tableLock.unlock();
    }

    // TODO: An ugly [0..-1] domain appears several times in the code --
    //       replace with a named constant/param?
    var postponeResize = false;

    override proc linksDistribution() param return false;
    override proc dsiLinksDistribution() return false;

    proc init(type idxType,
              param parSafe: bool,
              dist: unmanaged DefaultDist) {
      if !chpl__validDefaultAssocDomIdxType(idxType) then
        compilerError("Default Associative domains with idxType=",
                      idxType:string, " are not allowed", 2);

      this.idxType = idxType;
      this.parSafe = parSafe;
      this.dist = dist;
      this.tableSize = chpl__primes(tableSizeNum);
      this.table = nil;

      this.complete();

      this.table = _allocateTable(this.tableSize);
    }
    proc deinit() {
      // Go through the full slots in the current table and run
      // chpl__autoDestroy on the index
      if _keyNeedsDeinit() {
        if _deinitElementsIsParallel(idxType) {
          forall slot in _allSlots() {
            ref aSlot = table[slot];
            if _isSlotFull(aSlot) {
              _deinitSlot(aSlot);
            }
          }
        } else {
          for slot in _allSlots() {
            ref aSlot = table[slot];
            if _isSlotFull(aSlot) {
              _deinitSlot(aSlot);
            }
          }
        }
      }

      // Free the buffer
      _ddata_free(table, tableSize);
    }
    // deinitialization helpers
    proc _keyNeedsDeinit() param {
      return __primitive("needs auto destroy", idxType);
    }
    proc _deinitKey(ref key: idxType) {
      if _keyNeedsDeinit() {
        chpl__autoDestroy(key);
      }
    }
    proc _deinitSlot(ref aSlot: chpl_TableEntry(idxType)) {
      _deinitKey(aSlot.idx);
    }

    // Leaves the elements 0 initialized
    proc _allocateData(size:int, type tableEltType) {
      var callPostAlloc: bool;
      var ret = _ddata_allocate_noinit(tableEltType,
                                       size,
                                       callPostAlloc);

      var initMethod = init_elts_method(size, tableEltType);

      const numChunks = _computeNumChunks(size);
      if numChunks == 1 then
        initMethod = ArrayInit.serialInit;

      const sizeofElement = _ddata_sizeof_element(ret);

      // The memset call below needs to be able to set _array records.
      // But c_ptrTo on an _array will return a pointer to
      // the first element, which messes up the shallowCopy/shallowSwap code
      //
      // As a workaround, this function just returns a pointer to the argument,
      // whether or not it is an array.
      inline proc ptrTo(ref x) {
        return c_pointer_return(x);
      }

      select initMethod {
        when ArrayInit.noInit {
          // do nothing
        }
        when ArrayInit.serialInit {
          for slot in _allSlots(size) {
            c_memset(ptrTo(ret[slot]), 0:uint(8), sizeofElement);
          }
        }
        when ArrayInit.parallelInit {
          // This should match the 'these' iterator in terms of idx->task
          forall slot in _allSlots(size) {
            c_memset(ptrTo(ret[slot]), 0:uint(8), sizeofElement);
          }
        }
        otherwise {
          halt("ArrayInit.heuristicInit should have been made concrete");
        }
      }

      if callPostAlloc {
        _ddata_allocate_postalloc(ret, size);
      }

      return ret;
    }

    // Returns a _ddata(chpl_TableEntry(idxType)) storing the table
    // All elements are memset to 0 (no initializer is run for the idxType)
    // This allows them to be empty, but the idx is considered uninitialized.
    proc _allocateTable(size:int) {
      return _allocateData(size, chpl_TableEntry(idxType));
    }

    //
    // Standard Internal Domain Interface
    //
    proc dsiBuildArray(type eltType, param initElts:bool) {
      return new unmanaged DefaultAssociativeArr(eltType=eltType,
                                                 idxType=idxType,
                                                 parSafeDom=parSafe,
                                                 dom=_to_unmanaged(this),
                                                 initElts=initElts);
    }

    proc dsiSerialReadWrite(f /*: Reader or Writer*/) throws {

      var binary = f.binary();

      if f.writing {
        if binary {
          var numIndices: int = dsiNumIndices;
          f <~> numIndices;
          for idx in this {
            f <~> idx;
          }
        } else {
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
      } else {
        // Clear the domain so it only contains indices read in.
        dsiClear();

        if binary {
          var numIndices: int;
          f <~> numIndices;
          for i in 1..numIndices {
            var idx: idxType;
            f <~> idx;
            dsiAdd(idx);
          }
        } else {
          f <~> new ioLiteral("{");

          var first = true;
          var comma = new ioLiteral(",", true);
          var end = new ioLiteral("}");

          while true {

            // Try reading an end curly. If we get it, then break.
            try {
              f <~> end;
              break;
            } catch err: BadFormatError {
              // We didn't read an end brace, so continue on.
            }

            // Try reading a comma.
            if !first then f <~> comma;
            first = false;

            // Read an index.
            var idx: idxType;
            f <~> idx;
            dsiAdd(idx);
          }
        }
      }
    }
    proc dsiSerialWrite(f) throws { this.dsiSerialReadWrite(f); }
    proc dsiSerialRead(f) throws { this.dsiSerialReadWrite(f); }

    //
    // Standard user domain interface
    //

    proc dsiAssignDomain(rhs: domain, lhsPrivate:bool) {
      chpl_assignDomainWithIndsIterSafeForRemoving(this, rhs);
    }

    inline proc dsiNumIndices {
      return numEntries.read();
    }

    iter dsiIndsIterSafeForRemoving() {
      postponeResize = true;
      for i in this.these() do
        yield i;
      on this {
        postponeResize = false;
        if (numEntries.read()*8 < tableSize && tableSizeNum > 0) {
          lockTable();
          if (numEntries.read()*8 < tableSize && tableSizeNum > 0) {
            _resize(grow=false);
          }
          unlockTable();
        }
      }
    }

    // Returns the number of chunks to use in parallel iteration
    proc _allSlotsNumChunks(size: int) {
      const numTasks = if dataParTasksPerLocale==0 then here.maxTaskPar
                       else dataParTasksPerLocale;
      const ignoreRunning = dataParIgnoreRunningTasks;
      const minSizePerTask = dataParMinGranularity;

      // We are simply slicing up the table here.  Trying to do something
      //  more intelligent (like evenly dividing up the full slots, led
      //  to poor speed ups.

      if debugAssocDataPar {
        writeln("### numTasks = ", numTasks);
        writeln("### ignoreRunning = ", ignoreRunning);
        writeln("### minSizePerTask = ", minSizePerTask);
      }

      var numChunks = _computeNumChunks(numTasks, ignoreRunning,
                                        minSizePerTask,
                                        size);

      if debugAssocDataPar {
        writeln("### numChunks=", numChunks, ", size=", size);
      }

      return numChunks;
    }

    // _allSlots yields all slot numbers, empty or full,
    // but does so in the preferred iteration order across tasks.

    iter _allSlots(size=tableSize) {
      for slot in 0..#size {
        yield slot;
      }
    }

    iter _allSlots(size=tableSize, param tag: iterKind)
      where tag == iterKind.standalone {

      if debugDefaultAssoc {
        writeln("*** In associative domain _allSlots standalone iterator");
      }

      const numChunks = _allSlotsNumChunks(size);

      if numChunks == 1 {
        for slot in 0..#size {
          yield slot;
        }
      } else {
        coforall chunk in 0..#numChunks {
          const (lo, hi) = _computeBlock(size, numChunks, chunk, size-1);
          if debugAssocDataPar then
            writeln("*** chunk: ", chunk, " owns ", lo..hi);
          for slot in lo..hi {
            yield slot;
          }
        }
      }
    }

    iter _allSlots(size=tableSize, param tag: iterKind)
      where tag == iterKind.leader {

      if debugDefaultAssoc then
        writeln("*** In associative domain _allSlots leader iterator:");

      const numChunks = _allSlotsNumChunks(size);

      if numChunks == 1 {
        yield (0..#size, this);
      } else {
        coforall chunk in 0..#numChunks {
          const (lo, hi) = _computeBlock(size, numChunks, chunk, size-1);
          if debugDefaultAssoc then
            writeln("*** DI[", chunk, "]: tuple = ", (lo..hi,));
          yield (lo..hi, this);
        }
      }
    }

    iter _allSlots(size: int, followThis, param tag: iterKind)
      where tag == iterKind.follower {

      var (chunk, followThisDom) = followThis;

      if debugDefaultAssoc then
        writeln("In associative domain _allSlots follower iterator: ",
                "Following ", chunk);

      for slot in chunk {
        yield slot;
      }
    }

    inline proc _isSlotFull(const ref aSlot: chpl_TableEntry): bool {
      return aSlot.status == chpl__hash_status.full;
    }
    inline proc _isSlotFull(slot: int): bool {
      return table[slot].status == chpl__hash_status.full;
    }

    iter these() {
      for slot in _allSlots() {
        ref aSlot = table[slot];
        if _isSlotFull(aSlot) {
          yield aSlot.idx;
        }
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone {
      if debugDefaultAssoc {
        writeln("*** In associative domain standalone iterator");
      }

      for slot in _allSlots(tag=tag) {
        ref aSlot = table[slot];
        if _isSlotFull(aSlot) {
          yield aSlot.idx;
        }
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      if debugDefaultAssoc then
        writeln("*** In associative domain leader iterator");

      for followThis in _allSlots(tag=tag) do
        yield followThis;
    }

    iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
      var (chunk, followThisDom) = followThis;

      if debugDefaultAssoc then
        writeln("In associative domain follower code");

      const sameDom = followThisDom == this;

      if !sameDom then
        if followThisDom.dsiNumIndices != this.dsiNumIndices then
          halt("zippered associative domains do not match");

      var otherTable = followThisDom.table;
      for slot in chunk {
        const ref aSlot = otherTable[slot];
        if _isSlotFull(aSlot) {
          var idx = slot;
          if !sameDom {
            const (match, loc) = _findSlot(aSlot.idx, needLock=false);
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
    override proc dsiMyDist() : unmanaged BaseDist {
      return dist;
    }

    override proc dsiClear() {
      on this {
        lockTable();
        for slot in 0..<tableSize {
          table[slot].status = chpl__hash_status.empty;
        }
        numEntries.write(0);
        unlockTable();
      }
    }

    proc dsiMember(idx: idxType): bool {
      return _findSlot(idx)(0);
    }

    override proc dsiAdd(in idx) {
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
      const numInds = _addWrapper(idx)[1];
      return numInds;
    }

    proc _addWrapper(in idx: idxType, needLock = parSafe) {

      var slotNum = -1;
      var retVal = 0;
      on this {
        if parSafe && needLock then lockTable();
        defer {
          if parSafe && needLock then unlockTable();
        }
        if ((numEntries.read()+1)*2 > tableSize) {
          _resize(grow=true);
        }
        (slotNum, retVal) = _add(idx);
      }
      return (slotNum, retVal);
    }

    // This routine adds new indices without checking the table size and
    //  is thus appropriate for use by routines like _resize().
    //
    // NOTE: Calls to this routine assume that the tableLock has been acquired.
    //

    pragma "unsafe" // see issue #11666
    proc _add(pragma "no auto destroy" in idx: idxType) {
      var slotNum = -1;
      var foundSlot = false;

      // Note that when adding elements, if a deleted slot is encountered,
      // later slots need to be checked for the value.
      // That is why this uses the function to look for filled slots.
      (foundSlot, slotNum) = _findSlot(idx, needLock=false);

      if slotNum < 0 {
        // This can happen if there are too many deleted elements in the
        // table. In that event, we can garbage collect the table by rehashing
        // everything now.
        _do_rehash(tableSizeNum, tableSize);

        (foundSlot, slotNum) = _findSlot(idx, needLock=false);

        if slotNum < 0 {
          // This shouldn't be possible since we just garbage collected
          // the deleted entries & the table should only ever be half
          // full of non-deleted entries.
          halt("couldn't add ", idx, " -- ", numEntries.read(), " / ", tableSize, " taken");
          return (-1, 0);
        }
      }

      if foundSlot {
        // found a full slot

        // re-adding an index that's already in there,
        // so destroy the one passed in
        _deinitKey(idx);

        return (slotNum, 0);

      } else {
        // found an empty or deleted slot

        table[slotNum].status = chpl__hash_status.full;
        ref dst = table[slotNum].idx;
        __primitive("=", dst, idx);
        numEntries.add(1);

        // default initialize newly added array elements
        for arr in _arrs {
          arr._defaultInitSlot(slotNum);
        }
      }
      return (slotNum, 1);
    }

    proc dsiRemove(idx: idxType) {
      var retval = 1;
      on this {
        lockTable();
        const (foundSlot, slotNum) = _findSlot(idx, needLock=!parSafe);
        if (foundSlot) {
          ref aSlot = table[slotNum];
          // deinit the key
          _deinitSlot(aSlot);

          // deinit any array entries
          for arr in _arrs {
            arr._deinitSlot(slotNum);
          }
          aSlot.status = chpl__hash_status.deleted;
          numEntries.sub(1);
        } else {
          retval = 0;
        }
        if (numEntries.read()*8 < tableSize && tableSizeNum > 0) {
          _resize(grow=false);
        }
        unlockTable();
      }
      return retval;
    }

    proc findPrimeSizeIndex(numKeys:int) {
      //Find the first suitable prime
      var threshold = (numKeys + 1) * 2;
      var prime = 0;
      var primeLoc = 0;
      for i in 0..#chpl__primes.size {
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
      return primeLoc;
    }

    // newSize is the new table size
    // newSizeNum is an index into chpl__primes == newSize
    // assumes the array is already locked
    proc _do_rehash(newSizeNum, newSize) {
      var entries = numEntries.read();
      if entries > 0 {
        // There were entries, so carefully move them to the a new allocation

        // save the old table
        // oldTable has elements 0..<oldSize
        var oldSize = tableSize;
        var oldTable = table;

        tableSizeNum = newSizeNum;
        tableSize = newSize;
        table = _allocateTable(tableSize);

        for arr in _arrs {
          arr._startRehash(tableSize);
        }

        // numEntries stays the same during this operation

        // Move old data into newly resized table
        //
        // It would be nice if this could be done in parallel
        // but it's possible that multiple old keys will go to the
        // same position in the new array which would lead to data
        // races. So it's not as simple as using forall here.
        for oldslot in _allSlots(oldSize) {
          if oldTable[oldslot].status == chpl__hash_status.full {
            // move the index into a local variable
            pragma "no init" pragma "no auto destroy"
            var stealIdx: idxType;
            __primitive("=", stealIdx, oldTable[oldslot].idx);

            // find a destination slot
            var (foundSlot, newslot) = _findSlot(stealIdx, needLock=false);
            if foundSlot {
              halt("duplicate element found while resizing for idx ", stealIdx);
            }
            if newslot < 0 {
              halt("couldn't add element during resize - got slot ", newslot,
                   " for idx ", stealIdx);
            }

            // move the local variable into the destination slot
            ref dstSlot = table[newslot];
            dstSlot.status = chpl__hash_status.full;
            ref dstIdx = dstSlot.idx;
            __primitive("=", dstIdx, stealIdx);

            // move array elements to the new location
            for arr in _arrs {
              arr._moveElementDuringRehash(oldslot, newslot);
            }
          }
        }

        for arr in _arrs {
          arr._finishRehash(oldSize);
        }

        // delete the old allocation
        _ddata_free(oldTable, oldSize);

      } else {
        // There were no entries, so just make a new allocation

        // delete the old allocation
        _ddata_free(table, tableSize);

        tableSizeNum = newSizeNum;
        tableSize = newSize;
        table = _allocateTable(tableSize);
      }
    }

    proc dsiRequestCapacity(numKeys:int) {
      var entries = numEntries.read();

      if entries < numKeys {

        var primeLoc = findPrimeSizeIndex(numKeys);
        var prime = chpl__primes(primeLoc);

        //Changing underlying structure, time for locking
        lockTable();

        _do_rehash(primeLoc, prime);

        unlockTable();
      } else if entries > numKeys {
        warning("Requested capacity (", numKeys, ") ",
                "is less than current size (", entries, ")");
      }
    }

    iter dsiSorted(comparator) {
      use Sort;

      var tableCopy: [0..#numEntries.read()] idxType =
        for slot in _fullSlots() do table[slot].idx;

      sort(tableCopy, comparator=comparator);

      for ind in tableCopy do
        yield ind;
    }

    //
    // Internal interface (private)
    //
    // NOTE: Calls to this routine assume that the tableLock has been acquired.
    //
    proc _resize(grow:bool) {
      if postponeResize then return;

      var newSizeNum = tableSizeNum;
      newSizeNum += if grow then 1 else -1;
      if newSizeNum > chpl__primes.size then
        halt("associative array exceeds maximum size");

      var newSize = chpl__primes(newSizeNum);

      _do_rehash(newSizeNum, newSize);
    }

    // Searches for 'idx' in a filled slot.
    //
    // Returns (filledSlotFound, slot)
    // filledSlotFound will be true if a matching filled slot was found.
    // slot will be the matching filled slot in that event.
    //
    // If no matching slot was found, slot will store the
    // open slot that may be re-used for faster addition to the domain
    proc _findSlot(idx: idxType, needLock = true) : (bool, int) {
      if parSafe && needLock then lockTable();
      defer {
        if parSafe && needLock then unlockTable();
      }

      var firstOpen = -1;
      for slotNum in _lookForSlots(idx) {
        const slotStatus = table[slotNum].status;
        // if we encounter a slot that's empty, our element could not
        // be found past this point.
        if (slotStatus == chpl__hash_status.empty) {
          if firstOpen == -1 then firstOpen = slotNum;
          return (false, firstOpen);
        } else if (slotStatus == chpl__hash_status.full) {
          if (table[slotNum].idx == idx) {
            return (true, slotNum);
          }
        } else { // this entry was removed, but is the first slot we could use
          if firstOpen == -1 then firstOpen = slotNum;
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

    iter _fullSlots(n = tableSize, tab = table) {
      for slot in 0..<n {
        if tab[slot].status == chpl__hash_status.full then
          yield slot;
      }
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain(loc: locale) {
      if this.locale == loc {
        return _getDomain(_to_unmanaged(this));
      } else {
        var a: domain(idxType, parSafe=parSafe);
        return a;
      }
    }

  }

  class DefaultAssociativeArr: AbsBaseArr {
    type idxType;
    param parSafeDom: bool;
    var dom: unmanaged DefaultAssociativeDom(idxType, parSafe=parSafeDom);

    var dataSize: int;
    // elts initialized when they are added to the domain
    var data: _ddata(eltType);

    // used during rehashes (could move into an array in rehash fn)
    var tmpData: _ddata(eltType);

    proc init(type eltType,
              type idxType,
              param parSafeDom,
              dom:unmanaged DefaultAssociativeDom(idxType, parSafe=parSafeDom),
              param initElts) {
      super.init(eltType=eltType);
      this.idxType = idxType;
      this.parSafeDom = parSafeDom;
      this.dom = dom;
      this.data = dom._allocateData(dom.tableSize, eltType);
      this.tmpData = nil;
      this.complete();

      if initElts && isNonNilableClass(this.eltType) {
        param msg = "Cannot default initialize associative array because"
                  + " element type " + eltType:string
                  + " is a non-nilable class";
        compilerError(msg);
      }

      // Initialize array elements for any full slots
      if initElts {
        var initMethod = init_elts_method(dom.tableSize, eltType);
        select initMethod {
          when ArrayInit.noInit {
          }
          when ArrayInit.serialInit {
            for slot in dom._allSlots() {
              if dom._isSlotFull(slot) {
                _doDefaultInitSlot(slot, inAdd=false);
              }
            }
          }
          when ArrayInit.parallelInit {
            forall slot in dom._allSlots() {
              if dom._isSlotFull(slot) {
                _doDefaultInitSlot(slot, inAdd=false);
              }
            }
          }
          otherwise {
            halt("ArrayInit.heuristicInit should have been made concrete");
          }
        }
      }
    }

    proc deinit() {
      // elements are deinited in dsiDestroyArr if necessary
      _ddata_free(data, dom.tableSize);
    }

    //
    // Standard internal array interface
    //

    override proc dsiGetBaseDom() return dom;


    // ref version
    proc dsiAccess(idx : idxType) ref {
      // Attempt to look up the value
      var (found, slotNum) = dom._findSlot(idx, needLock=false);

      // if an element exists for that index, return (a ref to) it
      if found {
        return data[slotNum];

      // if the element didn't exist, then it is an error
      } else {
        halt("array index out of bounds: ", idx);
      }
    }

    // value version for POD types
    proc dsiAccess(idx : idxType)
    where shouldReturnRvalueByValue(eltType) {
      var (found, slotNum) = dom._findSlot(idx, needLock=false);
      if found {
        return data(slotNum);
      } else {
        halt("array index out of bounds: ", idx);
        return data(0);
      }
    }
    // const ref version for strings, records with copy ctor
    proc dsiAccess(idx : idxType) const ref
    where shouldReturnRvalueByConstRef(eltType) {
      var (found, slotNum) = dom._findSlot(idx, needLock=false);
      if found {
        return data(slotNum);
      } else {
        halt("array index out of bounds: ", idx);
        return data(0);
      }
    }

    inline proc dsiLocalAccess(i) ref
      return dsiAccess(i);

    inline proc dsiLocalAccess(i)
    where shouldReturnRvalueByValue(eltType)
      return dsiAccess(i);

    inline proc dsiLocalAccess(i) const ref
    where shouldReturnRvalueByConstRef(eltType)
      return dsiAccess(i);


    iter these() ref {
      for slot in dom._allSlots() {
        if dom._isSlotFull(slot) {
          yield data[slot];
        }
      }
    }

    iter these(param tag: iterKind) ref where tag == iterKind.standalone {
      if debugDefaultAssoc {
        writeln("*** In associative array standalone iterator");
      }

      for slot in dom._allSlots(tag=tag) {
        if dom._isSlotFull(slot) {
          yield data[slot];
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
      for slot in chunk {
        const ref aSlot = otherTable[slot];
        if followThisDom._isSlotFull(aSlot) {
          var idx = slot;
          if !sameDom {
            const (match, loc) = dom._findSlot(aSlot.idx, needLock=false);
            if !match then halt("zippered associative array does not match the iterated domain");
            idx = loc;
          }
          yield data[idx];
        }
      }
    }

    proc dsiSerialReadWrite(f /*: channel*/) throws {
      var binary = f.binary();
      var arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY);
      var isspace = arrayStyle == QIO_ARRAY_FORMAT_SPACE && !binary;
      var isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;
      var ischpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !binary;

      if !f.writing && ischpl {
        this.readChapelStyleAssocArray(f);
        return;
      }

      if isjson || ischpl then f <~> new ioLiteral("[");

      var first = true;

      for (key, val) in zip(this.dom, this) {
        if first then first = false;
        else if isspace then f <~> new ioLiteral(" ");
        else if isjson || ischpl then f <~> new ioLiteral(", ");

        if f.writing && ischpl {
          f <~> key;
          f <~> new ioLiteral(" => ");
        }

        f <~> val;
      }

      if isjson || ischpl then f <~> new ioLiteral("]");
    }

    proc readChapelStyleAssocArray(f) throws {
      const openBracket = new ioLiteral("[");
      const closedBracket = new ioLiteral("]");
      var first = true;
      var readEnd = false;

      f <~> openBracket;

      while true {
        if first {
          first = false;

          // Break if we read an immediate closed bracket.
          try {
            f <~> closedBracket;
            readEnd = true;
            break;
          } catch err: BadFormatError {
            // We didn't read a closed bracket, so continue on.
          }
        } else {

          // Try reading a comma. If we don't, then break.
          try {
            f <~> new ioLiteral(",");
          } catch err: BadFormatError {
            // Break out of the loop if we didn't read a comma.
            break;
          }
        }

        // Read a key.
        var key: idxType;
        f <~> key;
        f <~> new ioLiteral("=>");

        // Read the value.
        f <~> dsiAccess(key);
      }

      if !readEnd then f <~> closedBracket;
    }

    proc dsiSerialWrite(f) throws { this.dsiSerialReadWrite(f); }
    proc dsiSerialRead(f) throws { this.dsiSerialReadWrite(f); }

    //
    // Associative array interface
    //

    iter dsiSorted(comparator) {
      use Sort;

      var tableCopy: [0..#dom.dsiNumIndices] eltType =
        for slot in dom._fullSlots() do data(slot);

      sort(tableCopy, comparator=comparator);

      for elem in tableCopy do
        yield elem;
    }


    //
    // Internal associative array interface
    //

    // internal helper
    proc _doDefaultInitSlot(slot: int, inAdd: bool) {
      if (isNonNilableClass(eltType)) {
        if inAdd {
          halt("Can't resize domains whose arrays' elements don't have default values");
        } else {
          halt("Can't default initialize associative arrays whose elements have no default value");
        }
      }

      // default initialize an element and move it in to the
      // uninitialized storage.
      pragma "no auto destroy"
      var initval: eltType; // default initialize
      ref dst = data[slot];
      __primitive("=", dst, initval);
    }

    override proc _defaultInitSlot(slot: int) {
      _doDefaultInitSlot(slot, inAdd=true);
    }

    override proc _deinitSlot(slot: int) {
      // deinitalize the element at idx
      _deinitElement(data[slot]);
    }

    proc _elementNeedsDeinit() param {
      return __primitive("needs auto destroy", eltType);
    }
    proc _deinitElement(ref elt: eltType) {
      if _elementNeedsDeinit() {
        chpl__autoDestroy(elt);
      }
    }

    override proc _startRehash(newSize: int) {
      tmpData = data;
      data = dom._allocateData(newSize, eltType);
    }

    override proc _finishRehash(oldSize: int) {
      _ddata_free(tmpData, oldSize);
      tmpData = nil;
    }

    override proc _moveElementDuringRehash(oldslot: int, newslot: int) {
      const ref src = tmpData[oldslot];
      ref dst = data[newslot];
      __primitive("=", dst, src);
    }

    proc dsiTargetLocales() {
      return [this.locale, ];
    }

    proc dsiHasSingleLocalSubdomain() param return true;

    proc dsiLocalSubdomain(loc: locale) {
      if this.locale == loc {
        return _getDomain(dom);
      } else {
        var a: domain(dom.idxType, parSafe=dom.parSafe);
        return a;
      }
    }

    override proc dsiElementInitializationComplete() {
      // No action necessary because associative array
      // runs the post-allocate on the array in _allocateData
      // (because not all elements are necessarily initialized, but
      //  the access pattern is predictable at least in default forall
      //  iteration).
    }

    override proc dsiDestroyArr(param deinitElts:bool) {
      if deinitElts {
        if _elementNeedsDeinit() {
          if _deinitElementsIsParallel(eltType) {
            forall slot in dom._allSlots() {
              if dom._isSlotFull(slot) {
                _deinitElement(data[slot]);
              }
            }
          } else {
            for slot in dom._allSlots() {
              if dom._isSlotFull(slot) {
                _deinitElement(data[slot]);
              }
            }
          }
        }
      }
    }
  }


  proc chpl__defaultHashWrapper(x): chpl_table_index_type {
    const hash = chpl__defaultHash(x);
    return (hash & max(chpl_table_index_type)): chpl_table_index_type;
  }


  // Mix the bits, so that e.g. numbers in 0..N generate
  // random-looking data across all the bits even if N is small.
  proc _gen_key(i: uint): uint {
    // Thomas Wang's 64b mix function - 2007 version - see
    // http://web.archive.org/web/20071223173210/http://www.concentric.net/~Ttwang/tech/inthash.htm
    var key = i;
    key = (~key) + (key << 21); // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8); // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4); // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key;

    // See commit history for this comment for some other mixers
    // worth considering.
  }
  proc _gen_key(i: int): uint {
    return _gen_key(i:uint);
  }

  inline proc chpl__defaultHashCombine(a:uint, b:uint, fieldnum:int): uint {
    extern proc chpl_bitops_rotl_64(x: uint(64), n: uint(64)) : uint(64);
    var n:uint = (17 + fieldnum):uint;
    return _gen_key(a ^ chpl_bitops_rotl_64(b, n));
  }

  inline proc chpl__defaultHash(b: bool): uint {
    if (b) then
      return 0;
    else
      return 1;
  }

  inline proc chpl__defaultHash(i: int(64)): uint {
    return _gen_key(i);
  }

  inline proc chpl__defaultHash(u: uint(64)): uint {
    return _gen_key(u);
  }

  inline proc chpl__defaultHash(e) where isEnum(e) {
    return _gen_key(chpl__enumToOrder(e));
  }

  inline proc chpl__defaultHash(f: real): uint {
    return _gen_key(__primitive( "real2int", f));
  }

  inline proc chpl__defaultHash(c: complex): uint {
    return _gen_key(__primitive("real2int", c.re) ^ __primitive("real2int", c.im));
  }

  inline proc chpl__defaultHash(a: imag): uint {
    return _gen_key(__primitive( "real2int", _i2r(a)));
  }

  inline proc chpl__defaultHash(u: chpl_taskID_t): uint {
    return _gen_key(u:int(64));
  }

  inline proc chpl__defaultHash(l : []): uint {
    var hash : uint = 0;
    var i = 1;
    for obj in l {
      hash = chpl__defaultHashCombine(chpl__defaultHash(obj), hash, i);
      i += 1;
    }
    return hash;
  }

  // Nilable and non-nilable classes will coerce to this.
  inline proc chpl__defaultHash(o: borrowed object?): uint {
    return _gen_key(__primitive( "object2int", o));
  }

  inline proc chpl__defaultHash(l: locale): uint {
    return _gen_key(__primitive( "object2int", l._value));
  }

  //
  // Implementation of chpl__defaultHash for ranges, in case the 'idxType'
  // contains a range in some way (e.g. tuple of ranges).
  //
  inline proc chpl__defaultHash(r : range): uint {
    use Reflection;
    var ret : uint;
    for param i in 1..numImplementationFields(r.type) {
      if isParam(getImplementationField(r, i)) == false &&
         isType(getImplementationField(r, i)) == false &&
         isNothingType(getImplementationField(r, i).type) == false {
        const ref field = getImplementationField(r, i);
        const fieldHash = chpl__defaultHash(field);
        if i == 1 then
          ret = fieldHash;
        else
          ret = chpl__defaultHashCombine(fieldHash, ret, i);
      }
    }
    return ret;
  }

  // Is 'idxType' legal to create a default associative domain with?
  // Currently based on the availability of chpl__defaultHash().
  // Enumerated and sparse domains are handled separately.
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
      idxType == bytes            ||
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
