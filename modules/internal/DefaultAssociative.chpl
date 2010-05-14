use Sort /* only QuickSort */;

// TODO: make the domain parameterized by this?
// TODO: make int(64) the default index type here and in arithemtic domains
type chpl_table_index_type = int(32);


/* These declarations could/should both be nested within
   DefaultAssociativeDom? */
enum chpl__hash_status { empty, full, deleted };

record chpl_TableEntry {
  type idxType;
  var status: chpl__hash_status = chpl__hash_status.empty;
  var idx: idxType;
}

def chpl__primes return (23, 53, 97, 193, 389, 769, 1543,
                         3079, 6151, 12289, 24593, 49157, 98317, 196613,
                         393241, 786433, 1572869, 3145739, 6291469, 12582917, 25165843,
                         50331653, 100663319, 201326611, 402653189, 805306457, 1610612741);

class DefaultAssociativeDom: BaseAssociativeDom {
  type idxType;

  var dist: DefaultDist;

  // The guts of the associative domain
  var numEntries: chpl_table_index_type = 0;
  var tableSizeNum = 1;
  var tableSize = chpl__primes(tableSizeNum);
  var tableDom = [0..tableSize-1];
  var table: [tableDom] chpl_TableEntry(idxType);


  // Temporary arrays for side computations:
  // TODO: Slosh back and forth between two tables rather than copying back
  // TODO: This ugly [0..-1] domain appears several times in the code --
  //       replace with a named constant/param?
  var tmpDom = [0..-1:chpl_table_index_type];
  var tmpTable: [tmpDom] chpl_TableEntry(idxType);
  var tmpDom2 = [0..-1:chpl_table_index_type];
  var tmpTable2: [tmpDom2] idxType;
  var postponeResize = false;

  def linksDistribution() param return false;

  def DefaultAssociativeDom(type idxType, dist: DefaultDist) {
    this.dist = dist;
  }

  //
  // Standard Internal Domain Interface
  //
  def dsiBuildArray(type eltType) {
    return new DefaultAssociativeArr(eltType=eltType, idxType=idxType,
                                     dom=this); 
  }

  def dsiSerialWrite(f: Writer) {
    var first = true;
    f.write("[");
    for idx in this {
      if first then 
        first = false; 
      else 
        f.write(", ");
      f.write(idx);
    }
    f.write("]");
  }

  //
  // Standard user domain interface
  //

  def dsiNumIndices {
    return numEntries;
  }

  def dsiIndsIterSafeForRemoving() {
    postponeResize = true;
    for i in this.these() do
      yield i;
    postponeResize = false;
    if (numEntries*8 < tableSize && tableSizeNum > 1) {
      _resize(grow=false);
    }
  }

  def these() {
    if !_isEnumeratedType(idxType) {
      for slot in _fullSlots() {
        yield table(slot).idx;
      }
    } else {
      for val in chpl_enumerate(idxType) {
        var (match, slot) = _findFilledSlot(val);
        if match then
          yield table(slot).idx;
      }
    }
  }

  def these(param tag: iterator) where tag == iterator.leader {
    compilerWarning("parallel iteration over associative domain has been serialized (see note in $CHPL_HOME/STATUS)");
    yield 0..dsiNumIndices-1;
  }

  def these(param tag: iterator, follower) where tag == iterator.follower {
    for (slot,i) in (_fullSlots(),0..) {
      if follower.member(i) then
        yield table(slot).idx;
    }
  }

  //
  // Associative Domain Interface
  //
  def dsiClear() {
    for slot in tableDom {
      table(slot).status = chpl__hash_status.empty;
    }
    numEntries = 0;
  }

  def dsiMember(idx: idxType): bool {
    return _findFilledSlot(idx)(1);
  }

  pragma "inline" 
  def dsiAdd(idx: idxType): index(tableDom) {
    if ((numEntries+1)*2 > tableSize) {
      // TODO: should also make sure that we don't exceed the maximum table
      // size
      if atomicSupport then
	_resize(grow=true);
      else
	halt("manually increase size of table");
    }
    const (foundSlot, slotNum) = _findEmptySlot(idx);
    if (foundSlot) {
      table(slotNum).status = chpl__hash_status.full;
      table(slotNum).idx = idx;
      numEntries += 1;
    } else {
      if (slotNum < 0) {
        halt("couldn't add ", idx, " -- ", numEntries, " / ", tableSize, " taken");
        return -1;
      }
      // otherwise, re-adding an index that's already in there
    }
    return slotNum;
  }

  def dsiRemove(idx: idxType) {
    const (foundSlot, slotNum) = _findFilledSlot(idx);
    if (foundSlot) {
      for a in _arrs do
        a.clearEntry(idx);
      table(slotNum).status = chpl__hash_status.deleted;
      numEntries -= 1;
    } else {
      halt("index not in domain: ", idx);
    }
    if (numEntries*8 < tableSize && tableSizeNum > 1) {
      if atomicSupport then
	_resize(grow=false);
      else
	halt("manually increase size of table");
    }
  }

  def dsiSorted() {
    _createSortedTmpTable2();
    for ind in tmpTable2 do
      yield ind;
    _destroySortedTmpTable2();
  }

  //
  // Internal interface (private)
  //
  def _createSortedTmpTable2() {
    // TODO: should assert that tmpDom2 is non-empty to avoid conflicting
    // calls? Or not -- to support early exit from iterator?
    tmpDom2 = [0..numEntries-1];
    var count: chpl_table_index_type = 0;
    for slot in _fullSlots() {
      tmpTable2(count) = table(slot).idx;
      count += 1;
    }
    QuickSort(tmpTable2);
  }

  def _destroySortedTmpTable2() {
    tmpDom2 = [0..-1:chpl_table_index_type];
  }

  def _resize(grow:bool) {
    if postponeResize then return;
    // back up the arrays
    _backupArrays();

    // copy the table (TODO: could use swap between two versions)
    tmpDom = tableDom;
    tmpTable = table;

    // grow original table
    tableDom = [0..-1:chpl_table_index_type]; // non-preserving resize
    numEntries = 0; // reset, because the adds below will re-set this
    tableSizeNum += if grow then 1 else -1;
    tableSize = chpl__primes(tableSizeNum);
    tableDom = [0..tableSize-1];

    // insert old data into newly resized table
    for slot in _fullSlots(tmpTable) {
      const newslot = dsiAdd(tmpTable(slot).idx);
      _preserveArrayElement(oldslot=slot, newslot=newslot);
    }
    
    // deallocate tmpTable
    tmpDom = [0..-1:chpl_table_index_type];
    _removeArrayBackups();
  }

  def _findFilledSlot(idx: idxType, tab = table): (bool, index(tableDom)) {
    for slotNum in _lookForSlots(idx, tab.domain.high+1) {
      const slotStatus = tab(slotNum).status;
      if (slotStatus == chpl__hash_status.empty) {
        return (false, -1);
      } else if (slotStatus == chpl__hash_status.full) {
        if (tab(slotNum).idx == idx) {
          return (true, slotNum);
        }
      }
    }
    return (false, -1);
  }

  def _findEmptySlot(idx: idxType): (bool, index(tableDom)) {
    for slotNum in _lookForSlots(idx) {
      const slotStatus = table(slotNum).status;
      if (slotStatus == chpl__hash_status.empty ||
          slotStatus == chpl__hash_status.deleted) {
        return (true, slotNum);
      } else if (table(slotNum).idx == idx) {
        return (false, slotNum);
      }
    }
    return (false, -1);
  }
    
  def _lookForSlots(idx: idxType, numSlots = tableSize) {
    const baseSlot = chpl__defaultHashWrapper(idx);
    for probe in 0..numSlots/2 {
      yield (baseSlot + probe**2)%numSlots;
    }
  }

  def _fullSlots(tab = table) {
    for slot in tab.domain {
      if tab(slot).status == chpl__hash_status.full then
        yield slot;
    }
  }
}


class DefaultAssociativeArr: BaseArr {
  type eltType;
  type idxType;
  var dom : DefaultAssociativeDom(idxType);

  var data : [dom.tableDom] eltType;

  var tmpDom = [0..-1:chpl_table_index_type];
  var tmpTable: [tmpDom] eltType;

  //
  // Standard internal array interface
  // 

  def dsiGetBaseDom() return dom;

  def clearEntry(idx: idxType) {
    const initval: eltType;
    dsiAccess(idx) = initval;
  }

  def dsiAccess(idx : idxType) var : eltType {
    const (found, slotNum) = dom._findFilledSlot(idx);
    if (found) then
      return data(slotNum);
    else {
      halt("array index out of bounds: ", idx);
      return data(0);
    }
  }

  def these() var {
    for slot in dom {
      yield dsiAccess(slot);
    }
  }

  def these(param tag: iterator) where tag == iterator.leader {
    compilerWarning("parallel iteration over associative array has been serialized (see note in $CHPL_HOME/STATUS)");
    for block in dom.these(tag=iterator.leader) do
      yield block;
  }

  def these(param tag: iterator, follower) var where tag == iterator.follower {
    for i in dom.these(tag=iterator.follower, follower) do
      yield this(i);
  }

  def dsiSerialWrite(f: Writer) {
    var first = true;
    for val in this {
      if (first) then
        first = false;
      else
        f.write(" ");
      f.write(val);
    }
  }


  //
  // Associative array interface
  //

  def dsiSorted() {
    _createSortedTmpTable();
    for elem in tmpTable do
      yield elem;
    _destroySortedTmpTable();
  }


  //
  // Internal associative array interface
  //

  def _backupArray() {
    tmpDom = dom.tableDom;
    tmpTable = data;
  }

  def _removeArrayBackup() {
    tmpDom = [0..-1:chpl_table_index_type];
  }

  def _preserveArrayElement(oldslot, newslot) {
    data(newslot) = tmpTable(oldslot);
  }

  // private internal interface

  def _createSortedTmpTable() {
    tmpDom = [0..dom.dsiNumIndices-1];
    var count: chpl_table_index_type = 0;
    for slot in dom._fullSlots() {
      tmpTable(count) = data(slot);
      count += 1;
    }
    QuickSort(tmpTable);
  }

  def _destroySortedTmpTable() {
    tmpDom = [0..-1:chpl_table_index_type];
  }
}


def chpl__defaultHashWrapper(x): chpl_table_index_type {
  const hash = chpl__defaultHash(x); 
  return (hash & max(chpl_table_index_type)): chpl_table_index_type;
}


// Thomas Wang's 64b mix function from http://www.concentric.net/~Ttwang/tech/inthash.htm
def _gen_key(i: int(64)): int(64) {
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

pragma "inline"
def chpl__defaultHash(b: bool): int(64) {
  if (b) then
    return 0;
  else
    return 1;
}

pragma "inline"
def chpl__defaultHash(i: int(64)): int(64) {
  return _gen_key(i);
}

pragma "inline"
def chpl__defaultHash(u: uint(64)): int(64) {
  return _gen_key(u:int(64));
}

pragma "inline"
def chpl__defaultHash(f: real): int(64) {
  return _gen_key(__primitive( "real2int", f));
}

pragma "inline"
def chpl__defaultHash(c: complex): int(64) {
  return _gen_key(__primitive("real2int", c.re) ^ __primitive("real2int", c.im)); 
}

pragma "inline"
def chpl__defaultHash(u: chpl_taskID_t): int(64) {
  return _gen_key(u:int(64));
}

// Use djb2 (Dan Bernstein in comp.lang.c.
pragma "inline"
def chpl__defaultHash(x : string): int(64) {
  var hash: int(64) = 0;
  for c in 1..length(x) {
    hash = ((hash << 5) + hash) ^ ascii(x.substring(c));
  }
  return _gen_key(hash);
}

pragma "inline"
def chpl__defaultHash(o: object): int(64) {
  return _gen_key(__primitive( "object2int", o));
}
