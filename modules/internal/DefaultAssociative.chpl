/*
 * Copyright 2004-2018 Cray Inc.
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

  use DSIUtil;
  config param debugDefaultAssoc = false;
  config param debugAssocDataPar = false;

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
    var tableSize:int = chpl__primes(tableSizeNum);
    var tableDom = {0..tableSize-1};
    var table: [tableDom] chpl_TableEntry(idxType);
  
    inline proc lockTable() {
      while tableLock.testAndSet() do chpl_task_yield();
    }
  
    inline proc unlockTable() {
      tableLock.clear();
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
    }
  
    //
    // Standard Internal Domain Interface
    //
    proc dsiBuildArray(type eltType) {
      return new DefaultAssociativeArr(eltType=eltType, idxType=idxType,
                                       parSafeDom=parSafe, dom=this);
    }
  
    proc dsiSerialReadWrite(f /*: Reader or Writer*/) {
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
            const (match, loc) = _findFilledSlot(entry.idx, needLock=false);
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
      return _findFilledSlot(idx)(1);
    }
  
    proc dsiAdd(idx) {
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

    proc _addWrapper(idx: idxType, in slotNum : index(tableDom) = -1, 
                     needLock = parSafe) {

      const inSlot = slotNum;
      var retVal = 0;
      on this {
        const shouldLock = needLock && parSafe;
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
        numEntries.add(1);

        // default initialize newly added array elements
        for a in _arrs do
          a.clearEntry(idx);
      } else {
        if (slotNum < 0) {
          halt("couldn't add ", idx, " -- ", numEntries.read(), " / ", tableSize, " taken");
          return (-1, 0);
        }
        // otherwise, re-adding an index that's already in there
        return (slotNum, 0);
      }
      return (slotNum, 1);
    }
  
    proc dsiRemove(idx: idxType) {
      var retval = 1;
      on this {
        if parSafe then lockTable();
        const (foundSlot, slotNum) = _findFilledSlot(idx, needLock=!parSafe);
        if (foundSlot) {
          for a in _arrs do
            a.clearEntry(idx);
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
  
    proc findPrimeSizeIndex(numKeys:int) {
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
      return primeLoc;
    }

    proc dsiRequestCapacity(numKeys:int) {
      var entries = numEntries.read();

      if entries < numKeys {

        var primeLoc = findPrimeSizeIndex(numKeys);
        var prime = chpl__primes(primeLoc);

        //Changing underlying structure, time for locking
        if parSafe then lockTable();
        if entries > 0 {
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
        if parSafe then unlockTable();
      } else if entries > numKeys {
        warning("Requested capacity (" + numKeys + ") " +
                "is less than current size (" + entries + ")");
      }
    }
  
    iter dsiSorted(comparator) {
      use Sort;
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
    // NOTE: Calls to this routine assume that the tableLock has been acquired.
    //
    proc _resize(grow:bool) {
      if postponeResize then return;
      // back up the arrays
      _backupArrays();
  
      // copy the table (TODO: could use swap between two versions)
      var copyDom = tableDom;
      var copyTable: [copyDom] chpl_TableEntry(idxType) = table;
  
      // grow original table
      tableDom = {0..(-1:chpl_table_index_type)}; // non-preserving resize
      numEntries.write(0); // reset, because the adds below will re-set this
      tableSizeNum += if grow then 1 else -1;
      if tableSizeNum > chpl__primes.size then halt("associative array exceeds maximum size");
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
    proc _findFilledSlot(idx: idxType, needLock = true) : (bool, index(tableDom)) {
      if parSafe && needLock then lockTable();
      var firstOpen = -1;
      for slotNum in _lookForSlots(idx, table.domain.high+1) {
        const slotStatus = table[slotNum].status;
        // if we encounter a slot that's empty, our element could not
        // be found past this point.
        if (slotStatus == chpl__hash_status.empty) {
          if firstOpen == -1 then firstOpen = slotNum;
          if parSafe && needLock then unlockTable();
          return (false, firstOpen);
        } else if (slotStatus == chpl__hash_status.full) {
          if (table[slotNum].idx == idx) {
            if parSafe && needLock then unlockTable();
            return (true, slotNum);
          }
        } else { // this entry was removed, but is the first slot we could use
          if firstOpen == -1 then firstOpen = slotNum;
        }
      }
      if parSafe && needLock then unlockTable();
      return (false, -1);
    }

    //
    // NOTE: Calls to this routine assume that the tableLock has been acquired.
    //
    proc _findEmptySlot(idx: idxType): (bool, index(tableDom)) {
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
  
    proc clearEntry(idx: idxType) {
      const initval: eltType;
      dsiAccess(idx) = initval;
    }

    // ref version
    proc dsiAccess(idx : idxType) ref {
      // Attempt to look up the value
      var (found, slotNum) = dom._findFilledSlot(idx, needLock=false);

      // if an element exists for that index, return (a ref to) it
      if found {
        return data[slotNum];

      // if the element didn't exist, then this is either:
      //
      // - an error if the array does not own the domain (it's
      //   trying to get a reference to an element that doesn't exist)
      //
      // - an indication that we should grow the domain + array to
      //   include the element
      } else if slotNum != -1 {

        const arrOwnsDom = dom._arrs.length == 1;
        if !arrOwnsDom {
          // here's the error case
          halt("cannot implicitly add to an array's domain when the domain is used by more than one array: ", dom._arrs.length);
          return data(0);
        } else {
          // grow the table
          const (newSlot, _) = dom._addWrapper(idx, slotNum, needLock=false);

          // and return the element
          return data[newSlot];
        }
      } else {
        halt("array index out of bounds: ", idx);
        return data(0);
      }
    }

    // value version for POD types
    proc dsiAccess(idx : idxType)
    where shouldReturnRvalueByValue(eltType) {
      var (found, slotNum) = dom._findFilledSlot(idx, needLock=false);
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
      var (found, slotNum) = dom._findFilledSlot(idx, needLock=false);
      if found {
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
            const (match, loc) = dom._findFilledSlot(entry.idx, needLock=false);
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
  
    iter dsiSorted(comparator) {
      use Sort;
      var tableCopy: [0..dom.dsiNumIndices-1] eltType;
      for (copy, slot) in zip(tableCopy.domain, dom._fullSlots()) do
        tableCopy(copy) = data(slot);
  
      sort(tableCopy, comparator=comparator);
  
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

    proc dsiDestroyArr() {
      //
      // BHARSH 2017-09-08: Workaround to avoid recursive iterator generation.
      //
      // If this method didn't exist, the compiler would incorrectly think
      // that there was recursion between Replicated, DefaultAssociative, and
      // DefaultRectangular due to virtual method dispatch on dsiDestroyArr.
      //
      // The generated recursive iterator would result in a use-after-free bug
      // for the following test under --no-local:
      //
      // users/npadmana/bugs/replicated_invalid_ref_return/replicated_bug.chpl
      //
    }
  }
  
  
  proc chpl__defaultHashWrapper(x): chpl_table_index_type {
    const hash = chpl__defaultHash(x); 
    return (hash & max(chpl_table_index_type)): chpl_table_index_type;
  }
  
  
  // Thomas Wang's 64b mix function - see
  // https://web.archive.org/web/20060705164341/http://www.concentric.net/~Ttwang/tech/inthash.htm
  proc _gen_key(i: uint): uint {
    var key = i;
    key += ~(key << 32);
    key ^= (key >> 22);
    key += ~(key << 13);
    key ^= (key >> 8);
    key += (key << 3);
    key ^= (key >> 15);
    key += ~(key << 27);
    key ^= (key >> 31);
    return key;
  }
  proc _gen_key(i: int): uint {
    return _gen_key(i:uint);
  }
  
  inline proc chpl__defaultHashCombine(a:uint, b:uint, fieldnum:int): uint {
    extern proc chpl_bitops_rotl_64(x: uint(64), n: uint(64)) : uint(64);
    var n:uint = (17 + fieldnum):uint;
    return a ^ chpl_bitops_rotl_64(b, n);
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
  
  inline proc chpl__defaultHash(o: object): uint {
    return _gen_key(__primitive( "object2int", o));
  }

  //
  // Implementation of chpl__defaultHash for ranges, in case the 'idxType'
  // contains a range in some way (e.g. tuple of ranges).
  //
  inline proc chpl__defaultHash(r : range): uint {
    use Reflection;
    var ret : uint;
    for param i in 1..numFields(r.type) {
      if isParam(getField(r, i)) == false &&
         isType(getField(r, i)) == false &&
         isVoidType(getField(r, i).type) == false {
        const ref field = getField(r, i);
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
