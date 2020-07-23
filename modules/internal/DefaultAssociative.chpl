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
  use ChapelDistribution, ChapelRange, SysBasic, ChapelArray;
  use ChapelBase, ChapelLocks, IO;
  use ChapelHashing, ChapelHashtable;

  config param debugDefaultAssoc = false;
  config param debugAssocDataPar = false;

  // helps to move around array elements when rehashing the domain
  class DefaultAssociativeDomRehashHelper : chpl__rehashHelpers {
    var dom: unmanaged DefaultAssociativeDom;
    override proc startRehash(newSize: int) {
      for arr in dom._arrs {
        arr._startRehash(newSize);
      }
    }
    override proc moveElementDuringRehash(oldSlot: int, newSlot: int) {
      for arr in dom._arrs {
        arr._moveElementDuringRehash(oldSlot, newSlot);
      }
    }
    override proc finishRehash(oldSize: int) {
      for arr in dom._arrs {
        arr._finishRehash(oldSize);
      }
    }
  }

  class DefaultAssociativeDom: BaseAssociativeDom {
    type idxType;
    param parSafe: bool;

    var dist: unmanaged DefaultDist;

    // We explicitly use processor atomics here since this is not
    // by design a distributed data structure
    var numEntries: chpl__processorAtomicType(int);
    var tableLock: if parSafe then chpl_LocalSpinlock else nothing;
    var table: chpl__hashtable(idxType, nothing);

    inline proc lockTable() {
      if parSafe then tableLock.lock();
    }

    inline proc unlockTable() {
      if parSafe then tableLock.unlock();
    }

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
      this.table = new chpl__hashtable(idxType, nothing);
      this.complete();

      // set the rehash helpers
      this.table.rehashHelpers =
        new DefaultAssociativeDomRehashHelper(this:unmanaged class);
    }
    proc deinit() {
      // chpl__hashtable.deinit does all we need here
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
      table.postponeResize = true;
      for i in this.these() do
        yield i;
      on this {
        table.postponeResize = false;

        lockTable();
        defer {
          unlockTable();
        }

        table.maybeShrinkAfterRemove();
      }
    }

    inline proc _isSlotFull(slot: int): bool {
      return table.table[slot].isFull();
    }

    iter these() {
      for slot in table.allSlots() {
        ref aSlot = table.table[slot];
        if aSlot.status == chpl__hash_status.full {
          yield aSlot.key;
        }
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.standalone {
      if debugDefaultAssoc {
        writeln("*** In associative domain standalone iterator");
      }

      for slot in table.allSlots(tag=tag) {
        ref aSlot = table.table[slot];
        if aSlot.status == chpl__hash_status.full {
          yield aSlot.key;
        }
      }
    }

    iter these(param tag: iterKind) where tag == iterKind.leader {
      if debugDefaultAssoc then
        writeln("*** In associative domain leader iterator");

      for chunk in table.allSlots(tag=tag) do
        yield (chunk, this);
    }

    iter these(param tag: iterKind, followThis) where tag == iterKind.follower {
      var (chunk, followThisDom) = followThis;

      if debugDefaultAssoc then
        writeln("In associative domain follower code");

      const sameDom = followThisDom == this;

      if !sameDom then
        if followThisDom.dsiNumIndices != this.dsiNumIndices then
          halt("zippered associative domains do not match");

      const otherTable = followThisDom.table.table;
      for slot in chunk {
        const ref aSlot = otherTable[slot];
        if aSlot.isFull() {
          var idx = slot;
          if !sameDom {
            const (match, loc) = table.findFullSlot(aSlot.key);
            if !match then halt("zippered associative domains do not match");
            idx = loc;
          }
          yield table.table[idx].key;
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
        for slot in table.allSlots() {
          ref aSlot = table.table[slot];
          if aSlot.isFull() {
            var tmpKey: idxType;
            var tmpVal: nothing;
            table.clearSlot(aSlot, tmpKey, tmpVal);
            // deinit any array entries
            for arr in _arrs {
              arr._deinitSlot(slot);
            }
          }
          table.table[slot].status = chpl__hash_status.empty;
        }
        numEntries.write(0);
        table.maybeShrinkAfterRemove();
        unlockTable();
      }
    }

    proc dsiMember(idx: idxType): bool {
      lockTable(); defer { unlockTable(); }
      var (foundFullSlot, slotNum) = table.findFullSlot(idx);
      return foundFullSlot;
    }

    // returns the number of indices added
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

    proc _addWrapper(in idx: idxType) {
      var slotNum = -1;
      var retVal = 0;

      on this {
        lockTable();
        defer {
          unlockTable();
        }

        (slotNum, retVal) = _add(idx);
      }

      return (slotNum, retVal);
    }

    proc _add(in idx: idxType) {
      var foundFullSlot = false;
      var slotNum = -1;
      (foundFullSlot, slotNum) = table.findAvailableSlot(idx);
      if foundFullSlot {
        return (slotNum, 0);
      } else {
        // Add the element since it was not already present
        table.fillSlot(slotNum, idx, none);
        numEntries.add(1);

        // default initialize newly added array elements
        for arr in _arrs {
          arr._defaultInitSlot(slotNum);
        }

        return (slotNum, 1);
      }
    }

    // returns the number of indices removed
    proc dsiRemove(idx: idxType) {
      var retval: int;

      on this {
        lockTable();
        defer {
          unlockTable();
        }

        const (foundSlot, slotNum) = table.findFullSlot(idx);
        if foundSlot {
          var tmpIdx: idxType;
          var tmpVal: nothing;
          table.clearSlot(slotNum, tmpIdx, tmpVal);
          numEntries.sub(1);

          // deinit any array entries
          for arr in _arrs {
            arr._deinitSlot(slotNum);
          }
          retval = 1;
        } else {
          retval = 0;
        }
        table.maybeShrinkAfterRemove();
      }
      return retval;
    }

    proc dsiRequestCapacity(numKeys:int) {
      on this {
        var entries = numEntries.read();

        if entries < numKeys {

          lockTable();
          defer {
            unlockTable();
          }
          table.requestCapacity(numKeys);

        } else {
          warning("Requested capacity (", numKeys, ") ",
                  "is less than current size (", entries, ")");
        }
      }
    }

    iter dsiSorted(comparator) {
      use Sort;

      var tableCopy: [0..#numEntries.read()] idxType =
        for slot in _fullSlots() do table.table[slot].key;

      sort(tableCopy, comparator=comparator);

      for ind in tableCopy do
        yield ind;
    }

    iter _fullSlots() {
      for slot in table.allSlots() {
        if table.isSlotFull(slot) {
          yield slot;
        }
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

      var tableSize = dom.table.tableSize;

      this.data = dom.table.allocateData(tableSize, eltType);
      this.tmpData = nil;
      this.complete();

      if initElts {
        if isNonNilableClass(this.eltType) {
          param msg = "Cannot default initialize associative array because"
                    + " element type " + eltType:string
                    + " is a non-nilable class";
          compilerError(msg);
        } else if !isDefaultInitializable(this.eltType) {
          param msg = "Cannot default initialize associative array because"
                    + " element type " + eltType:string
                    + " cannot be default initialized";
          compilerError(msg);
        }
      }

      // Initialize array elements for any full slots
      if initElts {
        var initMethod = init_elts_method(tableSize, eltType);
        select initMethod {
          when ArrayInit.noInit {
          }
          when ArrayInit.serialInit {
            for slot in dom.table.allSlots() {
              if dom._isSlotFull(slot) {
                _doDefaultInitSlot(slot, inAdd=false);
              }
            }
          }
          when ArrayInit.parallelInit {
            forall slot in dom.table.allSlots() {
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
      _ddata_free(data, dom.table.tableSize);
    }

    //
    // Standard internal array interface
    //

    override proc dsiGetBaseDom() return dom;


    // ref version
    proc dsiAccess(idx : idxType) ref {
      // Attempt to look up the value

      // no lock needed
      var (found, slotNum) = dom.table.findFullSlot(idx);

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
      // no lock needed
      var (found, slotNum) = dom.table.findFullSlot(idx);
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
      // no lock needed
      var (found, slotNum) = dom.table.findFullSlot(idx);
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
      for slot in dom.table.allSlots() {
        if dom._isSlotFull(slot) {
          yield data[slot];
        }
      }
    }

    iter these(param tag: iterKind) ref where tag == iterKind.standalone {
      if debugDefaultAssoc {
        writeln("*** In associative array standalone iterator");
      }

      for slot in dom.table.allSlots(tag=tag) {
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

      const otherTable = followThisDom.table.table;
      for slot in chunk {
        const ref aSlot = otherTable[slot];
        if aSlot.isFull() {
          var idx = slot;
          if !sameDom {
            const (match, loc) = dom.table.findFullSlot(aSlot.key);
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
      if !isDefaultInitializable(eltType) {
        if inAdd {
          halt("Can't resize domains whose arrays' elements don't " +
               "have default values");
        } else {
          halt("Can't default initialize associative arrays whose " +
               "elements have no default value");
        }
      } else {
        // default initialize an element and move it in to the
        // uninitialized storage.
        pragma "no auto destroy"
        var initval: eltType; // default initialize
        ref dst = data[slot];
        __primitive("=", dst, initval);
      }
    }

    override proc _defaultInitSlot(slot: int) {
      _doDefaultInitSlot(slot, inAdd=true);
    }

    override proc _deinitSlot(slot: int) {
      // deinitialize the element at idx
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
      data = dom.table.allocateData(newSize, eltType);
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
      // runs the post-allocate on the array in dom.allocateData
      // (because not all elements are necessarily initialized, but
      //  the access pattern is predictable at least in default forall
      //  iteration).
    }

    override proc dsiDestroyArr(param deinitElts:bool) {
      if deinitElts {
        if _elementNeedsDeinit() {
          if _deinitElementsIsParallel(eltType) {
            forall slot in dom.table.allSlots() {
              if dom._isSlotFull(slot) {
                _deinitElement(data[slot]);
              }
            }
          } else {
            for slot in dom.table.allSlots() {
              if dom._isSlotFull(slot) {
                _deinitElement(data[slot]);
              }
            }
          }
        }
      }
    }
  }
}
