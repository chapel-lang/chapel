/*
 * Copyright 2004-2015 Cray Inc.
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
  
  use Sort /* only QuickSort */;
  
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
  
  proc chpl__primes return (23, 53, 97, 193, 389, 769, 1543,
                           3079, 6151, 12289, 24593, 49157, 98317, 196613,
                           393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
                           50331653, 100663319, 201326611, 402653189, 805306457, 1610612741);
  
  class DefaultAssociativeDom: BaseAssociativeDom {
    type idxType;
    param parSafe: bool;
  
    var dist: DefaultDist;
  
    // The guts of the associative domain
  
    // We explicitly use processor atomics here since this is not
    // by design a distributed data structure
    var numEntries: atomic_int64;
    var tableLock: atomicflag; // do not access directly, use function below
    var tableSizeNum = 1;
    var tableSize = chpl__primes(tableSizeNum);
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
                      typeToString(idxType), " are not allowed", 2);
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
    proc dsiSerialWrite(f: Writer) { this.dsiSerialReadWrite(f); }
    proc dsiSerialRead(f: Reader) { this.dsiSerialReadWrite(f); }
  
    //
    // Standard user domain interface
    //
  
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
      if followThisDom != this {
        // check to see if domains match
        var followThisTab = followThisDom.table;
        var myTab = table;
        var mismatch = false;
        // could use a reduction
        for slot in chunk.low..chunk.high do
          if followThisTab[slot].status != myTab[slot].status {
            mismatch = true;
            break;
          }
        if mismatch then
          halt("zippered associative domains do not match");
      }
  
      if debugDefaultAssoc then
        writeln("In domain follower code: Following ", chunk);
  
      for slot in chunk.low..chunk.high do
        if table[slot].status == chpl__hash_status.full then
          yield table[slot].idx;
    }
  
    //
    // Associative Domain Interface
    //
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
  
    proc dsiAdd(idx: idxType, in slotNum : index(tableDom) = -1, haveLock = !parSafe): index(tableDom) {
      const inSlot = slotNum;
      on this {
        const shouldLock = !haveLock && parSafe;
        if shouldLock then lockTable();
        var findAgain = shouldLock;
        if ((numEntries.read()+1)*2 > tableSize) {
          _resize(grow=true);
          findAgain = true;
        }
        if findAgain then
          slotNum = _add(idx, -1);
        else
          _add(idx, inSlot);
        if shouldLock then unlockTable();
      }
      return slotNum;
    }

    // This routine adds new indices without checking the table size and
    //  is thus appropriate for use by routines like _resize().
    //
    // NOTE: Calls to this routine assume that the tableLock has been acquired.
    //
    proc _add(idx: idxType, in slotNum : index(tableDom) = -1): index(tableDom) {
      var foundSlot : bool = (slotNum != -1);
      if !foundSlot then
        (foundSlot, slotNum) = _findEmptySlot(idx);
      if foundSlot {
        table[slotNum].status = chpl__hash_status.full;
        table[slotNum].idx = idx;
        numEntries.add(1);
      } else {
        if (slotNum < 0) {
          halt("couldn't add ", idx, " -- ", numEntries.read(), " / ", tableSize, " taken");
          return -1;
        }
        // otherwise, re-adding an index that's already in there
      }
      return slotNum;
    }
  
    proc dsiRemove(idx: idxType) {
      on this {
        if parSafe then lockTable();
        const (foundSlot, slotNum) = _findFilledSlot(idx, haveLock=parSafe);
        if (foundSlot) {
          for a in _arrs do
            a.clearEntry(idx, true);
          table[slotNum].status = chpl__hash_status.deleted;
          numEntries.sub(1);
        } else {
          halt("index not in domain: ", idx);
        }
        if (numEntries.read()*8 < tableSize && tableSizeNum > 1) {
          _resize(grow=false);
        }
        if parSafe then unlockTable();
      }
    }
  
    proc dsiRequestCapacity(numKeys:int) {
      var entries = numEntries.read();

      if entries < numKeys {

        //Find the first suitable prime
        var threshhold = (numKeys + 1) * 2;
        var prime = 0;
        var primeLoc = 0;
        for i in 1..chpl__primes.size {
            if chpl__primes(i) > threshhold {
              prime = chpl__primes(i);
              primeLoc = i;
              break;
            }
        }

        //No suitable prime found
        if prime == 0 {
          halt("Requested capacity (", numKeys, ") exceeds maximum size");
        }

        //Changing underlying strucure, time for locking
        if parSafe then lockTable();
        if entries > 0 {
          // Slow path: back up required
          _backupArrays();

          // copy the table (TODO: could use swap between two versions)
          var copyDom = tableDom;
          var copyTable: [copyDom] chpl_TableEntry(idxType) = table;

          tableSizeNum=primeLoc;
          tableSize=prime;
          tableDom = {0..tableSize-1};

          //numEntries will be reconstructed as keys are readded
          numEntries.write(0);

          // insert old data into newly resized table
          for slot in _fullSlots(copyTable) {
            const newslot = _add(copyTable[slot].idx);
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
  
    iter dsiSorted() {
      var tableCopy: [0..#numEntries.read()] idxType;
  
      for (tmp, slot) in zip(tableCopy.domain, _fullSlots()) do
        tableCopy(tmp) = table[slot].idx;
  
      QuickSort(tableCopy);
  
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
        const newslot = _add(copyTable[slot].idx);
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
    iter _lookForSlots(idx: idxType, const in numSlots = tableSize) {
      const baseSlot = chpl__defaultHashWrapper(idx);
      for probe in 0..numSlots/2 {
        yield (baseSlot + probe**2)%numSlots;
      }
    }
  
    // TODO: This may cause problems if the assignment goes through a deref temp.
    iter _fullSlots(tab = table) {
      for slot in tab.domain {
        if tab[slot].status == chpl__hash_status.full then
          yield slot;
      }
    }
  }
  
  pragma "auto copy fn"
  proc chpl__autoCopy(x: DefaultAssociativeDom) {
    if ! noRefCount then
      x.incRefCount();
    return x;
  }
  
  // TODO: See if the autoDestroy can be pushed up into a base class.
  proc chpl__autoDestroy(x: DefaultAssociativeDom) {
    if !noRefCount {
      var cnt = x.destroyDom();
      if cnt == 0 then
        delete x;
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
  
    proc ~DefaultAssociativeArr() {
      on dom {
        local dom.remove_arr(this);
        if ! noRefCount {
          var cnt = dom.destroyDom();
          if cnt == 0 then
            delete dom;
        }
      }
    }

    proc clearEntry(idx: idxType, haveLock = false) {
      const initval: eltType;
      dsiAccess(idx, haveLock) = initval;
    }

    proc dsiAccess(idx : idxType, haveLock = false) ref {
      const shouldLock = dom.parSafe && !haveLock;
      if shouldLock then dom.lockTable();
      var (found, slotNum) = dom._findFilledSlot(idx, haveLock=true);
      if found {
        if shouldLock then dom.unlockTable();
        return data(slotNum);
      }
      else if setter && slotNum != -1 { // do an insert using the slot we found
        if dom._arrs.length != 1 {
          halt("cannot implicitly add to an array's domain when the domain is used by more than one array: ", dom._arrs.length);
          return data(0);
        } else {
          const newSlot = dom.dsiAdd(idx, slotNum, haveLock=true);
          if shouldLock then dom.unlockTable();
          return data(newSlot);
        }
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
      if followThisDom != dom {
        // check to see if domains match
        var followThisTab = followThisDom.table;
        var myTab = dom.table;
        var mismatch = false;
        // could use a reduction
        for slot in chunk.low..chunk.high do
          if followThisTab[slot].status != myTab[slot].status {
            mismatch = true;
            break;
          }
        if mismatch then
          halt("zippered associative array does not match the iterated domain");
      }
      if debugDefaultAssoc then
        writeln("In array follower code: Following ", chunk);
      var tab = dom.table;  // cache table for performance
      for slot in chunk.low..chunk.high do
        if tab[slot].status == chpl__hash_status.full then
          yield data(slot);
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
    proc dsiSerialWrite(f: Writer) { this.dsiSerialReadWrite(f); }
    proc dsiSerialRead(f: Reader) { this.dsiSerialReadWrite(f); }
  
  
    //
    // Associative array interface
    //
  
    iter dsiSorted() {
      var tableCopy: [0..dom.dsiNumIndices-1] eltType;
      for (copy, slot) in zip(tableCopy.domain, dom._fullSlots()) do
        tableCopy(copy) = data(slot);
  
      QuickSort(tableCopy);
  
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

  pragma "auto copy fn"
  proc chpl__autoCopy(x: DefaultAssociativeArr) {
    if !noRefCount then
      x.incRefCount();
    return x;
  }

  proc chpl__autoDestroy(x: DefaultAssociativeArr) {
    if !noRefCount {
      var cnt = x.destroyArr();
      if cnt == 0 then
        delete x;
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
