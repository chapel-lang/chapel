/*
 * Copyright 2020-2023 Hewlett Packard Enterprise Development LP
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
  use ChapelDistribution, ChapelRange, ChapelArray;
  use ChapelBase, ChapelLocks, IO;
  use ChapelHashing, ChapelHashtable;
  use OS;

  config param debugDefaultAssoc = false;
  config param debugAssocDataPar = false;

  config param defaultAssociativeSupportsAutoLocalAccess = true;

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

    override proc linksDistribution() param do return false;
    override proc dsiLinksDistribution() do return false;

    proc init(type idxType,
              param parSafe: bool,
              dist: unmanaged DefaultDist) {
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

    proc dsiSerialWrite(f) throws {
      const binary = f.binary();

      if binary {
        f.write(dsiNumIndices);
        for idx in this {
          f.write(idx);
        }
      } else {
        var first = true;
        f._writeLiteral("{");
        for idx in this {
          if first then
            first = false;
          else
            f._writeLiteral(", ");
          f.write(idx);
        }
        f._writeLiteral("}");
      }
    }
    proc dsiSerialRead(f) throws {
      const binary = f.binary();

      // Clear the domain so it only contains indices read in.
      dsiClear();

      if binary {
        const numIndices: int = f.read(int);
        for i in 1..numIndices {
          dsiAdd(f.read(idxType));
        }
      } else {
        f._readLiteral("{");

        var first = true;

        while true {

          // Try reading an end curly. If we get it, then break.
          try {
            f._readLiteral("}");
            break;
          } catch err: BadFormatError {
            // We didn't read an end brace, so continue on.
          }

          // Try reading a comma.
          if !first then f._readLiteral(",", true);
          first = false;

          // Read an index.
          dsiAdd(f.read(idxType));
        }
      }
    }

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
      foreach slot in table.allSlots() {
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

      foreach slot in table.allSlots(tag=tag) {
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
      foreach slot in chunk {
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

      foreach ind in tableCopy do
        yield ind;
    }

    iter _fullSlots() {
      foreach slot in table.allSlots() {
        if table.isSlotFull(slot) {
          yield slot;
        }
      }
    }

    proc dsiTargetLocales() const ref {
      return chpl_getSingletonLocaleArray(this.locale);
    }

    proc dsiHasSingleLocalSubdomain() param do return true;

    proc dsiLocalSubdomain(loc: locale) {
      if this.locale == loc {
        return _getDomain(_to_unmanaged(this));
      } else {
        var a: domain(idxType, parSafe=parSafe);
        return a;
      }
    }

    override proc dsiSupportsAutoLocalAccess() param {
      return defaultAssociativeSupportsAutoLocalAccess;
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

    // indicates if elements need to be deinitialized
    var eltsNeedDeinit = true;

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
      this.eltsNeedDeinit = initElts;
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
          when ArrayInit.gpuInit {
            // may not be too difficult, not a priority at the moment
            halt("Associative arrays cannot be initialized on GPU locales with",
                 " CHPL_MEM_STRATEGY=array_on_device yet.");
          }
          otherwise {
            halt("ArrayInit.", initMethod,
                 " heuristicInit should have been implemented");
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

    proc rank param { return 1; }

    override proc dsiGetBaseDom() do return dom;


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

    proc dsiAccess(idx: 1*idxType) ref {
      return dsiAccess(idx(0));
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

    proc dsiAccess(idx : 1*idxType) ref
    where shouldReturnRvalueByValue(eltType) {
      return dsiAccess(idx(0));
    }

    // const ref version for strings, records with copy ctor
    proc dsiAccess(idx : idxType) const ref {
      // no lock needed
      var (found, slotNum) = dom.table.findFullSlot(idx);
      if found {
        return data(slotNum);
      } else {
        halt("array index out of bounds: ", idx);
        return data(0);
      }
    }

    proc dsiAccess(idx : 1*idxType) const ref {
      return dsiAccess(idx(0));
    }

    inline proc dsiLocalAccess(i) ref do
      return dsiAccess(i);

    inline proc dsiLocalAccess(i)
    where shouldReturnRvalueByValue(eltType) do
      return dsiAccess(i);

    inline proc dsiLocalAccess(i) const ref do
      return dsiAccess(i);


    iter these() ref {
      foreach slot in dom.table.allSlots() {
        if dom._isSlotFull(slot) {
          yield data[slot];
        }
      }
    }

    iter these(param tag: iterKind) ref where tag == iterKind.standalone {
      if debugDefaultAssoc {
        writeln("*** In associative array standalone iterator");
      }

      foreach slot in dom.table.allSlots(tag=tag) {
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
      foreach slot in chunk {
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

    proc dsiSerialReadWrite(f, in printBraces=true, inout first = true) throws
    where chpl_useIOSerializers && !_isDefaultDeser(f) {
      ref fmt = if f.writing then f.serializer else f.deserializer;

      if f.writing then
        fmt.startMap(f, dom.dsiNumIndices:uint);
      else
        fmt.startMap(f);

      if f.writing {
        for (key, val) in zip(this.dom, this) {
          fmt.writeKey(f, key);
          fmt.writeValue(f, val);
        }
      } else {
        for 0..<dom.dsiNumIndices {
          const k = fmt.readKey(f, idxType);

          if !dom.dsiMember(k) {
            // TODO: throw error
          } else {
            dsiAccess(k) = fmt.readValue(f, eltType);
          }
        }
      }

      fmt.endMap(f);
    }

    proc _isDefaultDeser(f) param : bool {
      if f.writing then return f.serializerType == IO.DefaultSerializer;
      else return f.deserializerType == IO.DefaultDeserializer;
    }

    proc dsiSerialReadWrite(f, in printBraces=true, inout first = true) throws
    where chpl_useIOSerializers && _isDefaultDeser(f) {
      ref fmt = if f.writing then f.serializer else f.deserializer;

      if f.writing then
        fmt.startArray(f, dom.dsiNumIndices:uint);
      else
        fmt.startArray(f);

      if f.writing {
        for (key, val) in zip(this.dom, this) {
          fmt.writeArrayElement(f, val);
        }
      } else {
        compilerError("Formatter '" + f.deserializerType:string + "' does not support reading associative arrays");
      }

      fmt.endArray(f);
    }

    proc dsiSerialReadWrite(f /*: channel*/, in printBraces=true, inout first = true) throws {
      var binary = f.binary();
      var arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY);
      var isspace = arrayStyle == QIO_ARRAY_FORMAT_SPACE && !binary;
      var isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;
      var ischpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !binary;

      if !f.writing && ischpl {
        this.readChapelStyleAssocArray(f);
        return;
      }

      printBraces &&= (isjson || ischpl);

      inline proc rwLiteral(lit:string) throws {
        if f.writing then f._writeLiteral(lit); else f._readLiteral(lit);
      }

      if printBraces then rwLiteral("[");

      for (key, val) in zip(this.dom, this) {
        if first then first = false;
        else if isspace then rwLiteral(" ");
        else if isjson || ischpl then rwLiteral(", ");

        if f.writing && ischpl {
          f.write(key);
          f._writeLiteral(" => ");
        }

        if f.writing then f.write(val);
        else val = f.read(eltType);
      }

      if printBraces then rwLiteral("]");
    }

    proc readChapelStyleAssocArray(f) throws {
      const openBracket   = "[";
      const closedBracket = "]";
      var first = true;
      var readEnd = true;

      f._readLiteral(openBracket);

      while true {
        if first {
          first = false;

          // Break if we read an immediate closed bracket.
          try {
            f._readLiteral(closedBracket);
            readEnd = false;
            break;
          } catch err: BadFormatError {
            // We didn't read a closed bracket, so continue on.
          }
        } else {

          // Try reading a comma. If we don't, then break.
          try {
            f._readLiteral(",");
          } catch err: BadFormatError {
            // Break out of the loop if we didn't read a comma.
            break;
          }
        }

        // Read a key.
        var key: idxType = f.read(idxType);
        f._readLiteral("=>");

        // Read the value.
        dsiAccess(key) = f.read(eltType);
      }

      if readEnd then f._readLiteral(closedBracket);
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

      foreach elem in tableCopy do
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

    proc dsiTargetLocales() const ref {
      return chpl_getSingletonLocaleArray(this.locale);
    }

    proc dsiHasSingleLocalSubdomain() param do return true;

    proc dsiLocalSubdomain(loc: locale) {
      if this.locale == loc {
        return _getDomain(dom);
      } else {
        var a: domain(dom.idxType, parSafe=dom.parSafe);
        return a;
      }
    }

    override proc dsiElementInitializationComplete() {
      // No post-allocate necessary because associative array
      // runs the post-allocate on the array in dom.allocateData
      // (because not all elements are necessarily initialized, but
      //  the access pattern is predictable at least in default forall
      //  iteration).
      this.eltsNeedDeinit = true;
    }

    override proc dsiElementDeinitializationComplete() {
      this.eltsNeedDeinit = false;
    }

    override proc dsiDestroyArr(deinitElts:bool) {
      if deinitElts && this.eltsNeedDeinit {
        if _elementNeedsDeinit() {
          if _deinitElementsIsParallel(eltType, dom.table.tableSize) {
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
      this.eltsNeedDeinit = false;
    }
  }

  proc chpl_serialReadWriteAssociativeHelper(f, arr, dom) throws {
    var binary = f.binary();
    var arrayStyle = f.styleElement(QIO_STYLE_ELEMENT_ARRAY);
    var isspace = arrayStyle == QIO_ARRAY_FORMAT_SPACE && !binary;
    var isjson = arrayStyle == QIO_ARRAY_FORMAT_JSON && !binary;
    var ischpl = arrayStyle == QIO_ARRAY_FORMAT_CHPL && !binary;

    if !f.writing && ischpl {
      halt("This form of I/O on a default array slice is not yet supported");
      return;
    }

    inline proc rwLiteral(lit:string) throws {
      if f.writing then f._writeLiteral(lit); else f._readLiteral(lit);
    }

    if isjson || ischpl then rwLiteral("[");

    var first = true;

    for key in dom {
      if first then first = false;
      else if isspace then rwLiteral(" ");
      else if isjson || ischpl then rwLiteral(", ");

      if f.writing && ischpl {
        f.write(key);
        f._writeLiteral(" => ");
      }

      if f.writing then f.write(arr.dsiAccess(key));
      else arr.dsiAccess(key) = f.read(arr.eltType);
    }

    if isjson || ischpl then rwLiteral("]");
  }
}
