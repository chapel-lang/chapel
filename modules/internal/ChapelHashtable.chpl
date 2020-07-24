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

// ChapelHashtable.chpl
//
// This is a low-level hashtable for use by DefaultAssociative.
// The API exposes lots of implementation details.
//
// chpl_TableEntry is the type for each hashtable slot
// chpl__hashtable is the record implementing a hashtable
// chpl__defaultHash is the default hash function for most types
pragma "unsafe"
module ChapelHashtable {

  use ChapelBase, DSIUtil;

  // empty needs to be 0 so memset 0 sets it
  enum chpl__hash_status { empty=0, full, deleted };

  record chpl_TableEntry {
    var status: chpl__hash_status = chpl__hash_status.empty;
    var key;
    var val;
    inline proc isFull() {
      return this.status == chpl__hash_status.full;
    }
  }

  private inline proc chpl__primes return
    (0, 23, 53, 89, 191, 383, 761, 1531, 3067, 6143, 12281, 24571, 49139, 98299,
     196597, 393209, 786431, 1572853, 3145721, 6291449, 12582893, 25165813,
     50331599, 100663291, 201326557, 402653171, 805306357, 1610612711, 3221225461,
     6442450939, 12884901877, 25769803751, 51539607551, 103079215087,
     206158430183, 412316860387, 824633720831, 1649267441651, 3298534883309,
     6597069766631, 13194139533299, 26388279066623, 52776558133177,
     105553116266489, 211106232532969, 422212465065953, 844424930131963,
     1688849860263901, 3377699720527861, 6755399441055731, 13510798882111483,
     27021597764222939, 54043195528445869, 108086391056891903, 216172782113783773,
     432345564227567561, 864691128455135207);

  // ### allocation helpers ###

  // returns the value referred to by arg
  // arg should be considered uninitialized after this point
  private proc _moveToReturn(const ref arg) {
    if arg.type == nothing {
      return none;
    } else {
      pragma "no init"
      pragma "no copy"
      pragma "no auto destroy"
      var moved: arg.type;
      __primitive("=", moved, arg);
      return moved;
    }
  }
  // sets lhs to rhs using a move initialization
  // only makes sense if lhs is currently uninitialized
  private proc _moveInit(ref lhs, pragma "no auto destroy" in rhs) {
    if lhs.type != rhs.type {
      compilerError("type mismatch in _moveInit");
    }
    if lhs.type == nothing {
      // then do nothing
    } else {
      __primitive("=", lhs, rhs);
    }
  }

  // Leaves the elements 0 initialized
  private proc _allocateData(size:int, type tableEltType) {

    if size == 0 then
      halt("attempt to allocate hashtable with size 0");

    var callPostAlloc: bool;
    var ret = _ddata_allocate_noinit(tableEltType,
                                     size,
                                     callPostAlloc);

    var initMethod = init_elts_method(size, tableEltType);

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

  private proc _freeData(data, size:int) {
    if data != nil {
      _ddata_free(data, size);
    }
  }

  // #### deinit helpers ####
  private proc _typeNeedsDeinit(type t) param {
    return __primitive("needs auto destroy", t);
  }
  private proc _deinitSlot(ref aSlot: chpl_TableEntry) {
    if _typeNeedsDeinit(aSlot.key.type) {
      chpl__autoDestroy(aSlot.key);
    }
    if _typeNeedsDeinit(aSlot.val.type) {
      chpl__autoDestroy(aSlot.val);
    }
  }

  private inline proc _isSlotFull(const ref aSlot: chpl_TableEntry): bool {
    return aSlot.status == chpl__hash_status.full;
  }

  // #### iteration helpers ####

  // Returns the number of chunks to use in parallel iteration
  private proc _allSlotsNumChunks(size: int) {
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

  iter _allSlots(size: int) {
    for slot in 0..#size {
      yield slot;
    }
  }

  private iter _allSlots(size: int, param tag: iterKind)
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

  private iter _allSlots(size: int, param tag: iterKind)
    where tag == iterKind.leader {

    if debugDefaultAssoc then
      writeln("*** In associative domain _allSlots leader iterator:");

    const numChunks = _allSlotsNumChunks(size);

    if numChunks == 1 {
      yield 0..#size;
    } else {
      coforall chunk in 0..#numChunks {
        const (lo, hi) = _computeBlock(size, numChunks, chunk, size-1);
        if debugDefaultAssoc then
          writeln("*** DI[", chunk, "]: tuple = ", (lo..hi,));
        yield lo..hi;
      }
    }
  }

  private iter _allSlots(size: int, followThis, param tag: iterKind)
    where tag == iterKind.follower {

    var (chunk, followThisDom) = followThis;

    if debugDefaultAssoc then
      writeln("In associative domain _allSlots follower iterator: ",
              "Following ", chunk);

    for slot in chunk {
      yield slot;
    }
  }


  class chpl__rehashHelpers {
    proc startRehash(newSize: int) { }
    proc moveElementDuringRehash(oldSlot: int, newSlot: int) { }
    proc finishRehash(oldSize: int) { }
  }

  record chpl__hashtable {
    type keyType;
    type valType;

    var tableNumFullSlots: int;
    var tableNumDeletedSlots: int;

    var tableSizeNum: int;
    var tableSize: int;
    var table: _ddata(chpl_TableEntry(keyType, valType)); // 0..<tableSize

    var rehashHelpers: owned chpl__rehashHelpers?;

    var postponeResize: bool;

    proc init(type keyType, type valType,
              in rehashHelpers: owned chpl__rehashHelpers? = nil) {
      this.keyType = keyType;
      this.valType = valType;
      this.tableNumFullSlots = 0;
      this.tableNumDeletedSlots = 0;
      this.tableSizeNum = 0;
      this.tableSize = chpl__primes(tableSizeNum);
      this.rehashHelpers = rehashHelpers;
      this.postponeResize = false;
      this.complete();

      // allocates a _ddata(chpl_TableEntry(keyType,valType)) storing the table
      // All elements are memset to 0 (no initializer is run for the idxType)
      // This allows them to be empty, but the key and val
      // are considered uninitialized.
      this.table = allocateTable(this.tableSize);
    }
    proc deinit() {
      // Go through the full slots in the current table and run
      // chpl__autoDestroy on the index
      if _typeNeedsDeinit(keyType) || _typeNeedsDeinit(valType) {
        if _deinitElementsIsParallel(keyType) &&
           _deinitElementsIsParallel(valType) {
          forall slot in _allSlots(tableSize) {
            ref aSlot = table[slot];
            if _isSlotFull(aSlot) {
              _deinitSlot(aSlot);
            }
          }
        } else {
          for slot in _allSlots(tableSize) {
            ref aSlot = table[slot];
            if _isSlotFull(aSlot) {
              _deinitSlot(aSlot);
            }
          }
        }
      }

      // Free the buffer
      _freeData(table, tableSize);
    }

    // #### iteration helpers ####

    inline proc isSlotFull(slot: int): bool {
      return table[slot].status == chpl__hash_status.full;
    }

    iter allSlots() {
      for slot in _allSlots(tableSize) {
        yield slot;
      }
    }

    iter allSlots(param tag: iterKind)
      where tag == iterKind.standalone {

      for slot in _allSlots(tableSize, tag=tag) {
        yield slot;
      }
    }

    iter allSlots(param tag: iterKind)
      where tag == iterKind.leader {

      for followThis in _allSlots(tableSize, tag=tag) {
        yield followThis;
      }
    }

    iter allSlots(followThis, param tag: iterKind)
      where tag == iterKind.follower {

      for i in _allSlots(tableSize, followThis, tag=tag) {
        yield i;
      }
    }


    // #### add & remove helpers ####

    // Searches for 'key' in a filled slot.
    //
    // Returns (filledSlotFound, slot)
    // filledSlotFound will be true if a matching filled slot was found.
    // slot will be the matching filled slot in that event.
    //
    // If no matching slot was found, slot will store an
    // empty slot that may be re-used for faster addition to the domain
    //
    // This function never returns deleted slots.
    proc _findSlot(key: keyType) : (bool, int) {
      var firstOpen = -1;
      for slotNum in _lookForSlots(key) {
        const slotStatus = table[slotNum].status;
        // if we encounter a slot that's empty, our element could not
        // be found past this point.
        if (slotStatus == chpl__hash_status.empty) {
          if firstOpen == -1 then firstOpen = slotNum;
          return (false, firstOpen);
        } else if (slotStatus == chpl__hash_status.full) {
          if (table[slotNum].key == key) {
            return (true, slotNum);
          }
        } else { // this entry was removed, but is the first slot we could use
          if firstOpen == -1 then firstOpen = slotNum;
        }
      }
      return (false, -1);
    }

    iter _lookForSlots(key: keyType, numSlots = tableSize) {
      const baseSlot = chpl__defaultHashWrapper(key):uint;
      if numSlots == 0 then return;
      for probe in 0..numSlots/2 {
        var uprobe = probe:uint;
        var n = numSlots:uint;
        yield ((baseSlot + uprobe**2)%n):int;
      }
    }

    // add pattern:
    //  findAvailableSlot
    //  fillSlot

    // Finds a slot available for adding a key
    // or a slot that was already present with that key.
    // It can rehash the table.
    // returns (foundFullSlot, slotNum)
    proc findAvailableSlot(key: keyType): (bool, int) {
      var slotNum = -1;
      var foundSlot = false;

      if (tableNumFullSlots+tableNumDeletedSlots+1)*2 > tableSize {
        resize(grow=true);
      }

      // Note that when adding elements, if a deleted slot is encountered,
      // later slots need to be checked for the value.
      // That is why this uses the same function that looks for filled slots.
      (foundSlot, slotNum) = _findSlot(key);

      if slotNum >= 0 {
        return (foundSlot, slotNum);
      } else {
        // slotNum < 0
        //
        // This can happen if there are too many deleted elements in the
        // table. In that event, we can garbage collect the table by rehashing
        // everything now.
        rehash(tableSizeNum, tableSize);

        (foundSlot, slotNum) = _findSlot(key);

        if slotNum < 0 {
          // This shouldn't be possible since we just garbage collected
          // the deleted entries & the table should only ever be half
          // full of non-deleted entries.
          halt("couldn't add key -- ", tableNumFullSlots, " / ", tableSize, " taken");
          return (false, -1);
        }
        return (foundSlot, slotNum);
      }
    }

    proc fillSlot(ref tableEntry: chpl_TableEntry(keyType, valType),
                  in key: keyType,
                  in val: valType) {
      if tableEntry.status == chpl__hash_status.full {
        _deinitSlot(tableEntry);
      } else {
        if tableEntry.status == chpl__hash_status.deleted {
          tableNumDeletedSlots -= 1;
        }
        tableNumFullSlots += 1;
      }

      tableEntry.status = chpl__hash_status.full;
      // move the key/val into the table
      _moveInit(tableEntry.key, key);
      _moveInit(tableEntry.val, val);
    }
    proc fillSlot(slotNum: int,
                  in key: keyType,
                  in val: valType) {
      ref tableEntry = table[slotNum];
      fillSlot(tableEntry, key, val);
    }

    // remove pattern:
    //   findFullSlot
    //   clearSlot
    //   maybeShrinkAfterRemove
    //

    // Finds a slot containing a key
    // returns (foundFullSlot, slotNum)
    proc findFullSlot(key: keyType): (bool, int) {
      var slotNum = -1;
      var foundSlot = false;

      (foundSlot, slotNum) = _findSlot(key);

      return (foundSlot, slotNum);
    }

    // Clears a slot that is full
    // (Should not be called on empty/deleted slots)
    // Returns the key and value that were removed in the out arguments
    proc clearSlot(ref tableEntry: chpl_TableEntry(keyType, valType),
                   out key: keyType, out val: valType) {
      // move the table entry into the key/val variables to be returned
      key = _moveToReturn(tableEntry.key);
      val = _moveToReturn(tableEntry.val);

      // set the slot status to deleted
      tableEntry.status = chpl__hash_status.deleted;

      // update the table counts
      tableNumFullSlots -= 1;
      tableNumDeletedSlots += 1;
    }
    proc clearSlot(slotNum: int, out key: keyType, out val: valType) {
      // move the table entry into the key/val variables to be returned
      ref tableEntry = table[slotNum];
      clearSlot(tableEntry, key, val);
    }

    proc maybeShrinkAfterRemove() {
      if (tableNumFullSlots*8 < tableSize && tableSizeNum > 0) {
        resize(grow=false);
      }
    }

    // #### rehash / resize helpers ####

    proc _findPrimeSizeIndex(numKeys:int) {
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

    proc allocateData(size: int, type tableEltType) {
      if size == 0 {
        return nil;
      } else {
        return _allocateData(size, tableEltType);
      }
    }
    proc allocateTable(size:int) {
      if size == 0 {
        return nil;
      } else {
        return _allocateData(size, chpl_TableEntry(keyType, valType));
      }
    }

    // newSize is the new table size
    // newSizeNum is an index into chpl__primes == newSize
    // assumes the array is already locked
    proc rehash(newSizeNum:int, newSize:int) {
      // save the old table
      var oldSize = tableSize;
      var oldTable = table;

      tableSizeNum = newSizeNum;
      tableSize = newSize;

      var entries = tableNumFullSlots;
      if entries > 0 {
        // There were entries, so carefully move them to the a new allocation

        if newSize == 0 {
          halt("attempt to resize to 0 a table that is not empty");
        }

        table = allocateTable(tableSize);

        if rehashHelpers != nil then
          rehashHelpers!.startRehash(tableSize);

        // tableNumFullSlots stays the same during this operation
        // and all all deleleted slots are removed
        tableNumDeletedSlots = 0;

        // Move old data into newly resized table
        //
        // It would be nice if this could be done in parallel
        // but it's possible that multiple old keys will go to the
        // same position in the new array which would lead to data
        // races. So it's not as simple as using forall here.
        for oldslot in _allSlots(oldSize) {
          if oldTable[oldslot].status == chpl__hash_status.full {
            ref oldEntry = oldTable[oldslot];
            // find a destination slot
            var (foundSlot, newslot) = _findSlot(oldEntry.key);
            if foundSlot {
              halt("duplicate element found while resizing for key");
            }
            if newslot < 0 {
              halt("couldn't add element during resize - got slot ", newslot,
                   " for key");
            }

            // move the key and value from the old entry into the new one
            ref dstSlot = table[newslot];
            dstSlot.status = chpl__hash_status.full;
            _moveInit(dstSlot.key, _moveToReturn(oldEntry.key));
            _moveInit(dstSlot.val, _moveToReturn(oldEntry.val));

            // move array elements to the new location
            if rehashHelpers != nil then
              rehashHelpers!.moveElementDuringRehash(oldslot, newslot);
          }
        }

        if rehashHelpers != nil then
          rehashHelpers!.finishRehash(oldSize);

        // delete the old allocation
        _freeData(oldTable, oldSize);

      } else {
        // There were no entries, so just make a new allocation


        if rehashHelpers != nil {
          rehashHelpers!.startRehash(tableSize);
          rehashHelpers!.finishRehash(oldSize);
        }

        // delete the old allocation
        _freeData(oldTable, oldSize);

        table = allocateTable(tableSize);
        tableNumDeletedSlots = 0;
      }
    }

    proc requestCapacity(numKeys:int) {
      if tableNumFullSlots < numKeys {

        var primeLoc = _findPrimeSizeIndex(numKeys);
        var prime = chpl__primes(primeLoc);

        rehash(primeLoc, prime);
      }
    }

    proc resize(grow:bool) {
      if postponeResize then return;

      var newSizeNum = tableSizeNum;
      newSizeNum += if grow then 1 else -1;
      if newSizeNum > chpl__primes.size then
        halt("associative array exceeds maximum size");

      var newSize = chpl__primes(newSizeNum);

      if grow==false && 2*tableNumFullSlots > newSize {
        // don't shrink if the number of elements would not
        // fit into the new size.
        return;
      }

      rehash(newSizeNum, newSize);
    }
  }


  // This is a simple set implementation for internal usage. It's a thin
  // wrapper over chpl__hashtable. It is not parallel safe and is expected to
  // be called on the node that owns it (so uses will likely wrap with
  // locks/on-stmts).
  record chpl__simpleSet {
    type eltType;
    var table: chpl__hashtable(eltType, nothing);

    inline proc size {
      return table.tableNumFullSlots;
    }

    proc add(elem) {
      var (isFullSlot, idx) = table.findAvailableSlot(elem);
      assert(!isFullSlot);
      table.fillSlot(idx, elem, none);
    }

    proc remove(elem) {
      var (hasFoundSlot, idx) = table.findFullSlot(elem);
      // note that this is a noop if the element isn't in the set
      if hasFoundSlot {
        var key: eltType, val: nothing;
        table.clearSlot(idx, key, val);
        table.maybeShrinkAfterRemove();
      }
    }

    iter these() {
      for slot in table.allSlots() do
        if table.isSlotFull(slot) then
          yield table.table[slot].key;
    }

    proc writeThis(f) throws {
      var count = 1;
      f <~> "{";
      for e in this {
        if count <= (size - 1) {
          count += 1;
          f <~> e <~> ", ";
        } else {
          f <~> e;
        }
      }
      f <~> "}";
    }
  }

}
